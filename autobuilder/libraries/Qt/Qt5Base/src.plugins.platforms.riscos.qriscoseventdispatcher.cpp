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

#include <QCoreApplication>
#include <QGuiApplication>
#include <qscreen.h>
#include "qriscoseventdispatcher.h"
#include "qriscosintegration.h"
#include "qriscosbackingstore.h"
#include "qriscoswindow.h"
#include <qpa/qwindowsysteminterface.h>
#include <private/qcoreapplication_p.h>

#include "oslib/wimp.h"
#include "oslib/osbyte.h"
#include "oslib/report.h"

#include <assert.h>

QT_BEGIN_NAMESPACE

QT_USE_NAMESPACE

struct keymapping
{
    int riscos_key_code;
    Qt::Key qt_key_code;
};

static keymapping function_keymap[] {
    { wimp_KEY_LOGO, Qt::Key_Meta },
    { wimp_KEY_MENU, Qt::Key_Menu },
    { wimp_KEY_BACKSPACE, Qt::Key_Backspace },
    { wimp_KEY_RETURN, Qt::Key_Return },
    { wimp_KEY_ESCAPE, Qt::Key_Escape },
    { wimp_KEY_HOME, Qt::Key_Home },
    { wimp_KEY_DELETE, Qt::Key_Delete },
    { wimp_KEY_PRINT, Qt::Key_Print },
    { wimp_KEY_F1, Qt::Key_F1 },
    { wimp_KEY_F2, Qt::Key_F2 },
    { wimp_KEY_F3, Qt::Key_F3 },
    { wimp_KEY_F4, Qt::Key_F4 },
    { wimp_KEY_F5, Qt::Key_F5 },
    { wimp_KEY_F6, Qt::Key_F6 },
    { wimp_KEY_F7, Qt::Key_F7 },
    { wimp_KEY_F8, Qt::Key_F8 },
    { wimp_KEY_F9, Qt::Key_F9 },
    { wimp_KEY_TAB, Qt::Key_Tab },
    { wimp_KEY_COPY, Qt::Key_End },
    { wimp_KEY_LEFT, Qt::Key_Left },
    { wimp_KEY_RIGHT, Qt::Key_Right },
    { wimp_KEY_DOWN, Qt::Key_Down },
    { wimp_KEY_UP, Qt::Key_Up },
    { wimp_KEY_F10, Qt::Key_F10 },
    { wimp_KEY_F11, Qt::Key_F11 },
    { wimp_KEY_F12, Qt::Key_F12 },
    { wimp_KEY_INSERT, Qt::Key_Insert },
    { wimp_KEY_PRINT | wimp_KEY_SHIFT, Qt::Key_Print },
    { wimp_KEY_F1 | wimp_KEY_SHIFT, Qt::Key_F1 },
    { wimp_KEY_F2 | wimp_KEY_SHIFT, Qt::Key_F2 },
    { wimp_KEY_F3 | wimp_KEY_SHIFT, Qt::Key_F3 },
    { wimp_KEY_F4 | wimp_KEY_SHIFT, Qt::Key_F4 },
    { wimp_KEY_F5 | wimp_KEY_SHIFT, Qt::Key_F5 },
    { wimp_KEY_F6 | wimp_KEY_SHIFT, Qt::Key_F6 },
    { wimp_KEY_F7 | wimp_KEY_SHIFT, Qt::Key_F7 },
    { wimp_KEY_F8 | wimp_KEY_SHIFT, Qt::Key_F8 },
    { wimp_KEY_F9 | wimp_KEY_SHIFT, Qt::Key_F9 },
    { wimp_KEY_TAB | wimp_KEY_SHIFT, Qt::Key_Tab },
    { wimp_KEY_COPY | wimp_KEY_SHIFT, Qt::Key_End },
    { wimp_KEY_LEFT | wimp_KEY_SHIFT, Qt::Key_Left },
    { wimp_KEY_RIGHT | wimp_KEY_SHIFT, Qt::Key_Right },
    { wimp_KEY_DOWN | wimp_KEY_SHIFT, Qt::Key_PageDown },
    { wimp_KEY_UP | wimp_KEY_SHIFT, Qt::Key_PageUp },
    { wimp_KEY_F10 | wimp_KEY_SHIFT, Qt::Key_F10 },
    { wimp_KEY_F11 | wimp_KEY_SHIFT, Qt::Key_F11 },
    { wimp_KEY_F12 | wimp_KEY_SHIFT, Qt::Key_F12 },
    { wimp_KEY_INSERT | wimp_KEY_SHIFT, Qt::Key_Insert },
    { wimp_KEY_PRINT | wimp_KEY_CONTROL, Qt::Key_Print },
    { wimp_KEY_F1 | wimp_KEY_CONTROL, Qt::Key_F1 },
    { wimp_KEY_F2 | wimp_KEY_CONTROL, Qt::Key_F2 },
    { wimp_KEY_F3 | wimp_KEY_CONTROL, Qt::Key_F3 },
    { wimp_KEY_F4 | wimp_KEY_CONTROL, Qt::Key_F4 },
    { wimp_KEY_F5 | wimp_KEY_CONTROL, Qt::Key_F5 },
    { wimp_KEY_F6 | wimp_KEY_CONTROL, Qt::Key_F6 },
    { wimp_KEY_F7 | wimp_KEY_CONTROL, Qt::Key_F7 },
    { wimp_KEY_F8 | wimp_KEY_CONTROL, Qt::Key_F8 },
    { wimp_KEY_F9 | wimp_KEY_CONTROL, Qt::Key_F9 },
    { wimp_KEY_TAB | wimp_KEY_CONTROL, Qt::Key_Tab },
    { wimp_KEY_COPY | wimp_KEY_CONTROL, Qt::Key_End },
    { wimp_KEY_LEFT | wimp_KEY_CONTROL, Qt::Key_Left },
    { wimp_KEY_RIGHT | wimp_KEY_CONTROL, Qt::Key_Right },
    { wimp_KEY_DOWN | wimp_KEY_CONTROL, Qt::Key_Down },
    { wimp_KEY_UP | wimp_KEY_CONTROL, Qt::Key_Up },
    { wimp_KEY_F10 | wimp_KEY_CONTROL, Qt::Key_F10 },
    { wimp_KEY_F11 | wimp_KEY_CONTROL, Qt::Key_F11 },
    { wimp_KEY_F12 | wimp_KEY_CONTROL, Qt::Key_F12 },
    { wimp_KEY_INSERT | wimp_KEY_CONTROL, Qt::Key_Insert },
    { wimp_KEY_PRINT | wimp_KEY_SHIFT | wimp_KEY_CONTROL, Qt::Key_Print },
    { wimp_KEY_F1 | wimp_KEY_SHIFT | wimp_KEY_CONTROL, Qt::Key_F1 },
    { wimp_KEY_F2 | wimp_KEY_SHIFT | wimp_KEY_CONTROL, Qt::Key_F2 },
    { wimp_KEY_F3 | wimp_KEY_SHIFT | wimp_KEY_CONTROL, Qt::Key_F3 },
    { wimp_KEY_F4 | wimp_KEY_SHIFT | wimp_KEY_CONTROL, Qt::Key_F4 },
    { wimp_KEY_F5 | wimp_KEY_SHIFT | wimp_KEY_CONTROL, Qt::Key_F5 },
    { wimp_KEY_F6 | wimp_KEY_SHIFT | wimp_KEY_CONTROL, Qt::Key_F6 },
    { wimp_KEY_F7 | wimp_KEY_SHIFT | wimp_KEY_CONTROL, Qt::Key_F7 },
    { wimp_KEY_F8 | wimp_KEY_SHIFT | wimp_KEY_CONTROL, Qt::Key_F8 },
    { wimp_KEY_F9 | wimp_KEY_SHIFT | wimp_KEY_CONTROL, Qt::Key_F9 },
    { wimp_KEY_TAB | wimp_KEY_SHIFT | wimp_KEY_CONTROL, Qt::Key_Tab },
    { wimp_KEY_COPY | wimp_KEY_SHIFT | wimp_KEY_CONTROL, Qt::Key_End },
    { wimp_KEY_LEFT | wimp_KEY_SHIFT | wimp_KEY_CONTROL, Qt::Key_Left },
    { wimp_KEY_RIGHT | wimp_KEY_SHIFT | wimp_KEY_CONTROL, Qt::Key_Right },
    { wimp_KEY_DOWN | wimp_KEY_SHIFT | wimp_KEY_CONTROL, Qt::Key_Down },
    { wimp_KEY_UP | wimp_KEY_SHIFT | wimp_KEY_CONTROL, Qt::Key_Up },
    { wimp_KEY_F10 | wimp_KEY_SHIFT | wimp_KEY_CONTROL, Qt::Key_F10 },
    { wimp_KEY_F11 | wimp_KEY_SHIFT | wimp_KEY_CONTROL, Qt::Key_F11 },
    { wimp_KEY_F12 | wimp_KEY_SHIFT | wimp_KEY_CONTROL, Qt::Key_F12 },
    { wimp_KEY_INSERT | wimp_KEY_SHIFT | wimp_KEY_CONTROL, Qt::Key_Insert }
};

