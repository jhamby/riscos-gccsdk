--- !RiscPkg-GenControl/Makefile.orig	2015-02-03 12:55:29.667708000 +0000
+++ !RiscPkg-GenControl/Makefile	2015-02-06 13:02:58.344375070 +0000
@@ -9,11 +9,11 @@
 OBJ = main.o
 
 LIB = -lstdc++ \
- -llibpkg:a.libpkg
+ -lpkg
 
 all: riscpkg-gencontrol
 
 riscpkg-gencontrol: $(OBJ)
 	$(LD) -o riscpkg-gencontrol $(OBJ) $(LIB)
 
-include $(OBJ:.o=.d)
+#include $(OBJ:.o=.d)
