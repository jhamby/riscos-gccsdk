--- lib/ExecutionEngine/RTDyldMemoryManager.cpp.orig	2014-01-07 11:48:04.000000000 +0000
+++ lib/ExecutionEngine/RTDyldMemoryManager.cpp	2015-01-21 21:49:20.904422944 +0000
@@ -125,13 +125,25 @@
 
   // How can it find the end? Because crtendS.o is linked 
   // in and it has an .eh_frame section with four zero chars.
+#ifdef __riscos__
+  // Not interested in this at the moment, but the build stops
+  // with __register_frame being an undefined reference.
+  abort();
+#else
   __register_frame(Addr);
+#endif
 }
 
 void RTDyldMemoryManager::deregisterEHFrames(uint8_t *Addr,
                                            uint64_t LoadAddr,
                                            size_t Size) {
+#ifdef __riscos__
+  // Not interested in this at the moment, but the build stops
+  // with __deregister_frame being an undefined reference.
+  abort();
+#else
   __deregister_frame(Addr);
+#endif
 }
 
 #endif
