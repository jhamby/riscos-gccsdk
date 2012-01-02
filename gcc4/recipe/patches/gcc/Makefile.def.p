Index: Makefile.def
===================================================================
--- Makefile.def	(revision 182810)
+++ Makefile.def	(working copy)
@@ -57,7 +57,8 @@
 host_modules= { module= flex; no_check_cross= true; };
 host_modules= { module= gas; bootstrap=true; };
 host_modules= { module= gcc; bootstrap=true; 
-		extra_make_flags="$(EXTRA_GCC_FLAGS)"; };
+		extra_make_flags="$(EXTRA_GCC_FLAGS)";
+		extra_configure_flags='`echo @target_configargs@ | egrep -o -- "--(enable|disable)-shared"`'; };
 host_modules= { module= gawk; };
 host_modules= { module= gettext; };
 host_modules= { module= gmp; lib_path=.libs; bootstrap=true;
@@ -154,6 +155,7 @@
 		   raw_cxx=true; };
 target_modules = { module= libmudflap; lib_path=.libs; };
 target_modules = { module= libssp; lib_path=.libs; };
+target_modules = { module= libunixlib; lib_path=.libs; };
 target_modules = { module= newlib; };
 target_modules = { module= libgcc; bootstrap=true; no_check=true; };
 target_modules = { module= libquadmath; };
@@ -539,6 +541,7 @@
 // on libgcc and newlib/libgloss.
 lang_env_dependencies = { module=gperf; cxx=true; };
 lang_env_dependencies = { module=libjava; cxx=true; };
+lang_env_dependencies = { module=libunixlib; no_c=true; };
 lang_env_dependencies = { module=newlib; no_c=true; };
 lang_env_dependencies = { module=libgloss; no_c=true; };
 lang_env_dependencies = { module=libgcc; no_gcc=true; no_c=true; };
@@ -580,6 +583,8 @@
 
 dependencies = { module=all-target-libgloss; on=all-target-newlib; };
 dependencies = { module=all-target-winsup; on=all-target-libtermcap; };
+dependencies = { module=configure-target-libunixlib; on=all-binutils; };
+dependencies = { module=configure-target-libunixlib; on=all-ld; };
 dependencies = { module=configure-target-newlib; on=all-binutils; };
 dependencies = { module=configure-target-newlib; on=all-ld; };
 dependencies = { module=configure-target-libgfortran; on=all-target-libquadmath; };
