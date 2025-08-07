/*
 * Presenter_DFPMini.c
 *
 *  Created on: Jul 21, 2025
 *      Author: kccistc
 */
#include "Presenter_DFPMini.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "cmsis_os.h"
#include "tim.h"
#include "dfp_mini.h"

void Presenter_DFPMiniInit()
{
	dfp_init(25);	// 0~30 까지 볼륨 설정 ㄱㄱ
}


void Dfp_playbykey(char key) {
	if (key >= 'a' && key <= 'z') {
		uint16_t track = (key - 'a') + 1;   // 'a'→1.mp3, 'b'→2.mp3 ...

		dfp_stop();
		osDelay(250);

		dfp_setVolume(25);
		osDelay(200);

		dfp_playTrack(track);
		osDelay(200);

	}
}

