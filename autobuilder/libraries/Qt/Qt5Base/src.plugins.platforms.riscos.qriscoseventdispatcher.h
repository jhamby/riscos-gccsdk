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

#ifndef QEVENTDISPATCHER_RISCOS_H
#define QEVENTDISPATCHER_RISCOS_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/qabstracteventdispatcher.h>
//#include <QtCore/private/qabstracteventdispatcher_p.h>
#include <private/qeventdispatcher_unix_p.h>
#include <QPoint>
#include "oslib/wimp.h"
#include "swis.h"

#define USE_QEventDispatcherUNIX_BASE

QT_BEGIN_NAMESPACE

class QWindow;

#ifndef USE_QEventDispatcherUNIX_BASE
struct RiscosTimerInfo
{
    QObject *obj;
    uint32_t expire_time;
    int timerId;
    int interval;
    Qt::TimerType type;
    bool in_timer;
};
#endif

class QRiscosEventDispatcherPrivate;
class QRiscosEventDispatcher :
#ifndef USE_QEventDispatcherUNIX_BASE
public QAbstractEventDispatcher
#else
public QEventDispatcherUNIX
#endif
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QRiscosEventDispatcher)

public:
    QRiscosEventDispatcher(QRiscosEventDispatcherPrivate &priv, QObject *parent = 0);
    explicit QRiscosEventDispatcher(QObject *parent = 0);
    ~QRiscosEventDispatcher();

    bool processEvents(QEventLoop::ProcessEventsFlags flags);
    bool hasPendingEvents();

#ifndef USE_QEventDispatcherUNIX_BASE
    // These are required if QAbstractEventDispatcher is the base class. If QEventDispatcherUNIX
    // is the base class then we inherit them.
    void registerSocketNotifier(QSocketNotifier *notifier);
    void unregisterSocketNotifier(QSocketNotifier *notifier);

    void registerTimer(int timerId, int interval, Qt::TimerType timerType, QObject *object);
    bool unregisterTimer(int timerId);
    bool unregisterTimers(QObject *object);
    QList<TimerInfo> registeredTimers(QObject *object) const;

    int remainingTime(int timerId);

    void wakeUp();
    void interrupt();
#endif
    void flush();
};

class QRiscosEventDispatcherPrivate :
#ifndef USE_QEventDispatcherUNIX_BASE
public QAbstractEventDispatcherPrivate
#else
public QEventDispatcherUNIXPrivate
#endif
{
    Q_DECLARE_PUBLIC(QRiscosEventDispatcher)

public:
    QRiscosEventDispatcherPrivate();

    void poll() {
#if (defined(__VFP_FP__) && !defined(__SOFTFP__))
	_swix(Wimp_Poll, _INR(0,1)|_OUT(0), 0, &m_pollBlock, &m_eventType);
#else
	xwimp_poll (0, &m_pollBlock, NULL, &m_eventType);
#endif
    }

    wimp_event_no getNextEvent () const {
	return m_eventType;
    }

#ifndef USE_QEventDispatcherUNIX_BASE
    // These are required if QAbstractEventDispatcherPrivate is the base class.
    // If QEventDispatcherUNIXPrivate is the base class then we inherit them.
    void registerTimer(RiscosTimerInfo *t) {
	m_timerDict.insert(t->timerId, t);
    }

    void unregisterTimer(RiscosTimerInfo *t) {
	m_timerDict.remove(t->timerId);
    }

    RiscosTimerInfo *getTimer(int timerId) {
	if (m_timerDict.contains(timerId))
	    return m_timerDict.value(timerId);
	else
	    return nullptr;
    }

    bool unregisterTimers(QObject *);

    QList<QAbstractEventDispatcher::TimerInfo> registeredTimers(QObject *object) const;

    int remainingTime(int timerId);
#endif
private:
#ifndef USE_QEventDispatcherUNIX_BASE
    void handleTimers();
#endif

    void handleNullEvent();
    void handleOpenEvent();
    void handleCloseEvent();
    void handleWimpUserMessage();
    void handleRedrawEvent();
    void handleMouseEvent();
    void handleKeyPressEvent();
    void handleModeChange();
    void startResizeWindow(wimp_pointer &mouse);
    void foreignMouseEvent(QWindow *, wimp_pointer &);

private:
    wimp_event_no m_eventType;
    wimp_block m_pollBlock;

    wimp_pointer m_lastClick;

    bool m_systemDrag;

    QPoint m_lastMouseMovePos;

#ifndef USE_QEventDispatcherUNIX_BASE
    QHash<int, RiscosTimerInfo *> m_timerDict;
#endif
};

QT_END_NAMESPACE

#endif
