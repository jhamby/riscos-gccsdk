--- src/corelib/io/qtemporarydir.cpp.orig	2014-05-15 18:12:17.000000000 +0100
+++ src/corelib/io/qtemporarydir.cpp	2014-06-12 19:35:04.989616516 +0100
@@ -144,6 +144,15 @@
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
