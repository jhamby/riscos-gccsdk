--- src/init_base.lisp	2006-01-17 09:10:59.000000000 +0000
+++ src/init.lisp	2006-09-12 21:07:31.000000000 +0100
@@ -527,6 +527,7 @@
   (import clos-extra "EXT")
   (export clos-extra "EXT"))
 
+
 (in-package "SYSTEM")
 
 (use-package '("COMMON-LISP" "EXT") "CL-USER")
@@ -2160,6 +2161,8 @@
 
 (LOAD "clos")                   ; CLOS
 
+(LOAD "riscos")
+
 (LOAD "gray")                   ; STREAM-DEFINITION-BY-USER:GENERIC-FUNCTIONS
 
 (LOAD "fill-out")               ; FILL-OUTPUT-STREAM (for condition & describe)
