--- GuiLib/GuiFlex/Makefile.cross.orig	2021-08-11 12:40:36.019070188 -0700
+++ GuiLib/GuiFlex/Makefile.cross	2021-08-11 12:42:48.736083597 -0700
@@ -1,5 +1,3 @@
-CC=gcc
-
 OBJS = Flex_c.o GuiFlexDA.o GuiFlexFlex.o GuiFlexGlobal.o flex.o
 
 CXXFLAGS = -Wall -D__APCS_32 -O3 -I. -I ../../GuiLib/Task -I ../../GuiLib/Wimp -I ../../GuiLib/Util -I ../../GuiLib/GuiFlex
