# this one is important
SET(CMAKE_SYSTEM_NAME GNU)
SET(CMAKE_SYSTEM_PROCESSOR arm)
#this one not so much
SET(CMAKE_SYSTEM_VERSION 1)

IF(NOT DEFINED CMAKE_C_COMPILER_TARGET)
    SET(CMAKE_C_COMPILER_TARGET arm-unknown-riscos)
ENDIF()
IF(NOT DEFINED CMAKE_CXX_COMPILER_TARGET)
    SET(CMAKE_CXX_COMPILER_TARGET arm-unknown-riscos)
ENDIF()

# specify the cross compiler
SET(CMAKE_C_COMPILER   GCCSDK_BIN/arm-unknown-riscos-gcc)
SET(CMAKE_CXX_COMPILER GCCSDK_BIN/arm-unknown-riscos-g++)

# where is the target environment 
SET(CMAKE_FIND_ROOT_PATH  GCCSDK_ENV)

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

SET(ENV{PKG_CONFIG_LIBDIR} GCCSDK_ENV/lib/pkgconfig:GCCSDK_ENV/share/pkgconfig)
