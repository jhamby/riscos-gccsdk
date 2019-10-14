Index: Tools/DefMod2/assembler.h
===================================================================
--- Tools/DefMod2/assembler.h	(revision 455)
+++ Tools/DefMod2/assembler.h	(working copy)
@@ -22,8 +22,9 @@
   AsmType_eGCCAOF,	/* Assembler used in GCCSDK 3.4 (AOF) release, called
     'as' (predecessor of AsAsm).  */
   AsmType_eARMASM,	/* ARM's assembler.  */
-  AsmType_eGCCELF	/* Assembler used in GCCSDK 4 (ELF) release, called
+  AsmType_eGCCELF,	/* Assembler used in GCCSDK 4 (ELF) release, called
     'gas' (GNU binutils assembler).  */
+  AsmType_eGNUEABIHF	/* Assembler used in GCC 8 (ELF) release. In implied hard float. */
 } AsmType_e;
 
 typedef enum
@@ -37,7 +38,7 @@
 extern AsmType_e AsmType;
 extern FloatABI_e FloatABI;
   
-#define GASSyntax (AsmType == AsmType_eGCCELF)
+#define GASSyntax (AsmType == AsmType_eGCCELF || AsmType == AsmType_eGNUEABIHF)
 
 /*if |separate|, output is to the directory |output|; otherwise, to the file |file|.*/
 extern os_error *assembler_output (FILE *file, const char *title, const char *author, lookup_t types, lookup_t swis, osbool separate, osbool apcs32, char *output);
