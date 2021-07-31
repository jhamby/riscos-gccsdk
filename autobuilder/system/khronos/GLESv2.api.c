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

#include "GLES/gl.h"
#include "../types.h"
#include "../enums.h"

GL_API void GL_APIENTRY glTexImage2D (GLenum target, GLint level,
				      GLint internalformat,
				      GLsizei width, GLsizei height,
				      GLint border, GLenum format,
				      GLenum type, const GLvoid *pixels)
{
  volatile struct glTexImage2D_params params;

  params.target = target;
  params.level = level;
  params.internalformat = internalformat;
  params.width = width;
  params.height = height;
  params.border = border;
  params.format = format;
  params.type = type;
  params.pixels = pixels;

  __asm volatile ("	MOV	r0, %[params];\n"
		  "	MOV	r8, %[gl_TexImage2D];\n"
		  "	SWI	%[SWI_GL];\n"
		  :
		  : [gl_TexImage2D] "I" (gl_TexImage2D),
		    [SWI_GL] "i" (SWI_GL),
		    [params] "r" (&params)
		  : "r0", "r8");
}

GL_API void GL_APIENTRY glTexSubImage2D (GLenum target, GLint level,
					 GLint xoffset, GLint yoffset,
					 GLsizei width, GLsizei height,
					 GLenum format, GLenum type,
					 const GLvoid *pixels)
{
  volatile struct glTexSubImage2D_params params;

  params.target = target;
  params.level = level;
  params.xoffset = xoffset;
  params.yoffset = yoffset;
  params.width = width;
  params.height = height;
  params.format = format;
  params.type = type;
  params.pixels = pixels;

  __asm volatile ("	MOV	r0, %[params];\n"
		  "	MOV	r8, %[gl_TexSubImage2D];\n"
		  "	SWI	%[SWI_GL];\n"
		  :
		  : [gl_TexSubImage2D] "I" (gl_TexSubImage2D),
		    [SWI_GL] "i" (SWI_GL),
		    [params] "r" (&params)
		  : "r0", "r8");
}

GL_API void GL_APIENTRY texSubImage2DAsync (GLenum target, GLint level,
					    GLint xoffset, GLint yoffset,
					    GLsizei width, GLsizei height,
					    GLenum format, GLenum type,
					    const GLint hpixels)
{
  volatile struct texSubImage2DAsync_params params;

  params.target = target;
  params.level = level;
  params.xoffset = xoffset;
  params.yoffset = yoffset;
  params.width = width;
  params.height = height;
  params.format = format;
  params.type = type;
  params.hpixels = hpixels;

  __asm volatile ("	MOV	r0, %[params];\n"
		  "	MOV	r8, %[texSubImage2DAsync];\n"
		  "	SWI	%[SWI_GL];\n"
		  :
		  : [texSubImage2DAsync] "I" (_texSubImage2DAsync),
		    [SWI_GL] "i" (SWI_GL),
		    [params] "r" (&params)
		  : "r0", "r8");
}

GL_API void GL_APIENTRY glCompressedTexImage2D (GLenum target, GLint level,
						GLenum internalformat,
						GLsizei width, GLsizei height,
						GLint border, GLsizei imageSize,
						const GLvoid *data)
{
  volatile struct glCompressedTexImage2D_params params;

  params.target = target;
  params.level = level;
  params.internalformat = internalformat;
  params.width = width;
  params.height = height;
  params.border = border;
  params.imageSize = imageSize;
  params.data = data;

  __asm volatile ("	MOV	r0, %[params];\n"
		  "	MOV	r8, %[gl_CompressedTexImage2D];\n"
		  "	SWI	%[SWI_GL];\n"
		  :
		  : [gl_CompressedTexImage2D] "I" (gl_CompressedTexImage2D),
		    [SWI_GL] "i" (SWI_GL),
		    [params] "r" (&params)
		  : "r0", "r8");
}

GL_API void GL_APIENTRY glCompressedTexSubImage2D (GLenum target, GLint level,
						   GLint xoffset, GLint yoffset,
						   GLsizei width, GLsizei height,
						   GLenum format, GLsizei imageSize,
						   const GLvoid *data)
{
  volatile struct glCompressedTexSubImage2D_params params;

  params.target = target;
  params.level = level;
  params.xoffset = xoffset;
  params.yoffset = yoffset;
  params.width = width;
  params.height = height;
  params.format = format;
  params.imageSize = imageSize;
  params.data = data;

  __asm volatile ("	MOV	r0, %[params];\n"
		  "	MOV	r8, %[gl_CompressedTexSubImage2D];\n"
		  "	SWI	%[SWI_GL];\n"
		  :
		  : [gl_CompressedTexSubImage2D] "I" (gl_CompressedTexSubImage2D),
		    [SWI_GL] "i" (SWI_GL),
		    [params] "r" (&params)
		  : "r0", "r8");
}

GL_API void GL_APIENTRY glCopyTexImage2D (GLenum target, GLint level,
					  GLenum internalformat,
					  GLint x, GLint y,
					  GLsizei width, GLsizei height,
					  GLint border)
{
  volatile struct glCopyTexImage2D_params params;

  params.target = target;
  params.level = level;
  params.internalformat = internalformat;
  params.x = x;
  params.y = y;
  params.width = width;
  params.height = height;
  params.border = border;

  __asm volatile ("	MOV	r0, %[params];\n"
		  "	MOV	r8, %[gl_CopyTexImage2D];\n"
		  "	SWI	%[SWI_GL];\n"
		  :
		  : [gl_CopyTexImage2D] "I" (gl_CopyTexImage2D),
		    [SWI_GL] "i" (SWI_GL),
		    [params] "r" (&params)
		  : "r0", "r8");
}

GL_API void GL_APIENTRY glCopyTexSubImage2D (GLenum target, GLint level,
					     GLint xoffset, GLint yoffset,
					     GLint x, GLint y,
					     GLsizei width, GLsizei height)
{
  volatile struct glCopyTexSubImage2D_params params;

  params.target = target;
  params.level = level;
  params.xoffset = xoffset;
  params.yoffset = yoffset;
  params.x = x;
  params.y = y;
  params.width = width;
  params.height = height;

  __asm volatile ("	MOV	r0, %[params];\n"
		  "	MOV	r8, %[gl_CopyTexSubImage2D];\n"
		  "	SWI	%[SWI_GL];\n"
		  :
		  : [gl_CopyTexSubImage2D] "I" (gl_CopyTexSubImage2D),
		    [SWI_GL] "i" (SWI_GL),
		    [params] "r" (&params)
		  : "r0", "r8");
}
