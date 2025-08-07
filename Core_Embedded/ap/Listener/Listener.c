/*
 * Listener.c
 *
 *  Created on: Jul 16, 2025
 *      Author: kccistc
 */


#include "Listener.h"

Button_Handler_t hbtnMode;
Button_Handler_t hbtnStart;
Button_Handler_t hbtnVolumeUp;
Button_Handler_t hbtnVolumeDown;

uint8_t mode= S_RF_MODE;
uint8_t cnn_state = S_CNN_STOP;
uint8_t rf_state = S_RX_MODE;

volatile uint8_t tx_done_flag;
uint8_t tx_start;

void ussart();

void Listener_Init()
{
	Button_Init(&hbtnStart, GPIOB, GPIO_PIN_2);
	Button_Init(&hbtnMode, GPIOC, GPIO_PIN_4);
	Button_Init(&hbtnVolumeUp, GPIOB, GPIO_PIN_8);
	Button_Init(&hbtnVolumeDown, GPIOB, GPIO_PIN_9);
	Model_Listener_Controller_Init();
}

void Listener_Execute()
{
    char msg[64];
	switch(mode)
	{
		case S_CNN_MODE:
			switch(cnn_state)
			{
				case S_CNN_STOP:
//					if (Button_GetState(&hbtnVolumeUp) == ACT_RELEASED){
//						dfp_volumeup();
//					} else if( Button_GetState(&hbtnVolumeDown) == ACT_RELEASED){
//						dfp_volumedown();
//					}
					if (Button_GetState(&hbtnMode) == ACT_RELEASED){
						mode = S_RF_MODE;
					    snprintf(msg, sizeof(msg), "Mode: %s \r\n", "rf mode");
					    HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), 1000);
					}
					else {
						Listener_CNNStop();
						if (Button_GetState(&hbtnStart) == ACT_RELEASED) cnn_state = S_CNN_START;
					}
					break;
				case S_CNN_START:
					if (Button_GetState(&hbtnMode) == ACT_RELEASED){
						mode = S_RF_MODE;
					    snprintf(msg, sizeof(msg), "Mode: %s ", "rf mode");
					    HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), 1000);
					}
					else {
						Listener_CNNExecute();
						cnn_state = S_CNN_STOP;
					}
					break;
			}
			break;

		case S_RF_MODE:
			switch(rf_state)
			{
				case S_RX_MODE:
//					if (Button_GetState(&hbtnVolumeUp) == ACT_RELEASED){
//						dfp_volumeup();
//					} else if( Button_GetState(&hbtnVolumeDown) == ACT_RELEASED){
//						dfp_volumedown();
//					}
					if (Button_GetState(&hbtnMode) == ACT_RELEASED){
						mode = S_CNN_MODE;
					    snprintf(msg, sizeof(msg), "Mode: %s \r\n", "cnn mode");
					    HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), 1000);
					}
					else {
						Listener_Switch_RxExecute();
						if (Button_GetState(&hbtnStart) == ACT_RELEASED) {
							nrf24l01p_tx_init(2500, _1Mbps);
					        rf_state = S_TX_MODE;
					        tx_done_flag = 2;
					        tx_start = 1;
						}
					}
					break;
				case S_TX_MODE:
					if (Button_GetState(&hbtnMode) == ACT_RELEASED){
						mode = S_CNN_MODE;
					    snprintf(msg, sizeof(msg), "Mode: %s ", "cnn mode");
					    HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), 1000);
					}
					else if (tx_done_flag==2 && tx_start) { // 송신 모드
						Listener_Switch_TxExecute();  // 🔸 전송 실행
						tx_start = 0;
					    HAL_TIM_Base_Start_IT(&htim4);
					}
					else if (tx_done_flag==1) { // 수신 모드로 변경
					    HAL_TIM_Base_Stop_IT(&htim4);  // 타이머 인터럽트 정지 (1회만 사용)
						nrf24l01p_rx_init(2500, _1Mbps);
						rf_state = S_RX_MODE;
					}
					break;
			}
			break;
	}

	if (Button_GetState(&hbtnVolumeUp) == ACT_RELEASED){
		dfp_volumeup();
	} else if( Button_GetState(&hbtnVolumeDown) == ACT_RELEASED){
		dfp_volumedown();
	}


//	ussart();

}


uint8_t RxByte;

void ussart()
{
	if ( HAL_UART_Receive(&huart6, &RxByte, sizeof(RxByte), 1000) == HAL_OK)
	{
		HAL_UART_Transmit(&huart2, &RxByte, sizeof(RxByte), 1000);
	}
}
