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

#include "qriscoscursor.h"
#include "qriscosscreen.h"
#include "qsprite.h"
#include "oslib/wimp.h"
#include "oslib/wimpspriteop.h"
#include "swis.h"
#include <QPixmap>
#include "qpa/qplatformcursor.h"
#include "qnamespace.h"

QT_BEGIN_NAMESPACE

QRiscosCursor::QRiscosCursor(QRiscosScreen *screen) :
	mScreen(screen)
{
}

void
QRiscosCursor::changeCursor(QCursor *cursorIn, QWindow *window)
{
    /* Might as well use the default WIMP pointer. */
    if (cursorIn->shape() == Qt::ArrowCursor) {
        _swix(Wimp_SpriteOp, _IN(0)|_INR(2,7), WimpSpriteOp_SetPointerShape,
					       "ptr_default",
					       2,
					       0, 0,
					       nullptr,
					       nullptr);
	return;
    }
    
    QRiscosCursor::riscos_pointer *pointer = nullptr;

    if (mCursors.contains(cursorIn->shape())) {
	pointer = mCursors.value(cursorIn->shape());
    } else {
	pointer = create_cursor(cursorIn);
	mCursors.insert(cursorIn->shape(), pointer);
    }

    if (pointer)
	pointer->sprite.setPointer(pointer->hot_spot);
}

QPoint
QRiscosCursor::pos() const
{
    wimp_pointer mouse;
    _swix(Wimp_GetPointerInfo, _IN(1), &mouse);

    QPoint mousePos(mScreen->osPointToPixel(mouse.pos.x, mouse.pos.y));

    return mScreen->globalPoint(mousePos);;
}

QRiscosCursor::riscos_pointer *
QRiscosCursor::create_cursor(QCursor *cursor)
{
    const QString ptr_name = QString("ptr_qt_%1").arg(cursor->shape());

    // These sprites are small and permanent, so we can use malloc to
    // to store them.

    QPlatformCursorImage platform_cursor(nullptr, nullptr, 0, 0, 0, 0);
    platform_cursor.set(cursor->shape());

    QImage *image = platform_cursor.image();
    riscos_pointer *pointer = new riscos_pointer;
    pointer->sprite.create(ptr_name, image->width(), image->height(), (os_mode)19);
    pointer->hot_spot = platform_cursor.hotspot();

    int x, y, sprite_pixel;
    int width = image->width();
    int height = image->height();

    for (y = 0; y < height; y++) {
	for (x = 0; x < width; x++) {
	    switch (image->pixel(x, y)) {
	    case 0xFFFFFFFF:
		sprite_pixel = 1;
		break;
	    case 0xFF000000:
		sprite_pixel = 2;
		break;
	    default:
		sprite_pixel = 0;
		break;
	    }

	    // Flip the image on the Y axis
	    pointer->sprite.setPixel(x, height - y - 1, sprite_pixel);
	}
    }

    return pointer;
}

QT_END_NAMESPACE
