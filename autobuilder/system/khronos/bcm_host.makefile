DEPTH	= ..

TARGET	= $(DEPTH)/libbcm_host

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

SONAME = libbcm_host.so

include $(DEPTH)/rules.mk
 
install:
	cp -f ../libbcm_host.* $(INSTALL_DIR)/lib
