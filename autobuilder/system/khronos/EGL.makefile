DEPTH	= ..

TARGET	= $(DEPTH)/libEGL

include $(DEPTH)/config.mk

ASMSRCS = api-asm.s
CSRCS =  egl_client_get_proc.c

USERLAND_DIR = $(DEPTH)/userland

INCLUDES += \
	-I. \
	-I$(USERLAND_DIR)/interface/vcos \
	-I$(USERLAND_DIR)/interface/vcos/riscos \
	-I$(USERLAND_DIR)/interface/khronos/include \
	-I$(USERLAND_DIR)

include $(DEPTH)/rules.mk
 

install:
	cp -f ../libEGL.* $(INSTALL_DIR)/lib
