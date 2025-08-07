/*
 * Switch.c
 *
 *  Created on: Jul 16, 2025
 *      Author: kccistc
 */

#include "Switch.h"

const BrailleChar_t braille_table[] = {
    {0x01, 'a'},  // ●1
    {0x05, 'b'},  // ●1 ●3
    {0x03, 'c'},  // ●1 ●2
    {0x0B, 'd'},  // ●1 ●2 ●4
    {0x09, 'e'},  // ●1 ●4
    {0x07, 'f'},  // ●1 ●2 ●3
    {0x0F, 'g'},  // ●1 ●2 ●3 ●4
    {0x0D, 'h'},  // ●1 ●3 ●4
    {0x06, 'i'},  // ●2 ●3
    {0x0E, 'j'},  // ●2 ●3 ●4
    {0x11, 'k'},  // ●1 ●5
    {0x15, 'l'},  // ●1 ●3 ●5
    {0x13, 'm'},  // ●1 ●2 ●5
    {0x1B, 'n'},  // ●1 ●2 ●4 ●5
    {0x19, 'o'},  // ●1 ●4 ●5
    {0x17, 'p'},  // ●1 ●2 ●3 ●5
    {0x1F, 'q'},  // ●1 ●2 ●3 ●4 ●5
    {0x1D, 'r'},  // ●1 ●3 ●4 ●5
    {0x16, 's'},  // ●2 ●3 ●5 //
    {0x1E, 't'},  // ●2 ●3 ●4 ●5
    {0x31, 'u'},  // ●1 ●5 ●6
    {0x35, 'v'},  // ●1 ●3 ●5 ●6
    {0x2E, 'w'},  // ●2 ●3 ●4 ●6 //
    {0x33, 'x'},  // ●1 ●2 ●5 ●6
    {0x3B, 'y'},  // ●1 ●2 ●4 ●5 ●6
    {0x39, 'z'}   // ●1 ●4 ●5 ●6
};

void Switch_TxExecute()
{
    osEvent evt = osMessageGet(RFTx_brailleMsgBox, 0); // 패턴 수신
    uint8_t pattern;

    if (evt.status == osEventMessage) {
        pattern = evt.value.v;

        char matchedChar = '?';  // 기본값: 매칭 안 됨 표시
        for (int i = 0; i < sizeof(braille_table)/sizeof(BrailleChar_t); i++) {
            if (braille_table[i].pattern == pattern) {
            	pattern = braille_table[i].pattern;
                matchedChar = braille_table[i].character;
                break;
            }
        }

        if (matchedChar == '?'){
        	pattern = 0;
        }

        // 구조체 생성 및 메일 큐 전송
        BrailleChar_t *pData = osMailAlloc(RFTx_brailleCharMailBox, 0);
        if (pData != NULL) {
            pData->pattern = pattern;
            pData->character = matchedChar;

            char msg[64];
            snprintf(msg, sizeof(msg), "pattern: 0x%02X, character: %c\r\n", pData->pattern, pData->character);
            HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), 1000);

            osMailPut(RFTx_brailleCharMailBox, pData);
          //  osMailFree(brailleCharMailBox, pData);  // 💡 이거 없으면 8개 제한
        }
    }
}


void Switch_RxExecute()
{
    osEvent evt = osMessageGet(RFRx_brailleMsgBox, 0); // 패턴 수신
    uint8_t pattern;

    if (evt.status == osEventMessage) {
        pattern = evt.value.v;

        char matchedChar = '?';  // 기본값: 매칭 안 됨 표시
        for (int i = 0; i < sizeof(braille_table)/sizeof(BrailleChar_t); i++) {
            if (braille_table[i].pattern == pattern) {
            	pattern = braille_table[i].pattern;
                matchedChar = braille_table[i].character;
                break;
            }
        }

        if (matchedChar == '?'){
        	pattern = 0;
        }


        // 구조체 생성 및 메일 큐 전송
        BrailleChar_t *pData_rx = osMailAlloc(RFRx_brailleCharMailBox, 0);
        if (pData_rx != NULL) {
        	pData_rx->pattern = pattern;
        	pData_rx->character = matchedChar;

            char msg[64];
            snprintf(msg, sizeof(msg), "pattern: 0x%02X, character: %c\r\n", pData_rx->pattern, pData_rx->character);
            HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), 1000);

            osMailPut(RFRx_brailleCharMailBox, pData_rx);
          //  osMailFree(brailleCharMailBox, pData);  // 💡 이거 없으면 8개 제한
        }
    }
}
