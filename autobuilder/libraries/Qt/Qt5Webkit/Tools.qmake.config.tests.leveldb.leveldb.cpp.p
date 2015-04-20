--- Tools/qmake/config.tests/leveldb/leveldb.cpp.orig	2014-05-15 18:12:22.000000000 +0100
+++ Tools/qmake/config.tests/leveldb/leveldb.cpp	2014-06-16 21:16:30.992297828 +0100
@@ -19,7 +19,7 @@
 
 #include <leveldb/db.h>
 #include <leveldb/env.h>
-#include <helpers/memenv/memenv.h>
+#include <leveldb/helpers/memenv.h>
 
 int main(int, char**)
 {
