--- Makefile.tpl.orig	2007-07-09 03:04:22.000000000 +0200
+++ Makefile.tpl	2007-07-09 02:26:26.000000000 +0200
@@ -28,11 +28,11 @@
 # -------------------------------
 VPATH=@srcdir@
 
-build_alias=@build_alias@
+build_alias=@build_noncanonical@
 build=@build@
-host_alias=@host_alias@
+host_alias=@host_noncanonical@
 host=@host@
-target_alias=@target_alias@
+target_alias=@target_noncanonical@
 target=@target@
 
 program_transform_name = @program_transform_name@
@@ -1407,7 +1411,7 @@
 [+ ENDFOR target_modules +]
 
 [+ FOR lang_env_dependencies +]
-configure-target-[+module+]: maybe-all-target-newlib maybe-all-target-libgloss
+configure-target-[+module+]: maybe-all-target-libunixlib maybe-all-target-libscl maybe-all-target-newlib maybe-all-target-libgloss
 [+ IF cxx +]configure-target-[+module+]: maybe-all-target-libstdc++-v3
 [+ ENDIF cxx +][+ ENDFOR lang_env_dependencies +]
 
