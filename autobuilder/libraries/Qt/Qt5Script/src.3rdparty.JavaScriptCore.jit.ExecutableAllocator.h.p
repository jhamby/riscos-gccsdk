--- src/3rdparty/javascriptcore/JavaScriptCore/jit/ExecutableAllocator.h.orig	2015-06-29 21:05:58.000000000 +0100
+++ src/3rdparty/javascriptcore/JavaScriptCore/jit/ExecutableAllocator.h	2015-11-26 20:25:21.408555106 +0000
@@ -49,6 +49,10 @@
 extern "C" __declspec(dllimport) void CacheRangeFlush(LPVOID pAddr, DWORD dwLength, DWORD dwFlags);
 #endif
 
+#if OS(RISCOS)
+extern "C" void __clear_cache (void *beg, void *end);
+#endif
+
 #define JIT_ALLOCATOR_PAGE_SIZE (ExecutableAllocator::pageSize)
 #define JIT_ALLOCATOR_LARGE_ALLOC_SIZE (ExecutableAllocator::pageSize * 4)
 
@@ -238,6 +242,14 @@
     {
         CacheRangeFlush(code, size, CACHE_SYNC_ALL);
     }
+#elif OS(RISCOS)
+    static void cacheFlush(void* code, size_t size)
+    {
+        // __clear_cache is a function provided by libgcc, the body of which
+        // is produced by the macro CLEAR_INSN_CACHE defined in
+        // gcc/config/arm/riscos-elf.h
+        __clear_cache(code, (char *)code + size);
+    }
 #else
     #error "The cacheFlush support is missing on this platform."
 #endif
