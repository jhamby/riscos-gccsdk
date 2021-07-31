--- Source/JavaScriptCore/assembler/ARMAssembler.h.orig	2015-06-29 21:11:04.000000000 +0100
+++ Source/JavaScriptCore/assembler/ARMAssembler.h	2016-01-25 15:38:40.870688381 +0000
@@ -32,6 +32,11 @@
 #include "AssemblerBufferWithConstantPool.h"
 #include "JITCompilationEffort.h"
 #include <wtf/Assertions.h>
+
+#if OS(RISCOS)
+extern "C" void __clear_cache (void *beg, void *end);
+#endif
+
 namespace JSC {
 
     typedef uint32_t ARMWord;
@@ -637,6 +642,20 @@
             m_buffer.putInt(BKPT | ((value & 0xff0) << 4) | (value & 0xf));
         }
 
+#if OS(RISCOS)
+        void swi(ARMWord swi_num, bool x_bit = 0)
+        {
+            m_buffer.putInt(0xef000000 | swi_num | (x_bit << 20));
+        }
+
+        AssemblerLabel blx_v4(int rm)
+        {
+            mov(ARMRegisters::lr, ARMRegisters::pc);
+            mov(ARMRegisters::pc, rm);
+           return m_buffer.label();
+        }
+#endif
+
         void nop()
         {
             m_buffer.putInt(NOP);
@@ -781,9 +800,20 @@
         {
             // Check for call
             if ((*insn & LdrPcImmediateInstructionMask) != LdrPcImmediateInstruction) {
+#if OS(RISCOS)
+		if (Options::useBX()) {
+		    // Must be BLX
+		    ASSERT((*insn & BlxInstructionMask) == BlxInstruction);
+		    insn--;
+		}
+		else {
+		    insn-=2;
+		}
+#else
                 // Must be BLX
                 ASSERT((*insn & BlxInstructionMask) == BlxInstruction);
                 insn--;
+#endif
             }
 
             // Must be an ldr ..., [pc +/- imm]
@@ -1069,6 +1099,8 @@
             UNUSED_PARAM(size);
 #elif OS(QNX)
             msync(code, size, MS_INVALIDATE_ICACHE);
+#elif OS(RISCOS)
+	    __clear_cache (code, (char *)code + size);
 #else
 #error "The cacheFlush support is missing on this platform."
 #endif
