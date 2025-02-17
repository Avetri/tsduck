//----------------------------------------------------------------------------
//
// TSDuck - The MPEG Transport Stream Toolkit
// Copyright (c) 2005-2023, Thierry Lelegard
// BSD-2-Clause license, see LICENSE.txt file or https://tsduck.io/license
//
//----------------------------------------------------------------------------
//
//  TSUnit test suite for class DumpCharset.
//
//----------------------------------------------------------------------------

#include "tsDumpCharset.h"
#include "tsByteBlock.h"
#include "tsunit.h"


//----------------------------------------------------------------------------
// The test fixture
//----------------------------------------------------------------------------

class DumpCharsetTest: public tsunit::Test
{
public:
    virtual void beforeTest() override;
    virtual void afterTest() override;

    void testDecode();
    void testEncode();
    void testCanEncode();

    TSUNIT_TEST_BEGIN(DumpCharsetTest);
    TSUNIT_TEST(testDecode);
    TSUNIT_TEST(testEncode);
    TSUNIT_TEST(testCanEncode);
    TSUNIT_TEST_END();
};

TSUNIT_REGISTER(DumpCharsetTest);


//----------------------------------------------------------------------------
// Initialization.
//----------------------------------------------------------------------------

// Test suite initialization method.
void DumpCharsetTest::beforeTest()
{
}

// Test suite cleanup method.
void DumpCharsetTest::afterTest()
{
}


//----------------------------------------------------------------------------
// Test cases
//----------------------------------------------------------------------------

void DumpCharsetTest::testCanEncode()
{
    const ts::Charset& cset(ts::DumpCharset::DUMP);

    TSUNIT_ASSERT(cset.canEncode(u""));
    TSUNIT_ASSERT(cset.canEncode(u" 012 345 "));
    TSUNIT_ASSERT(!cset.canEncode(u"012 345 6"));
    TSUNIT_ASSERT(!cset.canEncode(u"01 a"));
}

void DumpCharsetTest::testDecode()
{
    static const uint8_t data[] = {0x00, 0x01, 0x02, 0x11, 0xEA, 0x07, 0x80, 0x34, 0xB2};

    const ts::Charset& cset(ts::DumpCharset::DUMP);

    TSUNIT_EQUAL(u"00 01 02 11 EA 07 80 34 B2", cset.decoded(data, 9));
    TSUNIT_EQUAL(u"02 11 EA", cset.decoded(data + 2, 3));
    TSUNIT_EQUAL(u"", cset.decoded(data, 0));
    TSUNIT_EQUAL(u"", cset.decoded(nullptr, 3));
}

void DumpCharsetTest::testEncode()
{
    const ts::Charset& cset(ts::DumpCharset::DUMP);

    uint8_t buffer[20];
    uint8_t* data = buffer;
    size_t size = sizeof(buffer);
    std::memset(buffer, 0, sizeof(buffer));

    TSUNIT_EQUAL(0, cset.encode(data, size, u""));
    TSUNIT_EQUAL(buffer, data);
    TSUNIT_EQUAL(20, size);

    data = buffer;
    size = sizeof(buffer);
    std::memset(buffer, 0, sizeof(buffer));

    TSUNIT_EQUAL(14, cset.encode(data, size, u"  01 0211 e a zz 01"));
    TSUNIT_EQUAL(buffer + 4, data);
    TSUNIT_EQUAL(16, size);
    TSUNIT_EQUAL(0x01, buffer[0]);
    TSUNIT_EQUAL(0x02, buffer[1]);
    TSUNIT_EQUAL(0x11, buffer[2]);
    TSUNIT_EQUAL(0xEA, buffer[3]);
}
