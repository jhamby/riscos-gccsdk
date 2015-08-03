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

#include "../macros.s"
#include "../enums.h"

/* gl.h - Available only in Common profile */
DEFINE_FUNC	glAlphaFunc		SWI_GL	gl_AlphaFunc
DEFINE_FUNC	glClearColor		SWI_GL	gl_ClearColor
DEFINE_FUNC	glClearDepthf		SWI_GL	gl_ClearDepthf
DEFINE_FUNC	glClipPlanef		SWI_GL	gl_ClipPlanef
DEFINE_FUNC	glColor4f		SWI_GL	gl_Color4f
DEFINE_FUNC	glDepthRangef		SWI_GL	gl_DepthRangef
DEFINE_FUNC	glFogf			SWI_GL	gl_Fogf
DEFINE_FUNC	glFogfv			SWI_GL	gl_Fogfv
DEFINE_FUNC_6	glFrustumf		SWI_GL	gl_Frustumf
DEFINE_FUNC	glGetClipPlanef		SWI_GL	gl_GetClipPlanef
DEFINE_FUNC	glGetFloatv		SWI_GL	gl_GetFloatv
DEFINE_FUNC	glGetLightfv		SWI_GL	gl_GetLightfv
DEFINE_FUNC	glGetMaterialfv		SWI_GL	gl_GetMaterialfv
DEFINE_FUNC	glGetTexEnvfv		SWI_GL	gl_GetTexEnvfv
DEFINE_FUNC	glGetTexParameterfv	SWI_GL	gl_GetTexParameterfv
DEFINE_FUNC	glLightModelf		SWI_GL	gl_LightModelf
DEFINE_FUNC	glLightModelfv		SWI_GL	gl_LightModelfv
DEFINE_FUNC	glLightf		SWI_GL	gl_Lightf
DEFINE_FUNC	glLightfv		SWI_GL	gl_Lightfv
DEFINE_FUNC	glLineWidth		SWI_GL	gl_LineWidth
DEFINE_FUNC	glLoadMatrixf		SWI_GL	gl_LoadMatrixf
DEFINE_FUNC	glMaterialf		SWI_GL	gl_Materialf
DEFINE_FUNC	glMaterialfv		SWI_GL	gl_Materialfv
DEFINE_FUNC	glMultMatrixf		SWI_GL	gl_MultMatrixf
DEFINE_FUNC_5	glMultiTexCoord4f	SWI_GL	gl_MultiTexCoord4f
DEFINE_FUNC	glNormal3f		SWI_GL	gl_Normal3f
DEFINE_FUNC_6	glOrthof		SWI_GL	gl_Orthof
DEFINE_FUNC	glPointParameterf	SWI_GL	gl_PointParameterf
DEFINE_FUNC	glPointParameterfv	SWI_GL	gl_PointParameterfv
DEFINE_FUNC	glPointSize		SWI_GL	gl_PointSize
DEFINE_FUNC	glPolygonOffset		SWI_GL	gl_PolygonOffset
DEFINE_FUNC	glRotatef		SWI_GL	gl_Rotatef
DEFINE_FUNC	glScalef		SWI_GL	gl_Scalef
DEFINE_FUNC	glTexEnvf		SWI_GL	gl_TexEnvf
DEFINE_FUNC	glTexEnvfv		SWI_GL	gl_TexEnvfv
DEFINE_FUNC	glTexParameterf		SWI_GL	gl_TexParameterf
DEFINE_FUNC	glTexParameterfv	SWI_GL	gl_TexParameterfv
DEFINE_FUNC	glTranslatef		SWI_GL	gl_Translatef

