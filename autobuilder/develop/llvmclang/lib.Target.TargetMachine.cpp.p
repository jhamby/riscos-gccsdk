--- lib/Target/TargetMachine.cpp.orig	2014-05-28 19:15:43.000000000 +0100
+++ lib/Target/TargetMachine.cpp	2014-12-24 11:48:35.131440284 +0000
@@ -196,3 +196,8 @@
       getTargetLowering()->getObjFileLowering();
   return TLOF.getContext().GetOrCreateSymbol(NameStr.str());
 }
+
+bool TargetMachine::hasBigEndianDoubles() const
+{
+  return AsmInfo->hasBigEndianDoubles();
+}
