DEPTH	= .

include $(DEPTH)/config.mk

USERLAND_DIR = $(DEPTH)/userland

DIRS = module bcm_host EGL GLESv2 OpenVG wimp_test

include $(DEPTH)/rules.mk

	# Just install all headers regardless of whether we need them.
install:
	mkdir -p $(GCCSDK_INSTALL_ENV)/include/EGL
	cp -f $(USERLAND_DIR)/interface/khronos/include/EGL/* $(GCCSDK_INSTALL_ENV)/include/EGL
	mkdir -p $(GCCSDK_INSTALL_ENV)/include/GLES
	cp -f $(USERLAND_DIR)/interface/khronos/include/GLES/* $(GCCSDK_INSTALL_ENV)/include/GLES
	mkdir -p $(GCCSDK_INSTALL_ENV)/include/GLES2
	cp -f $(USERLAND_DIR)/interface/khronos/include/GLES2/* $(GCCSDK_INSTALL_ENV)/include/GLES2
	mkdir -p $(GCCSDK_INSTALL_ENV)/include/KHR
	cp -f $(USERLAND_DIR)/interface/khronos/include/KHR/* $(GCCSDK_INSTALL_ENV)/include/KHR
	mkdir -p $(GCCSDK_INSTALL_ENV)/include/VG
	cp -f $(USERLAND_DIR)/interface/khronos/include/VG/* $(GCCSDK_INSTALL_ENV)/include/VG
	mkdir -p $(GCCSDK_INSTALL_ENV)/include/WF
	cp -f $(USERLAND_DIR)/interface/khronos/include/WF/* $(GCCSDK_INSTALL_ENV)/include/WF
	cp -f $(USERLAND_DIR)/host_applications/linux/libs/bcm_host/include/bcm_host.h \
			$(GCCSDK_INSTALL_ENV)/include/bcm_host.h
	mkdir -p $(GCCSDK_INSTALL_ENV)/include/interface/vcos
	cp -f $(USERLAND_DIR)/interface/vcos/*.h $(GCCSDK_INSTALL_ENV)/include/interface/vcos
	cp -f $(USERLAND_DIR)/interface/vcos/riscos/*.h \
			$(GCCSDK_INSTALL_ENV)/include/interface/vcos
	mkdir -p $(GCCSDK_INSTALL_ENV)/include/interface/vcos/generic
	cp -f $(USERLAND_DIR)/interface/vcos/generic/*.h \
			$(GCCSDK_INSTALL_ENV)/include/interface/vcos/generic
	mkdir -p $(GCCSDK_INSTALL_ENV)/include/interface/vchiq_arm
	cp -f $(USERLAND_DIR)/interface/vchiq_arm/*.h $(GCCSDK_INSTALL_ENV)/include/interface/vchiq_arm
	mkdir -p $(GCCSDK_INSTALL_ENV)/include/interface/vchi
	cp -f $(USERLAND_DIR)/interface/vchi/*.h $(GCCSDK_INSTALL_ENV)/include/interface/vchi
	mkdir -p $(GCCSDK_INSTALL_ENV)/include/interface/vchi/connections
	cp -f $(USERLAND_DIR)/interface/vchi/connections/*.h \
			$(GCCSDK_INSTALL_ENV)/include/interface/vchi/connections
	mkdir -p $(GCCSDK_INSTALL_ENV)/include/interface/vchi/message_drivers
	cp -f $(USERLAND_DIR)/interface/vchi/message_drivers/*.h \
			$(GCCSDK_INSTALL_ENV)/include/interface/vchi/message_drivers
	mkdir -p $(GCCSDK_INSTALL_ENV)/include/interface/vctypes
	cp -f $(USERLAND_DIR)/interface/vctypes/*.h $(GCCSDK_INSTALL_ENV)/include/interface/vctypes
	mkdir -p $(GCCSDK_INSTALL_ENV)/include/vcinclude
	cp -f $(USERLAND_DIR)/vcinclude/*.h $(GCCSDK_INSTALL_ENV)/include/vcinclude
	mkdir -p $(GCCSDK_INSTALL_ENV)/include/interface/vmcs_host
	cp -f $(USERLAND_DIR)/interface/vmcs_host/*.h $(GCCSDK_INSTALL_ENV)/include/interface/vmcs_host
	mkdir -p $(GCCSDK_INSTALL_ENV)/include/interface/vmcs_host/linux
	cp -f $(USERLAND_DIR)/interface/vmcs_host/linux/*.h \
			$(GCCSDK_INSTALL_ENV)/include/interface/vmcs_host/linux
	mkdir -p $(GCCSDK_INSTALL_ENV)/include/interface/vmcs_host/khronos/IL
	cp -f $(USERLAND_DIR)/interface/vmcs_host/khronos/IL/*.h \
			$(GCCSDK_INSTALL_ENV)/include/interface/vmcs_host/khronos/IL
