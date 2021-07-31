--- Source/WTF/wtf/Platform.h.orig	2015-06-29 21:11:01.000000000 +0100
+++ Source/WTF/wtf/Platform.h	2015-12-01 15:51:34.180171381 +0000
@@ -28,6 +28,12 @@
 #ifndef WTF_Platform_h
 #define WTF_Platform_h
 
+#if defined(__riscos__) && defined(__ARM_ARCH_3__)
+/* GCC defines __ARM_ARCH_3__ which is technically too old, so we'll fudge
+ * it a bit.  */
+#define __ARM_ARCH_4__
+#endif
+
 /* Include compiler specific macros */
 #include <wtf/Compiler.h>
 
@@ -451,6 +457,10 @@
 #define WTF_OS_WINDOWS 1
 #endif
 
+#if defined(__riscos__)
+#define WTF_OS_RISCOS 1
+#endif
+
 #define WTF_OS_WIN ERROR "USE WINDOWS WITH OS NOT WIN"
 #define WTF_OS_MAC ERROR "USE MAC_OS_X WITH OS NOT MAC"
 
@@ -465,6 +475,7 @@
     || OS(OPENBSD)          \
     || OS(QNX)              \
     || OS(SOLARIS)          \
+    || OS(RISCOS)           \
     || defined(unix)        \
     || defined(__unix)      \
     || defined(__unix__)
@@ -497,7 +508,6 @@
 
 #endif
 
-
 /* Operating environments */
 
 /* FIXME: these are all mixes of OS, operating environment and policy choices. */
@@ -660,6 +670,17 @@
 #define HAVE_PTHREAD_NP_H 1
 #endif
 
+#if OS(RISCOS)
+#define HAVE_LANGINFO_H 1
+#define HAVE_ERRNO_H 1
+#define HAVE_SIGNAL_H 1
+#define HAVE_STRINGS_H 1
+#define HAVE_SYS_PARAM_H 1
+#define HAVE_SYS_TIME_H 1 
+#define WTF_USE_PTHREADS 1
+#define HAVE_PTHREAD_SETNAME_NP 1
+#endif
+
 #if !defined(HAVE_VASPRINTF)
 #if !COMPILER(MSVC) && !COMPILER(RVCT) && !COMPILER(MINGW) && !(COMPILER(GCC) && OS(QNX))
 #define HAVE_VASPRINTF 1
@@ -732,6 +753,18 @@
 #define ENABLE_GLOBAL_FASTMALLOC_NEW 0
 #endif
 
+#if OS(RISCOS)
+#if (defined(__VFP_FP__) && !defined(__SOFTFP__))
+#define ENABLE_JIT 1
+#else
+// The JIT generates VFP floating point instructions which don't work
+// in rpcemu.
+#define ENABLE_JIT 0
+#endif
+#define ENABLE_GLOBAL_FASTMALLOC_NEW 0
+#define USE_SYSTEM_MALLOC 1
+#endif
+
 #if !defined(ENABLE_GLOBAL_FASTMALLOC_NEW)
 #define ENABLE_GLOBAL_FASTMALLOC_NEW 1
 #endif
