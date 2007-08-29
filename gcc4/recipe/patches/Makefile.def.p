--- Makefile.def.orig	2007-07-09 03:04:18.000000000 +0200
+++ Makefile.def	2007-07-09 02:45:06.000000000 +0200
@@ -113,6 +113,8 @@
 host_modules= { module= utils; no_check=true; };
 host_modules= { module= gnattools; };
 
+target_modules = { module= libunixlib; lib_path=.libs; };
+target_modules = { module= libscl; lib_path=.libs; };
 target_modules = { module= libstdc++-v3; lib_path=.libs; raw_cxx=true; };
 target_modules = { module= libmudflap; lib_path=.libs; };
 target_modules = { module= libssp; lib_path=.libs; };
@@ -433,6 +436,8 @@
 dependencies = { module=all-target-libjava; on=all-target-libffi; };
 dependencies = { module=all-target-libobjc; on=all-target-libiberty; };
 dependencies = { module=all-target-libstdc++-v3; on=all-target-libiberty; };
+dependencies = { module=configure-target-libiberty; on=all-target-libunixlib; };
+dependencies = { module=configure-target-libiberty; on=all-target-libscl; };
 
 // Target modules in the 'src' repository.
 lang_env_dependencies = { module=examples; };
