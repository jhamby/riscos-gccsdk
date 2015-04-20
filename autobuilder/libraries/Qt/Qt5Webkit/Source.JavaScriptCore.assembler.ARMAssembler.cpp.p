--- Source/JavaScriptCore/assembler/ARMAssembler.cpp.orig	2015-02-17 04:57:12.000000000 +0000
+++ Source/JavaScriptCore/assembler/ARMAssembler.cpp	2015-04-16 15:31:34.701599957 +0100
@@ -310,6 +310,9 @@
 
 void ARMAssembler::dataTransfer16(DataTransferTypeB transferType, RegisterID srcDst, RegisterID base, int32_t offset)
 {
+#if OS(RISCOS)
+  if (Options::useHalfWord()) {
+#endif
     if (offset >= 0) {
         if (offset <= 0xff)
             halfDtrUp(transferType, srcDst, base, getOp2Half(offset));
@@ -331,6 +334,44 @@
             halfDtrUpRegister(transferType, srcDst, base, ARMRegisters::S0);
         }
     }
+#if OS(RISCOS)
+  } else {
+    // NOTE: Don't use ARMAssembler::getOp2Byte() for the offset; it only works on data processing
+    // instructions. The meaning of bit 25 is reversed for data loads.
+    if (offset >= 0) {
+        if (offset <= 0xff) {
+	    ASSERT(transferType == LoadUint16);
+	    // NOTE: Visually verified as producing correct code (loads only)
+	    dtrUp(LoadUint8, srcDst, base, offset);
+	    dtrUp(LoadUint8, ARMRegisters::S1, base, offset + 0x1);
+	    orr(srcDst, srcDst, lsl(ARMRegisters::S1, 8));
+//            halfDtrUp(transferType, srcDst, base, getOp2Half(offset));
+	}
+        else if (offset <= 0xffff) {
+            add(ARMRegisters::S0, base, Op2Immediate | (offset >> 8) | (12 << 8));
+            halfDtrUp(transferType, srcDst, ARMRegisters::S0, getOp2Half(offset & 0xff));
+        } else {
+            moveImm(offset, ARMRegisters::S0);
+            halfDtrUpRegister(transferType, srcDst, base, ARMRegisters::S0);
+        }
+    } else {
+        if (offset >= -0xff) {
+	    ASSERT(transferType == LoadUint16);
+	    dtrDown(LoadUint8, srcDst, base, -offset);
+	    dtrDown(LoadUint8, ARMRegisters::S1, base, -offset - 0x1);
+	    orr(srcDst, srcDst, lsl(ARMRegisters::S1, 8));
+//            halfDtrDown(transferType, srcDst, base, getOp2Half(-offset));
+	}
+        else if (offset >= -0xffff) {
+            sub(ARMRegisters::S0, base, Op2Immediate | (-offset >> 8) | (12 << 8));
+            halfDtrDown(transferType, srcDst, ARMRegisters::S0, getOp2Half(-offset & 0xff));
+        } else {
+            moveImm(offset, ARMRegisters::S0);
+            halfDtrUpRegister(transferType, srcDst, base, ARMRegisters::S0);
+        }
+    }
+  }
+#endif
 }
 
 void ARMAssembler::baseIndexTransfer16(DataTransferTypeB transferType, RegisterID srcDst, RegisterID base, RegisterID index, int scale, int32_t offset)
