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

#include "VG/openvg.h"
#include "../types.h"
#include "../enums.h"

VG_API_CALL void VG_API_ENTRY vgConvolve(VGImage dst, VGImage src,
					 VGint kernelWidth, VGint kernelHeight,
					 VGint shiftX, VGint shiftY,
					 const VGshort * kernel,
					 VGfloat scale,
					 VGfloat bias,
					 VGTilingMode tilingMode) VG_API_EXIT
{
  volatile struct vgConvolve_params params;

  params.dst = dst;
  params.src = src;
  params.kernelWidth = kernelWidth;
  params.kernelHeight = kernelHeight;
  params.shiftX = shiftX;
  params.shiftY = shiftY;
  params.kernel = kernel;
  params.scale = scale;
  params.bias = bias;
  params.tilingMode = tilingMode;

  __asm volatile ("	MOV	r0, %[params];\n"
		  "	MOV	r8, %[vg_Convolve];\n"
		  "	SWI	%[SWI_OpenVG];\n"
		  :
		  : [vg_Convolve] "I" (vg_Convolve),
		    [SWI_OpenVG] "i" (SWI_OpenVG),
		    [params] "r" (&params)
		  : "r0", "r8");
}

VG_API_CALL void VG_API_ENTRY vgCopyImage(VGImage dst, VGint dx, VGint dy,
                             VGImage src, VGint sx, VGint sy,
                             VGint width, VGint height,
                             VGboolean dither) VG_API_EXIT
{
  volatile struct vgCopyImage_params params;

  params.dst = dst;
  params.dx = dx;
  params.dy = dy;
  params.src = src;
  params.sx = sx;
  params.sy = sy;
  params.width = width;
  params.height = height;
  params.dither = dither;

  __asm volatile ("	MOV	r0, %[params];\n"
		  "	MOV	r8, %[vg_CopyImage];\n"
		  "	SWI	%[SWI_OpenVG];\n"
		  :
		  : [vg_CopyImage] "I" (vg_CopyImage),
		    [SWI_OpenVG] "i" (SWI_OpenVG),
		    [params] "r" (&params)
		  : "r0", "r8");
}

VG_API_CALL void VG_API_ENTRY vgGetImageSubData(VGImage image,
                                void * data, VGint dataStride,
                                VGImageFormat dataFormat,
                                VGint x, VGint y, VGint width, VGint height) VG_API_EXIT
{
  volatile struct vgImageSubData_params params;

  params.image = image;
  params.data = data;
  params.dataStride = dataStride;
  params.dataFormat = dataFormat;
  params.x = x;
  params.y = y;
  params.width = width;
  params.height = height;

  __asm volatile ("	MOV	r0, %[params];\n"
		  "	MOV	r8, %[vg_GetImageSubData];\n"
		  "	SWI	%[SWI_OpenVG];\n"
		  :
		  : [vg_GetImageSubData] "I" (vg_GetImageSubData),
		    [SWI_OpenVG] "i" (SWI_OpenVG),
		    [params] "r" (&params)
		  : "r0", "r8");
}

VG_API_CALL void VG_API_ENTRY vgImageSubData(VGImage image,
                                const void * data, VGint dataStride,
                                VGImageFormat dataFormat,
                                VGint x, VGint y, VGint width, VGint height) VG_API_EXIT
{
  volatile struct vgImageSubData_params params;

  params.image = image;
  params.data = data;
  params.dataStride = dataStride;
  params.dataFormat = dataFormat;
  params.x = x;
  params.y = y;
  params.width = width;
  params.height = height;

  __asm volatile ("	MOV	r0, %[params];\n"
		  "	MOV	r8, %[vg_ImageSubData];\n"
		  "	SWI	%[SWI_OpenVG];\n"
		  :
		  : [vg_ImageSubData] "I" (vg_ImageSubData),
		    [SWI_OpenVG] "i" (SWI_OpenVG),
		    [params] "r" (&params)
		  : "r0", "r8");
}