QRiscosEventDispatcherPrivate::QRiscosEventDispatcherPrivate() :
	m_eventType(0),
	m_systemDrag(false),
	m_lastMouseMovePos(QPoint(-1, -1))
{
    m_lastClick.w = (wimp_w)-1;
}

QRiscosEventDispatcher::QRiscosEventDispatcher(QObject *parent)
    : QEventDispatcherUNIX(*new QRiscosEventDispatcherPrivate, parent)
//    : QAbstractEventDispatcher(*new QRiscosEventDispatcherPrivate, parent)
{
//    Q_D(QRiscosEventDispatcher);
}

void QRiscosEventDispatcher::flush()
{
}

QRiscosEventDispatcher::~QRiscosEventDispatcher()
{
//    Q_D(QRiscosEventDispatcher);
}

bool QRiscosEventDispatcher::processEvents(QEventLoop::ProcessEventsFlags flags)
{
    Q_UNUSED(flags)

    Q_D(QRiscosEventDispatcher);

    QCoreApplicationPrivate::sendPostedEvents(0, 0, d->threadData);

    emit aboutToBlock();
    d->poll ();
    emit awake();
    
    switch (d->getNextEvent())
    {
    case wimp_NULL_REASON_CODE:
	d->handleNullEvent();
	break;
    case wimp_REDRAW_WINDOW_REQUEST:
	d->handleRedrawEvent();
	break;
    case wimp_OPEN_WINDOW_REQUEST:
	d->handleOpenEvent();
	break;
    case wimp_CLOSE_WINDOW_REQUEST:
	d->handleCloseEvent();
	break;
    case wimp_MOUSE_CLICK:
	d->handleMouseEvent();
	break;
    case wimp_KEY_PRESSED:
	d->handleKeyPressEvent();
	break;
    case wimp_USER_MESSAGE:
	d->handleWimpUserMessage();
	break;
    }

    return true;
}

