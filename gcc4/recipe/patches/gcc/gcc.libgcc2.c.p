--- gcc/libgcc2.c.orig	2007-03-24 16:55:45.000000000 +0100
+++ gcc/libgcc2.c	2007-03-19 22:09:02.000000000 +0100
@@ -1902,6 +1902,11 @@
 #define HAS_INIT_SECTION
 #endif
 
+/* FIXME: (JOTY) module code has problems with __do_global_{d,c}tors() and
+   __gccmain() and I haven't identified what precisely the problem
+   is.  */
+#ifndef __TARGET_MODULE__
+
 #if !defined (HAS_INIT_SECTION) || !defined (OBJECT_FORMAT_ELF)
 
 /* Some ELF crosses use crtstuff.c to provide __CTOR_LIST__, but use this
@@ -1979,6 +1984,13 @@
     }
 }
 #endif /* no HAS_INIT_SECTION or INVOKE__main */
+#else /* __TARGET_MODULE__ */
+extern void SYMBOL__MAIN (void);
+void
+SYMBOL__MAIN (void)
+{
+}
+#endif
 
 #endif /* L__main */
 #endif /* __CYGWIN__ */
