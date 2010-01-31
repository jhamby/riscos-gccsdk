--- gcc/unwind-pe.h.orig	2009-07-03 20:53:37.000000000 +0100
+++ gcc/unwind-pe.h	2009-06-27 18:58:40.000000000 +0100
@@ -175,6 +175,22 @@ read_sleb128 (const unsigned char *p, _U
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
@@ -264,7 +280,12 @@ read_encoded_value_with_base (unsigned c
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
 
