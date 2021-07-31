--- Source/WebCore/platform/leveldb/LevelDBDatabase.cpp.orig	2014-05-15 18:12:29.000000000 +0100
+++ Source/WebCore/platform/leveldb/LevelDBDatabase.cpp	2014-06-17 16:21:25.534406127 +0100
@@ -38,7 +38,7 @@
 #include <leveldb/comparator.h>
 #include <leveldb/db.h>
 #include <leveldb/env.h>
-#include <helpers/memenv/memenv.h>
+#include <leveldb/helpers/memenv.h>
 #include <leveldb/slice.h>
 #include <string>
 #include <wtf/PassOwnPtr.h>
