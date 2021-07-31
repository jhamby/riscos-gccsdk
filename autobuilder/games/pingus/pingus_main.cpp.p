--- ./src/pingus/pingus_main.cpp.orig	2011-09-08 14:01:33.805232400 +0100
+++ ./src/pingus/pingus_main.cpp	2011-09-08 14:24:14.114247900 +0100
@@ -66,6 +66,10 @@
 #pragma warning( disable : 4996 ) 
 #endif
 
+#ifdef __riscos__
+#include "config.h"
+#endif
+
 extern tinygettext::DictionaryManager dictionary_manager;
 
 void
