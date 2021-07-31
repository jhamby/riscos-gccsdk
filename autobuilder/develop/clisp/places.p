--- src/places.lisp.orig	2006-04-19 09:11:17.000000000 +0100
+++ src/places.lisp	2006-10-22 19:49:50.000000000 +0100
@@ -582,6 +582,7 @@
 ;;;----------------------------------------------------------------------------
 (defsetf svref SYSTEM::SVSTORE)
 (defsetf row-major-aref system::row-major-store)
+(defsetf riscos::memoryref system::riscos-memorystore)
 ;;;----------------------------------------------------------------------------
 ;; Simplify a form, when its values are not needed, only its side effects.
 ;; Returns a list of subforms.
