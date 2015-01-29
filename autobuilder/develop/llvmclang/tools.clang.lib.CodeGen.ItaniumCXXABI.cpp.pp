--- tools/clang/lib/CodeGen/ItaniumCXXABI.cpp.orig	2015-01-28 21:20:05.000000000 +0000
+++ tools/clang/lib/CodeGen/ItaniumCXXABI.cpp	2015-01-29 15:34:29.839669907 +0000
@@ -282,7 +282,17 @@
                   /* UseARMGuardVarABI = */ true) {}
 
   bool HasThisReturn(GlobalDecl GD) const override {
-    return (isa<CXXConstructorDecl>(GD.getDecl()) || (
+    if (CGM.getTarget().getTriple().getOS() == llvm::Triple::RISCOS) {
+      // The ARM EABI declares that constructors should return the 'this' pointer.
+      // Clang adheres to this by default, meaning that it does not preserve 'this'
+      // across a call to a constructor. However, RISCOS GCC 4.7.4 does not return
+      // 'this' from a constructor (although it can and probably should) and does
+      // preserve 'this' across a constructor call.
+      // Here we match the behaviour of GCC to be compatible.
+      return false;
+    }
+    else
+      return (isa<CXXConstructorDecl>(GD.getDecl()) || (
               isa<CXXDestructorDecl>(GD.getDecl()) &&
               GD.getDtorType() != Dtor_Deleting));
   }
