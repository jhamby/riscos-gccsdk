--- Makefile.def.orig	2020-07-23 07:35:16.904379704 +0100
+++ Makefile.def	2020-07-24 17:16:30.901913990 +0100
@@ -46,7 +46,8 @@
 host_modules= { module= flex; no_check_cross= true; };
 host_modules= { module= gas; bootstrap=true; };
 host_modules= { module= gcc; bootstrap=true; 
-		extra_make_flags="$(EXTRA_GCC_FLAGS)"; };
+		extra_make_flags="$(EXTRA_GCC_FLAGS)";
+		extra_configure_flags='`echo @target_configargs@ | egrep -o -- "--(enable|disable)-shared"`'; };
 host_modules= { module= gmp; lib_path=.libs; bootstrap=true;
 		// Work around in-tree gmp configure bug with missing flex.
 		extra_configure_flags='--disable-shared LEX="touch lex.yy.c"';
@@ -150,6 +151,7 @@
 		   lib_path=.libs;
 		   extra_configure_flags='@extra_liboffloadmic_configure_flags@'; };
 target_modules = { module= libssp; lib_path=.libs; };
+target_modules = { module= libunixlib; lib_path=.libs; };
 target_modules = { module= newlib; };
 target_modules = { module= libgcc; bootstrap=true; no_check=true; };
 target_modules = { module= libbacktrace; };
@@ -565,6 +567,7 @@
 lang_env_dependencies = { module=libitm; cxx=true; };
 lang_env_dependencies = { module=libffi; cxx=true; };
 lang_env_dependencies = { module=liboffloadmic; cxx=true; };
+lang_env_dependencies = { module=libunixlib; no_c=true; };
 lang_env_dependencies = { module=newlib; no_c=true; };
 lang_env_dependencies = { module=libgloss; no_c=true; };
 lang_env_dependencies = { module=libgcc; no_gcc=true; no_c=true; };
@@ -616,6 +619,8 @@
 
 dependencies = { module=all-target-libgloss; on=all-target-newlib; };
 dependencies = { module=all-target-winsup; on=all-target-libtermcap; };
+dependencies = { module=configure-target-libunixlib; on=all-binutils; };
+dependencies = { module=configure-target-libunixlib; on=all-ld; };
 dependencies = { module=configure-target-newlib; on=all-binutils; };
 dependencies = { module=configure-target-newlib; on=all-ld; };
 dependencies = { module=configure-target-libgfortran; on=all-target-libquadmath; };
