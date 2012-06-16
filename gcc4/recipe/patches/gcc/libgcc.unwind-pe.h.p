Index: libgcc/unwind-pe.h
===================================================================
--- libgcc/unwind-pe.h	(revision 161055)
+++ libgcc/unwind-pe.h	(working copy)
@@ -173,6 +173,22 @@
   return p;
 }
 
+#if defined (TARGET_RISCOSELF) && defined (__PIC__)
+static inline _Unwind_Internal_Ptr
+reloc (_Unwind_Internal_Ptr val)
+{
+_Unwind_Internal_Ptr res;
+
+  asm volatile ("MOV	r0, %[val];\n"
+		"SWI	%[SOM_Reloc];\n"
+		"MOV	%[res], r0;\n"
+		: [res] "=r" (res)
+		: [val] "r" (val), [SOM_Reloc] "i" (0x7858D)
+		: "r0", "cc");
+  return res;
+}
+#endif
+
 /* Load an encoded value from memory at P.  The value is returned in VAL;
    The function returns P incremented past the value.  BASE is as given
    by base_of_encoded_value for this encoding in the appropriate context.  */
@@ -262,7 +278,12 @@
 	  result += ((encoding & 0x70) == DW_EH_PE_pcrel
 		     ? (_Unwind_Internal_Ptr) u : base);
 	  if (encoding & DW_EH_PE_indirect)
-	    result = *(_Unwind_Internal_Ptr *) result;
+	    {
+#if defined (TARGET_RISCOSELF) && defined (__PIC__)
+	      result = reloc (result);
+#endif
+	      result = *(_Unwind_Internal_Ptr *) result;
+	    }
 	}
     }
 
