--- mono/mini/patch-info.h.orig	2013-03-08 04:31:48.000000000 +0000
+++ mono/mini/patch-info.h	2013-04-14 14:54:09.000000000 +0100
@@ -47,5 +47,6 @@
 PATCH_INFO(SIGNATURE, "signature")
 PATCH_INFO(GSHAREDVT_CALL, "gsharedvt_call")
 PATCH_INFO(JIT_TLS_ID, "jit_tls_id")
+PATCH_INFO(RISCOS_STKOVF, "riscos_stkovf")
 PATCH_INFO(NONE, "none")
 
