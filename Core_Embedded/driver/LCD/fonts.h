/* vim: set ai et ts=4 sw=4: */
#ifndef __FONTS_H__
#define __FONTS_H__

#include <stdint.h>



typedef struct {
    const uint8_t width;
    uint8_t height;
    const uint16_t *data;
    uint8_t scale_x;    // 추가: 가로 확대 배율
    uint8_t scale_y;    // 추가: 세로 확대 배율
} FontDef;

void draw_char_scaled(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor);

extern FontDef Font_7x10;
extern FontDef Font_11x18;
extern FontDef Font_16x26;

#endif // __FONTS_H__
