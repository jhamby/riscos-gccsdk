--- gcc/config/arm/arm.h.orig	2007-03-04 03:50:32.000000000 +0100
+++ gcc/config/arm/arm.h	2007-02-19 23:49:32.000000000 +0100
@@ -190,7 +190,7 @@
 				         : TARGET_TPCS_FRAME)
 #define TARGET_LDRD			(arm_arch5e && ARM_DOUBLEWORD_ALIGN)
 #define TARGET_AAPCS_BASED \
-    (arm_abi != ARM_ABI_APCS && arm_abi != ARM_ABI_ATPCS)
+    (arm_abi != ARM_ABI_APCS && arm_abi != ARM_ABI_ATPCS && arm_abi != ARM_ABI_APCS32)
 
 #define TARGET_HARD_TP			(target_thread_pointer == TP_CP15)
 #define TARGET_SOFT_TP			(target_thread_pointer == TP_SOFT)
@@ -283,7 +283,8 @@
   ARM_ABI_ATPCS,
   ARM_ABI_AAPCS,
   ARM_ABI_IWMMXT,
-  ARM_ABI_AAPCS_LINUX
+  ARM_ABI_AAPCS_LINUX,
+  ARM_ABI_APCS32
 };
 
 extern enum arm_abi_type arm_abi;
@@ -719,8 +720,10 @@
       fixed_regs[PIC_OFFSET_TABLE_REGNUM] = 1;			\
       call_used_regs[PIC_OFFSET_TABLE_REGNUM] = 1;		\
     }								\
-  else if (TARGET_APCS_STACK)					\
+  if (TARGET_APCS_STACK || arm_abi == ARM_ABI_APCS32)		\
     {								\
+      /* For APCS-32, we preserve SL because we need to		\
+	 interwork with functions that have stack checking.  */	\
       fixed_regs[10]     = 1;					\
       call_used_regs[10] = 1;					\
     }								\
@@ -788,9 +791,13 @@
 #define LAST_HI_REGNUM		11
 
 #ifndef TARGET_UNWIND_INFO
+/* We use sjlj exceptions for backwards compatibility.
+   For RISC OS/elf, we will not use SJLJ.  */
+#ifndef TARGET_RISCOSELF
 /* We use sjlj exceptions for backwards compatibility.  */
 #define MUST_USE_SJLJ_EXCEPTIONS 1
 #endif
+#endif
 
 /* We can generate DWARF2 Unwind info, even though we don't use it.  */
 #define DWARF2_UNWIND_INFO 1
@@ -826,13 +833,16 @@
 #define ARM_HARD_FRAME_POINTER_REGNUM	11
 #define THUMB_HARD_FRAME_POINTER_REGNUM	 7
 
+#define FP_REGNUM	\
+  (arm_abi == ARM_ABI_APCS32		\
+   ? ARM_HARD_FRAME_POINTER_REGNUM	\
+   : HARD_FRAME_POINTER_REGNUM)
+
 #define HARD_FRAME_POINTER_REGNUM		\
   (TARGET_ARM					\
    ? ARM_HARD_FRAME_POINTER_REGNUM		\
    : THUMB_HARD_FRAME_POINTER_REGNUM)
 
-#define FP_REGNUM	                HARD_FRAME_POINTER_REGNUM
-
 /* Register to use for pushing function arguments.  */
 #define STACK_POINTER_REGNUM	SP_REGNUM
 
@@ -852,10 +862,18 @@
   (((REGNUM) >= FIRST_IWMMXT_GR_REGNUM) && ((REGNUM) <= LAST_IWMMXT_GR_REGNUM))
 
 /* Base register for access to local variables of the function.  */
+#ifdef TARGET_RISCOSAOF
+#define FRAME_POINTER_REGNUM	9
+#else
 #define FRAME_POINTER_REGNUM	25
+#endif
 
 /* Base register for access to arguments of the function.  */
