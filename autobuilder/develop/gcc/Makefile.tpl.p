--- Makefile.tpl.orig	2016-04-13 19:02:08.000000000 +0100
+++ Makefile.tpl	2016-10-25 17:44:24.493363493 +0100
@@ -1894,7 +1894,7 @@
 @endif gcc-no-bootstrap
 
 [+ FOR target_modules +][+ IF (not (lang-dep "no_c")) +]
-configure-target-[+module+]: maybe-all-target-newlib maybe-all-target-libgloss[+
+configure-target-[+module+]: maybe-all-target-libunixlib maybe-all-target-newlib maybe-all-target-libgloss[+
   ENDIF +][+ IF (lang-dep "cxx") +]
 configure-target-[+module+]: maybe-all-target-libstdc++-v3[+
   ENDIF +]
