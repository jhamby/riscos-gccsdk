--- include/elf/common.h.orig	2013-11-04 16:33:39.000000000 +0100
+++ include/elf/common.h	2013-12-31 13:16:36.482126367 +0100
@@ -754,6 +754,9 @@
 #define DT_LOPROC	0x70000000
 #define DT_HIPROC	0x7fffffff
 
+#define DT_RISCOS_PIC	DT_LOOS
+#define DT_RISCOS_ABI_VERSION DT_LOOS+1
+
 /* The next 2 dynamic tag ranges, integer value range (DT_VALRNGLO to
    DT_VALRNGHI) and virtual address range (DT_ADDRRNGLO to DT_ADDRRNGHI),
    are used on Solaris.  We support them everywhere.  Note these values
