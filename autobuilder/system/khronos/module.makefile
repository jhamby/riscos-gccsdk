DEPTH	= ..

TARGET	= $(DEPTH)/khronos,ffa

include $(DEPTH)/config.mk

USERLAND_DIR = $(DEPTH)/userland

BCM_HOST_SRC = \
	$(USERLAND_DIR)/host_applications/linux/libs/bcm_host/bcm_host.c

VMCS_HOST_SRC = \
	$(USERLAND_DIR)/interface/vmcs_host/vc_vchi_dispmanx.c \
	$(USERLAND_DIR)/interface/vmcs_host/vc_vchi_gencmd.c \
	$(USERLAND_DIR)/interface/vmcs_host/vc_vchi_tvservice.c \
	$(USERLAND_DIR)/interface/vmcs_host/vc_vchi_cecservice.c \
	$(USERLAND_DIR)/interface/vmcs_host/vc_service_common.c

VCOS_SRC = \
	$(USERLAND_DIR)/interface/vcos/generic/vcos_init.c \
	$(USERLAND_DIR)/interface/vcos/generic/vcos_logcat.c \
	$(USERLAND_DIR)/interface/vcos/generic/vcos_abort.c \
	$(USERLAND_DIR)/interface/vcos/generic/vcos_mem_from_malloc.c \
	$(USERLAND_DIR)/interface/vcos/generic/vcos_generic_named_sem.c \
	$(USERLAND_DIR)/interface/vcos/generic/vcos_generic_blockpool.c \
	$(USERLAND_DIR)/interface/vcos/generic/vcos_msgqueue.c \
	$(USERLAND_DIR)/interface/vcos/generic/vcos_generic_reentrant_mtx.c \
	$(USERLAND_DIR)/interface/vcos/riscos/vcos_threads.c

VCHIQ_SRC = \
	$(USERLAND_DIR)/interface/vchiq_arm/vchiq_lib.c \
	$(USERLAND_DIR)/interface/vchiq_arm/vchiq_util.c
#	$(USERLAND_DIR)/interface/vchiq_arm/vchiq_core.c

KHRONOS_SRC = \
	$(USERLAND_DIR)/interface/khronos/egl/egl_client.c \
	$(USERLAND_DIR)/interface/khronos/egl/egl_client_context.c \
	$(USERLAND_DIR)/interface/khronos/egl/egl_client_surface.c \
	$(USERLAND_DIR)/interface/khronos/egl/egl_client_config.c \
	$(USERLAND_DIR)/interface/khronos/egl/egl_client_get_proc.c \
	$(USERLAND_DIR)/interface/khronos/glxx/glxx_client.c \
	$(USERLAND_DIR)/interface/khronos/vg/vg_client.c \
	$(USERLAND_DIR)/interface/khronos/vg/vg_int_mat3x3.c \
	$(USERLAND_DIR)/interface/khronos/common/khrn_client.c \
	$(USERLAND_DIR)/interface/khronos/common/khrn_client_cache.c \
	$(USERLAND_DIR)/interface/khronos/common/khrn_client_vector.c \
	$(USERLAND_DIR)/interface/khronos/common/khrn_options.c \
	$(USERLAND_DIR)/interface/khronos/common/khrn_client_pointermap.c \
	$(USERLAND_DIR)/interface/khronos/common/khrn_client_global_image_map.c \
	$(USERLAND_DIR)/interface/khronos/common/khrn_int_image.c \
	$(USERLAND_DIR)/interface/khronos/common/khrn_int_hash.c \
	$(USERLAND_DIR)/interface/khronos/common/khrn_int_util.c \
	$(USERLAND_DIR)/interface/khronos/common/linux/khrn_client_rpc_linux.c \
	$(USERLAND_DIR)/interface/khronos/common/linux/khrn_client_platform_linux.c \
	$(USERLAND_DIR)/interface/khronos/ext/egl_brcm_global_image_client.c \
	$(USERLAND_DIR)/interface/khronos/ext/egl_brcm_driver_monitor_client.c \
	$(USERLAND_DIR)/interface/khronos/ext/egl_khr_sync_client.c \
	$(USERLAND_DIR)/interface/khronos/ext/egl_khr_image_client.c \
	$(USERLAND_DIR)/interface/khronos/ext/egl_khr_lock_surface_client.c \
	$(USERLAND_DIR)/interface/khronos/ext/gl_oes_draw_texture_client.c \
	$(USERLAND_DIR)/interface/khronos/ext/gl_oes_egl_image_client.c \
	$(USERLAND_DIR)/interface/khronos/ext/gl_oes_matrix_palette_client.c \
	$(USERLAND_DIR)/interface/khronos/ext/gl_oes_map_buffer.c \
	$(USERLAND_DIR)/interface/khronos/ext/gl_oes_framebuffer_object.c \
	$(USERLAND_DIR)/interface/khronos/ext/gl_oes_query_matrix_client.c \
	$(USERLAND_DIR)/interface/khronos/ext/egl_brcm_flush_client.c \
	$(USERLAND_DIR)/interface/khronos/ext/ext_gl_debug_marker.c

MODULE_SRC = main.c swi_vc.c swi_egl.c swi_gl.c swi_openvg.c

CSRCS	= \
	$(MODULE_SRC) \
	$(BCM_HOST_SRC) \
	$(VMCS_HOST_SRC) \
	$(VCOS_SRC) \
	$(VCHIQ_SRC) \
	$(KHRONOS_SRC)

ASMSRCS = \
	$(USERLAND_DIR)/interface/khronos/common/khrn_int_hash_asm.s \
	asm.s

MODULE_OBJECT = khronos.o
MODULE_INCLUDE = khronos.h
MODULE_CMHG = khronos.cmhg

EXTRA_TARGET_DEPS = $(MODULE_INCLUDE)

INCLUDES += \
	-I. \
	-I$(USERLAND_DIR) \
	-I$(USERLAND_DIR)/interface/vcos \
	-I$(USERLAND_DIR)/interface/vcos/riscos \
	-I$(USERLAND_DIR)/interface/vmcs_host/linux \
	-I$(USERLAND_DIR)/interface/vchiq_arm \
	-I$(USERLAND_DIR)/interface/khronos/include \
	-I$(USERLAND_DIR)/interface/khronos \
	-I$(USERLAND_DIR)/host_applications/linux/libs/bcm_host/include

include $(DEPTH)/rules.mk
