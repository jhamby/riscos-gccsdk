Index: libstdc++-v3/libsupc++/eh_personality.cc
===================================================================
--- libstdc++-v3/libsupc++/eh_personality.cc	(revision 214062)
+++ libstdc++-v3/libsupc++/eh_personality.cc	(working copy)
@@ -204,7 +204,30 @@
   return *e == 0;
 }
 
+#ifdef __riscos__
+
+static inline unsigned int
+clear_status_bits (unsigned int pc_in)
+{
+  unsigned int pc_out = pc_in;
+
+  asm volatile ("	TEQ	r0, r0\n"
+		"	TEQ	pc, pc\n"
+		"	BICNE	%[pc_out], %[pc_in], #0xfc000003\n"
+		: [pc_out] "=r" (pc_out)
+		: [pc_in] "r" (pc_in)
+		: "cc");
+
+  return pc_out;
+}
+
 #else
+
+#define clear_status_bits(x) x
+
+#endif
+
+#else
 typedef const std::type_info _throw_typet;
 
 
@@ -323,6 +346,7 @@
 }
 
 #endif // !__ARM_EABI_UNWINDER__
+#include <stdio.h>
 
 namespace __cxxabiv1
 {
@@ -434,7 +458,7 @@
   p = parse_lsda_header (context, language_specific_data, &info);
   info.ttype_base = base_of_encoded_value (info.ttype_encoding, context);
 #ifdef _GLIBCXX_HAVE_GETIPINFO
-  ip = _Unwind_GetIPInfo (context, &ip_before_insn);
+  ip = clear_status_bits(_Unwind_GetIPInfo (context, &ip_before_insn));
 #else
   ip = _Unwind_GetIP (context);
 #endif
