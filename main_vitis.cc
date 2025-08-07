#include "xparameters.h"

#include "platform/platform.h"
#include "ov5640/OV5640.h"
#include "ov5640/ScuGicInterruptController.h"
#include "ov5640/PS_GPIO.h"
#include "ov5640/AXI_VDMA.h"
#include "ov5640/PS_IIC.h"

#include "MIPI_D_PHY_RX.h"
#include "MIPI_CSI_2_RX.h"

/*
#include <stdint.h>
#include <string.h>
#include "xil_printf.h"
#include "xuartlite.h"
#include "xparameters.h"
#include "sleep.h"
 */
#include "xil_io.h"
#include "xparameters.h"
#include "xuartps.h"
#include "xuartlite.h"
#include "xil_printf.h"
#include <string.h>
#include "sleep.h"
#include "xgpiops.h"
#include "xgpio.h"					// axi gpio (+)
#include "xaxidma.h"
#include "xil_cache.h"

// CNN IP base address
#define CNN_BASEADDR 0x43c50000U
typedef struct {
	volatile uint32_t alpha;
	volatile uint32_t out_valid;
	volatile uint32_t pcam_data;
} CNN_TypeDef;
#define CNN ((CNN_TypeDef *)CNN_BASEADDR)

// UART 설정
#define UART_DEVICE_ID XPAR_AXI_UARTLITE_0_DEVICE_ID
XUartLite Uart;

#define IRPT_CTL_DEVID 		XPAR_PS7_SCUGIC_0_DEVICE_ID
#define GPIO_DEVID			XPAR_PS7_GPIO_0_DEVICE_ID
#define GPIO_IRPT_ID			XPAR_PS7_GPIO_0_INTR
#define CAM_I2C_DEVID		XPAR_PS7_I2C_0_DEVICE_ID
#define CAM_I2C_IRPT_ID		XPAR_PS7_I2C_0_INTR
#define VDMA_DEVID			XPAR_AXIVDMA_0_DEVICE_ID
#define VDMA_MM2S_IRPT_ID	XPAR_FABRIC_AXI_VDMA_0_MM2S_INTROUT_INTR
#define VDMA_S2MM_IRPT_ID	XPAR_FABRIC_AXI_VDMA_0_S2MM_INTROUT_INTR
#define CAM_I2C_SCLK_RATE	100000

#define DDR_BASE_ADDR		XPAR_DDR_MEM_BASEADDR
#define MEM_BASE_ADDR		(DDR_BASE_ADDR + 0x0A000000)

#define GAMMA_BASE_ADDR     XPAR_AXI_GAMMACORRECTION_0_BASEADDR

#define IMG_BASE_ADDR		(DDR_BASE_ADDR + 0x01000000)					//
#define IMG_BYTE_COUNT  	(28*28*4)										//
//////////////////////////////////////////////////////////////////
#define NUM_FSTORES   XPAR_AXIVDMA_0_NUM_FSTORES  // = 3
#define FRAME_WIDTH 		1280
#define FRAME_HEIGHT 		720
#define FRAME_PIXEL 		(FRAME_WIDTH * FRAME_HEIGHT)
#define BYTES_PER_PIXEL		3
#define FRAME_BYTE 			(FRAME_PIXEL * BYTES_PER_PIXEL)
#define SAMPLE_PIXELS 		140
#define RED_R           	0xFF
#define RED_G           	0x00
#define RED_B           	0x00
#define SAMPLE_W 140
#define SAMPLE_H 140
#define DOWNSAMPLED_W 28
#define DOWNSAMPLED_H 28
#define RGB_BYTES 3
#define FACTOR  5

#define BTN_DEVICE_ID XPAR_AXI_GPIO_0_DEVICE_ID

#define NOP() __asm__ volatile("nop")
///////////////////////////////////////////////////////////////////
uint8_t* frame_buf_data = (uint8_t *) MEM_BASE_ADDR;

static uint8_t src_140x140[SAMPLE_W * SAMPLE_H * RGB_BYTES];
static uint8_t dst_28x28[DOWNSAMPLED_W * DOWNSAMPLED_H * RGB_BYTES];
static uint32_t dst_28x28_padded[DOWNSAMPLED_W * DOWNSAMPLED_H * 4];

