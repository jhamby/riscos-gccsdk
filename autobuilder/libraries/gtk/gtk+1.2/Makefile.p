--- gtk/Makefile.in.orig	2007-11-24 16:48:33.000000000 -0800
+++ gtk/Makefile.in	2007-11-24 16:48:42.000000000 -0800
@@ -258,7 +258,7 @@
 noinst_PROGRAMS =  testgtk$(EXEEXT) testinput$(EXEEXT) \
 testselection$(EXEEXT) testrgb$(EXEEXT) testdnd$(EXEEXT) \
 simple$(EXEEXT)
-PROGRAMS =  $(noinst_PROGRAMS)
+#PROGRAMS =  $(noinst_PROGRAMS)
 
 testgtk_SOURCES = testgtk.c
 testgtk_OBJECTS =  testgtk.$(OBJEXT)
