--- src/3rdparty/javascriptcore/JavaScriptCore/wtf/Platform.h.orig	2015-02-17 04:56:21.000000000 +0000
+++ src/3rdparty/javascriptcore/JavaScriptCore/wtf/Platform.h	2015-04-25 15:13:44.608384091 +0100
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
@@ -501,6 +507,10 @@
 #define WTF_OS_SYMBIAN 1
 #endif
 
+#if defined(__riscos__)
+#define WTF_OS_RISCOS 1
+#endif
+
 /* OS(UNIX) - Any Unix-like system */
 #if   OS(AIX)              \
     || OS(ANDROID)          \
@@ -512,6 +522,7 @@
     || OS(NETBSD)           \
     || OS(OPENBSD)          \
     || OS(QNX)              \
+    || OS(RISCOS)           \
     || OS(SOLARIS)          \
     || OS(SYMBIAN)          \
     || defined(unix)        \
@@ -654,7 +665,21 @@
 
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
@@ -856,6 +881,14 @@
 
 #endif
 
+#if OS(RISCOS)
+// The JIT generates VFP floating point instructions which don't work
+// in rpcemu.
+#define ENABLE_JIT 0
+#define ENABLE_GLOBAL_FASTMALLOC_NEW 0
+#define USE_SYSTEM_MALLOC 1
+#endif
+
 /* ENABLE macro defaults */
 
 /* fastMalloc match validation allows for runtime verification that