+#ifdef TARGET_RISCOSAOF
+#define ARG_POINTER_REGNUM	ARM_HARD_FRAME_POINTER_REGNUM
+#else
 #define ARG_POINTER_REGNUM	26
+#endif
 
 #define FIRST_CIRRUS_FP_REGNUM	27
 #define LAST_CIRRUS_FP_REGNUM	42
@@ -865,7 +883,8 @@
 #define FIRST_VFP_REGNUM	63
 #define LAST_VFP_REGNUM		94
 #define IS_VFP_REGNUM(REGNUM) \
-  (((REGNUM) >= FIRST_VFP_REGNUM) && ((REGNUM) <= LAST_VFP_REGNUM))
+  ((((REGNUM) >= FIRST_VFP_REGNUM) && ((REGNUM) <= LAST_VFP_REGNUM)) \
+   || (REGNUM) == 95)
 
 /* The number of hard registers is 16 ARM + 8 FPA + 1 CC + 1 SFP + 1 AFP.  */
 /* + 16 Cirrus registers take us up to 43.  */
@@ -880,16 +899,18 @@
    via the stack pointer) in functions that seem suitable.
    If we have to have a frame pointer we might as well make use of it.
    APCS says that the frame pointer does not need to be pushed in leaf
-   functions, or simple tail call functions.  */
-
+   functions, or simple tail call functions.
+   The APCS-32 ABI never requires a frame pointer.  */
 #ifndef SUBTARGET_FRAME_POINTER_REQUIRED
 #define SUBTARGET_FRAME_POINTER_REQUIRED 0
 #endif
 
-#define FRAME_POINTER_REQUIRED					\
-  (current_function_has_nonlocal_label				\
-   || SUBTARGET_FRAME_POINTER_REQUIRED				\
-   || (TARGET_ARM && TARGET_APCS_FRAME && ! leaf_function_p ()))
+#define FRAME_POINTER_REQUIRED						\
+  (arm_abi == ARM_ABI_APCS32						\
+   ? 0									\
+   : (current_function_has_nonlocal_label				\
+      || SUBTARGET_FRAME_POINTER_REQUIRED				\
+      || (TARGET_ARM && TARGET_APCS_FRAME && ! leaf_function_p ())))
 
 /* Return number of consecutive hard regs needed starting at reg REGNO
    to hold something of mode MODE.
@@ -898,6 +919,13 @@
 
    On the ARM regs are UNITS_PER_WORD bits wide; FPA regs can hold any FP
    mode.  */
+#ifdef TARGET_RISCOSAOF
+#define HARD_REGNO_NREGS(REGNO, MODE)  	\
+  ((TARGET_ARM 				\
+    && REGNO >= FIRST_FPA_REGNUM)	\
+    && !IS_VFP_REGNUM (REGNO)		\
+   ? 1 : ARM_NUM_REGS (MODE))
+#else
 #define HARD_REGNO_NREGS(REGNO, MODE)  	\
   ((TARGET_ARM 				\
     && REGNO >= FIRST_FPA_REGNUM	\
@@ -905,6 +933,7 @@
     && REGNO != ARG_POINTER_REGNUM)	\
     && !IS_VFP_REGNUM (REGNO)		\
    ? 1 : ARM_NUM_REGS (MODE))
+#endif
 
 /* Return true if REGNO is suitable for holding a quantity of type MODE.  */
 #define HARD_REGNO_MODE_OK(REGNO, MODE)					\
@@ -1350,7 +1379,7 @@
    is at the high-address end of the local variables;
    that is, each additional local variable allocated
    goes at a more negative offset in the frame.  */
-#define FRAME_GROWS_DOWNWARD 1
+#define FRAME_GROWS_DOWNWARD ((arm_abi == ARM_ABI_APCS32) ? 0 : 1)
 
 /* The amount of scratch space needed by _interwork_{r7,r11}_call_via_rN().
    When present, it is one word in size, and sits at the top of the frame,
@@ -1370,7 +1399,10 @@
    If FRAME_GROWS_DOWNWARD, this is the offset to the END of the
    first local allocated.  Otherwise, it is the offset to the BEGINNING
    of the first local allocated.  */
