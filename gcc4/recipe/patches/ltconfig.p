--- ltconfig.orig	2007-06-18 21:13:36.000000000 +0100
+++ ltconfig	2007-06-18 21:13:25.000000000 +0100
@@ -1412,6 +1412,14 @@
   fi
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
