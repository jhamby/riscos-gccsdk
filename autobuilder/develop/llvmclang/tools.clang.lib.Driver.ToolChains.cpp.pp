--- tools/clang/lib/Driver/ToolChains.cpp.orig	2014-08-27 21:07:31.000000000 +0100
+++ tools/clang/lib/Driver/ToolChains.cpp	2015-01-11 17:52:24.928166732 +0000
@@ -1322,7 +1322,8 @@
 
   static const char *const ARMLibDirs[] = { "/lib" };
   static const char *const ARMTriples[] = { "arm-linux-gnueabi",
-                                            "arm-linux-androideabi" };
+                                            "arm-linux-androideabi",
+                                            "arm-unknown-riscos" };
   static const char *const ARMHFTriples[] = { "arm-linux-gnueabihf",
                                               "armv7hl-redhat-linux-gnueabi" };
   static const char *const ARMebLibDirs[] = { "/lib" };
@@ -3601,3 +3602,15 @@
                                 ArgStringList &CmdArgs) const {
   // We don't output any lib args. This is handled by xcc.
 }
+
+RISCOS::RISCOS(const Driver &D, const llvm::Triple &Triple, const ArgList &Args)
+  : Generic_ELF(D, Triple, Args) {
+  GCCInstallation.init(D, Triple, Args);
+//  Multilibs = GCCInstallation.getMultilibs();
+}
+
+std::string RISCOS::ComputeEffectiveClangTriple(const ArgList &DriverArgs,
+                                                types::ID InputType) const {
+  llvm::Triple Triple(ComputeLLVMTriple(DriverArgs, InputType));
+  return Triple.getTriple();
+}
