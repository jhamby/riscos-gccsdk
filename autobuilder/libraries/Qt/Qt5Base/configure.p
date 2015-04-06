--- configure.orig	2014-05-15 18:12:04.000000000 +0100
+++ configure	2014-05-27 20:48:20.626564105 +0100
@@ -644,6 +644,7 @@
 # Set when the -arch or -host-arch arguments are used
 OPT_OBSOLETE_HOST_ARG=no
 
+CFG_RISCOS=no
 CFG_USE_GNUMAKE=no
 CFG_XINPUT2=auto
 CFG_XINPUT=runtime
@@ -1719,6 +1720,13 @@
             UNKNOWN_OPT=yes
         fi
         ;;
+    riscos)
+        if [ "$VAL" = "yes" ] || [ "$VAL" = "no" ]; then
+            CFG_RISCOS="$VAL"
+        else
+            UNKNOWN_OPT=yes
+        fi
+        ;;
     linuxfb)
         if [ "$VAL" = "yes" ] || [ "$VAL" = "no" ]; then
             CFG_LINUXFB="$VAL"
@@ -4808,6 +4816,10 @@
     if [ -n "$PKG_CONFIG" ] && $PKG_CONFIG --exists fontconfig --exists freetype2 2>/dev/null; then
         QT_CFLAGS_FONTCONFIG=`$PKG_CONFIG --cflags fontconfig --cflags freetype2 2>/dev/null`
         QT_LIBS_FONTCONFIG=`$PKG_CONFIG --libs fontconfig --libs freetype2 2>/dev/null`
+        if $PKG_CONFIG –exists freetype2 2>/dev/null; then
+            QT_CFLAGS_FONTCONFIG=”$QT_CFLAGS_FONTCONFIG `$PKG_CONFIG –cflags freetype2 2>/dev/null`”
+            QT_LIBS_FONTCONFIG=”$QT_LIBS_FONTCONFIG `$PKG_CONFIG –libs freetype2 2>/dev/null`”
+        fi
     else
         QT_CFLAGS_FONTCONFIG=
         QT_LIBS_FONTCONFIG="-lfreetype -lfontconfig"
@@ -4822,6 +4834,13 @@
         CFG_FONTCONFIG=no
     fi
 
+    QT_CFLAGS_FONTCONFIG="-I$GCCSDK_INSTALL_ENV/include/freetype2"
+    QT_LIBS_FONTCONFIG="-lfreetype -lfontconfig"
+    QT_CONFIG="$QT_CONFIG fontconfig"
+    QMakeVar set QMAKE_CFLAGS_FONTCONFIG "$QT_CFLAGS_FONTCONFIG"
+    QMakeVar set QMAKE_LIBS_FONTCONFIG "$QT_LIBS_FONTCONFIG"
+    CFG_FONTCONFIG=yes
+    CFG_LIBFREETYPE=system
 fi
 
 # Save these for a check later
@@ -5197,7 +5216,7 @@
 fi
 
 if [ "$XPLATFORM_MAC" = "no" ] && [ "$XPLATFORM_MINGW" = "no" ] && [ "$XPLATFORM_QNX" = "no" ] && [ "$XPLATFORM_ANDROID" = "no" ]; then
-    if [ "$CFG_XCB" = "no" ] && [ "$CFG_EGLFS" = "no" ] && [ "$CFG_DIRECTFB" = "no" ] && [ "$CFG_LINUXFB" = "no" ] && [ "$CFG_KMS" = "no" ]; then
+    if [ "$CFG_XCB" = "no" ] && [ "$CFG_EGLFS" = "no" ] && [ "$CFG_DIRECTFB" = "no" ] && [ "$CFG_LINUXFB" = "no" ] && [ "$CFG_KMS" = "no" ] && [ "$CFG_RISCOS" = "no" ]; then
         if [ "$QPA_PLATFORM_GUARD" = "yes" ] &&
             ( [ "$ORIG_CFG_XCB" = "auto" ] || [ "$ORIG_CFG_EGLFS" = "auto" ] || [ "$ORIG_CFG_DIRECTFB" = "auto" ] || [ "$ORIG_CFG_LINUXFB" = "auto" ] || [ "$ORIG_CFG_KMS" = "auto" ] ); then
         echo "No QPA platform plugin enabled!"
@@ -6495,6 +6514,7 @@
 report_support "    KMS .................." "$CFG_KMS"
 report_support "    LinuxFB .............." "$CFG_LINUXFB"
 report_support "    XCB .................." "$CFG_XCB" system "system library" qt "bundled copy"
+report_support "    RISC OS .............." "$CFG_RISCOS"
 if [ "$CFG_XCB" != "no" ]; then
     report_support "      EGL on X ..........." "$CFG_EGL_X"
     report_support "      GLX ................" "$CFG_XCB_GLX"
