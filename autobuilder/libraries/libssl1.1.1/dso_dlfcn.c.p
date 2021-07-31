--- crypto/dso/dso_dlfcn.c.orig	2018-09-12 00:48:21.000000000 +1200
+++ crypto/dso/dso_dlfcn.c	2018-12-01 13:37:26.171152027 +1300
@@ -30,7 +30,7 @@
      defined(__SCO_VERSION__) || defined(_SCO_ELF) || \
      (defined(__osf__) && !defined(RTLD_NEXT))     || \
      (defined(__OpenBSD__) && !defined(RTLD_SELF)) || \
-        defined(__ANDROID__)
+        defined(__ANDROID__) || defined (__riscos__)
 #   undef HAVE_DLINFO
 #  endif
 # endif
