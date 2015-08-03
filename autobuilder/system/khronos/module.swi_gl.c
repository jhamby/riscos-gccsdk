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
#include "GLES/gl.h"
#include "GLES2/gl2.h"
#include "GLES/glext.h"
#include "glxx/glxx_client.h"
#include "../types.h"
#include "../enums.h"

GLenum last_gl_error __attribute__((visibility("hidden"))) = GL_NO_ERROR;

_kernel_oserror *gl_swi(_kernel_swi_regs *r)
{
  switch ((gl_reason_code)r->r[8])
  {
    /* gl.h - Available only in Common profile */
    case gl_AlphaFunc:
      glAlphaFunc((GLenum)r->r[0], RPC_FLOAT_RES(r->r[1]));
      break;
    case gl_ClearColor:
      glClearColor(RPC_FLOAT_RES(r->r[0]), RPC_FLOAT_RES(r->r[1]),
		   RPC_FLOAT_RES(r->r[2]), RPC_FLOAT_RES(r->r[3]));
      break;
    case gl_ClearDepthf:
      glClearDepthf(RPC_FLOAT_RES(r->r[0]));
      break;
    case gl_ClipPlanef:
      glClipPlanef((GLenum)r->r[0], (const GLfloat *)r->r[1]);
      break;
    case gl_Color4f:
      glColor4f(RPC_FLOAT_RES(r->r[0]), RPC_FLOAT_RES(r->r[1]),
		RPC_FLOAT_RES(r->r[2]), RPC_FLOAT_RES(r->r[3]));
      break;
    case gl_DepthRangef:
      glDepthRangef(RPC_FLOAT_RES(r->r[0]), RPC_FLOAT_RES(r->r[1]));
      break;
    case gl_Fogf:
      glFogf((GLenum)r->r[0], RPC_FLOAT_RES(r->r[1]));
      break;
    case gl_Fogfv:
      glFogfv((GLenum)r->r[0], (const GLfloat *)r->r[1]);
      break;
    case gl_Frustumf:
      glFrustumf(RPC_FLOAT_RES(r->r[0]), RPC_FLOAT_RES(r->r[1]), RPC_FLOAT_RES(r->r[2]),
		 RPC_FLOAT_RES(r->r[3]), RPC_FLOAT_RES(r->r[4]), RPC_FLOAT_RES(r->r[5]));
      break;
    case gl_GetClipPlanef:
      /* Param 2 is actually (GLfloat eqn[4]); does it matter?  */
      glGetClipPlanef((GLenum)r->r[0], (GLfloat *)r->r[1]);
      break;
    case gl_GetFloatv:
      glGetFloatv((GLenum)r->r[0], (GLfloat *)r->r[1]);
      break;
    case gl_GetLightfv:
      glGetLightfv((GLenum)r->r[0], (GLenum)r->r[1], (GLfloat *)r->r[2]);
      break;
    case gl_GetMaterialfv:
      glGetMaterialfv ((GLenum)r->r[0], (GLenum)r->r[1], (GLfloat *)r->r[2]);
      break;
    case gl_GetTexEnvfv:
      glGetTexEnvfv((GLenum)r->r[0], (GLenum)r->r[1], (GLfloat *)r->r[2]);
      break;
    case gl_GetTexParameterfv:
      glGetTexParameterfv((GLenum)r->r[0], (GLenum)r->r[1], (GLfloat *)r->r[2]);
      break;
    case gl_LightModelf:
      glLightModelf ((GLenum)r->r[0], RPC_FLOAT_RES(r->r[1]));
      break;
    case gl_LightModelfv:
      glLightModelfv ((GLenum)r->r[0], (const GLfloat *)r->r[1]);
      break;
    case gl_Lightf:
      glLightf ((GLenum)r->r[0], (GLenum)r->r[1], RPC_FLOAT_RES(r->r[2]));
      break;
    case gl_Lightfv:
      glLightfv ((GLenum)r->r[0], (GLenum)r->r[1], (const GLfloat *)r->r[2]);
      break;
    case gl_LineWidth:
      glLineWidth (RPC_FLOAT_RES(r->r[0]));
      break;
    case gl_LoadMatrixf:
      glLoadMatrixf ((const GLfloat *)r->r[0]);
      break;
    case gl_Materialf:
      glMaterialf ((GLenum)r->r[0], (GLenum)r->r[1], RPC_FLOAT_RES(r->r[2]));
      break;
    case gl_Materialfv:
      glMaterialfv ((GLenum)r->r[0], (GLenum)r->r[1], (const GLfloat *)r->r[2]);
      break;
    case gl_MultMatrixf:
      glMultMatrixf ((const GLfloat *)r->r[0]);
      break;
    case gl_MultiTexCoord4f:
      glMultiTexCoord4f ((GLenum)r->r[0], RPC_FLOAT_RES(r->r[1]), RPC_FLOAT_RES(r->r[2]),
					  RPC_FLOAT_RES(r->r[3]), RPC_FLOAT_RES(r->r[4]));
      break;
    case gl_Normal3f:
      glNormal3f (RPC_FLOAT_RES(r->r[0]), RPC_FLOAT_RES(r->r[1]), RPC_FLOAT_RES(r->r[2]));
      break;
    case gl_Orthof:
      glOrthof (RPC_FLOAT_RES(r->r[0]), RPC_FLOAT_RES(r->r[1]), RPC_FLOAT_RES(r->r[2]),
		RPC_FLOAT_RES(r->r[3]), RPC_FLOAT_RES(r->r[4]), RPC_FLOAT_RES(r->r[5]));
      break;
    case gl_PointParameterf:
      glPointParameterf ((GLenum)r->r[0], RPC_FLOAT_RES(r->r[1]));
      break;
    case gl_PointParameterfv:
      glPointParameterfv ((GLenum)r->r[0], (const GLfloat *)r->r[1]);
      break;
    case gl_PointSize:
      glPointSize (RPC_FLOAT_RES(r->r[0]));
      break;
    case gl_PolygonOffset:
      glPolygonOffset (RPC_FLOAT_RES(r->r[0]), RPC_FLOAT_RES(r->r[1]));
      break;
    case gl_Rotatef:
      glRotatef(RPC_FLOAT_RES(r->r[0]), RPC_FLOAT_RES(r->r[1]),
		RPC_FLOAT_RES(r->r[2]), RPC_FLOAT_RES(r->r[3]));
      break;
    case gl_Scalef:
      glScalef (RPC_FLOAT_RES(r->r[0]), RPC_FLOAT_RES(r->r[1]), RPC_FLOAT_RES(r->r[2]));
      break;
    case gl_TexEnvf:
      glTexEnvf ((GLenum)r->r[0], (GLenum)r->r[1], RPC_FLOAT_RES(r->r[2]));
      break;
    case gl_TexEnvfv:
      glTexEnvfv ((GLenum)r->r[0], (GLenum)r->r[1], (const GLfloat *)r->r[2]);
      break;
    case gl_TexParameterf:
      glTexParameterf((GLenum)r->r[0], (GLenum)r->r[1], RPC_FLOAT_RES(r->r[2]));
      break;
    case gl_TexParameterfv:
      glTexParameterfv((GLenum)r->r[0], (GLenum)r->r[1], (const GLfloat *)r->r[2]);
      break;
    case gl_Translatef:
      glTranslatef(RPC_FLOAT_RES(r->r[0]), RPC_FLOAT_RES(r->r[1]), RPC_FLOAT_RES(r->r[2]));
      break;


    /* gl.h - Available in both Common and Common-Lite profiles */
    case gl_ActiveTexture:
      glActiveTexture ((GLenum)r->r[0]);
      break;
    case gl_AlphaFuncx:
      glAlphaFuncx ((GLenum)r->r[0], RPC_FLOAT_RES(r->r[1]));
      break;
    case gl_BindBuffer:
      glBindBuffer ((GLenum)r->r[0], (GLuint)r->r[1]);
      break;
    case gl_BindTexture:
      glBindTexture((GLenum)r->r[0], (GLuint)r->r[1]);
      break;
    case gl_BlendFunc:
      glBlendFunc ((GLenum)r->r[0], (GLenum)r->r[1]);
      break;
    case gl_BufferData:
      glBufferData ((GLenum)r->r[0], (GLsizeiptr)r->r[1],
		    (const GLvoid *)r->r[2], (GLenum)r->r[3]);
      break;
    case gl_BufferSubData:
      glBufferSubData ((GLenum)r->r[0], (GLintptr)r->r[1],
		       (GLsizeiptr)r->r[2], (const GLvoid *)r->r[3]);
      break;
    case gl_Clear:
      glClear((GLbitfield)r->r[0]);
      break;
    case gl_ClearColorx:
      glClearColorx ((GLclampx)r->r[0], (GLclampx)r->r[1], (GLclampx)r->r[2], (GLclampx)r->r[3]);
      break;
    case gl_ClearDepthx:
      glClearDepthx ((GLclampx)r->r[0]);
      break;
    case gl_ClearStencil:
      glClearStencil ((GLint)r->r[0]);
      break;
    case gl_ClientActiveTexture:
      glClientActiveTexture ((GLenum)r->r[0]);
      break;
    case gl_ClipPlanex:
      glClipPlanex ((GLenum)r->r[0], (const GLfixed *)r->r[1]);
      break;
    case gl_Color4ub:
      glColor4ub ((GLubyte)r->r[0], (GLubyte)r->r[1], (GLubyte)r->r[2], (GLubyte)r->r[3]);
      break;
    case gl_Color4x:
      glColor4x ((GLfixed)r->r[0], (GLfixed)r->r[1], (GLfixed)r->r[2], (GLfixed)r->r[3]);
      break;
    case gl_ColorMask:
      glColorMask ((GLboolean)r->r[0], (GLboolean)r->r[1],
		   (GLboolean)r->r[2], (GLboolean)r->r[3]);
      break;
    case gl_ColorPointer:
      glColorPointer ((GLclampx)r->r[0], (GLenum)r->r[1],
		      (GLsizei)r->r[2], (const GLvoid *)r->r[3]);
      break;
    case gl_CompressedTexImage2D: {
      struct glCompressedTexImage2D_params *params = (struct glCompressedTexImage2D_params *)r->r[0];
      glCompressedTexImage2D(params->target,
			     params->level,
			     params->internalformat,
			     params->width,
			     params->height,
			     params->border,
			     params->imageSize,
			     params->data);
      break;
    }
    case gl_CompressedTexSubImage2D: {
      struct glCompressedTexSubImage2D_params *params = (struct glCompressedTexSubImage2D_params *)r->r[0];
      glCompressedTexSubImage2D(params->target,
				params->level,
				params->xoffset,
				params->yoffset,
				params->width,
				params->height,
				params->format,
				params->imageSize,
				params->data);
      break;
    }
    case gl_CopyTexImage2D: {
      struct glCopyTexImage2D_params *params = (struct glCopyTexImage2D_params *)r->r[0];
      glCopyTexImage2D(params->target,
		       params->level,
		       params->internalformat,
		       params->x,
		       params->y,
		       params->width,
		       params->height,
		       params->border);
      break;
    }
    case gl_CopyTexSubImage2D: {
      struct glCopyTexSubImage2D_params *params = (struct glCopyTexSubImage2D_params *)r->r[0];
      glCopyTexSubImage2D(params->target,
			  params->level,
			  params->xoffset,
			  params->yoffset,
			  params->x,
			  params->y,
			  params->width,
			  params->height);
      break;
    }
    case gl_CullFace:
      glCullFace ((GLenum)r->r[0]);
      break;
    case gl_DeleteBuffers:
      glDeleteBuffers ((GLsizei)r->r[0], (const GLuint *)r->r[1]);
      break;
    case gl_DeleteTextures:
      glDeleteTextures ((GLsizei)r->r[0], (const GLuint *)r->r[1]);
      break;
    case gl_DepthFunc:
      glDepthFunc ((GLenum)r->r[0]);
      break;
    case gl_DepthMask:
      glDepthMask ((GLboolean)r->r[0]);
      break;
    case gl_DepthRangex:
      glDepthRangex ((GLclampx)r->r[0], (GLclampx)r->r[1]);
      break;
    case gl_Disable:
      glDisable ((GLenum)r->r[0]);
      break;
    case gl_DisableClientState:
      glDisableClientState ((GLenum)r->r[0]);
      break;
    case gl_DrawArrays:
      glDrawArrays((GLenum)r->r[0], (GLint)r->r[1], (GLsizei)r->r[2]);
      break;
    case gl_DrawElements:
      glDrawElements ((GLenum)r->r[0], (GLsizei)r->r[1], (GLenum)r->r[2], (const GLvoid *)r->r[3]);
      break;
    case gl_Enable:
      glEnable((GLenum)r->r[0]);
      break;
    case gl_EnableClientState:
      glEnableClientState((GLenum)r->r[0]);
      break;
    case gl_Finish:
      glFinish();
      break;
    case gl_Flush:
      glFlush();
      break;
    case gl_Fogx:
      glFogx ((GLenum)r->r[0], (GLfixed)r->r[1]);
      break;
    case gl_Fogxv:
      glFogxv ((GLenum)r->r[0], (const GLfixed *)r->r[1]);
      break;
    case gl_FrontFace:
      glFrontFace ((GLenum)r->r[0]);
      break;
    case gl_Frustumx:
      glFrustumx ((GLfixed)r->r[0], (GLfixed)r->r[1], (GLfixed)r->r[2],
		  (GLfixed)r->r[3], (GLfixed)r->r[4], (GLfixed)r->r[5]);
      break;
    case gl_GetBooleanv:
      glGetBooleanv ((GLenum)r->r[0], (GLboolean *)r->r[1]);
      break;
    case gl_GetBufferParameteriv:
      glGetBufferParameteriv ((GLenum)r->r[0], (GLenum)r->r[1], (GLint *)r->r[2]);
      break;
    case gl_GetClipPlanex:
      /* FIXME: Param 2 is actually (GLfixed eqn[4]); does it matter?  */
      glGetClipPlanex ((GLenum)r->r[0], (GLfixed *)r->r[1]);
      break;
    case gl_GenBuffers:
      glGenBuffers ((GLsizei)r->r[0], (GLuint *)r->r[1]);
      break;
    case gl_GenTextures:
      glGenTextures((GLsizei)r->r[0], (GLuint *)r->r[1]);
      break;
    case gl_GetError: {
      GLenum error = glGetError();

      if (error != GL_NO_ERROR)
	last_gl_error = error;

      r->r[0] = (int)error;
      break;
    }
    case gl_GetFixedv:
      glGetFixedv ((GLenum)r->r[0], (GLfixed *)r->r[1]);
      break;
    case gl_GetIntegerv:
      glGetIntegerv ((GLenum)r->r[0], (GLint *)r->r[1]);
      break;
    case gl_GetLightxv:
      glGetLightxv ((GLenum)r->r[0], (GLenum)r->r[1], (GLfixed *)r->r[2]);
      break;
    case gl_GetMaterialxv:
      glGetMaterialxv ((GLenum)r->r[0], (GLenum)r->r[1], (GLfixed *)r->r[2]);
      break;
    case gl_GetPointerv:
      glGetPointerv ((GLenum)r->r[0], (GLvoid **)r->r[1]);
      break;
    case gl_GetString:
      r->r[0] = (int)glGetString ((GLenum)r->r[0]);
      break;
    case gl_GetTexEnviv:
      glGetTexEnviv ((GLenum)r->r[0], (GLenum)r->r[1], (GLint *)r->r[2]);
      break;
    case gl_GetTexEnvxv:
      glGetTexEnvxv ((GLenum)r->r[0], (GLenum)r->r[1], (GLfixed *)r->r[2]);
      break;
    case gl_GetTexParameteriv:
      glGetTexParameteriv ((GLenum)r->r[0], (GLenum)r->r[1], (GLint *)r->r[2]);
      break;
    case gl_GetTexParameterxv:
      glGetTexParameterxv ((GLenum)r->r[0], (GLenum)r->r[1], (GLfixed *)r->r[2]);
      break;
    case gl_Hint:
      glHint((GLenum)r->r[0], (GLenum)r->r[1]);
      break;
    case gl_intAttrib:
      glintAttrib ((uint32_t)r->r[0], (uint32_t)r->r[1],
		   RPC_FLOAT_RES(r->r[2]), RPC_FLOAT_RES(r->r[3]),
		   RPC_FLOAT_RES(r->r[4]), RPC_FLOAT_RES(r->r[5]));
      break;
    case gl_intAttribEnable:
      glintAttribEnable((uint32_t)r->r[0], (uint32_t)r->r[1], (bool)r->r[2]);
      break;
    case gl_intAttribGetPointer:
      r->r[0] = (int)glintAttribGetPointer((uint32_t)r->r[0], (uint32_t)r->r[1]);
      break;
    case gl_intAttribPointer:
      glintAttribPointer ((uint32_t)r->r[0], (uint32_t)r->r[1],
			  (GLint)r->r[2], (GLenum)r->r[3],
			  (GLboolean)r->r[4], (GLsizei)r->r[5],
			  (const GLvoid *)r->r[6]);
      break;
    case gl_intColor:
      glintColor(RPC_FLOAT_RES(r->r[0]), RPC_FLOAT_RES(r->r[1]),
		 RPC_FLOAT_RES(r->r[2]), RPC_FLOAT_RES(r->r[3]));
      break;
    case gl_IsBuffer:
      r->r[0] = (int)glIsBuffer ((GLuint)r->r[0]);
      break;
    case gl_IsEnabled:
      r->r[0] = (int)glIsEnabled ((GLenum)r->r[0]);
      break;
    case gl_IsTexture:
      r->r[0] = (int)glIsTexture ((GLuint)r->r[0]);
      break;
    case gl_LightModelx:
      glLightModelx ((GLenum)r->r[0], (GLfixed)r->r[1]);
      break;
    case gl_LightModelxv:
      glLightModelxv ((GLenum)r->r[0], (const GLfixed *)r->r[1]);
      break;
    case gl_Lightx:
      glLightx ((GLenum)r->r[0], (GLenum)r->r[1], (GLfixed)r->r[2]);
      break;
    case gl_Lightxv:
      glLightxv ((GLenum)r->r[0], (GLenum)r->r[1], (const GLfixed *)r->r[2]);
      break;
    case gl_LineWidthx:
      glLineWidthx ((GLfixed)r->r[0]);
      break;
    case gl_LoadIdentity:
      glLoadIdentity();
      break;
    case gl_LoadMatrixx:
      glLoadMatrixx ((const GLfixed *)r->r[0]);
      break;
    case gl_LogicOp:
      glLogicOp ((GLenum)r->r[0]);
      break;
    case gl_Materialx:
      glMaterialx ((GLenum)r->r[0], (GLenum)r->r[1], (GLfixed)r->r[2]);
      break;
    case gl_Materialxv:
      glMaterialxv ((GLenum)r->r[0], (GLenum)r->r[1], (const GLfixed *)r->r[2]);
      break;
    case gl_MatrixMode:
      glMatrixMode((GLenum)r->r[0]);
      break;
    case gl_MultMatrixx:
      glMultMatrixx ((const GLfixed *)r->r[0]);
      break;
    case gl_MultiTexCoord4x:
      glMultiTexCoord4x ((GLenum)r->r[0], (GLfixed)r->r[1],
			 (GLfixed)r->r[2], (GLfixed)r->r[3],
			 (GLfixed)r->r[4]);
      break;
    case gl_Normal3x:
      glNormal3x ((GLfixed)r->r[0], (GLfixed)r->r[1], (GLfixed)r->r[2]);
      break;
    case gl_NormalPointer:
      glNormalPointer ((GLenum)r->r[0], (GLsizei)r->r[1], (const GLvoid *)r->r[2]);
      break;
    case gl_Orthox:
      glOrthox ((GLfixed)r->r[0], (GLfixed)r->r[1],
		(GLfixed)r->r[2], (GLfixed)r->r[3],
		(GLfixed)r->r[4], (GLfixed)r->r[5]);
      break;
    case gl_PixelStorei:
      glPixelStorei ((GLenum)r->r[0], (GLint)r->r[1]);
      break;
    case gl_PointParameterx:
      glPointParameterx ((GLenum)r->r[0], (GLfixed)r->r[1]);
      break;
    case gl_PointParameterxv:
      glPointParameterxv ((GLenum)r->r[0], (const GLfixed *)r->r[1]);
      break;
    case gl_PointSizex:
      glPointSizex ((GLfixed)r->r[0]);
      break;
    case gl_PolygonOffsetx:
      glPolygonOffsetx ((GLfixed)r->r[0], (GLfixed)r->r[1]);
      break;
    case gl_PopMatrix:
      glPopMatrix ();
      break;
    case gl_PushMatrix:
      glPushMatrix ();
      break;
    case gl_ReadPixels:
      glReadPixels ((GLint)r->r[0], (GLint)r->r[1],
		    (GLsizei)r->r[2], (GLsizei)r->r[3],
		    (GLenum)r->r[4], (GLenum)r->r[5], (GLvoid *)r->r[6]);
      break;
    case gl_Rotatex:
      glRotatex ((GLfixed)r->r[0], (GLfixed)r->r[1], (GLfixed)r->r[2], (GLfixed)r->r[3]);
      break;
    case gl_SampleCoverage:
      glSampleCoverage ((GLclampf)r->r[0], (GLboolean)r->r[1]);
      break;
    case gl_SampleCoveragex:
      glSampleCoveragex ((GLclampx)r->r[0], (GLboolean)r->r[1]);
      break;
    case gl_Scalex:
      glScalex ((GLfixed)r->r[0], (GLfixed)r->r[1], (GLfixed)r->r[2]);
      break;
    case gl_Scissor:
      glScissor ((GLint)r->r[0], (GLint)r->r[1], (GLsizei)r->r[2], (GLsizei)r->r[3]);
      break;
    case gl_ShadeModel:
      glShadeModel ((GLenum)r->r[0]);
      break;
    case gl_StencilFunc:
      glStencilFunc ((GLenum)r->r[0], (GLint)r->r[1], (GLuint)r->r[2]);
      break;
    case gl_StencilMask:
      glStencilMask ((GLuint)r->r[0]);
      break;
    case gl_StencilOp:
      glStencilOp ((GLenum)r->r[0], (GLenum)r->r[1], (GLenum)r->r[2]);
      break;
    case gl_TexCoordPointer:
      glTexCoordPointer((GLint)r->r[0], (GLenum)r->r[1],
			(GLsizei)r->r[2], (const GLvoid *)r->r[3]);
      break;
    case gl_TexEnvi:
      glTexEnvi ((GLenum)r->r[0], (GLenum)r->r[1], (GLint)r->r[2]);
      break;
    case gl_TexEnvx:
      glTexEnvx ((GLenum)r->r[0], (GLenum)r->r[1], (GLfixed)r->r[2]);
      break;
    case gl_TexEnviv:
      glTexEnviv ((GLenum)r->r[0], (GLenum)r->r[1], (const GLint *)r->r[2]);
      break;
    case gl_TexEnvxv:
      glTexEnvxv ((GLenum)r->r[0], (GLenum)r->r[1], (const GLfixed *)r->r[2]);
      break;
    case gl_TexImage2D: {
      struct glTexImage2D_params *params = (struct glTexImage2D_params *)r->r[0];
      glTexImage2D(params->target,
		   params->level,
		   params->internalformat,
		   params->width,
		   params->height,
		   params->border,
		   params->format,
		   params->type,
		   params->pixels);
      break;
    }
    case gl_TexParameteri:
      glTexParameteri ((GLenum)r->r[0], (GLenum)r->r[1], (GLint)r->r[2]);
      break;
    case gl_TexParameterx:
      glTexParameterx ((GLenum)r->r[0], (GLenum)r->r[1], (GLfixed)r->r[2]);
      break;
    case gl_TexParameteriv:
      glTexParameteriv ((GLenum)r->r[0], (GLenum)r->r[1], (const GLint *)r->r[2]);
      break;
    case gl_TexParameterxv:
      glTexParameterxv ((GLenum)r->r[0], (GLenum)r->r[1], (const GLfixed *)r->r[2]);
      break;
    case gl_TexSubImage2D: {
      struct glTexSubImage2D_params *params = (struct glTexSubImage2D_params *)r->r[0];
      glTexSubImage2D(params->target,
		      params->level,
		      params->xoffset,
		      params->yoffset,
		      params->width,
		      params->height,
		      params->format,
		      params->type,
		      params->pixels);
      break;
    }
    case _texSubImage2DAsync: {
      struct texSubImage2DAsync_params *params = (struct texSubImage2DAsync_params *)r->r[0];
      texSubImage2DAsync (params->target,
			  params->level,
			  params->xoffset,
			  params->yoffset,
			  params->width,
			  params->height,
			  params->format,
			  params->type,
			  params->hpixels);
      break;
    }
    case gl_Translatex:
      glTranslatex((GLfixed)r->r[0], (GLfixed)r->r[1], (GLfixed)r->r[2]);
      break;
    case gl_VertexPointer:
      glVertexPointer((GLint)r->r[0], (GLenum)r->r[1],
		      (GLsizei)r->r[2], (const GLvoid *)r->r[3]);
      break;
    case gl_Viewport:
      glViewport((GLint)r->r[0], (GLint)r->r[1], (GLsizei)r->r[2], (GLsizei)r->r[3]);
      break;


    /* gl2.h */
    case gl_AttachShader:
      glAttachShader((GLuint)r->r[0], (GLuint)r->r[1]);
      break;
    case gl_BindAttribLocation:
      glBindAttribLocation((GLuint)r->r[0], (GLuint)r->r[1], (const GLchar*)r->r[2]);
      break;
    case gl_BindFramebuffer:
      glBindFramebuffer ((GLenum)r->r[0], (GLuint)r->r[1]);
      break;
    case gl_BindRenderbuffer:
      glBindRenderbuffer ((GLenum)r->r[0], (GLuint)r->r[1]);
      break;
    case gl_BlendColor:
      glBlendColor ((GLclampf)r->r[0], (GLclampf)r->r[1],
		    (GLclampf)r->r[2], (GLclampf)r->r[3]);
      break;
    case gl_BlendEquation:
      glBlendEquation ((GLenum)r->r[0]);
      break;
    case gl_BlendEquationSeparate:
      glBlendEquationSeparate ((GLenum)r->r[0], (GLenum)r->r[1]);
      break;
    case gl_BlendFuncSeparate:
      glBlendFuncSeparate ((GLenum)r->r[0], (GLenum)r->r[1],
			   (GLenum)r->r[2], (GLenum)r->r[3]);
      break;
    case gl_CheckFramebufferStatus:
      r->r[0] = (int)glCheckFramebufferStatus ((GLenum)r->r[0]);
      break;
    case gl_CompileShader:
      glCompileShader((GLenum)r->r[0]);
      break;
    case gl_CreateProgram:
      r->r[0] = (int)glCreateProgram();
      break;
    case gl_CreateShader:
      r->r[0] = (int)glCreateShader((GLenum)r->r[0]);
      break;
    case gl_DeleteFramebuffers:
      glDeleteFramebuffers ((GLsizei)r->r[0], (const GLuint*)r->r[1]);
      break;
    case gl_DeleteProgram:
      glDeleteProgram((GLuint)r->r[0]);
      break;
    case gl_DeleteRenderbuffers:
      glDeleteRenderbuffers((GLsizei)r->r[0], (const GLuint*)r->r[1]);
      break;
    case gl_DeleteShader:
      glDeleteShader((GLenum)r->r[0]);
      break;
    case gl_DetachShader:
      glDetachShader((GLuint)r->r[0], (GLuint)r->r[1]);
      break;
    case gl_DisableVertexAttribArray:
      glDisableVertexAttribArray ((GLuint)r->r[0]);
      break;
    case gl_EnableVertexAttribArray:
      glEnableVertexAttribArray((GLuint)r->r[0]);
      break;
    case gl_FramebufferRenderbuffer:
      glFramebufferRenderbuffer ((GLenum)r->r[0], (GLenum)r->r[1],
				 (GLenum)r->r[2], (GLuint)r->r[3]);
      break;
    case gl_FramebufferTexture2D:
      glFramebufferTexture2D ((GLenum)r->r[0], (GLenum)r->r[1],
			      (GLenum)r->r[2], (GLuint)r->r[3],
			      (GLint)r->r[4]);
      break;
    case gl_GenerateMipmap:
      glGenerateMipmap ((GLenum)r->r[0]);
      break;
    case gl_GenFramebuffers:
      glGenFramebuffers ((GLsizei)r->r[0], (GLuint*)r->r[1]);
      break;
    case gl_GenRenderbuffers:
      glGenRenderbuffers ((GLsizei)r->r[0], (GLuint*)r->r[1]);
      break;
    case gl_GetActiveAttrib:
      glGetActiveAttrib ((GLuint)r->r[0], (GLuint)r->r[1], (GLsizei)r->r[2],
			 (GLsizei*)r->r[3], (GLint*)r->r[4], (GLenum*)r->r[5],
			 (GLchar*)r->r[6]);
      break;
    case gl_GetActiveUniform:
      glGetActiveUniform ((GLuint)r->r[0], (GLuint)r->r[1], (GLsizei)r->r[2],
			  (GLsizei*)r->r[3], (GLint*)r->r[4], (GLenum*)r->r[5],
			  (GLchar*)r->r[6]);
      break;
    case gl_GetAttachedShaders:
      glGetAttachedShaders ((GLuint)r->r[0], (GLsizei)r->r[1],
			    (GLsizei*)r->r[2], (GLuint*)r->r[3]);
      break;
    case gl_GetAttribLocation:
      r->r[0] = (int)glGetAttribLocation ((GLuint)r->r[0], (const GLchar*)r->r[1]);
      break;
    case gl_GetFramebufferAttachmentParameteriv:
      glGetFramebufferAttachmentParameteriv ((GLenum)r->r[0], (GLenum)r->r[1],
					     (GLenum)r->r[2], (GLint*)r->r[3]);
      break;
    case gl_GetProgramiv:
      glGetProgramiv((GLuint)r->r[0], (GLenum)r->r[1], (GLint*)r->r[2]);
      break;
    case gl_GetProgramInfoLog:
      glGetProgramInfoLog((GLuint)r->r[0], (GLsizei)r->r[1],
			  (GLsizei*)r->r[2], (GLchar*)r->r[3]);
      break;
    case gl_GetRenderbufferParameteriv:
      glGetRenderbufferParameteriv((GLenum)r->r[0], (GLenum)r->r[1], (GLint*)r->r[2]);
      break;
    case gl_GetShaderiv:
      glGetShaderiv((GLuint)r->r[0], (GLenum)r->r[1], (GLint*)r->r[2]);
      break;
    case gl_GetShaderInfoLog:
      glGetShaderInfoLog((GLuint)r->r[0], (GLsizei)r->r[1],
			 (GLsizei*)r->r[2], (GLchar*)r->r[3]);
      break;
    case gl_GetShaderPrecisionFormat:
      glGetShaderPrecisionFormat ((GLenum)r->r[0], (GLenum)r->r[1],
				  (GLint*)r->r[2], (GLint*)r->r[3]);
      break;
    case gl_GetShaderSource:
      glGetShaderSource ((GLuint)r->r[0], (GLsizei)r->r[1],
			 (GLsizei*)r->r[2], (GLchar*)r->r[3]);
      break;
    case gl_GetUniformfv:
      glGetUniformfv ((GLuint)r->r[0], (GLint)r->r[1], (GLfloat*)r->r[2]);
      break;
    case gl_GetUniformiv:
      glGetUniformiv ((GLuint)r->r[0], (GLint)r->r[1], (GLint*)r->r[2]);
      break;
    case gl_GetUniformLocation:
      r->r[0] = (int)glGetUniformLocation ((GLuint)r->r[0], (const GLchar*)r->r[1]);
      break;
    case gl_GetVertexAttribfv:
      glGetVertexAttribfv ((GLuint)r->r[0], (GLenum)r->r[1], (GLfloat*)r->r[2]);
      break;
    case gl_GetVertexAttribiv:
      glGetVertexAttribiv ((GLuint)r->r[0], (GLenum)r->r[1], (GLint*)r->r[2]);
      break;
    case gl_GetVertexAttribPointerv:
      glGetVertexAttribPointerv ((GLuint)r->r[0], (GLenum)r->r[1], (GLvoid**)r->r[2]);
      break;
    case gl_IsFramebuffer:
      r->r[0] = (int)glIsFramebuffer((GLuint)r->r[0]);
      break;
    case gl_IsProgram:
      r->r[0] = (int)glIsProgram ((GLuint)r->r[0]);
      break;
    case gl_IsRenderbuffer:
      r->r[0] = (int)glIsRenderbuffer ((GLuint)r->r[0]);
      break;
    case gl_IsShader:
      r->r[0] = (int)glIsShader ((GLuint)r->r[0]);
      break;
    case gl_LinkProgram:
      glLinkProgram((GLuint)r->r[0]);
      break;
    case gl_ReleaseShaderCompiler:
      glReleaseShaderCompiler();
      break;
    case gl_RenderbufferStorage:
      glRenderbufferStorage ((GLenum)r->r[0], (GLenum)r->r[1],
			     (GLsizei)r->r[2], (GLsizei)r->r[3]);
      break;
    case gl_ShaderBinary:
      glShaderBinary ((GLsizei)r->r[0], (const GLuint*)r->r[1],
		      (GLenum)r->r[2], (const GLvoid*)r->r[3],
		      (GLsizei)r->r[4]);
      break;
    case gl_ShaderSource:
      glShaderSource((GLuint)r->r[0], (GLsizei)r->r[1],
		     (const GLchar**)r->r[2], (const GLint*)r->r[3]);
      break;
    case gl_StencilFuncSeparate:
      glStencilFuncSeparate ((GLenum)r->r[0], (GLenum)r->r[1],
			     (GLint)r->r[2], (GLuint)r->r[3]);
      break;
    case gl_StencilMaskSeparate:
      glStencilMaskSeparate ((GLenum)r->r[0], (GLuint)r->r[1]);
      break;
    case gl_StencilOpSeparate:
      glStencilOpSeparate ((GLenum)r->r[0], (GLenum)r->r[1],
			   (GLenum)r->r[2], (GLenum)r->r[3]);
      break;
    case gl_Uniform1f:
      glUniform1f ((GLint)r->r[0], RPC_FLOAT_RES(r->r[1]));
      break;
    case gl_Uniform1fv:
      glUniform1fv ((GLint)r->r[0], (GLsizei)r->r[1], (const GLfloat*)r->r[2]);
      break;
    case gl_Uniform1i:
      glUniform1i ((GLint)r->r[0], (GLint)r->r[1]);
      break;
    case gl_Uniform1iv:
      glUniform1iv ((GLint)r->r[0], (GLsizei)r->r[1], (const GLint*)r->r[2]);
      break;
    case gl_Uniform2f:
      glUniform2f ((GLint)r->r[0], RPC_FLOAT_RES(r->r[1]), RPC_FLOAT_RES(r->r[2]));
      break;
    case gl_Uniform2fv:
      glUniform2fv ((GLint)r->r[0], (GLsizei)r->r[1], (const GLfloat*)r->r[2]);
      break;
    case gl_Uniform2i:
      glUniform2i ((GLint)r->r[0], (GLint)r->r[1], (GLint)r->r[2]);
      break;
    case gl_Uniform2iv:
      glUniform2iv ((GLint)r->r[0], (GLsizei)r->r[1], (const GLint*)r->r[2]);
      break;
    case gl_Uniform3f:
      glUniform3f ((GLint)r->r[0], RPC_FLOAT_RES(r->r[1]),
		   RPC_FLOAT_RES(r->r[2]), RPC_FLOAT_RES(r->r[3]));
      break;
    case gl_Uniform3fv:
      glUniform3fv ((GLint)r->r[0], (GLsizei)r->r[1], (const GLfloat*)r->r[2]);
      break;
    case gl_Uniform3i:
      glUniform3i ((GLint)r->r[0], (GLint)r->r[1], (GLint)r->r[2], (GLint)r->r[3]);
      break;
    case gl_Uniform3iv:
      glUniform3iv ((GLint)r->r[0], (GLsizei)r->r[1], (const GLint*)r->r[2]);
      break;
    case gl_Uniform4f:
      glUniform4f ((GLint)r->r[0], RPC_FLOAT_RES(r->r[1]), RPC_FLOAT_RES(r->r[2]),
				   RPC_FLOAT_RES(r->r[3]), RPC_FLOAT_RES(r->r[4]));
      break;
    case gl_Uniform4fv:
      glUniform4fv ((GLint)r->r[0], (GLsizei)r->r[1], (const GLfloat*)r->r[2]);
      break;
    case gl_Uniform4i:
      glUniform4i ((GLint)r->r[0], (GLint)r->r[1], (GLint)r->r[2],
		   (GLint)r->r[3], (GLint)r->r[4]);
      break;
    case gl_Uniform4iv:
      glUniform4iv ((GLint)r->r[0], (GLsizei)r->r[1], (const GLint*)r->r[2]);
      break;
    case gl_UniformMatrix2fv:
      glUniformMatrix2fv ((GLint)r->r[0], (GLsizei)r->r[1],
			  (GLboolean)r->r[2], (const GLfloat*)r->r[3]);
      break;
    case gl_UniformMatrix3fv:
      glUniformMatrix3fv ((GLint)r->r[0], (GLsizei)r->r[1],
			  (GLboolean)r->r[2], (const GLfloat*)r->r[3]);
      break;
    case gl_UniformMatrix4fv:
      glUniformMatrix4fv ((GLint)r->r[0], (GLsizei)r->r[1],
			  (GLboolean)r->r[2], (const GLfloat*)r->r[3]);
      break;
    case gl_UseProgram:
      glUseProgram((GLuint)r->r[0]);
      break;
    case gl_ValidateProgram:
      glValidateProgram ((GLuint)r->r[0]);
      break;
    case gl_VertexAttrib1f:
      glVertexAttrib1f ((GLuint)r->r[0], RPC_FLOAT_RES(r->r[1]));
      break;
    case gl_VertexAttrib1fv:
      glVertexAttrib1fv ((GLuint)r->r[0], (const GLfloat*)r->r[1]);
      break;
    case gl_VertexAttrib2f:
      glVertexAttrib2f ((GLuint)r->r[0], RPC_FLOAT_RES(r->r[1]), RPC_FLOAT_RES(r->r[2]));
      break;
    case gl_VertexAttrib2fv:
      glVertexAttrib2fv ((GLuint)r->r[0], (const GLfloat*)r->r[1]);
      break;
    case gl_VertexAttrib3f:
      glVertexAttrib3f ((GLuint)r->r[0], RPC_FLOAT_RES(r->r[1]),
			RPC_FLOAT_RES(r->r[2]), RPC_FLOAT_RES(r->r[3]));
      break;
    case gl_VertexAttrib3fv:
      glVertexAttrib3fv ((GLuint)r->r[0], (const GLfloat*)r->r[1]);
      break;
    case gl_VertexAttrib4f:
      glVertexAttrib4f ((GLuint)r->r[0], RPC_FLOAT_RES(r->r[1]), RPC_FLOAT_RES(r->r[2]),
					 RPC_FLOAT_RES(r->r[3]), RPC_FLOAT_RES(r->r[4]));
      break;
    case gl_VertexAttrib4fv:
      glVertexAttrib4fv ((GLuint)r->r[0], (const GLfloat*)r->r[1]);
      break;
    case gl_VertexAttribPointer:
      glVertexAttribPointer((GLuint)r->r[0], (GLint)r->r[1], (GLenum)r->r[2],
			    (GLboolean)r->r[3], (GLsizei)r->r[4], (const GLvoid*)r->r[5]);
      break;



    case gl_BindFramebufferOES:
      glBindFramebufferOES ((GLenum)r->r[0], (GLuint)r->r[1]);
      break;
    case gl_BindRenderbufferOES:
      glBindRenderbufferOES ((GLenum)r->r[0], (GLuint)r->r[1]);
      break;
    case gl_CheckFramebufferStatusOES:
      glCheckFramebufferStatusOES ((GLenum)r->r[0]);
      break;
    case gl_CurrentPaletteMatrixOES:
      glCurrentPaletteMatrixOES ((GLuint)r->r[0]);
      break;
    case gl_DeleteFramebuffersOES:
      glDeleteFramebuffersOES ((GLsizei)r->r[0], (const GLuint*)r->r[1]);
      break;
    case gl_DeleteRenderbuffersOES:
      glDeleteRenderbuffersOES ((GLsizei)r->r[0], (const GLuint*)r->r[1]);
      break;
    case gl_DrawTexsOES:
      glDrawTexsOES ((GLshort)r->r[0], (GLshort)r->r[1], (GLshort)r->r[2],
		     (GLshort)r->r[3], (GLshort)r->r[4]);
      break;
    case gl_DrawTexiOES:
      glDrawTexiOES ((GLint)r->r[0], (GLint)r->r[1], (GLint)r->r[2],
		     (GLint)r->r[3], (GLint)r->r[4]);
      break;
    case gl_DrawTexxOES:
      glDrawTexxOES ((GLfixed)r->r[0], (GLfixed)r->r[1], (GLfixed)r->r[2],
		     (GLfixed)r->r[3], (GLfixed)r->r[4]);
      break;
    case gl_DrawTexsvOES:
      glDrawTexsvOES ((const GLshort *)r->r[0]);
      break;
    case gl_DrawTexivOES:
      glDrawTexivOES ((const GLint *)r->r[0]);
      break;
    case gl_DrawTexxvOES:
      glDrawTexxvOES ((const GLfixed *)r->r[0]);
      break;
    case gl_DrawTexfOES:
      glDrawTexfOES (RPC_FLOAT_RES(r->r[0]), RPC_FLOAT_RES(r->r[1]), RPC_FLOAT_RES(r->r[2]),
		     RPC_FLOAT_RES(r->r[3]), RPC_FLOAT_RES(r->r[4]));
      break;
    case gl_DrawTexfvOES:
      glDrawTexfvOES ((const GLfloat *)r->r[0]);
      break;
    case gl_PointSizePointerOES:
      glPointSizePointerOES ((GLenum)r->r[0], (GLsizei)r->r[1], (const GLvoid *)r->r[2]);
      break;
    case gl_EGLImageTargetRenderbufferStorageOES:
      glEGLImageTargetRenderbufferStorageOES ((GLenum)r->r[0], (GLeglImageOES)r->r[1]);
      break;
    case gl_EGLImageTargetTexture2DOES:
      glEGLImageTargetTexture2DOES ((GLenum)r->r[0], (GLeglImageOES)r->r[1]);
      break;
    case gl_FramebufferRenderbufferOES:
      glFramebufferRenderbufferOES ((GLenum)r->r[0], (GLenum)r->r[1],
				    (GLenum)r->r[2], (GLuint)r->r[3]);
      break;
    case gl_FramebufferTexture2DOES:
      glFramebufferTexture2DOES ((GLenum)r->r[0], (GLenum)r->r[1],
				 (GLenum)r->r[2], (GLuint)r->r[3], (GLint)r->r[4]);
      break;
    case gl_GenFramebuffersOES:
      glGenFramebuffersOES ((GLsizei)r->r[0], (GLuint*)r->r[1]);
      break;
    case gl_GenRenderbuffersOES:
      glGenRenderbuffersOES ((GLsizei)r->r[0], (GLuint*)r->r[1]);
      break;
    case gl_GetBufferPointervOES:
      glGetBufferPointervOES ((GLenum)r->r[0], (GLenum)r->r[1], (GLvoid **)r->r[2]);
      break;
    case gl_GetFramebufferAttachmentParameterivOES:
      glGetFramebufferAttachmentParameterivOES ((GLenum)r->r[0], (GLenum)r->r[1],
						(GLenum)r->r[2], (GLint*)r->r[3]);
      break;
    case gl_GetRenderbufferParameterivOES:
      glGetRenderbufferParameterivOES ((GLenum)r->r[0], (GLenum)r->r[1], (GLint*)r->r[2]);
      break;
    case gl_IsFramebufferOES:
      glIsFramebufferOES ((GLuint)r->r[0]);
      break;
    case gl_IsRenderbufferOES:
      glIsRenderbufferOES ((GLuint)r->r[0]);
      break;
    case gl_LoadPaletteFromModelViewMatrixOES:
      glLoadPaletteFromModelViewMatrixOES ();
      break;
    case gl_MapBufferOES:
      r->r[0] = (int)glMapBufferOES ((GLenum)r->r[0], (GLenum)r->r[1]);
      break;
    case gl_MatrixIndexPointerOES:
      glMatrixIndexPointerOES ((GLint)r->r[0], (GLenum)r->r[1],
			       (GLsizei)r->r[2], (const GLvoid *)r->r[3]);
      break;
    case gl_GenerateMipmapOES:
      glGenerateMipmapOES ((GLenum)r->r[0]);
      break;
    case gl_RenderbufferStorageOES:
      glRenderbufferStorageOES ((GLenum)r->r[0], (GLenum)r->r[1],
				(GLsizei)r->r[2], (GLsizei)r->r[3]);
      break;
    case gl_WeightPointerOES:
      glWeightPointerOES ((GLint)r->r[0], (GLenum)r->r[1],
			  (GLsizei)r->r[2], (const GLvoid *)r->r[3]);
      break;
    case gl_QueryMatrixxOES:
      r->r[0] = (int)glQueryMatrixxOES((GLfixed *)r->r[0], (GLint *)r->r[1]);
      break;
    case gl_UnmapBufferOES:
      r->r[0] = (int)glUnmapBufferOES ((GLenum)r->r[0]);
      break;
    case gl_DiscardFramebufferEXT:
      glDiscardFramebufferEXT((GLenum)r->r[0], (GLsizei)r->r[1], (const GLenum *)r->r[2]);
      break;
    default:
      return khronos_bad_gl_reason;
  }
  return 0;
}
