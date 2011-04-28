Index: libgcc/Makefile.in
===================================================================
--- libgcc/Makefile.in	(revision 172858)
+++ libgcc/Makefile.in	(working copy)
@@ -245,7 +245,12 @@
 gcc_compile_bare = $(CC) $(INTERNAL_CFLAGS)
 compile_deps = -MT $@ -MD -MP -MF $(basename $@).dep
 gcc_compile = $(gcc_compile_bare) -o $@ $(compile_deps)
+
+ifeq (,$(findstring module,$(inst_libdir)))
+gcc_s_compile = $(gcc_compile) -DSHARED -fPIC
+else
 gcc_s_compile = $(gcc_compile) -DSHARED
+endif
 
 objects = $(filter %$(objext),$^)
 
