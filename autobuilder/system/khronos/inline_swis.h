/*
 * Copyright (c) 2015, GCCSDK Developers.
 * Written by Lee Noar.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the copyright holder nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "main.h"
#include <swis.h>
#include "vchiq_if.h"
#include "interface/vchi/vchi_common.h"
#include "interface/vchi/vchi.h"

static inline _kernel_oserror *
swi_vchiq_initialise(void *module_word, VCHIQ_INSTANCE_T *instance_ret)
{
  register void *__module_word __asm__("r0") = module_word;
  register _kernel_oserror *error __asm__("r0");

  __asm__ volatile ("SWI	%[XVCHIQ_Initialise];\n\t"
		    "MOVVC	%[instance], r0;\n\t"
		    "MOVVC	r0, #0;\n\t"
		    : "=r" (error), [instance] "=r" (*instance_ret)
		    : [XVCHIQ_Initialise] "i" (XVCHIQ_Initialise),
		      "r" (__module_word)
		    : "r14", "cc");
  return error;
}

static inline _kernel_oserror *
swi_vchiq_connect(VCHIQ_INSTANCE_T instance)
{
  register VCHIQ_INSTANCE_T __instance __asm__("r0") = instance;
  register _kernel_oserror *error __asm__("r0");

  __asm__ volatile ("SWI	%[XVCHIQ_Connect];\n\t"
		    "MOVVC	r0, #0;\n\t"
		    : "=r" (error)
		    : [XVCHIQ_Connect] "i" (XVCHIQ_Connect),
		      "r" (__instance)
		    : "r14", "cc");
  return error;
}

static inline _kernel_oserror *
swi_vchiq_service_open(VCHIQ_INSTANCE_T instance,
		       SERVICE_CREATION_T *setup,
		       VCHIQ_SERVICE_HANDLE_T *handle_ret)
{
  register VCHIQ_INSTANCE_T __instance __asm__("r0") = instance;
  register SERVICE_CREATION_T *__setup __asm__("r1") = setup;
  register _kernel_oserror *error __asm__("r0");

  __asm__ volatile ("SWI	%[XVCHIQ_ServiceOpen];\n\t"
		    "STRVC	r0, %[handle_ret];\n\t"
		    "MOVVC	r0, #0;\n\t"
		    : "=r" (error), [handle_ret] "=m" (*handle_ret)
		    : [XVCHIQ_ServiceOpen] "i" (XVCHIQ_ServiceOpen),
		      "r" (__instance), "r" (__setup)
		    : "r14", "cc");
  return error;
}

static inline _kernel_oserror *
swi_vchiq_service_create(VCHIQ_INSTANCE_T instance,
			 SERVICE_CREATION_T *setup,
			 VCHIQ_SERVICE_HANDLE_T *handle_ret)
{
  register VCHIQ_INSTANCE_T __instance __asm__("r0") = instance;
  register SERVICE_CREATION_T *__setup __asm__("r1") = setup;
  register _kernel_oserror *error __asm__("r0");

  __asm__ volatile ("SWI	%[XVCHIQ_ServiceCreate];\n\t"
		    "STRVC	r0, %[handle_ret];\n\t"
		    "MOVVC	r0, #0;\n\t"
		    : "=r" (error), [handle_ret] "=m" (*handle_ret)
		    : [XVCHIQ_ServiceCreate] "i" (XVCHIQ_ServiceCreate),
		      "r" (__instance), "r" (__setup)
		    : "r14", "cc");
  return error;
}

static inline _kernel_oserror *
swi_vchiq_service_close(VCHIQ_SERVICE_HANDLE_T service)
{
  register VCHIQ_SERVICE_HANDLE_T __service __asm__("r0") = service;
  register _kernel_oserror *error __asm__("r0");

  __asm__ volatile ("SWI	%[XVCHIQ_ServiceClose];\n\t"
		    "MOVVC	r0, #0;\n\t"
		    : "=r" (error)
		    : [XVCHIQ_ServiceClose] "i" (XVCHIQ_ServiceClose),
		      "r" (__service)
		    : "r14", "cc");
  return error;
}

static inline _kernel_oserror *
swi_vchiq_service_destroy(VCHIQ_SERVICE_HANDLE_T service)
{
  register VCHIQ_SERVICE_HANDLE_T __service __asm__("r0") = service;
  register _kernel_oserror *error __asm__("r0");

  __asm__ volatile ("SWI	%[XVCHIQ_ServiceDestroy];\n\t"
		    "MOVVC	r0, #0;\n\t"
		    : "=r" (error)
		    : [XVCHIQ_ServiceDestroy] "i" (XVCHIQ_ServiceDestroy),
		      "r" (__service)
		    : "r14", "cc");
  return error;
}

static inline _kernel_oserror *
swi_vchiq_service_release(VCHIQ_SERVICE_HANDLE_T service)
{
  register VCHIQ_SERVICE_HANDLE_T __service __asm__("r0") = service;
  register _kernel_oserror *error __asm__("r0");

  __asm__ volatile ("SWI	%[XVCHIQ_ServiceRelease];\n\t"
		    "MOVVC	r0, #0;\n\t"
		    : "=r" (error)
		    : [XVCHIQ_ServiceRelease] "i" (XVCHIQ_ServiceRelease),
		      "r" (__service)
		    : "r14", "cc");
  return error;
}

static inline _kernel_oserror *
swi_vchiq_service_use(VCHIQ_SERVICE_HANDLE_T service)
{
  register VCHIQ_SERVICE_HANDLE_T __service __asm__("r0") = service;
  register _kernel_oserror *error __asm__("r0");

  __asm__ volatile ("SWI	%[XVCHIQ_ServiceUse];\n\t"
		    "MOVVC	r0, #0;\n\t"
		    : "=r" (error)
		    : [XVCHIQ_ServiceUse] "i" (XVCHIQ_ServiceUse),
		      "r" (__service)
		    : "r14", "cc");
  return error;
}

static inline _kernel_oserror *
swi_vchiq_msg_queue(VCHIQ_SERVICE_HANDLE_T service,
		    const void *data,
		    const int size,
		    VCHI_FLAGS_T flags)
{
  register VCHIQ_SERVICE_HANDLE_T __service __asm__("r0") = service;
  register const void * __data __asm__("r1") = data;
  register const int __size __asm__("r2") = size;
  register VCHI_FLAGS_T __flags __asm__("r3") = flags;
  register _kernel_oserror *error __asm__("r0");

  __asm__ volatile ("SWI	%[XVCHIQ_MsgQueue];\n\t"
		    "MOVVC	r0, #0;\n\t"
		    : "=r" (error)
		    : [XVCHIQ_MsgQueue] "i" (XVCHIQ_MsgQueue),
		      "r" (__service), "r" (__data), "r" (__size), "r" (__flags)
		    : "r14", "cc");
  return error;
}

static inline _kernel_oserror *
swi_vchiq_msg_dequeue(VCHIQ_SERVICE_HANDLE_T service,
		      void *data,
		      uint32_t size,
		      VCHI_FLAGS_T flags,
		      uint32_t *size_ret)
{
  register VCHIQ_SERVICE_HANDLE_T __service __asm__("r0") = service;
  register void * __data __asm__("r1") = data;
  register const int __size __asm__("r2") = size;
  register VCHI_FLAGS_T __flags __asm__("r3") = flags;
  register _kernel_oserror *error __asm__("r0");

  __asm__ volatile ("SWI	%[XVCHIQ_MsgDequeue];\n\t"
		    "STRVC	r2, %[size_ret];\n\t"
		    "MOVVC	r0, #0;\n\t"
		    : "=r" (error), [size_ret] "=m" (*size_ret)
		    : [XVCHIQ_MsgDequeue] "i" (XVCHIQ_MsgDequeue),
		      "r" (__service), "r" (__data), "r" (__size), "r" (__flags)
		    : "r14", "cc");
  return error;
}

static inline _kernel_oserror *
swi_vchiq_msg_queuev(VCHI_SERVICE_HANDLE_T service,
		     VCHI_MSG_VECTOR_T * vector,
		     uint32_t count,
		     VCHI_FLAGS_T flags,
		     void *msg_handle)
{
  register VCHIQ_SERVICE_HANDLE_T __service __asm__("r0") = service;
  register VCHI_MSG_VECTOR_T * __vector __asm__("r1") = vector;
  register uint32_t __count __asm__("r2") = count;
  register VCHI_FLAGS_T __flags __asm__("r3") = flags;
  register void *__msg_handle __asm__("r4") = msg_handle;
  register _kernel_oserror *error __asm__("r0");

  __asm__ volatile ("SWI	%[XVCHIQ_MsgQueueV];\n\t"
		    "MOVVC	r0, #0;\n\t"
		    : "=r" (error)
		    : [XVCHIQ_MsgQueueV] "i" (XVCHIQ_MsgQueueV),
		      "r" (__service), "r" (__vector), "r" (__count),
		      "r" (__flags), "r" (__msg_handle)
		    : "r14", "cc");
  return error;
}

static inline _kernel_oserror *
swi_vchiq_queue_bulk_transmit(VCHIQ_SERVICE_HANDLE_T service,
			      const void *data,
			      int size,
			      VCHI_FLAGS_T flags,
			      void *userdata)
{
  register VCHIQ_SERVICE_HANDLE_T __service __asm__("r0") = service;
  register const void * __data __asm__("r1") = data;
  register int __size __asm__("r2") = size;
  register VCHI_FLAGS_T __flags __asm__("r3") = flags;
  register void *__userdata __asm__("r4") = userdata;
  register _kernel_oserror *error __asm__("r0");

  __asm__ volatile ("SWI	%[XVCHIQ_BulkQueueTransmit];\n\t"
		    "MOVVC	r0, #0;\n\t"
		    : "=r" (error)
		    : [XVCHIQ_BulkQueueTransmit] "i" (XVCHIQ_BulkQueueTransmit),
		      "r" (__service), "r" (__data), "r" (__size),
		      "r" (__flags), "r" (__userdata)
		    : "r14", "cc");
  return error;
}

static inline _kernel_oserror *
swi_vchiq_msg_peek(VCHIQ_SERVICE_HANDLE_T service,
		   VCHI_FLAGS_T flags,
		   void **data_ret,
		   uint32_t *size_ret)
{
  register VCHIQ_SERVICE_HANDLE_T __service __asm__("r0") = service;
  register VCHI_FLAGS_T __flags __asm__("r1") = flags;
  register _kernel_oserror *error __asm__("r0");

  __asm__ volatile ("SWI	%[XVCHIQ_MsgPeek];\n\t"
		    "STRVC	r0, %[data];\n\t"
		    "STRVC	r1, %[size];\n\t"
		    "MOVVC	r0, #0;\n\t"
		    : "=r" (error), [data] "=m" (*data_ret), [size] "=m" (*size_ret)
		    : [XVCHIQ_MsgPeek] "i" (XVCHIQ_MsgPeek),
		      "r" (__service), "r" (__flags)
		    : "r14", "cc");
  return error;
}

static inline _kernel_oserror *
swi_vchiq_msg_remove(VCHIQ_SERVICE_HANDLE_T service)
{
  register VCHIQ_SERVICE_HANDLE_T __service __asm__("r0") = service;
  register _kernel_oserror *error __asm__("r0");

  __asm__ volatile ("SWI	%[XVCHIQ_MsgRemove];\n\t"
		    "MOVVC	r0, #0;\n\t"
		    : "=r" (error)
		    : [XVCHIQ_MsgRemove] "i" (XVCHIQ_MsgRemove),
		      "r" (__service)
		    : "r14", "cc");
  return error;
}

static inline void *kernel_alloc(size_t size)
{
  register int __reason __asm__("r0") = 6;
  register size_t __size __asm__("r3") = size;
  register void *res __asm("r2");

  __asm__ volatile ("SWI	%[SWI_OS_Module];\n\t"
		    :
		    : [SWI_OS_Module] "i" (OS_Module),
		      "r" (__reason), "r" (__size)
		    : "r14", "cc");
  return res;
}

static inline void kernel_free(void *ptr)
{
  register int __reason __asm__("r0") = 7;
  register void *__ptr __asm__("r2") = ptr;

  __asm__ volatile ("SWI	%[SWI_OS_Module];\n\t"
		    :
		    : [SWI_OS_Module] "i" (OS_Module),
		      "r" (__reason), "r" (__ptr)
		    : "r14", "cc");
}

/* 32 bit hex to string
 * return pointer to terminating null in buffer
 */
