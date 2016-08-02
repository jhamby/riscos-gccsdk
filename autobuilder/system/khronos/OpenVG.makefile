DEPTH	= ..

TARGET	= $(DEPTH)/libOpenVG

include $(DEPTH)/config.mk

USERLAND_DIR = $(DEPTH)/userland

ASMSRCS = api-asm.s
CSRCS = api.c vg_int_mat3x3.c

INCLUDES += \
	-I. \
	-I$(USERLAND_DIR)/interface/vcos \
	-I$(USERLAND_DIR)/interface/vcos/riscos \
	-I$(USERLAND_DIR)/interface/khronos/include \
	-I$(USERLAND_DIR)

include $(DEPTH)/rules.mk
 

install:
	cp -f ../libOpenVG.* $(INSTALL_DIR)/lib
