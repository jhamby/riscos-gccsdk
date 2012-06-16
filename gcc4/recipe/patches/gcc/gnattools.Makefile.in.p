Index: gnattools/Makefile.in
===================================================================
--- gnattools/Makefile.in	(revision 179880)
+++ gnattools/Makefile.in	(working copy)
@@ -183,7 +183,11 @@
 # put the host RTS dir first in the PATH to hide the default runtime
 # files that are among the sources
 # FIXME: This should be done in configure.
+ifeq (@host_alias@,)
 RTS_DIR:=$(strip $(subst \,/,$(shell gnatls -v | grep adalib )))
+else
+RTS_DIR:=$(strip $(subst \,/,$(shell @host_alias@-gnatls -v | grep adalib )))
+endif
 gnattools-cross: $(GCC_DIR)/stamp-tools
 	# gnattools1-re
 	$(MAKE) -C $(GCC_DIR)/ada/tools -f ../Makefile \
