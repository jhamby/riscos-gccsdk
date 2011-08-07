Index: libgcc/Makefile.in
===================================================================
--- libgcc/Makefile.in	(revision 177425)
+++ libgcc/Makefile.in	(working copy)
@@ -247,6 +247,10 @@
 gcc_compile = $(gcc_compile_bare) -o $@ $(compile_deps)
 gcc_s_compile = $(gcc_compile) -DSHARED
 
+ifeq ($(host_noncanonical),arm-unknown-riscos)
+  gcc_s_compile += -fPIC
+endif
+
 objects = $(filter %$(objext),$^)
 
 # Collect any host-specific information from Makefile fragments.
