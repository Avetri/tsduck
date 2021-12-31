//----------------------------------------------------------------------------
//
// TSDuck - The MPEG Transport Stream Toolkit
// Copyright (c) 2005-2022, Thierry Lelegard
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
// THE POSSIBILITY OF SUCH DAMAGE.
//
//----------------------------------------------------------------------------

#include "tsTeletextCharset.h"


//-----------------------------------------------------------------------------
// Static parity checking table.
//-----------------------------------------------------------------------------

namespace {
    const uint8_t PARITY_8[256] = {
        0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x00, 0x01, 0x01, 0x00,
        0x01, 0x00, 0x00, 0x01, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x00, 0x01,
        0x01, 0x00, 0x00, 0x01, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x00, 0x01,
        0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x00, 0x01, 0x01, 0x00,
        0x01, 0x00, 0x00, 0x01, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x00, 0x01,
        0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x00, 0x01, 0x01, 0x00,
        0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x00, 0x01, 0x01, 0x00,
        0x01, 0x00, 0x00, 0x01, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x00, 0x01,
        0x01, 0x00, 0x00, 0x01, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x00, 0x01,
        0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x00, 0x01, 0x01, 0x00,
        0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x00, 0x01, 0x01, 0x00,
        0x01, 0x00, 0x00, 0x01, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x00, 0x01,
        0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x00, 0x01, 0x01, 0x00,
        0x01, 0x00, 0x00, 0x01, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x00, 0x01,
        0x01, 0x00, 0x00, 0x01, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x00, 0x01,
        0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x00, 0x01, 0x01, 0x00
    };
}


//-----------------------------------------------------------------------------
// Teletext character set tables. All characters are encoded in UCS-2.
//-----------------------------------------------------------------------------

