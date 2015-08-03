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

#ifndef TYPES_H
#define TYPES_H

#include "GLES/gl.h"
#include "VG/openvg.h"
#include "VG/vgu.h"
#include "interface/khronos/common/khrn_client_rpc.h"

struct vc_dispmanx_element_params {
  DISPMANX_UPDATE_HANDLE_T update;
  DISPMANX_DISPLAY_HANDLE_T display;
  int32_t layer;
  const VC_RECT_T *dest_rect;
  DISPMANX_RESOURCE_HANDLE_T src;
  const VC_RECT_T *src_rect;
  DISPMANX_PROTECTION_T protection;
  VC_DISPMANX_ALPHA_T *alpha;
  DISPMANX_CLAMP_T *clamp;
  DISPMANX_TRANSFORM_T transform;
};

struct vc_dispmanx_element_change_attributes_params {
  DISPMANX_UPDATE_HANDLE_T update;
  DISPMANX_ELEMENT_HANDLE_T element;
  uint32_t change_flags;
  int32_t layer;
  uint8_t opacity;
  const VC_RECT_T *dest_rect;
  const VC_RECT_T *src_rect;
  DISPMANX_RESOURCE_HANDLE_T mask;
  DISPMANX_TRANSFORM_T transform;
};

struct glTexImage2D_params {
  GLenum target;
  GLint level;
  GLint internalformat;
  GLsizei width;
  GLsizei height;
  GLint border;
  GLenum format;
  GLenum type;
  const GLvoid *pixels;
};

struct glTexSubImage2D_params {
  GLenum target;
  GLint level;
  GLint xoffset;
  GLint yoffset;
  GLsizei width;
  GLsizei height;
  GLenum format;
  GLenum type;
  const GLvoid *pixels;
};

struct texSubImage2DAsync_params {
  GLenum target;
  GLint level;
  GLint xoffset;
  GLint yoffset;
  GLsizei width;
  GLsizei height;
  GLenum format;
  GLenum type;
  GLint hpixels;
};

struct glCompressedTexImage2D_params {
  GLenum target;
  GLint level;
  GLenum internalformat;
  GLsizei width;
  GLsizei height;
  GLint border;
  GLsizei imageSize;
  const GLvoid *data;
};

struct glCompressedTexSubImage2D_params {
  GLenum target;
  GLint level;
  GLint xoffset;
  GLint yoffset;
  GLsizei width;
  GLsizei height;
  GLenum format;
  GLsizei imageSize;
  const GLvoid *data;
};

struct glCopyTexImage2D_params {
  GLenum target;
  GLint level;
  GLenum internalformat;
  GLint x;
  GLint y;
  GLsizei width;
  GLsizei height;
  GLint border;
};

struct glCopyTexSubImage2D_params {
  GLenum target;
  GLint level;
  GLint xoffset;
  GLint yoffset;
  GLint x;
  GLint y;
  GLsizei width;
  GLsizei height;
};

struct vgConvolve_params {
  VGImage dst;
  VGImage src;
  VGint kernelWidth;
  VGint kernelHeight;
  VGint shiftX;
  VGint shiftY;
  const VGshort * kernel;
  VGfloat scale;
  VGfloat bias;
  VGTilingMode tilingMode;
};

struct vgSeparableConvolve_params {
  VGImage dst;
  VGImage src;
  VGint kernelWidth;
  VGint kernelHeight;
  VGint shiftX;
  VGint shiftY;
  const VGshort * kernelX;
  const VGshort * kernelY;
  VGfloat scale;
  VGfloat bias;
  VGTilingMode tilingMode;
};

struct vgCopyImage_params {
  VGImage dst;
  VGint dx;
  VGint dy;
  VGImage src;
  VGint sx;
  VGint sy;
  VGint width;
  VGint height;
  VGboolean dither;
};

struct vgImageSubData_params {
  VGImage image;
  const void * data;
  VGint dataStride;
  VGImageFormat dataFormat;
  VGint x;
  VGint y;
  VGint width;
  VGint height;
};

struct vgLookupSingle_params {
  VGImage dst;
  VGImage src;
  const VGubyte * redLUT;
  const VGubyte * greenLUT;
  const VGubyte * blueLUT;
  const VGubyte * alphaLUT;
  VGboolean outputLinear;
  VGboolean outputPremultiplied;
};

struct vgPointAlongPath_params {
  VGPath path;
  VGint startSegment;
  VGint numSegments;
  VGfloat distance;
  VGfloat * x;
  VGfloat * y;
  VGfloat * tangentX;
  VGfloat * tangentY;
};

struct vguArc_params {
  VGPath path;
  VGfloat x;
  VGfloat y;
  VGfloat width;
  VGfloat height;
  VGfloat startAngle;
  VGfloat angleExtent;
  VGUArcType arcType;
};

struct vguComputeWarpQuadToQuad_params {
  VGfloat dx0;
  VGfloat dy0;
  VGfloat dx1;
  VGfloat dy1;
  VGfloat dx2;
  VGfloat dy2;
  VGfloat dx3;
  VGfloat dy3;
  VGfloat sx0;
  VGfloat sy0;
  VGfloat sx1;
  VGfloat sy1;
  VGfloat sx2;
  VGfloat sy2;
  VGfloat sx3;
  VGfloat sy3;
  VGfloat * matrix;
};

struct vguComputeWarp_params {
  VGfloat x0;
  VGfloat y0;
  VGfloat x1;
  VGfloat y1;
  VGfloat x2;
  VGfloat y2;
  VGfloat x3;
  VGfloat y3;
  VGfloat * matrix;
};

#endif
