--- bfd/elf32-arm.c.orig	2006-11-15 20:22:20.000000000 +0000
+++ bfd/elf32-arm.c	2006-11-15 20:22:10.000000000 +0000
@@ -1470,7 +1470,7 @@
 
 /* The name of the dynamic interpreter.  This is put in the .interp
    section.  */
-#define ELF_DYNAMIC_INTERPRETER     "/usr/lib/ld.so.1"
+#define ELF_DYNAMIC_INTERPRETER     "ld-riscos/so/1"
 
 #ifdef FOUR_WORD_PLT
 
@@ -1570,6 +1570,31 @@
     0x00000000,         /* dcd   R_ARM_GLOB_DAT(X) */
   };
 
+#define PLT_RISCOS_HEADER_SIZE 12
+#define PLT_RISCOS_ENTRY_SIZE 20
+
+/* The first entry in a procedure linkage table looks like
+   this.  It is set up so that any shared library function that is
+   called before the relocation has been set up calls the dynamic
+   linker first.  */
+static const bfd_vma elf32_arm_riscos_plt0_entry [PLT_RISCOS_HEADER_SIZE / 4] =
+  {
+    0xe92d509f,		/* stmfd r13!,{r0-r4,r7,r12,r14} */
+    0xef078581,		/* swi	 "XSOM_Resolver" */
+    0xe5b8f008,		/* ldr   pc, [r8, #8]!  */
+  };
+
+/* Subsequent entries in a procedure linkage table look like
+   this.  */
+static const bfd_vma elf32_arm_riscos_plt_entry [PLT_RISCOS_ENTRY_SIZE / 4] =
+  {
+    0xe24f8c00,		/* sub   r8, pc, 0x0000XX00 */
+    0xe2488000,		/* sub   r8, r8, 0x000000XX */
+    0xef078580,		/* swi   "XSOM_Prologue" */
+    0xe2888a00,		/* add   r8, r8, #0x000XX000 */
+    0xe5b8f000,		/* ldr   pc, [r8,#0x00000XXX]! */
+  };
+
 /* Used to build a map of a section.  This is required for mixed-endian
    code/data.  */
 
@@ -1888,6 +1913,12 @@
 	}
     }
 
+  if (info->flag_pic == 2)
+    {
+      htab->plt_header_size = PLT_RISCOS_HEADER_SIZE;
+      htab->plt_entry_size = PLT_RISCOS_ENTRY_SIZE;
+    }
+
   if (!htab->splt 
       || !htab->srelplt
       || !htab->sdynbss
@@ -6930,7 +6961,10 @@
 	     in all the symbols for which we are making plt entries.  The
 	     first three entries in .got.plt are reserved; after that
 	     symbols appear in the same order as in .plt.  */
-	  plt_index = (got_offset - 12) / 4;
+          if (info->flag_pic == 2)
+            plt_index = (h->plt.offset - PLT_RISCOS_HEADER_SIZE) / PLT_RISCOS_ENTRY_SIZE;
+          else
+            plt_index = (got_offset - 12) / 4;
 
 	  /* Calculate the address of the GOT entry.  */
 	  got_address = (sgot->output_section->vma
@@ -7012,22 +7046,47 @@
 			      splt->contents + h->plt.offset - 2);
 		}
 