//
// Initial base content of a charset.
//
const ts::TeletextCharset::G0CharsetData ts::TeletextCharset::G0Base = {
    {
        // Latin G0 Primary Set
        0x0020, 0x0021, 0x0022, 0x00A3, 0x0024, 0x0025, 0x0026, 0x0027, 0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F,
        0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 0x0038, 0x0039, 0x003A, 0x003B, 0x003C, 0x003D, 0x003E, 0x003F,
        0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047, 0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x004E, 0x004F,
        0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057, 0x0058, 0x0059, 0x005A, 0x00AB, 0x00BD, 0x00BB, 0x005E, 0x0023,
        0x002D, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067, 0x0068, 0x0069, 0x006A, 0x006B, 0x006C, 0x006D, 0x006E, 0x006F,
        0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077, 0x0078, 0x0079, 0x007A, 0x00BC, 0x00A6, 0x00BE, 0x00F7, 0x007F
    },
    {
        // Cyrillic G0 Primary Set - Option 1 - Serbian/Croatian
        0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x044B, 0x0027, 0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F,
        0x0030, 0x0031, 0x3200, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 0x0038, 0x0039, 0x003A, 0x003B, 0x003C, 0x003D, 0x003E, 0x003F,
        0x0427, 0x0410, 0x0411, 0x0426, 0x0414, 0x0415, 0x0424, 0x0413, 0x0425, 0x0418, 0x0408, 0x041A, 0x041B, 0x041C, 0x041D, 0x041E,
        0x041F, 0x040C, 0x0420, 0x0421, 0x0422, 0x0423, 0x0412, 0x0403, 0x0409, 0x040A, 0x0417, 0x040B, 0x0416, 0x0402, 0x0428, 0x040F,
        0x0447, 0x0430, 0x0431, 0x0446, 0x0434, 0x0435, 0x0444, 0x0433, 0x0445, 0x0438, 0x0428, 0x043A, 0x043B, 0x043C, 0x043D, 0x043E,
        0x043F, 0x042C, 0x0440, 0x0441, 0x0442, 0x0443, 0x0432, 0x0423, 0x0429, 0x042A, 0x0437, 0x042B, 0x0436, 0x0422, 0x0448, 0x042F
    },
    {
        // Cyrillic G0 Primary Set - Option 2 - Russian/Bulgarian
        0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x044B, 0x0027, 0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F,
        0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 0x0038, 0x0039, 0x003A, 0x003B, 0x003C, 0x003D, 0x003E, 0x003F,
        0x042E, 0x0410, 0x0411, 0x0426, 0x0414, 0x0415, 0x0424, 0x0413, 0x0425, 0x0418, 0x0419, 0x041A, 0x041B, 0x041C, 0x041D, 0x041E,
        0x041F, 0x042F, 0x0420, 0x0421, 0x0422, 0x0423, 0x0416, 0x0412, 0x042C, 0x042A, 0x0417, 0x0428, 0x042D, 0x0429, 0x0427, 0x042B,
        0x044E, 0x0430, 0x0431, 0x0446, 0x0434, 0x0435, 0x0444, 0x0433, 0x0445, 0x0438, 0x0439, 0x043A, 0x043B, 0x043C, 0x043D, 0x043E,
        0x043F, 0x044F, 0x0440, 0x0441, 0x0442, 0x0443, 0x0436, 0x0432, 0x044C, 0x044A, 0x0437, 0x0448, 0x044D, 0x0449, 0x0447, 0x044B
    },
    {
        // Cyrillic G0 Primary Set - Option 3 - Ukrainian
        0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x00EF, 0x0027, 0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F,
        0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 0x0038, 0x0039, 0x003A, 0x003B, 0x003C, 0x003D, 0x003E, 0x003F,
        0x042E, 0x0410, 0x0411, 0x0426, 0x0414, 0x0415, 0x0424, 0x0413, 0x0425, 0x0418, 0x0419, 0x041A, 0x041B, 0x041C, 0x041D, 0x041E,
        0x041F, 0x042F, 0x0420, 0x0421, 0x0422, 0x0423, 0x0416, 0x0412, 0x042C, 0x0049, 0x0417, 0x0428, 0x042D, 0x0429, 0x0427, 0x00CF,
        0x044E, 0x0430, 0x0431, 0x0446, 0x0434, 0x0435, 0x0444, 0x0433, 0x0445, 0x0438, 0x0439, 0x043A, 0x043B, 0x043C, 0x043D, 0x043E,
        0x043F, 0x044F, 0x0440, 0x0441, 0x0442, 0x0443, 0x0436, 0x0432, 0x044C, 0x0069, 0x0437, 0x0448, 0x044D, 0x0449, 0x0447, 0x00FF
    },
    {
        // Greek G0 Primary Set
        0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027, 0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F,
        0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 0x0038, 0x0039, 0x003A, 0x003B, 0x003C, 0x003D, 0x003E, 0x003F,
        0x0390, 0x0391, 0x0392, 0x0393, 0x0394, 0x0395, 0x0396, 0x0397, 0x0398, 0x0399, 0x039A, 0x039B, 0x039C, 0x039D, 0x039E, 0x039F,
        0x03A0, 0x03A1, 0x03A2, 0x03A3, 0x03A4, 0x03A5, 0x03A6, 0x03A7, 0x03A8, 0x03A9, 0x03AA, 0x03AB, 0x03AC, 0x03AD, 0x03AE, 0x03AF,
        0x03B0, 0x03B1, 0x03B2, 0x03B3, 0x03B4, 0x03B5, 0x03B6, 0x03B7, 0x03B8, 0x03B9, 0x03BA, 0x03BB, 0x03BC, 0x03BD, 0x03BE, 0x03BF,
        0x03C0, 0x03C1, 0x03C2, 0x03C3, 0x03C4, 0x03C5, 0x03C6, 0x03C7, 0x03C8, 0x03C9, 0x03CA, 0x03CB, 0x03CC, 0x03CD, 0x03CE, 0x03CF
    },
    {
        // Arabic G0 Primary Set (not yet implemented)
        0x0000,
    },
    {
        // Hebrew G0 Primary Set (not yet implemented)
        0x0000,
    }
};

namespace {
    // Array positions where chars from G0_LATIN_NATIONAL_SUBSETS are injected into G0[LATIN]
    const uint8_t G0_LATIN_NATIONAL_SUBSETS_POSITIONS[13] = {
        0x03, 0x04, 0x20, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x5B, 0x5C, 0x5D, 0x5E
    };