bool QRiscosEventDispatcher::hasPendingEvents()
{
    return true;
}
#if 0
void QRiscosEventDispatcher::registerSocketNotifier(QSocketNotifier *notifier)
{
    Q_UNUSED(notifier)
}

void QRiscosEventDispatcher::unregisterSocketNotifier(QSocketNotifier *notifier)
{
    Q_UNUSED(notifier)
}
#endif
void
QRiscosEventDispatcherPrivate::handleOpenEvent()
{
    QGuiApplication *app = static_cast<QGuiApplication *>(QCoreApplication::instance());
    QRiscosScreen *screen = static_cast<QRiscosScreen *>(app->primaryScreen()->handle());
    wimp_open &open = m_pollBlock.open;

    xwimp_open_window (&open);

    QRiscosWindow *rwindow = screen->windowFromId (open.w);
    if (!rwindow)
	return;

    QWindow *qwindow = rwindow->window();
    QPoint oldPos(qwindow->x(), qwindow->y());
    QSize oldSize(qwindow->width(), qwindow->height());
    int new_os_width = open.visible.x1 - open.visible.x0;
    int new_os_height = open.visible.y1 - open.visible.y0;
    QSize newSize(screen->osSizeToPixel(new_os_width, new_os_height));
    QSize screen_os_size = screen->sizeOS();
    QPoint newPos(screen->osPointToPixel(open.visible.x0, screen_os_size.height() - open.visible.y1));

    // It would seem that we have to update the QWindow geometry ourselves. The
    // other platforms don't seem to, but if we don't do it here, then it doesn't happen.
    if (newPos != oldPos) {
	qwindow->setX(newPos.x());
	qwindow->setY(newPos.y());
	QMoveEvent event (newPos, oldPos);
	QCoreApplication::sendEvent(qwindow, &event);
    }
    if (newSize != oldSize) {
	qwindow->setWidth(newSize.width());
	qwindow->setHeight(newSize.height());
	QResizeEvent event (newSize, oldSize);
	QCoreApplication::sendEvent(qwindow, &event);
    }
}

