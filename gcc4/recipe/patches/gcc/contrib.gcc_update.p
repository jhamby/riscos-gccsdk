--- contrib/gcc_update.orig	2007-01-07 19:13:19.000000000 +0100
+++ contrib/gcc_update	2007-01-07 18:02:47.000000000 +0100
@@ -102,6 +102,12 @@
 libgfortran/aclocal.m4: libgfortran/configure.ac libgfortran/acinclude.m4
 libgfortran/Makefile.in: libgfortran/Makefile.am libgfortran/configure.ac libgfortran/aclocal.m4
 libgfortran/configure: libgfortran/configure.ac libgfortran/aclocal.m4
+libunixlib/aclocal.m4: libunixlib/configure.ac
+libunixlib/Makefile.in: libunixlib/Makefile.am libunixlib/configure.ac libunixlib/aclocal.m4
+libunixlib/configure: libunixlib/configure.ac libunixlib/aclocal.m4
+libscl/aclocal.m4: libscl/configure.ac
+libscl/Makefile.in: libscl/Makefile.am libscl/configure.ac libscl/aclocal.m4
+libscl/configure: libscl/configure.ac libscl/aclocal.m4
 libjava/aclocal.m4: libjava/configure.ac
 libjava/Makefile.in: libjava/Makefile.am libjava/configure.ac libjava/aclocal.m4
 libjava/configure: libjava/configure.ac libjava/aclocal.m4