    // See ETSI EN 300 706, section 15.2: Designation of default G0 and G2 sets and national option sub-sets,
    // Table 32: Function of Default G0 and G2 Character Set Designation and National Option
    // Selection bits in packets X/28/0 Format 1, X/28/4, M/29/0 and M/29/4

    const uint16_t G0_LATIN_NATIONAL_SUBSETS[13][13] = {
        {
            // 0, English
            0x00A3, 0x0024, 0x0040, 0x00AB, 0x00BD, 0x00BB, 0x005E, 0x0023, 0x002D, 0x00BC, 0x00A6, 0x00BE, 0x00F7
        },
        {
            // 1, French
            0x00E9, 0x00EF, 0x00E0, 0x00EB, 0x00EA, 0x00F9, 0x00EE, 0x0023, 0x00E8, 0x00E2, 0x00F4, 0x00FB, 0x00E7
        },
        {
            // 2, Swedish, Finnish, Hungarian
            0x0023, 0x00A4, 0x00C9, 0x00C4, 0x00D6, 0x00C5, 0x00DC, 0x005F, 0x00E9, 0x00E4, 0x00F6, 0x00E5, 0x00FC
        },
        {
            // 3, Czech, Slovak
            0x0023, 0x016F, 0x010D, 0x0165, 0x017E, 0x00FD, 0x00ED, 0x0159, 0x00E9, 0x00E1, 0x011B, 0x00FA, 0x0161
        },
        {
            // 4, German
            0x0023, 0x0024, 0x00A7, 0x00C4, 0x00D6, 0x00DC, 0x005E, 0x005F, 0x00B0, 0x00E4, 0x00F6, 0x00FC, 0x00DF
        },
        {
            // 5, Portuguese, Spanish
            0x00E7, 0x0024, 0x00A1, 0x00E1, 0x00E9, 0x00ED, 0x00F3, 0x00FA, 0x00BF, 0x00FC, 0x00F1, 0x00E8, 0x00E0
        },
        {
            // 6, Italian
            0x00A3, 0x0024, 0x00E9, 0x00B0, 0x00E7, 0x00BB, 0x005E, 0x0023, 0x00F9, 0x00E0, 0x00F2, 0x00E8, 0x00EC
        },
        {
            // 7, Romanian
            0x0023, 0x00A4, 0x0162, 0x00C2, 0x015E, 0x0102, 0x00CE, 0x0131, 0x0163, 0x00E2, 0x015F, 0x0103, 0x00EE
        },
        {
            // 8, Polish
            0x0023, 0x0144, 0x0105, 0x017B, 0x015A, 0x0141, 0x0107, 0x00F3, 0x0119, 0x017C, 0x015B, 0x0142, 0x017A
        },
        {
            // 9, Turkish
            0x0054, 0x011F, 0x0130, 0x015E, 0x00D6, 0x00C7, 0x00DC, 0x011E, 0x0131, 0x015F, 0x00F6, 0x00E7, 0x00FC
        },
        {
            // 10, Serbian, Croatian, Slovenian
            0x0023, 0x00CB, 0x010C, 0x0106, 0x017D, 0x0110, 0x0160, 0x00EB, 0x010D, 0x0107, 0x017E, 0x0111, 0x0161
        },
        {
            // 11, Estonian
            0x0023, 0x00F5, 0x0160, 0x00C4, 0x00D6, 0x017E, 0x00DC, 0x00D5, 0x0161, 0x00E4, 0x00F6, 0x017E, 0x00FC
        },
        {
            // 12, Lettish, Lithuanian
            0x0023, 0x0024, 0x0160, 0x0117, 0x0119, 0x017D, 0x010D, 0x016B, 0x0161, 0x0105, 0x0173, 0x017E, 0x012F
        }
    };