void
QRiscosEventDispatcherPrivate::handleCloseEvent()
{
    QGuiApplication *app = static_cast<QGuiApplication *>(QCoreApplication::instance());
    QRiscosScreen *screen = static_cast<QRiscosScreen *>(app->primaryScreen()->handle());
    wimp_close &close = m_pollBlock.close;

    QRiscosWindow *rwindow = screen->windowFromId (close.w);
    if (!rwindow)
	return;

    QCloseEvent event;
    QCoreApplication::sendEvent(rwindow->window(), &event);
}

static os_error *
null_redraw (wimp_draw *redraw)
{
    os_error *err;
    osbool more;

    qWarning ("RISCOS: Unrecognised window %p during redraw event.\n", redraw->w);

    err = xwimp_redraw_window (redraw, &more);
    if (!err) {
	while (more)
	    err = xwimp_get_rectangle (redraw, &more);
    }

    return err;
}

void
QRiscosEventDispatcherPrivate::handleRedrawEvent()
{
    QGuiApplication *app = static_cast<QGuiApplication *>(QCoreApplication::instance());
    assert(app);
    QRiscosScreen *screen = static_cast<QRiscosScreen *>(app->primaryScreen()->handle());
    assert(screen);
    wimp_draw &redraw = m_pollBlock.redraw;
    os_error *err;

    QRiscosBackingStore *surface = screen->surfaceFromId (redraw.w);
    if (!surface)
    {
        // This should never happen.
	err = null_redraw(&redraw);
	if (err)
	    qWarning ("RISCOS: Error during redraw event; %s\n", err->errmess);
	return;
    }
    
    osbool more;
    os_coord origin;

    err = xwimp_redraw_window (&redraw, &more);
    
    origin.x = redraw.box.x0 - redraw.xscroll;
    origin.y = redraw.box.y1 - redraw.yscroll -
	       screen->yPixelToOS(surface->height());
    
    while (more) {
	err = surface->render (origin.x, origin.y);
	xwimp_get_rectangle (&redraw, &more);
    }
}

static Qt::MouseButton
translate_mouse_buttons (wimp_mouse_state mouse_buttons)
{
    // Have to use uint32_t here instead of Qt::MouseButton to prevent compiler errors.
    uint32_t state = Qt::NoButton;

    if (mouse_buttons & (wimp_DOUBLE_SELECT | wimp_SINGLE_SELECT | wimp_DRAG_SELECT))
	state |= Qt::LeftButton;
    if (mouse_buttons & wimp_CLICK_MENU)
	state |= Qt::MiddleButton;
    if (mouse_buttons & (wimp_DOUBLE_ADJUST | wimp_SINGLE_ADJUST | wimp_DRAG_ADJUST))
	state |= Qt::RightButton;

    return (Qt::MouseButton)state;
}

static Qt::MouseButton
translate_mouse_button (wimp_mouse_state mouse_button)
{
    Qt::MouseButton button = Qt::NoButton;

    if (mouse_button & (wimp_DOUBLE_SELECT | wimp_SINGLE_SELECT | wimp_DRAG_SELECT))
	button = Qt::LeftButton;
    if (mouse_button & wimp_CLICK_MENU)
	button = Qt::MiddleButton;
    if (mouse_button & (wimp_DOUBLE_ADJUST | wimp_SINGLE_ADJUST | wimp_DRAG_ADJUST))
	button = Qt::RightButton;

    return button;
}

