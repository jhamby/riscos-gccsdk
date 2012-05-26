--- mono/mini/cpu-arm.md.orig	2011-12-19 21:10:25.000000000 +0000
+++ mono/mini/cpu-arm.md	2012-05-25 21:38:22.000000000 +0100
@@ -110,23 +110,23 @@
 storer8_membase_reg: dest:b src1:f len:24
 store_memindex: dest:b src1:i src2:i len:4
 storei1_memindex: dest:b src1:i src2:i len:4
-storei2_memindex: dest:b src1:i src2:i len:4
+storei2_memindex: dest:b src1:i src2:i len:16
 storei4_memindex: dest:b src1:i src2:i len:4
 load_membase: dest:i src1:b len:20
-loadi1_membase: dest:i src1:b len:4
+loadi1_membase: dest:i src1:b len:12
 loadu1_membase: dest:i src1:b len:4
-loadi2_membase: dest:i src1:b len:4
-loadu2_membase: dest:i src1:b len:4
+loadi2_membase: dest:i src1:b len:20
+loadu2_membase: dest:i src1:b len:12
 loadi4_membase: dest:i src1:b len:4
 loadu4_membase: dest:i src1:b len:4
 loadi8_membase: dest:i src1:b
 loadr4_membase: dest:f src1:b len:8
 loadr8_membase: dest:f src1:b len:24
 load_memindex: dest:i src1:b src2:i len:4
-loadi1_memindex: dest:i src1:b src2:i len:4
+loadi1_memindex: dest:i src1:b src2:i len:12
 loadu1_memindex: dest:i src1:b src2:i len:4
-loadi2_memindex: dest:i src1:b src2:i len:4
-loadu2_memindex: dest:i src1:b src2:i len:4
+loadi2_memindex: dest:i src1:b src2:i len:24
+loadu2_memindex: dest:i src1:b src2:i len:16
 loadi4_memindex: dest:i src1:b src2:i len:4
 loadu4_memindex: dest:i src1:b src2:i len:4
 loadu4_mem: dest:i len:8
