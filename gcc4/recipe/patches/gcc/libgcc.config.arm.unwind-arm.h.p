Index: libgcc/config/arm/unwind-arm.h
===================================================================
--- libgcc/config/arm/unwind-arm.h	(revision 196237)
+++ libgcc/config/arm/unwind-arm.h	(working copy)
@@ -49,7 +49,7 @@
       if (!tmp)
 	return 0;
 
-#if (defined(linux) && !defined(__uClinux__)) || defined(__NetBSD__)
+#if (defined(linux) && !defined(__uClinux__)) || defined(__NetBSD__) || (defined(__riscos__) && !defined(__TARGET_MODULE__))
       /* Pc-relative indirect.  */
 #define _GLIBCXX_OVERRIDE_TTYPE_ENCODING (DW_EH_PE_pcrel | DW_EH_PE_indirect)
       tmp += ptr;
