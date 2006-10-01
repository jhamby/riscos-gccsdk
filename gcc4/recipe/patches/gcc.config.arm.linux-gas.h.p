Index: gcc/config/arm/linux-gas.h
===================================================================
RCS file: gcc/config/arm/linux-gas.h,v
retrieving revision 1.16
diff -u -r1.16 linux-gas.h
--- gcc/config/arm/linux-gas.h	1 May 2005 15:28:53 -0000	1.16
+++ gcc/config/arm/linux-gas.h	17 May 2006 21:59:32 -0000
@@ -42,6 +42,10 @@
 #undef  WCHAR_TYPE_SIZE
 #define WCHAR_TYPE_SIZE BITS_PER_WORD
 
+/* When compiling libgcc for Thumb, we need some other way of
+   flushing the processor cache.  */
+#ifndef __thumb__
+
 /* Clear the instruction cache from `beg' to `end'.  This makes an
    inline system call to SYS_cacheflush.  */
 #define CLEAR_INSN_CACHE(BEG, END)					\
@@ -53,3 +57,4 @@
 		    : "=r" (_beg)					\
 		    : "0" (_beg), "r" (_end), "r" (_flg));		\
 }
+#endif