static XAxiDma AxiDma;

static uint8_t frame_data[FRAME_BYTE];
///////////////////////////////////////////////////////////////////

using namespace digilent;

// 반복문 기반 지연 (CPU 사이클 수는 튜닝 필요)
static void delay_20ns_approx(void) {
	for (int i = 0; i < 14; i++) {
		NOP();
	}
}

void pipeline_mode_change(AXI_VDMA<ScuGicInterruptController>& vdma_driver, OV5640& cam, VideoOutput& vid, Resolution res, OV5640_cfg::mode_t mode)
{
	//Bring up input pipeline back-to-front
	{
		vdma_driver.resetWrite();
		MIPI_CSI_2_RX_mWriteReg(XPAR_MIPI_CSI_2_RX_0_S_AXI_LITE_BASEADDR, CR_OFFSET, (CR_RESET_MASK & ~CR_ENABLE_MASK));
		MIPI_D_PHY_RX_mWriteReg(XPAR_MIPI_D_PHY_RX_0_S_AXI_LITE_BASEADDR, CR_OFFSET, (CR_RESET_MASK & ~CR_ENABLE_MASK));
		cam.reset();
	}

	{
		vdma_driver.configureWrite(timing[static_cast<int>(res)].h_active, timing[static_cast<int>(res)].v_active);
		Xil_Out32(GAMMA_BASE_ADDR, 3); // Set Gamma correction factor to 1/1.8
		//TODO CSI-2, D-PHY config here
		cam.init();
	}

	{
		vdma_driver.enableWrite();
		MIPI_CSI_2_RX_mWriteReg(XPAR_MIPI_CSI_2_RX_0_S_AXI_LITE_BASEADDR, CR_OFFSET, CR_ENABLE_MASK);
		MIPI_D_PHY_RX_mWriteReg(XPAR_MIPI_D_PHY_RX_0_S_AXI_LITE_BASEADDR, CR_OFFSET, CR_ENABLE_MASK);
		cam.set_mode(mode);
		cam.set_awb(OV5640_cfg::awb_t::AWB_ADVANCED);
	}

	//Bring up output pipeline back-to-front
	{
		vid.reset();
		vdma_driver.resetRead();
	}

	{
		vid.configure(res);
		vdma_driver.configureRead(timing[static_cast<int>(res)].h_active, timing[static_cast<int>(res)].v_active);
	}

	{
		vid.enable();
		vdma_driver.enableRead();
	}
}

void downsample_avg5(const uint8_t *src, uint8_t *dst) {
	for (int y = 0; y < DOWNSAMPLED_H; y++) {           //
		for (int x = 0; x < DOWNSAMPLED_W; x++) {       //
			uint32_t sum[3] = {0,0,0};          // R, G, B

			//
			for (int dy = 0; dy < FACTOR; dy++) {
				for (int dx = 0; dx < FACTOR; dx++) {
					int sx = x*FACTOR + dx;     //
					int sy = y*FACTOR + dy;     //
					//
					const uint8_t *p = src + (sy*SAMPLE_W + sx)*RGB_BYTES;
					sum[0] += p[0];             // Blue
					sum[1] += p[1];             // Green
					sum[2] += p[2];             // Red
				}
			}

			//
			uint8_t *q = dst + (y*DOWNSAMPLED_W + x)*RGB_BYTES;
			q[0] = sum[0] / (FACTOR*FACTOR);
			q[1] = sum[1] / (FACTOR*FACTOR);
			q[2] = sum[2] / (FACTOR*FACTOR);
		}
	}
}

