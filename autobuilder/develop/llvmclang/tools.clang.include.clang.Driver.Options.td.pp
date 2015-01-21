--- tools/clang/include/clang/Driver/Options.td.orig	2014-08-07 05:51:51.000000000 +0100
+++ tools/clang/include/clang/Driver/Options.td	2014-10-09 11:28:01.831226183 +0100
@@ -1136,6 +1136,15 @@
 def mno_long_calls : Flag<["-"], "mno-long-calls">, Group<m_arm_Features_Group>,
   HelpText<"Restore the default behaviour of not generating long calls">;
 
+def mpoke_function_name : Flag<["-"], "mpoke-function-name">, Group<m_arm_Features_Group>,
+  HelpText<"Embed function names in object code before each function">;
+def mno_poke_function_name : Flag<["-"], "mno-poke-function-name">, Group<m_arm_Features_Group>,
+  HelpText<"Do not embed function names in object code before each function">;
+def mapcs_stack_check : Flag<["-"], "mapcs-stack-check">, Group<m_arm_Features_Group>,
+  HelpText<"Use stack size checking">;
+def mno_apcs_stack_check : Flag<["-"], "mno-apcs-stack-check">, Group<m_arm_Features_Group>,
+  HelpText<"Do not use stack size checking">;
+
 def mgeneral_regs_only : Flag<["-"], "mgeneral-regs-only">, Group<m_aarch64_Features_Group>,
   HelpText<"Generate code which only uses the general purpose registers (AArch64 only)">;
 
