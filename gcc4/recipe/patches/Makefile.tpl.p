--- Makefile.tpl.orig	2007-08-31 01:21:32.000000000 +0200
+++ Makefile.tpl	2007-08-31 01:20:09.000000000 +0200
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
@@ -970,6 +970,15 @@
 [+ all prefix="target-" subdir="$(TARGET_SUBDIR)"
        exports="$(RAW_CXX_TARGET_EXPORTS)"
        args="$(TARGET_FLAGS_TO_PASS) 'CXX=$$(RAW_CXX_FOR_TARGET)' 'CXX_FOR_TARGET=$$(RAW_CXX_FOR_TARGET)'" +]
+[+ ELIF (= (get "module") "libscl") +]
+[+ configure prefix="target-" subdir="$(TARGET_SUBDIR)"
+	     deps=(string-append "$(TARGET_SUBDIR)/" (get "module") "/multilib.out")
+	     exports="$(subst /libunixlib/,/libscl/,$(NORMAL_TARGET_EXPORTS))"
+	     args="$(TARGET_CONFIGARGS)" no-config-site=true +]
+
+[+ all prefix="target-" subdir="$(TARGET_SUBDIR)"
+       exports="$(subst /libunixlib/,/libscl/,$(NORMAL_TARGET_EXPORTS))"
+       args="$(TARGET_FLAGS_TO_PASS)" +]
 [+ ELSE +]
 [+ configure prefix="target-" subdir="$(TARGET_SUBDIR)"
 	     deps=(string-append "$(TARGET_SUBDIR)/" (get "module") "/multilib.out")
@@ -1407,7 +1416,7 @@
 [+ ENDFOR target_modules +]
 
 [+ FOR lang_env_dependencies +]
-configure-target-[+module+]: maybe-all-target-newlib maybe-all-target-libgloss
+configure-target-[+module+]: maybe-all-target-libunixlib maybe-all-target-libscl maybe-all-target-newlib maybe-all-target-libgloss
 [+ IF cxx +]configure-target-[+module+]: maybe-all-target-libstdc++-v3
 [+ ENDIF cxx +][+ ENDFOR lang_env_dependencies +]
 
