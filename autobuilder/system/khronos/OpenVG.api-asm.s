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

DEFINE_FUNC	vgAppendPath			SWI_OpenVG vg_AppendPath
DEFINE_FUNC	vgAppendPathData		SWI_OpenVG vg_AppendPathData
DEFINE_FUNC_5	vgChildImage			SWI_OpenVG vg_ChildImage
DEFINE_FUNC	vgClear				SWI_OpenVG vg_Clear
DEFINE_FUNC	vgClearGlyph			SWI_OpenVG vg_ClearGlyph
DEFINE_FUNC_5	vgClearImage			SWI_OpenVG vg_ClearImage
DEFINE_FUNC	vgClearPath			SWI_OpenVG vg_ClearPath
DEFINE_FUNC	vgColorMatrix			SWI_OpenVG vg_ColorMatrix
DEFINE_FUNC_7	vgCopyMask			SWI_OpenVG vg_CopyMask
DEFINE_FUNC_6	vgCopyPixels			SWI_OpenVG vg_CopyPixels
DEFINE_FUNC	vgCreateEGLImageTargetKHR	SWI_OpenVG vg_CreateEGLImageTargetKHR
DEFINE_FUNC	vgCreateFont			SWI_OpenVG vg_CreateFont
DEFINE_FUNC	vgCreateImage			SWI_OpenVG vg_CreateImage
DEFINE_FUNC	vgCreateMaskLayer		SWI_OpenVG vg_CreateMaskLayer
DEFINE_FUNC	vgCreatePaint			SWI_OpenVG vg_CreatePaint
DEFINE_FUNC_7	vgCreatePath			SWI_OpenVG vg_CreatePath
DEFINE_FUNC	vgDestroyFont			SWI_OpenVG vg_DestroyFont
DEFINE_FUNC	vgDestroyImage			SWI_OpenVG vg_DestroyImage
DEFINE_FUNC	vgDestroyMaskLayer		SWI_OpenVG vg_DestroyMaskLayer
DEFINE_FUNC	vgDestroyPaint			SWI_OpenVG vg_DestroyPaint
DEFINE_FUNC	vgDestroyPath			SWI_OpenVG vg_DestroyPath
DEFINE_FUNC	vgDrawGlyph			SWI_OpenVG vg_DrawGlyph
DEFINE_FUNC_7	vgDrawGlyphs			SWI_OpenVG vg_DrawGlyphs
DEFINE_FUNC	vgDrawImage			SWI_OpenVG vg_DrawImage
DEFINE_FUNC	vgDrawPath			SWI_OpenVG vg_DrawPath
DEFINE_FUNC_6	vgFillMaskLayer			SWI_OpenVG vg_FillMaskLayer
DEFINE_FUNC	vgFinish			SWI_OpenVG vg_Finish
DEFINE_FUNC	vgFlush				SWI_OpenVG vg_Flush
DEFINE_FUNC_5	vgGaussianBlur			SWI_OpenVG vg_GaussianBlur
DEFINE_FUNC	vgGetColor			SWI_OpenVG vg_GetColor
DEFINE_FUNC	vgGetError			SWI_OpenVG vg_GetError
DEFINE_FUNC	vgGetf				SWI_OpenVG vg_Getf
DEFINE_FUNC	vgGetfv				SWI_OpenVG vg_Getfv
DEFINE_FUNC	vgGeti				SWI_OpenVG vg_Geti
DEFINE_FUNC	vgGetiv				SWI_OpenVG vg_Getiv
DEFINE_FUNC	vgGetMatrix			SWI_OpenVG vg_GetMatrix
DEFINE_FUNC	vgGetPaint			SWI_OpenVG vg_GetPaint
DEFINE_FUNC	vgGetParameterf			SWI_OpenVG vg_GetParameterf
DEFINE_FUNC	vgGetParameterfv		SWI_OpenVG vg_GetParameterfv
DEFINE_FUNC	vgGetParameteri			SWI_OpenVG vg_GetParameteri
DEFINE_FUNC	vgGetParameteriv		SWI_OpenVG vg_GetParameteriv
DEFINE_FUNC	vgGetParameterVectorSize	SWI_OpenVG vg_GetParameterVectorSize
DEFINE_FUNC	vgGetParent			SWI_OpenVG vg_GetParent
DEFINE_FUNC	vgGetPathCapabilities		SWI_OpenVG vg_GetPathCapabilities
DEFINE_FUNC_7	vgGetPixels			SWI_OpenVG vg_GetPixels
DEFINE_FUNC	vgGetString			SWI_OpenVG vg_GetString
DEFINE_FUNC	vgGetVectorSize			SWI_OpenVG vg_GetVectorSize
DEFINE_FUNC	vgHardwareQuery			SWI_OpenVG vg_HardwareQuery
DEFINE_FUNC	vgInterpolatePath		SWI_OpenVG vg_InterpolatePath
DEFINE_FUNC	vgLoadIdentity			SWI_OpenVG vg_LoadIdentity
DEFINE_FUNC	vgLoadMatrix			SWI_OpenVG vg_LoadMatrix
DEFINE_FUNC_6	vgLookupSingle			SWI_OpenVG vg_LookupSingle
DEFINE_FUNC_6	vgMask				SWI_OpenVG vg_Mask
DEFINE_FUNC	vgModifyPathCoords		SWI_OpenVG vg_ModifyPathCoords
DEFINE_FUNC	vgMultMatrix			SWI_OpenVG vg_MultMatrix
DEFINE_FUNC	vgPaintPattern			SWI_OpenVG vg_PaintPattern
DEFINE_FUNC_5	vgPathBounds			SWI_OpenVG vg_PathBounds
DEFINE_FUNC	vgPathLength			SWI_OpenVG vg_PathLength
DEFINE_FUNC_5	vgPathTransformedBounds		SWI_OpenVG vg_PathTransformedBounds
DEFINE_FUNC_7	vgReadPixels			SWI_OpenVG vg_ReadPixels
DEFINE_FUNC	vgRemovePathCapabilities	SWI_OpenVG vg_RemovePathCapabilities
DEFINE_FUNC	vgRenderToMask			SWI_OpenVG vg_RenderToMask
DEFINE_FUNC	vgRotate			SWI_OpenVG vg_Rotate
DEFINE_FUNC	vgScale				SWI_OpenVG vg_Scale
DEFINE_FUNC	vgSetColor			SWI_OpenVG vg_SetColor
DEFINE_FUNC	vgSetf				SWI_OpenVG vg_Setf
DEFINE_FUNC	vgSetfv				SWI_OpenVG vg_Setfv
DEFINE_FUNC_5	vgSetGlyphToImage		SWI_OpenVG vg_SetGlyphToImage
DEFINE_FUNC_6	vgSetGlyphToPath		SWI_OpenVG vg_SetGlyphToPath
DEFINE_FUNC	vgSeti				SWI_OpenVG vg_Seti
DEFINE_FUNC	vgSetiv				SWI_OpenVG vg_Setiv
DEFINE_FUNC	vgSetPaint			SWI_OpenVG vg_SetPaint
DEFINE_FUNC	vgSetParameterf			SWI_OpenVG vg_SetParameterf
DEFINE_FUNC	vgSetParameterfv		SWI_OpenVG vg_SetParameterfv
DEFINE_FUNC	vgSetParameteri			SWI_OpenVG vg_SetParameteri
DEFINE_FUNC	vgSetParameteriv		SWI_OpenVG vg_SetParameteriv
DEFINE_FUNC_7	vgSetPixels			SWI_OpenVG vg_SetPixels
DEFINE_FUNC	vgShear				SWI_OpenVG vg_Shear
DEFINE_FUNC	vgTransformPath			SWI_OpenVG vg_TransformPath
DEFINE_FUNC	vgTranslate			SWI_OpenVG vg_Translate
DEFINE_FUNC_7	vgWritePixels			SWI_OpenVG vg_WritePixels
DEFINE_FUNC_5	vguEllipse			SWI_OpenVG vgu_Ellipse
DEFINE_FUNC_5	vguLine				SWI_OpenVG vgu_Line
DEFINE_FUNC	vguPolygon			SWI_OpenVG vgu_Polygon
DEFINE_FUNC_5	vguRect				SWI_OpenVG vgu_Rect
DEFINE_FUNC_7	vguRoundRect			SWI_OpenVG vgu_RoundRect
