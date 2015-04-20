--- Tools/DumpRenderTree/DumpRenderTree.h.orig	2014-05-15 18:12:21.000000000 +0100
+++ Tools/DumpRenderTree/DumpRenderTree.h	2015-04-06 15:03:13.726564934 +0100
@@ -49,7 +49,7 @@
 #include <string>
 #include <wtf/RefPtr.h>
 
-#if !OS(OPENBSD)
+#if !OS(OPENBSD) && !OS(RISCOS)
 std::wstring urlSuitableForTestResult(const std::wstring& url);
 #endif
 
