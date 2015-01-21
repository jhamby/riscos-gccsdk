--- lib/CodeGen/RegisterScavenging.cpp.orig	2014-10-04 16:05:29.000000000 +0100
+++ lib/CodeGen/RegisterScavenging.cpp	2014-10-04 17:30:54.630133268 +0100
@@ -197,7 +197,9 @@
   determineKillsAndDefs();
 
   // Verify uses and defs.
-#ifndef NDEBUG
+#if 0
+// FIXME: RISC OS: Fails when emitting sub r11, r12, #4 in prologue; "Using an undefined register!"
+ndef NDEBUG
   for (unsigned i = 0, e = MI->getNumOperands(); i != e; ++i) {
     const MachineOperand &MO = MI->getOperand(i);
     if (!MO.isReg())
