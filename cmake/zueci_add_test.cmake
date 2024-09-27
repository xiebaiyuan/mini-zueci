# libzueci - an open source UTF-8 ECI library adapted from libzint
# Copyright (C) 2022 gitlost
# SPDX-License-Identifier: BSD-3-Clause
# vim: set ts=4 sw=4 et :

macro(zueci_add_test test_name test_command)
    set(ADDITIONAL_LIBS "${ARGN}" ${LIBRARY_FLAGS})
    add_executable(${test_command} ${test_command}.c)
    target_link_libraries(${test_command} testcommon ${ADDITIONAL_LIBS})
    add_test(${test_name} ${test_command})
    set_tests_properties(${test_name} PROPERTIES ENVIRONMENT "CMAKE_CURRENT_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}")
    if(ZINT_STATIC)
        add_executable(${test_command}-static ${test_command}.c)
        target_link_libraries(${test_command}-static testcommon-static ${ADDITIONAL_LIBS})
        add_test(${test_name}-static ${test_command}-static)
        set_tests_properties(${test_name}-static PROPERTIES ENVIRONMENT "CMAKE_CURRENT_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}")
    endif()
endmacro()
