# libzueci - an open source UTF-8 ECI library adapted from libzint
# Copyright (C) 2022 gitlost
# SPDX-License-Identifier: BSD-3-Clause
# vim: set ts=4 sw=4 et :

cmake_minimum_required(VERSION 3.5)
project(libzueci)

set(ZUECI_VERSION_MAJOR 1)
set(ZUECI_VERSION_MINOR 0)
set(ZUECI_VERSION_RELEASE 1)
set(ZUECI_VERSION "${ZUECI_VERSION_MAJOR}.${ZUECI_VERSION_MINOR}.${ZUECI_VERSION_RELEASE}")

add_definitions(-DZUECI_VERSION=\"${ZUECI_VERSION}\")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

option(ZUECI_DEBUG      "Set debug compile flags"         ON)
option(ZUECI_NOOPT      "Set no optimize compile flags"   OFF)
option(ZUECI_SANITIZE   "Set sanitize compile/link flags" OFF)
option(ZUECI_TEST       "Set test compile flag"           ON)
option(ZUECI_TEST_ICONV "Use libiconv when testing"       OFF)
option(ZUECI_COVERAGE   "Set code coverage flags"         OFF)
option(ZUECI_STATIC     "Build static library"            OFF)

include(CheckCCompilerFlag)
include(CheckFunctionExists)
# close to eci..
add_definitions("-DZUECI_EMBED_NO_TO_ECI")

if(NOT MSVC) # Use default warnings if MSVC otherwise inundated
    check_c_compiler_flag("-Wall" C_COMPILER_FLAG_WALL)
    if(C_COMPILER_FLAG_WALL)
        add_compile_options("-Wall")
    endif()

    check_c_compiler_flag("-Wextra" C_COMPILER_FLAG_WEXTRA)
    if(C_COMPILER_FLAG_WEXTRA)
        add_compile_options("-Wextra")
    endif()

    check_c_compiler_flag("-Wpedantic" C_COMPILER_FLAG_WPEDANTIC)
    if(C_COMPILER_FLAG_WPEDANTIC)
        add_compile_options("-Wpedantic")
    endif()
endif()

if(ZUECI_DEBUG)
    check_c_compiler_flag("-g" C_COMPILER_FLAG_G)
    if(C_COMPILER_FLAG_G)
        add_compile_options("-g")
    endif()
endif()

if(ZUECI_NOOPT)
    check_c_compiler_flag("-O0" C_COMPILER_FLAG_O0)
    if(C_COMPILER_FLAG_O0)
        add_compile_options("-O0")
    endif()
endif()

if(ZUECI_SANITIZE)
    if(MSVC)
        if(MSVC_VERSION GREATER_EQUAL 1920)
            add_compile_options(-fsanitize=address)
            message(STATUS "ZUECI_SANITIZE: setting -fsanitize=address for MSVC 2019")
        else()
            message(STATUS "ZUECI_SANITIZE: ignoring for MSVC < 2019")
        endif()
    else()
        set(SANITIZERS address undefined)
        foreach(sanitizer IN ITEMS ${SANITIZERS})
            set(CMAKE_REQUIRED_LIBRARIES -fsanitize=${sanitizer})
            check_c_compiler_flag(-fsanitize=${sanitizer} C_COMPILER_FLAG_FSANITIZE_${sanitizer})
            if(C_COMPILER_FLAG_FSANITIZE_${sanitizer})
                add_compile_options(-fsanitize=${sanitizer})
                link_libraries(-fsanitize=${sanitizer})
            endif()
            unset(CMAKE_REQUIRED_LIBRARIES)
        endforeach()
    endif()
endif()

if(ZUECI_TEST)
    enable_testing()
endif()

if(ZUECI_COVERAGE)
    set(CMAKE_REQUIRED_LIBRARIES -fprofile-arcs)
    check_c_compiler_flag(--coverage C_COMPILER_FLAG_COVERAGE)
    unset(CMAKE_REQUIRED_LIBRARIES)
    if(C_COMPILER_FLAG_COVERAGE)
        add_compile_options(--coverage)
        link_libraries(-fprofile-arcs)

        check_c_compiler_flag(-O0 C_COMPILER_FLAG_O0)
        if(C_COMPILER_FLAG_O0)
            add_compile_options(-O0)
        endif()
    endif()
endif()

set(zueci_SRCS zueci.c)

add_library(zueci SHARED ${zueci_SRCS})

if(WIN32)
    target_sources(zueci PRIVATE libzueci.rc)
endif()

target_include_directories(zueci
    PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
    INTERFACE "$<INSTALL_INTERFACE:include>")

if(ZUECI_STATIC)
    add_library(zueci-static STATIC ${zueci_SRCS})

    target_include_directories(zueci-static
        PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        INTERFACE "$<INSTALL_INTERFACE:include>")
endif()

function(zueci_target_link_libraries library)
    target_link_libraries(zueci ${library})
    if(ZUECI_STATIC)
        target_link_libraries(zueci-static ${library})
    endif()
endfunction()

function(zueci_target_compile_definitions scope definition)
    target_compile_definitions(zueci ${scope} ${definition})
    if(ZUECI_STATIC)
        target_compile_definitions(zueci-static ${scope} ${definition})
    endif()
endfunction()

set_target_properties(zueci
    PROPERTIES SOVERSION "${ZUECI_VERSION_MAJOR}.${ZUECI_VERSION_MINOR}" VERSION ${ZUECI_VERSION})

if(ZUECI_TEST)
    zueci_target_compile_definitions(PUBLIC ZUECI_TEST)
else ()
    add_definitions("-DZUECI_EMBED_NO_TO_ECI")
endif()

if(MSVC)
    target_compile_definitions(zueci PRIVATE DLL_EXPORT)
endif()

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

install(TARGETS zueci ${INSTALL_TARGETS_DEFAULT_ARGS})
if(ZUECI_STATIC)
    install(TARGETS zueci-static ${INSTALL_TARGETS_DEFAULT_ARGS})
endif()
install(FILES zueci.h DESTINATION "${CMAKE_INSTALL_PREFIX}/include" COMPONENT Devel)

if(ZUECI_TEST)
    add_subdirectory(tests)
endif()