static Qt::KeyboardModifier
read_keyboard_modifiers ()
{
    // Have to use uint32_t here instead of Qt::KeyboardModifier due to compiler
    // errors.
    uint32_t modifiers = Qt::NoModifier;

    // Check for shift key. Internal key code 0 covers both left and right.
    if (osbyte1 (osbyte_IN_KEY, 0 ^ 0xff, 0xff)) {
	modifiers |= Qt::ShiftModifier;
    }
    // Check for control key. Internal key code 1 covers both left and right.
    if (osbyte1 (osbyte_IN_KEY, 1 ^ 0xff, 0xff)) {
	modifiers |= Qt::ControlModifier;
    }
    // Check for alt key. Internal key code 2 covers both left and right.
    if (osbyte1 (osbyte_IN_KEY, 2 ^ 0xff, 0xff)) {
	modifiers |= Qt::AltModifier;
    }
    return (Qt::KeyboardModifier)modifiers;
}

static Qt::Key
map_keyboard (int key)
{
    Qt::Key result = Qt::Key_unknown;

    if (key >= 0x20 && key <= 0x7E)
	result = (Qt::Key)key;
    else {
	for (uint32_t i = 0; i < sizeof (function_keymap) / sizeof (keymapping); i++)
	    if (function_keymap[i].riscos_key_code == key) {
		result = function_keymap[i].qt_key_code;
		break;
	    }
    }

    return result;
}

static QWindow *
foreignWindowFromId (wimp_w window)
{
    QWindowList list = QGuiApplication::topLevelWindows();
    for (int i = list.size()-1; i >= 0; --i) {
        QWindow *w = list[i];
        if (w->winId() == WId(window))
            return w;
    }

    return nullptr;
}

void
QRiscosEventDispatcherPrivate::foreignMouseEvent(QWindow *qwindow,
						 wimp_pointer &mouse)
{
    Qt::KeyboardModifier modifiers = read_keyboard_modifiers();

    QMouseEvent event(QEvent::MouseButtonPress,
		      QPoint(),
		      QPoint(),
		      translate_mouse_button(mouse.buttons),
		      translate_mouse_buttons(mouse.buttons),
		      modifiers);
    QCoreApplication::sendEvent(qwindow, &event);
}

void
QRiscosEventDispatcherPrivate::startResizeWindow(wimp_pointer &mouse)
{
    // TODO: Detect mouse position and initiate only in specific places?
    //       Allow resizes in fixed/any direction?
    wimp_drag drag;

    drag.w = mouse.w;
    drag.type = wimp_DRAG_SYSTEM_SIZE;
    if (xwimp_drag_box(&drag) == nullptr) {
	m_systemDrag = true;
    }
}

void
QRiscosEventDispatcherPrivate::handleMouseEvent()
{
    QGuiApplication *app = static_cast<QGuiApplication *>(QCoreApplication::instance());
    QRiscosScreen *screen = static_cast<QRiscosScreen *>(app->primaryScreen()->handle());
    wimp_pointer& mouse = m_pollBlock.pointer;

    QRiscosWindow *rwindow = screen->windowFromId(mouse.w);
    if (!rwindow) {
	// This is for the iconbar.
	QWindow *qwindow = foreignWindowFromId(mouse.w);
	if (qwindow)
	    foreignMouseEvent(qwindow, mouse);
	return;
    }

    QPoint mousePos(screen->osPointToPixel(mouse.pos.x, mouse.pos.y));
    QPoint globalPos = screen->globalPoint(mousePos);
    QPoint localPos = rwindow->mapFromGlobal(mousePos);
    Qt::KeyboardModifier modifiers = read_keyboard_modifiers();

    rwindow->setCaret();

#if 0
    if (mouse.buttons & wimp_CLICK_SELECT && modifiers & Qt::ControlModifier) {
	QRiscosBackingStore *surface = screen->surfaceFromId (mouse.w);
	surface->saveToFile("ram:$.surface");
	return;
    }
#endif

    QMouseEvent event(QEvent::MouseButtonPress,
		      localPos,
		      globalPos,
		      translate_mouse_button(mouse.buttons),
		      translate_mouse_buttons(mouse.buttons),
		      modifiers);
    QCoreApplication::sendEvent(rwindow->window(), &event);

    m_lastClick = mouse;
}

