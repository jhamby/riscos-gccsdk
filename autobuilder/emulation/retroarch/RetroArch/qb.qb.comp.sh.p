--- qb/qb.comp.sh.orig	2020-03-14 13:19:26.507961488 +0000
+++ qb/qb.comp.sh	2020-03-14 13:22:28.736223315 +0000
@@ -99,6 +99,10 @@
 	PKG_CONF_PATH="${PKGCONF:-none}"
 fi
 
+if [ "$OS" = "riscos" ]; then
+       PKG_CONF_PATH="ro-pkg-config"
+fi
+
 printf %s\\n "Checking for pkg-config ... $PKG_CONF_PATH"
 
 if [ "$PKG_CONF_PATH" = "none" ]; then
