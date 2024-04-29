
cmake_minimum_required(VERSION 3.22.1)

# 1. 添加内联支持
enable_language(C ASM)


project("detect")

# 2. 根據架構選擇匯編文件 (可選, 主要是為了兼容)
if (${CMAKE_ANDROID_ARCH_ABI} STREQUAL "arm64-v8a")
    set(syscall syscall64.s)
elseif (${CMAKE_ANDROID_ARCH_ABI} STREQUAL "armeabi-v7a")
    set(syscall syscall32.s)
endif ()

# 3. 添加到當前so中
add_library(${CMAKE_PROJECT_NAME} SHARED
        # List C/C++ source files with relative paths to this CMakeLists.txt.
        native-lib.cpp
        detect-crc32.cpp
        elf-parser.c

        ${syscall}
        )


target_link_libraries(${CMAKE_PROJECT_NAME}
        # List libraries link to the target library
        z
        android
        log)