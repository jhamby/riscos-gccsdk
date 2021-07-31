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

#ifndef QRISCOSCURSOR_H
#define QRISCOSCURSOR_H

#include <qpa/qplatformcursor.h>
#include "qsprite.h"

QT_BEGIN_NAMESPACE

class QRiscosScreen;
class QCursor;

class QRiscosCursor : public QPlatformCursor
{
public:
    QRiscosCursor(QRiscosScreen *screen);

    void changeCursor(QCursor * widgetCursor, QWindow * widget) Q_DECL_OVERRIDE;
    QPoint pos() const Q_DECL_OVERRIDE;
//    void setPos(const QPoint &pos) Q_DECL_OVERRIDE;

private:
    struct riscos_pointer {
	QSprite sprite;

	// We can't trust the hot spot in the base class to be anything
	// other than 0;
	QPoint hot_spot;
    };

    QRiscosScreen *mScreen;

    QHash<Qt::CursorShape, riscos_pointer *> mCursors;

private:
    riscos_pointer *create_cursor(QCursor *);
};

QT_END_NAMESPACE

#endif
