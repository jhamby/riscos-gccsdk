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
--- src/network/core/os_abstraction.h.orig	2009-03-14 17:32:18.000000000 -0700
+++ src/network/core/os_abstraction.h	2009-11-03 19:17:36.000000000 -0800
@@ -62,7 +62,7 @@
 #		include <net/if.h>
 /* According to glibc/NEWS, <ifaddrs.h> appeared in glibc-2.3. */
 #		if !defined(__sgi__) && !defined(SUNOS) && !defined(__MORPHOS__) && !defined(__BEOS__) && !defined(__INNOTEK_LIBC__) \
-		   && !(defined(__GLIBC__) && (__GLIBC__ <= 2) && (__GLIBC_MINOR__ <= 2)) && !defined(__dietlibc__) && !defined(HPUX)
+		   && !(defined(__GLIBC__) && (__GLIBC__ <= 2) && (__GLIBC_MINOR__ <= 2)) && !defined(__dietlibc__) && !defined(HPUX) && !defined(__riscos__)
 /* If for any reason ifaddrs.h does not exist on your system, comment out
  *   the following two lines and an alternative way will be used to fetch
  *   the list of IPs from the system. */
@@ -91,6 +91,11 @@
 	typedef int socklen_t;
 #endif
 
+#ifdef __riscos__
+        #define AI_ADDRCONFIG   0x00000400  // Resolution only if global address configured
+        #define IPV6_V6ONLY 27
+#endif
+
 #if defined(PSP)
 #	include <sys/socket.h>
 #	include <netinet/in.h>
--- src/stdafx.h.orig	2009-05-10 14:33:55.000000000 -0700
+++ src/stdafx.h	2009-11-03 19:41:09.000000000 -0800
@@ -66,6 +66,11 @@
 	#define strcasecmp stricmp
 #endif
 
+#if defined(__riscos__)
+	#define strcasestr strstr
+	#define _stricmp strcasecmp
+#endif
+
 #if defined(PSP)
 	#include <psptypes.h>
 	#include <pspdebug.h>
@@ -372,7 +377,7 @@
 	#define _stricmp strcasecmp
 #endif
 
-#if !defined(MORPHOS) && !defined(OPENBSD) && !defined(__NDS__) && !defined(__DJGPP__)
+#if !defined(MORPHOS) && !defined(OPENBSD) && !defined(__NDS__) && !defined(__DJGPP__) && !defined(__riscos__)
 	/* NDS, MorphOS & OpenBSD don't know wchars, the rest does :( */
 	#define HAS_WCHAR
 #endif /* !defined(MORPHOS) && !defined(OPENBSD) && !defined(__NDS__) */