VG_API_CALL void VG_API_ENTRY vgLookup(VGImage dst, VGImage src,
                          const VGubyte * redLUT,
                          const VGubyte * greenLUT,
                          const VGubyte * blueLUT,
                          const VGubyte * alphaLUT,
                          VGboolean outputLinear,
                          VGboolean outputPremultiplied) VG_API_EXIT
{
  struct vgLookupSingle_params params;

  params.dst = dst;
  params.src = src;
  params.redLUT = redLUT;
  params.greenLUT = greenLUT;
  params.blueLUT = blueLUT;
  params.alphaLUT = alphaLUT;
  params.outputLinear = outputLinear;
  params.outputPremultiplied = outputPremultiplied;

  __asm volatile ("	MOV	r0, %[params];\n"
		  "	MOV	r8, %[vg_Lookup];\n"
		  "	SWI	%[SWI_OpenVG];\n"
		  :
		  : [vg_Lookup] "I" (vg_Lookup),
		    [SWI_OpenVG] "i" (SWI_OpenVG),
		    [params] "r" (&params)
		  : "r0", "r8");
}

VG_API_CALL void VG_API_ENTRY vgPointAlongPath(VGPath path,
                                  VGint startSegment, VGint numSegments,
                                  VGfloat distance,
                                  VGfloat * x, VGfloat * y,
                                  VGfloat * tangentX, VGfloat * tangentY) VG_API_EXIT
{
  volatile struct vgPointAlongPath_params params;

  params.path = path;
  params.startSegment = startSegment;
  params.numSegments = numSegments;
  params.distance = distance;
  params.x = x;
  params.y = y;
  params.tangentX = tangentX;
  params.tangentY = tangentY;

  __asm volatile ("	MOV	r0, %[params];\n"
		  "	MOV	r8, %[vg_PointAlongPath];\n"
		  "	SWI	%[SWI_OpenVG];\n"
		  :
		  : [vg_PointAlongPath] "I" (vg_PointAlongPath),
		    [SWI_OpenVG] "i" (SWI_OpenVG),
		    [params] "r" (&params)
		  : "r0", "r8");
}

VG_API_CALL void VG_API_ENTRY vgSeparableConvolve(VGImage dst, VGImage src,
					 VGint kernelWidth, VGint kernelHeight,
					 VGint shiftX, VGint shiftY,
					 const VGshort * kernelX,
					 const VGshort * kernelY,
					 VGfloat scale,
					 VGfloat bias,
					 VGTilingMode tilingMode) VG_API_EXIT
{
  volatile struct vgSeparableConvolve_params params;

  params.dst = dst;
  params.src = src;
  params.kernelWidth = kernelWidth;
  params.kernelHeight = kernelHeight;
  params.shiftX = shiftX;
  params.shiftY = shiftY;
  params.kernelX = kernelX;
  params.kernelY = kernelY;
  params.scale = scale;
  params.bias = bias;
  params.tilingMode = tilingMode;

  __asm volatile ("	MOV	r0, %[params];\n"
		  "	MOV	r8, %[vg_SeparableConvolve];\n"
		  "	SWI	%[SWI_OpenVG];\n"
		  :
		  : [vg_SeparableConvolve] "I" (vg_SeparableConvolve),
		    [SWI_OpenVG] "i" (SWI_OpenVG),
		    [params] "r" (&params)
		  : "r0", "r8");
}

VGU_API_CALL VGUErrorCode VGU_API_ENTRY vguArc(VGPath path,
                                 VGfloat x, VGfloat y,
                                 VGfloat width, VGfloat height,
                                 VGfloat startAngle, VGfloat angleExtent,
                                 VGUArcType arcType) VGU_API_EXIT
{
  volatile struct vguArc_params params;
  register VGUErrorCode result __asm("r0");

  params.path = path;
  params.x = x;
  params.y = y;
  params.width = width;
  params.height = height;
  params.startAngle = startAngle;
  params.angleExtent = angleExtent;
  params.arcType = arcType;

  __asm volatile ("	MOV	r0, %[params];\n"
		  "	MOV	r8, %[vgu_Arc];\n"
		  "	SWI	%[SWI_OpenVG];\n"
		  :
		  : [vgu_Arc] "I" (vgu_Arc),
		    [SWI_OpenVG] "i" (SWI_OpenVG),
		    [params] "r" (&params)
		  : "r0", "r8");
  return result;
}

