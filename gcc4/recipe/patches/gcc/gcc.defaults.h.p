Index: gcc/defaults.h
===================================================================
--- gcc/defaults.h	(revision 181309)
+++ gcc/defaults.h	(working copy)
@@ -402,7 +402,7 @@
 #endif
 
 /* This decision to use a .jcr section can be overridden by defining
-   USE_JCR_SECTION to 0 in target file.  This is necessary if target
+   TARGET_USE_JCR_SECTION to 0 in target file.  This is necessary if target
    can define JCR_SECTION_NAME but does not have crtstuff or
    linker support for .jcr section.  */
 #ifndef TARGET_USE_JCR_SECTION
@@ -411,7 +411,11 @@
 #else
 #define TARGET_USE_JCR_SECTION 0
 #endif
+#else
+#if !TARGET_USE_JCR_SECTION
+#undef JCR_SECTION_NAME
 #endif
+#endif
 
 /* Number of hardware registers that go into the DWARF-2 unwind info.
    If not defined, equals FIRST_PSEUDO_REGISTER  */
