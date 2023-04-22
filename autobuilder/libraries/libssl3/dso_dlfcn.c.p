--- crypto/dso/dso_dlfcn.c.orig	2022-12-17 09:03:07.077418340 +0000
+++ crypto/dso/dso_dlfcn.c	2022-12-17 09:08:02.351193688 +0000
@@ -30,7 +30,7 @@
 #  if defined(__SCO_VERSION__) || defined(_SCO_ELF) || \
      (defined(__osf__) && !defined(RTLD_NEXT))     || \
      (defined(__OpenBSD__) && !defined(RTLD_SELF)) || \
-     defined(__ANDROID__) || defined(__TANDEM)
+     defined(__ANDROID__) || defined(__TANDEM) || defined(__riscos__)
 #   undef HAVE_DLINFO
 #  endif
 # endif
