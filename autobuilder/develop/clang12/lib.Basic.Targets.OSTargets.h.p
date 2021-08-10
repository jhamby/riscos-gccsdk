--- lib/Basic/Targets/OSTargets.h.orig	2021-06-28 09:23:38.000000000 -0700
+++ lib/Basic/Targets/OSTargets.h	2021-08-09 17:21:11.779247871 -0700
@@ -580,6 +580,33 @@
   }
 };
 
+// RISC OS Target
+template <typename Target>
+class LLVM_LIBRARY_VISIBILITY RISCOSTargetInfo : public OSTargetInfo<Target> {
+protected:
+  void getOSDefines(const LangOptions &Opts, const llvm::Triple &Triple,
+                    MacroBuilder &Builder) const override {
+    // RISC OS defines from gcc/config/arm/riscos-gnueabihf.h
+    Builder.defineMacro("__ELF__");
+    Builder.defineMacro("__riscos");
+    Builder.defineMacro("__riscos__");
+    Builder.defineMacro("__TARGET_UNIXLIB__");
+    if (Opts.CPlusPlus)
+      Builder.defineMacro("_GNU_SOURCE");
+  }
+
+public:
+  RISCOSTargetInfo(const llvm::Triple &Triple, const TargetOptions &Opts)
+      : OSTargetInfo<Target>(Triple, Opts) {
+    switch (Triple.getArch()) {
+    default:
+    case llvm::Triple::arm:
+      // TODO: anything to customize?
+      break;
+    }
+  }
+};
+
 // RTEMS Target
 template <typename Target>
 class LLVM_LIBRARY_VISIBILITY RTEMSTargetInfo : public OSTargetInfo<Target> {
