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

#include "qriscosbackingstore.h"
#include "qriscosintegration.h"
#include "qriscoswindow.h"
#include "qscreen.h"
#include <qdynamicarea.h>
#include <private/qguiapplication_p.h>

QT_BEGIN_NAMESPACE

static const char sprite_name[] = "qtsurface";

QSprite::QSprite (QDynamicArea *da) :
      m_dynamicArea(da),
      m_surfaceSpriteArea(nullptr),
      m_surfaceSpritePtr(nullptr),
      m_surfaceSpriteData(nullptr),
      m_areaSize(0)
{
}

QSprite::~QSprite()
{
    if (!isNull())
	destroy();
}

bool
QSprite::create (int width, int height, QImage::Format format)
{
    const os_error *err = nullptr;

    // Backing store sprites are always 32 bits.
    int byte_width = 4 * width;

    // Start with the size of a sprite area header which is 4 words and the
    // size of a sprite header which is 11 words.
    m_areaSize = (4 * 4) + (11 * 4) + byte_width * height;

    // FIXME: How do we handle failure?
    m_surfaceSpriteArea = static_cast<osspriteop_area*>(m_dynamicArea->alloc(m_areaSize));
    if (m_surfaceSpriteArea == nullptr) {
        fprintf (stderr, "Failed to allocate memory in dynamic area for sprite area.\n");
	return false;
    }

    m_surfaceSpriteArea->size = m_areaSize;
    m_surfaceSpriteArea->first = 16;
    err = xosspriteop_clear_sprites(osspriteop_USER_AREA, m_surfaceSpriteArea);
    if (err != nullptr) {
        fprintf (stderr, "Failed to create sprite area; %s.\n", err->errmess);
	return false;
    }
    
    err = xosspriteop_create_sprite (osspriteop_USER_AREA,
				     m_surfaceSpriteArea,
				     sprite_name,
				     false,
				     width,
				     height,
				     QSprite::typeFromQImageFormat(format));
    if (err != nullptr) {
        fprintf (stderr, "Failed to create sprite; %s.\n", err->errmess);
	return false;
    }

    m_surfaceSpriteData = getSpriteDataPtr();
    m_surfaceSpritePtr = getSpritePtr();

    m_image = QImage (static_cast<uchar*>(m_surfaceSpriteData),
		      width,
		      height,
		      QSysInfo::requireRedBlueSwap() ?
			     QImage::Format_RGBX8888 :
			     QImage::Format_RGB32);

    return true;
}

void
QSprite::destroy()
{
    m_dynamicArea->free(m_surfaceSpriteArea, m_areaSize);

    m_surfaceSpriteArea = nullptr;
    m_surfaceSpritePtr = nullptr;
    m_surfaceSpriteData = nullptr;
    m_areaSize = 0;
}

os_mode
QSprite::typeFromQImageFormat (QImage::Format format)
{
    /* Using new style sprite mode word.  */
    osspriteop_mode_word type = osspriteop_NEW_STYLE;
    
    switch (format)
    {
    case QImage::Format_Mono:
	type |= (osspriteop_TYPE1BPP << osspriteop_TYPE_SHIFT);
	break;
    case QImage::Format_Indexed8:
	type |= (osspriteop_TYPE8BPP << osspriteop_TYPE_SHIFT);
	break;
    case QImage::Format_RGB16:
	type |= (osspriteop_TYPE16BPP << osspriteop_TYPE_SHIFT);
	break;
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied:
    case QImage::Format_Invalid:
    default:
	type |= (osspriteop_TYPE32BPP << osspriteop_TYPE_SHIFT);
	break;
    }

    /* Horizontal DPI.  */
    type |= (90 << osspriteop_XRES_SHIFT);
    /* Vertical DPI.  */
    type |= (90 << osspriteop_YRES_SHIFT);

    return (os_mode)type;
}

void *
QSprite::getSpriteDataPtr () const
{
    osspriteop_header *sprite;

    sprite = (osspriteop_header *)((unsigned)m_surfaceSpriteArea + m_surfaceSpriteArea->first);

    return (void*)((unsigned)sprite + sprite->image);
}

osspriteop_header *
QSprite::getSpritePtr () const
{
    os_error *err = nullptr;
    osspriteop_header *sprite_pointer = nullptr;

    err = xosspriteop_select_sprite (osspriteop_USER_AREA,
				     m_surfaceSpriteArea,
				     (osspriteop_id)sprite_name,
				     &sprite_pointer);
    return err ? nullptr : sprite_pointer;
}

QRiscosBackingStore::QRiscosBackingStore(QWindow *window)
    : QPlatformBackingStore(window),
      m_sprite(static_cast<QRiscosIntegration *>(QGuiApplicationPrivate::platformIntegration())->windowSprites())    
{
    QRiscosScreen *screen = static_cast<QRiscosScreen *>(window->screen()->handle());

    if (window->width() != 0 && window->height() != 0)
	m_sprite.create(window->width(), window->height(), QImage::Format_RGB32);

    screen->addSurface(this);
#if 0
    printf ("[%s:%d:%s] - this (%p), x (%d), y (%d), width (%d), height (%d) - bits (%p)\n",
	  __func__, __LINE__, __FILE__,this,
	  window->x(),window->y(),window->width(), window->height(), m_sprite.image().bits());
#endif
}

QRiscosBackingStore::~QRiscosBackingStore()
{
    QRiscosScreen *screen = static_cast<QRiscosScreen *>(window()->screen()->handle());

    screen->removeSurface(this);

    // m_sprite is destructed automatically.
}

QPaintDevice *
QRiscosBackingStore::paintDevice()
{
    QImage& image = m_sprite.image();
    Q_ASSERT(!image.isNull());

    return &image;
}

void
QRiscosBackingStore::flush(QWindow *window, const QRegion &region, const QPoint &offset)
{
    Q_UNUSED(region);
    Q_UNUSED(offset);

    QRiscosWindow *rwindow = static_cast<QRiscosWindow *>(window->handle());

    rwindow->update();
}

void
QRiscosBackingStore::resize(const QSize &size, const QRegion &staticContents)
{
    Q_UNUSED(staticContents);

    if (width() == size.width() && height() == size.height())
	return;
    if (!m_sprite.isNull())
	m_sprite.destroy();
    m_sprite.create(size.width(), size.height(), QImage::Format_RGB32);
}

QT_END_NAMESPACE
