--- src/imagemgr.h.orig	2008-07-23 09:32:43.838534000 +0100
+++ src/imagemgr.h	2008-07-23 09:33:03.732534000 +0100
@@ -102,7 +102,7 @@
     static void destroy();
 
     ImageInfo *get(const std::string &name);
-    SubImage *ImageMgr::getSubImage(const std::string &name);
+    SubImage *getSubImage(const std::string &name);
     void freeIntroBackgrounds();
     const std::vector<std::string> &getSetNames();
 
