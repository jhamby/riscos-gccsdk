/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the RISC OS platform plugin of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

/****************************************************************************
**
** Written by Lee Noar
**
****************************************************************************/

#include "qsprite.h"
#include <qdynamicarea.h>
#include "swis.h"
#include "oslib/osspriteop.h"

#include <QtGui/QImage>

QT_BEGIN_NAMESPACE

QSprite::QSprite (QDynamicArea *da) :
      m_dynamicArea(da),
      m_spriteArea(nullptr),
      m_spritePtr(nullptr),
      m_spriteData(nullptr),
      m_areaSize(0)
{
}

QSprite::~QSprite()
{
    if (!isNull())
	destroy();
}

bool
QSprite::create(const char *name,
		int width,
		int height,
		os_mode type)
{
    m_name = name;
    
    int byte_width = byteWidth(type, width);

    // Start with the size of a sprite area header which is 4 words and the
    // size of a sprite header which is 11 words.
    m_areaSize = sizeof(osspriteop_area) +
		 sizeof(osspriteop_header) +
		 byte_width * height;

    // FIXME: How do we handle failure?
    if (m_dynamicArea) {
	m_spriteArea = static_cast<osspriteop_area*>(m_dynamicArea->alloc(m_areaSize));
    } else {
	m_spriteArea = static_cast<osspriteop_area*>(malloc(m_areaSize));
    }

    if (m_spriteArea == nullptr) {
	fprintf (stderr, "Failed to allocate memory for sprite area.\n");
	return false;
    }

    m_spriteArea->size = m_areaSize;
    m_spriteArea->first = 16;
#if (defined(__VFP_FP__) && !defined(__SOFTFP__))
    const _kernel_oserror *err;
    err = _swix(OS_SpriteOp, _INR(0,1),
		osspriteop_USER_AREA | OSSpriteOp_ClearSprites,
		m_spriteArea);
#else
    const os_error *err;
    err = xosspriteop_clear_sprites(osspriteop_USER_AREA, m_spriteArea);
#endif
    if (err != nullptr) {
        fprintf (stderr, "Failed to create sprite area; %s.\n", err->errmess);
	return false;
    }

#if (defined(__VFP_FP__) && !defined(__SOFTFP__))
    err = _swix(OS_SpriteOp, _INR(0,6),
		osspriteop_USER_AREA | OSSpriteOp_CreateSprite,
		m_spriteArea,
		name,
		false,
		width,
		height,
		type);
#else
    err = xosspriteop_create_sprite (osspriteop_USER_AREA,
				     m_spriteArea,
				     name,
				     false,
				     width,
				     height,
				     type);
#endif
    if (err != nullptr) {
        fprintf (stderr, "Failed to create sprite; %s.\n", err->errmess);
	return false;
    }

    m_spriteData = getSpriteDataPtr();
    m_spritePtr = getSpritePtr();
    m_width = width;
    m_height = height;

    return true;
}

void
QSprite::destroy()
{
    if (m_dynamicArea) {
	m_dynamicArea->free(m_spriteArea, m_areaSize);
    } else {
	free(m_spriteArea);
    }

    m_spriteArea = nullptr;
    m_spritePtr = nullptr;
    m_spriteData = nullptr;
    m_areaSize = 0;
}

QImage
QSprite::asQImage() const
{
    return QImage (static_cast<uchar*>(m_spriteData),
		   m_width,
		   m_height,
		   QSysInfo::requireRedBlueSwap() ?
			QImage::Format_RGBX8888 :
			QImage::Format_RGB32);
}

void *
QSprite::getSpriteDataPtr () const
{
    osspriteop_header *sprite;

    sprite = (osspriteop_header *)((unsigned)m_spriteArea + m_spriteArea->first);

    return (void*)((unsigned)sprite + sprite->image);
}

osspriteop_header *
QSprite::getSpritePtr () const
{
    osspriteop_header *sprite_pointer = nullptr;
    const char *sprite_name = m_name.toUtf8().constData();

#if (defined(__VFP_FP__) && !defined(__SOFTFP__))
    _kernel_oserror *err;
    err = _swix(OS_SpriteOp, _INR(0,2)|_OUT(2),
		osspriteop_USER_AREA | OSSpriteOp_SelectSprite,
		m_spriteArea,
		sprite_name,
		&sprite_pointer);
#else
    os_error *err = nullptr;
    err = xosspriteop_select_sprite (osspriteop_USER_AREA,
				     m_spriteArea,
				     (osspriteop_id)sprite_name,
				     &sprite_pointer);
#endif
    return err ? nullptr : sprite_pointer;
}

int
QSprite::byteWidth(os_mode type, int pixel_width) const
{
    int log2bpp, byte_width = 0;

    _swix(OS_ReadModeVariable, _INR(0,1)|_OUT(2),
	  type,
	  os_VDUVAR_LOG2_BPP,
	  &log2bpp);

    switch (1 << log2bpp) {
    case 1:
	byte_width = pixel_width >> 3;
	break;
    case 2:
	byte_width = pixel_width >> 2;
	break;
    case 8:
	byte_width = pixel_width;
	break;
    case 15:
    case 16:
	byte_width = pixel_width << 1;
	break;
    case 24:
    case 32:
	byte_width = pixel_width << 2;
	break;
    }

    // Sprite rows are word aligned
    return (byte_width + 3) & ~3;
}

QT_END_NAMESPACE