void
QRiscosEventDispatcherPrivate::handleKeyPressEvent()
{
    QGuiApplication *app = static_cast<QGuiApplication *>(QCoreApplication::instance());
    QRiscosScreen *screen = static_cast<QRiscosScreen *>(app->primaryScreen()->handle());
    wimp_key& key = m_pollBlock.key;

    QRiscosWindow *rwindow = screen->windowFromId (key.w);
    if (!rwindow)
	return;

    Qt::KeyboardModifier modifiers = read_keyboard_modifiers();
    int qt_key_code = map_keyboard(key.c);
    QString key_ascii;

    if (qt_key_code >= Qt::Key_Space && qt_key_code <= Qt::Key_AsciiTilde)
	key_ascii = QString (QChar (qt_key_code));
 
    QKeyEvent event(QEvent::KeyPress,
		    map_keyboard(key.c),
		    modifiers,
		    key_ascii);
    QCoreApplication::sendEvent(rwindow->window(), &event);
}

void
QRiscosEventDispatcherPrivate::handleNullEvent()
{
    Q_Q(QRiscosEventDispatcher);
    QGuiApplication *app;
    QRiscosScreen *screen;
  
    app = static_cast<QGuiApplication *>(QCoreApplication::instance());
    assert(app);
    screen = static_cast<QRiscosScreen *>(app->primaryScreen()->handle());
    assert(screen);

    wimp_pointer mouse;

    xwimp_get_pointer_info(&mouse);

    q->activateTimers();

    QRiscosWindow *rwindow = screen->windowFromId (mouse.w);
    if (!rwindow)
	return;

    QPoint mousePos(screen->osPointToPixel(mouse.pos.x, mouse.pos.y));
    QPoint globalPos = screen->globalPoint(mousePos);
    QPoint localPos = rwindow->mapFromGlobal(mousePos);
    Qt::KeyboardModifier modifiers = read_keyboard_modifiers();

    Qt::MouseButtons buttons = translate_mouse_buttons(mouse.buttons);
    Qt::MouseButtons old_buttons = translate_mouse_buttons(m_lastClick.buttons);
    Qt::MouseButton current_button = translate_mouse_button(m_lastClick.buttons);

    if (m_systemDrag) {
        if (old_buttons != 0 && buttons == 0) {
	    m_systemDrag = false;
	    return;
	}
    } else {
	// FIXME: Looking at the Qt docs, this is over simplified. We should be able to
	// work out which button was pressed even if another button was already pressed.
	if (old_buttons != 0 && buttons == 0) {
	    QMouseEvent event(QEvent::MouseButtonRelease,
			      localPos,
			      globalPos,
			      current_button,
			      buttons,
			      modifiers);
	    QCoreApplication::sendEvent(rwindow->window(), &event);
	    return;
	}

	if (current_button == Qt::LeftButton && modifiers & Qt::ShiftModifier) {
	    startResizeWindow(mouse);
	    return;
	}
    }

    // If there's been no mouse movement, then don't send an event.
    if (mousePos == m_lastMouseMovePos)
	return;

    m_lastMouseMovePos = mousePos;

    QMouseEvent event(QEvent::MouseMove,
		      localPos, globalPos,
		      Qt::NoButton, buttons,
		      modifiers);
    QCoreApplication::sendEvent(rwindow->window(), &event);
}

void
QRiscosEventDispatcherPrivate::handleWimpUserMessage()
{
    const wimp_message *message = &m_pollBlock.message;

    switch (message->action)
    {
    case message_QUIT:
	{
	    QCoreApplication::instance()->quit();
	    break;
	}
    }
}
#if 0
bool
QRiscosEventDispatcherPrivate::unregisterTimers(QObject *obj)
{
    foreach (RiscosTimerInfo *timer, m_timerDict) {
        if (timer->obj == obj) {
	    m_timerDict.remove(timer->timerId);
	    delete timer;
	}
    }

    return true;
}

