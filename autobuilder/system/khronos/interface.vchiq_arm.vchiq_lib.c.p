--- interface/vchiq_arm/vchiq_lib.c.orig	2016-05-28 10:25:01.527146917 +0100
+++ interface/vchiq_arm/vchiq_lib.c	2016-05-28 10:25:01.795146925 +0100
@@ -30,6 +30,13 @@
 #include <sys/ioctl.h>
 #include <stdio.h>
 
+#ifdef __riscos__
+#include <kernel.h>
+#include <stdint.h>
+#include "main.h"
+#include "../inline_swis.h"
+#endif
+
 #include "vchiq.h"
 #include "vchiq_cfg.h"
 #include "vchiq_ioctl.h"
@@ -38,13 +45,23 @@
 #include "interface/vcos/vcos.h"
 
 #define IS_POWER_2(x) ((x & (x - 1)) == 0)
-#define VCHIQ_MAX_INSTANCE_SERVICES 32
+//#define VCHIQ_MAX_INSTANCE_SERVICES 32
 #define MSGBUF_SIZE (VCHIQ_MAX_MSG_SIZE + sizeof(VCHIQ_HEADER_T))
 
+#ifdef __riscos__
+#include "vchiq_module.h"
+
+#define SERVICE_ADD(service) \
+  globals->services[((SHIM_SERVICE_T *)service)->handle & (VCHIQ_MAX_INSTANCE_SERVICES - 1)] = service;
+#define SERVICE_REMOVE(service) \
+  globals->services[((SHIM_SERVICE_T *)service)->handle & (VCHIQ_MAX_INSTANCE_SERVICES - 1)] = 0;
+#endif
+
 #define RETRY(r,x) do { r = x; } while ((r == -1) && (errno == EINTR))
 
 #define VCOS_LOG_CATEGORY (&vchiq_lib_log_category)
 
+#ifndef __riscos__
 typedef struct vchiq_service_struct
 {
    VCHIQ_SERVICE_BASE_T base;
@@ -73,13 +90,15 @@
 } vchiq_instance;
 
 typedef struct vchiq_instance_struct VCHI_STATE_T;
+#endif
+
 
 /* Local data */
 static VCOS_LOG_LEVEL_T vchiq_default_lib_log_level = VCOS_LOG_WARN;
 static VCOS_LOG_CAT_T vchiq_lib_log_category;
-static VCOS_MUTEX_T vchiq_lib_mutex;
-static void *free_msgbufs;
-static unsigned int handle_seq;
+//static VCOS_MUTEX_T vchiq_lib_mutex;
+//static void *free_msgbufs;
+//static unsigned int handle_seq;
 
 vcos_static_assert(IS_POWER_2(VCHIQ_MAX_INSTANCE_SERVICES));
 
@@ -87,7 +106,9 @@
 static VCHIQ_INSTANCE_T
 vchiq_lib_init(void);
 
+#ifndef __riscos__
 static void *completion_thread(void *);
+#endif
 
 static VCHIQ_STATUS_T
 create_service(VCHIQ_INSTANCE_T instance,
@@ -96,6 +117,7 @@
    int is_open,
    VCHIQ_SERVICE_HANDLE_T *phandle);
 
+#ifndef __riscos__
 static int
 fill_peek_buf(VCHI_SERVICE_T *service,
    VCHI_FLAGS_T flags);
@@ -105,22 +127,26 @@
 
 static void
 free_msgbuf(void *buf);