-#define STARTING_FRAME_OFFSET  0
+#define STARTING_FRAME_OFFSET				\
+   (arm_abi == ARM_ABI_APCS32				\
+    ? current_function_outgoing_args_size		\
+    : 0)
 
 /* If we generate an insn to push BYTES bytes,
    this says how many the stack pointer really advances by.  */
@@ -1495,6 +1527,7 @@
   int soft_frame;	/* FRAME_POINTER_REGNUM.  */
   int locals_base;	/* THUMB_HARD_FRAME_POINTER_REGNUM.  */
   int outgoing_args;	/* STACK_POINTER_REGNUM.  */
+  int frame_size;       /* get_frame_size () */
 }
 arm_stack_offsets;
 
@@ -1519,6 +1552,10 @@
   /* Records if sibcalls are blocked because an argument
      register is needed to preserve stack alignment.  */
   int sibcall_blocked;
+  /* Non-zero if this is a leaf function.  */
+  int leaf;
+  /* Non-zero if this function calls __builtin_apply.  */
+  int apply_args;
   /* Labels for per-function Thumb call-via stubs.  One per potential calling
      register.  We can never call via LR or PC.  We can call via SP if a
      trampoline happens to be on the top of the stack.  */
@@ -1539,6 +1576,8 @@
   int nregs;
   /* This is the number of iWMMXt register arguments scanned so far.  */
   int iwmmxt_nregs;
+  /* This is the number of FPA register arguments scanned so far.  */
+  int fpa_nregs;
   int named_count;
   int nargs;
   /* One of CALL_NORMAL, CALL_LONG or CALL_SHORT.  */
@@ -1593,6 +1632,9 @@
   if (arm_vector_mode_supported_p (MODE)	       	\
       && (CUM).named_count > (CUM).nargs)		\
     (CUM).iwmmxt_nregs += 1;				\
+  else if (TARGET_FPA && TARGET_APCS_FLOAT		\
+	   && (MODE == SFmode || MODE == DFmode))	\
+    (CUM).fpa_nregs += 1;				\
   else							\
     (CUM).nregs += ARM_NUM_REGS2 (MODE, TYPE)
 
@@ -1607,9 +1649,7 @@
 /* 1 if N is a possible register number for function argument passing.
    On the ARM, r0-r3 are used to pass args.  */
 #define FUNCTION_ARG_REGNO_P(REGNO)	\
-   (IN_RANGE ((REGNO), 0, 3)		\
-    || (TARGET_IWMMXT_ABI		\
-	&& IN_RANGE ((REGNO), FIRST_IWMMXT_REGNUM, FIRST_IWMMXT_REGNUM + 9)))
+  arm_function_arg_regno_p ((REGNO))
 
 
 /* If your target environment doesn't prefix user functions with an
@@ -1695,6 +1735,11 @@
    pointer.  Note we have to use {ARM|THUMB}_HARD_FRAME_POINTER_REGNUM
    because the definition of HARD_FRAME_POINTER_REGNUM is not a constant.  */
 
+#ifdef TARGET_RISCOSAOF
+#define ELIMINABLE_REGS                                         \
+{{ ARG_POINTER_REGNUM,        STACK_POINTER_REGNUM            },\
+ { FRAME_POINTER_REGNUM,      STACK_POINTER_REGNUM            }}
+#else
 #define ELIMINABLE_REGS						\
 {{ ARG_POINTER_REGNUM,        STACK_POINTER_REGNUM            },\
  { ARG_POINTER_REGNUM,        FRAME_POINTER_REGNUM            },\
@@ -1703,21 +1748,11 @@
  { FRAME_POINTER_REGNUM,      STACK_POINTER_REGNUM            },\
  { FRAME_POINTER_REGNUM,      ARM_HARD_FRAME_POINTER_REGNUM   },\
  { FRAME_POINTER_REGNUM,      THUMB_HARD_FRAME_POINTER_REGNUM }}
