#VPATH	= $(COMPILER)

LIBFILE		=
LINK		= arm-unknown-riscos-gcc
ASM		= arm-unknown-riscos-gcc
CC		= arm-unknown-riscos-gcc
AR		= arm-unknown-riscos-ar

OS_MOD_ASMFLAGS	= -c -mmodule -mlibscl -x assembler-with-cpp -D__ASSEMBLY__
OS_SHLIB_ASMFLAGS = -c -x assembler-with-cpp -D__ASSEMBLY__ -mfpu=vfp
OS_LIB_ASMFLAGS	= -c -x assembler-with-cpp -D__ASSEMBLY__ -mfpu=vfp
OS_MOD_CFLAGS	= -c -mmodule -Wall -std=c99 -fgnu89-inline
OS_SHLIB_CFLAGS	= -c -fPIC -Wall -std=c99 -fgnu89-inline -mfpu=vfp
OS_LIB_CFLAGS	= -c -Wall -std=c99 -fgnu89-inline -mfpu=vfp

INCLUDES	=

OPTIMISE	= -O3

SYSTEM_MOD_LIBS	= -L$(GCCSDK_INSTALL_ENV)/lib -lOSLib32 -mlibscl
SYSTEM_LIB_LIBS	= -L$(GCCSDK_INSTALL_ENV)/lib
SYSTEM_BIN_LIBS	= -L$(GCCSDK_INSTALL_ENV)/lib

DEFINES		= -D_GNU_SOURCE -DUSE_VCHIQ_ARM -DEGL_SERVER_DISPMANX

ASM_INCLUDES	=

MOD_CFLAGS	+= $(OS_MOD_CFLAGS) $(DEFINES) $(INCLUDES) $(OPTIMISE)
SHLIB_CFLAGS	+= $(OS_SHLIB_CFLAGS) $(DEFINES) $(INCLUDES) $(OPTIMISE)
LIB_CFLAGS	+= $(OS_LIB_CFLAGS) $(DEFINES) $(INCLUDES) $(OPTIMISE)
CPPFLAGS	+= $(OS_CPPFLAGS) $(DEFINES) $(INCLUDES) $(OPTIMISE)
MOD_ASMFLAGS	+= $(OS_MOD_ASMFLAGS) $(ASM_DEFINES) $(ASM_INCLUDES)
SHLIB_ASMFLAGS	+= $(OS_SHLIB_ASMFLAGS) $(ASM_DEFINES) $(ASM_INCLUDES)
LIB_ASMFLAGS	+= $(OS_LIB_ASMFLAGS) $(ASM_DEFINES) $(ASM_INCLUDES)
CMUNGEFLAGS	= -tgcc -32bit -p

TEMP_DIR	=
