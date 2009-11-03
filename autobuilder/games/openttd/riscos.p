Index: src/intro_gui.cpp
===================================================================
--- src/intro_gui.cpp	(revision 17960)
+++ src/intro_gui.cpp	(working copy)
@@ -288,6 +288,8 @@
 		SetDParam(0, STR_OSNAME_SUNOS);
 #elif defined(DOS)
 		SetDParam(0, STR_OSNAME_DOS);
+#elif defined(__riscos__)
+                SetDParam(0, STR_OSNAME_RISCOS);
 #else
 		SetDParam(0, STR_OSNAME_UNIX);
 #endif
Index: src/ini.cpp
===================================================================
--- src/ini.cpp	(revision 17960)
+++ src/ini.cpp	(working copy)
@@ -17,10 +17,12 @@
 #include "string_func.h"
 #include "fileio_func.h"
 
+#if !defined(__riscos__)
 #if (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 199309L) || (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 500)
 # define WITH_FDATASYNC
 # include <unistd.h>
 #endif
+#endif
 
 #ifdef WIN32
 # include <shellapi.h>
Index: src/network/core/os_abstraction.h
===================================================================
--- src/network/core/os_abstraction.h	(revision 17960)
+++ src/network/core/os_abstraction.h	(working copy)
@@ -156,7 +156,7 @@
 #		include <net/if.h>
 /* According to glibc/NEWS, <ifaddrs.h> appeared in glibc-2.3. */
 #		if !defined(__sgi__) && !defined(SUNOS) && !defined(__MORPHOS__) && !defined(__BEOS__) && !defined(__HAIKU__) && !defined(__INNOTEK_LIBC__) \
-		   && !(defined(__GLIBC__) && (__GLIBC__ <= 2) && (__GLIBC_MINOR__ <= 2)) && !defined(__dietlibc__) && !defined(HPUX)
+		   && !(defined(__GLIBC__) && (__GLIBC__ <= 2) && (__GLIBC_MINOR__ <= 2)) && !defined(__dietlibc__) && !defined(HPUX) && !defined(__riscos)
 /* If for any reason ifaddrs.h does not exist on your system, comment out
  *   the following two lines and an alternative way will be used to fetch
  *   the list of IPs from the system. */
@@ -189,6 +189,11 @@
 	#define IPV6_V6ONLY 27
 #endif
 
+#ifdef __riscos__
+        #define AI_ADDRCONFIG   0x00000400  // Resolution only if global address configured
+        #define IPV6_V6ONLY 27
+#endif
+
 #if defined(PSP)
 #	include <sys/socket.h>
 #	include <netinet/in.h>
Index: src/screenshot.cpp
===================================================================
--- src/screenshot.cpp	(revision 17960)
+++ src/screenshot.cpp	(working copy)
@@ -78,7 +78,9 @@
 struct RgbTriplet {
 	byte b, g, r;
 };
+#ifndef __riscos__
 assert_compile(sizeof(RgbTriplet) == 3);
+#endif
 
 /**
  * Generic .BMP writer
Index: src/stdafx.h
===================================================================
--- src/stdafx.h	(revision 17960)
+++ src/stdafx.h	(working copy)
@@ -69,6 +69,10 @@
 	#define strcasecmp stricmp
 #endif
 
+#if defined(__riscos__)
+	#define strcasestr strstr
+#endif
+
 #if defined(PSP)
 	#include <psptypes.h>
 	#include <pspdebug.h>
