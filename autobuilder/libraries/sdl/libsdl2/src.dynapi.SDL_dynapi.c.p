--- src/dynapi/SDL_dynapi.c.orig	2017-10-28 08:41:47.042244089 +0100
+++ src/dynapi/SDL_dynapi.c	2017-10-28 08:43:16.485410384 +0100
@@ -222,7 +222,7 @@
     return retval;
 }
 
-#elif defined(unix) || defined(__unix__) || defined(__APPLE__) || defined(__HAIKU__) || defined(__QNX__)
+#elif defined(unix) || defined(__unix__) || defined(__APPLE__) || defined(__HAIKU__) || defined(__QNX__) || defined(__riscos__)
 #include <dlfcn.h>
 static SDL_INLINE void *get_sdlapi_entry(const char *fname, const char *sym)
 {