void capture_one_frame() {
	memcpy(frame_data, frame_buf_data, FRAME_BYTE);
	xil_printf("captured one frame : %d bytes at %p -> %p\n",
			FRAME_BYTE, frame_buf_data, frame_data);

	// (x,y) = (570,290)     140x140
	for (int y = 0; y < SAMPLE_H; y++) {
		for (int x = 0; x < SAMPLE_W; x++) {
			int src_x = 570 + x;
			int src_y = 290 + y;
			size_t base = (size_t)src_y * FRAME_WIDTH * RGB_BYTES + src_x * RGB_BYTES;

			// RGB
			src_140x140[(y * SAMPLE_W + x) * 3 + 0] = frame_data[base + 0]; // B
			src_140x140[(y * SAMPLE_W + x) * 3 + 1] = frame_data[base + 1]; // G
			src_140x140[(y * SAMPLE_W + x) * 3 + 2] = frame_data[base + 2]; // R
		}
	}

	// 140x140    28x28
	downsample_avg5(src_140x140, dst_28x28);

	//
	for (int i = 0; i < DOWNSAMPLED_W * DOWNSAMPLED_H; i++) {
		uint8_t b = dst_28x28[i * 3 + 0];
		uint8_t g = dst_28x28[i * 3 + 1];
		uint8_t r = dst_28x28[i * 3 + 2];
		//		xil_printf("%02x%02x%02x ", b, g, r);
		//xil_printf("dst_28x28 @ %p\n", dst_28x28);
		dst_28x28_padded[i] =  (0u       << 24)
                                				| (b   << 16)
												| (g   <<  8)
												| (r   <<  0);
	}
	xil_printf("\r\n");
	xil_printf("dst_28x28_padded : %p \r\n", dst_28x28_padded);

	for (int i = 0; i < DOWNSAMPLED_W * DOWNSAMPLED_H; i++) {
		uint32_t w = dst_28x28_padded[i];
		uint8_t zero = (w >> 24) & 0xFF;
		uint8_t b_1  = (w >> 16) & 0xFF;
		uint8_t g_1  = (w >>  8) & 0xFF;
		uint8_t r_1  = (w >>  0) & 0xFF;
		xil_printf("%02x%02x%02x%02x ", zero, b_1, g_1, r_1);
	}
}



int init_dma() {
	XAxiDma_Config *cfg = XAxiDma_LookupConfig(XPAR_AXI_DMA_0_DEVICE_ID);
	if (!cfg) return XST_FAILURE;
	if (XAxiDma_CfgInitialize(&AxiDma, cfg) != XST_SUCCESS) return XST_FAILURE;

	XAxiDma_IntrDisable(&AxiDma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);
	return XST_SUCCESS;
}

uint32_t start_transfer() {
	memcpy((void *)IMG_BASE_ADDR, dst_28x28_padded, IMG_BYTE_COUNT);

	Xil_DCacheFlushRange(IMG_BASE_ADDR, IMG_BYTE_COUNT);


	return XAxiDma_SimpleTransfer(
			&AxiDma,
			(UINTPTR)IMG_BASE_ADDR,
			IMG_BYTE_COUNT,
			XAXIDMA_DMA_TO_DEVICE
	);
}






static inline uint8_t* slot_addr(int i) {
	return frame_buf_data + (size_t)i * FRAME_BYTE;
}

void draw_red_box_once(uint8_t* buf) {
	int x0 = ((FRAME_WIDTH  - SAMPLE_PIXELS) / 2) - 1;	//569
	int y0 = ((FRAME_HEIGHT - SAMPLE_PIXELS) / 2) - 1;	//289
	int x1 = x0 + SAMPLE_PIXELS ;				//710
	int y1 = y0 + SAMPLE_PIXELS ;				//430

	for (int x = x0; x <= x1; x++) {
		size_t idx_top    = ((size_t)y0 * FRAME_WIDTH + x) * BYTES_PER_PIXEL;
		size_t idx_bottom = ((size_t)y1 * FRAME_WIDTH + x) * BYTES_PER_PIXEL;
		buf[idx_top + 0]    = RED_B;
		buf[idx_top + 1]    = RED_B;
		buf[idx_top + 2]    = RED_R;
		buf[idx_bottom + 0] = RED_B;
		buf[idx_bottom + 1] = RED_B;
		buf[idx_bottom + 2] = RED_R;
	}
	for (int y = y0; y <= y1; y++) {
		size_t idx_left  = ((size_t)y * FRAME_WIDTH + x0) * BYTES_PER_PIXEL;
		size_t idx_right = ((size_t)y * FRAME_WIDTH + x1) * BYTES_PER_PIXEL;
		buf[idx_left  + 0] = RED_B;
		buf[idx_left  + 1] = RED_B;
		buf[idx_left  + 2] = RED_R;
		buf[idx_right + 0] = RED_B;
		buf[idx_right + 1] = RED_B;
		buf[idx_right + 2] = RED_R;
	}
}

