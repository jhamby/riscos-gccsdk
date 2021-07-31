--- tools/clang/lib/Driver/Tools.cpp.orig	2014-08-07 05:51:51.000000000 +0100
+++ tools/clang/lib/Driver/Tools.cpp	2015-02-02 19:39:49.556334155 +0000
@@ -628,6 +628,9 @@
         break;
       }
       break;
+    case llvm::Triple::RISCOS:
+      FloatABI = "soft";
+      break;
 
     default:
       switch(Triple.getEnvironment()) {
@@ -821,6 +824,21 @@
     CmdArgs.push_back("-backend-option");
     CmdArgs.push_back("-arm-reserve-r9");
   }
+
+  if (Triple.isOSRISCOS()) {
+    if (Args.hasFlag(options::OPT_mpoke_function_name,
+		     options::OPT_mno_poke_function_name,
+		     true)) {
+      CmdArgs.push_back("-backend-option");
+      CmdArgs.push_back("-embed-function-names");
+    }
+    if (Args.hasFlag(options::OPT_mapcs_stack_check,
+		     options::OPT_mno_apcs_stack_check,
+		     true)) {
+      CmdArgs.push_back("-backend-option");
+      CmdArgs.push_back("-apcs-stack-check");
+    }
+  }
 }
 
 /// getAArch64TargetCPU - Get the (LLVM) name of the AArch64 cpu we are
@@ -4954,9 +4972,17 @@
   if (!customGCCName.empty())
     GCCName = customGCCName.c_str();
   else if (D.CCCIsCXX()) {
+#ifdef __riscos__
+    GCCName = "GCCbin:g++";
+#else
     GCCName = "g++";
+#endif
   } else
+#ifdef __riscos__
+    GCCName = "GCCbin:gcc";
+#else
     GCCName = "gcc";
+#endif
 
   const char *Exec =
     Args.MakeArgString(getToolChain().GetProgramPath(GCCName));