+#endif
 
 static __inline int
 is_valid_instance(VCHIQ_INSTANCE_T instance)
 {
-   return (instance == &vchiq_instance) && (instance->initialised > 0);
+   return (instance == vchiq_instance) && (instance->state->initialised > 0);
 }
 
 static inline VCHIQ_SERVICE_T *
 handle_to_service(VCHIQ_SERVICE_HANDLE_T handle)
 {
-   return &vchiq_instance.services[handle & (VCHIQ_MAX_INSTANCE_SERVICES - 1)];
+   return vchiq_instance->state->services[handle & (VCHIQ_MAX_INSTANCE_SERVICES - 1)];
 }
 
 static VCHIQ_SERVICE_T *
 find_service_by_handle(VCHIQ_SERVICE_HANDLE_T handle)
 {
+#ifdef __riscos__
+   return (VCHIQ_SERVICE_T *)handle;
+#else
    VCHIQ_SERVICE_T *service;
 
    service = handle_to_service(handle);
@@ -131,6 +157,7 @@
       vcos_log_info("Invalid service handle 0x%x", handle);
 
    return service;
+#endif
 }
 
 /*
@@ -154,6 +181,12 @@
 VCHIQ_STATUS_T
 vchiq_shutdown(VCHIQ_INSTANCE_T instance)
 {
+  int i;
+
+  for (i = 0; i < VCHIQ_MAX_INSTANCE_SERVICES; i++)
+     if (globals->services[i])
+	vchi_service_destroy(globals->services[i]);
+#ifndef __riscos__
    vcos_log_trace( "%s called", __func__ );
 
    if (!is_valid_instance(instance))
@@ -208,7 +241,7 @@
    vcos_global_unlock();
 
    vcos_log_trace( "%s returning", __func__ );
-
+#endif
    return VCHIQ_SUCCESS;
 }
 
@@ -216,19 +249,29 @@
 vchiq_connect(VCHIQ_INSTANCE_T instance)
 {
    VCHIQ_STATUS_T status = VCHIQ_SUCCESS;
-   VCOS_THREAD_ATTR_T attrs;
-   int ret;
 
    vcos_log_trace( "%s called", __func__ );
 
    if (!is_valid_instance(instance))
       return VCHIQ_ERROR;
 
+#ifndef __riscos__
    vcos_mutex_lock(&instance->mutex);
+#endif
 
    if (instance->connected)
       goto out;
 
+#ifdef __riscos__
+   if (swi_vchiq_connect(instance) != NULL)
+   {
+      vcos_log_error("RISC OS: Error returned by SWI in '%s'", __func__);
+      status = VCHIQ_ERROR;
+      goto out;
+   }
+#else
+   VCOS_THREAD_ATTR_T attrs;
+   int ret;
    ret = ioctl(instance->fd, VCHIQ_IOC_CONNECT, 0);
    if (ret != 0)
    {
@@ -245,9 +288,12 @@
    }
 
    instance->connected = 1;
+#endif
 
 out:
+#ifndef __riscos__
    vcos_mutex_unlock(&instance->mutex);
+#endif
    return status;
 }
 
@@ -288,8 +334,6 @@
    const VCHIQ_SERVICE_PARAMS_T *params,
    VCHIQ_SERVICE_HANDLE_T *phandle)
 {
-   VCHIQ_STATUS_T status;
-
    vcos_log_trace( "%s called fourcc = 0x%08x (%c%c%c%c)",
                    __func__,
                    params->fourcc,
@@ -304,20 +348,56 @@
    if (!is_valid_instance(instance))
       return VCHIQ_ERROR;
 
+#ifdef __riscos__
+   SERVICE_CREATION_T setup;
+
+   setup.version.version = params->version;
+   setup.version.version_min = params->version_min;
+   setup.service_id = params->fourcc;
+   setup.connection = 0;
+   setup.rx_fifo_size = 0;
+   setup.tx_fifo_size = 0;
+   setup.callback = params->callback;
+   setup.callback_param = params->userdata;
+   setup.want_unaligned_bulk_rx = 0;
+   setup.want_unaligned_bulk_tx = 0;
+   setup.want_crc = 0;
+
+   if (swi_vchiq_service_open(instance, &setup, phandle) != NULL)
+   {
+      vcos_log_error("RISC OS: Error returned by SWI in '%s'", __func__);
+      return VCHIQ_ERROR;
+   }
+
+   SERVICE_ADD(*phandle);
+
+   vcos_log_trace( "%s returning service handle = 0x%08x", __func__, (uint32_t)*phandle );
+
+   return VCHIQ_SUCCESS;
+#else
+   VCHIQ_STATUS_T status;
    status = create_service(instance,
       params,
       NULL/*vchi_callback*/,
       1/*open*/,
       phandle);
-
    vcos_log_trace( "%s returning service handle = 0x%08x", __func__, (uint32_t)*phandle );
 
    return status;
