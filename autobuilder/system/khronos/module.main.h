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

#ifndef main_h
#define main_h

#include "GLES/gl.h"
#include "EGL/egl.h"
#include "VG/openvg.h"
#include "interface/vchi/vchi_common.h"
#include "riscosbits.h"
#include <kernel.h>

#define VCHIQ_MAX_INSTANCE_SERVICES 32

typedef struct opaque_vchi_instance_handle_t *VCHI_INSTANCE_T;

typedef struct module_globals {
  VCHI_SERVICE_HANDLE_T services[VCHIQ_MAX_INSTANCE_SERVICES];

  VCHI_INSTANCE_T *instance_handle;
  struct list_head tasks;

  struct task_struct foreground;

  void *private_word;

} module_globals;

extern module_globals *globals;

extern GLenum last_gl_error;
extern EGLint last_egl_error;
extern VGErrorCode last_vg_error;

enum {
  VCHIQ_Initialise = 0x59200,
  VCHIQ_Connect,
  VCHIQ_Disconnect,
  VCHIQ_BulkQueueTransmit,
  VCHIQ_MsgDequeue,
  VCHIQ_MsgQueue,
  VCHIQ_MsgQueueV,
  VCHIQ_MsgPeek,
  VCHIQ_MsgRemove,
  VCHIQ_ServiceClose,
  VCHIQ_ServiceOpen,
  VCHIQ_ServiceCreate,
  VCHIQ_ServiceDestroy,
  VCHIQ_ServiceUse,
  VCHIQ_ServiceRelease
};

enum {
  XVCHIQ_Initialise = 0x79200,
  XVCHIQ_Connect,
  XVCHIQ_Disconnect,
  XVCHIQ_BulkQueueTransmit,
  XVCHIQ_MsgDequeue,
  XVCHIQ_MsgQueue,
  XVCHIQ_MsgQueueV,
  XVCHIQ_MsgPeek,
  XVCHIQ_MsgRemove,
  XVCHIQ_ServiceClose,
  XVCHIQ_ServiceOpen,
  XVCHIQ_ServiceCreate,
  XVCHIQ_ServiceDestroy,
  XVCHIQ_ServiceUse,
  XVCHIQ_ServiceRelease
};

extern _kernel_oserror *vc_swi(_kernel_swi_regs *r);
extern _kernel_oserror *egl_swi(_kernel_swi_regs *r);
extern _kernel_oserror *openvg_swi(_kernel_swi_regs *r);
extern _kernel_oserror *gl_swi(_kernel_swi_regs *r);

extern void asm_khrn_callback(void *callback_param, VCHI_CALLBACK_REASON_T reason, void *handle);
extern void asm_khhn_callback(void *callback_param, VCHI_CALLBACK_REASON_T reason, void *handle);
extern void asm_khan_callback(void *callback_param, VCHI_CALLBACK_REASON_T reason, void *handle);

#endif
