--- Source/JavaScriptCore/assembler/MacroAssemblerARM.cpp.orig	2015-02-17 04:57:12.000000000 +0000
+++ Source/JavaScriptCore/assembler/MacroAssemblerARM.cpp	2015-04-16 15:27:44.989596980 +0100
@@ -83,6 +83,9 @@
     ASSERT(address.scale >= 0 && address.scale <= 3);
     op2 = m_assembler.lsl(address.index, static_cast<int>(address.scale));
 
+#if OS(RISCOS)
+    if (Options::useHalfWord()) {
+#endif
     if (address.offset >= 0 && address.offset + 0x2 <= 0xff) {
         m_assembler.add(ARMRegisters::S0, address.base, op2);
         m_assembler.halfDtrUp(ARMAssembler::LoadUint16, dest, ARMRegisters::S0, ARMAssembler::getOp2Half(address.offset));
@@ -99,6 +102,44 @@
         m_assembler.halfDtrUpRegister(ARMAssembler::LoadUint16, ARMRegisters::S0, address.base, ARMRegisters::S0);
     }
     m_assembler.orr(dest, dest, m_assembler.lsl(ARMRegisters::S0, 16));
+#if OS(RISCOS)
+    } else {
+	// Don't use ARMAssembler::getOp2Byte() for the offset; it only works on data processing
+        // instructions. The meaning of bit 25 is reversed for data loads.
+        if (address.offset >= 0 && address.offset + 0x2 <= 0xff) {
+           m_assembler.add(ARMRegisters::S0, address.base, op2);
+	   m_assembler.dtrUp(ARMAssembler::LoadUint8, dest, ARMRegisters::S0, address.offset);
+           m_assembler.dtrUp(ARMAssembler::LoadUint8, ARMRegisters::S1, ARMRegisters::S0, address.offset + 0x1);
+           m_assembler.orr(dest, dest, m_assembler.lsl(ARMRegisters::S1, 8));
+           m_assembler.dtrUp(ARMAssembler::LoadUint8, ARMRegisters::S1, ARMRegisters::S0, address.offset + 0x2);
+           m_assembler.orr(dest, dest, m_assembler.lsl(ARMRegisters::S1, 16));
+           m_assembler.dtrUp(ARMAssembler::LoadUint8, ARMRegisters::S1, ARMRegisters::S0, address.offset + 0x3);
+           m_assembler.orr(dest, dest, m_assembler.lsl(ARMRegisters::S1, 24));
+       } else if (address.offset < 0 && address.offset >= -0xff) {
+           m_assembler.add(ARMRegisters::S0, address.base, op2);
+           m_assembler.dtrDown(ARMAssembler::LoadUint8, dest, ARMRegisters::S0, -address.offset);
+           m_assembler.dtrDown(ARMAssembler::LoadUint8, ARMRegisters::S1, ARMRegisters::S0, -address.offset - 0x1);
+           m_assembler.orr(dest, dest, m_assembler.lsl(ARMRegisters::S1, 8));
+           m_assembler.dtrDown(ARMAssembler::LoadUint8, ARMRegisters::S1, ARMRegisters::S0, -address.offset - 0x2);
+           m_assembler.orr(dest, dest, m_assembler.lsl(ARMRegisters::S1, 16));
+           m_assembler.dtrDown(ARMAssembler::LoadUint8, ARMRegisters::S1, ARMRegisters::S0, -address.offset - 0x3);
+           m_assembler.orr(dest, dest, m_assembler.lsl(ARMRegisters::S1, 24));
+       } else {
+           m_assembler.ldrUniqueImmediate(ARMRegisters::S0, address.offset);
+           m_assembler.add(ARMRegisters::S0, ARMRegisters::S0, op2);
+           m_assembler.dtrUpRegister(ARMAssembler::LoadUint8, dest, address.base, ARMRegisters::S0);
+           m_assembler.add(ARMRegisters::S0, ARMRegisters::S0, ARMAssembler::Op2Immediate | 0x1);
+           m_assembler.dtrUpRegister(ARMAssembler::LoadUint8, ARMRegisters::S1, address.base, ARMRegisters::S0);
+           m_assembler.orr(dest, dest, m_assembler.lsl(ARMRegisters::S1, 8));
+           m_assembler.add(ARMRegisters::S0, ARMRegisters::S0, ARMAssembler::Op2Immediate | 0x2);
+           m_assembler.dtrUpRegister(ARMAssembler::LoadUint8, ARMRegisters::S1, address.base, ARMRegisters::S0);
+           m_assembler.orr(dest, dest, m_assembler.lsl(ARMRegisters::S1, 16));
+           m_assembler.add(ARMRegisters::S0, ARMRegisters::S0, ARMAssembler::Op2Immediate | 0x3);
+           m_assembler.dtrUpRegister(ARMAssembler::LoadUint8, ARMRegisters::S1, address.base, ARMRegisters::S0);
+           m_assembler.orr(dest, dest, m_assembler.lsl(ARMRegisters::S1, 24));
+       }
+    }
+#endif
 }
 #endif
 