VGU_API_CALL VGUErrorCode VGU_API_ENTRY vguComputeWarpQuadToQuad(VGfloat dx0, VGfloat dy0,
                                                   VGfloat dx1, VGfloat dy1,
                                                   VGfloat dx2, VGfloat dy2,
                                                   VGfloat dx3, VGfloat dy3,
                                                   VGfloat sx0, VGfloat sy0,
                                                   VGfloat sx1, VGfloat sy1,
                                                   VGfloat sx2, VGfloat sy2,
                                                   VGfloat sx3, VGfloat sy3,
                                                   VGfloat * matrix) VGU_API_EXIT
{
  volatile struct vguComputeWarpQuadToQuad_params params;
  register VGUErrorCode result __asm("r0");

  params.dx0 = dx0;
  params.dy0 = dy0;
  params.dx1 = dx1;
  params.dy1 = dy1;
  params.dx2 = dx2;
  params.dy2 = dy2;
  params.dx3 = dx3;
  params.dy3 = dy3;
  params.sx0 = sx0;
  params.sy0 = sy0;
  params.sx1 = sx1;
  params.sy1 = sy1;
  params.sx2 = sx2;
  params.sy2 = sy2;
  params.sx3 = sx3;
  params.sy3 = sy3;
  params.matrix = matrix;

  __asm volatile ("	MOV	r0, %[params];\n"
		  "	MOV	r8, %[vgu_ComputeWarpQuadToQuad];\n"
		  "	SWI	%[SWI_OpenVG];\n"
		  :
		  : [vgu_ComputeWarpQuadToQuad] "I" (vgu_ComputeWarpQuadToQuad),
		    [SWI_OpenVG] "i" (SWI_OpenVG),
		    [params] "r" (&params)
		  : "r0", "r8");
  return result;
}

VGU_API_CALL VGUErrorCode VGU_API_ENTRY vguComputeWarpQuadToSquare(VGfloat sx0, VGfloat sy0,
                                                     VGfloat sx1, VGfloat sy1,
                                                     VGfloat sx2, VGfloat sy2,
                                                     VGfloat sx3, VGfloat sy3,
                                                     VGfloat * matrix) VGU_API_EXIT
{
  volatile struct vguComputeWarp_params params;
  register VGUErrorCode result __asm("r0");

  params.x0 = sx0;
  params.y0 = sy0;
  params.x1 = sx1;
  params.y1 = sy1;
  params.x2 = sx2;
  params.y2 = sy2;
  params.x3 = sx3;
  params.y3 = sy3;
  params.matrix = matrix;

  __asm volatile ("	MOV	r0, %[params];\n"
		  "	MOV	r8, %[vgu_ComputeWarpQuadToSquare];\n"
		  "	SWI	%[SWI_OpenVG];\n"
		  :
		  : [vgu_ComputeWarpQuadToSquare] "I" (vgu_ComputeWarpQuadToSquare),
		    [SWI_OpenVG] "i" (SWI_OpenVG),
		    [params] "r" (&params)
		  : "r0", "r8");
  return result;
}

VGU_API_CALL VGUErrorCode VGU_API_ENTRY vguComputeWarpSquareToQuad(VGfloat dx0, VGfloat dy0,
                                                     VGfloat dx1, VGfloat dy1,
                                                     VGfloat dx2, VGfloat dy2,
                                                     VGfloat dx3, VGfloat dy3,
                                                     VGfloat * matrix) VGU_API_EXIT
{
  volatile struct vguComputeWarp_params params;
  register VGUErrorCode result __asm("r0");

  params.x0 = dx0;
  params.y0 = dy0;
  params.x1 = dx1;
  params.y1 = dy1;
  params.x2 = dx2;
  params.y2 = dy2;
  params.x3 = dx3;
  params.y3 = dy3;
  params.matrix = matrix;

  __asm volatile ("	MOV	r0, %[params];\n"
		  "	MOV	r8, %[vgu_ComputeWarpSquareToQuad];\n"
		  "	SWI	%[SWI_OpenVG];\n"
		  :
		  : [vgu_ComputeWarpSquareToQuad] "I" (vgu_ComputeWarpSquareToQuad),
		    [SWI_OpenVG] "i" (SWI_OpenVG),
		    [params] "r" (&params)
		  : "r0", "r8");
  return result;
}
