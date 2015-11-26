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

#ifndef QBACKINGSTORE_RISCOS_H
#define QBACKINGSTORE_RISCOS_H

#include <qpa/qplatformbackingstore.h>
#include <qpa/qplatformwindow.h>
#include <qdynamicarea.h>
#include "oslib/osspriteop.h"
#include "swis.h"

#include <QtGui/QImage>

QT_BEGIN_NAMESPACE

class QSprite
{
public:
    QSprite(QDynamicArea *da);
    ~QSprite();

    // Returns true for success.
    bool create(int width, int height, QImage::Format format);

    void destroy();

    QImage& image() { return m_image; }

    os_error *render (int x, int y) const {
	if (isNull())
	    return nullptr;

#if (defined(__VFP_FP__) && !defined(__SOFTFP__))
	return (os_error *)_swix(OS_SpriteOp, _INR(0,5),
				 osspriteop_PTR | 34, // Put sprite at user coords
				 m_surfaceSpriteArea,
				 m_surfaceSpritePtr,
				 x,
				 y,
				 os_ACTION_OVERWRITE);
#else
	return xosspriteop_put_sprite_user_coords (osspriteop_PTR,
						   m_surfaceSpriteArea,
						   (osspriteop_id)m_surfaceSpritePtr,
						   x,
						   y,
						   os_ACTION_OVERWRITE);
#endif
    }

    os_error *render (int x, int y,
		      osspriteop_trans_tab const *table,
		      os_factors *factors = nullptr) const {
	if (isNull())
	    return nullptr;

#if (defined(__VFP_FP__) && !defined(__SOFTFP__))
	return (os_error *)_swix(OS_SpriteOp, _INR(0,7),
				 osspriteop_PTR | 52, // Put sprite scaled
				 m_surfaceSpriteArea,
				 m_surfaceSpritePtr,
				 x,
				 y,
				 os_ACTION_OVERWRITE,
				 factors,
				 table);
#else
	return xosspriteop_put_sprite_scaled (osspriteop_PTR,
					      m_surfaceSpriteArea,
					      (osspriteop_id)m_surfaceSpritePtr,
					      x,
					      y,
					      os_ACTION_OVERWRITE,
					      factors,
					      table);
#endif
    }

    int width() const {
	return m_image.width();
    }
    int height() const {
	return m_image.height();
    }

    // Return true if there is no sprite area or sprite allocated.
    bool isNull() const {
	return m_surfaceSpriteArea == nullptr || m_surfaceSpritePtr == nullptr;
    }

    os_error *saveToFile(const char *filename) const {
#if (defined(__VFP_FP__) && !defined(__SOFTFP__))
	return (os_error *)_swix(OS_SpriteOp, _INR(0,2),
				 osspriteop_PTR | 12, // Save sprite file
				 m_surfaceSpriteArea,
				 filename);
#else
	return xosspriteop_save_sprite_file(osspriteop_PTR,
					    m_surfaceSpriteArea,
					    filename);
#endif
    }

    static os_mode typeFromQImageFormat (QImage::Format format);

private:
    QDynamicArea *m_dynamicArea;
    osspriteop_area *m_surfaceSpriteArea;
    osspriteop_header *m_surfaceSpritePtr;
    void *m_surfaceSpriteData;
    int m_areaSize;

    QImage m_image;

private:
    void *getSpriteDataPtr () const;
    osspriteop_header *getSpritePtr () const;
};

class QRiscosBackingStore : public QPlatformBackingStore
{
public:
    QRiscosBackingStore(QWindow *window);
    ~QRiscosBackingStore();

    QPaintDevice *paintDevice();
    void flush(QWindow *window, const QRegion &region, const QPoint &offset);
    void resize(const QSize &size, const QRegion &staticContents);

    os_error *render (int x, int y) const {
	return m_sprite.render (x, y);
    }

    os_error *render (int x, int y,
		      osspriteop_trans_tab const *table,
		      os_factors *factors = nullptr) const {
	return m_sprite.render (x, y, table, factors);
    }

    int width() const {
	return m_sprite.width();
    }
    int height() const {
	return m_sprite.height();
    }

    os_error *saveToFile(const char *filename) const {
	return m_sprite.saveToFile(filename);
    }

    QImage &image() {
	return m_sprite.image();
    }
private:
    QSprite m_sprite;
};

QT_END_NAMESPACE

#endif
