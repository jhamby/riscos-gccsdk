--- src/modeltoolbar.cpp.orig	2015-04-10 20:47:29.380351907 +0100
+++ src/modeltoolbar.cpp	2015-04-11 14:03:09.669979319 +0100
@@ -23,6 +23,8 @@
 #include "modelmenu.h"
 
 #include <qevent.h>
+#include <qdrag.h>
+#include <qmimedata.h>
 #include <qtoolbutton.h>
 
 ModelToolBar::ModelToolBar(QWidget *parent)
