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

#include "qriscosscreen.h"
#include "qriscosintegration.h"
#include "qriscosbackingstore.h"
#include "qriscoseventdispatcher.h"
#include "qriscoswindow.h"

#include "oslib/os.h"
#include "oslib/wimp.h"
#include "swis.h"

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

#if (defined(__VFP_FP__) && !defined(__SOFTFP__))
    _swix(OS_ReadVduVariables, _INR(0,1), &var_list, &var_list);
#else
    xos_read_vdu_variables ((os_vdu_var_list *)&var_list, (int *)&var_list);
#endif

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
#if (defined(__VFP_FP__) && !defined(__SOFTFP__))
    _kernel_oserror *err;
    err = _swix(ColourTrans_GenerateTable, _INR(0,7)|_OUT(4), 
		QSprite::typeFromQImageFormat(QImage::Format_RGB32),
		/* source_palette=*/nullptr,	// Default palette for mode
		QSprite::typeFromQImageFormat(mFormat),
		/* dest_palette=*/nullptr,	// Default palette for mode
		/* trans_tab=*/nullptr, 	// NULL means return required size
		/* flags=*/0,
		/* workspace=*/0,
		/* transfer_fn=*/nullptr,
		&size);
#else
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
#endif
    if (err) {
	qWarning("Failed to read translation table size because %s\n", err->errmess);
	return;
    }

    mTranslationTable = (osspriteop_trans_tab *)malloc (size);
    if (mTranslationTable == nullptr) {
	qWarning("Failed to allocate memory for translation table\n");
	return;
    }

#if (defined(__VFP_FP__) && !defined(__SOFTFP__))
    err = _swix(ColourTrans_GenerateTable, _INR(0,7), 
		QSprite::typeFromQImageFormat(QImage::Format_RGB32),
		/* source_palette=*/nullptr,	// Default palette for mode
		QSprite::typeFromQImageFormat(mFormat),
		/* dest_palette=*/nullptr,	// Default palette for mode
		mTranslationTable,
		/* flags=*/0,
		/* workspace=*/0,
		/* transfer_fn=*/nullptr);
#else
    err = xcolourtrans_generate_table (QSprite::typeFromQImageFormat(QImage::Format_RGB32),
				       /* source_palette=*/nullptr,
				       QSprite::typeFromQImageFormat(mFormat),
				       /* dest_palette=*/nullptr,
				       mTranslationTable,
				       /* flags=*/0,
				       /* workspace=*/0,
				       /* transfer_fn=*/nullptr,
				       nullptr);
#endif
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

QT_END_NAMESPACE
