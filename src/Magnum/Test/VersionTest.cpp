/*
    This file is part of Magnum.

    Copyright © 2010, 2011, 2012, 2013, 2014, 2015, 2016, 2017, 2018, 2019,
                2020, 2021, 2022, 2023 Vladimír Vondruš <mosra@centrum.cz>

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

#include <Corrade/Containers/String.h>
#include <Corrade/TestSuite/Tester.h>
#include <Corrade/TestSuite/Compare/Numeric.h>
#include <Corrade/TestSuite/Compare/String.h>

#include "Magnum/Magnum.h"
#include "Magnum/versionPlugins.h"
#include "Magnum/Implementation/formatPluginsVersion.h"

namespace Magnum { namespace Test { namespace {

struct VersionTest: TestSuite::Tester {
    explicit VersionTest();

    void test();
    void format();
};

VersionTest::VersionTest() {
    addTests({&VersionTest::test,
              &VersionTest::format});
}

void VersionTest::test() {
    #ifdef MAGNUMPLUGINS_VERSION_COMMIT
    CORRADE_INFO(
        "MAGNUMPLUGINS_VERSION_YEAR:" << MAGNUMPLUGINS_VERSION_YEAR
        << Debug::newline <<
        "        MAGNUMPLUGINS_VERSION_MONTH:" << MAGNUMPLUGINS_VERSION_MONTH
        << Debug::newline <<
        "        MAGNUMPLUGINS_VERSION_COMMIT:" << MAGNUMPLUGINS_VERSION_COMMIT
        << Debug::newline <<
        "        MAGNUMPLUGINS_VERSION_HASH:" << reinterpret_cast<void*>(MAGNUMPLUGINS_VERSION_HASH)
        << Debug::newline <<
        "        MAGNUMPLUGINS_VERSION_STRING:" << MAGNUMPLUGINS_VERSION_STRING);
    #else
    CORRADE_INFO(
        "MAGNUMPLUGINS_VERSION_YEAR:" << MAGNUMPLUGINS_VERSION_YEAR
        << Debug::newline <<
        "        MAGNUMPLUGINS_VERSION_MONTH:" << MAGNUMPLUGINS_VERSION_MONTH
        << Debug::newline <<
        "        No Git version information available.");
    #endif

    CORRADE_COMPARE_AS(MAGNUMPLUGINS_VERSION_YEAR, 2019, TestSuite::Compare::GreaterOrEqual);
    CORRADE_COMPARE_AS(MAGNUMPLUGINS_VERSION_YEAR, 2100, TestSuite::Compare::LessOrEqual);
    CORRADE_COMPARE_AS(MAGNUMPLUGINS_VERSION_MONTH, 0, TestSuite::Compare::Greater);
    CORRADE_COMPARE_AS(MAGNUMPLUGINS_VERSION_MONTH, 12, TestSuite::Compare::LessOrEqual);
    #ifdef MAGNUMPLUGINS_VERSION_COMMIT
    CORRADE_COMPARE_AS(MAGNUMPLUGINS_VERSION_COMMIT, 0, TestSuite::Compare::GreaterOrEqual);
    #endif
}

void VersionTest::format() {
    Containers::String version = Implementation::formatPluginsVersion();
    CORRADE_INFO("Formatted version:" << version);

    #ifdef CORRADE_IS_DEBUG_BUILD
    CORRADE_COMPARE(version, "v<dev>");
    #else
    /* This check will break in the year 2030 */
    CORRADE_COMPARE_AS(version, "v202", TestSuite::Compare::StringHasPrefix);

    /* If no commit info is available, it's just the tag alone, thus no dashes
       or `g` characters */
    #if !defined(CORRADE_VERSION_COMMIT) && !defined(MAGNUM_VERSION_COMMIT) && !defined(MAGNUMPLUGINS_VERSION_COMMIT)
    CORRADE_COMPARE_AS(version, "-", TestSuite::Compare::StringNotContains);
    CORRADE_COMPARE_AS(version, "g", TestSuite::Compare::StringNotContains);

    /* Otherwise, if some info is not available, there are placeholders but
       also commit info */
    #elif !defined(CORRADE_VERSION_COMMIT) || !defined(MAGNUM_VERSION_COMMIT) || !defined(MAGNUMPLUGINS_VERSION_COMMIT)
    CORRADE_COMPARE_AS(version, "-xxxx", TestSuite::Compare::StringContains);
    CORRADE_COMPARE_AS(version, "g", TestSuite::Compare::StringContains);

    /* Otherwise there should be no placeholders and only commit info */
    #else
    CORRADE_COMPARE_AS(version, "-xxxx", TestSuite::Compare::StringNotContains);
    CORRADE_COMPARE_AS(version, "-", TestSuite::Compare::StringContains);
    CORRADE_COMPARE_AS(version, "g", TestSuite::Compare::StringContains);
    #endif
    #endif
}

}}}

CORRADE_TEST_MAIN(Magnum::Test::VersionTest)