+#endif
 
 /* Given FROM and TO register numbers, say whether this elimination is
-   allowed.  Frame pointer elimination is automatically handled.
-
-   All eliminations are permissible.  Note that ARG_POINTER_REGNUM and
-   HARD_FRAME_POINTER_REGNUM are in fact the same thing.  If we need a frame
-   pointer, we must eliminate FRAME_POINTER_REGNUM into
-   HARD_FRAME_POINTER_REGNUM and not into STACK_POINTER_REGNUM or
-   ARG_POINTER_REGNUM.  */
-#define CAN_ELIMINATE(FROM, TO)						\
-  (((TO) == FRAME_POINTER_REGNUM && (FROM) == ARG_POINTER_REGNUM) ? 0 :	\
-   ((TO) == STACK_POINTER_REGNUM && frame_pointer_needed) ? 0 :		\
-   ((TO) == ARM_HARD_FRAME_POINTER_REGNUM && TARGET_THUMB) ? 0 :	\
-   ((TO) == THUMB_HARD_FRAME_POINTER_REGNUM && TARGET_ARM) ? 0 :	\
-   1)
+   allowed.  Frame pointer elimination is automatically handled.  */
+#define CAN_ELIMINATE(FROM, TO)	arm_can_eliminate ((FROM), (TO))
 
 /* Define the offset between two registers, one to be eliminated, and the
    other its replacement, at the start of a routine.  */
@@ -1834,8 +1869,8 @@
 /*   On the ARM, don't allow the pc to be used.  */
 #define ARM_REGNO_OK_FOR_BASE_P(REGNO)			\
   (TEST_REGNO (REGNO, <, PC_REGNUM)			\
-   || TEST_REGNO (REGNO, ==, FRAME_POINTER_REGNUM)	\
-   || TEST_REGNO (REGNO, ==, ARG_POINTER_REGNUM))
+   || (! TARGET_APCS_STACK && TEST_REGNO (REGNO, ==, FRAME_POINTER_REGNUM)) \
+   || (! TARGET_APCS_STACK && TEST_REGNO (REGNO, ==, ARG_POINTER_REGNUM)))
 
 #define THUMB_REGNO_MODE_OK_FOR_BASE_P(REGNO, MODE)		\
   (TEST_REGNO (REGNO, <=, LAST_LO_REGNUM)			\
@@ -2012,8 +2047,8 @@
 #define ARM_REG_OK_FOR_BASE_P(X)		\
   (REGNO (X) <= LAST_ARM_REGNUM			\
    || REGNO (X) >= FIRST_PSEUDO_REGISTER	\
-   || REGNO (X) == FRAME_POINTER_REGNUM		\
-   || REGNO (X) == ARG_POINTER_REGNUM)
+   || (! TARGET_APCS_STACK && REGNO (X) == FRAME_POINTER_REGNUM) \
+   || (! TARGET_APCS_STACK && REGNO (X) == ARG_POINTER_REGNUM))
 
 #define THUMB_REG_MODE_OK_FOR_BASE_P(X, MODE)	\
   (REGNO (X) <= LAST_LO_REGNUM			\
@@ -2530,7 +2565,7 @@
      in 26 bit mode, the condition codes must be masked out of the	\
      return address.  This does not apply to ARM6 and later processors	\
      when running in 32 bit mode.  */					\
-  ((arm_arch4 || TARGET_THUMB)						\
+  ((arm_arch4 || TARGET_THUMB)		\
    ? (gen_int_mode ((unsigned long)0xffffffff, Pmode))			\
    : arm_gen_return_addr_mask ())
 
@@ -2699,6 +2734,8 @@
 
   ARM_BUILTIN_THREAD_POINTER,
 
+  ARM_BUILTIN_SWI,
+
   ARM_BUILTIN_MAX
 };
 #endif /* ! GCC_ARM_H */
