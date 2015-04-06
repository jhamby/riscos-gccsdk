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
** This file defines a class that wraps a RISC OS window.
**
****************************************************************************/

#ifndef QRISCOSWINDOW_H
#define QRISCOSWINDOW_H

#include <qpa/qplatformwindow.h>
#include "qriscosintegration.h"
#include "oslib/wimp.h"

QT_BEGIN_NAMESPACE

#ifndef wimp_INVALID_WINDOW
#define wimp_INVALID_WINDOW (wimp_w)0xFFFFFFFF
#endif

class QRiscosWindow : public QPlatformWindow
{
public:
    QRiscosWindow(QWindow *window);
    ~QRiscosWindow();

    // Virtual functions that have been overridden.
    void setVisible(bool visible);
    
    void setGeometry(const QRect &rect);
    
    void setWindowTitle(const QString &title);

    WId winId() const {
	return WId(m_window);
    }

    void raise();

    void propagateSizeHints();

    // This is overridden from the base class, so we must assume that the
    // input and output are both pixels.
    QPoint mapFromGlobal(const QPoint& point) const;

    // Functions specific to RISC OS and therefore not virtual.
    int os_x() const {
	return m_screen->xPixelToOS(window()->x());
    }
    int os_y() const {
	return m_screen->yPixelToOS(window()->y());
    }
    int os_width() const {
	return m_screen->xPixelToOS(window()->width());
    }
    int os_height() const {
	return m_screen->yPixelToOS(window()->height());
    }

    void update () const;

    void setCaret () const;

private:
    void create();

    void openToplevel();
    void openNested();

    os_error *getState(wimp_window_state &state) const {
	state.w = m_window;
	return xwimp_get_window_state (&state);
    }

    os_error *setExtent(const QRect &rect) const {
	os_box extent;

	extent.x0 = extent.y1 = 0;
	extent.x1 = m_screen->xPixelToOS(rect.width());
	extent.y0 = -m_screen->yPixelToOS(rect.height());
	return xwimp_set_extent(m_window, &extent);
    }

    QRiscosScreen *m_screen;

    wimp_w m_window;
    char *m_title;
    char *m_titleValidation;
    int m_titleMaxSize;

    static const int window_title_max_len = 100;
};

QT_END_NAMESPACE

#endif
