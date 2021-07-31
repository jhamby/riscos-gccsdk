--- Source/WTF/wtf/RAMSize.cpp.orig	2014-05-15 18:12:23.000000000 +0100
+++ Source/WTF/wtf/RAMSize.cpp	2014-06-14 19:51:36.445940974 +0100
@@ -58,11 +58,15 @@
         return ramSizeGuess;
     return ramSize > std::numeric_limits<size_t>::max() ? std::numeric_limits<size_t>::max() : static_cast<size_t>(ramSize);
 #elif OS(UNIX)
+#if OS(RISCOS)
+    return ramSizeGuess;
+#else
     long pages = sysconf(_SC_PHYS_PAGES);
     long pageSize = sysconf(_SC_PAGE_SIZE);
     if (pages == -1 || pageSize == -1)
         return ramSizeGuess;
     return pages * pageSize;
+#endif
 #elif OS(WINCE)
     MEMORYSTATUS status;
     status.dwLength = sizeof(status);
