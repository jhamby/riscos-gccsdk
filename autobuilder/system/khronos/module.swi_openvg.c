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
#include "VG/vgu.h"
#include "VG/vgext.h"
#include "khronos.h"
#include "main.h"
#include "../types.h"
#include "../enums.h"
#include <kernel.h>

VGErrorCode last_vg_error __attribute__((visibility("hidden"))) = VG_NO_ERROR;

_kernel_oserror *openvg_swi(_kernel_swi_regs *r)
{
  switch ((openvg_reason_code)r->r[8])
  {
    case vg_AppendPath:
      vgAppendPath((VGPath)r->r[0], (VGPath)r->r[1]);
      break;
    case vg_AppendPathData:
      vgAppendPathData((VGPath)r->r[0], (VGint)r->r[1],
		       (const VGubyte *)r->r[2], (const void *)r->r[3]);
      break;
    case vg_ChildImage:
      r->r[0] = (int)vgChildImage((VGImage)r->r[0], (VGint)r->r[1], 
				  (VGint)r->r[2], (VGint)r->r[3], (VGint)r->r[4]);
      break;
    case vg_Clear:
      vgClear((VGint)r->r[0], (VGint)r->r[1], (VGint)r->r[2], (VGint)r->r[3]);
      break;
    case vg_ClearGlyph:
      vgClearGlyph((VGFont)r->r[0], (VGuint)r->r[1]);
      break;
    case vg_ClearImage:
      vgClearImage((VGImage)r->r[0], (VGint)r->r[1], (VGint)r->r[2],
		   (VGint)r->r[3], (VGint)r->r[4]);
      break;
    case vg_ClearPath:
      vgClearPath((VGPath)r->r[0], (VGbitfield)r->r[1]);
      break;
    case vg_ColorMatrix:
      vgColorMatrix((VGImage)r->r[0], (VGImage)r->r[1],
		    (const VGfloat *)r->r[2]);
      break;
    case vg_Convolve: {
      struct vgConvolve_params *params = (struct vgConvolve_params *)r->r[0];

      vgConvolve(params->dst, params->src,
		 params->kernelWidth, params->kernelHeight,
		 params->shiftX, params->shiftY,
		 params->kernel, params->scale,
		 params->bias, params->tilingMode);
      break;
    }
    case vg_CopyImage: {
      struct vgCopyImage_params *params = (struct vgCopyImage_params *)r->r[0];

      vgCopyImage(params->dst, params->dx, params->dy,
                  params->src, params->sx, params->sy,
                  params->width, params->height,
                  params->dither);
      break;
    }
    case vg_CopyMask:
      vgCopyMask((VGMaskLayer)r->r[0],
                 (VGint)r->r[1], (VGint)r->r[2],
                 (VGint)r->r[3], (VGint)r->r[4],
                 (VGint)r->r[5], (VGint)r->r[6]);
      break;
    case vg_CopyPixels:
      vgCopyPixels((VGint)r->r[0], (VGint)r->r[1],
                   (VGint)r->r[2], (VGint)r->r[3],
                   (VGint)r->r[4], (VGint)r->r[5]);
      break;
    case vg_CreateEGLImageTargetKHR:
      vgCreateEGLImageTargetKHR((VGeglImageKHR)r->r[0]);
      break;
    case vg_CreateFont:
      r->r[0] = (int)vgCreateFont((VGint)r->r[0]);
      break;
    case vg_CreateImage:
      r->r[0] = (int)vgCreateImage((VGImageFormat)r->r[0],
				   (VGint)r->r[1], (VGint)r->r[2],
				   (VGbitfield)r->r[3]);
      break;
    case vg_CreateMaskLayer:
      r->r[0] = (int)vgCreateMaskLayer((VGint)r->r[0], (VGint)r->r[1]);
      break;
    case vg_CreatePaint:
      r->r[0] = (int)vgCreatePaint();
      break;
    case vg_CreatePath:
      r->r[0] = (int)vgCreatePath((VGint)r->r[0], (VGPathDatatype)r->r[1],
				  RPC_FLOAT_RES(r->r[2]), RPC_FLOAT_RES(r->r[3]),
				  (VGint)r->r[4], (VGint)r->r[5], (VGbitfield)r->r[6]);
      break;
    case vg_DestroyFont:
      vgDestroyFont((VGFont)r->r[0]);
      break;
    case vg_DestroyImage:
      vgDestroyImage((VGImage)r->r[0]);
      break;
    case vg_DestroyMaskLayer:
      vgDestroyMaskLayer((VGMaskLayer)r->r[0]);
      break;
    case vg_DestroyPaint:
      vgDestroyPaint((VGPaint)r->r[0]);
      break;
    case vg_DestroyPath:
      vgDestroyPath((VGPath)r->r[0]);
      break;
    case vg_DrawGlyph:
      vgDrawGlyph((VGFont)r->r[0], (VGuint)r->r[1],
		  (VGbitfield)r->r[2], (VGboolean)r->r[3]);
      break;
    case vg_DrawGlyphs:
      vgDrawGlyphs((VGFont)r->r[0], (VGint)r->r[1], (const VGuint *)r->r[2],
		   (const VGfloat *)r->r[3], (const VGfloat *)r->r[4],
		   (VGbitfield)r->r[5], (VGboolean)r->r[6]);
      break;
    case vg_DrawImage:
      vgDrawImage((VGImage)r->r[0]);
      break;
    case vg_DrawPath:
      vgDrawPath((VGPath)r->r[0], (VGbitfield)r->r[1]);
      break;
    case vg_FillMaskLayer:
      vgFillMaskLayer((VGMaskLayer)r->r[0],
		      (VGint)r->r[1], (VGint)r->r[2],
		      (VGint)r->r[3], (VGint)r->r[4],
		      RPC_FLOAT_RES(r->r[5]));
      break;
    case vg_Finish:
      vgFinish();
      break;
    case vg_Flush:
      vgFlush();
      break;
    case vg_GaussianBlur:
      vgGaussianBlur((VGImage)r->r[0], (VGImage)r->r[1],
                                RPC_FLOAT_RES(r->r[2]),
                                RPC_FLOAT_RES(r->r[3]),
                                (VGTilingMode)r->r[4]);
      break;
    case vg_GetColor:
      r->r[0] = (int)vgGetColor((VGPaint)r->r[0]);
      break;
    case vg_GetError: {
      VGErrorCode error = vgGetError();

      if (error != VG_NO_ERROR)
	last_vg_error = error;

      r->r[0] = (int)error;
      break;
    }
    case vg_Getf:
      r->r[0] = (int)RPC_FLOAT(vgGetf((VGParamType)r->r[0]));
      break;
    case vg_Getfv:
      vgGetfv((VGParamType)r->r[0], (VGint)r->r[1], (VGfloat *)r->r[2]);
      break;
    case vg_Geti:
      r->r[0] = (int)vgGeti((VGParamType)r->r[0]);
      break;
    case vg_GetImageSubData: {
      struct vgImageSubData_params *params = (struct vgImageSubData_params *)r->r[0];

      vgGetImageSubData(params->image,
			params->data, params->dataStride,
			params->dataFormat,
			params->x, params->y,
			params->width, params->height);
      break;
    }
    case vg_Getiv:
      vgGetiv((VGParamType)r->r[0], (VGint)r->r[1], (VGint *)r->r[2]);
      break;
    case vg_GetMatrix:
      vgGetMatrix((VGfloat *)r->r[0]);
      break;
    case vg_GetPaint:
      r->r[0] = (int)vgGetPaint((VGPaintMode)r->r[0]);
      break;
    case vg_GetParameterf:
      r->r[0] = (int)RPC_FLOAT(vgGetParameterf((VGHandle)r->r[0], (VGint)r->r[1]));
      break;
    case vg_GetParameterfv:
      vgGetParameterfv((VGHandle)r->r[0], (VGint)r->r[1],
		       (VGint)r->r[2], (VGfloat *)r->r[3]);
      break;
    case vg_GetParameteri:
      r->r[0] = (int)vgGetParameteri((VGHandle)r->r[0], (VGint)r->r[1]);
      break;
    case vg_GetParameteriv:
      vgGetParameteriv((VGHandle)r->r[0], (VGint)r->r[1],
		       (VGint)r->r[2], (VGint *)r->r[3]);
      break;
    case vg_GetParameterVectorSize:
      r->r[0] = (int)vgGetParameterVectorSize((VGHandle)r->r[0], (VGint)r->r[1]);
      break;
    case vg_GetParent:
      r->r[0] = (int)vgGetParent((VGImage)r->r[0]);
      break;
    case vg_GetPathCapabilities:
      r->r[0] = (int)vgGetPathCapabilities((VGPath)r->r[0]);
      break;
    case vg_GetPixels:
      vgGetPixels((VGImage)r->r[0], (VGint)r->r[1], (VGint)r->r[2],
		  (VGint)r->r[3], (VGint)r->r[4],
		  (VGint)r->r[5], (VGint)r->r[6]);
      break;
    case vg_GetString:
      r->r[0] = (int)vgGetString((VGStringID)r->r[0]);
      break;
    case vg_GetVectorSize:
      r->r[0] = (int)vgGetVectorSize((VGParamType)r->r[0]);
      break;
    case vg_HardwareQuery:
      r->r[0] = (int)vgHardwareQuery((VGHardwareQueryType)r->r[0], (VGint)r->r[1]);
      break;
    case vg_ImageSubData: {
      struct vgImageSubData_params *params = (struct vgImageSubData_params *)r->r[0];

      vgImageSubData(params->image,
		     params->data, params->dataStride,
		     params->dataFormat,
		     params->x, params->y,
		     params->width, params->height);
      break;
    }
    case vg_InterpolatePath:
      r->r[0] = (int)vgInterpolatePath((VGPath)r->r[0], (VGPath)r->r[1],
				       (VGPath)r->r[2], RPC_FLOAT_RES(r->r[3]));
      break;
    case vg_LoadIdentity:
      vgLoadIdentity();
      break;
    case vg_LoadMatrix:
      vgLoadMatrix((const VGfloat *)r->r[0]);
      break;
    case vg_Lookup: {
      struct vgLookupSingle_params *params = (struct vgLookupSingle_params *)r->r[0];

      vgLookup(params->dst, params->src,
	       params->redLUT, params->greenLUT, params->blueLUT, params->alphaLUT,
	       params->outputLinear, params->outputPremultiplied);
      break;
    }
    case vg_LookupSingle:
      vgLookupSingle((VGImage)r->r[0], (VGImage)r->r[1],
		     (const VGuint *)r->r[2], (VGImageChannel)r->r[3],
		     (VGboolean)r->r[4],  (VGboolean)r->r[5]);
      break;
    case vg_Mask:
      vgMask((VGHandle)r->r[0], (VGMaskOperation)r->r[1],
	     (VGint)r->r[2], (VGint)r->r[3],
	     (VGint)r->r[4], (VGint)r->r[5]);
      break;
    case vg_ModifyPathCoords:
      vgModifyPathCoords((VGPath)r->r[0], (VGint)r->r[1],
			 (VGint)r->r[2], (const void *)r->r[3]);
      break;
    case vg_MultMatrix:
      vgMultMatrix((const VGfloat *)r->r[0]);
      break;
    case vg_PaintPattern:
      vgPaintPattern((VGPaint)r->r[0], (VGImage)r->r[1]);
      break;
    case vg_PathBounds:
      vgPathBounds((VGPath)r->r[0],
		   (VGfloat *)r->r[1], (VGfloat *)r->r[2],
		   (VGfloat *)r->r[3], (VGfloat *)r->r[4]);
      break;
    case vg_PathLength:
      r->r[0] = (int)RPC_FLOAT(vgPathLength((VGPath)r->r[0],
					    (VGint)r->r[1],
					    (VGint)r->r[2]));
      break;
    case vg_PathTransformedBounds:
      vgPathTransformedBounds((VGPath)r->r[0],
			      (VGfloat *)r->r[1], (VGfloat *)r->r[2],
			      (VGfloat *)r->r[3], (VGfloat *)r->r[4]);
      break;
    case vg_PointAlongPath: {
      struct vgPointAlongPath_params *params = (struct vgPointAlongPath_params *)r->r[0];

      vgPointAlongPath(params->path,
		       params->startSegment, params->numSegments,
		       params->distance,
		       params->x, params->y,
		       params->tangentX, params->tangentY);
      break;
    }
    case vg_ReadPixels:
      vgReadPixels((void *)r->r[0], (VGint)r->r[1], (VGImageFormat)r->r[2],
		   (VGint)r->r[3], (VGint)r->r[4],
		   (VGint)r->r[5], (VGint)r->r[6]);
      break;
    case vg_RemovePathCapabilities:
      vgRemovePathCapabilities((VGPath)r->r[0], (VGbitfield)r->r[1]);
      break;
    case vg_RenderToMask:
      vgRenderToMask((VGPath)r->r[0], (VGbitfield)r->r[1], (VGMaskOperation)r->r[2]);
      break;
    case vg_Rotate:
      vgRotate(RPC_FLOAT_RES(r->r[0]));
      break;
    case vg_Scale:
      vgScale(RPC_FLOAT_RES(r->r[0]), RPC_FLOAT_RES(r->r[1]));
      break;
    case vg_SeparableConvolve: {
      struct vgSeparableConvolve_params *params =
			(struct vgSeparableConvolve_params *)r->r[0];

      vgSeparableConvolve(params->dst, params->src,
			  params->kernelWidth, params->kernelHeight,
			  params->shiftX, params->shiftY,
			  params->kernelX, params->kernelY,
			  params->scale, params->bias,
			  params->tilingMode);
      break;
    }
    case vg_SetColor:
      vgSetColor((VGPaint)r->r[0], (VGuint)r->r[1]);
      break;
    case vg_Setf:
      vgSetf ((VGParamType)r->r[0], RPC_FLOAT_RES(r->r[1]));
      break;
    case vg_Setfv:
      vgSetfv((VGParamType)r->r[0], (VGint)r->r[1], (const VGfloat *)r->r[2]);
      break;
    case vg_SetGlyphToImage:
      /* TODO: Changed array parameters to pointers. Check OK. */
      vgSetGlyphToImage((VGFont)r->r[0], (VGuint)r->r[1],
			(VGImage)r->r[2], (VGfloat *)r->r[3],
			(VGfloat *)r->r[4]);
      break;
    case vg_SetGlyphToPath:
      /* TODO: Changed array parameters to pointers. Check OK. */
      vgSetGlyphToPath((VGFont)r->r[0], (VGuint)r->r[1], (VGPath)r->r[2],
		       (VGboolean)r->r[3], (VGfloat *)r->r[4], (VGfloat *)r->r[5]);
      break;
    case vg_Seti:
      vgSeti ((VGParamType)r->r[0], (VGint)r->r[1]);
      break;
    case vg_Setiv:
      vgSetiv((VGParamType)r->r[0], (VGint)r->r[1], (const VGint *)r->r[2]);
      break;
    case vg_SetPaint:
      vgSetPaint((VGPaint)r->r[0], (VGbitfield)r->r[1]);
      break;
    case vg_SetParameterf:
      vgSetParameterf((VGHandle)r->r[0], (VGint)r->r[1], RPC_FLOAT_RES(r->r[2]));
      break;
    case vg_SetParameterfv:
      vgSetParameterfv((VGHandle)r->r[0], (VGint)r->r[1],
		       (VGint)r->r[2], (const VGfloat *)r->r[3]);
      break;
    case vg_SetParameteri:
      vgSetParameteri((VGHandle)r->r[0], (VGint)r->r[1], (VGint)r->r[2]);
      break;
    case vg_SetParameteriv:
      vgSetParameteriv((VGHandle)r->r[0], (VGint)r->r[1],
		       (VGint)r->r[2], (const VGint *)r->r[3]);
      break;
    case vg_SetPixels:
      vgSetPixels((VGint)r->r[0], (VGint)r->r[1],
                             (VGImage)r->r[2], (VGint)r->r[3], (VGint)r->r[4],
                             (VGint)r->r[5], (VGint)r->r[6]);
      break;
    case vg_Shear:
      vgShear(RPC_FLOAT_RES(r->r[0]), RPC_FLOAT_RES(r->r[1]));
      break;
    case vg_TransformPath:
      vgTransformPath((VGPath)r->r[0], (VGPath)r->r[1]);
      break;
    case vg_Translate:
      vgTranslate(RPC_FLOAT_RES(r->r[0]), RPC_FLOAT_RES(r->r[1]));
      break;
    case vg_WritePixels:
      vgWritePixels((const void *)r->r[0], (VGint)r->r[1],
		    (VGImageFormat)r->r[2],
		    (VGint)r->r[3], (VGint)r->r[4],
		    (VGint)r->r[5], (VGint)r->r[6]);
      break;
    case vgu_Arc: {
      struct vguArc_params *params = (struct vguArc_params *)r->r[0];

       r->r[0] = (int)vguArc(params->path,
			     params->x, params->y,
			     params->width, params->height,
			     params->startAngle, params->angleExtent,
                             params->arcType);
      break;
    }
    case vgu_ComputeWarpQuadToQuad: {
      struct vguComputeWarpQuadToQuad_params *params =
			  (struct vguComputeWarpQuadToQuad_params *)r->r[0];

      r->r[0] = (int)vguComputeWarpQuadToQuad(params->dx0, params->dy0,
					      params->dx1, params->dy1,
					      params->dx2, params->dy2,
					      params->dx3, params->dy3,
					      params->sx0, params->sy0,
					      params->sx1, params->sy1,
					      params->sx2, params->sy2,
					      params->sx3, params->sy3,
					      params->matrix);
      break;
    }
    case vgu_ComputeWarpQuadToSquare: {
      struct vguComputeWarp_params *params = (struct vguComputeWarp_params *)r->r[0];

      r->r[0] = (int)vguComputeWarpQuadToSquare(params->x0, params->y0,
						params->x1, params->y1,
						params->x2, params->y2,
						params->x3, params->y3,
						params->matrix);
      break;
    }
    case vgu_ComputeWarpSquareToQuad: {
      struct vguComputeWarp_params *params = (struct vguComputeWarp_params *)r->r[0];

      r->r[0] = (int)vguComputeWarpSquareToQuad(params->x0, params->y0,
						params->x1, params->y1,
						params->x2, params->y2,
						params->x3, params->y3,
						params->matrix);
      break;
    }
    case vgu_Ellipse:
      r->r[0] = (int)vguEllipse((VGPath)r->r[0],
				RPC_FLOAT_RES(r->r[1]), RPC_FLOAT_RES(r->r[2]),
				RPC_FLOAT_RES(r->r[3]), RPC_FLOAT_RES(r->r[4]));
      break;
    case vgu_Line:
      r->r[0] = (int)vguLine((VGPath)r->r[0],
			     RPC_FLOAT_RES(r->r[1]), RPC_FLOAT_RES(r->r[2]),
			     RPC_FLOAT_RES(r->r[3]), RPC_FLOAT_RES(r->r[4]));
      break;
    case vgu_Polygon:
      r->r[0] = (int)vguPolygon((VGPath)r->r[0],
				(const VGfloat *)r->r[1], (VGint)r->r[2],
				(VGboolean)r->r[3]);
      break;
    case vgu_Rect:
      r->r[0] = (int)vguRect((VGPath)r->r[0],
			     RPC_FLOAT_RES(r->r[1]), RPC_FLOAT_RES(r->r[2]),
			     RPC_FLOAT_RES(r->r[3]), RPC_FLOAT_RES(r->r[4]));
      break;
    case vgu_RoundRect:
      r->r[0] = (int)vguRoundRect((VGPath)r->r[0],
				  RPC_FLOAT_RES(r->r[1]), RPC_FLOAT_RES(r->r[2]),
				  RPC_FLOAT_RES(r->r[3]), RPC_FLOAT_RES(r->r[4]),
				  RPC_FLOAT_RES(r->r[5]), RPC_FLOAT_RES(r->r[6]));
      break;
  }
  return NULL;
}
