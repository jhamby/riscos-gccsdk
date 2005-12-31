--- Common.tmpl.old	2003-01-31 12:00:20.000000000 +0000
+++ Common.tmpl	2003-01-31 13:55:32.000000000 +0000
@@ -36,14 +36,14 @@
 /* Uncomment and modify if you have the JPEG 6 library */
 
 #define Use_JPEG
-JPEGDIR = /usr
+JPEGDIR = $(GCCSDK_INSTALL_ENV)
 JPEGINCLUDE = -I$(JPEGDIR)/include
 JPEGLIB = -L$(JPEGDIR)/lib -ljpeg$(IMGLEXT)
 
 /* Uncomment and modify if you have the PNG library */
 
 #define Use_PNG
-PNGDIR = /usr
+PNGDIR = $(GCCSDK_INSTALL_ENV)
 PNGINCLUDE = -I$(PNGDIR)/include
 PNGLIB = -L$(PNGDIR)/lib -lpng$(IMGLEXT) -lz$(IMGLEXT)
 
@@ -107,7 +107,7 @@
 #define CConfigSet 1
 #endif
 
-/* FreeBSD, BSDI, NetBSD */
+/* FreeBSD, BSDI, NetBSD, RISC OS */
 #if defined(FreeBSDArchitecture) || defined(BSD386Architecture) || defined(NetBSDArchitecture)
 #define HAVE_SNPRINTF 1
 OS_DEFINES = -DHAVE_STDLIB_H -DHAVE_STRING_H -DHAVE_UNISTD_H -DHAVE_STDDEF_H \
@@ -172,6 +172,16 @@
 #define CConfigSet 1
 #endif
 
+#if defined(RISCOSArchitecture)
+#define HAVE_SNPRINTF 1
+OS_DEFINES = -DHAVE_STDLIB_H -DHAVE_STRING_H -DHAVE_UNISTD_H -DHAVE_STDDEF_H \
+             -DHAVE_UNAME -DHAVE_MKTIME -DHAVE_SIZE_T -DHAVE_SSIZE_T \
+             -DHAVE_STDARG_H -DHAVE_SNPRINTF
+OS_LIBS =
+OS_DEBUG_FLAGS = 
+#define CConfigSet 1
+#endif
+
 /* everything else hopefully */
 #if !defined(CConfigSet)
 OS_DEFINES = -DHAVE_STDLIB_H -DHAVE_STRING_H -DHAVE_UNISTD_H -DHAVE_STDDEF_H \
