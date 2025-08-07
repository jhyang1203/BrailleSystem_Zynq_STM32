/*
 * dfp_mini.h
 *
 *  Created on: Jul 20, 2025
 *      Author: kccistc
 */

#ifndef DRIVER_DFPLAYERMINI_DFP_MINI_H_
#define DRIVER_DFPLAYERMINI_DFP_MINI_H_

#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "usart.h"

#define BUSY_PORT    GPIOC
#define BUSY_PIN     GPIO_PIN_4

#define LED_PORT     GPIOA
#define LED_PIN      GPIO_PIN_5

// UART 핸들, CubeMX에서 MX_USART1_UART_Init() 등으로 초기화된 녀석을 사용
extern UART_HandleTypeDef huart1;



// 모듈 초기화 (전원 안정화 딜레이 등)
void dfp_init(uint8_t vol);
// 트랙 번호로 재생 (1부터 시작, SD 루트에 001.mp3, 002.mp3… 식으로 저장)
void dfp_playTrack(uint16_t track);
void dfp_playFolder(uint8_t folder, uint8_t track);
// (옵션) 볼륨 설정 0~30
void dfp_setVolume(uint8_t vol);
void dfp_stop();
void dfp_pause();
void dfp_playback();
void dfp_volumeup();
void dfp_volumedown();

#endif /* DRIVER_DFPLAYERMINI_DFP_MINI_H_ */
