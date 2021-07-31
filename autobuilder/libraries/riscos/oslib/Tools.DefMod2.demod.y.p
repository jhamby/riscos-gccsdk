Index: Tools/DefMod2/defmod.y
===================================================================
--- Tools/DefMod2/defmod.y	(revision 455)
+++ Tools/DefMod2/defmod.y	(working copy)
@@ -1179,7 +1179,7 @@
                      "[-o <output-dir> | > <output-file>] "
                      "[-byte_wide <byte-wide-file>] "
                      "[-26bit | -32bit] "
-                     "[-asmtype [objasm | asasmaof | asasmelf | gccaof | armasm | gccelf]] "
+                     "[-asmtype [objasm | asasmaof | asasmelf | gccaof | armasm | gccelf | gnueabihf]] "
 		     "[-float-abi [soft | hard]] "
                      "< <module-interface-file>\r"
                "Purpose: generate output from a module interface file\r"
@@ -1203,6 +1203,7 @@
                "\tgccaof\t\t"       "...use GCC AOF toolkit (GCCSDK 3.4 only)\r"
                "\tarmasm\t\t"       "...use ARM ASM\r"
                "\tgccelf\t\t"       "...use GCC ELF toolkit (GCCSDK 4 only)\r"
+               "\tgnueabihf\t\t"    "...use GCC ELF toolkit targeting ARMEABIHF\r"
 	       "-float-abi\t\t"     "marks object files which float-abi is in use\r"
 	       "\tsoft\t\t"         "...soft-float, no FP instructions used\r"
 	       "\thard\t\t"         "...hard-float, FP instructions can be used\r"
@@ -1345,6 +1346,8 @@
             AsmType = AsmType_eARMASM;
          else if (strcmp (argv[i], "gccelf") == 0)
             AsmType = AsmType_eGCCELF;
+         else if (strcmp (argv[i], "gnueabihf") == 0)
+            AsmType = AsmType_eGNUEABIHF;
          else
          {
             fprintf (stderr, "Unknown option value %s for -asmtype (only \"objasm\", \"asasmaof\", \"asasmelf\", \"armarm\", \"gccaof\" and \"gccelf\" are known)\n", argv[i]);
