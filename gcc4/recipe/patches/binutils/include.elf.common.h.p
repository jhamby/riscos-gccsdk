--- include/elf/common.h.orig	2012-03-09 18:06:14.000000000 +0000
+++ include/elf/common.h	2012-03-09 18:05:40.000000000 +0000
@@ -728,6 +728,9 @@
 #define DT_LOPROC	0x70000000
 #define DT_HIPROC	0x7fffffff
 
+#define DT_RISCOS_PIC	DT_LOOS
+#define DT_RISCOS_GCC_DIR DT_LOOS+1
+
 /* The next 2 dynamic tag ranges, integer value range (DT_VALRNGLO to
    DT_VALRNGHI) and virtual address range (DT_ADDRRNGLO to DT_ADDRRNGHI),
    are used on Solaris.  We support them everywhere.  Note these values
