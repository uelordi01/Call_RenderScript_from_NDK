# Sets the minimum version of CMake required to build the native
# library. You should either keep the default value or only pass a
# value of 3.4.0 or lower.

cmake_minimum_required(VERSION 3.4.1)
add_library(cpufeatures STATIC
            ${ANDROID_NDK}/sources/android/cpufeatures/cpu-features.c)
# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds it for you.
# Gradle automatically packages shared libraries with your APK.

SET(RENDER_SCRIPT_HEADERS_PATH
 ${ANDROID_NDK}/toolchains/renderscript/prebuilt/${ANDROID_HOST_TAG}/platform/rs
)
SET(ANDROID_TOOLCHAIN gcc)
INCLUDE_DIRECTORIES(
    ${RENDER_SCRIPT_HEADERS_PATH}/cpp
    ${RENDER_SCRIPT_HEADERS_PATH}/scriptc
    ${RENDER_SCRIPT_HEADERS_PATH}
    src/main/cpp/ScriptC_mono.h
    src/main/cpp
     ${ANDROID_NDK}/sources/android/cpufeatures
    )

link_directories( ${ANDROID_NDK}/toolchains/renderscript/prebuilt/${ANDROID_HOST_TAG}/platform/${ANDROID_SYSROOT_ABI})

add_library( # Sets the name of the library.
             native-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             # Associated headers in the same location as their source
             # file are automatically included.

             src/main/cpp/ScriptC_mono.cpp
             src/main/cpp/native-lib.cpp
             src/main/rs/mono.rs
             )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because system libraries are included in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )



# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in the
# build script, prebuilt third-party libraries, or system libraries.

MESSAGE(STATUS " -----------------------------------------------------")
MESSAGE(STATUS "    CURRENT SOURCE DIR -> ${CMAKE_CURRENT_SOURCE_DIR}")
MESSAGE(STATUS "    CMAKE_TOOLCHAIN_FILE -> ${CMAKE_TOOLCHAIN_FILE}")
MESSAGE(STATUS "    CURRENT_ABI -> ${ANDROID_ABI}")
MESSAGE(STATUS "    ANDROID_NDK -> ${ANDROID_NDK}")
MESSAGE(STATUS "    ANDROID_NATIVE_API_LEVEL -> ${ANDROID_NATIVE_API_LEVEL}")
MESSAGE(STATUS "    INCLUDE_DIRECTORIES -> ${RENDER_SCRIPT_HEADERS_PATH}")
MESSAGE(STATUS " -----------------------------------------------------")

target_link_libraries( # Specifies the target library.
                       native-lib
                       RScpp_static
                       cpufeatures
                       dl
                       jnigraphics
                       ${log-lib} )
