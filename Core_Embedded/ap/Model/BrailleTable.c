/*
 * BrailleTable.c
 *
 *  Created on: Jul 18, 2025
 *      Author: kccistc
 */


#include "BrailleTable.h"

const BrailleAscii_t global_brailleTable[] = {
    {0x01, 'a'}, {0x05, 'b'}, {0x03, 'c'}, {0x0B, 'd'}, {0x09, 'e'},
    {0x07, 'f'}, {0x0F, 'g'}, {0x0D, 'h'}, {0x06, 'i'}, {0x0E, 'j'},
    {0x11, 'k'}, {0x15, 'l'}, {0x13, 'm'}, {0x1B, 'n'}, {0x19, 'o'},
    {0x17, 'p'}, {0x1F, 'q'}, {0x1D, 'r'}, {0x16, 's'}, {0x1E, 't'},
    {0x31, 'u'}, {0x35, 'v'}, {0x2E, 'w'}, {0x33, 'x'}, {0x3B, 'y'}, {0x39, 'z'}
};

const int braille_table_size = sizeof(global_brailleTable) / sizeof(BrailleAscii_t);
