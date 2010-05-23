--- Makefile.tpl.orig	2010-03-19 23:23:58.873949956 +0100
+++ Makefile.tpl	2010-03-19 23:23:46.405314756 +0100
@@ -28,11 +28,11 @@ in
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
@@ -1407,7 +1407,7 @@ configure-target-[+module+]: maybe-all-g
 [+ ENDFOR target_modules +]
 
 [+ FOR lang_env_dependencies +]
-configure-target-[+module+]: maybe-all-target-newlib maybe-all-target-libgloss
+configure-target-[+module+]: maybe-all-target-libunixlib maybe-all-target-newlib maybe-all-target-libgloss
 [+ IF cxx +]configure-target-[+module+]: maybe-all-target-libstdc++-v3
 [+ ENDIF cxx +][+ ENDFOR lang_env_dependencies +]
 