+#endif
 }
 
 VCHIQ_STATUS_T
 vchiq_close_service(VCHIQ_SERVICE_HANDLE_T handle)
 {
+#ifdef __riscos__
+   vcos_log_trace( "%s called service handle = 0x%08x", __func__, (uint32_t)handle );
+
+   if (!handle)
+      return VCHIQ_ERROR;
+
+   return (swi_vchiq_service_close(handle) == NULL) ? VCHIQ_SUCCESS : VCHIQ_ERROR;
+#else
    VCHIQ_SERVICE_T *service = find_service_by_handle(handle);
    int ret;
 
@@ -335,11 +415,20 @@
       return VCHIQ_ERROR;
 
    return VCHIQ_SUCCESS;
+#endif
 }
 
 VCHIQ_STATUS_T
 vchiq_remove_service(VCHIQ_SERVICE_HANDLE_T handle)
 {
+#ifdef __riscos__
+   vcos_log_trace( "%s called service handle = 0x%08x", __func__, (uint32_t)handle );
+
+   if (!handle)
+      return VCHIQ_ERROR;
+
+   return (swi_vchiq_service_release(handle) == NULL) ? VCHIQ_SUCCESS : VCHIQ_ERROR;
+#else
    VCHIQ_SERVICE_T *service = find_service_by_handle(handle);
    int ret;
 
@@ -356,6 +445,7 @@
       return VCHIQ_ERROR;
 
    return VCHIQ_SUCCESS;
+#endif
 }
 
 VCHIQ_STATUS_T
@@ -363,6 +453,16 @@
    const VCHIQ_ELEMENT_T *elements,
    int count)
 {
+#ifdef __riscos__
+   vcos_log_trace( "%s called service handle = 0x%08x", __func__, (uint32_t)handle );
+
+   if (!handle)
+      return VCHIQ_ERROR;
+
+   return (swi_vchiq_msg_queue(handle, elements->data, elements->size,
+			       VCHI_FLAGS_BLOCK_UNTIL_QUEUED) == NULL ? VCHIQ_SUCCESS :
+									VCHIQ_ERROR);
+#else
    VCHIQ_SERVICE_T *service = find_service_by_handle(handle);
    VCHIQ_QUEUE_MESSAGE_T args;
    int ret;
@@ -378,6 +478,7 @@
    RETRY(ret, ioctl(service->fd, VCHIQ_IOC_QUEUE_MESSAGE, &args));
 
    return (ret >= 0) ? VCHIQ_SUCCESS : VCHIQ_ERROR;
+#endif
 }
 
 void
@@ -386,7 +487,11 @@
 {
    vcos_log_trace( "%s handle=%08x, header=%x", __func__, (uint32_t)handle, (uint32_t)header );
 
+#ifdef __riscos__
+   free(header);
+#else
    free_msgbuf(header);
+#endif
 }
 
 VCHIQ_STATUS_T
@@ -395,6 +500,17 @@
    int size,
    void *userdata)
 {
+#ifdef __riscos__
+   vcos_log_trace( "%s called service handle = 0x%08x", __func__, (uint32_t)handle );
+
+   if (!handle)
+      return VCHIQ_ERROR;
+
+   return (swi_vchiq_queue_bulk_transmit(handle, data, size,
+					 VCHI_FLAGS_CALLBACK_WHEN_OP_COMPLETE |
+					   VCHI_FLAGS_BLOCK_UNTIL_QUEUED,
+					 userdata) == NULL ? VCHIQ_SUCCESS : VCHIQ_ERROR);
+#else
    VCHIQ_SERVICE_T *service = find_service_by_handle(handle);
    VCHIQ_QUEUE_BULK_TRANSFER_T args;
    int ret;
@@ -412,6 +528,7 @@
    RETRY(ret, ioctl(service->fd, VCHIQ_IOC_QUEUE_BULK_TRANSMIT, &args));
 
    return (ret >= 0) ? VCHIQ_SUCCESS : VCHIQ_ERROR;
+#endif
 }
 
 VCHIQ_STATUS_T
@@ -420,6 +537,9 @@
    int size,
    void *userdata)
 {
+#ifdef __riscos__
+   return VCHIQ_ERROR;
+#else
    VCHIQ_SERVICE_T *service = find_service_by_handle(handle);
    VCHIQ_QUEUE_BULK_TRANSFER_T args;
    int ret;
@@ -437,6 +557,7 @@
    RETRY(ret, ioctl(service->fd, VCHIQ_IOC_QUEUE_BULK_RECEIVE, &args));
 
    return (ret >= 0) ? VCHIQ_SUCCESS : VCHIQ_ERROR;
+#endif
 }
 
 VCHIQ_STATUS_T
