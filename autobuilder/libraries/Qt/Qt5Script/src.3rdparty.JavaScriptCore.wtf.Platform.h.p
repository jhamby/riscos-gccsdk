--- src/3rdparty/javascriptcore/JavaScriptCore/wtf/Platform.h.orig	2015-06-29 21:05:58.000000000 +0100
+++ src/3rdparty/javascriptcore/JavaScriptCore/wtf/Platform.h	2015-09-14 16:35:04.075068876 +0100
@@ -27,6 +27,12 @@
 #ifndef WTF_Platform_h
 #define WTF_Platform_h
 
+#ifdef __riscos__
+/* GCC defines __ARM_ARCH_3__ which is technically too old, so we'll fudge
+ * it a bit.  */
+#define __ARM_ARCH_4__
+#endif
+
 /* ==== PLATFORM handles OS, operating environment, graphics API, and
    CPU. This macro will be phased out in favor of platform adaptation
    macros, policy decision macros, and top-level port definitions. ==== */
@@ -505,6 +511,10 @@
 #define WTF_OS_SYMBIAN 1
 #endif
 
+#if defined(__riscos__)
+#define WTF_OS_RISCOS 1
+#endif
+
 /* OS(UNIX) - Any Unix-like system */
 #if   OS(AIX)              \
     || OS(ANDROID)          \
@@ -516,6 +526,7 @@
     || OS(NETBSD)           \
     || OS(OPENBSD)          \
     || OS(QNX)              \
+    || OS(RISCOS)           \
     || OS(SOLARIS)          \
     || OS(SYMBIAN)          \
     || defined(unix)        \
@@ -658,7 +669,21 @@
 
 #endif  /* OS(WINCE) && !PLATFORM(QT) */
 
-#if PLATFORM(QT)
+#if OS(RISCOS)
+#define HAVE_LANGINFO_H 1
+#define HAVE_ERRNO_H 1
+#define HAVE_SIGNAL_H 1
+#define HAVE_STRINGS_H 1
+#define HAVE_SYS_PARAM_H 1
+#define HAVE_SYS_TIME_H 1
+// Defining WTF_USE_PTHREADS causes compiler errors, such as:
+// no matching function for call to '__pthread_lock::__pthread_lock(QMutex*)'
+// I think we implicitly use pthread because that's what Qt uses.
+//#define WTF_USE_PTHREADS 1
+#define HAVE_PTHREAD_SETNAME_NP 1
+#endif
+
+#if PLATFORM(QT) && !OS(RISCOS)
 #define WTF_USE_QT4_UNICODE 1
 #elif OS(WINCE)
 #define WTF_USE_WINCE_UNICODE 1
@@ -860,6 +885,19 @@
 
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
+
+#endif
+
 /* ENABLE macro defaults */
 
 /* fastMalloc match validation allows for runtime verification that
