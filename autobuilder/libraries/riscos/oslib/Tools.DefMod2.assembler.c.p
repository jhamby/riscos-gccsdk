Index: Tools/DefMod2/assembler.c
===================================================================
--- Tools/DefMod2/assembler.c	(revision 455)
+++ Tools/DefMod2/assembler.c	(working copy)
@@ -1242,6 +1242,14 @@
 		       goto finish;
                      break;
 		   }
+                 case AsmType_eGNUEABIHF:
+		   {
+                     if ((rc = sprintf (cmd,
+				"arm-riscos-gnueabihf-gcc -c -xassembler -o %s %s",
+				name2, name1)) < 0)
+		       goto finish;
+                     break;
+		   }
                  default:
                    fprintf (stderr, "Unknown assembler type\n");
                    goto finish;
