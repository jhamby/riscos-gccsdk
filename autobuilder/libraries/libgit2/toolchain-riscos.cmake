# this one is important
SET(CMAKE_SYSTEM_NAME Generic)
#this one not so much
SET(CMAKE_SYSTEM_VERSION 1)

# specify the cross compiler
SET(CMAKE_C_COMPILER   $ENV{GCCSDK_INSTALL_CROSSBIN}/arm-unknown-riscos-gcc)
SET(CMAKE_CXX_COMPILER $ENV{GCCSDK_INSTALL_CROSSBIN}/arm-unknown-riscos-g++)

# where is the target environment 
SET(CMAKE_FIND_ROOT_PATH  $ENV{GCCSDK_INSTALL_ENV})

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

SET(CMAKE_EXE_LINKER_FLAGS_INIT "-static")

SET(CMAKE_PREFIX_PATH $ENV{GCCSDK_INSTALL_ENV})
SET(PKG_CONFIG_USE_CMAKE_PREFIX_PATH 1)
