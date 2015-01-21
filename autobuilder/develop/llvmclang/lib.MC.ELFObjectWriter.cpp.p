--- lib/MC/ELFObjectWriter.cpp.orig	2014-07-21 00:15:06.000000000 +0100
+++ lib/MC/ELFObjectWriter.cpp	2015-01-19 21:32:50.012043074 +0000
@@ -35,6 +35,9 @@
 #include "llvm/Support/ELF.h"
 #include "llvm/Support/ErrorHandling.h"
 #include <vector>
+
+#include "llvm/MC/MCObjectFileInfo.h"
+
 using namespace llvm;
 
 #undef  DEBUG_TYPE
@@ -461,8 +464,29 @@
   WriteWord(SectionDataSize + (is64Bit() ? sizeof(ELF::Elf64_Ehdr) :
             sizeof(ELF::Elf32_Ehdr)));  // e_shoff = sec hdr table off in bytes
 
-  // e_flags = whatever the target wants
-  Write32(Asm.getELFHeaderEFlags());
+  {
+    Triple triple = Asm.getContext().getObjectFileInfo()->getTargetTriple();
+    if (triple.getOS() == Triple::RISCOS) {
+      if (Asm.getContext().getObjectFileInfo()->getRelocM() == Reloc::PIC_) {
+        // FIXME: There must be a better place to do this, but finding a place where
+        // the state of fPIC is known is difficult.
+        //
+        // Can't find EF_ARM_PIC defined anywhere, and GCC/GAS sets it for fPIC.
+        // If it's not set, __GOTT_BASE__ gains a text relocation (presumably ld is
+        // responsible for this). At this point I can't determine if fPIC has been
+        // selected. The relocation model can be found in MCObjectFileInfo via
+        // S->getAssembler().getContext().getObjectFileInfo()->getRelocM(), but
+        // unfortunately, it hasn't been initialised yet. TargetMachine seems to
+        // be the only other object that has this info, but there no access to such
+        // an object here.
+#define EF_ARM_PIC 0x20
+        Write32(Asm.getELFHeaderEFlags() | EF_ARM_PIC);
+      } else {
+        // e_flags = whatever the target wants
+        Write32(Asm.getELFHeaderEFlags());
+      }
+    }
+  }
 
   // e_ehsize = ELF header size
   Write16(is64Bit() ? sizeof(ELF::Elf64_Ehdr) : sizeof(ELF::Elf32_Ehdr));
@@ -988,6 +1012,14 @@
   }
 }
 
+static void makeSymbolWeak(const StringRef &Name, MCAssembler &Asm)
+{
+  MCSymbol *Sym = Asm.getContext().GetOrCreateSymbol(Name);
+  MCSymbolData &Data = Asm.getOrCreateSymbolData(*Sym);
+  Data.setExternal(true);
+  MCELF::SetBinding(Data, ELF::STB_WEAK);
+}
+
 void
 ELFObjectWriter::computeSymbolTable(MCAssembler &Asm, const MCAsmLayout &Layout,
                                     const SectionIndexMapTy &SectionIndexMap,
@@ -996,11 +1028,17 @@
   // FIXME: Is this the correct place to do this?
   // FIXME: Why is an undefined reference to _GLOBAL_OFFSET_TABLE_ needed?
   if (NeedsGOT) {
-    StringRef Name = "_GLOBAL_OFFSET_TABLE_";
-    MCSymbol *Sym = Asm.getContext().GetOrCreateSymbol(Name);
-    MCSymbolData &Data = Asm.getOrCreateSymbolData(*Sym);
-    Data.setExternal(true);
-    MCELF::SetBinding(Data, ELF::STB_GLOBAL);
+    Triple triple = Asm.getContext().getObjectFileInfo()->getTargetTriple();
+    if (triple.getOS() == Triple::RISCOS) {
+      makeSymbolWeak(StringRef("__GOTT_BASE__"), Asm);
+      makeSymbolWeak(StringRef("__GOTT_INDEX__"), Asm);
+    } else {
+      StringRef Name = "_GLOBAL_OFFSET_TABLE_";
+      MCSymbol *Sym = Asm.getContext().GetOrCreateSymbol(Name);
+      MCSymbolData &Data = Asm.getOrCreateSymbolData(*Sym);
+      Data.setExternal(true);
+      MCELF::SetBinding(Data, ELF::STB_GLOBAL);
+    }
   }
 
   // Add the data for the symbols.