/* gl.h - Available in both Common and Common-Lite profiles */
DEFINE_FUNC	glActiveTexture		SWI_GL	gl_ActiveTexture
DEFINE_FUNC	glAlphaFuncx		SWI_GL	gl_AlphaFuncx
DEFINE_FUNC	glBindBuffer		SWI_GL	gl_BindBuffer
DEFINE_FUNC	glBindTexture		SWI_GL	gl_BindTexture
DEFINE_FUNC	glBlendFunc		SWI_GL	gl_BlendFunc
DEFINE_FUNC	glBufferData		SWI_GL	gl_BufferData
DEFINE_FUNC	glBufferSubData		SWI_GL	gl_BufferSubData
DEFINE_FUNC	glClear			SWI_GL	gl_Clear
DEFINE_FUNC	glClearColorx		SWI_GL	gl_ClearColorx
DEFINE_FUNC	glClearDepthx		SWI_GL	gl_ClearDepthx
DEFINE_FUNC	glClearStencil		SWI_GL	gl_ClearStencil
DEFINE_FUNC	glClientActiveTexture	SWI_GL	gl_ClientActiveTexture
DEFINE_FUNC	glClipPlanex		SWI_GL	gl_ClipPlanex
DEFINE_FUNC	glColor4ub		SWI_GL	gl_Color4ub
DEFINE_FUNC	glColor4x		SWI_GL	gl_Color4x
DEFINE_FUNC	glColorMask		SWI_GL	gl_ColorMask
DEFINE_FUNC	glColorPointer		SWI_GL	gl_ColorPointer
DEFINE_FUNC	glCullFace		SWI_GL	gl_CullFace
DEFINE_FUNC	glDeleteBuffers		SWI_GL	gl_DeleteBuffers
DEFINE_FUNC	glDeleteTextures	SWI_GL	gl_DeleteTextures
DEFINE_FUNC	glDepthFunc		SWI_GL	gl_DepthFunc
DEFINE_FUNC	glDepthMask		SWI_GL	gl_DepthMask
DEFINE_FUNC	glDepthRangex		SWI_GL	gl_DepthRangex
DEFINE_FUNC	glDisable		SWI_GL	gl_Disable
DEFINE_FUNC	glDisableClientState	SWI_GL	gl_DisableClientState
DEFINE_FUNC	glDrawArrays		SWI_GL	gl_DrawArrays
DEFINE_FUNC	glDrawElements		SWI_GL	gl_DrawElements
DEFINE_FUNC	glEnable		SWI_GL	gl_Enable
DEFINE_FUNC	glEnableClientState	SWI_GL	gl_EnableClientState
DEFINE_FUNC	glFinish		SWI_GL	gl_Finish
DEFINE_FUNC	glFlush			SWI_GL	gl_Flush
DEFINE_FUNC	glFogx			SWI_GL	gl_Fogx
DEFINE_FUNC	glFogxv			SWI_GL	gl_Fogxv
DEFINE_FUNC	glFrontFace		SWI_GL	gl_FrontFace
DEFINE_FUNC_6	glFrustumx		SWI_GL	gl_Frustumx
DEFINE_FUNC	glGetBooleanv		SWI_GL	gl_GetBooleanv
DEFINE_FUNC	glGetBufferParameteriv	SWI_GL	gl_GetBufferParameteriv
DEFINE_FUNC	glGetClipPlanex		SWI_GL	gl_GetClipPlanex
DEFINE_FUNC	glGenBuffers		SWI_GL	gl_GenBuffers
DEFINE_FUNC	glGenTextures		SWI_GL	gl_GenTextures
DEFINE_FUNC	glGetError		SWI_GL	gl_GetError
DEFINE_FUNC	glGetFixedv		SWI_GL	gl_GetFixedv
DEFINE_FUNC	glGetIntegerv		SWI_GL	gl_GetIntegerv
DEFINE_FUNC	glGetLightxv		SWI_GL	gl_GetLightxv
DEFINE_FUNC	glGetMaterialxv		SWI_GL	gl_GetMaterialxv
DEFINE_FUNC	glGetPointerv		SWI_GL	gl_GetPointerv
DEFINE_FUNC	glGetString		SWI_GL	gl_GetString
DEFINE_FUNC	glGetTexEnviv		SWI_GL	gl_GetTexEnviv
DEFINE_FUNC	glGetTexEnvxv		SWI_GL	gl_GetTexEnvxv
DEFINE_FUNC	glGetTexParameteriv	SWI_GL	gl_GetTexParameteriv
DEFINE_FUNC	glGetTexParameterxv	SWI_GL	gl_GetTexParameterxv
DEFINE_FUNC	glHint			SWI_GL	gl_Hint
DEFINE_FUNC_6	glintAttrib		SWI_GL	gl_intAttrib
DEFINE_FUNC	glintAttribEnable	SWI_GL	gl_intAttribEnable
DEFINE_FUNC	glintAttribGetPointer	SWI_GL	gl_intAttribGetPointer
DEFINE_FUNC_7	glintAttribPointer	SWI_GL	gl_intAttribPointer
DEFINE_FUNC	glintColor		SWI_GL	gl_intColor
DEFINE_FUNC	glIsBuffer		SWI_GL	gl_IsBuffer
DEFINE_FUNC	glIsEnabled		SWI_GL	gl_IsEnabled
DEFINE_FUNC	glIsTexture		SWI_GL	gl_IsTexture
DEFINE_FUNC	glLightModelx		SWI_GL	gl_LightModelx
DEFINE_FUNC	glLightModelxv		SWI_GL	gl_LightModelxv
DEFINE_FUNC	glLightx		SWI_GL	gl_Lightx
DEFINE_FUNC	glLightxv		SWI_GL	gl_Lightxv
DEFINE_FUNC	glLineWidthx		SWI_GL	gl_LineWidthx
DEFINE_FUNC	glLoadIdentity		SWI_GL	gl_LoadIdentity
DEFINE_FUNC	glLoadMatrixx		SWI_GL	gl_LoadMatrixx
DEFINE_FUNC	glLogicOp		SWI_GL	gl_LogicOp
DEFINE_FUNC	glMaterialx		SWI_GL	gl_Materialx
DEFINE_FUNC	glMaterialxv		SWI_GL	gl_Materialxv
DEFINE_FUNC	glMatrixMode		SWI_GL	gl_MatrixMode
DEFINE_FUNC	glMultMatrixx		SWI_GL	gl_MultMatrixx
DEFINE_FUNC	glMultiTexCoord4x	SWI_GL	gl_MultiTexCoord4x
DEFINE_FUNC	glNormal3x		SWI_GL	gl_Normal3x
DEFINE_FUNC	glNormalPointer		SWI_GL	gl_NormalPointer
DEFINE_FUNC_6	glOrthox		SWI_GL	gl_Orthox
DEFINE_FUNC	glPixelStorei		SWI_GL	gl_PixelStorei
DEFINE_FUNC	glPointParameterx	SWI_GL	gl_PointParameterx
DEFINE_FUNC	glPointParameterxv	SWI_GL	gl_PointParameterxv
DEFINE_FUNC	glPointSizex		SWI_GL	gl_PointSizex
DEFINE_FUNC	glPolygonOffsetx	SWI_GL	gl_PolygonOffsetx
DEFINE_FUNC	glPopMatrix		SWI_GL	gl_PopMatrix
DEFINE_FUNC	glPushMatrix		SWI_GL	gl_PushMatrix
DEFINE_FUNC_7	glReadPixels		SWI_GL	gl_ReadPixels
DEFINE_FUNC	glRotatex		SWI_GL	gl_Rotatex
DEFINE_FUNC	glSampleCoverage	SWI_GL	gl_SampleCoverage
DEFINE_FUNC	glSampleCoveragex	SWI_GL	gl_SampleCoveragex
DEFINE_FUNC	glScalex		SWI_GL	gl_Scalex
DEFINE_FUNC	glScissor		SWI_GL	gl_Scissor
DEFINE_FUNC	glShadeModel		SWI_GL	gl_ShadeModel
DEFINE_FUNC	glStencilFunc		SWI_GL	gl_StencilFunc
DEFINE_FUNC	glStencilMask		SWI_GL	gl_StencilMask
DEFINE_FUNC	glStencilOp		SWI_GL	gl_StencilOp
DEFINE_FUNC	glTexCoordPointer	SWI_GL	gl_TexCoordPointer
DEFINE_FUNC	glTexEnvi		SWI_GL	gl_TexEnvi
DEFINE_FUNC	glTexEnvx		SWI_GL	gl_TexEnvx
DEFINE_FUNC	glTexEnviv		SWI_GL	gl_TexEnviv
DEFINE_FUNC	glTexEnvxv		SWI_GL	gl_TexEnvxv
DEFINE_FUNC	glTexParameteri		SWI_GL	gl_TexParameteri
DEFINE_FUNC	glTexParameterx		SWI_GL	gl_TexParameterx
DEFINE_FUNC	glTexParameteriv	SWI_GL	gl_TexParameteriv
DEFINE_FUNC	glTexParameterxv	SWI_GL	gl_TexParameterxv
DEFINE_FUNC	glTranslatex		SWI_GL	gl_Translatex
DEFINE_FUNC	glVertexPointer		SWI_GL	gl_VertexPointer
DEFINE_FUNC	glViewport		SWI_GL	gl_Viewport

