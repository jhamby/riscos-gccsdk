--- builds/unix/configure.ac.old	2008-03-17 15:01:07.915000000 +0000
+++ builds/unix/configure.ac	2008-03-17 15:02:08.353000000 +0000
@@ -40,7 +40,7 @@
 # checks for native programs to generate building tool
 
 if test ${cross_compiling} = yes; then
-  AC_CHECK_PROG(CC_BUILD, ${build}-gcc, ${build-gcc})
+  AC_CHECK_PROG(CC_BUILD, ${build}-gcc, ${build}-gcc)
   test -z "${CC_BUILD}" && AC_CHECK_PROG(CC_BUILD, gcc, gcc)
   test -z "${CC_BUILD}" && AC_CHECK_PROG(CC_BUILD, cc, cc, , , /usr/ucb/cc)
   test -z "${CC_BUILD}" && AC_MSG_ERROR([cannot find native C compiler])
