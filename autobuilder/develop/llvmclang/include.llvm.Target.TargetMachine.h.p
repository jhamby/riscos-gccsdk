--- include/llvm/Target/TargetMachine.h.orig	2014-05-21 00:59:50.000000000 +0100
+++ include/llvm/Target/TargetMachine.h	2014-12-24 12:00:06.123460101 +0000
@@ -260,6 +260,8 @@
   void getNameWithPrefix(SmallVectorImpl<char> &Name, const GlobalValue *GV,
                          Mangler &Mang, bool MayAlwaysUsePrivate = false) const;
   MCSymbol *getSymbol(const GlobalValue *GV, Mangler &Mang) const;
+
+  bool hasBigEndianDoubles() const;
 };
 
 /// LLVMTargetMachine - This class describes a target machine that is
