--- src/gengine/OptionAgent.cpp.orig	2010-08-29 20:31:11.000000000 +0100
+++ src/gengine/OptionAgent.cpp	2010-08-29 20:31:58.000000000 +0100
@@ -34,7 +34,7 @@
 
 //NOTE: SYSTEM_DATA_DIR is set to "$(datadir)/games/@PACKAGE@"
-#ifndef SYSTEM_DATA_DIR
+#ifndef SYSTEM_DATA_DIR || defined(__riscos__)
-#define SYSTEM_DATA_DIR ""
+#define SYSTEM_DATA_DIR "/<Fillets-ng$Dir>/data"
 #endif
 
 //NOTE: userdir = $HOME + USER_DATA_DIR
