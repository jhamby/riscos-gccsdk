DEPTH	= ..

TARGET	= $(DEPTH)/libGLESv2

include $(DEPTH)/config.mk

ASMSRCS = api-asm.s
CSRCS = api.c

USERLAND_DIR = $(DEPTH)/userland

INCLUDES += \
	-I. \
	-I$(USERLAND_DIR)/interface/vcos \
	-I$(USERLAND_DIR)/interface/vcos/riscos \
	-I$(USERLAND_DIR)/interface/khronos/include \
	-I$(USERLAND_DIR)

include $(DEPTH)/rules.mk
 

install:
	cp -f ../libGLESv2.* $(GCCSDK_INSTALL_ENV)/lib
