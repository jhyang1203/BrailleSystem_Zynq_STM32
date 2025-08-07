/*
 * BrailleTable.h
 *
 *  Created on: Jul 18, 2025
 *      Author: kccistc
 */

#ifndef AP_MODEL_BRAILLETABLE_H_
#define AP_MODEL_BRAILLETABLE_H_

#include <stdint.h>

// 점자 문자 구조체
typedef struct {
    uint8_t bits6;     // 6비트 점자 패턴
    char alphabet;     // 대응되는 알파벳 (소문자)
} BrailleAscii_t;

// 외부에 선언된 점자 테이블과 크기
extern const BrailleAscii_t global_brailleTable[];
extern const int braille_table_size;

#endif /* AP_MODEL_BRAILLETABLE_H_ */