void draw_red_box_all_slots() {
	for (int i = 0; i < 3; i++) {
		uint8_t* buf = slot_addr(i);

		Xil_DCacheInvalidateRange((INTPTR)buf, FRAME_BYTE);
		//
		draw_red_box_once(buf);
		//
		Xil_DCacheFlushRange((INTPTR)buf, FRAME_BYTE);
	}
}

int main()
{
	init_platform();
	init_dma();	//

	int status = XUartLite_Initialize(&Uart, UART_DEVICE_ID);
	if (status != XST_SUCCESS) {
		xil_printf("UART INIT FAILED\n");
		return XST_FAILURE;
	}

	uint8_t uart_result = 0;

	ScuGicInterruptController irpt_ctl(IRPT_CTL_DEVID);
	PS_GPIO<ScuGicInterruptController> gpio_driver(GPIO_DEVID, irpt_ctl, GPIO_IRPT_ID);
	PS_IIC<ScuGicInterruptController> iic_driver(CAM_I2C_DEVID, irpt_ctl, CAM_I2C_IRPT_ID, 100000);

	OV5640 cam(iic_driver, gpio_driver);
	AXI_VDMA<ScuGicInterruptController> vdma_driver(VDMA_DEVID, MEM_BASE_ADDR, irpt_ctl,
			VDMA_MM2S_IRPT_ID,
			VDMA_S2MM_IRPT_ID);
	VideoOutput vid(XPAR_VTC_0_DEVICE_ID, XPAR_VIDEO_DYNCLK_DEVICE_ID);

	//	pipeline_mode_change(vdma_driver, cam, vid, Resolution::R1920_1080_60_PP, OV5640_cfg::mode_t::MODE_1080P_1920_1080_30fps);
	pipeline_mode_change(vdma_driver, cam, vid, Resolution::R1280_720_60_PP, OV5640_cfg::mode_t::MODE_720P_1280_720_60fps);	//

	xil_printf("Video init done.\r\n");
	//////////////////////////////////////////////////////////////
	XGpio btnGpio;
	int statuss;

	u32 btnState = 0;
	u32 prevBtnState = 0;

	// AXI GPIO
	statuss = XGpio_Initialize(&btnGpio, BTN_DEVICE_ID);
	if (statuss != XST_SUCCESS) {
		xil_printf("AXI GPIO Init Failed\r\n");
		return XST_FAILURE;
	}

	XGpio_SetDataDirection(&btnGpio, 1, 0xFF);

	//---------------btn
	//---------------btn
	//---------------btn
	//---------------btn
	////////////////////////////////////////////////////////////////
	while (1) {

		btnState = XGpio_DiscreteRead(&btnGpio, 1);
		//	xil_printf("%x \n", 0x00112210);
		if ((btnState & 0x1) && !(prevBtnState & 0x1)) {
			xil_printf("button pressed -> capture frame\n");
			capture_one_frame();
			//			start_transfer();
			int ret = start_transfer();

			//			int ret = XAxiDma_SimpleTransfer(&AxiDma, (UINTPTR)IMG_BASE_ADDR, IMG_BYTE_COUNT, XAXIDMA_DMA_TO_DEVICE);
			//			xil_printf("SimpleTransfer returned %s\n", ret == XST_SUCCESS ? "OK":"FAIL");
			xil_printf("SimpleTransfer returned %d\n", ret);

			while (XAxiDma_Busy(&AxiDma, XAXIDMA_DMA_TO_DEVICE));

			uart_result = CNN->alpha;
			XUartLite_Send(&Uart, &uart_result, 1);
			xil_printf("CNN alpha = %c\n", uart_result);
			xil_printf("%x \n", CNN->pcam_data);
			usleep(500000); // 100ms 간격 전송

		}
//		if (CNN->out_valid & 0x01){
//			uart_result = CNN->alpha;
//			XUartLite_Send(&Uart, &uart_result, 1);
//			xil_printf("CNN alpha = 0x%02X\n", uart_result);
//			xil_printf("%x \n", CNN->pcam_data);
//			usleep(500000); // 100ms 간격 전송
//		}

		prevBtnState = btnState;
		draw_red_box_all_slots();

	}


	cleanup_platform();

	return 0;
}
