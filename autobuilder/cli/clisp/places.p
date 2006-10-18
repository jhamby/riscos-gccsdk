--- src/places_base.lisp	2006-04-19 09:11:17.000000000 +0100
+++ src/places.lisp	2006-09-12 21:44:48.000000000 +0100
@@ -583,6 +583,8 @@
 (defsetf svref SYSTEM::SVSTORE)
 (defsetf row-major-aref system::row-major-store)
 ;;;----------------------------------------------------------------------------
+(defsetf riscos-memoryref system::riscos-memorystore)
+;;;----------------------------------------------------------------------------
 ;; Simplify a form, when its values are not needed, only its side effects.
 ;; Returns a list of subforms.
 ;;   (values x y z) --> (x y z)
