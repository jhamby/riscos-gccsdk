--- src/dynapi/SDL_dynapi.c.orig	2016-01-02 19:56:31.000000000 +0000
+++ src/dynapi/SDL_dynapi.c	2016-06-22 11:07:29.310465071 +0100
@@ -230,7 +230,7 @@
     }
     return retval;
 }
-#elif defined(unix) || defined(__unix__) || defined(__APPLE__)
+#elif defined(unix) || defined(__unix__) || defined(__APPLE__) || defined(__riscos__)
 #include <dlfcn.h>
 static SDL_INLINE void *get_sdlapi_entry(const char *fname, const char *sym)
 {
