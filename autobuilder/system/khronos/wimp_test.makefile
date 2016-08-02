DEPTH	= ..

BIN_TARGET	= !RunImage,e1f

include $(DEPTH)/config.mk

USERLAND_DIR = $(DEPTH)/userland

CSRCS = main.c

INCLUDES += \
	-I. \
	-I$(USERLAND_DIR)/interface/vcos \
	-I$(USERLAND_DIR)/interface/vmcs_host/linux \
	-I$(USERLAND_DIR)/interface/vcos/riscos \
	-I$(USERLAND_DIR)/interface/khronos/include \
	-I$(USERLAND_DIR)/host_applications/linux/libs/bcm_host/include \
	-I$(USERLAND_DIR) \
	-I$(GCCSDK_INSTALL_ENV)/vfp/include -I$(GCCSDK_INSTALL_ENV)/include

SYSTEM_BIN_LIBS += -L../ -lbcm_host -lEGL -lGLESv2

include $(DEPTH)/rules.mk
