--- lib/CodeGen/GlobalMerge.cpp.orig	2014-07-08 15:55:06.000000000 +0100
+++ lib/CodeGen/GlobalMerge.cpp	2015-03-12 01:15:00.096152972 +0000
@@ -151,13 +151,26 @@
   unsigned MaxOffset = TLI->getMaximalGlobalOffset();
 
   // FIXME: Find better heuristics
-  std::stable_sort(Globals.begin(), Globals.end(),
+  Triple TT(TM->getTargetTriple());
+  if (TT.isOSRISCOS()) {
+    std::stable_sort(Globals.begin(), Globals.end(),
                    [DL](const GlobalVariable *GV1, const GlobalVariable *GV2) {
-    Type *Ty1 = cast<PointerType>(GV1->getType())->getElementType();
-    Type *Ty2 = cast<PointerType>(GV2->getType())->getElementType();
+      Type *Ty1 = cast<PointerType>(GV1->getType())->getElementType();
+      Type *Ty2 = cast<PointerType>(GV2->getType())->getElementType();
 
-    return (DL->getTypeAllocSize(Ty1) < DL->getTypeAllocSize(Ty2));
-  });
+      // Order merged globals so that byte sized ones are at the end of the
+      // area and those that need to be word aligned are at the beginning.
+      return (DL->getTypeAllocSize(Ty1) > DL->getTypeAllocSize(Ty2));
+    });
+  } else {
+    std::stable_sort(Globals.begin(), Globals.end(),
+                   [DL](const GlobalVariable *GV1, const GlobalVariable *GV2) {
+      Type *Ty1 = cast<PointerType>(GV1->getType())->getElementType();
+      Type *Ty2 = cast<PointerType>(GV2->getType())->getElementType();
+
+      return (DL->getTypeAllocSize(Ty1) < DL->getTypeAllocSize(Ty2));
+    });
+  }
 
   Type *Int32Ty = Type::getInt32Ty(M.getContext());
 
