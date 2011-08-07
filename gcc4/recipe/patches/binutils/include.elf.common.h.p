--- include/elf/common.h.orig	2011-07-16 19:16:43.000000000 +0100
+++ include/elf/common.h	2011-07-13 20:37:38.000000000 +0100
@@ -723,6 +723,8 @@
 #define DT_LOPROC	0x70000000
 #define DT_HIPROC	0x7fffffff
 
+#define DT_RISCOS_PIC	DT_LOOS
+
 /* The next 2 dynamic tag ranges, integer value range (DT_VALRNGLO to
    DT_VALRNGHI) and virtual address range (DT_ADDRRNGLO to DT_ADDRRNGHI),
    are used on Solaris.  We support them everywhere.  Note these values
