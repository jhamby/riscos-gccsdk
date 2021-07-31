--- src/tests.cpp.orig	2020-09-08 12:35:54.212895010 +0100
+++ src/tests.cpp	2020-09-08 12:38:35.765743749 +0100
@@ -210,7 +210,7 @@
  assert(sizeof(long) == SIZEOF_LONG);
  assert(sizeof(long long) == SIZEOF_LONG_LONG);
 
- assert(sizeof(off_t) == SIZEOF_OFF_T);
+// assert(sizeof(off_t) == SIZEOF_OFF_T);
  assert(sizeof(ptrdiff_t) == SIZEOF_PTRDIFF_T);
  assert(sizeof(size_t) == SIZEOF_SIZE_T);
  assert(sizeof(void*) == SIZEOF_VOID_P);