QList<QAbstractEventDispatcher::TimerInfo>
QRiscosEventDispatcherPrivate::registeredTimers(QObject *obj) const
{
    QList<QAbstractEventDispatcher::TimerInfo> list;

    foreach (RiscosTimerInfo *timer, m_timerDict) {
	if (timer->obj == obj)
	    list.append(QAbstractEventDispatcher::TimerInfo(timer->timerId,
							    timer->interval,
							    timer->type));
    }

    return list;
}

int
QRiscosEventDispatcherPrivate::remainingTime(int timerId)
{
    // Check if this timer is active.
    if (!m_timerDict.contains(timerId))
	return -1;

    RiscosTimerInfo *timer = m_timerDict.value(timerId);

    uint32_t time_now = os_read_monotonic_time();
    uint32_t remaining = timer->expire_time - time_now;

    return (remaining <= 0) ? 0 : remaining * 10;
}

void
QRiscosEventDispatcher::registerTimer(int timerId,
				      int interval,
				      Qt::TimerType timerType,
				      QObject *obj)
{
#if 0
  {
    char buffer[150];
    
    sprintf (buffer, "[%s:%d:%s] - timerId (%d), interval (%d), obj (%p)\n",
	  __func__, __LINE__, __FILE__,timerId,interval,obj);
    report_text0 (buffer);
  }
#endif
    Q_UNUSED(timerType)

    Q_D(QRiscosEventDispatcher);

    RiscosTimerInfo *t = new RiscosTimerInfo;

    t->obj = obj;
    t->timerId = timerId;
    // Convert from millisecond (1/1000s) to centisecond (1/100s).
    t->interval = interval / 10;
    t->expire_time = os_read_monotonic_time() + t->interval;
    t->type = timerType;
    t->in_timer = false;

    d->registerTimer (t);
}

bool
QRiscosEventDispatcher::unregisterTimers(QObject *obj)
{
    Q_D(QRiscosEventDispatcher);

    return d->unregisterTimers(obj);
}

bool QRiscosEventDispatcher::unregisterTimer(int timerId)
{
#if 0
  {
    char buffer[150];
    
    sprintf (buffer, "[%s:%d:%s] - timerId (%d)\n",
	  __func__, __LINE__, __FILE__,timerId);
    report_text0 (buffer);
  }
#endif
    Q_D(QRiscosEventDispatcher);

    RiscosTimerInfo *t = d->getTimer(timerId);

    if (t == nullptr)
	return false;

    d->unregisterTimer(t);

    delete t;

    return true;
}

int QRiscosEventDispatcher::remainingTime(int timerId)
{
    Q_D(QRiscosEventDispatcher);

    return d->remainingTime(timerId);
}

void QRiscosEventDispatcher::wakeUp()
{
}

void QRiscosEventDispatcher::interrupt()
{
}

QList<QAbstractEventDispatcher::TimerInfo>
QRiscosEventDispatcher::registeredTimers(QObject *object) const
{
    Q_D(const QRiscosEventDispatcher);

    return d->registeredTimers(object);
}
#include "/home/lee/ProgDev/debug-inlines.h"

void
QRiscosEventDispatcherPrivate::handleTimers()
{
    uint32_t time_now = os_read_monotonic_time();

    foreach (RiscosTimerInfo *timer, m_timerDict) {
	if (time_now >= timer->expire_time && !timer->in_timer) {
//report_text0("Fire timer!");
//report_dec(timer->interval);
	    int timer_id = timer->timerId;

	    timer->in_timer = true;

	    // Reset the timer.
	    timer->expire_time = time_now + timer->interval;

	    QTimerEvent timer_event(timer->timerId);
	    QCoreApplication::sendEvent(timer->obj, &timer_event);

	    // Make sure the timer wasn't removed by the handler
	    if (m_timerDict.contains (timer_id))
		timer->in_timer = false;
	}
    }
}
#endif

QT_END_NAMESPACE
