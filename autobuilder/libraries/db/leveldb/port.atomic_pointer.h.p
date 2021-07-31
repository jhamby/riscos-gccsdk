--- port/atomic_pointer.h.orig	2014-06-16 18:58:21.000000000 +0100
+++ port/atomic_pointer.h	2014-06-16 19:28:02.084213483 +0100
@@ -212,6 +212,16 @@
 }
 #define LEVELDB_HAVE_MEMORY_BARRIER
 
+#elif defined(ARCH_CPU_ARM_FAMILY) && defined(__riscos__)
+
+inline void ReadMemoryBarrier() {
+  asm volatile("" : : : "memory");
+}
+inline void WriteMemoryBarrier() {
+  asm volatile("" : : : "memory");
+}
+#define LEVELDB_HAVE_MEMORY_BARRIER
+
 #endif
 
 // AtomicPointer built using platform-specific MemoryBarrier()
