--- src/corelib/io/qtemporarydir.cpp.orig	2015-02-17 04:56:51.000000000 +0000
+++ src/corelib/io/qtemporarydir.cpp	2015-04-12 13:43:51.636657032 +0100
@@ -139,6 +139,15 @@
     return 0;
 }
 
+#elif defined(Q_OS_RISCOS)
+
+static char *q_mkdtemp(char *templateName)
+{
+    printf ("[%s:%d:%s]: Warning: Calling mktemp instead of mkdtemp\n",
+	    __func__, __LINE__, __FILE__);
+    return mktemp (templateName);
+}
+
 #else // defined(Q_OS_QNX ) || defined(Q_OS_WIN) || defined(Q_OS_ANDROID)
 
 static char *q_mkdtemp(char *templateName)
