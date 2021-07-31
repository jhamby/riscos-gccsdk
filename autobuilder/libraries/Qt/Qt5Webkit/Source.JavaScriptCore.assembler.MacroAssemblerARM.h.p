--- Source/JavaScriptCore/assembler/MacroAssemblerARM.h.orig	2015-06-29 21:11:04.000000000 +0100
+++ Source/JavaScriptCore/assembler/MacroAssemblerARM.h	2016-01-24 17:46:56.713294131 +0000
@@ -27,6 +27,7 @@
 
 #ifndef MacroAssemblerARM_h
 #define MacroAssemblerARM_h
+#include <unixlib/local.h>
 
 #if ENABLE(ASSEMBLER) && CPU(ARM_TRADITIONAL)
 
@@ -661,7 +662,15 @@
 
     void jump(RegisterID target)
     {
+#if OS(RISCOS)
+        if (Options::useBX()) {
+            m_assembler.bx(target);
+        } else {
+            m_assembler.mov(ARMRegisters::pc, target);
+	}
+#else
         m_assembler.bx(target);
+#endif
     }
 
     void jump(Address address)
@@ -823,7 +832,15 @@
     {
         internalCompare32(reg, imm);
         Jump jump(m_assembler.loadBranchTarget(ARMRegisters::S1, ARMCondition(cond), true));
+#if OS(RISCOS)
+        if (Options::useBX()) {
+	    m_assembler.bx(ARMRegisters::S1, ARMCondition(cond));
+	} else {
+	    m_assembler.mov(ARMRegisters::pc, ARMRegisters::S1, ARMCondition(cond));
+	}
+#else
         m_assembler.bx(ARMRegisters::S1, ARMCondition(cond));
+#endif
         return PatchableJump(jump);
     }
 
@@ -832,15 +849,60 @@
         m_assembler.bkpt(0);
     }
 
+#if OS(RISCOS)
+    void breakpoint_riscos(ARMWord id = 0)
+    {
+        ASSERT((id & 0xfffff000) == 0);
+        m_assembler.swi(0x17);
+        // The breakpoint handler will skip this.
+        m_assembler.bkpt(id);
+    }
+
+    void report_hex(RegisterID reg)
+    {
+        m_assembler.push(ARMRegisters::r0);
+        m_assembler.push(ARMRegisters::r1);
+        m_assembler.push(ARMRegisters::r2);
+	move(reg, ARMRegisters::r0);
+	sub32(ARMRegisters::sp, TrustedImm32(12), ARMRegisters::r1);
+	move(TrustedImm32(12), ARMRegisters::r2);
+	m_assembler.swi(0xD4);
+	move(TrustedImm32(0), ARMRegisters::r2);
+        m_assembler.dtrUp(ARMAssembler::StoreUint8, ARMRegisters::r2, ARMRegisters::r1, 0);
+	m_assembler.swi(0x54c80);
+        m_assembler.pop(ARMRegisters::r2);
+        m_assembler.pop(ARMRegisters::r1);
+        m_assembler.pop(ARMRegisters::r0);
+    }
+#endif
+
     Call nearCall()
     {
+#if OS(RISCOS)
+	ensureSpace(3 * sizeof(ARMWord), sizeof(ARMWord));
+        m_assembler.loadBranchTarget(ARMRegisters::S1, ARMAssembler::AL, true);
+        if (Options::useBX()) {
+	    return Call(m_assembler.blx(ARMRegisters::S1), Call::LinkableNear);
+	} else {
+	    return Call(m_assembler.blx_v4(ARMRegisters::S1), Call::LinkableNear);
+	}
+#else
         m_assembler.loadBranchTarget(ARMRegisters::S1, ARMAssembler::AL, true);
         return Call(m_assembler.blx(ARMRegisters::S1), Call::LinkableNear);
+#endif
     }
 
     Call call(RegisterID target)
     {
+#if OS(RISCOS)
+        if (Options::useBX()) {
+	    return Call(m_assembler.blx(target), Call::None);
+	} else {
+	    return Call(m_assembler.blx_v4(target), Call::None);
+	}
+#else
         return Call(m_assembler.blx(target), Call::None);
+#endif
     }
 
     void call(Address address)
@@ -850,7 +912,15 @@
 
     void ret()
     {
+#if OS(RISCOS)
+	if (Options::useBX()) {
+	    m_assembler.bx(linkRegister);
+	} else {
+	    m_assembler.mov(ARMRegisters::pc, linkRegister);
+	}
+#else
         m_assembler.bx(linkRegister);
+#endif
     }
 
     void compare32(RelationalCondition cond, RegisterID left, RegisterID right, RegisterID dest)
@@ -968,9 +1038,21 @@
 
     Call call()
     {
+#if OS(RISCOS)
+	if (Options::useBX()) {
+	    ensureSpace(2 * sizeof(ARMWord), sizeof(ARMWord));
+	    m_assembler.loadBranchTarget(ARMRegisters::S1, ARMAssembler::AL, true);
+	    return Call(m_assembler.blx(ARMRegisters::S1), Call::Linkable);
+	} else {
+	    ensureSpace(3 * sizeof(ARMWord), sizeof(ARMWord));	    
+	    m_assembler.loadBranchTarget(ARMRegisters::S1, ARMAssembler::AL, true);
+	    return Call(m_assembler.blx_v4(ARMRegisters::S1), Call::Linkable);
+	}
+#else
         ensureSpace(2 * sizeof(ARMWord), sizeof(ARMWord));
         m_assembler.loadBranchTarget(ARMRegisters::S1, ARMAssembler::AL, true);
         return Call(m_assembler.blx(ARMRegisters::S1), Call::Linkable);
+#endif
     }
 
     Call tailRecursiveCall()
@@ -1357,7 +1439,15 @@
     void call32(RegisterID base, int32_t offset)
     {
         load32(Address(base, offset), ARMRegisters::S1);
+#if OS(RISCOS)
+        if (Options::useBX()) {
+	    m_assembler.blx(ARMRegisters::S1);
+	} else {
+	    m_assembler.blx_v4(ARMRegisters::S1);
+	}
+#else
         m_assembler.blx(ARMRegisters::S1);
+#endif
     }
 
 private:
