--- Source/JavaScriptCore/assembler/ARMAssembler.h.orig	2015-02-17 04:57:12.000000000 +0000
+++ Source/JavaScriptCore/assembler/ARMAssembler.h	2015-04-16 16:13:23.093632462 +0100
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
@@ -637,6 +642,22 @@
             m_buffer.putInt(BKPT | ((value & 0xff0) << 4) | (value & 0xf));
         }
 
+#if OS(RISCOS)
+        void breakpoint_swi(ARMWord id = 0)
+        {
+	    m_buffer.putInt(0xef000017);
+            // The breakpoint handler will skip this.
+	    m_buffer.putInt(id);
+        }
+
+        AssemblerLabel blx_v4(int rm)
+	{
+	    mov(ARMRegisters::lr, ARMRegisters::pc);
+	    mov(ARMRegisters::pc, rm);
+	    return m_buffer.label();
+	}
+#endif
+
         void nop()
         {
             m_buffer.putInt(NOP);
@@ -781,9 +802,20 @@
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
@@ -1069,6 +1101,8 @@
             UNUSED_PARAM(size);
 #elif OS(QNX)
             msync(code, size, MS_INVALIDATE_ICACHE);
+#elif OS(RISCOS)
+	    __clear_cache (code, (char *)code + size);
 #else
 #error "The cacheFlush support is missing on this platform."
 #endif
