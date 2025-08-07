/*
 * Controller_CNN.c
 *
 *  Created on: Jul 18, 2025
 *      Author: kccistc
 */

#include "Controller_CNN.h"

void CNN_Execute()
{
    osEvent evt = osMessageGet(CNN_brailleMsgBox, 0); // 패턴 수신
    uint8_t pattern;

    if (evt.status == osEventMessage) {
        pattern = evt.value.v;

        char matchedChar = '?';  // 기본값: 매칭 안 됨 표시
        for (int i = 0; i < braille_table_size; i++) {
            if (global_brailleTable[i].bits6 == pattern) {
                matchedChar = global_brailleTable[i].alphabet;
                break;
            }
        }

        // 구조체 생성 및 메일 큐 전송
        BrailleAscii_t *pData = osMailAlloc(CNN_brailleCharMailBox, 0);
        if (pData != NULL) {
            pData->bits6 = pattern;
            pData->alphabet = matchedChar;

            char msg[64];
            snprintf(msg, sizeof(msg), "pattern: 0x%02X, character: %c\r\n", pData->bits6, pData->alphabet);
            HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), 1000);

            osMailPut(CNN_brailleCharMailBox, pData);
          //  osMailFree(brailleCharMailBox, pData);  // 💡 이거 없으면 8개 제한
        }
    }
}



