--- src/getsdir.h	1998-04-06 16:53:02.000000000 +0100
+++ src/getsdir.h	2002-12-10 23:12:44.000000000 +0000
@@ -18,6 +18,8 @@
 
 #include <dirent.h>
 
+#define MAXNAMLEN 255 
+
 typedef struct dirEntry {		/* structure of data item */
    char fname[MAXNAMLEN + 1];		/* filename + terminating null */
    time_t time;				/* last modification date */
diff -ur minicom-1.83.1/src/minicom.h minicom-1.83.1-ro/src/minicom.h
--- src/minicom.h	2002-12-18 16:24:43.000000000 +0000
+++ src/minicom.h	2002-12-10 23:15:54.000000000 +0000
@@ -111,7 +111,7 @@
 EXTERN int st_attr;	/* Status Bar attributes. */
 
 /* jl 04.09.97 conversion tables */
-EXTERN unsigned char vt_outmap[256], vt_inmap[256];
+extern unsigned char vt_outmap[256], vt_inmap[256];
 
 /* MARK updated 02/17/95 - history buffer */
 EXTERN int num_hist_lines;  /* History buffer size */
--- src/port.h.old	2003-04-02 16:17:17.000000000 +0100
+++ src/port.h	2003-04-02 15:17:30.000000000 +0100
@@ -109,6 +109,10 @@
 #  define HAS_REUID
 #endif
 
+#ifdef __riscos__
+#  undef HAS_REUID
+#endif
+
 /* Availability of fchown(int, uid_t, gid_t) */
 #if defined (_SYSV) || defined (_BSD43) || defined(_DGUX_SOURCE)
 #  if !defined(_SVR2) && !defined(_SYSV3) && !defined(_COH42)