    // References to the G0_LATIN_NATIONAL_SUBSETS array
    const uint8_t G0_LATIN_NATIONAL_SUBSETS_MAP[56] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x01, 0x02, 0x03, 0x04, 0xFF, 0x06, 0xFF,
        0x00, 0x01, 0x02, 0x09, 0x04, 0x05, 0x06, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0A, 0xFF, 0x07,
        0xFF, 0xFF, 0x0B, 0x03, 0x04, 0xFF, 0x0C, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0x09, 0xFF, 0xFF, 0xFF, 0xFF
    };

    // G2
    const ts::UChar G2[3][ts::TeletextCharset::CHAR_COUNT] = {
        {
            // Latin G2 Supplementary Set
            0x0020, 0x00A1, 0x00A2, 0x00A3, 0x0024, 0x00A5, 0x0023, 0x00A7, 0x00A4, 0x2018, 0x201C, 0x00AB, 0x2190, 0x2191, 0x2192, 0x2193,
            0x00B0, 0x00B1, 0x00B2, 0x00B3, 0x00D7, 0x00B5, 0x00B6, 0x00B7, 0x00F7, 0x2019, 0x201D, 0x00BB, 0x00BC, 0x00BD, 0x00BE, 0x00BF,
            0x0020, 0x0300, 0x0301, 0x0302, 0x0303, 0x0304, 0x0306, 0x0307, 0x0308, 0x0000, 0x030A, 0x0327, 0x005F, 0x030B, 0x0328, 0x030C,
            0x2015, 0x00B9, 0x00AE, 0x00A9, 0x2122, 0x266A, 0x20AC, 0x2030, 0x03B1, 0x0000, 0x0000, 0x0000, 0x215B, 0x215C, 0x215D, 0x215E,
            0x03A9, 0x00C6, 0x0110, 0x00AA, 0x0126, 0x0000, 0x0132, 0x013F, 0x0141, 0x00D8, 0x0152, 0x00BA, 0x00DE, 0x0166, 0x014A, 0x0149,
            0x0138, 0x00E6, 0x0111, 0x00F0, 0x0127, 0x0131, 0x0133, 0x0140, 0x0142, 0x00F8, 0x0153, 0x00DF, 0x00FE, 0x0167, 0x014B, 0x0020
        },
        {
            // Cyrillic G2 Supplementary Set
            0x00A0, 0x00A1, 0x00A2, 0x00A3, 0x0020, 0x00A5, 0x0023, 0x00A7, 0x0020, 0x2018, 0x201C, 0x00AB, 0x2190, 0x2191, 0x2192, 0x2193,
            0x00B0, 0x00B1, 0x00B2, 0x00B3, 0x00D7, 0x00B5, 0x00B6, 0x00B7, 0x00F7, 0x2019, 0x201D, 0x00BB, 0x00BC, 0x00BD, 0x00BE, 0x00BF,
            0x0020, 0x02CB, 0x02CA, 0x02C6, 0x02DC, 0x02C9, 0x02D8, 0x02D9, 0x00A8, 0x002E, 0x02DA, 0x02CF, 0x02CD, 0x02DD, 0x02DB, 0x02C7,
            0x2014, 0x00B9, 0x00AE, 0x00A9, 0x2122, 0x266A, 0x20A0, 0x2030, 0x0251, 0x0141, 0x0142, 0x00DF, 0x215B, 0x215C, 0x215D, 0x215E,
            0x0044, 0x0045, 0x0046, 0x0047, 0x0049, 0x004A, 0x004B, 0x004C, 0x004E, 0x0051, 0x0052, 0x0053, 0x0055, 0x0056, 0x0057, 0x005A,
            0x0064, 0x0065, 0x0066, 0x0067, 0x0069, 0x006A, 0x006B, 0x006C, 0x006E, 0x0071, 0x0072, 0x0073, 0x0075, 0x0076, 0x0077, 0x007A
        },
        {
            // Greek G2 Supplementary Set
            0x00A0, 0x0061, 0x0062, 0x00A3, 0x0065, 0x0068, 0x0069, 0x00A7, 0x003A, 0x2018, 0x201C, 0x006B, 0x2190, 0x2191, 0x2192, 0x2193,
            0x00B0, 0x00B1, 0x00B2, 0x00B3, 0x0078, 0x006D, 0x006E, 0x0070, 0x00F7, 0x2019, 0x201D, 0x0074, 0x00BC, 0x00BD, 0x00BE, 0x0078,
            0x0020, 0x02CB, 0x02CA, 0x02C6, 0x02DC, 0x02C9, 0x02D8, 0x02D9, 0x00A8, 0x002E, 0x02DA, 0x02CF, 0x02CD, 0x02DD, 0x02DB, 0x02C7,
            0x003F, 0x00B9, 0x00AE, 0x00A9, 0x2122, 0x266A, 0x20A0, 0x2030, 0x0251, 0x038A, 0x038E, 0x038F, 0x215B, 0x215C, 0x215D, 0x215E,
            0x0043, 0x0044, 0x0046, 0x0047, 0x004A, 0x004C, 0x0051, 0x0052, 0x0053, 0x0055, 0x0056, 0x0057, 0x0059, 0x005A, 0x0386, 0x0389,
            0x0063, 0x0064, 0x0066, 0x0067, 0x006A, 0x006C, 0x0071, 0x0072, 0x0073, 0x0075, 0x0076, 0x0077, 0x0079, 0x007A, 0x0388, 0x25A0
        },
        // Unimplemented character sets: Arabic G2 Supplementary Set
    };

    // Conversion between normal and accented letters (15 accents, 2x26 letters).

    constexpr size_t ACCENT_MODE_COUNT   = 15;  // number of forms of accents
    constexpr size_t ACCENT_LETTER_COUNT = 52;  // letters: A-Z, then a-z

    const ts::UChar G2_ACCENTS[ACCENT_MODE_COUNT][ACCENT_LETTER_COUNT] = {
        {
            // grave
            0x00C0, 0x0000, 0x0000, 0x0000, 0x00C8, 0x0000, 0x0000, 0x0000, 0x00CC, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x00D2, 0x0000,
            0x0000, 0x0000, 0x0000, 0x0000, 0x00D9, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x00E0, 0x0000, 0x0000, 0x0000, 0x00E8, 0x0000,
            0x0000, 0x0000, 0x00EC, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x00F2, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x00F9, 0x0000,
            0x0000, 0x0000, 0x0000, 0x0000
        },
        {
            // acute
            0x00C1, 0x0000, 0x0106, 0x0000, 0x00C9, 0x0000, 0x0000, 0x0000, 0x00CD, 0x0000, 0x0000, 0x0139, 0x0000, 0x0143, 0x00D3, 0x0000,
            0x0000, 0x0154, 0x015A, 0x0000, 0x00DA, 0x0000, 0x0000, 0x0000, 0x00DD, 0x0179, 0x00E1, 0x0000, 0x0107, 0x0000, 0x00E9, 0x0000,
            0x0123, 0x0000, 0x00ED, 0x0000, 0x0000, 0x013A, 0x0000, 0x0144, 0x00F3, 0x0000, 0x0000, 0x0155, 0x015B, 0x0000, 0x00FA, 0x0000,
            0x0000, 0x0000, 0x00FD, 0x017A
        },
        {
            // circumflex
            0x00C2, 0x0000, 0x0108, 0x0000, 0x00CA, 0x0000, 0x011C, 0x0124, 0x00CE, 0x0134, 0x0000, 0x0000, 0x0000, 0x0000, 0x00D4, 0x0000,
            0x0000, 0x0000, 0x015C, 0x0000, 0x00DB, 0x0000, 0x0174, 0x0000, 0x0176, 0x0000, 0x00E2, 0x0000, 0x0109, 0x0000, 0x00EA, 0x0000,
            0x011D, 0x0125, 0x00EE, 0x0135, 0x0000, 0x0000, 0x0000, 0x0000, 0x00F4, 0x0000, 0x0000, 0x0000, 0x015D, 0x0000, 0x00FB, 0x0000,
            0x0175, 0x0000, 0x0177, 0x0000
        },
        {
            // tilde
            0x00C3, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0128, 0x0000, 0x0000, 0x0000, 0x0000, 0x00D1, 0x00D5, 0x0000,
            0x0000, 0x0000, 0x0000, 0x0000, 0x0168, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x00E3, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
            0x0000, 0x0000, 0x0129, 0x0000, 0x0000, 0x0000, 0x0000, 0x00F1, 0x00F5, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0169, 0x0000,
            0x0000, 0x0000, 0x0000, 0x0000
        },
        {
            // macron
            0x0100, 0x0000, 0x0000, 0x0000, 0x0112, 0x0000, 0x0000, 0x0000, 0x012A, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x014C, 0x0000,
            0x0000, 0x0000, 0x0000, 0x0000, 0x016A, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0101, 0x0000, 0x0000, 0x0000, 0x0113, 0x0000,
            0x0000, 0x0000, 0x012B, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x014D, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x016B, 0x0000,
            0x0000, 0x0000, 0x0000, 0x0000
        },
        {
            // breve
            0x0102, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x011E, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
            0x0000, 0x0000, 0x0000, 0x0000, 0x016C, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0103, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
            0x011F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x016D, 0x0000,
            0x0000, 0x0000, 0x0000, 0x0000
        },
        {
            // dot
            0x0000, 0x0000, 0x010A, 0x0000, 0x0116, 0x0000, 0x0120, 0x0000, 0x0130, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
            0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x017B, 0x0000, 0x0000, 0x010B, 0x0000, 0x0117, 0x0000,
            0x0121, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
            0x0000, 0x0000, 0x0000, 0x017C
        },
        {
            // umlaut
            0x00C4, 0x0000, 0x0000, 0x0000, 0x00CB, 0x0000, 0x0000, 0x0000, 0x00CF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x00D6, 0x0000,
            0x0000, 0x0000, 0x0000, 0x0000, 0x00DC, 0x0000, 0x0000, 0x0000, 0x0178, 0x0000, 0x00E4, 0x0000, 0x0000, 0x0000, 0x00EB, 0x0000,
            0x0000, 0x0000, 0x00EF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x00F6, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x00FC, 0x0000,
            0x0000, 0x0000, 0x00FF, 0x0000
        },
        {
            // unused
            0
        },
        {
            // ring
            0x00C5, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
            0x0000, 0x0000, 0x0000, 0x0000, 0x016E, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x00E5, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
            0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x016F, 0x0000,
            0x0000, 0x0000, 0x0000, 0x0000
        },
        {
            // cedilla
            0x0000, 0x0000, 0x00C7, 0x0000, 0x0000, 0x0000, 0x0122, 0x0000, 0x0000, 0x0000, 0x0136, 0x013B, 0x0000, 0x0145, 0x0000, 0x0000,
            0x0000, 0x0156, 0x015E, 0x0162, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x00E7, 0x0000, 0x0000, 0x0000,
            0x0000, 0x0000, 0x0000, 0x0000, 0x0137, 0x013C, 0x0000, 0x0146, 0x0000, 0x0000, 0x0000, 0x0157, 0x015F, 0x0163, 0x0000, 0x0000,
            0x0000, 0x0000, 0x0000, 0x0000
        },
        {
            // unused
            0
        },
        {
            // double acute
            0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0150, 0x0000,
            0x0000, 0x0000, 0x0000, 0x0000, 0x0170, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
            0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0151, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0171, 0x0000,
            0x0000, 0x0000, 0x0000, 0x0000
        },
        {
            // ogonek
            0x0104, 0x0000, 0x0000, 0x0000, 0x0118, 0x0000, 0x0000, 0x0000, 0x012E, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
            0x0000, 0x0000, 0x0000, 0x0000, 0x0172, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0105, 0x0000, 0x0000, 0x0000, 0x0119, 0x0000,
            0x0000, 0x0000, 0x012F, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0173, 0x0000,
            0x0000, 0x0000, 0x0000, 0x0000
        },
        {
            // caron
            0x0000, 0x0000, 0x010C, 0x010E, 0x011A, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x013D, 0x0000, 0x0147, 0x0000, 0x0000,
            0x0000, 0x0158, 0x0160, 0x0164, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x017D, 0x0000, 0x0000, 0x010D, 0x010F, 0x011B, 0x0000,
            0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x013E, 0x0000, 0x0148, 0x0000, 0x0000, 0x0000, 0x0159, 0x0161, 0x0165, 0x0000, 0x0000,
            0x0000, 0x0000, 0x0000, 0x017E
        }
    };
}


