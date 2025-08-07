/*
 * dfp_mini.c
 *
 *  Created on: Jul 20, 2025
 *      Author: kccistc
 */

#include "dfp_mini.h"

#define FRAME_LEN      10
#define START_BYTE     0x7E
#define VERSION_BYTE   0xFF
#define LENGTH_BYTE    0x06
#define NO_FEEDBACK    0x00
#define END_BYTE       0xEF
#define DFP_CMD_PLAY_TRACK 0x03
#define DFP_CMD_PLAY_FOLDER 0x0F
#define DFP_CMD_STOP 0x16

static uint8_t frame[FRAME_LEN];

static uint16_t make_checksum(void) {
    uint16_t sum = 0;
    for (int i = 1; i <= 6; i++) sum += frame[i];
    // 바이트 1 ~ 6의 합 = 체크섬
    return 0xFFFF - sum + 1;
}

static void send_cmd(uint8_t cmd, uint16_t param) {
    frame[0] = START_BYTE; 		// 프레임 시작 표시 (고정)
    frame[1] = VERSION_BYTE;	// 버전 정보 (고정)
    frame[2] = LENGTH_BYTE;		// frame1 ~ frame6까지 실제 데이터 길이 (6바이트) (고정)
    frame[3] = cmd;				// 명령어 코드
    frame[4] = NO_FEEDBACK;		// 응답 피드백 필요 없으니까 0x00, 응답 요청 필요하면 0x01 (현재는 고정)
    frame[5] = (param >> 8) & 0xFF;	// 파라미터 상위 8비트
    frame[6] = param & 0xFF;		// 파라미터 하위 8비트

    uint16_t chk = make_checksum();
    frame[7] = (chk >> 8) & 0xFF;	//체크섬 상위 8비트
    frame[8] = chk & 0xFF;			// 체크섬 하위 8비트
    frame[9] = END_BYTE;			// 프레임 종료 표시(고정)


//    if (cmd != DFP_CMD_PLAY_TRACK && cmd != DFP_CMD_PLAY_FOLDER){
//    	while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == GPIO_PIN_RESET) {
//    		osDelay(1);
//    	}
//    }
    // 한번에 10바이트 전송
//    HAL_UART_Transmit_IT(&huart1, frame, FRAME_LEN);

    HAL_UART_Transmit(&huart1, frame, FRAME_LEN, 20);
    osDelay(10);
}

//void dfp_send(uint8_t cmd, uint16_t param, bool wait_busy) {
//    frame[0] = START_BYTE; 		// 프레임 시작 표시 (고정)
//    frame[1] = VERSION_BYTE;	// 버전 정보 (고정)
//    frame[2] = LENGTH_BYTE;		// frame1 ~ frame6까지 실제 데이터 길이 (6바이트) (고정)
//    frame[3] = cmd;				// 명령어 코드
//    frame[4] = NO_FEEDBACK;		// 응답 피드백 필요 없으니까 0x00, 응답 요청 필요하면 0x01 (현재는 고정)
//    frame[5] = (param >> 8) & 0xFF;	// 파라미터 상위 8비트
//    frame[6] = param & 0xFF;		// 파라미터 하위 8비트
//
//    uint16_t chk = make_checksum();
//    frame[7] = (chk >> 8) & 0xFF;	//체크섬 상위 8비트
//    frame[8] = chk & 0xFF;			// 체크섬 하위 8비트
//    frame[9] = END_BYTE;			// 프레임 종료 표시(고정)
//
//
////    if (cmd != DFP_CMD_PLAY_TRACK && cmd != DFP_CMD_PLAY_FOLDER){
////    	while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == GPIO_PIN_RESET) {
////    		osDelay(1);
////    	}
////    }
//    // 한번에 10바이트 전송
////    HAL_UART_Transmit_IT(&huart1, frame, FRAME_LEN);
//
//    HAL_UART_Transmit(&huart1, frame, FRAME_LEN, HAL_MAX_DELAY);
//    if (wait_busy) {
//		while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == GPIO_PIN_RESET){
//			osDelay(10);
//		}
//	}
//}


void dfp_init(uint8_t vol) {
    // 전원 안정화 대기 (최소 500ms 권장)
    osDelay(500);
    // 리셋 명령 보내기 (모듈 내부 초기화)
    send_cmd(0x0C, 0); // -> 볼륨 30
    osDelay(1000);

//    send_cmd(0x16, 0);   // STOP
//    osDelay(200);
    // 볼륨 초기화 (volume 0~30) -> 리셋하면 초기 볼륨이 최대 30인 상태라 무조건 볼륨 설정해줘야 함
    dfp_setVolume(vol);
    osDelay(200);
}

//void dfp_init_second(uint8_t volume) {
//    // 전원 안정화 대기 (최소 500ms 권장)
//    osDelay(500);
//    dfp_send(0x0C, 0, true);
//    dfp_send(0x06, volume, false);
//}

void dfp_playTrack(uint16_t track) {	// 트랙번호 넣으면 해당 순서의 mp3 재생됨
    // track: 1~3000 (001.mp3 → track=1)
    send_cmd(0x03, track);
}

void dfp_playFolder(uint8_t folder, uint8_t track) {
	uint16_t param = ((uint16_t)folder << 8) | track;
	send_cmd(DFP_CMD_PLAY_FOLDER, param);
}

void dfp_setVolume(uint8_t vol) { 	//최대 30
    if (vol > 30) vol = 30;
    send_cmd(0x06, vol);
}

void dfp_pause()	// 일시정지
{
	send_cmd(0x0e, 0);
	osDelay(30);
}

void dfp_playback() // 일시정지 한 곳부터 다시 재생
{
	send_cmd(0x0D,0);
	osDelay(30);
}

void dfp_stop()	// 정지 곡의 처음으로 돌아감
{
	send_cmd(0x16, 0);
	osDelay(30);
}

void dfp_volumeup()
{
	send_cmd(0x04, 0);
	osDelay(30);
}

void dfp_volumedown()
{
	send_cmd(0x05, 0);
	osDelay(30);
}
