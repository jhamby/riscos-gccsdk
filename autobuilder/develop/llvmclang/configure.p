--- configure.orig	2014-09-28 20:45:44.000000000 +0100
+++ configure	2014-09-28 20:49:39.931945487 +0100
@@ -3979,6 +3979,11 @@
     llvm_cv_no_link_all_option="-Wl,--no-whole-archive"
     llvm_cv_os_type="Linux"
     llvm_cv_platform_type="Unix" ;;
+  *-*-riscos*)
+    llvm_cv_link_all_option="-Wl,--whole-archive"
+    llvm_cv_no_link_all_option="-Wl,--no-whole-archive"
+    llvm_cv_os_type="Linux"
+    llvm_cv_platform_type="Unix" ;;
   *-*-gnu*)
     llvm_cv_link_all_option="-Wl,--whole-archive"
     llvm_cv_no_link_all_option="-Wl,--no-whole-archive"
@@ -4061,6 +4066,8 @@
     llvm_cv_target_os_type="Interix" ;;
   *-*-linux*)
     llvm_cv_target_os_type="Linux" ;;
+  *-*-riscos*)
+    llvm_cv_target_os_type="Linux" ;;
   *-*-gnu*)
     llvm_cv_target_os_type="GNU" ;;
   *-*-solaris*)
