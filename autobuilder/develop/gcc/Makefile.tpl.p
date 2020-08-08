--- Makefile.tpl.orig	2020-07-23 07:35:16.912379792 +0100
+++ Makefile.tpl	2020-07-24 17:16:31.201911500 +0100
@@ -1968,7 +1968,7 @@
 @endif gcc-no-bootstrap
 
 [+ FOR target_modules +][+ IF (not (lang-dep "no_c")) +]
-configure-target-[+module+]: maybe-all-target-newlib maybe-all-target-libgloss[+
+configure-target-[+module+]: maybe-all-target-libunixlib maybe-all-target-newlib maybe-all-target-libgloss[+
   ENDIF +][+ IF (lang-dep "cxx") +]
 configure-target-[+module+]: maybe-all-target-libstdc++-v3[+
   ENDIF +]
