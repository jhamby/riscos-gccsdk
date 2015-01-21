--- tools/clang/lib/Driver/ToolChains.h.orig	2014-08-27 21:07:13.000000000 +0100
+++ tools/clang/lib/Driver/ToolChains.h	2014-11-30 16:48:38.957408943 +0000
@@ -775,6 +775,15 @@
                            llvm::opt::ArgStringList &CmdArgs) const override;
 };
 
+class LLVM_LIBRARY_VISIBILITY RISCOS : public Generic_ELF {
+public:
+  RISCOS(const Driver &D, const llvm::Triple &Triple,
+         const llvm::opt::ArgList &Args);
+
+  std::string ComputeEffectiveClangTriple(const llvm::opt::ArgList &Args,
+                                          types::ID InputType) const override;
+};
+
 } // end namespace toolchains
 } // end namespace driver
 } // end namespace clang
