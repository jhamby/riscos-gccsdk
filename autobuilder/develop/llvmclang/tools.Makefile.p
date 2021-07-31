--- tools/Makefile.orig	2014-06-05 09:49:55.000000000 +0100
+++ tools/Makefile	2015-01-22 15:36:19.903047319 +0000
@@ -31,7 +31,8 @@
                  lli llvm-extract llvm-mc bugpoint llvm-bcanalyzer llvm-diff \
                  macho-dump llvm-objdump llvm-readobj llvm-rtdyld \
                  llvm-dwarfdump llvm-cov llvm-size llvm-stress llvm-mcmarkup \
-                 llvm-profdata llvm-symbolizer obj2yaml yaml2obj llvm-c-test
+                 llvm-profdata llvm-symbolizer obj2yaml yaml2obj
+#llvm-c-test
 
 # If Intel JIT Events support is configured, build an extra tool to test it.
 ifeq ($(USE_INTEL_JITEVENTS), 1)
