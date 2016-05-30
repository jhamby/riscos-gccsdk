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

DEFINE_FUNC	vc_dispmanx_display_close		SWI_VC	vc_DispManxDisplayClose
DEFINE_FUNC	vc_dispmanx_display_get_info		SWI_VC	vc_DispManxDisplayGetInfo
DEFINE_FUNC	vc_dispmanx_display_open		SWI_VC	vc_DispManxDisplayOpen
DEFINE_FUNC	vc_dispmanx_element_modified		SWI_VC	vc_DispManxElementModified
DEFINE_FUNC	vc_dispmanx_element_remove		SWI_VC	vc_DispManxElementRemove
DEFINE_FUNC	vc_dispmanx_element_change_source	SWI_VC	vc_DispManxElementChangeSource
DEFINE_FUNC	vc_dispmanx_element_change_layer	SWI_VC	vc_DispManxElementChangeLayer
DEFINE_FUNC	vc_dispmanx_resource_create		SWI_VC	vc_DispManxResourceCreate
DEFINE_FUNC	vc_dispmanx_resource_delete		SWI_VC	vc_DispManxResourceDelete
DEFINE_FUNC_5	vc_dispmanx_resource_write_data		SWI_VC	vc_DispManxResourceWriteData
DEFINE_FUNC	vc_dispmanx_snapshot			SWI_VC	vc_DispManxSnapshot
DEFINE_FUNC	vc_dispmanx_update_start		SWI_VC	vc_DispManxUpdateStart
DEFINE_FUNC	vc_dispmanx_update_submit_sync		SWI_VC	vc_DispManxUpdateSubmitSync
DEFINE_FUNC	vc_dispmanx_update_submit		SWI_VC	vc_DispManxUpdateSubmit
