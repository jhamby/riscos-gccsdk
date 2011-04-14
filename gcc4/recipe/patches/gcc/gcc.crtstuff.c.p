Index: gcc/crtstuff.c
===================================================================
--- gcc/crtstuff.c	(revision 168231)
+++ gcc/crtstuff.c	(working copy)
@@ -198,11 +198,11 @@
    __CTOR_LIST__ does not undo our behind-the-back change to .ctors.  */
 static func_ptr force_to_data[1] __attribute__ ((__used__)) = { };
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
@@ -211,11 +211,11 @@
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
@@ -297,11 +297,11 @@
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
@@ -313,8 +313,12 @@
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
@@ -448,7 +452,7 @@
 void
 __do_global_dtors (void)
 {
-  func_ptr *p, f;
+  const func_ptr *p, f;
   for (p = __DTOR_LIST__ + 1; (f = *p); p++)
     f ();
 
@@ -502,11 +506,11 @@
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
@@ -517,7 +521,7 @@
 #ifdef DTORS_SECTION_ASM_OP
 asm (DTORS_SECTION_ASM_OP);
 #endif
-func_ptr __DTOR_END__[1]
+const func_ptr __DTOR_END__[1]
   __attribute__ ((used,
 #ifndef DTORS_SECTION_ASM_OP
 		  section(".dtors"),
@@ -526,11 +530,11 @@
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
@@ -571,7 +575,7 @@
 static void __attribute__((used))
 __do_global_ctors_aux (void)
 {
-  func_ptr *p;
+  const func_ptr *p;
   for (p = __CTOR_END__ - 1; *p != (func_ptr) -1; p--)
     (*p) ();
 }
@@ -624,7 +628,7 @@
 void
 __do_global_ctors (void)
 {
-  func_ptr *p;
+  const func_ptr *p;
 #if defined(USE_EH_FRAME_REGISTRY) || defined(JCR_SECTION_NAME)
   __do_global_ctors_1();
 #endif