//----------------------------------------------------------------------------
// Constructor.
//----------------------------------------------------------------------------

ts::TeletextCharset::TeletextCharset() :
    _current(0x00),
    _g0m29(UNDEFINED),
    _g0x28(UNDEFINED),
    _g0Default(LATIN)
{
    ::memcpy(&_G0, G0Base, sizeof(_G0));
}


//-----------------------------------------------------------------------------
// Check parity and translate any reasonable Teletext character into UCS-2.
//-----------------------------------------------------------------------------

ts::UChar ts::TeletextCharset::teletextToUcs2(uint8_t chr) const
{
    if (PARITY_8[chr] == 0) {
        // Unrecoverable parity data error.
        return 0x20;
    }
    else {
        const UChar rchr = chr & 0x7F;
        if (rchr < 0x20) {
            return rchr;
        }
        else if (_g0Default == LATIN) {
            return _G0[LATIN][rchr - 0x20];
        }
        else {
            return G0Base[_g0Default][rchr - 0x20];
        }
    }
}


//-----------------------------------------------------------------------------
// Translate a G2 character into UCS-2.
//-----------------------------------------------------------------------------

ts::UChar ts::TeletextCharset::g2ToUcs2(uint8_t chr) const
{
    return chr >= 0x20 && chr < 0x20 + CHAR_COUNT ? G2[0][chr - 0x20] : 0x00;
}