static inline char *
hex_to_string(unsigned int v, char *buffer, int l)
{
char *term;

  __asm volatile (
#ifdef __TARGET_MODULE__
		  "	STR	lr, [sp, #-4]!;\n"
#endif
		  "	MOV	r0, %1;\n"
		  "	MOV	r1, %2;\n"
		  "	MOV	r2, %3;\n"
		  "	SWI	%[os_converthex8];\n"
		  "	MOV	%0, r1;\n"
#ifdef __TARGET_MODULE__
		  "	LDR	lr, [sp], #4;\n"
#endif
		  : "=r" (term)
		  : "r" (v), "r" (buffer), "rI" (l), [os_converthex8] "i" (0xD4)
		  : "r0", "r1", "r2", "r14", "cc");
  return term;
}

/* Signed 32 bit integer to string
 * return pointer to terminating null in buffer
 */
static inline char *
sint32_to_string(int v, char *buffer, int l)
{
char *term;

  __asm volatile (
#ifdef __TARGET_MODULE__
		  "	STR	lr, [sp, #-4]!;\n"
#endif
		  "	MOV	r0, %1;\n"
		  "	MOV	r1, %2;\n"
		  "	MOV	r2, %3;\n"
		  "	SWI	%[os_convertint4];\n"
		  "	MOV	%0, r1;\n"
#ifdef __TARGET_MODULE__
		  "	LDR	lr, [sp], #4;\n"
#endif
		  : "=r" (term)
		  : "r" (v), "r" (buffer), "rI" (l), [os_convertint4] "i" (0xDC)
		  : "r0", "r1", "r2", "r14", "cc");
  return term;
}

static inline void report_text(const char *s)
{
  register const char *__s __asm__("r0") = s;

  __asm volatile (
#ifdef __TARGET_MODULE__
		  "	STR	lr, [sp, #-4]!;\n"
#endif
		  "	SWI	%[report_text0];\n"
#ifdef __TARGET_MODULE__
		  "	LDR	lr, [sp], #4;\n"
#endif
		  : /* no outputs */
		  : "r" (__s), [report_text0] "i" (0x74C80)
		  : "r14", "cc");
}

static inline void report_hex(unsigned int v)
{
  char buf[12];

  hex_to_string(v, buf, 10);
  report_text(buf);
}

static inline void report_dec(int v)
{
  char buf[20];

  sint32_to_string(v, buf, 20);
  report_text(buf);
}
