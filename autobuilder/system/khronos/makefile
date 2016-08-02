DEPTH	= .

include $(DEPTH)/config.mk

USERLAND_DIR = $(DEPTH)/userland

DIRS = module bcm_host EGL GLESv2 OpenVG wimp_test

include $(DEPTH)/rules.mk

	# Just install all headers regardless of whether we need them.
install:
	mkdir -p $(INSTALL_DIR)/include/EGL
	cp -f $(USERLAND_DIR)/interface/khronos/include/EGL/* $(INSTALL_DIR)/include/EGL
	mkdir -p $(INSTALL_DIR)/include/GLES
	cp -f $(USERLAND_DIR)/interface/khronos/include/GLES/* $(INSTALL_DIR)/include/GLES
	mkdir -p $(INSTALL_DIR)/include/GLES2
	cp -f $(USERLAND_DIR)/interface/khronos/include/GLES2/* $(INSTALL_DIR)/include/GLES2
	mkdir -p $(INSTALL_DIR)/include/KHR
	cp -f $(USERLAND_DIR)/interface/khronos/include/KHR/* $(INSTALL_DIR)/include/KHR
	mkdir -p $(INSTALL_DIR)/include/VG
	cp -f $(USERLAND_DIR)/interface/khronos/include/VG/* $(INSTALL_DIR)/include/VG
	mkdir -p $(INSTALL_DIR)/include/WF
	cp -f $(USERLAND_DIR)/interface/khronos/include/WF/* $(INSTALL_DIR)/include/WF
	cp -f $(USERLAND_DIR)/host_applications/linux/libs/bcm_host/include/bcm_host.h \
			$(INSTALL_DIR)/include/bcm_host.h
	mkdir -p $(INSTALL_DIR)/include/interface/vcos
	cp -f $(USERLAND_DIR)/interface/vcos/*.h $(INSTALL_DIR)/include/interface/vcos
	cp -f $(USERLAND_DIR)/interface/vcos/riscos/*.h \
			$(INSTALL_DIR)/include/interface/vcos
	# Not the private one:
	rm -f $(INSTALL_DIR)/include/interface/vcos/riscosbits_priv.h
	mkdir -p $(INSTALL_DIR)/include/interface/vcos/generic
	cp -f $(USERLAND_DIR)/interface/vcos/generic/*.h \
			$(INSTALL_DIR)/include/interface/vcos/generic
	mkdir -p $(INSTALL_DIR)/include/interface/vchiq_arm
	cp -f $(USERLAND_DIR)/interface/vchiq_arm/*.h $(INSTALL_DIR)/include/interface/vchiq_arm
	# Don't need to make this one public:
	rm -f $(INSTALL_DIR)/include/interface/vchiq_arm/vchiq_module.h
	mkdir -p $(INSTALL_DIR)/include/interface/vchi
	cp -f $(USERLAND_DIR)/interface/vchi/*.h $(INSTALL_DIR)/include/interface/vchi
	mkdir -p $(INSTALL_DIR)/include/interface/vchi/connections
	cp -f $(USERLAND_DIR)/interface/vchi/connections/*.h \
			$(INSTALL_DIR)/include/interface/vchi/connections
	mkdir -p $(INSTALL_DIR)/include/interface/vchi/message_drivers
	cp -f $(USERLAND_DIR)/interface/vchi/message_drivers/*.h \
			$(INSTALL_DIR)/include/interface/vchi/message_drivers
	mkdir -p $(INSTALL_DIR)/include/interface/vctypes
	cp -f $(USERLAND_DIR)/interface/vctypes/*.h $(INSTALL_DIR)/include/interface/vctypes
	mkdir -p $(INSTALL_DIR)/include/vcinclude
	cp -f $(USERLAND_DIR)/vcinclude/*.h $(INSTALL_DIR)/include/vcinclude
	mkdir -p $(INSTALL_DIR)/include/interface/vmcs_host
	cp -f $(USERLAND_DIR)/interface/vmcs_host/*.h $(INSTALL_DIR)/include/interface/vmcs_host
	mkdir -p $(INSTALL_DIR)/include/interface/vmcs_host/linux
	cp -f $(USERLAND_DIR)/interface/vmcs_host/linux/*.h \
			$(INSTALL_DIR)/include/interface/vmcs_host/linux
	mkdir -p $(INSTALL_DIR)/include/interface/vmcs_host/khronos/IL
	cp -f $(USERLAND_DIR)/interface/vmcs_host/khronos/IL/*.h \
			$(INSTALL_DIR)/include/interface/vmcs_host/khronos/IL
	mkdir -p $(INSTALL_DIR)/include/interface/peer
	cp -f $(USERLAND_DIR)/interface/peer/*.h $(INSTALL_DIR)/include/interface/peer
