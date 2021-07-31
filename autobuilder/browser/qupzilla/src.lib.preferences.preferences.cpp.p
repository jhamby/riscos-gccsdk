--- src/lib/preferences/preferences.cpp.orig	2016-02-21 11:51:08.802837469 +0000
+++ src/lib/preferences/preferences.cpp	2016-02-21 11:52:12.622837535 +0000
@@ -48,7 +48,9 @@
 #include "registerqappassociation.h"
 #include "profilemanager.h"
 #include "html5permissions/html5permissionsdialog.h"
+#ifndef __riscos__
 #include "pac/pacmanager.h"
+#endif
 #include "searchenginesdialog.h"
 
 #ifdef USE_HUNSPELL
@@ -798,7 +800,9 @@
 
 void Preferences::reloadPacFileClicked()
 {
+#ifndef __riscos__
     mApp->networkManager()->proxyFactory()->pacManager()->downloadPacFile();
+#endif
 }
 
 void Preferences::showPassManager(bool state)
