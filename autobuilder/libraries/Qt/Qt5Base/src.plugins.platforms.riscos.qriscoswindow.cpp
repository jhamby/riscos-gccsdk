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

#include <QScreen>
#include "qriscoswindow.h"
#include "qriscosintegration.h"
#include "qriscosbackingstore.h"

#include <stdio.h>

#include "oslib/report.h"
#include <unixlib/local.h>

QRiscosWindow::QRiscosWindow (QWindow *window)
    : QPlatformWindow (window)
    , m_window(wimp_INVALID_WINDOW)
    , m_title(nullptr)
    , m_titleValidation(nullptr)
    , m_titleMaxSize(0)
{
    m_screen = static_cast<QRiscosScreen *>(window->screen()->handle());
    
    if (window->type() != Qt::ForeignWindow)
	create();
#if 0
    printf ("[%s:%d:%s] - x (%d), y (%d), width (%d), height (%d), parent (%p), id(%p), flags (%X)\n",
	  __func__, __LINE__, __FILE__,
	  window->x(), window->y(),
	  os_width(), os_height(),
	  parent(), m_window,
	  window->flags());
#endif
}

QRiscosWindow::~QRiscosWindow ()
{
    if (m_window != wimp_INVALID_WINDOW && window()->type() != Qt::ForeignWindow) {
	os_error *err;

	err = xwimp_delete_window (m_window);
	if (err)
	    qWarning ("RISCOS: Failed to delete window; %s\n", err->errmess);
    }

    if (m_title)
	delete [] m_title;
}

void QRiscosWindow::create()
{
    if (window()->type() == Qt::ForeignWindow) {
	m_window = (wimp_w)window()->winId();
        return;
    }

    os_error *err;
    wimp_window window_blk;
    
    window_blk.flags = wimp_WINDOW_NEW_FORMAT |
		       wimp_WINDOW_MOVEABLE/* |
		       wimp_WINDOW_AUTO_REDRAW*/;

    if (window()->isTopLevel()) {
	m_title = new char[window_title_max_len];
	m_titleValidation = NULL;
	m_titleMaxSize = window_title_max_len;

	if (m_title == NULL) {
	    qWarning ("RISCOS: Failed to allocate memory for window title");
	    m_window = wimp_INVALID_WINDOW;
	    return;
	}

	*m_title = '\0';

	Qt::WindowFlags flags = window()->flags();
	if (flags & Qt::WindowMinMaxButtonsHint)
	    window_blk.flags |= wimp_WINDOW_TOGGLE_ICON;
	if (flags & Qt::WindowCloseButtonHint)
	    window_blk.flags |= wimp_WINDOW_CLOSE_ICON;
	switch (flags & Qt::WindowType_Mask) {
	case Qt::Window:
	case Qt::Dialog:
	    window_blk.flags |= wimp_WINDOW_TITLE_ICON;
	    break;
	}

	window_blk.title_fg = wimp_COLOUR_BLACK;

	QSize os_screen_size = m_screen->sizeOS();

	window_blk.extent.x1 = os_screen_size.width();
	window_blk.extent.y0 = -os_screen_size.height();
    }
    else {
	window_blk.title_fg = 0xff; // No frame on child windows.
	window_blk.extent.x1 = os_width();
	window_blk.extent.y0 = -os_height();
    }

    window_blk.title_bg = wimp_COLOUR_LIGHT_GREY;
    window_blk.work_fg = wimp_COLOUR_BLACK;
    window_blk.work_bg = wimp_COLOUR_WHITE;
    window_blk.scroll_outer = wimp_COLOUR_MID_LIGHT_GREY;
    window_blk.scroll_inner = wimp_COLOUR_VERY_LIGHT_GREY;
    window_blk.highlight_bg = wimp_COLOUR_CREAM;
    window_blk.extra_flags = 0;
    window_blk.extent.x0 = 0;
    window_blk.extent.y1 = 0;
    window_blk.title_flags = wimp_ICON_TEXT |
			 wimp_ICON_HCENTRED |
			 wimp_ICON_VCENTRED |
			 wimp_ICON_INDIRECTED;
    // Using a wimp_BUTTON_CLICK_DRAG button here causes problems with sliders;
    // the ability to drag them becomes erratic.
    window_blk.work_flags = wimp_BUTTON_CLICK << wimp_ICON_BUTTON_TYPE_SHIFT;
    window_blk.xmin = 0;
    window_blk.ymin = 0;
    window_blk.icon_count = 0;
  
    window_blk.title_data.indirected_text_and_sprite.text = m_title;
    window_blk.title_data.indirected_text_and_sprite.validation = (char *)-1;
    window_blk.title_data.indirected_text_and_sprite.size = window_title_max_len;

    err = xwimp_create_window (&window_blk, &m_window);
    if (err) {
	qWarning ("RISCOS: Failed to create window; %s", err->errmess);
	m_window = wimp_INVALID_WINDOW;
	return;
    }
}

