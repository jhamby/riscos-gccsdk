--- gnattools/Makefile.in.orig	2008-04-28 23:08:44.000000000 +0200
+++ gnattools/Makefile.in	2008-04-28 23:08:34.000000000 +0200
@@ -224,7 +224,11 @@
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
@@ -302,6 +306,9 @@
 # Build DVI (none here).
 dvi:
 
+# Build HTML (none here).
+html:
+
 # Build TAGS (none here).
 TAGS:
 
