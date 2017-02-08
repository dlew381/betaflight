/*
 * This file is part of Cleanflight.
 *
 * Cleanflight is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Cleanflight is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Cleanflight.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdbool.h>
#include <stdint.h>

#include "huffman.h"


int huffmanEncodeBuf(uint8_t *outBuf, int outBufLen, const uint8_t *inBuf, int inLen, const huffmanTable_t *huffmanTable)
{
    int ret = 0;

    uint8_t *outByte = outBuf;
    *outByte = 0;
    uint8_t outBit = 0x80;
    for (int ii = 0; ii< inLen; ++ii) {
        const int huffCodeLen = huffmanTable[*inBuf].codeLen;
        const uint16_t huffCode = huffmanTable[*inBuf].code;
        ++inBuf;
        uint16_t testBit = 0x8000;
        for (int jj = 0; jj < huffCodeLen; ++jj) {
            if (huffCode & testBit) {
                *outByte |= outBit;
            }
            testBit >>= 1;
            outBit >>= 1;
            if (outBit == 0) {
                outBit = 0x80;
                ++outByte;
                *outByte = 0;
                ++ret;
            }
            if (ret >= outBufLen) {
                return -1;
            }
        }
    }
    if (outBit != 0x80) {
        // ensure last character in output buffer is counted
        ++ret;
    }
    return ret;
}

