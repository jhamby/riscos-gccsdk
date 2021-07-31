--- ./src/editor/editor_level.cpp.orig	2011-09-08 13:55:32.258997400 +0100
+++ ./src/editor/editor_level.cpp	2011-09-08 14:07:13.562832600 +0100
@@ -23,6 +23,10 @@
 #include "pingus/pingus_level.hpp"
 #include "util/sexpr_file_writer.hpp"
 
+#ifdef __riscos__
+#include "../riscos/config.h"
+#endif
+
 namespace Editor {
 
 static bool LevelObjSort(LevelObj *a, LevelObj *b)
