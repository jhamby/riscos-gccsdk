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

#ifndef QSPRITE_H
#define QSPRITE_H

#include <qdynamicarea.h>
#include "oslib/osspriteop.h"
#include "swis.h"

#include <QtGui/QImage>

QT_BEGIN_NAMESPACE

class QSprite
{
public:
    // If da is NULL, then use malloc. Useful for small permanent sprites.
    QSprite(QDynamicArea *da = nullptr);
    ~QSprite();

    // Returns true for success.
    bool create(const char *name, int width, int height, os_mode type);

    // Returns true for success.
    bool create(const QString &name, int width, int height, os_mode type)
    {
      return create(name.toUtf8().constData(),
		    width,
		    height,
		    type);
    }

    void destroy();

    os_error *render (int x, int y) const {
	if (isNull())
	    return nullptr;

#if (defined(__VFP_FP__) && !defined(__SOFTFP__))
	return (os_error *)_swix(OS_SpriteOp, _INR(0,5),
				 osspriteop_PTR | OSSpriteOp_PutSpriteUserCoords,
				 m_spriteArea,
				 m_spritePtr,
				 x,
				 y,
				 os_ACTION_OVERWRITE);
#else
	return xosspriteop_put_sprite_user_coords (osspriteop_PTR,
						   m_spriteArea,
						   (osspriteop_id)m_spritePtr,
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
				 osspriteop_PTR | OSSpriteOp_PutSpriteScaled,
				 m_spriteArea,
				 m_spritePtr,
				 x,
				 y,
				 os_ACTION_OVERWRITE,
				 factors,
				 table);
#else
	return xosspriteop_put_sprite_scaled (osspriteop_PTR,
					      m_spriteArea,
					      (osspriteop_id)m_spritePtr,
					      x,
					      y,
					      os_ACTION_OVERWRITE,
					      factors,
					      table);
#endif
    }

    int width() const {
	return m_width;
    }
    int height() const {
	return m_height;
    }

    void setPixel(int x, int y, unsigned colour) {
	_swix(OS_SpriteOp, _INR(0,6),
	      osspriteop_PTR | OSSpriteOp_WritePixelColour,
	      m_spriteArea,
	      m_spritePtr,
	      x,
	      y,
	      colour,
	      0);
    }

    os_error *setPointer(QPoint &active) {
	return (os_error *)_swix(OS_SpriteOp, _INR(0,7),
				 osspriteop_PTR | OSSpriteOp_SetPointerShape,
				 m_spriteArea,
				 m_spritePtr,
				 2, /* flags (pointer shape 2) */
				 active.x(), active.y(),
				 0, 0);
    }

    // Return true if there is no sprite area or sprite allocated.
    bool isNull() const {
	return m_spriteArea == nullptr || m_spritePtr == nullptr;
    }

    os_error *saveToFile(const char *filename) const {
#if (defined(__VFP_FP__) && !defined(__SOFTFP__))
	return (os_error *)_swix(OS_SpriteOp, _INR(0,2),
				 osspriteop_PTR | OSSpriteOp_SaveSpriteFile,
				 m_spriteArea,
				 filename);
#else
	return xosspriteop_save_sprite_file(osspriteop_PTR,
					    m_spriteArea,
					    filename);
#endif
    }

    QImage asQImage() const;

private:
    QDynamicArea *m_dynamicArea;
    QString m_name;
    osspriteop_area *m_spriteArea;
    osspriteop_header *m_spritePtr;
    void *m_spriteData;
    int m_areaSize;

    int m_width;
    int m_height;

private:
    void *getSpriteDataPtr () const;
    osspriteop_header *getSpritePtr () const;
    int byteWidth(os_mode, int) const;
};

QT_END_NAMESPACE

#endif
