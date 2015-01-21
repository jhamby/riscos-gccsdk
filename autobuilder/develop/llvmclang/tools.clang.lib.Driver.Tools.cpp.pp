--- tools/clang/lib/Driver/Tools.cpp.orig	2014-08-07 05:51:51.000000000 +0100
+++ tools/clang/lib/Driver/Tools.cpp	2014-10-17 18:15:58.642875787 +0100
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
