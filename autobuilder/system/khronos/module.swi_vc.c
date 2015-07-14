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
#include "../types.h"
#include "../enums.h"
#include "interface/vmcs_host/vc_dispmanx.h"

_kernel_oserror *vc_swi(_kernel_swi_regs *r)
{
  switch ((vc_reason_code)r->r[8])
  {
    case vc_DispManxDisplayOpen:
      r->r[0] = (int)vc_dispmanx_display_open((uint32_t)r->r[0]);
      break;
    case vc_DispManxUpdateStart:
      r->r[0] = (int)vc_dispmanx_update_start(r->r[0]);
      break;
    case vc_DispManxElementAdd: {
      struct vc_dispmanx_element_params *params = (struct vc_dispmanx_element_params *)r->r[0];
      r->r[0] = (int)vc_dispmanx_element_add(params->update,
					     params->display,
					     params->layer,
					     params->dest_rect,
					     params->src,
					     params->src_rect,
					     params->protection,
					     params->alpha,
					     params->clamp,
					     params->transform);
      break;
    }
    case vc_DispManxElementModified:
      r->r[0] = (int)vc_dispmanx_element_modified((DISPMANX_UPDATE_HANDLE_T)r->r[0],
						  (DISPMANX_ELEMENT_HANDLE_T)r->r[1],
						  (const VC_RECT_T *)r->r[2]);
      break;
    case vc_DispManxElementChangeAttributes: {
      struct vc_dispmanx_element_change_attributes_params *params =
			(struct vc_dispmanx_element_change_attributes_params *)r->r[0];

      r->r[0] = (int)vc_dispmanx_element_change_attributes(params->update, 
							   params->element,
							   params->change_flags,
							   params->layer,
							   params->opacity,
							   params->dest_rect,
							   params->src_rect,
							   params->mask,
							   params->transform);
      break;
    }
    case vc_DispManxElementChangeSource:
      r->r[0] = (int)vc_dispmanx_element_change_source((DISPMANX_UPDATE_HANDLE_T)r->r[0],
						       (DISPMANX_ELEMENT_HANDLE_T)r->r[1],
						       (DISPMANX_RESOURCE_HANDLE_T)r->r[2]);
      break;
    case vc_DispManxElementChangeLayer:
      r->r[0] = (int)vc_dispmanx_element_change_layer ((DISPMANX_UPDATE_HANDLE_T)r->r[0],
						       (DISPMANX_ELEMENT_HANDLE_T)r->r[1],
                                                       (int32_t)r->r[2]);
      break;
    case vc_DispManxUpdateSubmitSync:
      r->r[0] = (int)vc_dispmanx_update_submit_sync((DISPMANX_UPDATE_HANDLE_T)r->r[0]);
      break;
    case vc_DispManxDisplayGetInfo:
      r->r[0] = (int)vc_dispmanx_display_get_info((DISPMANX_UPDATE_HANDLE_T)r->r[0],
						  (DISPMANX_MODEINFO_T *)r->r[1]);
      break;
    case vc_DispManxResourceCreate:
      r->r[0] = (int)vc_dispmanx_resource_create((VC_IMAGE_TYPE_T)r->r[0],
						 (uint32_t)r->r[1],
						 (uint32_t)r->r[2],
						 (uint32_t *)r->r[3]);
      break;
    case vc_DispManxResourceWriteData:
      r->r[0] = (int)vc_dispmanx_resource_write_data((DISPMANX_RESOURCE_HANDLE_T)r->r[0],
						     (VC_IMAGE_TYPE_T)r->r[1],
						     r->r[2],
						     (void *)r->r[3],
						     (const VC_RECT_T *)r->r[4]);
      break;
    case vc_DispManxElementRemove:
      r->r[0] = (int)vc_dispmanx_element_remove((DISPMANX_UPDATE_HANDLE_T)r->r[0],
						(DISPMANX_ELEMENT_HANDLE_T)r->r[1]);
      break;
    case vc_DispManxResourceDelete:
      r->r[0] = (int)vc_dispmanx_resource_delete((DISPMANX_RESOURCE_HANDLE_T)r->r[0]);
      break;
    case vc_DispManxDisplayClose:
      r->r[0] = (int)vc_dispmanx_display_close((DISPMANX_DISPLAY_HANDLE_T)r->r[0]);
      break;
    case vc_DispManxSnapshot:
      r->r[0] = (int)vc_dispmanx_snapshot((DISPMANX_DISPLAY_HANDLE_T)r->r[0], 
                                          (DISPMANX_RESOURCE_HANDLE_T)r->r[1], 
                                          (DISPMANX_TRANSFORM_T)r->r[2]);
      break;
    default:
      return khronos_bad_vc_reason;
  }

  return 0;
}