@@ -474,6 +595,30 @@
    void *userdata,
    VCHIQ_BULK_MODE_T mode)
 {
+#ifdef __riscos__
+   vcos_log_trace( "%s called service handle = 0x%08x", __func__, (uint32_t)handle );
+
+   if (!handle)
+      return VCHIQ_ERROR;
+
+   VCHI_FLAGS_T flags;
+   switch (mode)
+   {
+     case VCHIQ_BULK_MODE_CALLBACK:
+       flags = VCHI_FLAGS_CALLBACK_WHEN_OP_COMPLETE | VCHI_FLAGS_BLOCK_UNTIL_QUEUED;
+       break;
+     case VCHIQ_BULK_MODE_BLOCKING:
+       flags = VCHI_FLAGS_BLOCK_UNTIL_DATA_READ;
+       break;
+     case VCHIQ_BULK_MODE_NOCALLBACK:
+     default:
+       flags = VCHI_FLAGS_NONE;
+       break;
+   }
+
+   return (swi_vchiq_queue_bulk_transmit(handle, data, size, flags, userdata) == NULL ?
+		VCHIQ_SUCCESS : VCHIQ_ERROR);
+#else
    VCHIQ_SERVICE_T *service = find_service_by_handle(handle);
    VCHIQ_QUEUE_BULK_TRANSFER_T args;
    int ret;
@@ -491,6 +636,7 @@
    RETRY(ret, ioctl(service->fd, VCHIQ_IOC_QUEUE_BULK_TRANSMIT, &args));
 
    return (ret >= 0) ? VCHIQ_SUCCESS : VCHIQ_ERROR;
+#endif
 }
 
 VCHIQ_STATUS_T
@@ -525,6 +671,9 @@
    VCHIQ_BULK_MODE_T mode,
    int (*copy_pagelist)())
 {
+#ifdef __riscos__
+   return VCHIQ_ERROR;
+#else
    VCHIQ_SERVICE_T *service = find_service_by_handle(handle);
    VCHIQ_QUEUE_BULK_TRANSFER_T args;
    int ret;
@@ -544,17 +693,22 @@
    RETRY(ret, ioctl(service->fd, VCHIQ_IOC_QUEUE_BULK_RECEIVE, &args));
 
    return (ret >= 0) ? VCHIQ_SUCCESS : VCHIQ_ERROR;
+#endif
 }
 
 int
 vchiq_get_client_id(VCHIQ_SERVICE_HANDLE_T handle)
 {
+#ifdef __riscos__
+   return VCHIQ_ERROR;
+#else
    VCHIQ_SERVICE_T *service = find_service_by_handle(handle);
 
    if (!service)
       return VCHIQ_ERROR;
 
    return ioctl(service->fd, VCHIQ_IOC_GET_CLIENT_ID, service->handle);
+#endif
 }
 
 void *
