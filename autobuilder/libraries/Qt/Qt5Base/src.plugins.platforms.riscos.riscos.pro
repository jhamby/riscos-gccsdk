TARGET = qriscos

PLUGIN_TYPE = platforms
PLUGIN_CLASS_NAME = QRiscosIntegrationPlugin
load(qt_plugin)

QT += core-private gui-private platformsupport-private

SOURCES += main.cpp \
           qriscosintegration.cpp \
           qriscosbackingstore.cpp \
           qriscoseventdispatcher.cpp \
           qriscoswindow.cpp \
           qriscosmemory.cpp
HEADERS += qriscosintegration.h \
           qriscosbackingstore.h \
           qriscoseventdispatcher.h \
           qriscoswindow.h \
           qriscos.h

OTHER_FILES += riscos.json
