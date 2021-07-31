/*
 * Copyright (c) 2015, 2016 GCCSDK Developers.
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

#include "../macros.s"
#include "../enums.h"

DEFINE_FUNC	eglBindAPI			SWI_EGL egl_BindAPI
DEFINE_FUNC	eglBindTexImage			SWI_EGL egl_BindTexImage
DEFINE_FUNC	eglClientWaitSyncKHR		SWI_EGL egl_ClientWaitSyncKHR
DEFINE_FUNC_5	eglChooseConfig			SWI_EGL egl_ChooseConfig
DEFINE_FUNC	eglCopyBuffers			SWI_EGL egl_CopyBuffers
DEFINE_FUNC	eglCreateContext		SWI_EGL egl_CreateContext
DEFINE_FUNC_5	eglCreateImageKHR		SWI_EGL egl_CreateImageKHR
DEFINE_FUNC_5	eglCreatePbufferFromClientBuffer SWI_EGL egl_CreatePbufferFromClientBuffer
DEFINE_FUNC	eglCreatePbufferSurface		SWI_EGL egl_CreatePbufferSurface
DEFINE_FUNC	eglCreatePixmapSurface 		SWI_EGL egl_CreatePixmapSurface
DEFINE_FUNC	eglCreateSyncKHR 		SWI_EGL egl_CreateSyncKHR
DEFINE_FUNC	eglCreateWindowSurface		SWI_EGL egl_CreateWindowSurface
DEFINE_FUNC	eglDestroyContext		SWI_EGL egl_DestroyContext
DEFINE_FUNC	eglDestroyImageKHR		SWI_EGL egl_DestroyImageKHR
DEFINE_FUNC	eglDestroySurface		SWI_EGL egl_DestroySurface
DEFINE_FUNC	eglDestroySyncKHR		SWI_EGL egl_DestroySyncKHR
DEFINE_FUNC	eglFlushBRCM			SWI_EGL egl_FlushBRCM
DEFINE_FUNC	eglGetConfigAttrib		SWI_EGL egl_GetConfigAttrib
DEFINE_FUNC	eglGetConfigs			SWI_EGL egl_GetConfigs
DEFINE_FUNC	eglGetCurrentContext		SWI_EGL egl_GetCurrentContext
DEFINE_FUNC	eglGetCurrentDisplay		SWI_EGL egl_GetCurrentDisplay
DEFINE_FUNC	eglGetCurrentSurface		SWI_EGL egl_GetCurrentSurface
DEFINE_FUNC	eglGetDisplay			SWI_EGL egl_GetDisplay
DEFINE_FUNC	eglGetError			SWI_EGL egl_GetError
#DEFINE_FUNC	eglGetProcAddress		SWI_EGL egl_GetProcAddress
DEFINE_FUNC	eglGetSyncAttribKHR		SWI_EGL egl_GetSyncAttribKHR
DEFINE_FUNC	eglInitialize			SWI_EGL egl_Initialize
DEFINE_FUNC	eglLockSurfaceKHR		SWI_EGL egl_LockSurfaceKHR
DEFINE_FUNC	eglMakeCurrent			SWI_EGL egl_MakeCurrent
DEFINE_FUNC	eglQueryAPI			SWI_EGL egl_QueryAPI
DEFINE_FUNC	eglQueryContext			SWI_EGL egl_QueryContext
DEFINE_FUNC	eglQueryString			SWI_EGL egl_QueryString
DEFINE_FUNC	eglQuerySurface			SWI_EGL egl_QuerySurface
DEFINE_FUNC	eglReleaseTexImage		SWI_EGL egl_ReleaseTexImage
DEFINE_FUNC	eglReleaseThread		SWI_EGL egl_ReleaseThread
DEFINE_FUNC	eglSignalSyncKHR		SWI_EGL egl_SignalSyncKHR
DEFINE_FUNC	eglSurfaceAttrib		SWI_EGL egl_SurfaceAttrib
DEFINE_FUNC	eglSwapBuffers			SWI_EGL egl_SwapBuffers
DEFINE_FUNC	eglSwapInterval			SWI_EGL egl_SwapInterval
DEFINE_FUNC	eglTerminate			SWI_EGL egl_Terminate
DEFINE_FUNC	eglUnlockSurfaceKHR		SWI_EGL egl_UnlockSurfaceKHR
DEFINE_FUNC	eglWaitClient			SWI_EGL egl_WaitClient
DEFINE_FUNC	eglWaitGL			SWI_EGL egl_WaitGL
DEFINE_FUNC	eglWaitNative			SWI_EGL egl_WaitNative

DEFINE_FUNC	eglQueryGlobalImageBRCM		SWI_EGL egl_QueryGlobalImageBRCM
DEFINE_FUNC_6	eglCreateGlobalImageBRCM	SWI_EGL egl_CreateGlobalImageBRCM
DEFINE_FUNC	eglCreateCopyGlobalImageBRCM	SWI_EGL egl_CreateCopyGlobalImageBRCM
DEFINE_FUNC	eglDestroyGlobalImageBRCM	SWI_EGL egl_DestroyGlobalImageBRCM
DEFINE_FUNC	eglProcStateValid		SWI_EGL egl_ProcStateValid
DEFINE_FUNC	eglInitDriverMonitorBRCM	SWI_EGL egl_InitDriverMonitorBRCM
DEFINE_FUNC	eglGetDriverMonitorXMLBRCM	SWI_EGL egl_GetDriverMonitorXMLBRCM
DEFINE_FUNC	eglTermDriverMonitorBRCM	SWI_EGL egl_TermDriverMonitorBRCM

# These symbols are referenced by eglGetProcAddress, but we don't want to create a
# dependency to the libraries that contain them, so we create weak versions that
# this library links to at build time. At runtime, the dynamic linker will use the
# strong references in the other libraries.
	NAME	"WeakDummyStub"
WEAK_STUB_DECL	vgCreateEGLImageTargetKHR
WEAK_STUB_DECL	glInsertEventMarkerEXT
WEAK_STUB_DECL	glDiscardFramebufferEXT
WEAK_STUB_DECL	glPushGroupMarkerEXT
WEAK_STUB_DECL	glPointSizePointerOES
WEAK_STUB_DECL	glPopGroupMarkerEXT
WEAK_STUB_DECL	glEGLImageTargetTexture2DOES
WEAK_STUB_DECL	glEGLImageTargetRenderbufferStorageOES
WEAK_STUB_DECL	glCurrentPaletteMatrixOES
WEAK_STUB_DECL	glLoadPaletteFromModelViewMatrixOES
WEAK_STUB_DECL	glMatrixIndexPointerOES
WEAK_STUB_DECL	glWeightPointerOES
WEAK_STUB_DECL	glUnmapBufferOES
WEAK_STUB_DECL	glMapBufferOES
WEAK_STUB_DECL	glGetBufferPointervOES
WEAK_STUB_DECL	glGenerateMipmapOES
WEAK_STUB_DECL	glGetFramebufferAttachmentParameterivOES
WEAK_STUB_DECL	glFramebufferTexture2DOES
WEAK_STUB_DECL	glFramebufferRenderbufferOES
WEAK_STUB_DECL	glCheckFramebufferStatusOES
WEAK_STUB_DECL	glGenFramebuffersOES
WEAK_STUB_DECL	glDeleteFramebuffersOES
WEAK_STUB_DECL	glBindFramebufferOES
WEAK_STUB_DECL	glIsFramebufferOES
WEAK_STUB_DECL	glGetRenderbufferParameterivOES
WEAK_STUB_DECL	glRenderbufferStorageOES
WEAK_STUB_DECL	glGenRenderbuffersOES
WEAK_STUB_DECL	glDeleteRenderbuffersOES
WEAK_STUB_DECL	glBindRenderbufferOES
WEAK_STUB_DECL	glIsRenderbufferOES
WEAK_STUB_DECL	glQueryMatrixxOES
WEAK_STUB_DECL	glDrawTexfvOES
WEAK_STUB_DECL	glDrawTexfOES
WEAK_STUB_DECL	glDrawTexxvOES
WEAK_STUB_DECL	glDrawTexivOES
WEAK_STUB_DECL	glDrawTexsvOES
WEAK_STUB_DECL	glDrawTexxOES
WEAK_STUB_DECL	glDrawTexiOES
WEAK_STUB_DECL	glDrawTexsOES
	REPORT_TEXT "Called weak stub function instead of full strong version"
	MOV	pc, lr
