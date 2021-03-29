--- ./src/video/movie.cpp.orig	2021-03-28 19:15:52.909811268 +0000
+++ ./src/video/movie.cpp	2021-03-28 19:16:42.369224194 +0000
@@ -298,6 +298,7 @@
 
 #else
 
+#include <cstring>
 #include <string>
 
 /**
