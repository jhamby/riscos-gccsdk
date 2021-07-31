--- Makefile.orig	2019-11-07 18:49:05.998030262 +0000
+++ Makefile	2019-11-07 18:49:46.594030304 +0000
@@ -20,7 +20,7 @@
 # Do not specify CFLAGS or LIBS on the make invocation line - specify
 # XCFLAGS or XLIBS instead. Make ignores any lines in the makefile that
 # set a variable that was set on the command line.
-CFLAGS += $(XCFLAGS) -Iinclude
+CFLAGS += $(XCFLAGS) -Iinclude -I$(GCCSDK_INSTALL_ENV)/include/harfbuzz
 LIBS += $(XLIBS) -lm
 
 ifneq ($(threading),no)
