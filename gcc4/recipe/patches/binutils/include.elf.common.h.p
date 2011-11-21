--- include/elf/common.h.orig	2011-07-22 22:22:36.000000000 +0200
+++ include/elf/common.h	2011-11-21 19:42:18.682236863 +0100
@@ -728,6 +728,8 @@
 #define DT_LOPROC	0x70000000
 #define DT_HIPROC	0x7fffffff
 
+#define DT_RISCOS_PIC	DT_LOOS
+
 /* The next 2 dynamic tag ranges, integer value range (DT_VALRNGLO to
    DT_VALRNGHI) and virtual address range (DT_ADDRRNGLO to DT_ADDRRNGHI),
    are used on Solaris.  We support them everywhere.  Note these values
