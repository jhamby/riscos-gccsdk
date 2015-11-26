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

#include <QtPlatformSupport/private/qfontconfigdatabase_p.h>
#include <QtGui/private/qguiapplication_p.h>
#include "qcoreapplication.h"
#include "qfileinfo.h"

#include "qriscosintegration.h"
#include "qriscosbackingstore.h"
#include "qriscoseventdispatcher.h"
#include "qriscoswindow.h"

#include "oslib/os.h"
#include "oslib/wimp.h"
#include "swis.h"

QT_BEGIN_NAMESPACE

QRiscosIntegration::QRiscosIntegration ()
    : m_fontDb(new QFontconfigDatabase())
{
    QRiscosScreen *mPrimaryScreen = new QRiscosScreen ();

    m_riscosPlatformNativeInterface = new QRiscosPlatformNativeInterface();

    screenAdded (mPrimaryScreen);

    QString app_name = QFileInfo(QCoreApplication::applicationFilePath()).fileName();
    QByteArray appNameByteArray = app_name.toUtf8();
    
    mWindowSprites = new QDynamicArea (appNameByteArray.constData(), 200 * 1024 * 1024);
    mWindowSprites->create();

    wimpInit ();
}

QRiscosIntegration::~QRiscosIntegration ()
{
    delete mWindowSprites;
    delete m_riscosPlatformNativeInterface;
}

QPlatformNativeInterface *
QRiscosIntegration::nativeInterface() const
{
    return m_riscosPlatformNativeInterface;
}

QPlatformWindow *
QRiscosIntegration::createWindow(QWindow *window) const
{
    return new QRiscosWindow(window);
}

QPlatformWindow *
QRiscosIntegration::createPlatformWindow(QWindow *window) const
{
    QPlatformWindow *w = createWindow(window);
    if (window->type() != Qt::ToolTip)
	w->requestActivateWindow();
    return w;
}

QPlatformBackingStore *
QRiscosIntegration::createPlatformBackingStore(QWindow *window) const
{
    return new QRiscosBackingStore(window);
}

QAbstractEventDispatcher *
QRiscosIntegration::createEventDispatcher() const
{
    return new QRiscosEventDispatcher();
}

QPlatformFontDatabase *QRiscosIntegration::fontDatabase() const
{
    return m_fontDb;
}

os_error *
QRiscosIntegration::createIconbarIcon(const char *task_name)
{
    wimp_icon_create bar;

    bar.w = wimp_ICON_BAR_RIGHT;
    bar.icon.extent.x0 = 0;
    bar.icon.extent.y0 = 0;
    bar.icon.extent.x1 = 68;
    bar.icon.extent.y1 = 68;
    bar.icon.flags = wimp_ICON_SPRITE |
		     wimp_ICON_HCENTRED |
		     wimp_ICON_VCENTRED |
		     wimp_ICON_SPRITE |
		     wimp_ICON_HCENTRED |
		     wimp_ICON_VCENTRED |
		     (wimp_BUTTON_CLICK << wimp_ICON_BUTTON_TYPE_SHIFT);
    if (*task_name != '!') {
      *bar.icon.data.sprite = '!';
      memcpy(bar.icon.data.sprite + 1, task_name, 11);
    } else
      memcpy(bar.icon.data.sprite, task_name, 12);
  
#if (defined(__VFP_FP__) && !defined(__SOFTFP__))
    return (os_error *)_swix(Wimp_CreateIcon, _IN(1)|_OUT(0),
			     &bar, &mIconbarIconHandle);
#else
    return xwimp_create_icon (&bar, &mIconbarIconHandle);
#endif
}

void
QRiscosIntegration::wimpInit ()
{
    wimp_MESSAGE_LIST(2) message_list = { { 0 } };

    QString app_name = QFileInfo(QCoreApplication::applicationFilePath()).fileName();
    QByteArray appNameByteArray = app_name.toUtf8();
    
#if (defined(__VFP_FP__) && !defined(__SOFTFP__))
    _kernel_oserror *err;
    err = _swix(Wimp_Initialise, _INR(0,3)|_OUT(1),
		wimp_VERSION_RO38, 0x4B534154,
		appNameByteArray.constData(), &message_list,
		&mTaskHandle);
#else
    os_error *err;
    err = xwimp_initialise (wimp_VERSION_RO38,
			    appNameByteArray.constData(),
			    (wimp_message_list *)&message_list,
			    NULL,
			    &mTaskHandle);
#endif
    if (err != NULL)
	qWarning("Failed to initialise task; %s\n", err->errmess);
}

QT_END_NAMESPACE
