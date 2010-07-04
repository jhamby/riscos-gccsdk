--- gcc/cppdefault.c.orig	2010-07-04 15:44:02.086903139 +0200
+++ gcc/cppdefault.c	2010-07-04 15:43:36.806901829 +0200
@@ -48,44 +48,44 @@ const struct default_include cpp_include
 = {
 #ifdef GPLUSPLUS_INCLUDE_DIR
     /* Pick up GNU C++ generic include files.  */
-    { GPLUSPLUS_INCLUDE_DIR, "G++", 1, 1, 0 },
+    { GPLUSPLUS_INCLUDE_DIR, "G++", 1, 1, 0, 0 },
 #endif
 #ifdef GPLUSPLUS_TOOL_INCLUDE_DIR
     /* Pick up GNU C++ target-dependent include files.  */
-    { GPLUSPLUS_TOOL_INCLUDE_DIR, "G++", 1, 1, 0 },
+    { GPLUSPLUS_TOOL_INCLUDE_DIR, "G++", 1, 1, 0, 1 },
 #endif
 #ifdef GPLUSPLUS_BACKWARD_INCLUDE_DIR
     /* Pick up GNU C++ backward and deprecated include files.  */
-    { GPLUSPLUS_BACKWARD_INCLUDE_DIR, "G++", 1, 1, 0 },
+    { GPLUSPLUS_BACKWARD_INCLUDE_DIR, "G++", 1, 1, 0, 0 },
 #endif
 #ifdef LOCAL_INCLUDE_DIR
     /* /usr/local/include comes before the fixincluded header files.  */
-    { LOCAL_INCLUDE_DIR, 0, 0, 1, 1 },
+    { LOCAL_INCLUDE_DIR, 0, 0, 1, 1, 0 },
 #endif
 #ifdef PREFIX_INCLUDE_DIR
-    { PREFIX_INCLUDE_DIR, 0, 0, 1, 0 },
+    { PREFIX_INCLUDE_DIR, 0, 0, 1, 0, 0 },
 #endif
 #ifdef GCC_INCLUDE_DIR
     /* This is the dir for fixincludes and for gcc's private headers.  */
-    { GCC_INCLUDE_DIR, "GCC", 0, 0, 0 },
+    { GCC_INCLUDE_DIR, "GCC", 0, 0, 0, 0 },
 #endif
 #ifdef CROSS_INCLUDE_DIR
     /* One place the target system's headers might be.  */
-    { CROSS_INCLUDE_DIR, "GCC", 0, 0, 0 },
+    { CROSS_INCLUDE_DIR, "GCC", 0, 0, 0, 0 },
 #endif
 #ifdef TOOL_INCLUDE_DIR
     /* Another place the target system's headers might be.  */
-    { TOOL_INCLUDE_DIR, "BINUTILS", 0, 1, 0 },
+    { TOOL_INCLUDE_DIR, "BINUTILS", 0, 1, 0, 0 },
 #endif
 #ifdef SYSTEM_INCLUDE_DIR
     /* Some systems have an extra dir of include files.  */
-    { SYSTEM_INCLUDE_DIR, 0, 0, 0, 1 },
+    { SYSTEM_INCLUDE_DIR, 0, 0, 0, 1, 0 },
 #endif
 #ifdef STANDARD_INCLUDE_DIR
     /* /usr/include comes dead last.  */
-    { STANDARD_INCLUDE_DIR, STANDARD_INCLUDE_COMPONENT, 0, 0, 1 },
+    { STANDARD_INCLUDE_DIR, STANDARD_INCLUDE_COMPONENT, 0, 0, 1, 0 },
 #endif
-    { 0, 0, 0, 0, 0 }
+    { 0, 0, 0, 0, 0, 0 }
   };
 #endif /* no INCLUDE_DEFAULTS */
 
