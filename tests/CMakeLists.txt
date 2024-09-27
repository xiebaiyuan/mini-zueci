# libzueci - an open source UTF-8 ECI library adapted from libzint
# Copyright (C) 2022 gitlost
# Adapted from qrencode/tests/CMakeLists.txt
# Copyright (C) 2006-2017 Kentaro Fukuchi <kentaro@fukuchi.org>
# Due to above:
# SPDX-License-Identifier: LGPL-2.1+
# vim: set ts=4 sw=4 et :

cmake_minimum_required(VERSION 3.5)
project(zueci_tests)

enable_testing()

include(${libzueci_SOURCE_DIR}/cmake/zueci_add_test.cmake)

set(testcommon_SRCS zueci_testcommon.c zueci_testcommon.h)

add_library(testcommon ${testcommon_SRCS})
if(ZUECI_TEST_ICONV)
    target_link_libraries(testcommon zueci iconv)
    add_definitions(-DZUECI_TEST_ICONV)
else()
    target_link_libraries(testcommon zueci)
endif()
target_include_directories(testcommon PUBLIC ${zueci_tests_SOURCE_DIR})

if(ZINT_STATIC)
    add_library(testcommon-static ${testcommon_SRCS})
    if(ZUECI_TEST_ICONV)
        target_link_libraries(testcommon-static zueci-static iconv)
    else()
        target_link_libraries(testcommon-static zueci-static)
    endif()
    target_include_directories(testcommon-static PUBLIC ${zueci_tests_SOURCE_DIR})
endif()

zueci_add_test(sb test_sb)
zueci_add_test(big5 test_big5)
zueci_add_test(gb2312 test_gb2312)
zueci_add_test(gbk test_gbk)
zueci_add_test(gb18030 test_gb18030)
zueci_add_test(ksx1001 test_ksx1001)
zueci_add_test(sjis test_sjis)
