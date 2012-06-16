Index: libgcc/crtstuff.c
===================================================================
--- libgcc/crtstuff.c	(revision 186670)
+++ libgcc/crtstuff.c	(working copy)
@@ -209,13 +209,13 @@
 #elif defined(CTORS_SECTION_ASM_OP)
 /* Hack: force cc1 to switch to .data section early, so that assembling
    __CTOR_LIST__ does not undo our behind-the-back change to .ctors.  */
-static func_ptr force_to_data[1] __attribute__ ((__used__)) = { };
+static const func_ptr force_to_data[1] __attribute__ ((__used__)) = { };
 asm (CTORS_SECTION_ASM_OP);
-STATIC func_ptr __CTOR_LIST__[1]
+STATIC const func_ptr __CTOR_LIST__[1]
   __attribute__ ((__used__, aligned(sizeof(func_ptr))))
   = { (func_ptr) (-1) };
 #else
-STATIC func_ptr __CTOR_LIST__[1]
+STATIC const func_ptr __CTOR_LIST__[1]
   __attribute__ ((__used__, section(".ctors"), aligned(sizeof(func_ptr))))
   = { (func_ptr) (-1) };
 #endif /* __CTOR_LIST__ alternatives */
@@ -224,11 +224,11 @@
 DTOR_LIST_BEGIN;
 #elif defined(DTORS_SECTION_ASM_OP)
 asm (DTORS_SECTION_ASM_OP);
-STATIC func_ptr __DTOR_LIST__[1]
+STATIC const func_ptr __DTOR_LIST__[1]
   __attribute__ ((aligned(sizeof(func_ptr))))
   = { (func_ptr) (-1) };
 #else
-STATIC func_ptr __DTOR_LIST__[1]
+STATIC const func_ptr __DTOR_LIST__[1]
   __attribute__((section(".dtors"), aligned(sizeof(func_ptr))))
   = { (func_ptr) (-1) };
 #endif /* __DTOR_LIST__ alternatives */
@@ -251,11 +251,11 @@
 #endif /* JCR_SECTION_NAME */
 
 #if USE_TM_CLONE_REGISTRY
-STATIC func_ptr __TMC_LIST__[]
+STATIC const func_ptr __TMC_LIST__[]
   __attribute__((used, section(".tm_clone_table"), aligned(sizeof(void*))))
   = { };
 # ifdef HAVE_GAS_HIDDEN
-extern func_ptr __TMC_END__[] __attribute__((__visibility__ ("hidden")));
+extern const func_ptr __TMC_END__[] __attribute__((__visibility__ ("hidden")));
 # endif
 
 static inline void
@@ -360,11 +360,11 @@
 #ifdef FINI_ARRAY_SECTION_ASM_OP
   /* If we are using .fini_array then destructors will be run via that
      mechanism.  */
-#elif defined(HIDDEN_DTOR_LIST_END)
+#elif defined(HIDDEN_DTOR_LIST_END) && !defined(__riscos__)
   {
     /* Safer version that makes sure only .dtors function pointers are
        called even if the static variable is maliciously changed.  */
-    extern func_ptr __DTOR_END__[] __attribute__((visibility ("hidden")));
+    extern const func_ptr __DTOR_END__[] __attribute__((visibility ("hidden")));
     static size_t dtor_idx;
     const size_t max_idx = __DTOR_END__ - __DTOR_LIST__ - 1;
     func_ptr f;
@@ -376,8 +376,12 @@
       }
   }
 #else /* !defined (FINI_ARRAY_SECTION_ASM_OP) */
+  /* For RISC OS we force this code to be used, as the default choice above produces
+     code that doesn't work. The compiler optimises so aggressively that it assumes
+     that the array only ever contains one value of -1. The code produced does not
+     look at the array, but simply jumps directly to 0xFFFFFFFF.  */
   {
-    static func_ptr *p = __DTOR_LIST__ + 1;
+    static const func_ptr *p = __DTOR_LIST__ + 1;
     func_ptr f;
 
     while ((f = *p))
@@ -522,7 +526,7 @@
 void
 __do_global_dtors (void)
 {
-  func_ptr *p, f;
+  const func_ptr *p, f;
   for (p = __DTOR_LIST__ + 1; (f = *p); p++)
     f ();
 
@@ -590,11 +594,11 @@
    __CTOR_LIST__ does not undo our behind-the-back change to .ctors.  */
 static func_ptr force_to_data[1] __attribute__ ((__used__)) = { };
 asm (CTORS_SECTION_ASM_OP);
-STATIC func_ptr __CTOR_END__[1]
+STATIC const func_ptr __CTOR_END__[1]
   __attribute__((aligned(sizeof(func_ptr))))
   = { (func_ptr) 0 };
 #else
-STATIC func_ptr __CTOR_END__[1]
+STATIC const func_ptr __CTOR_END__[1]
   __attribute__((section(".ctors"), aligned(sizeof(func_ptr))))
   = { (func_ptr) 0 };
 #endif
@@ -605,7 +609,7 @@
 #ifdef DTORS_SECTION_ASM_OP
 asm (DTORS_SECTION_ASM_OP);
 #endif
-func_ptr __DTOR_END__[1]
+const func_ptr __DTOR_END__[1]
   __attribute__ ((used,
 #ifndef DTORS_SECTION_ASM_OP
 		  section(".dtors"),
@@ -614,11 +618,11 @@
   = { (func_ptr) 0 };
 #elif defined(DTORS_SECTION_ASM_OP)
 asm (DTORS_SECTION_ASM_OP);
-STATIC func_ptr __DTOR_END__[1]
+STATIC const func_ptr __DTOR_END__[1]
   __attribute__ ((used, aligned(sizeof(func_ptr))))
   = { (func_ptr) 0 };
 #else
-STATIC func_ptr __DTOR_END__[1]
+STATIC const func_ptr __DTOR_END__[1]
   __attribute__((used, section(".dtors"), aligned(sizeof(func_ptr))))
   = { (func_ptr) 0 };
 #endif
@@ -673,7 +677,7 @@
 static void __attribute__((used))
 __do_global_ctors_aux (void)
 {
-  func_ptr *p;
+  const func_ptr *p;
   for (p = __CTOR_END__ - 1; *p != (func_ptr) -1; p--)
     (*p) ();
 }
@@ -726,7 +730,7 @@
 void
 __do_global_ctors (void)
 {
-  func_ptr *p;
+  const func_ptr *p;
 #if defined(USE_EH_FRAME_REGISTRY) \
     || defined(JCR_SECTION_NAME) \
     || defined(USE_TM_CLONE_REGISTRY)
@@ -740,6 +744,13 @@
 #error "What are you doing with crtstuff.c, then?"
 #endif
 
+#ifdef __riscos__
+static const char riscos_abi_version[]
+  __attribute__((used, section(".riscos.abi.version"), aligned(4))) =
+#include "config/arm/riscos-abi.h"
+  ;
+#endif
+
 #else /* ! CRT_BEGIN && ! CRT_END */
 #error "One of CRT_BEGIN or CRT_END must be defined."
 #endif
