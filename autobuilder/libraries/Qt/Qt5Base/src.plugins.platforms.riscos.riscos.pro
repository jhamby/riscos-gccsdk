TARGET = qriscos

PLUGIN_TYPE = platforms
PLUGIN_CLASS_NAME = QRiscosIntegrationPlugin
load(qt_plugin)

QT += core-private gui-private platformsupport-private

INCLUDEPATH += $(GCCSDK_INSTALL_ENV)/include

SOURCES += main.cpp \
           qriscosintegration.cpp \
           qriscosbackingstore.cpp \
           qriscoseventdispatcher.cpp \
           qriscoswindow.cpp \
           qriscosscreen.cpp
HEADERS += qriscosintegration.h \
           qriscosbackingstore.h \
           qriscoseventdispatcher.h \
           qriscoswindow.h \
           qriscosscreen.h

OTHER_FILES += riscos.json