/* gl2.h */
DEFINE_FUNC	glAttachShader			SWI_GL gl_AttachShader
DEFINE_FUNC	glBindAttribLocation		SWI_GL gl_BindAttribLocation
DEFINE_FUNC	glBindFramebuffer		SWI_GL gl_BindFramebuffer
DEFINE_FUNC	glBindRenderbuffer		SWI_GL gl_BindRenderbuffer
DEFINE_FUNC	glBlendColor			SWI_GL gl_BlendColor
DEFINE_FUNC	glBlendEquation			SWI_GL gl_BlendEquation
DEFINE_FUNC	glBlendEquationSeparate		SWI_GL gl_BlendEquationSeparate
DEFINE_FUNC	glBlendFuncSeparate		SWI_GL gl_BlendFuncSeparate
DEFINE_FUNC	glCheckFramebufferStatus	SWI_GL gl_CheckFramebufferStatus
DEFINE_FUNC	glCompileShader			SWI_GL gl_CompileShader
DEFINE_FUNC	glCreateShader			SWI_GL gl_CreateShader
DEFINE_FUNC	glCreateProgram			SWI_GL gl_CreateProgram
DEFINE_FUNC	glDeleteFramebuffers		SWI_GL gl_DeleteFramebuffers
DEFINE_FUNC	glDeleteProgram			SWI_GL gl_DeleteProgram
DEFINE_FUNC	glDeleteRenderbuffers		SWI_GL gl_DeleteRenderbuffers
DEFINE_FUNC	glDeleteShader			SWI_GL gl_DeleteShader
DEFINE_FUNC	glDetachShader			SWI_GL gl_DetachShader
DEFINE_FUNC	glDisableVertexAttribArray	SWI_GL gl_DisableVertexAttribArray
DEFINE_FUNC	glEnableVertexAttribArray	SWI_GL gl_EnableVertexAttribArray
DEFINE_FUNC	glFramebufferRenderbuffer	SWI_GL gl_FramebufferRenderbuffer
DEFINE_FUNC_5	glFramebufferTexture2D		SWI_GL gl_FramebufferTexture2D
DEFINE_FUNC	glGenerateMipmap		SWI_GL gl_GenerateMipmap
DEFINE_FUNC	glGenFramebuffers		SWI_GL gl_GenFramebuffers
DEFINE_FUNC	glGenRenderbuffers		SWI_GL gl_GenRenderbuffers
DEFINE_FUNC_7	glGetActiveAttrib		SWI_GL gl_GetActiveAttrib
DEFINE_FUNC_7	glGetActiveUniform		SWI_GL gl_GetActiveUniform
DEFINE_FUNC	glGetAttachedShaders		SWI_GL gl_GetAttachedShaders
DEFINE_FUNC	glGetAttribLocation		SWI_GL gl_GetAttribLocation
DEFINE_FUNC	glGetFramebufferAttachmentParameteriv	SWI_GL gl_GetFramebufferAttachmentParameteriv
DEFINE_FUNC	glGetProgramiv			SWI_GL gl_GetProgramiv
DEFINE_FUNC	glGetProgramInfoLog		SWI_GL gl_GetProgramInfoLog
DEFINE_FUNC	glGetRenderbufferParameteriv	SWI_GL gl_GetRenderbufferParameteriv
DEFINE_FUNC	glGetShaderInfoLog		SWI_GL gl_GetShaderInfoLog
DEFINE_FUNC	glGetShaderiv			SWI_GL gl_GetShaderiv
DEFINE_FUNC	glGetShaderPrecisionFormat	SWI_GL gl_GetShaderPrecisionFormat
DEFINE_FUNC	glGetShaderSource		SWI_GL gl_GetShaderSource
DEFINE_FUNC	glGetUniformfv			SWI_GL gl_GetUniformfv
DEFINE_FUNC	glGetUniformiv			SWI_GL gl_GetUniformiv
DEFINE_FUNC	glGetUniformLocation		SWI_GL gl_GetUniformLocation
DEFINE_FUNC	glGetVertexAttribfv		SWI_GL gl_GetVertexAttribfv
DEFINE_FUNC	glGetVertexAttribiv		SWI_GL gl_GetVertexAttribiv
DEFINE_FUNC	glGetVertexAttribPointerv	SWI_GL gl_GetVertexAttribPointerv
DEFINE_FUNC	glIsFramebuffer			SWI_GL gl_IsFramebuffer
DEFINE_FUNC	glIsProgram			SWI_GL gl_IsProgram
DEFINE_FUNC	glIsRenderbuffer		SWI_GL gl_IsRenderbuffer
DEFINE_FUNC	glIsShader			SWI_GL gl_IsShader
DEFINE_FUNC	glLinkProgram			SWI_GL gl_LinkProgram
DEFINE_FUNC	glReleaseShaderCompiler		SWI_GL gl_ReleaseShaderCompiler
DEFINE_FUNC	glRenderbufferStorage		SWI_GL gl_RenderbufferStorage
DEFINE_FUNC_5	glShaderBinary			SWI_GL gl_ShaderBinary
DEFINE_FUNC	glShaderSource			SWI_GL gl_ShaderSource
DEFINE_FUNC	glStencilFuncSeparate		SWI_GL gl_StencilFuncSeparate
DEFINE_FUNC	glStencilMaskSeparate		SWI_GL gl_StencilMaskSeparate
DEFINE_FUNC	glStencilOpSeparate		SWI_GL gl_StencilOpSeparate
DEFINE_FUNC	glUniform1f			SWI_GL gl_Uniform1f
DEFINE_FUNC	glUniform1fv			SWI_GL gl_Uniform1fv
DEFINE_FUNC	glUniform1i			SWI_GL gl_Uniform1i
DEFINE_FUNC	glUniform1iv			SWI_GL gl_Uniform1iv
DEFINE_FUNC	glUniform2f			SWI_GL gl_Uniform2f
DEFINE_FUNC	glUniform2fv			SWI_GL gl_Uniform2fv
DEFINE_FUNC	glUniform2i			SWI_GL gl_Uniform2i
DEFINE_FUNC	glUniform2iv			SWI_GL gl_Uniform2iv
DEFINE_FUNC	glUniform3f			SWI_GL gl_Uniform3f
DEFINE_FUNC	glUniform3fv			SWI_GL gl_Uniform3fv
DEFINE_FUNC	glUniform3i			SWI_GL gl_Uniform3i
DEFINE_FUNC	glUniform3iv			SWI_GL gl_Uniform3iv
DEFINE_FUNC_5	glUniform4f			SWI_GL gl_Uniform4f
DEFINE_FUNC	glUniform4fv			SWI_GL gl_Uniform4fv
DEFINE_FUNC_5	glUniform4i			SWI_GL gl_Uniform4i
DEFINE_FUNC	glUniform4iv			SWI_GL gl_Uniform4iv
DEFINE_FUNC	glUniformMatrix2fv		SWI_GL gl_UniformMatrix2fv
DEFINE_FUNC	glUniformMatrix3fv		SWI_GL gl_UniformMatrix3fv
DEFINE_FUNC	glUniformMatrix4fv		SWI_GL gl_UniformMatrix4fv
DEFINE_FUNC	glUseProgram			SWI_GL gl_UseProgram
DEFINE_FUNC	glValidateProgram		SWI_GL gl_ValidateProgram
DEFINE_FUNC	glVertexAttrib1f		SWI_GL gl_VertexAttrib1f
DEFINE_FUNC	glVertexAttrib1fv		SWI_GL gl_VertexAttrib1fv
DEFINE_FUNC	glVertexAttrib2f		SWI_GL gl_VertexAttrib2f
DEFINE_FUNC	glVertexAttrib2fv		SWI_GL gl_VertexAttrib2fv
DEFINE_FUNC	glVertexAttrib3f		SWI_GL gl_VertexAttrib3f
DEFINE_FUNC	glVertexAttrib3fv		SWI_GL gl_VertexAttrib3fv
DEFINE_FUNC	glVertexAttrib4f		SWI_GL gl_VertexAttrib4f
DEFINE_FUNC	glVertexAttrib4fv		SWI_GL gl_VertexAttrib4fv
DEFINE_FUNC_6	glVertexAttribPointer		SWI_GL gl_VertexAttribPointer

