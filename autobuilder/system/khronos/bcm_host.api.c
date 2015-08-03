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

#include "EGL/egl.h"
#include "../types.h"
#include "../enums.h"
#include "interface/vmcs_host/vc_dispmanx.h"
#include <stdio.h>
#include <assert.h>
#include <swis.h>

#define KHRONOS_VERSION "1.00"

void bcm_host_init(void)
{
   if (_swix(OS_CLI, _IN(0), "RMEnsure Khronos " KHRONOS_VERSION \
			    " RMLoad System:Modules.Khronos") == NULL)
     return;
   if (_swix(OS_CLI, _IN(0), "RMEnsure Khronos " KHRONOS_VERSION \
			    " RMLoad Khronos:Khronos") == NULL)
     return;
   if (_swix(OS_CLI, _IN(0), "RMEnsure Khronos " KHRONOS_VERSION \
			    " Error Require Khronos module, version " KHRONOS_VERSION) != NULL) {
     fputs("Require Khronos module, version " KHRONOS_VERSION "\n", stderr);
     exit(1);
   }
}

int32_t graphics_get_display_size(const uint16_t display_number,
                                  uint32_t *width,
                                  uint32_t *height)
{
   DISPMANX_DISPLAY_HANDLE_T display_handle = 0;
   DISPMANX_MODEINFO_T mode_info;
   int32_t success = -1;

   if (display_handle == 0) {
      // Display must be opened first.
      display_handle = vc_dispmanx_display_open(display_number);
      assert(display_handle);
   }
   if (display_handle) {
      success = vc_dispmanx_display_get_info(display_handle, &mode_info);
         
      if( success >= 0 )
      {
         if( NULL != width )
         {
            *width = mode_info.width;
         }
         
         if( NULL != height )
         {
            *height = mode_info.height;
         }
      }
   }
      
   if ( display_handle )
   {
      vc_dispmanx_display_close(display_handle);
      display_handle = 0;
   }

   return success;
}

DISPMANX_ELEMENT_HANDLE_T vc_dispmanx_element_add (DISPMANX_UPDATE_HANDLE_T update,
                                                   DISPMANX_DISPLAY_HANDLE_T display,
                                                   int32_t layer,
                                                   const VC_RECT_T *dest_rect,
                                                   DISPMANX_RESOURCE_HANDLE_T src,
                                                   const VC_RECT_T *src_rect,
                                                   DISPMANX_PROTECTION_T protection,
                                                   VC_DISPMANX_ALPHA_T *alpha,
                                                   DISPMANX_CLAMP_T *clamp,
                                                   DISPMANX_TRANSFORM_T transform)
{
  register DISPMANX_ELEMENT_HANDLE_T result __asm ("r0");
  /* Make volatile otherwise GCC doesn't bother to fill in params.  */
  volatile struct vc_dispmanx_element_params params;

  params.update = update;
  params.display = display;
  params.layer = layer;
  params.dest_rect = dest_rect;
  params.src = src;
  params.src_rect = src_rect;
  params.protection = protection;
  params.alpha = alpha;
  params.clamp = clamp;
  params.transform = transform;

  __asm volatile ("	MOV	r8, %[vc_DispManxElementAdd];\n"
		  "	MOV	r0, %[params];\n"
		  "	SWI	%[SWI_VC];\n"
		  : "=r" (result)
		  : [vc_DispManxElementAdd] "I" (vc_DispManxElementAdd),
		    [SWI_VC] "i" (SWI_VC),
		    [params] "r" (&params)
		  : "r8");
  return result;
}

int vc_dispmanx_rect_set( VC_RECT_T *rect, uint32_t x_offset, uint32_t y_offset, uint32_t width, uint32_t height ) {
   rect->x = (int32_t) x_offset;
   rect->y = (int32_t) y_offset;
   rect->width = (int32_t) width;
   rect->height = (int32_t) height;
   return 0;
}

VCHPRE_ int VCHPOST_ vc_dispmanx_element_change_attributes( DISPMANX_UPDATE_HANDLE_T update, 
                                                            DISPMANX_ELEMENT_HANDLE_T element,
                                                            uint32_t change_flags,
                                                            int32_t layer,
                                                            uint8_t opacity,
                                                            const VC_RECT_T *dest_rect,
                                                            const VC_RECT_T *src_rect,
                                                            DISPMANX_RESOURCE_HANDLE_T mask,
                                                            DISPMANX_TRANSFORM_T transform )
{
  register int result __asm ("r0");
  volatile struct vc_dispmanx_element_change_attributes_params params;
  
  params.update = update;
  params.element = element;
  params.change_flags = change_flags;
  params.layer = layer;
  params.opacity = opacity;
  params.dest_rect = dest_rect;
  params.src_rect = src_rect;
  params.mask = mask;
  params.transform = transform;

  __asm volatile ("	MOV	r8, %[vc_DispManxElementChangeAttributes];\n"
		  "	MOV	r0, %[params];\n"
		  "	SWI	%[SWI_VC];\n"
		  : "=r" (result)
		  : [vc_DispManxElementChangeAttributes] "I" (vc_DispManxElementChangeAttributes),
		    [SWI_VC] "i" (SWI_VC),
		    [params] "r" (&params)
		  : "r8");
  return result;
}

