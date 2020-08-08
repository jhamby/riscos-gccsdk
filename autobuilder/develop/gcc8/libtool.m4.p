--- libtool.m4.orig	2015-11-26 13:20:59.000000000 +0000
+++ libtool.m4	2018-05-17 19:56:29.205314001 +0100
@@ -1747,6 +1747,12 @@
     lt_cv_dlopen_self=yes
     ])
     ;;
+ 
+  *riscos | *linux-gnueabihf)
+    lt_cv_dlopen="dlopen"
+    lt_cv_dlopen_libs="-ldl"
+    lt_cv_dlopen_self=yes
+   ;;
 
   *)
     AC_CHECK_FUNC([shl_load],
@@ -2678,6 +2684,14 @@
   shlibpath_var=LD_LIBRARY_PATH
   ;;
 
+*riscos | linux-gnueabihf)
+  version_type=linux
+  library_names_spec='${libname}$versuffix.so ${libname}.so$major $libname.so'
+  soname_spec='${libname}.so$major'
+  dynamic_linker='RISC OS ld-riscos.so'
+  lt_cv_dlopen="dlopen"
+  ;;
+
 *)
   dynamic_linker=no
   ;;
@@ -3655,12 +3669,28 @@
       # it will coredump.
       _LT_TAGVAR(lt_prog_compiler_pic, $1)='-fPIC -shared'
       ;;
+    *linux-gnueabihf)
+      _LT_TAGVAR(lt_prog_compiler_pic, $1)='-fpic'
+      _LT_TAGVAR(lt_prog_compiler_static, $1)='-static'
+      _LT_TAGVAR(lt_prog_compiler_wl, $1)='-Wl,'
+      ;;
+    *riscos)
+      _LT_TAGVAR(lt_prog_compiler_pic, $1)='-fPIC'
+      _LT_TAGVAR(lt_prog_compiler_static, $1)='-static'
+      _LT_TAGVAR(lt_prog_compiler_wl, $1)='-Wl,'
+      ;;
     *)
       _LT_TAGVAR(lt_prog_compiler_pic, $1)='-fPIC'
       ;;
     esac
   else
     case $host_os in
+      linux-gnueabihf)
+	# RISC OS
+        _LT_TAGVAR(lt_prog_compiler_pic, $1)='-fPIC'
+        _LT_TAGVAR(lt_prog_compiler_static, $1)='-static'
+        _LT_TAGVAR(lt_prog_compiler_wl, $1)='-Wl,'
+        ;;
       aix[[4-9]]*)
 	# All AIX code is PIC.
 	if test "$host_cpu" = ia64; then
@@ -3880,6 +3910,16 @@
 	;;
       vxworks*)
 	;;
+      *linux-gnueabihf)
+        _LT_TAGVAR(lt_prog_compiler_pic, $1)='-fpic'
+        _LT_TAGVAR(lt_prog_compiler_static, $1)='-static'
+        _LT_TAGVAR(lt_prog_compiler_wl, $1)='-Wl,'
+        ;;
+      *riscos)
+        _LT_TAGVAR(lt_prog_compiler_pic, $1)='-fPIC'
+        _LT_TAGVAR(lt_prog_compiler_static, $1)='-static'
+        _LT_TAGVAR(lt_prog_compiler_wl, $1)='-Wl,'
+        ;;
       *)
 	_LT_TAGVAR(lt_prog_compiler_can_build_shared, $1)=no
 	;;
@@ -3979,6 +4019,18 @@
       fi
       ;;
 
+    *linux-gnueabihf)
+      _LT_TAGVAR(lt_prog_compiler_pic, $1)='-fpic'
+      _LT_TAGVAR(lt_prog_compiler_static, $1)='-static'
+      _LT_TAGVAR(lt_prog_compiler_wl, $1)='-Wl,'
+      ;;
+
+    *riscos | linux-gnueabihf)
+      _LT_TAGVAR(lt_prog_compiler_pic, $1)='-fPIC'
+      _LT_TAGVAR(lt_prog_compiler_static, $1)='-static'
+      _LT_TAGVAR(lt_prog_compiler_wl, $1)='-Wl,'
+      ;;
+
     *)
       _LT_TAGVAR(lt_prog_compiler_pic, $1)='-fPIC'
       ;;
@@ -3993,6 +4045,12 @@
   else
     # PORTME Check for flag to pass linker flags through the system compiler.
     case $host_os in
+    linux-gnueabihf)
+      # RISC OS
+      _LT_TAGVAR(lt_prog_compiler_pic, $1)='-fPIC'
+      _LT_TAGVAR(lt_prog_compiler_static, $1)='-static'
+      _LT_TAGVAR(lt_prog_compiler_wl, $1)='-Wl,'
+      ;;
     aix*)
       _LT_TAGVAR(lt_prog_compiler_wl, $1)='-Wl,'
       if test "$host_cpu" = ia64; then
@@ -4157,6 +4215,18 @@
       _LT_TAGVAR(lt_prog_compiler_static, $1)='-Bstatic'
       ;;
 
+    *linux-gnueabihf)
+      _LT_TAGVAR(lt_prog_compiler_pic, $1)='-fpic'
+      _LT_TAGVAR(lt_prog_compiler_static, $1)='-static'
+      _LT_TAGVAR(lt_prog_compiler_wl, $1)='-Wl,'
+      ;;
+
+    *riscos)
+      _LT_TAGVAR(lt_prog_compiler_pic, $1)='-fPIC'
+      _LT_TAGVAR(lt_prog_compiler_static, $1)='-static'
+      _LT_TAGVAR(lt_prog_compiler_wl, $1)='-Wl,'
+      ;;
+
     *)
       _LT_TAGVAR(lt_prog_compiler_can_build_shared, $1)=no
       ;;
@@ -4612,6 +4682,10 @@
   else
     # PORTME fill in a description of your system's linker (not GNU ld)
     case $host_os in
+    linux-gnueabihf)
+      # RISC OS
+      _LT_TAGVAR(ld_shlibs, $1)=yes
+      ;;
     aix3*)
       _LT_TAGVAR(allow_undefined_flag, $1)=unsupported
       _LT_TAGVAR(always_export_symbols, $1)=yes
@@ -5176,6 +5250,10 @@
       _LT_TAGVAR(hardcode_shlibpath_var, $1)=no
       ;;
 
+    *riscos | *linux-gnueabihf)
+      _LT_TAGVAR(ld_shlibs, $1)=yes
+      ;;
+
     *)
       _LT_TAGVAR(ld_shlibs, $1)=no
       ;;
@@ -5570,6 +5648,10 @@
     AC_MSG_CHECKING([whether the $compiler linker ($LD) supports shared libraries])
     _LT_TAGVAR(ld_shlibs, $1)=yes
     case $host_os in
+      linux-gnueabihf)
+	 # RISC OS
+         _LT_TAGVAR(ld_shlibs, $1)=yes
+         ;;
       aix3*)
         # FIXME: insert proper C++ library support
         _LT_TAGVAR(ld_shlibs, $1)=no
@@ -6403,6 +6485,10 @@
         _LT_TAGVAR(ld_shlibs, $1)=no
         ;;
 
+     *riscos | *linux-gnueabihf)
+        _LT_TAGVAR(ld_shlibs, $1)=yes
+        ;;
+
       *)
         # FIXME: insert proper C++ library support
         _LT_TAGVAR(ld_shlibs, $1)=no