DEFINE_FUNC	glBindFramebufferOES		SWI_GL gl_BindFramebufferOES
DEFINE_FUNC	glBindRenderbufferOES		SWI_GL gl_BindRenderbufferOES
DEFINE_FUNC	glCheckFramebufferStatusOES	SWI_GL gl_CheckFramebufferStatusOES
DEFINE_FUNC	glCurrentPaletteMatrixOES	SWI_GL gl_CurrentPaletteMatrixOES
DEFINE_FUNC_5	glDeleteFramebuffersOES		SWI_GL gl_DeleteFramebuffersOES
DEFINE_FUNC_5	glDeleteRenderbuffersOES	SWI_GL gl_DeleteRenderbuffersOES
DEFINE_FUNC_5	glDrawTexsOES			SWI_GL gl_DrawTexsOES
DEFINE_FUNC_5	glDrawTexiOES			SWI_GL gl_DrawTexiOES
DEFINE_FUNC_5	glDrawTexxOES			SWI_GL gl_DrawTexxOES
DEFINE_FUNC	glDrawTexsvOES			SWI_GL gl_DrawTexsvOES
DEFINE_FUNC	glDrawTexivOES			SWI_GL gl_DrawTexivOES
DEFINE_FUNC	glDrawTexxvOES			SWI_GL gl_DrawTexxvOES
DEFINE_FUNC_5	glDrawTexfOES			SWI_GL gl_DrawTexfOES
DEFINE_FUNC	glDrawTexfvOES			SWI_GL gl_DrawTexfvOES
DEFINE_FUNC	glPointSizePointerOES		SWI_GL gl_PointSizePointerOES
DEFINE_FUNC	glEGLImageTargetRenderbufferStorageOES	SWI_GL gl_EGLImageTargetRenderbufferStorageOES
DEFINE_FUNC	glEGLImageTargetTexture2DOES	SWI_GL gl_EGLImageTargetTexture2DOES
DEFINE_FUNC	glFramebufferRenderbufferOES	SWI_GL gl_FramebufferRenderbufferOES
DEFINE_FUNC_5	glFramebufferTexture2DOES	SWI_GL gl_FramebufferTexture2DOES
DEFINE_FUNC	glGenFramebuffersOES		SWI_GL gl_GenFramebuffersOES
DEFINE_FUNC	glGetBufferPointervOES		SWI_GL gl_GetBufferPointervOES
DEFINE_FUNC	glGetFramebufferAttachmentParameterivOES SWI_GL gl_GetFramebufferAttachmentParameterivOES
DEFINE_FUNC	glGetRenderbufferParameterivOES	SWI_GL gl_GetRenderbufferParameterivOES
DEFINE_FUNC	glGenRenderbuffersOES		SWI_GL gl_GenRenderbuffersOES
DEFINE_FUNC	glIsFramebufferOES		SWI_GL gl_IsFramebufferOES
DEFINE_FUNC	glIsRenderbufferOES		SWI_GL gl_IsRenderbufferOES
DEFINE_FUNC	glLoadPaletteFromModelViewMatrixOES SWI_GL gl_LoadPaletteFromModelViewMatrixOES
DEFINE_FUNC	glMapBufferOES 			SWI_GL gl_MapBufferOES
DEFINE_FUNC	glMatrixIndexPointerOES 	SWI_GL gl_MatrixIndexPointerOES
DEFINE_FUNC	glRenderbufferStorageOES 	SWI_GL gl_RenderbufferStorageOES
DEFINE_FUNC	glWeightPointerOES 		SWI_GL gl_WeightPointerOES
DEFINE_FUNC	glQueryMatrixxOES 		SWI_GL gl_QueryMatrixxOES
DEFINE_FUNC	glUnmapBufferOES 		SWI_GL gl_UnmapBufferOES

DEFINE_FUNC	glDiscardFramebufferEXT		SWI_GL gl_DiscardFramebufferEXT
