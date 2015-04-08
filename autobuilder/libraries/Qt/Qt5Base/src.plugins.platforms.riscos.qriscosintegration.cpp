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

#include "qriscosintegration.h"
#include "qriscosbackingstore.h"
#include "qriscoseventdispatcher.h"
#include "qriscoswindow.h"

#include <QtPlatformSupport/private/qfontconfigdatabase_p.h>
#include <QtGui/private/qguiapplication_p.h>
#include "qcoreapplication.h"
#include "qfileinfo.h"

#include "oslib/os.h"
#include "oslib/wimp.h"

QT_BEGIN_NAMESPACE

QRiscosScreen::QRiscosScreen () :
	mTranslationTable(nullptr)
{
    update();
}

void
QRiscosScreen::update()
{
    os_VDU_VAR_LIST(6) var_list = { { os_VDUVAR_XEIG_FACTOR,
				      os_VDUVAR_YEIG_FACTOR,
				      os_VDUVAR_XWIND_LIMIT,
				      os_VDUVAR_YWIND_LIMIT,
				      os_VDUVAR_LOG2_BPP,
				      os_VDUVAR_END_LIST } };

    xos_read_vdu_variables ((os_vdu_var_list *)&var_list, (int *)&var_list);

    mXEigenFactor = var_list.var[0];
    mYEigenFactor = var_list.var[1];
    mGeometry = QRect(0, 0, var_list.var[2] + 1, var_list.var[3] + 1);
    mDepth = 1 << var_list.var[4];

    // These need to match up with what QSprite::typeFromQImageFormat returns.
    switch (mDepth)
    {
    case 32:
      mFormat = QImage::Format_RGB32;
      break;
    case 16:
      mFormat = QImage::Format_RGB16;
      break;
    case 8:
      mFormat = QImage::Format_Indexed8;
      break;
    default:
      mFormat = QImage::Format_Invalid;
      break;
    }

    generateTranslationTable();
}

void
QRiscosScreen::generateTranslationTable()
{
    if (mDepth == 32) {
	// Don't need a translation table going from 32bit to 32bit.
	if (mTranslationTable) {
	    free (mTranslationTable);
	    mTranslationTable = nullptr;
	}
	return;
    }

    int size;
    os_error *err;
    err = xcolourtrans_generate_table (QSprite::typeFromQImageFormat(QImage::Format_RGB32),
				       /* source_palette=*/nullptr,	// Default palette for mode
				       QSprite::typeFromQImageFormat(mFormat),
				       /* dest_palette=*/nullptr,	// Default palette for mode
				       /* trans_tab=*/nullptr, 		// NULL means return required size
				       /* flags=*/0,
				       /* workspace=*/0,
				       /* transfer_fn=*/nullptr,
				       &size);
    if (err) {
	qWarning("Failed to read translation table size because %s\n", err->errmess);
	return;
    }

    mTranslationTable = (osspriteop_trans_tab *)malloc (size);
    if (mTranslationTable == nullptr) {
	qWarning("Failed to allocate memory for translation table\n");
	return;
    }

    err = xcolourtrans_generate_table (QSprite::typeFromQImageFormat(QImage::Format_RGB32),
				       /* source_palette=*/nullptr,
				       QSprite::typeFromQImageFormat(mFormat),
				       /* dest_palette=*/nullptr,
				       mTranslationTable,
				       /* flags=*/0,
				       /* workspace=*/0,
				       /* transfer_fn=*/nullptr,
				       nullptr);
    if (!err)
	return;

    qWarning("Failed to create translation table because %s\n", err->errmess);
    free(mTranslationTable);
    mTranslationTable = nullptr;
}

void
QRiscosScreen::addSurface (QRiscosBackingStore *surface)
{
    // Make sure the window exists.
    surface->window()->create();

    m_surfaceHash.insert(surface->window()->winId(), surface);
}

void
QRiscosScreen::removeSurface (QRiscosBackingStore *surface)
{
    m_surfaceHash.remove(surface->window()->winId());
}

QRiscosWindow *
QRiscosScreen::windowFromId (WId handle) const
{
    QRiscosBackingStore *surface = surfaceFromId(handle);
    
    if (surface) {
	return static_cast<QRiscosWindow *>(surface->window()->handle());
    }

    return nullptr;
}

QRiscosBackingStore *
QRiscosScreen::surfaceFromId (WId handle) const
{
    if (m_surfaceHash.contains(handle))
	return m_surfaceHash.value(handle);

    return nullptr;
}

QRiscosIntegration::QRiscosIntegration ()
    : m_fontDb(new QFontconfigDatabase())
{
    QRiscosScreen *mPrimaryScreen = new QRiscosScreen ();

    screenAdded (mPrimaryScreen);

    QString app_name = QFileInfo(QCoreApplication::applicationFilePath()).fileName();
    QByteArray appNameByteArray = app_name.toUtf8();
    
    mWindowSprites = new QDynamicArea (appNameByteArray.constData());

    wimpInit ();
}

QRiscosIntegration::~QRiscosIntegration ()
{
    delete mWindowSprites;
}

QPlatformWindow *
QRiscosIntegration::createPlatformWindow(QWindow *window) const
{
    QPlatformWindow *w = new QRiscosWindow(window);
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
  
    return xwimp_create_icon (&bar, &mIconbarIconHandle);
}

void
QRiscosIntegration::wimpInit ()
{
    wimp_MESSAGE_LIST(2) message_list = { { 0 } };

    os_error *err;
    QString app_name = QFileInfo(QCoreApplication::applicationFilePath()).fileName();
    QByteArray appNameByteArray = app_name.toUtf8();
    
    err = xwimp_initialise (wimp_VERSION_RO38,
			    appNameByteArray.constData(),
			    (wimp_message_list *)&message_list,
			    NULL,
			    &mTaskHandle);
    if (err != NULL)
	qWarning("Failed to initialise task; %s\n", err->errmess);
}

QT_END_NAMESPACE
