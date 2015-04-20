--- Source/JavaScriptCore/assembler/MacroAssemblerARM.h.orig	2015-02-17 04:57:12.000000000 +0000
+++ Source/JavaScriptCore/assembler/MacroAssemblerARM.h	2015-04-16 15:18:10.137589531 +0100
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
 
@@ -832,15 +849,40 @@
         m_assembler.bkpt(0);
     }
 
+#if OS(RISCOS)
+    void breakpoint_riscos(ARMWord id = 0)
+    {
+      m_assembler.breakpoint_swi(id);
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
@@ -850,7 +892,15 @@
 
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
@@ -968,9 +1018,21 @@
 
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
@@ -1357,7 +1419,15 @@
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
