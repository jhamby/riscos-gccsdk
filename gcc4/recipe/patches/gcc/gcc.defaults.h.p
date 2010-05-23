--- gcc/defaults.h.orig	2010-05-02 20:10:05.294534071 +0200
+++ gcc/defaults.h	2010-05-02 20:09:51.582032153 +0200
@@ -353,7 +353,7 @@ Software Foundation, 51 Franklin Street,
 #endif
 
 /* This decision to use a .jcr section can be overridden by defining
-   USE_JCR_SECTION to 0 in target file.  This is necessary if target
+   TARGET_USE_JCR_SECTION to 0 in target file.  This is necessary if target
    can define JCR_SECTION_NAME but does not have crtstuff or
    linker support for .jcr section.  */
 #ifndef TARGET_USE_JCR_SECTION
@@ -362,6 +362,10 @@ Software Foundation, 51 Franklin Street,
 #else
 #define TARGET_USE_JCR_SECTION 0
 #endif
+#else
+#if !TARGET_USE_JCR_SECTION
+#undef JCR_SECTION_NAME
+#endif
 #endif
 
 /* Number of hardware registers that go into the DWARF-2 unwind info.