@@ -578,6 +732,9 @@
    int config_size,
    VCHIQ_CONFIG_T *pconfig)
 {
+#ifdef __riscos__
+   return VCHIQ_ERROR;
+#else
    VCHIQ_GET_CONFIG_T args;
    int ret;
 
@@ -590,11 +747,18 @@
    RETRY(ret, ioctl(instance->fd, VCHIQ_IOC_GET_CONFIG, &args));
 
    return (ret >= 0) ? VCHIQ_SUCCESS : VCHIQ_ERROR;
+#endif
 }
 
 int32_t
 vchiq_use_service( const VCHIQ_SERVICE_HANDLE_T handle )
 {
+#ifdef __riscos__
+   if (!handle)
+      return VCHIQ_ERROR;
+
+   return (swi_vchiq_service_use(handle) == NULL ? 0 : -1);
+#else
    VCHIQ_SERVICE_T *service = find_service_by_handle(handle);
    int ret;
 
@@ -603,11 +767,18 @@
 
    RETRY(ret,ioctl(service->fd, VCHIQ_IOC_USE_SERVICE, service->handle));
    return ret;
+#endif
 }
 
 int32_t
 vchiq_release_service( const VCHIQ_SERVICE_HANDLE_T handle )
 {
+#ifdef __riscos__
+   if (!handle)
+      return VCHIQ_ERROR;
+
+   return (swi_vchiq_service_release(handle) == NULL ? 0 : -1);
+#else
    VCHIQ_SERVICE_T *service = find_service_by_handle(handle);
    int ret;
 
@@ -616,12 +787,16 @@
 
    RETRY(ret,ioctl(service->fd, VCHIQ_IOC_RELEASE_SERVICE, service->handle));
    return ret;
+#endif
 }
 
 VCHIQ_STATUS_T
 vchiq_set_service_option(VCHIQ_SERVICE_HANDLE_T handle,
    VCHIQ_SERVICE_OPTION_T option, int value)
 {
+#ifdef __riscos__
+   return VCHIQ_ERROR;
+#else
    VCHIQ_SET_SERVICE_OPTION_T args;
    VCHIQ_SERVICE_T *service = find_service_by_handle(handle);
    int ret;
@@ -636,6 +811,7 @@
    RETRY(ret, ioctl(service->fd, VCHIQ_IOC_SET_SERVICE_OPTION, &args));
 
    return (ret >= 0) ? VCHIQ_SUCCESS : VCHIQ_ERROR;
+#endif
 }
 
 /*
@@ -667,6 +843,11 @@
    vcos_unused(function_table);
    vcos_unused(low_level);
 
+// Returning NULL here *may* be a bad idea.
+//   VCHI_CONNECTION_T *t = malloc(sizeof(VCHI_CONNECTION_T));
+//   memset(t, 0xfc, sizeof(VCHI_CONNECTION_T));
+//   return t;
+
    return NULL;
 }
 
@@ -690,6 +871,14 @@
    uint32_t *msg_size,
    VCHI_FLAGS_T flags )
 {
+#ifdef __riscos__
+   if (swi_vchiq_msg_peek(handle, flags, data, msg_size) != NULL)
+   {
+      vcos_log_error("RISC OS: Error returned by SWI in '%s'", __func__);
+      return -1;
+   }
+   return 0;
+#else
    VCHI_SERVICE_T *service = find_service_by_handle(handle);
    int ret;
 
@@ -705,6 +894,7 @@
    }
 
    return ret;
+#endif
 }
 
 /***********************************************************
@@ -720,6 +910,12 @@
 int32_t
 vchi_msg_remove( VCHI_SERVICE_HANDLE_T handle )
 {
+#ifdef __riscos__
+   if (!handle)
+      return VCHIQ_ERROR;
+
+   return (swi_vchiq_msg_remove(handle) == NULL ? 0 : -1);
+#else
    VCHI_SERVICE_T *service = find_service_by_handle(handle);
 
    if (!service)
@@ -732,6 +928,7 @@
    service->peek_size = -1;
 
    return 0;
+#endif
 }
 
 /***********************************************************
@@ -755,6 +952,17 @@
    VCHI_FLAGS_T flags,
    void * msg_handle )
 {
+#ifdef __riscos__
+   VCHIQ_ELEMENT_T element = {data, data_size};
+
+   vcos_unused(msg_handle);
+   vcos_assert(flags == VCHI_FLAGS_BLOCK_UNTIL_QUEUED);
+
+   if (!handle)
+      return VCHIQ_ERROR;
+
+   return (swi_vchiq_msg_queue(handle, &element, 1, flags) == NULL ? 0 : -1);
+#else
    VCHI_SERVICE_T *service = find_service_by_handle(handle);
    VCHIQ_QUEUE_MESSAGE_T args;
    VCHIQ_ELEMENT_T element = {data, data_size};
@@ -772,6 +980,7 @@
    RETRY(ret, ioctl(service->fd, VCHIQ_IOC_QUEUE_MESSAGE, &args));
 
    return ret;
+#endif
 }
 
 /***********************************************************
@@ -795,6 +1004,10 @@
    VCHI_FLAGS_T flags,
    void * bulk_handle )
 {
+#ifdef __riscos__
+   fprintf(stderr, "[%s:%d:%s] - forced to return VCHIQ_ERROR\n", __func__, __LINE__, __FILE__);
+   return VCHIQ_ERROR;
+#else
    VCHI_SERVICE_T *service = find_service_by_handle(handle);
    VCHIQ_QUEUE_BULK_TRANSFER_T args;
    int ret;
@@ -825,6 +1038,7 @@
    RETRY(ret, ioctl(service->fd, VCHIQ_IOC_QUEUE_BULK_RECEIVE, &args));
 
    return ret;
+#endif
 }
 
 /***********************************************************
@@ -848,6 +1062,16 @@
    VCHI_FLAGS_T flags,
    void * bulk_handle )
 {
+#ifdef __riscos__
+   if (!handle)
+      return VCHIQ_ERROR;
+
+   return (swi_vchiq_queue_bulk_transmit(handle,
+					 data_src,
+					 data_size,
+					 flags,
+					 bulk_handle) == NULL ? 0 : -1);
+#else
    VCHI_SERVICE_T *service = find_service_by_handle(handle);
    VCHIQ_QUEUE_BULK_TRANSFER_T args;
    int ret;
@@ -879,6 +1103,7 @@
    RETRY(ret, ioctl(service->fd, VCHIQ_IOC_QUEUE_BULK_TRANSMIT, &args));
 
    return ret;
+#endif
 }
 
 /***********************************************************
@@ -902,6 +1127,16 @@
    uint32_t *actual_msg_size,
    VCHI_FLAGS_T flags )
 {
+#ifdef __riscos__
+   vcos_assert(flags == VCHI_FLAGS_NONE || flags == VCHI_FLAGS_BLOCK_UNTIL_OP_COMPLETE);
+
+   if (!handle)
+      return VCHIQ_ERROR;
+
+   return (swi_vchiq_msg_dequeue(handle, data,
+				 max_data_size_to_read,
+				 flags, actual_msg_size) == 0 ? 0 : -1);
+#else
    VCHI_SERVICE_T *service = find_service_by_handle(handle);
    VCHIQ_DEQUEUE_MESSAGE_T args;
    int ret;
@@ -945,6 +1180,7 @@
       fprintf(stderr, "vchi_msg_dequeue -> %d(%d)\n", ret, errno);
 
    return ret;
+#endif
 }
 
 /***********************************************************
@@ -973,6 +1209,16 @@
    VCHI_FLAGS_T flags,
    void *msg_handle )
 {
+#ifdef __riscos__
+   vcos_unused(msg_handle);
+
+   vcos_assert(flags == VCHI_FLAGS_BLOCK_UNTIL_QUEUED);
+
+   if (!handle)
+      return VCHIQ_ERROR;
+
+   return (swi_vchiq_msg_queuev(handle, vector, count, flags, msg_handle) == NULL ? 0 : -1);
+#else
    VCHI_SERVICE_T *service = find_service_by_handle(handle);
    VCHIQ_QUEUE_MESSAGE_T args;
    int ret;
@@ -990,6 +1236,7 @@
    RETRY(ret, ioctl(service->fd, VCHIQ_IOC_QUEUE_MESSAGE, &args));
 
    return ret;
+#endif
 }
 
 /***********************************************************
@@ -1007,11 +1254,13 @@
 {
    int ret = -1;
 
+#ifndef __riscos__
    if (message && message->message && !message->service)
    {
       free_msgbuf(message->message);
       ret = 0;
    }
+#endif
 
    return ret;
 }
@@ -1039,6 +1288,10 @@
    VCHI_FLAGS_T flags,
    VCHI_HELD_MSG_T *message_handle )
 {
+#ifdef __riscos__
+   fprintf(stderr, "[%s:%d:%s] - forced to return VCHIQ_ERROR\n", __func__, __LINE__, __FILE__);
+   return VCHIQ_ERROR;
+#else
    VCHI_SERVICE_T *service = find_service_by_handle(handle);
    int ret;
 
@@ -1060,6 +1313,7 @@
    }
 
    return 0;
+#endif
 }
 
 /***********************************************************
@@ -1159,6 +1413,25 @@
    SERVICE_CREATION_T *setup,
    VCHI_SERVICE_HANDLE_T *handle )
 {
+#ifdef __riscos__
+   {
+     if (swi_vchiq_service_open((VCHIQ_INSTANCE_T)instance_handle, setup, handle) != NULL)
+     {
+        vcos_log_error("RISC OS: Error returned by SWI in '%s'", __func__);
+        return -1;
+     }
+     SERVICE_ADD(*handle);
+   }
+
+   vcos_log_info("service handle %x  using fourcc 0x%x (%c%c%c%c)",
+		 *handle, setup->service_id,
+                 (setup->service_id >> 24) & 0xff,
+                 (setup->service_id >> 16) & 0xff,
+                 (setup->service_id >>  8) & 0xff,
+                 (setup->service_id      ) & 0xff );
+
+   return 0;
+#else
    VCHIQ_SERVICE_PARAMS_T params;
    VCHIQ_STATUS_T status;
 
@@ -1175,14 +1448,25 @@
       (VCHIQ_SERVICE_HANDLE_T *)handle);
 
    return (status == VCHIQ_SUCCESS) ? 0 : -1;
+#endif
 }
 
 int32_t
 vchi_service_create( VCHI_INSTANCE_T instance_handle,
    SERVICE_CREATION_T *setup, VCHI_SERVICE_HANDLE_T *handle )
 {
-   VCHIQ_SERVICE_PARAMS_T params;
+#ifdef __riscos__
+   if (swi_vchiq_service_create((VCHIQ_INSTANCE_T)instance_handle, setup, handle) != NULL)
+   {
+      vcos_log_error("RISC OS: Error returned by SWI in '%s'", __func__);
+      return -1;
+   }
+   SERVICE_ADD(*handle);
+
+   return 0;
+#else
    VCHIQ_STATUS_T status;
+   VCHIQ_SERVICE_PARAMS_T params;
 
    memset(&params, 0, sizeof(params));
    params.fourcc = setup->service_id;
@@ -1197,11 +1481,20 @@
       (VCHIQ_SERVICE_HANDLE_T *)handle);
 
    return (status == VCHIQ_SUCCESS) ? 0 : -1;
+#endif
 }
 
 int32_t
 vchi_service_close( const VCHI_SERVICE_HANDLE_T handle )
 {
+#ifdef __riscos__
+   if (!handle)
+      return VCHIQ_ERROR;
+
+   SERVICE_REMOVE(handle);
+
+   return (swi_vchiq_service_close(handle) == NULL ? 0 : -1);
+#else
    VCHI_SERVICE_T *service = find_service_by_handle(handle);
    int ret;
 
@@ -1214,11 +1507,20 @@
       service->lib_handle = VCHIQ_SERVICE_HANDLE_INVALID;
 
    return ret;
+#endif
 }
 
 int32_t
 vchi_service_destroy( const VCHI_SERVICE_HANDLE_T handle )
 {
+#ifdef __riscos__
+   if (!handle)
+      return VCHIQ_ERROR;
+
+   SERVICE_REMOVE(handle);
+
+   return (swi_vchiq_service_destroy(handle) == NULL ? 0 : -1);
+#else
    VCHI_SERVICE_T *service = find_service_by_handle(handle);
    int ret;
 
@@ -1230,6 +1532,7 @@
    service->lib_handle = VCHIQ_SERVICE_HANDLE_INVALID;
 
    return ret;
+#endif
 }
 
 /* ----------------------------------------------------------------------
@@ -1293,6 +1596,12 @@
 int32_t
 vchi_service_use( const VCHI_SERVICE_HANDLE_T handle )
 {
+#ifdef __riscos__
+   if (!handle)
+      return VCHIQ_ERROR;
+
+   return (swi_vchiq_service_use(handle) == NULL ? 0 : -1);
+#else
    VCHI_SERVICE_T *service = find_service_by_handle(handle);
    int ret;
 
@@ -1301,6 +1610,7 @@
 
    RETRY(ret,ioctl(service->fd, VCHIQ_IOC_USE_SERVICE, service->handle));
    return ret;
+#endif
 }
 
 /***********************************************************
@@ -1315,6 +1625,12 @@
  ***********************************************************/
 int32_t vchi_service_release( const VCHI_SERVICE_HANDLE_T handle )
 {
+#ifdef __riscos__
+   if (!handle)
+      return VCHIQ_ERROR;
+
+   return (swi_vchiq_service_release(handle) == NULL ? 0 : -1);
+#else
    VCHI_SERVICE_T *service = find_service_by_handle(handle);
    int ret;
 
@@ -1323,6 +1639,7 @@
 
    RETRY(ret,ioctl(service->fd, VCHIQ_IOC_RELEASE_SERVICE, service->handle));
    return ret;
+#endif
 }
 
 /***********************************************************
@@ -1340,6 +1657,10 @@
 int32_t vchi_service_set_option( const VCHI_SERVICE_HANDLE_T handle,
    VCHI_SERVICE_OPTION_T option, int value)
 {
+#ifdef __riscos__
+   fprintf(stderr, "[%s:%d:%s] - forced to return VCHIQ_ERROR\n", __func__, __LINE__, __FILE__);
+   return VCHIQ_ERROR;
+#else
    VCHIQ_SET_SERVICE_OPTION_T args;
    VCHI_SERVICE_T *service = find_service_by_handle(handle);
    int ret;
@@ -1363,6 +1684,7 @@
    RETRY(ret, ioctl(service->fd, VCHIQ_IOC_SET_SERVICE_OPTION, &args));
 
    return ret;
+#endif
 }
 
 /***********************************************************
@@ -1382,6 +1704,10 @@
                              void *ptr,
                              size_t num_bytes )
 {
+#ifdef __riscos__
+   fprintf(stderr, "[%s:%d:%s] - forced to return VCHIQ_ERROR\n", __func__, __LINE__, __FILE__);
+   return VCHIQ_ERROR;
+#else
    VCHIQ_SERVICE_T *service = (VCHIQ_SERVICE_T *)handle;
    VCHIQ_DUMP_MEM_T  dump_mem;
    int ret;
@@ -1394,6 +1720,7 @@
 
    RETRY(ret,ioctl(service->fd, VCHIQ_IOC_DUMP_PHYS_MEM, &dump_mem));
    return (ret >= 0) ? VCHIQ_SUCCESS : VCHIQ_ERROR;
+#endif
 }
 
 
@@ -1407,7 +1734,7 @@
 {
    static int mutex_initialised = 0;
    static VCOS_MUTEX_T vchiq_lib_mutex;
-   VCHIQ_INSTANCE_T instance = &vchiq_instance;
+   VCHIQ_INSTANCE_T instance = vchiq_instance;
 
    vcos_global_lock();
    if (!mutex_initialised)
@@ -1423,8 +1750,17 @@
 
    vcos_mutex_lock(&vchiq_lib_mutex);
 
-   if (instance->initialised == 0)
+   if (instance == NULL || instance->state == NULL || instance->state->initialised == 0)
    {
+#ifdef __riscos__
+      if (swi_vchiq_initialise(globals->private_word, &instance) == NULL)
+      {
+	 vchiq_instance = instance;
+//	 vcos_mutex_create(&instance->mutex, "VCHIQ instance");
+//	 instance->used_services = 0;
+	 instance->state->initialised = 1;
+      }
+#else
       instance->fd = open("/dev/vchiq", O_RDWR);
       if (instance->fd >= 0)
       {
@@ -1467,10 +1803,11 @@
       {
          instance = NULL;
       }
+#endif
    }
-   else if (instance->initialised > 0)
+   else if (instance->state->initialised > 0)
    {
-      instance->initialised++;
+      instance->state->initialised++;
    }
 
    vcos_mutex_unlock(&vchiq_lib_mutex);
@@ -1478,6 +1815,7 @@
    return instance;
 }
 
+#ifndef __riscos__
 static void *
 completion_thread(void *arg)
 {
@@ -1562,6 +1900,7 @@
 
    return NULL;
 }
+#endif
 
 static VCHIQ_STATUS_T
 create_service(VCHIQ_INSTANCE_T instance,
@@ -1570,6 +1909,38 @@
    int is_open,
    VCHIQ_SERVICE_HANDLE_T *phandle)
 {
+#ifdef __riscos__
+   SERVICE_CREATION_T setup;
+
+   setup.version.version = params->version;
+   setup.version.version_min = params->version_min;
+   setup.service_id = params->fourcc;
+   setup.connection = 0;
+   setup.rx_fifo_size = 0;
+   setup.tx_fifo_size = 0;
+   setup.callback = params->callback;//vchi_callback;
+   setup.callback_param = params->userdata;
+   setup.want_unaligned_bulk_rx = 0;
+   setup.want_unaligned_bulk_tx = 0;
+   setup.want_crc = 0;
+
+   if (swi_vchiq_service_create(instance, &setup, phandle) != NULL)
+   {
+      vcos_log_error("RISC OS: Error returned by SWI in '%s'", __func__);
+      return VCHIQ_ERROR;
+   }
+
+   vcos_log_info("handle %x, service handle %x  using fourcc 0x%x",
+		 *phandle, ((SHIM_SERVICE_T *)*phandle)->handle, params->fourcc);
+//   int i = ((SHIM_SERVICE_T *)*phandle)->handle & (VCHIQ_MAX_SERVICES - 1);
+//   ((VCHIQ_INSTANCE_T)instance)->state->services[i]->service_use_count++;
+//   ((VCHIQ_INSTANCE_T)instance)->state->services[i]->srvstate = VCHIQ_SRVSTATE_OPEN;
+
+//   ((SHIM_SERVICE_T *)*phandle)->callback = setup.callback;
+//   ((SHIM_SERVICE_T *)*phandle)->callback_param = setup.callback_param;
+
+   return VCHIQ_SUCCESS;
+#else
    VCHIQ_SERVICE_T *service = NULL;
    VCHIQ_STATUS_T status = VCHIQ_SUCCESS;
    int i;
@@ -1679,8 +2050,10 @@
    }
 
    return status;
+#endif
 }
 
+#ifndef __riscos__
 static int
 fill_peek_buf(VCHI_SERVICE_T *service,
    VCHI_FLAGS_T flags)
@@ -1723,7 +2096,6 @@
    return ret;
 }
 
-
 static void *
 alloc_msgbuf(void)
 {
@@ -1746,3 +2118,4 @@
    free_msgbufs = buf;
    vcos_mutex_unlock(&vchiq_lib_mutex);
 }
+#endif
