--- Makefile.def.orig	2006-09-29 02:08:14.000000000 +0100
+++ Makefile.def	2006-09-29 01:58:16.000000000 +0100
@@ -113,6 +113,7 @@
 host_modules= { module= utils; no_check=true; };
 host_modules= { module= gnattools; };
 
+target_modules = { module= libunixlib; lib_path=.libs; };
 target_modules = { module= libstdc++-v3; lib_path=.libs; raw_cxx=true; };
 target_modules = { module= libmudflap; lib_path=.libs; };
 target_modules = { module= libssp; lib_path=.libs; };
@@ -228,6 +229,7 @@
 flags_to_pass = { flag= LIBCFLAGS_FOR_TARGET ; };
 flags_to_pass = { flag= LIBCXXFLAGS_FOR_TARGET ; };
 flags_to_pass = { flag= NM_FOR_TARGET ; };
+flags_to_pass = { flag= OBJCOPY_FOR_TARGET ; };
 flags_to_pass = { flag= OBJDUMP_FOR_TARGET ; };
 flags_to_pass = { flag= RANLIB_FOR_TARGET ; };
 flags_to_pass = { flag= STRIP_FOR_TARGET ; };
@@ -406,6 +408,7 @@
 
 // Target modules.  These can also have dependencies on the language
 // environment (e.g. on libstdc++).
+lang_env_dependencies = { module=libunixlib; };
 lang_env_dependencies = { module=boehm-gc; };
 lang_env_dependencies = { module=gperf; cxx=true; };
 lang_env_dependencies = { module=libada; };
@@ -433,6 +436,7 @@
 dependencies = { module=all-target-libjava; on=all-target-libffi; };
 dependencies = { module=all-target-libobjc; on=all-target-libiberty; };
 dependencies = { module=all-target-libstdc++-v3; on=all-target-libiberty; };
+dependencies = { module=all-target-libiberty; on=all-target-libunixlib; };
 
 // Target modules in the 'src' repository.
 lang_env_dependencies = { module=examples; };
