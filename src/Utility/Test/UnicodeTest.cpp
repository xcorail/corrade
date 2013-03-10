/*
    This file is part of Corrade.

    Copyright © 2007, 2008, 2009, 2010, 2011, 2012, 2013
              Vladimír Vondruš <mosra@centrum.cz>

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

#include <tuple>

#include "TestSuite/Tester.h"
#include "Utility/Unicode.h"

namespace Corrade { namespace Utility { namespace Test {

class UnicodeTest: public TestSuite::Tester {
    public:
        explicit UnicodeTest();

        void nextUtf8();
        void nextUtf8Error();
};

UnicodeTest::UnicodeTest() {
    addTests({&UnicodeTest::nextUtf8,
              &UnicodeTest::nextUtf8Error});
}

void UnicodeTest::nextUtf8() {
    std::uint32_t codepoint;
    std::size_t next;

    /* One-byte sequence */
    std::tie(codepoint, next) = Unicode::nextChar("   \x7f", 3);
    CORRADE_COMPARE(next, 4);
    CORRADE_COMPARE(codepoint, 127);

    /* Two byte sequence */
    std::tie(codepoint, next) = Unicode::nextChar("   \xce\xac", 3);
    CORRADE_COMPARE(next, 5);
    CORRADE_COMPARE(codepoint, 940);

    /* Three-byte sequence */
    std::tie(codepoint, next) = Unicode::nextChar("   \xea\xb8\x89", 3);
    CORRADE_COMPARE(next, 6);
    CORRADE_COMPARE(codepoint, 44553);

    /* Four-byte sequence */
    std::tie(codepoint, next) = Unicode::nextChar("   \xf6\x85\x98\x80", 3);
    CORRADE_COMPARE(next, 7);
    CORRADE_COMPARE(codepoint, 1594880);
}

void UnicodeTest::nextUtf8Error() {
    std::uint32_t codepoint;
    std::size_t next;

    /* Wrong start of a sequence */
    std::tie(codepoint, next) = Unicode::nextChar("   \xb0", 3);
    CORRADE_COMPARE(next, 4);
    CORRADE_COMPARE(codepoint, 0xffffffffu);

    /* Garbage in multibyte sequence */
    std::tie(codepoint, next) = Unicode::nextChar("   \xea\x40\xb8", 3);
    CORRADE_COMPARE(next, 4);
    CORRADE_COMPARE(codepoint, 0xffffffffu);

    /* Too small string for mulibyte sequence */
    std::tie(codepoint, next) = Unicode::nextChar("   \xce", 3);
    CORRADE_COMPARE(next, 4);
    CORRADE_COMPARE(codepoint, 0xffffffffu);
}

}}}

CORRADE_TEST_MAIN(Corrade::Utility::Test::UnicodeTest)