--- Makefile.def.orig	2010-03-19 23:22:07.853949294 +0100
+++ Makefile.def	2010-03-19 23:21:58.602482655 +0100
@@ -113,6 +113,7 @@ host_modules= { module= libtermcap; no_c
 host_modules= { module= utils; no_check=true; };
 host_modules= { module= gnattools; };
 
+target_modules = { module= libunixlib; lib_path=.libs; };
 target_modules = { module= libstdc++-v3; lib_path=.libs; raw_cxx=true; };
 target_modules = { module= libmudflap; lib_path=.libs; };
 target_modules = { module= libssp; lib_path=.libs; };
@@ -433,6 +434,7 @@ dependencies = { module=all-target-libja
 dependencies = { module=all-target-libjava; on=all-target-libffi; };
 dependencies = { module=all-target-libobjc; on=all-target-libiberty; };
 dependencies = { module=all-target-libstdc++-v3; on=all-target-libiberty; };
+dependencies = { module=configure-target-libiberty; on=all-target-libunixlib; };
 
 // Target modules in the 'src' repository.
 lang_env_dependencies = { module=examples; };
