--- tools/clang/lib/Basic/Targets.cpp.orig	2014-08-08 23:59:37.000000000 +0100
+++ tools/clang/lib/Basic/Targets.cpp	2015-02-19 18:52:52.078705140 +0000
@@ -3495,6 +3495,8 @@
   static const Builtin::Info BuiltinInfo[];
 
   static bool shouldUseInlineAtomic(const llvm::Triple &T) {
+    if (T.getOS() == llvm::Triple::RISCOS)
+      return true;
     StringRef ArchName = T.getArchName();
     if (T.getArch() == llvm::Triple::arm ||
         T.getArch() == llvm::Triple::armeb) {
@@ -6012,6 +6014,29 @@
 };
 } // end anonymous namespace.
 
+namespace {
+template<typename Target>
+class RISCOSTargetInfo : public OSTargetInfo<Target> {
+protected:
+  void getOSDefines(const LangOptions &Opts, const llvm::Triple &Triple,
+		    MacroBuilder &Builder) const override {
+    Builder.defineMacro("__riscos__");
+    Builder.defineMacro("__riscos");
+    Builder.defineMacro("__ELF__");
+    if (Opts.CPlusPlus)
+      Builder.defineMacro("_GNU_SOURCE");
+  }
+public:
+  RISCOSTargetInfo(const llvm::Triple &Triple) : OSTargetInfo<Target>(Triple) {
+    this->UserLabelPrefix = "";
+    // ptrdiff_t is defined as being of type long in GCC and libstdc++, but defaults
+    // to int here for an ARM target. We have to define it as long too to ensure
+    // functions that use ptrdiff_t as an argument type will have signatures that
+    // match.
+    this->PtrDiffType = TargetInfo::SignedLong;
+  }
+};
+} // end anonymous namespace.
 
 //===----------------------------------------------------------------------===//
 // Driver code
@@ -6056,6 +6081,8 @@
     }
 
   case llvm::Triple::arm:
+    if (os == llvm::Triple::RISCOS)
+      return new RISCOSTargetInfo<ARMleTargetInfo>(Triple);
   case llvm::Triple::thumb:
     if (Triple.isOSBinFormatMachO())
       return new DarwinARMTargetInfo(Triple);
