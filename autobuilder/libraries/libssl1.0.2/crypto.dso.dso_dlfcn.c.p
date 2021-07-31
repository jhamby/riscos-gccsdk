--- crypto/dso/dso_dlfcn.c.orig	2015-05-16 13:31:08.149947055 +0100
+++ crypto/dso/dso_dlfcn.c	2015-05-16 13:32:02.069946470 +0100
@@ -87,7 +87,7 @@
      defined(__SCO_VERSION__) || defined(_SCO_ELF) || \
      (defined(__osf__) && !defined(RTLD_NEXT))     || \
      (defined(__OpenBSD__) && !defined(RTLD_SELF)) || \
-        defined(__ANDROID__)
+        defined(__ANDROID__) || defined(__riscos__)
 #   undef HAVE_DLINFO
 #  endif
 # endif
