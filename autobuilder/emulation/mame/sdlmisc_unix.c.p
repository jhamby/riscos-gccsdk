--- src/osd/sdl/sdlmisc_unix.c.orig	2012-04-09 14:59:21.000000000 +0100
+++ src/osd/sdl/sdlmisc_unix.c	2012-04-09 15:00:06.000000000 +0100
@@ -43,7 +43,7 @@
 
 void osd_free_executable(void *ptr, size_t size)
 {
-#ifdef SDLMAME_SOLARIS
+#if defined(SDLMAME_SOLARIS) || defined(__riscos__)
 	munmap((char *)ptr, size);
 #else
 	munmap(ptr, size);
