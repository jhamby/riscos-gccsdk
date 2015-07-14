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
#include <kernel.h>
#include "main.h"
#include "khronos.h"
#include "EGL/egl.h"
#include "EGL/eglext.h"
#include "../types.h"
#include "../enums.h"

EGLint last_egl_error __attribute__((visibility("hidden"))) = EGL_SUCCESS;

_kernel_oserror *
egl_swi(_kernel_swi_regs *r)
{
  switch ((egl_reason_code)r->r[8])
  {
    case egl_BindAPI:
      r->r[0] = (int)eglBindAPI((EGLenum)r->r[0]);
      break;
    case egl_BindTexImage:
      r->r[0] = (int)eglBindTexImage((EGLDisplay)r->r[0],
				     (EGLSurface)r->r[1],
				     (EGLint)r->r[2]);
      break;
    case egl_ClientWaitSyncKHR:
      r->r[0] = (int)eglClientWaitSyncKHR((EGLDisplay)r->r[0], (EGLSyncKHR)r->r[1],
					  (EGLint)r->r[2], (EGLTimeKHR)r->r[3]);
      break;
    case egl_ChooseConfig:
      r->r[0] = (int)eglChooseConfig((EGLDisplay)r->r[0], (const EGLint *)r->r[1],
				     (EGLConfig *)r->r[2], (EGLint)r->r[3],
				     (EGLint *)r->r[4]);
      break;
    case egl_CopyBuffers:
      r->r[0] = (int)eglCopyBuffers((EGLDisplay)r->r[0], (EGLSurface)r->r[1],
				    (EGLNativePixmapType)r->r[2]);
      break;
    case egl_CreateContext:
      r->r[0] = (int)eglCreateContext((EGLDisplay)r->r[0], (EGLConfig)r->r[1],
				      (EGLContext)r->r[2], (const EGLint *)r->r[3]);
      break;
    case egl_CreateImageKHR:
      r->r[0] = (int)eglCreateImageKHR ((EGLDisplay)r->r[0], (EGLContext)r->r[1],
					(EGLenum)r->r[2], (EGLClientBuffer)r->r[3],
					(const EGLint *)r->r[4]);
      break;
    case egl_CreatePbufferFromClientBuffer:
      r->r[0] = (int)eglCreatePbufferFromClientBuffer((EGLDisplay)r->r[0], (EGLenum)r->r[1],
						      (EGLClientBuffer)r->r[2],
						      (EGLConfig)r->r[3],
						      (const EGLint *)r->r[4]);
      break;
    case egl_CreatePbufferSurface:
      r->r[0] = (int)eglCreatePbufferSurface((EGLDisplay)r->r[0], (EGLConfig)r->r[1],
					     (const EGLint *)r->r[2]);
      break;
    case egl_CreatePixmapSurface:
      r->r[0] = (int)eglCreatePixmapSurface((EGLDisplay)r->r[0], (EGLConfig)r->r[1],
					    (EGLNativePixmapType)r->r[2],
					    (const EGLint *)r->r[3]);
      break;
    case egl_CreateSyncKHR:
      r->r[0] = (int)eglCreateSyncKHR((EGLDisplay)r->r[0], (EGLenum)r->r[1],
				      (const EGLint *)r->r[2]);
      break;
    case egl_CreateWindowSurface:
      r->r[0] = (int)eglCreateWindowSurface((EGLDisplay)r->r[0],
					    (EGLConfig)r->r[1],
					    (EGLNativeWindowType)r->r[2],
					    (const EGLint *)r->r[3]);
      break;
    case egl_DestroyContext:
      r->r[0] = (int)eglDestroyContext((EGLDisplay)r->r[0], (EGLContext)r->r[1]);
      break;
    case egl_DestroyImageKHR:
      r->r[0] = (int)eglDestroyImageKHR((EGLDisplay)r->r[0], (EGLImageKHR)r->r[1]);
      break;
    case egl_DestroySurface:
      r->r[0] = (int)eglDestroySurface((EGLDisplay)r->r[0], (EGLSurface)r->r[1]);
      break;
    case egl_DestroySyncKHR:
      r->r[0] = (int)eglDestroySyncKHR((EGLDisplay)r->r[0], (EGLSyncKHR)r->r[1]);
      break;
    case egl_FlushBRCM:
      eglFlushBRCM();
      break;
    case egl_GetConfigAttrib:
      r->r[0] = (int)eglGetConfigAttrib((EGLDisplay)r->r[0], (EGLConfig)r->r[1],
					(EGLint)r->r[2], (EGLint *)r->r[3]);
      break;
    case egl_GetConfigs:
      r->r[0] = (int)eglGetConfigs((EGLDisplay)r->r[0], (EGLConfig *)r->r[1],
				   (EGLint)r->r[2], (EGLint *)r->r[3]);
      break;
    case egl_GetCurrentContext:
      r->r[0] = (int)eglGetCurrentContext();
      break;
    case egl_GetCurrentDisplay:
      r->r[0] = (int)eglGetCurrentDisplay();
      break;
    case egl_GetCurrentSurface:
      r->r[0] = (int)eglGetCurrentSurface((EGLint)r->r[0]);
      break;
    case egl_GetDisplay:
      r->r[0] = (int)eglGetDisplay((EGLNativeDisplayType)r->r[0]);
      break;
    case egl_GetError: {
      EGLint error = eglGetError();

      if (error != EGL_SUCCESS)
	last_egl_error = error;

      r->r[0] = (int)error;
      break;
    }
    case egl_GetProcAddress:
      r->r[0] = (int)eglGetProcAddress((const char *)r->r[0]);
      break;
    case egl_GetSyncAttribKHR:
      r->r[0] = (int)eglGetSyncAttribKHR((EGLDisplay)r->r[0], (EGLSyncKHR)r->r[1],
					 (EGLint)r->r[2], (EGLint *)r->r[3]);
      break;
    case egl_Initialize:
      r->r[0] = (int)eglInitialize((EGLDisplay)r->r[0],
				   (EGLint *)r->r[1],
				   (EGLint *)r->r[2]);
      break;
    case egl_MakeCurrent:
      r->r[0] = (int)eglMakeCurrent((EGLDisplay)r->r[0], (EGLSurface)r->r[1],
				    (EGLSurface)r->r[2], (EGLContext)r->r[3]);
      break;
    case egl_LockSurfaceKHR:
      r->r[0] = (int)eglLockSurfaceKHR ((EGLDisplay)r->r[0], (EGLSurface)r->r[1],
					(const EGLint *)r->r[2]);
      break;
    case egl_QueryAPI:
      r->r[0] = (int)eglQueryAPI();
      break;
    case egl_QueryContext:
      r->r[0] = (int)eglQueryContext((EGLDisplay)r->r[0], (EGLContext)r->r[1],
				     (EGLint)r->r[2], (EGLint *)r->r[3]);
      break;
    case egl_QueryString:
      r->r[0] = (int)eglQueryString((EGLDisplay)r->r[0], (EGLint)r->r[1]);
      break;
    case egl_QuerySurface:
      r->r[0] = (int)eglQuerySurface((EGLDisplay)r->r[0], (EGLSurface)r->r[1],
				     (EGLint)r->r[2], (EGLint *)r->r[3]);
      break;
    case egl_ReleaseTexImage:
      r->r[0] = (int)eglReleaseTexImage((EGLDisplay)r->r[0], (EGLSurface)r->r[1],
					(EGLint)r->r[2]);
      break;
    case egl_ReleaseThread:
      r->r[0] = (int)eglReleaseThread();
      break;
    case egl_SignalSyncKHR:
      r->r[0] = (int)eglSignalSyncKHR((EGLDisplay)r->r[0], (EGLSyncKHR)r->r[1],
				      (EGLenum)r->r[2]);
      break;
    case egl_SurfaceAttrib:
      r->r[0] = (int)eglSurfaceAttrib((EGLDisplay)r->r[0], (EGLSurface)r->r[1],
				      (EGLint)r->r[2], (EGLint)r->r[3]);
      break;
    case egl_SwapBuffers:
      r->r[0] = (int)eglSwapBuffers((EGLDisplay)r->r[0], (EGLSurface)r->r[1]);
      break;
    case egl_SwapInterval:
      r->r[0] = (int)eglSwapInterval((EGLDisplay)r->r[0], (EGLint)r->r[1]);
      break;
    case egl_Terminate:
      r->r[0] = (int)eglTerminate((EGLDisplay)r->r[0]);
      break;
    case egl_UnlockSurfaceKHR:
      r->r[0] = (int)eglUnlockSurfaceKHR((EGLDisplay)r->r[0], (EGLSurface)r->r[1]);
      break;
    case egl_WaitClient:
      r->r[0] = (int)eglWaitClient();
      break;
    case egl_WaitGL:
      r->r[0] = (int)eglWaitGL();
      break;
    case egl_WaitNative:
      r->r[0] = (int)eglWaitNative((EGLint)r->r[0]);
      break;
    default:
      return khronos_bad_egl_reason;
  }

  return 0;
}
