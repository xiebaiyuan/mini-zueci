if(NOT EXISTS "/Users/xiebaiyuan/workspace/github/libzueci-1.0.1-src/build_v7/install_manifest.txt")
    message(FATAL_ERROR "Cannot find install manifest: \"/Users/xiebaiyuan/workspace/github/libzueci-1.0.1-src/build_v7/install_manifest.txt\"")
endif()

file(READ "/Users/xiebaiyuan/workspace/github/libzueci-1.0.1-src/build_v7/install_manifest.txt" files)
string(REGEX REPLACE "\n" ";" files "${files}")
foreach(file ${files})
    message(STATUS "Uninstalling \"$ENV{DESTDIR}${file}\"")
    exec_program(
        "/Users/xiebaiyuan/Library/Android/sdk/cmake/3.22.1/bin/cmake" ARGS "-E remove \"$ENV{DESTDIR}${file}\""
        OUTPUT_VARIABLE rm_out
        RETURN_VALUE rm_retval)
    if(NOT "${rm_retval}" STREQUAL 0)
        message(FATAL_ERROR "Problem when removing \"$ENV{DESTDIR}${file}\"")
    endif()
endforeach(file)
