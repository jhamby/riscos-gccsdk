--- ltconfig.old	2003-05-20 16:07:14.000000000 +0100
+++ ltconfig	2003-05-20 16:08:09.000000000 +0100
@@ -2179,6 +2179,11 @@
   sys_lib_dlsearch_path_spec="$sys_lib_search_path_spec"
   ;;
 
+riscos*)
+  sys_lib_dlsearch_path_spec="/home/riscos/env/lib"
+  sys_lib_search_path_spec="/home/riscos/env/lib"
+  ;;
+
 sco3.2v5*)
   version_type=osf
   soname_spec='${libname}${release}.so$major'