-	      bfd_put_32 (output_bfd,
-			  elf32_arm_plt_entry[0]
-			  | ((got_displacement & 0x0ff00000) >> 20),
-			  splt->contents + h->plt.offset + 0);
-	      bfd_put_32 (output_bfd,
-			  elf32_arm_plt_entry[1]
-			  | ((got_displacement & 0x000ff000) >> 12),
-			  splt->contents + h->plt.offset + 4);
-	      bfd_put_32 (output_bfd,
-			  elf32_arm_plt_entry[2]
-			  | (got_displacement & 0x00000fff),
-			  splt->contents + h->plt.offset + 8);
+	      if (info->flag_pic == 2)
+		{
+		  bfd_put_32 (output_bfd,
+			      elf32_arm_riscos_plt_entry[0]
+			      | ((h->plt.offset & 0x0000ff00) >> 8),
+			      splt->contents + h->plt.offset);
+		  bfd_put_32 (output_bfd,
+			      elf32_arm_riscos_plt_entry[1]
+			      | (h->plt.offset & 0x000000ff),
+			      splt->contents + h->plt.offset + 4);
+		  bfd_put_32 (output_bfd,
+			      elf32_arm_riscos_plt_entry[2],
+			      splt->contents + h->plt.offset + 8);
+		  bfd_put_32 (output_bfd,
+			      elf32_arm_riscos_plt_entry[3]
+			      | ((got_offset & 0x000ff000) >> 12),
+			      splt->contents + h->plt.offset + 12);
+		  bfd_put_32 (output_bfd,
+			      elf32_arm_riscos_plt_entry[4]
+			      | (got_offset & 0x00000fff),
+			      splt->contents + h->plt.offset + 16);
+		}
+	      else
+		{
+		  bfd_put_32 (output_bfd,
+			      elf32_arm_plt_entry[0]
+			      | ((got_displacement & 0x0ff00000) >> 20),
+			      splt->contents + h->plt.offset + 0);
+		  bfd_put_32 (output_bfd,
+			      elf32_arm_plt_entry[1]
+			      | ((got_displacement & 0x000ff000) >> 12),
+			      splt->contents + h->plt.offset + 4);
+		  bfd_put_32 (output_bfd,
+			      elf32_arm_plt_entry[2]
+			      | (got_displacement & 0x00000fff),
+			      splt->contents + h->plt.offset + 8);
 #ifdef FOUR_WORD_PLT
-	      bfd_put_32 (output_bfd, elf32_arm_plt_entry[3],
-			  splt->contents + h->plt.offset + 12);
+		  bfd_put_32 (output_bfd, elf32_arm_plt_entry[3],
+			      splt->contents + h->plt.offset + 12);
 #endif
+		}
 	    }
 
 	  /* Fill in the entry in the global offset table.  */
@@ -7350,21 +7409,30 @@
 	    }
 	  else
 	    {
-	      got_displacement = got_address - (plt_address + 16);
+	      if (info->flag_pic == 2)
+	        {
+		  bfd_put_32 (output_bfd, elf32_arm_riscos_plt0_entry[0], splt->contents +  0);
+		  bfd_put_32 (output_bfd, elf32_arm_riscos_plt0_entry[1], splt->contents +  4);
+		  bfd_put_32 (output_bfd, elf32_arm_riscos_plt0_entry[2], splt->contents +  8);
+	        }
+	      else
+	        {
+		  got_displacement = got_address - (plt_address + 16);
 
-	      plt0_entry = elf32_arm_plt0_entry;
-	      bfd_put_32 (output_bfd, plt0_entry[0], splt->contents + 0);
-	      bfd_put_32 (output_bfd, plt0_entry[1], splt->contents + 4);
-	      bfd_put_32 (output_bfd, plt0_entry[2], splt->contents + 8);
-	      bfd_put_32 (output_bfd, plt0_entry[3], splt->contents + 12);
+		  plt0_entry = elf32_arm_plt0_entry;
+		  bfd_put_32 (output_bfd, plt0_entry[0], splt->contents + 0);
+		  bfd_put_32 (output_bfd, plt0_entry[1], splt->contents + 4);
+		  bfd_put_32 (output_bfd, plt0_entry[2], splt->contents + 8);
+		  bfd_put_32 (output_bfd, plt0_entry[3], splt->contents + 12);
 
 #ifdef FOUR_WORD_PLT
-	      /* The displacement value goes in the otherwise-unused
-		 last word of the second entry.  */
-	      bfd_put_32 (output_bfd, got_displacement, splt->contents + 28);
+		/* The displacement value goes in the otherwise-unused
+		   last word of the second entry.  */
+		  bfd_put_32 (output_bfd, got_displacement, splt->contents + 28);
 #else
-	      bfd_put_32 (output_bfd, got_displacement, splt->contents + 16);
+		  bfd_put_32 (output_bfd, got_displacement, splt->contents + 16);
 #endif
+	        }
 	    }
 	}
 
