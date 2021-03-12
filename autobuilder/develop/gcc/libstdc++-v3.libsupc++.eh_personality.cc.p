--- libstdc++-v3/libsupc++/eh_personality.cc.orig	2021-03-04 19:19:12.000000000 +0000
+++ libstdc++-v3/libsupc++/eh_personality.cc	2021-03-04 19:20:44.152062778 +0000
@@ -95,7 +95,7 @@
 
   i *= size_of_encoded_value (info->ttype_encoding);
   read_encoded_value_with_base (
-#if __FDPIC__
+#if __FDPIC__ || defined(__riscos__)
 				/* Force these flags to nake sure to
 				   take the GOT into account.  */
 				(DW_EH_PE_pcrel | DW_EH_PE_indirect),
