--- lib/CodeGen/AsmPrinter/EHStreamer.cpp.orig	2014-06-11 02:19:03.000000000 +0100
+++ lib/CodeGen/AsmPrinter/EHStreamer.cpp	2014-12-21 23:25:39.976042013 +0000
@@ -425,7 +425,10 @@
   // Begin the exception table.
   // Sometimes we want not to emit the data into separate section (e.g. ARM
   // EHABI). In this case LSDASection will be NULL.
-  if (LSDASection)
+  // RISCOS: Despite using ARM EHABI, we still see the change of section below.
+  // This causes the exception data to be split up which the handler code
+  // in libstdc++/libgcc isn't expecting.
+  if (LSDASection && !MMI->getContext().getObjectFileInfo()->getTargetTriple().getOS() == Triple::RISCOS)
     Asm->OutStreamer.SwitchSection(LSDASection);
   Asm->EmitAlignment(2);
 
