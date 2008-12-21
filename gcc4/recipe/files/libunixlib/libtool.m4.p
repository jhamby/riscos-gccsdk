--- libtool-org.m4	2008-12-21 00:30:30.000000000 +0100
+++ libtool.m4	2008-12-21 01:03:43.000000000 +0100
@@ -1039,6 +1039,12 @@
     ])
    ;;
 
+  *riscos)
+    lt_cv_dlopen="dlopen"
+    lt_cv_dlopen_libs="-ldl"
+    lt_cv_dlopen_self=yes
+   ;;
+
   *)
     AC_CHECK_FUNC([shl_load],
 	  [lt_cv_dlopen="shl_load"],
@@ -1882,6 +1888,14 @@
   shlibpath_var=LD_LIBRARY_PATH
   ;;
 
+*riscos)
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
@@ -3823,6 +3837,9 @@
     # FIXME: insert proper C++ library support
     _LT_AC_TAGVAR(ld_shlibs, $1)=no
     ;;
+  *riscos)
+    _LT_AC_TAGVAR(ld_shlibs, $1)=yes
+    ;;
   *)
     # FIXME: insert proper C++ library support
     _LT_AC_TAGVAR(ld_shlibs, $1)=no
@@ -5061,6 +5078,11 @@
 	;;
       esac
       ;;
+    *riscos)
+      _LT_AC_TAGVAR(lt_prog_compiler_pic, $1)='-fPIC'
+      _LT_AC_TAGVAR(lt_prog_compiler_static, $1)='-static'
+      _LT_AC_TAGVAR(lt_prog_compiler_wl, $1)='-Wl,'
+      ;;
     *)
       _LT_AC_TAGVAR(lt_prog_compiler_pic, $1)='-fPIC'
       ;;
@@ -5278,6 +5300,11 @@
 	;;
       vxworks*)
 	;;
+      *riscos)
+        _LT_AC_TAGVAR(lt_prog_compiler_pic, $1)='-fPIC'
+        _LT_AC_TAGVAR(lt_prog_compiler_static, $1)='-static'
+        _LT_AC_TAGVAR(lt_prog_compiler_wl, $1)='-Wl,'
+        ;;
       *)
 	_LT_AC_TAGVAR(lt_prog_compiler_can_build_shared, $1)=no
 	;;
@@ -5355,6 +5382,12 @@
       esac
       ;;
 
+    *riscos)
+      _LT_AC_TAGVAR(lt_prog_compiler_pic, $1)='-fPIC'
+      _LT_AC_TAGVAR(lt_prog_compiler_static, $1)='-static'
+      _LT_AC_TAGVAR(lt_prog_compiler_wl, $1)='-Wl,'
+      ;;
+
     *)
       _LT_AC_TAGVAR(lt_prog_compiler_pic, $1)='-fPIC'
       ;;
@@ -5510,6 +5543,12 @@
       _LT_AC_TAGVAR(lt_prog_compiler_static, $1)='-Bstatic'
       ;;
 
+    *riscos)
+      _LT_AC_TAGVAR(lt_prog_compiler_pic, $1)='-fPIC'
+      _LT_AC_TAGVAR(lt_prog_compiler_static, $1)='-static'
+      _LT_AC_TAGVAR(lt_prog_compiler_wl, $1)='-Wl,'
+      ;;
+
     *)
       _LT_AC_TAGVAR(lt_prog_compiler_can_build_shared, $1)=no
       ;;
