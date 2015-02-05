--- configure.orig	2014-10-31 19:33:25.000000000 +0000
+++ configure	2015-01-23 17:45:45.934682090 +0000
@@ -3979,6 +3979,11 @@
     llvm_cv_no_link_all_option="-Wl,--no-whole-archive"
     llvm_cv_os_type="Linux"
     llvm_cv_platform_type="Unix" ;;
+  *-*-riscos*)
+    llvm_cv_link_all_option="-Wl,--whole-archive"
+    llvm_cv_no_link_all_option="-Wl,--no-whole-archive"
+    llvm_cv_os_type="RISCOS"
+    llvm_cv_platform_type="Unix" ;;
   *-*-gnu*)
     llvm_cv_link_all_option="-Wl,--whole-archive"
     llvm_cv_no_link_all_option="-Wl,--no-whole-archive"
@@ -4061,6 +4066,8 @@
     llvm_cv_target_os_type="Interix" ;;
   *-*-linux*)
     llvm_cv_target_os_type="Linux" ;;
+  *-*-riscos*)
+    llvm_cv_target_os_type="RISCOS" ;;
   *-*-gnu*)
     llvm_cv_target_os_type="GNU" ;;
   *-*-solaris*)