ts::UChar ts::TeletextCharset::g2AccentToUcs2(uint8_t chr, uint8_t accent) const
{
    if (chr >= 65 && chr <= 90 && accent < ACCENT_MODE_COUNT) { // A - Z
        return G2_ACCENTS[accent][chr - 65];
    }
    else if (chr >= 97 && chr <= 122 && accent < ACCENT_MODE_COUNT) { // a - z
        return G2_ACCENTS[accent][chr - 71];
    }
    else { // other
        return teletextToUcs2(chr);
    }
}


//-----------------------------------------------------------------------------
// Change character sets.
//-----------------------------------------------------------------------------

void ts::TeletextCharset::setG0Charset(uint32_t triplet)
{
    // ETS 300 706, Table 32
    if ((triplet & 0x3c00) != 0x1000) {
        _g0Default = LATIN;
    }
    else if ((triplet & 0x0380) == 0x0000) {
        _g0Default = CYRILLIC1;
    }
    else if ((triplet & 0x0380) == 0x0200) {
        _g0Default = CYRILLIC2;
    }
    else if ((triplet & 0x0380) == 0x0280) {
        _g0Default = CYRILLIC3;
    }
    else {
        _g0Default = LATIN;
    }
}

void ts::TeletextCharset::setX28(uint8_t charset)
{
    if (_g0Default == LATIN) {
        remapG0(_g0x28 = charset);
    }
}

void ts::TeletextCharset::setM29(uint8_t charset)
{
    if (_g0Default == LATIN) {
        _g0m29 = charset;
        if (_g0x28 == UNDEFINED) {
            remapG0(_g0m29);
        }
    }
}

void ts::TeletextCharset::resetX28(uint8_t fallback)
{
    if (_g0Default == LATIN) {
        _g0x28 = UNDEFINED;
        remapG0(_g0m29 != UNDEFINED ? _g0m29 : fallback);
    }
}


//-----------------------------------------------------------------------------
// Remap the GO character set.
//-----------------------------------------------------------------------------

void ts::TeletextCharset::remapG0(uint8_t charset)
{
    if (charset != _current && charset < sizeof(G0_LATIN_NATIONAL_SUBSETS_MAP)) {
        const uint8_t map = G0_LATIN_NATIONAL_SUBSETS_MAP[charset];
        if (map != 0xFF) {
            for (uint8_t n = 0; n < 13; n++) {
                _G0[LATIN][G0_LATIN_NATIONAL_SUBSETS_POSITIONS[n]] = G0_LATIN_NATIONAL_SUBSETS[map][n];
            }
            _current = charset;
        }
    }
}
