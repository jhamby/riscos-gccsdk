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

#include <stdint.h>
#include <stdio.h>
#include <swis.h>
#include "main.h"
#include "khronos.h"
#include "kernel.h"

#include "interface/khronos/common/khrn_client.h"
#include "interface/vchiq_arm/vchiq_module.h"
#include "bcm_host.h"
#include "EGL/egl.h"
#include "GLES/gl.h"
#include "GLES2/gl2.h"
#include "../types.h"
#include "../enums.h"
#include "../inline_swis.h"

module_globals *globals;

_kernel_oserror *
khronos_init(const char *cmd_tail,int podule_base,void *pw)
{
  if (_swix(OS_SWINumberFromString,_IN(1),"VCHIQ_Initialise") != NULL)
    return khronos_no_vchiq;

  globals = (struct module_globals *)malloc(sizeof(module_globals));
  if (globals == NULL)
    return khronos_nomem;

  memset(globals, 0, sizeof(module_globals));

  globals->private_word = pw;

  INIT_LIST_HEAD(&globals->tasks);

  struct task_struct *fg = &globals->foreground;
  /* Mostly useless zero'ing because already used memset above.  */
  fg->rt_handle = 0;
  fg->stop = false;
  fg->retval = 0;
  fg->stopped = false;
  fg->stack = NULL;
  fg->wakeup = 0;
  fg->thread_function = NULL;
  fg->data = NULL;
  INIT_LIST_HEAD(&fg->keys);
  INIT_LIST_HEAD(&fg->link);

  list_add(&fg->link, &globals->tasks);

  bcm_host_init();

  /* Force call to vc_vchi_khronos_init().  */
  CLIENT_GET_THREAD_STATE();

  return 0;
}

_kernel_oserror *
khronos_final(int fatal,int podule,void *pw)
{
  vchiq_shutdown(globals->instance_handle);

  free(globals);

  return 0;
}

_kernel_oserror *
khronos_swi_handler(int swi_no,_kernel_swi_regs *r,void *private_word)
{
  switch (swi_no)
  {
  case Khronos_EGL - Khronos_00:
    return egl_swi(r);
  case Khronos_GL - Khronos_00:
    return gl_swi(r);
  case Khronos_OpenVG - Khronos_00:
    return openvg_swi(r);
  case Khronos_VC - Khronos_00:
    return vc_swi(r);
  }

  return 0;
}

void dump_service(VCHIQ_SERVICE_T *service)
{
  printf("[%c%c%c%c] ch: %2d\tctxc: %.8X\tcrxc: %.8X\tbtxc: %.8X\tbrxc: %.8X\t"
			     "ctxb: %.8X\tcrxb: %.8X\tbtxb: %.8X\tbrxb: %.8X\n",
	 VCHIQ_FOURCC_AS_4CHARS(service->base.fourcc),
	 service->handle & (VCHIQ_MAX_SERVICES - 1),
	 service->stats.ctrl_tx_count,
	 service->stats.ctrl_rx_count,
	 service->stats.bulk_tx_count,
	 service->stats.bulk_rx_count,
	 service->stats.ctrl_tx_bytes,
	 service->stats.ctrl_rx_bytes,
	 service->stats.bulk_tx_bytes,
	 service->stats.bulk_rx_bytes);
}

static const char *
egl_last_error_string(void)
{
  static const char *strings[] = {
    "EGL_SUCCESS",
    "EGL_NOT_INITIALIZED",
    "EGL_BAD_ACCESS",
    "EGL_BAD_ALLOC",
    "EGL_BAD_ATTRIBUTE",
    "EGL_BAD_CONFIG",
    "EGL_BAD_CONTEXT",
    "EGL_BAD_CURRENT_SURFACE",
    "EGL_BAD_DISPLAY",
    "EGL_BAD_MATCH",
    "EGL_BAD_NATIVE_PIXMAP",
    "EGL_BAD_NATIVE_WINDOW",
    "EGL_BAD_PARAMETER",
    "EGL_BAD_SURFACE",
    "EGL_CONTEXT_LOST"
  };

  EGLint error = last_egl_error;

  if (error < EGL_SUCCESS || error > EGL_CONTEXT_LOST)
    return "Unknown error code";
  
  return strings[error - EGL_SUCCESS];
}

static const char *
gl_last_error_string(void)
{
  static const char *strings[] = {
    "GL_NO_ERROR",
    "GL_INVALID_ENUM",
    "GL_INVALID_VALUE",
    "GL_INVALID_OPERATION",
    "BAD ERROR CODE",
    "BAD ERROR CODE",
    "GL_OUT_OF_MEMORY",
    "GL_INVALID_FRAMEBUFFER_OPERATION"
  };

  GLenum error = last_gl_error;

  if (error == GL_NO_ERROR)
    return strings[error];

  if (error < GL_INVALID_ENUM || error > GL_INVALID_FRAMEBUFFER_OPERATION)
    return "Unknown error code";

  return strings[error - GL_INVALID_ENUM];
}

static const char *
vg_last_error_string(void)
{
  static const char *strings[] = {
    "VG_NO_ERROR",
    "VG_BAD_HANDLE_ERROR",
    "VG_ILLEGAL_ARGUMENT_ERROR",
    "VG_OUT_OF_MEMORY_ERROR",
    "VG_PATH_CAPABILITY_ERROR",
    "VG_UNSUPPORTED_IMAGE_FORMAT_ERROR",
    "VG_UNSUPPORTED_PATH_FORMAT_ERROR",
    "VG_IMAGE_IN_USE_ERROR",
    "VG_NO_CONTEXT_ERROR"
  };

  VGErrorCode error = last_vg_error;

  if (error == VG_NO_ERROR)
    return strings[error];

  if (error < VG_BAD_HANDLE_ERROR || error > VG_NO_CONTEXT_ERROR)
    return "Unknown error code";

  return strings[error - VG_BAD_HANDLE_ERROR];
}

_kernel_oserror *
module_command (const char *arg_string, int argc, int number, void *pw)
{
  _kernel_oserror *err = NULL;

  switch (number)
  {
    case CMD_KhronosStatus:
      /* Just in case the *GetError() functions have not actually been called.  */
      if (last_egl_error == EGL_SUCCESS)
	last_egl_error = eglGetError();
      if (last_gl_error == GL_NO_ERROR)
	last_gl_error = glGetError();

      printf("   Last EGL error: %s (%X)\n", egl_last_error_string(), last_egl_error);
      printf("    Last GL error: %s (%X)\n", gl_last_error_string(), last_gl_error);
      printf("Last OpenVG error: %s (%X)\n", vg_last_error_string(), last_vg_error);
      break;
    case CMD_KhronosInfo: {
      int i;

      for (i = 0; i < VCHIQ_MAX_INSTANCE_SERVICES; i++)
	if (globals->services[i])
	  dump_service(vchiq_instance->state->services[i]);
      break;
    }
  }

  return err;
}
