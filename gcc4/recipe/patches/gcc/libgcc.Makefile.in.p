Index: libgcc/Makefile.in
===================================================================
--- libgcc/Makefile.in	(revision 177425)
+++ libgcc/Makefile.in	(working copy)
@@ -245,7 +245,7 @@
 gcc_compile_bare = $(CC) $(INTERNAL_CFLAGS)
 compile_deps = -MT $@ -MD -MP -MF $(basename $@).dep
 gcc_compile = $(gcc_compile_bare) -o $@ $(compile_deps)
-gcc_s_compile = $(gcc_compile) -DSHARED
+gcc_s_compile = $(gcc_compile) -DSHARED -fPIC
 
 objects = $(filter %$(objext),$^)
 
