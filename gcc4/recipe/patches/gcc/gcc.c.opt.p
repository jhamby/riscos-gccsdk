--- gcc/c.opt.orig	2006-01-26 20:06:06.000000000 +0100
+++ gcc/c.opt	2010-07-04 15:43:36.824399309 +0200
@@ -753,6 +753,10 @@ imacros
 C ObjC C++ ObjC++ Joined Separate
 -imacros <file>	Accept definition of macros in <file>
 
+imultilib
+C ObjC C++ ObjC++ Joined Separate
+-imultilib <dir> Set <dir> to be the multilib include subdirectory
+
 include
 C ObjC C++ ObjC++ Joined Separate
 -include <file>	Include the contents of <file> before other files
