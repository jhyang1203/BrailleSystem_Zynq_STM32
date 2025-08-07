/*
 * Listener_CNN.c
 *
 *  Created on: Jul 18, 2025
 *      Author: kccistc
 */

#include "Listener_CNN.h"


void Listener_CNNStop()
{

}

void Listener_CNNExecute()
{
	char input = UART_ReceiveChar();
	uint8_t pattern = GetBraillePatternFromChar(input);
    osMessagePut(CNN_brailleMsgBox, pattern, 0);  // pattern 값을 메시지로 전송
}

// 문자 하나를 점자 6비트로 변환
uint8_t GetBraillePatternFromChar(char c)
{
    for (int i = 0; i < braille_table_size; i++) {
        if (global_brailleTable[i].alphabet == c) {
            return global_brailleTable[i].bits6;
        }
    }
    return 0x00;  // 잘못된 문자일 경우
}

// 메인 루프 안에서 호출할 UART → 점자 변환 로직
char UART_ReceiveChar()
{
    char ch = 0;
    HAL_UART_Receive(&huart6, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}