void QRiscosWindow::openToplevel()
{
    wimp_open open;
    os_error *err;

    open.w = m_window;
#if 1
    open.visible.x0 = os_x();
    open.visible.x1 = open.visible.x0 + os_width();
    open.visible.y1 = m_screen->heightOS() - os_y();
    open.visible.y0 = open.visible.y1 - os_height();
#else
    open.visible.x0 = os_x();
    open.visible.x1 = os_x() + os_width();
    open.visible.y1 = os_y() + os_height();
    open.visible.y0 = os_y();
#endif
    open.xscroll = 0;
    open.yscroll = 0;
    open.next = wimp_BACKGROUND;
    err = xwimp_open_window (&open);
    if (err)
	qWarning ("RISCOS: Failed to open window; %s", err->errmess);
}

void QRiscosWindow::openNested()
{
    wimp_open open;
    os_error *err;
    wimp_window_state parent_state;
    QRiscosWindow *my_parent = static_cast<QRiscosWindow *>(parent());

    my_parent->getState(parent_state);

    open.w = m_window;
    open.visible.x0 = parent_state.visible.x0 + os_x();
    open.visible.y0 = parent_state.visible.y0 + os_y();
    open.visible.x1 = open.visible.x0 + os_width();
    open.visible.y1 = open.visible.y0 + os_height();
    open.xscroll = 0;
    open.yscroll = 0;
    open.next = wimp_BACKGROUND;
    err = xwimp_open_window_nested (&open,
				    my_parent->m_window,
				    wimp_CHILD_LINKS_PARENT_WORK_AREA);
    if (err)
	qWarning ("RISCOS: Failed to open nested window; %s", err->errmess);
}

void QRiscosWindow::setVisible(bool visible)
{
    if (m_window == wimp_INVALID_WINDOW)
	return;

    if (!visible) {
	xwimp_close_window(m_window);
    } else {
	QPlatformWindow::setVisible (visible);

        if (parent())
	    openNested();
	else
	    openToplevel();
    }
}

void QRiscosWindow::setWindowTitle(const QString &title)
{
    QByteArray ba = title.toLocal8Bit();
    const char *c_str = ba.data();

    strncpy (m_title, c_str, m_titleMaxSize);
    m_title[m_titleMaxSize - 1] = '\0';

    if (window()->isVisible() && m_window != wimp_INVALID_WINDOW)
	xwimp_force_redraw_title (m_window);
}

// Pixels in, pixels out
QPoint
QRiscosWindow::mapFromGlobal(const QPoint& point) const
{
    wimp_window_state state;
    QPoint os_point = m_screen->pixelPointToOS(point);

    getState(state);

    QPoint origin = QPoint(state.visible.x0 - state.xscroll,
			   state.visible.y1 - state.yscroll);
    return m_screen->osPointToPixel(os_point.x() - origin.x(),
				    -(os_point.y() - origin.y()));
}

void
QRiscosWindow::update() const
{
    wimp_draw update;
    os_coord origin;
    osbool more;

    update.w = m_window;
    update.box.x0 = 0;
    update.box.y0 = -os_height();
    update.box.x1 = os_width();
    update.box.y1 = 0;
    xwimp_update_window (&update, &more);

    origin.x = update.box.x0 - update.xscroll;
    origin.y = update.box.y1 - update.yscroll - os_height();

    QRiscosBackingStore *surface = m_screen->surfaceFromId(update.w);

    while (more) {
        if (m_screen->depth() == 32) {
	    // Fast case; render 32 bit to 32 bit.
	    surface->render (origin.x, origin.y);
	} else {
	    // Render 32 bit to something else...
	    surface->render (origin.x, origin.y,
			     m_screen->translationTable());
	}
	xwimp_get_rectangle (&update, &more);
    }
}

void
QRiscosWindow::setCaret() const
{
    os_error *err =
    xwimp_set_caret_position (m_window,
			      wimp_ICON_WINDOW,
			      0, 0,
			      1 << 25, // Flags: Invisible caret.
			      0);
    if (err)
        qWarning("RISC OS: Failed to set caret; %s\n", err->errmess);
}

void
QRiscosWindow::setGeometry(const QRect &rect)
{
    QPlatformWindow::setGeometry(rect);

    if (window()->isVisible() && m_window != wimp_INVALID_WINDOW) {
	if (parent())
	    openNested();
	else
	    openToplevel();
    }
}

void
QRiscosWindow::raise()
{
}

void
QRiscosWindow::propagateSizeHints()
{
    //shut up warning from default implementation
}

bool
QRiscosWindow::isActive() const
{
    // FIXME: Do we need to check for something here to determine
    // if the window is active? For example, is the pointer within it.
    return true;
}
