--- gcc/config/arm/arm.c.orig	2007-12-21 00:04:48.000000000 +0100
+++ gcc/config/arm/arm.c	2007-12-20 20:18:29.000000000 +0100
@@ -52,6 +52,7 @@
 #include "target-def.h"
 #include "debug.h"
 #include "langhooks.h"
+#include "version.h"
 
 /* Forward definitions of types.  */
 typedef struct minipool_node    Mnode;
@@ -112,6 +113,7 @@
 #if TARGET_DLLIMPORT_DECL_ATTRIBUTES
 static tree arm_handle_notshared_attribute (tree *, tree, tree, int, bool *);
 #endif
+static int arm_leaf_function_p (void);
 static void arm_output_function_epilogue (FILE *, HOST_WIDE_INT);
 static void arm_output_function_prologue (FILE *, HOST_WIDE_INT);
 static void thumb_output_function_prologue (FILE *, HOST_WIDE_INT);
@@ -695,7 +697,8 @@
   {"atpcs",   ARM_ABI_ATPCS},
   {"aapcs",   ARM_ABI_AAPCS},
   {"iwmmxt",  ARM_ABI_IWMMXT},
-  {"aapcs-linux",   ARM_ABI_AAPCS_LINUX}
+  {"aapcs-linux",   ARM_ABI_AAPCS_LINUX},
+  {"apcs-32",  ARM_ABI_APCS32}
 };
 
 /* Supported TLS relocations.  */
@@ -1070,6 +1073,16 @@
   if (TARGET_POKE_FUNCTION_NAME)
     target_flags |= MASK_APCS_FRAME;
 
+#ifdef TARGET_RISCOSELF
+  /* For RISC OS module support, we need -fpic specified.  */
+  if (TARGET_MODULE)
+    {
+      if (flag_pic == 2)
+        error ("-mmodule and -fPIC are incompatible");
+      flag_pic = 1;
+    }
+#endif
+
   if (TARGET_APCS_REENT && flag_pic)
     error ("-fpic and -mapcs-reent are incompatible");
 
@@ -1087,10 +1100,18 @@
   /* If stack checking is disabled, we can use r10 as the PIC register,
      which keeps r9 available.  */
   if (flag_pic)
+#ifdef TARGET_RISCOSELF
+    /* RISC OS Loader always expects r7 to be used.  For module code this is
+       r8.  */
+    arm_pic_register = TARGET_MODULE ? 8 : 7;
+#else
     arm_pic_register = TARGET_APCS_STACK ? 9 : 10;
+#endif
 
+#if 0
   if (TARGET_APCS_FLOAT)
     warning (0, "passing floating point arguments in fp regs not yet supported");
+#endif
 
   /* Initialize boolean versions of the flags, for use in the arm.md file.  */
   arm_arch3m = (insn_flags & FL_ARCH3M) != 0;
@@ -1223,6 +1244,17 @@
   if (TARGET_IWMMXT && !TARGET_SOFT_FLOAT)
     sorry ("iWMMXt and hardware floating point");
 
+#ifdef TARGET_RISCOSELF
+  if (arm_float_abi == ARM_FLOAT_ABI_SOFTFP)
+    sorry ("arm-unknown-riscos and -mfloat-abi=softfp");
+
+  if (TARGET_SOFT_FLOAT && !TARGET_UNIXLIB)
+    sorry ("-mlibscl and -mfloat-abi=soft");
+
+  if (TARGET_UNIXLIB && TARGET_MODULE)
+    sorry ("-munixlib and -mmodule");
+#endif
+
   /* If soft-float is specified then don't use FPU.  */
   if (TARGET_SOFT_FLOAT)
     arm_fpu_arch = FPUTYPE_NONE;
@@ -1472,11 +1504,14 @@
     return 0;
 
   /* So do interrupt functions that use the frame pointer.  */
-  if (IS_INTERRUPT (func_type) && frame_pointer_needed)
+  if (IS_INTERRUPT (func_type) && arm_apcs_frame_needed ())
     return 0;
 
   offsets = arm_get_frame_offsets ();
-  stack_adjust = offsets->outgoing_args - offsets->saved_regs;
+  if (arm_abi == ARM_ABI_APCS32)
+    stack_adjust = abs (- offsets->frame_size - offsets->outgoing_args);
+  else
+    stack_adjust = offsets->outgoing_args - offsets->saved_regs;
 
   /* As do variadic functions.  */
   if (current_function_pretend_args_size
@@ -1484,10 +1519,11 @@
       /* Or if the function calls __builtin_eh_return () */
       || current_function_calls_eh_return
       /* Or if the function calls alloca */
-      || current_function_calls_alloca
+      || (current_function_calls_alloca && ! TARGET_APCS_FRAME)
       /* Or if there is a stack adjustment.  However, if the stack pointer
 	 is saved on the stack, we can use a pre-incrementing stack load.  */
-      || !(stack_adjust == 0 || (frame_pointer_needed && stack_adjust == 4)))
+      || !(stack_adjust == 0
+	   || (arm_apcs_frame_needed () && stack_adjust == 4)))
     return 0;
 
   saved_int_regs = arm_compute_save_reg_mask ();
@@ -2657,6 +2693,7 @@
   /* On the ARM, the offset starts at 0.  */
   pcum->nregs = ((fntype && aggregate_value_p (TREE_TYPE (fntype), fntype)) ? 1 : 0);
   pcum->iwmmxt_nregs = 0;
+  pcum->fpa_nregs = 0;
   pcum->can_split = true;
 
   pcum->call_cookie = CALL_NORMAL;
@@ -2737,6 +2774,18 @@
 	}
     }
 
+  if (TARGET_FPA && TARGET_APCS_FLOAT
+      && (mode == SFmode || mode == DFmode))
+    {
+      if (pcum->fpa_nregs <= 3)
+	return gen_rtx_REG (mode, pcum->fpa_nregs + FIRST_FPA_REGNUM);
+      else
+	{
+	  pcum->can_split = false;
+	  return NULL_RTX;
+	}
+    }
+
   /* Put doubleword aligned quantities in even register pairs.  */
   if (pcum->nregs & 1
       && ARM_DOUBLEWORD_ALIGN
@@ -2770,6 +2819,10 @@
   if (arm_vector_mode_supported_p (mode))
     return 0;
 
+  else if (TARGET_FPA && TARGET_APCS_FLOAT
+	   && (mode == DFmode || mode == SFmode))
+    return 0;
+
   if (NUM_ARG_REGS > nregs
       && (NUM_ARG_REGS < nregs + ARM_NUM_REGS2 (mode, type))
       && pcum->can_split)
@@ -2778,6 +2831,26 @@
   return 0;
 }
 
+/* Return 1 if 'regno' is a possible register number for function argument
+   passing.  */
+int
+arm_function_arg_regno_p (int regno)
+{
+  /* On the ARM, r0-r3 are used to pass regs.  */
+  if (regno >= 0 && regno <= 3)
+    return 1;
+
+  if (TARGET_FPA && TARGET_APCS_FLOAT
+      && regno >= FIRST_FPA_REGNUM && regno <= FIRST_FPA_REGNUM + 3)
+    return 1;
+
+  if (TARGET_IWMMXT_ABI
+      && regno >= FIRST_IWMMXT_REGNUM && regno <= FIRST_IWMMXT_REGNUM + 9)
+    return 1;
+
+  return 0;
+}
+
 /* Variable sized types are passed by reference.  This is a GCC
    extension to the ARM ABI.  */
 
@@ -3136,7 +3209,8 @@
     return false;
 
   /* Never tailcall something for which we have no decl, or if we
-     are in Thumb mode.  */
+     are in Thumb mode.  With explicit stack checking, tail calls
+     result in undefined behaviour.  */
   if (decl == NULL || TARGET_THUMB)
     return false;
 
@@ -3162,6 +3236,26 @@
   if (IS_INTERRUPT (arm_current_func_type ()))
     return false;
 
+  /* When stack checking we can only allow a sibcall when there is
+     exactly one function call, and that is the candidate for the
+     sibcall.  In other words, we can prevent a full stack-frame
+     being setup.  */
+  if (TARGET_APCS_STACK
+      && (arm_apcs_frame_needed ()
+	  || current_function_outgoing_args_size))
+    return false;
+
+#ifdef TARGET_RISCOSELF
+  /* When compiling PIC don't tail-call an external function. The PLT
+     requires that the PIC register be loaded by the caller, but the caller
+     must restore the PIC register before the call.  */
+  if (flag_pic == 2 && TREE_PUBLIC(decl))
+  {
+    current_function_uses_pic_offset_table = 1;
+    return false;
+  }
+#endif
+
   /* Everything else is ok.  */
   return true;
 }
@@ -3205,11 +3299,42 @@
 	  subregs = 1;
 	}
 
-#ifdef AOF_ASSEMBLER
-      /* The AOF assembler can generate relocations for these directly, and
-	 understands that the PIC register has to be added into the offset.  */
-      insn = emit_insn (gen_pic_load_addr_based (reg, orig));
-#else
+#ifdef TARGET_RISCOSELF
+      if (TARGET_MODULE)
+	{
+	  tree ftree;
+
+	  insn = emit_insn (gen_pic_load_addr_arm (reg, orig));
+	  /* Don't do reallocation based on PIC register when our
+	     symbol is a function or read-only data, i.e. do this only for
+	     global/local writable variables.  */
+	  /* FIXME: this requires more attention : in case ftree is NULL
+	     it is not clear what should be done with symbols generated
+	     by the compiler itself and not coming from src code.  */
+	  ftree = SYMBOL_REF_DECL (orig);
+	  /* printf ("legitimize_pic_address: sym=%s, tree code=%d (%d, %d), RO data=%d, const data=%d\n",
+		  XSTR (orig, 0),
+		  TREE_CODE (ftree), FUNCTION_DECL, VAR_DECL,
+		  decl_readonly_section (ftree, 1),
+		  TREE_READONLY (ftree) || TREE_CONSTANT (ftree)); */
+	  gcc_assert (TREE_CODE (ftree) == FUNCTION_DECL || TREE_CODE (ftree) == VAR_DECL);
+	  if (ftree != NULL
+	      && TREE_CODE (ftree) != FUNCTION_DECL
+	      && ! decl_readonly_section (ftree, 1)
+	      && ! (TREE_READONLY (ftree) || TREE_CONSTANT (ftree)))
+	    {
+	      /* Ptr to functions or array holding function pointers should not be pic offseted.  */
+	      tree ptrcont = (TREE_CODE (TREE_TYPE (ftree)) == ARRAY_TYPE) ? TREE_TYPE (ftree) : ftree;
+	      if (TREE_CODE (TREE_TYPE (ptrcont)) != POINTER_TYPE || TREE_CODE (TREE_TYPE (TREE_TYPE (ptrcont))) != FUNCTION_TYPE)
+		{
+		  insn = emit_insn (gen_addsi3 (reg, reg, pic_offset_table_rtx));
+		  current_function_uses_pic_offset_table = 1;
+		}
+	    }
+	}
+      else
+        {
+#endif
       if (subregs)
 	address = gen_reg_rtx (Pmode);
       else
@@ -3220,12 +3345,14 @@
       else
 	emit_insn (gen_pic_load_addr_thumb (address, orig));
 
+#ifndef TARGET_RISCOSELF
       if ((GET_CODE (orig) == LABEL_REF
 	   || (GET_CODE (orig) == SYMBOL_REF &&
 	       SYMBOL_REF_LOCAL_P (orig)))
 	  && NEED_GOT_RELOC)
 	pic_ref = gen_rtx_PLUS (Pmode, pic_offset_table_rtx, address);
       else
+#endif
 	{
 	  pic_ref = gen_const_mem (Pmode,
 				   gen_rtx_PLUS (Pmode, pic_offset_table_rtx,
@@ -3233,8 +3360,8 @@
 	}
 
       insn = emit_move_insn (reg, pic_ref);
-#endif
       current_function_uses_pic_offset_table = 1;
+        }
       /* Put a REG_EQUAL note on this insn, so that it can be optimized
 	 by loop.  */
       REG_NOTES (insn) = gen_rtx_EXPR_LIST (REG_EQUAL, orig,
@@ -3357,14 +3484,26 @@
 arm_load_pic_register (unsigned long saved_regs ATTRIBUTE_UNUSED)
 {
 #ifndef AOF_ASSEMBLER
-  rtx l1, labelno, pic_tmp, pic_tmp2, pic_rtx;
-  rtx global_offset_table;
-
   if (current_function_uses_pic_offset_table == 0 || TARGET_SINGLE_PIC_BASE)
     return;
 
   gcc_assert (flag_pic);
 
+  if (TARGET_MODULE)
+    {
+      rtx sl_reg, sb_ref;
+
+      sl_reg = gen_rtx_REG (Pmode, SL_REGNUM);
+      sb_ref = gen_rtx_MEM (Pmode,
+			    gen_rtx_PLUS (Pmode, sl_reg, GEN_INT (-536)));
+      MEM_READONLY_P (sb_ref) = 1;
+      emit_insn (gen_rtx_SET (SImode, pic_offset_table_rtx, sb_ref));
+    }
+  else
+    {
+  rtx l1, labelno, pic_tmp, pic_tmp2, pic_rtx;
+  rtx global_offset_table;
+
   /* We use an UNSPEC rather than a LABEL_REF because this label never appears
      in the code stream.  */
 
@@ -3375,7 +3514,10 @@
   global_offset_table = gen_rtx_SYMBOL_REF (Pmode, "_GLOBAL_OFFSET_TABLE_");
   /* On the ARM the PC register contains 'dot + 8' at the time of the
      addition, on the Thumb it is 'dot + 4'.  */
-  pic_tmp = plus_constant (l1, TARGET_ARM ? 8 : 4);
+  if (flag_pic == 2)
+    pic_tmp = plus_constant(l1, 4);
+  else
+    pic_tmp = plus_constant (l1, TARGET_ARM ? 8 : 4);
   if (GOT_PCREL)
     pic_tmp2 = gen_rtx_CONST (VOIDmode,
 			    gen_rtx_PLUS (Pmode, global_offset_table, pc_rtx));
@@ -3386,9 +3528,21 @@
 
   if (TARGET_ARM)
     {
-      emit_insn (gen_pic_load_addr_arm (pic_offset_table_rtx, pic_rtx));
-      emit_insn (gen_pic_add_dot_plus_eight (pic_offset_table_rtx,
+      if (flag_pic == 2) /* -fPIC */
+      {
+	emit_insn (gen_pic_load_addr_arm (pic_offset_table_rtx, pic_rtx));
+	emit_insn (gen_pic_add_dot_plus_eight (pic_offset_table_rtx,
+			pic_offset_table_rtx, labelno));
+	emit_insn (gen_rt_loadpic(pic_offset_table_rtx,
+			gen_rtx_REG (SImode, PIC_PLT_SCRATCH_REGNUM)));
+	emit_insn (gen_blockage ());
+      }
+      else if (flag_pic == 1) /* -fpic */
+      {
+        emit_insn (gen_pic_load_addr_arm (pic_offset_table_rtx, pic_rtx));
+        emit_insn (gen_pic_add_dot_plus_eight (pic_offset_table_rtx,
 					     pic_offset_table_rtx, labelno));
+      }
     }
   else
     {
@@ -3410,6 +3564,7 @@
   /* Need to emit this whether or not we obey regdecls,
      since setjmp/longjmp can cause life info to screw up.  */
   emit_insn (gen_rtx_USE (VOIDmode, pic_offset_table_rtx));
+    }
 #endif /* AOF_ASSEMBLER */
 }
 
@@ -9197,7 +9352,7 @@
 
       /* Handle the frame pointer as a special case.  */
       if (! TARGET_APCS_FRAME
-	  && ! frame_pointer_needed
+	  && ! arm_apcs_frame_needed ()
 	  && regs_ever_live[HARD_FRAME_POINTER_REGNUM]
 	  && ! call_used_regs[HARD_FRAME_POINTER_REGNUM])
 	save_reg_mask |= 1 << HARD_FRAME_POINTER_REGNUM;
@@ -9207,7 +9362,8 @@
       if (flag_pic
 	  && !TARGET_SINGLE_PIC_BASE
 	  && (regs_ever_live[PIC_OFFSET_TABLE_REGNUM]
-	      || current_function_uses_pic_offset_table))
+	      || current_function_uses_pic_offset_table
+	      || !arm_leaf_function_p ()))
 	save_reg_mask |= 1 << PIC_OFFSET_TABLE_REGNUM;
     }
 
@@ -9243,7 +9399,7 @@
 
   /* If we are creating a stack frame, then we must save the frame pointer,
      IP (which will hold the old stack pointer), LR and the PC.  */
-  if (frame_pointer_needed)
+  if (arm_apcs_frame_needed ())
     save_reg_mask |=
       (1 << ARM_HARD_FRAME_POINTER_REGNUM)
       | (1 << IP_REGNUM)
@@ -9327,7 +9483,7 @@
     mask |= 1 << PIC_OFFSET_TABLE_REGNUM;
 
   /* See if we might need r11 for calls to _interwork_r11_call_via_rN().  */
-  if (!frame_pointer_needed && CALLER_INTERWORKING_SLOT_SIZE > 0)
+  if (!arm_apcs_frame_needed () && CALLER_INTERWORKING_SLOT_SIZE > 0)
     mask |= 1 << ARM_HARD_FRAME_POINTER_REGNUM;
 
   /* LR will also be pushed if any lo regs are pushed.  */
@@ -9463,7 +9619,7 @@
 	     corrupted it, or 3) it was saved to align the stack on
 	     iWMMXt.  In case 1, restore IP into SP, otherwise just
 	     restore IP.  */
-	  if (frame_pointer_needed)
+	  if (arm_apcs_frame_needed ())
 	    {
 	      live_regs_mask &= ~ (1 << IP_REGNUM);
 	      live_regs_mask |=   (1 << SP_REGNUM);
@@ -9498,27 +9654,39 @@
 	     registers.  Note we can get here, even if
 	     frame_pointer_needed is true, but only if sp already
 	     points to the base of the saved core registers.  */
-	  if (live_regs_mask & (1 << SP_REGNUM))
+	  if (arm_abi == ARM_ABI_APCS32)
 	    {
-	      unsigned HOST_WIDE_INT stack_adjust;
-
-	      offsets = arm_get_frame_offsets ();
-	      stack_adjust = offsets->outgoing_args - offsets->saved_regs;
-	      gcc_assert (stack_adjust == 0 || stack_adjust == 4);
-
-	      if (stack_adjust && arm_arch5)
-		sprintf (instr, "ldm%sib\t%%|sp, {", conditional);
-	      else
+	      if (arm_apcs_frame_needed ())
+		sprintf (instr, "ldm%sea\t%%|fp, {", conditional);
+	      else if (live_regs_mask & (1 << SP_REGNUM))
+		sprintf (instr, "ldm%sfd\t%%|sp, {", conditional);
+              else
+		sprintf (instr, "ldm%sfd\t%%|sp!, {", conditional);
+            }
+	  else
+	    {
+	      if (live_regs_mask & (1 << SP_REGNUM))
 		{
-		  /* If we can't use ldmib (SA110 bug),
-		     then try to pop r3 instead.  */
-		  if (stack_adjust)
-		    live_regs_mask |= 1 << 3;
-		  sprintf (instr, "ldm%sfd\t%%|sp, {", conditional);
+		  unsigned HOST_WIDE_INT stack_adjust;
+		  
+		  offsets = arm_get_frame_offsets ();
+		  stack_adjust = offsets->outgoing_args - offsets->saved_regs;
+		  gcc_assert (stack_adjust == 0 || stack_adjust == 4);
+
+		  if (stack_adjust && arm_arch5)
+		    sprintf (instr, "ldm%sib\t%%|sp, {", conditional);
+		  else
+		    {
+		      /* If we can't use ldmib (SA110 bug),
+			 then try to pop r3 instead.  */
+		      if (stack_adjust)
+			live_regs_mask |= 1 << 3;
+		      sprintf (instr, "ldm%sfd\t%%|sp, {", conditional);
+		    }
 		}
+	      else
+		sprintf (instr, "ldm%sfd\t%%|sp!, {", conditional);
 	    }
-	  else
-	    sprintf (instr, "ldm%sfd\t%%|sp!, {", conditional);
 
 	  p = instr + strlen (instr);
 
@@ -9685,12 +9853,13 @@
   if (IS_NESTED (func_type))
     asm_fprintf (f, "\t%@ Nested: function declared inside another function.\n");
 
-  asm_fprintf (f, "\t%@ args = %d, pretend = %d, frame = %wd\n",
+  asm_fprintf (f, "\t%@ args = %d, pretend = %d, frame = %wd, outgoing = %d\n",
 	       current_function_args_size,
-	       current_function_pretend_args_size, frame_size);
+	       current_function_pretend_args_size, frame_size,
+	       current_function_outgoing_args_size);
 
   asm_fprintf (f, "\t%@ frame_needed = %d, uses_anonymous_args = %d\n",
-	       frame_pointer_needed,
+	       arm_apcs_frame_needed (),
 	       cfun->machine->uses_anonymous_args);
 
   if (cfun->machine->lr_save_eliminated)
@@ -9762,7 +9931,7 @@
     if (saved_regs_mask & (1 << reg))
       floats_offset += 4;
 
-  if (frame_pointer_needed)
+  if (arm_apcs_frame_needed ())
     {
       /* This variable is for the Virtual Frame Pointer, not VFP regs.  */
       int vfp_offset = offsets->frame;
@@ -9886,20 +10055,27 @@
       else
 	saved_regs_mask &= ~ (1 << PC_REGNUM);
 
-      /* We must use SP as the base register, because SP is one of the
-         registers being restored.  If an interrupt or page fault
-         happens in the ldm instruction, the SP might or might not
-         have been restored.  That would be bad, as then SP will no
-         longer indicate the safe area of stack, and we can get stack
-         corruption.  Using SP as the base register means that it will
-         be reset correctly to the original value, should an interrupt
-         occur.  If the stack pointer already points at the right
-         place, then omit the subtraction.  */
-      if (offsets->outgoing_args != (1 + (int) bit_count (saved_regs_mask))
-	  || current_function_calls_alloca)
-	asm_fprintf (f, "\tsub\t%r, %r, #%d\n", SP_REGNUM, FP_REGNUM,
-		     4 * bit_count (saved_regs_mask));
-      print_multi_reg (f, "ldmfd\t%r", SP_REGNUM, saved_regs_mask);
+      if (arm_abi == ARM_ABI_APCS32)
+	{
+	  print_multi_reg (f, "ldmea\t%r", FP_REGNUM, saved_regs_mask);
+	}
+      else
+	{
+	  /* We must use SP as the base register, because SP is one of the
+	     registers being restored.  If an interrupt or page fault
+	     happens in the ldm instruction, the SP might or might not
+	     have been restored.  That would be bad, as then SP will no
+	     longer indicate the safe area of stack, and we can get stack
+	     corruption.  Using SP as the base register means that it will
+	     be reset correctly to the original value, should an interrupt
+	     occur.  If the stack pointer already points at the right
+	     place, then omit the subtraction.  */
+	  if (offsets->outgoing_args != (1 + (int) bit_count (saved_regs_mask))
+	      || current_function_calls_alloca)
+	    asm_fprintf (f, "\tsub\t%r, %r, #%d\n", SP_REGNUM, FP_REGNUM,
+			 4 * bit_count (saved_regs_mask));
+	  print_multi_reg (f, "ldmfd\t%r", SP_REGNUM, saved_regs_mask);
+	}
 
       if (IS_INTERRUPT (func_type))
 	/* Interrupt handlers will have pushed the
@@ -9909,12 +10085,21 @@
   else
     {
       /* Restore stack pointer if necessary.  */
-      if (offsets->outgoing_args != offsets->saved_regs)
+      if (arm_abi != ARM_ABI_APCS32
+	  && offsets->outgoing_args != offsets->saved_regs)
 	{
 	  operands[0] = operands[1] = stack_pointer_rtx;
 	  operands[2] = GEN_INT (offsets->outgoing_args - offsets->saved_regs);
 	  output_add_immediate (operands);
 	}
+      else if (arm_abi == ARM_ABI_APCS32
+	       && (abs (offsets->frame_size) + offsets->outgoing_args))
+	{
+	  operands[0] = operands[1] = stack_pointer_rtx;
+	  operands[2] = GEN_INT (abs (offsets->frame_size)
+				 + offsets->outgoing_args);
+	  output_add_immediate (operands);
+	}
 
       if (arm_fpu_arch == FPUTYPE_FPA_EMU2)
 	{
@@ -10091,8 +10276,12 @@
 
       gcc_assert (!use_return_insn (FALSE, NULL)
 		  || !return_used_this_function
-		  || offsets->saved_regs == offsets->outgoing_args
-		  || frame_pointer_needed);
+		  || (arm_abi == ARM_ABI_APCS32
+		      && abs (offsets->frame_size
+			      + offsets->outgoing_args) <= 256)
+		  || (arm_abi != ARM_ABI_APCS32
+		      && offsets->saved_regs == offsets->outgoing_args)
+		  || arm_apcs_frame_needed ());
 
       /* Reset the ARM-specific per-function variables.  */
       after_arm_reorg = 0;
@@ -10310,6 +10499,47 @@
 }
 
 
+/* Given FROM and TO register numbers, say whether this elimination is
+   allowed.  Frame pointer elimination is automatically handled.  */
+int
+arm_can_eliminate (int from, int to)
+{
+  if (arm_abi == ARM_ABI_APCS32)
+    {
+      /* We can eliminate ARGP to STACKP if no alloca, no stack checks needed
+	 and frame not needed.  */
+      if (from == ARG_POINTER_REGNUM && to == STACK_POINTER_REGNUM
+	  && ! arm_apcs_frame_needed ())
+	return 1;
+  
+      /* FRAMEP can be eliminated to STACKP.  */
+      if (from == FRAME_POINTER_REGNUM && to == STACK_POINTER_REGNUM)
+	return 1;
+  
+      /* Can't do any other eliminations.  */
+      return 0;
+    }
+
+  /*  All eliminations are permissible.  Note that ARG_POINTER_REGNUM and
+      HARD_FRAME_POINTER_REGNUM are in fact the same thing.  If we need a frame
+      pointer, we must eliminate FRAME_POINTER_REGNUM into
+      HARD_FRAME_POINTER_REGNUM and not into STACK_POINTER_REGNUM or
+      ARG_POINTER_REGNUM.  */
+  if (to == FRAME_POINTER_REGNUM && from == ARG_POINTER_REGNUM)
+    return 0;
+  
+  if (to == STACK_POINTER_REGNUM && arm_apcs_frame_needed ())
+    return 0;
+      
+  if (to == ARM_HARD_FRAME_POINTER_REGNUM && TARGET_THUMB)
+    return 0;
+  
+  if (to == THUMB_HARD_FRAME_POINTER_REGNUM && TARGET_ARM)
+    return 0;
+  
+  return 1;
+}
+
 /* Compute the distance from register FROM to register TO.
    These can be the arg pointer (26), the soft frame pointer (25),
    the stack pointer (13) or the hard frame pointer (11).
@@ -10369,7 +10599,6 @@
   unsigned long func_type;
   int leaf;
   int saved;
-  HOST_WIDE_INT frame_size;
 
   offsets = &cfun->machine->stack_offsets;
 
@@ -10388,14 +10617,14 @@
 
   /* Initially this is the size of the local variables.  It will translated
      into an offset once we have determined the size of preceding data.  */
-  frame_size = ROUND_UP_WORD (get_frame_size ());
+  offsets->frame_size = ROUND_UP_WORD (get_frame_size ());
 
-  leaf = leaf_function_p ();
+  leaf = arm_leaf_function_p ();
 
   /* Space for variadic functions.  */
   offsets->saved_args = current_function_pretend_args_size;
 
-  offsets->frame = offsets->saved_args + (frame_pointer_needed ? 4 : 0);
+  offsets->frame = offsets->saved_args + (arm_apcs_frame_needed () ? 4 : 0);
 
   if (TARGET_ARM)
     {
@@ -10440,9 +10669,10 @@
   /* Saved registers include the stack frame.  */
   offsets->saved_regs = offsets->saved_args + saved;
   offsets->soft_frame = offsets->saved_regs + CALLER_INTERWORKING_SLOT_SIZE;
+
   /* A leaf function does not need any stack alignment if it has nothing
      on the stack.  */
-  if (leaf && frame_size == 0)
+  if (leaf && offsets->frame_size == 0 && arm_abi != ARM_ABI_APCS32)
     {
       offsets->outgoing_args = offsets->soft_frame;
       return offsets;
@@ -10453,7 +10683,10 @@
       && (offsets->soft_frame & 7))
     offsets->soft_frame += 4;
 
-  offsets->locals_base = offsets->soft_frame + frame_size;
+  if (arm_abi == ARM_ABI_APCS32)
+    offsets->locals_base = 0;
+  else
+    offsets->locals_base = offsets->soft_frame + offsets->frame_size;
   offsets->outgoing_args = (offsets->locals_base
 			    + current_function_outgoing_args_size);
 
@@ -10464,7 +10697,6 @@
 	offsets->outgoing_args += 4;
       gcc_assert (!(offsets->outgoing_args & 7));
     }
-
   return offsets;
 }
 
@@ -10475,9 +10707,30 @@
 HOST_WIDE_INT
 arm_compute_initial_elimination_offset (unsigned int from, unsigned int to)
 {
-  arm_stack_offsets *offsets;
+  arm_stack_offsets *offsets = arm_get_frame_offsets ();
+  
+  if (arm_abi == ARM_ABI_APCS32)
+    {
+      if (from == ARG_POINTER_REGNUM || to == ARG_POINTER_REGNUM)
+	{
+	  int stack_frame = arm_apcs_frame_needed () ? 16 : 0;
+	  /*printf ("elim: outgoing=%d, saved_regs=%d, frame_size=%d, saved_args=%d, stack_frame=%d\n",
+		  offsets->outgoing_args, offsets->saved_regs,
+		  offsets->frame_size, offsets->saved_args, stack_frame); */
+	  return (offsets->outgoing_args
+		  + (offsets->saved_regs - offsets->saved_args)
+		  + stack_frame
+		  + offsets->frame_size - 4);
+	}
 
-  offsets = arm_get_frame_offsets ();
+      /* return (offsets->outgoing_args + offsets->saved_regs
+	 + offsets->frame - 4); */
+  
+      if (from == FRAME_POINTER_REGNUM && to == STACK_POINTER_REGNUM)
+	return 0;
+  
+      abort ();
+    }
 
   /* OK, now we have enough information to compute the distances.
      There must be an entry in these switch tables for each pair
@@ -10502,7 +10755,7 @@
 	  if (offsets->frame == offsets->saved_regs)
 	    return 0;
 	  /* FIXME:  Not sure about this.  Maybe we should always return 0 ?  */
-	  return (frame_pointer_needed
+	  return (arm_apcs_frame_needed ()
 		  && cfun->static_chain_decl != NULL
 		  && ! cfun->machine->uses_anonymous_args) ? 4 : 0;
 
@@ -10548,15 +10801,77 @@
     }
 }
 
+/* Return non-zero if this is a leaf function.  */
+static int
+arm_leaf_function_p (void)
+{
+  if (cfun->machine->leaf < 0)
+    cfun->machine->leaf = leaf_function_p ();
+  
+  return cfun->machine->leaf;
+}
+
+/* Return 1 if the function prologue should contain an explicit
+   stack check.  */
+static bool
+arm_stack_check_needed (void)
+{
+  int frame_size;
+
+  /* Don't do any stack checking if it was not asked for, or when producing
+     RISC OS Module code.  */
+  if (! TARGET_APCS_STACK || TARGET_MODULE)
+    return false;
+  
+  /* We will always use stack checking for non-optimising
+     circumstances.  */
+  if (! optimize)
+    return true;
+
+  frame_size = ROUND_UP_WORD (get_frame_size ());
+  
+  /* Don't do any stack checking if the function is a leaf function
+     and the amount of stack actually needed <= 256 bytes.  */
+  if (arm_leaf_function_p () && abs (frame_size) <= 256)
+    return false;
+  
+  return true;
+}
+
+/* Return 1 if the function prologue really needs to setup an APCS
+   frame.  */
+bool
+arm_apcs_frame_needed (void)
+{
+  /* If we are not targeting the APCS, we will not use a stack frame.  */
+  if (! TARGET_APCS_FRAME)
+    return false;
+  
+  /* If we are not optimising, or we call alloca, we will always
+     setup a frame.  */
+  if (current_function_calls_alloca
+      || current_function_has_nonlocal_goto
+      || current_function_has_nonlocal_label
+      || ! optimize)
+    return true;
+  
+  /* A frame will need to be setup for the cases where there are external
+     function calls within the current function or there is a need
+     for definite stack checking.  */
+  if (! arm_leaf_function_p () || arm_stack_check_needed ())
+    return true;
+  
+  return false;
+}
+
 
 /* Generate the prologue instructions for entry into an ARM function.  */
 void
 arm_expand_prologue (void)
 {
-  int reg;
-  rtx amount;
+  int reg, frame_size;
   rtx insn;
-  rtx ip_rtx;
+  rtx ip_rtx, fp_rtx;
   unsigned long live_regs_mask;
   unsigned long func_type;
   int fp_offset = 0;
@@ -10571,6 +10886,15 @@
   if (IS_NAKED (func_type))
     return;
 
+#ifdef TARGET_RISCOSELF
+  if (flag_pic == 2 && arm_apcs_frame_needed())
+    {
+      /* If there's a stack extension call, then we need the PIC
+         register setting up before it for a call via the PLT.  */
+      current_function_uses_pic_offset_table = 1;
+    }
+#endif
+
   /* Make a copy of c_f_p_a_s as we may need to modify it locally.  */
   args_to_push = current_function_pretend_args_size;
 
@@ -10578,8 +10902,9 @@
   live_regs_mask = arm_compute_save_reg_mask ();
 
   ip_rtx = gen_rtx_REG (SImode, IP_REGNUM);
+  fp_rtx = gen_rtx_REG (SImode, FP_REGNUM);
 
-  if (frame_pointer_needed)
+  if (arm_apcs_frame_needed ())
     {
       if (IS_INTERRUPT (func_type))
 	{
@@ -10699,7 +11024,7 @@
      can be done with a single instruction.  */
   if ((func_type == ARM_FT_ISR || func_type == ARM_FT_FIQ)
       && (live_regs_mask & (1 << LR_REGNUM)) != 0
-      && ! frame_pointer_needed)
+      && ! arm_apcs_frame_needed ())
     emit_insn (gen_rtx_SET (SImode,
 			    gen_rtx_REG (SImode, LR_REGNUM),
 			    gen_rtx_PLUS (SImode,
@@ -10800,13 +11125,67 @@
 	}
     }
 
-  if (frame_pointer_needed)
-    {
+  offsets = arm_get_frame_offsets ();
+  if (arm_abi == ARM_ABI_APCS32)
+    frame_size = - offsets->frame_size - offsets->outgoing_args;
+  else
+    frame_size = offsets->saved_args + saved_regs - offsets->outgoing_args;
+
+#ifdef TARGET_RISCOSELF
+  if (flag_pic == 2)
+    arm_load_pic_register (0UL);
+#endif
+
+  if (arm_apcs_frame_needed ())
+    {      
       /* Create the new frame pointer.  */
       insn = GEN_INT (-(4 + args_to_push + fp_offset));
-      insn = emit_insn (gen_addsi3 (hard_frame_pointer_rtx, ip_rtx, insn));
+      insn = emit_insn (gen_addsi3 (fp_rtx, ip_rtx, insn));
       RTX_FRAME_RELATED_P (insn) = 1;
 
+#if defined(ARM_STKOVF_SPLIT_SMALL) && defined(ARM_STKOVF_SPLIT_BIG)
+      /* Explicit stack checks.  */
+      if (arm_abi == ARM_ABI_APCS32 && arm_stack_check_needed ())
+	{
+	  rtx last = get_last_insn ();
+	  rtx sl_reg = gen_rtx_REG (GET_MODE (stack_pointer_rtx), 10);
+	  if (frame_size <= -256)
+	    {
+	      rtx stkovf = gen_rtx_SYMBOL_REF (Pmode, ARM_STKOVF_SPLIT_BIG);
+	      emit_insn (gen_addsi3 (ip_rtx, stack_pointer_rtx,
+					GEN_INT (frame_size)));
+	      if (call_used_regs[8])
+		{
+		  rtx null = gen_rtx_SYMBOL_REF(Pmode, "__ABC__");
+		  insn = emit_insn(gen_rt_stkovf_v5_clobbered (ip_rtx, sl_reg, stkovf, null));
+		}
+	      else
+		insn = emit_insn (gen_rt_stkovf (ip_rtx, sl_reg, stkovf));
+	    }
+	  else
+	    {
+	      rtx stkovf = gen_rtx_SYMBOL_REF (Pmode, ARM_STKOVF_SPLIT_SMALL);
+	      if (call_used_regs[8])
+		{
+		  rtx null = gen_rtx_SYMBOL_REF(Pmode, "__ABC__");
+		  insn = emit_insn(gen_rt_stkovf_v5_clobbered (stack_pointer_rtx, sl_reg, stkovf, null));
+		}
+	      else
+	        insn = emit_insn (gen_rt_stkovf (stack_pointer_rtx,
+					       sl_reg, stkovf));
+	    }
+	  /* Create barrier to prevent real stack adjustment from being
+	     scheduled before call to stack checker.  */
+	  emit_insn (gen_blockage ());
+	  do
+	    {
+	      last = last ? NEXT_INSN(last) : get_insns ();
+	      RTX_FRAME_RELATED_P (last) = 1;
+	    }
+          while (last != insn);
+	}
+#endif
+
       if (IS_NESTED (func_type))
 	{
 	  /* Recover the static chain register.  */
@@ -10815,8 +11194,11 @@
 	    insn = gen_rtx_REG (SImode, 3);
 	  else /* if (current_function_pretend_args_size == 0) */
 	    {
-	      insn = gen_rtx_PLUS (SImode, hard_frame_pointer_rtx,
-				   GEN_INT (4));
+	      if (arm_abi == ARM_ABI_APCS32)
+		insn = gen_rtx_PLUS (SImode, fp_rtx, GEN_INT (4));
+	      else
+		insn = gen_rtx_PLUS (SImode, hard_frame_pointer_rtx,
+				     GEN_INT (4));
 	      insn = gen_frame_mem (SImode, insn);
 	    }
 
@@ -10826,18 +11208,14 @@
 	}
     }
 
-  offsets = arm_get_frame_offsets ();
-  if (offsets->outgoing_args != offsets->saved_args + saved_regs)
+  if (frame_size != 0)
     {
       /* This add can produce multiple insns for a large constant, so we
 	 need to get tricky.  */
       rtx last = get_last_insn ();
 
-      amount = GEN_INT (offsets->saved_args + saved_regs
-			- offsets->outgoing_args);
-
       insn = emit_insn (gen_addsi3 (stack_pointer_rtx, stack_pointer_rtx,
-				    amount));
+				    GEN_INT (frame_size)));
       do
 	{
 	  last = last ? NEXT_INSN (last) : get_insns ();
@@ -10848,13 +11226,20 @@
       /* If the frame pointer is needed, emit a special barrier that
 	 will prevent the scheduler from moving stores to the frame
 	 before the stack adjustment.  */
-      if (frame_pointer_needed)
+      if (arm_apcs_frame_needed ())
 	insn = emit_insn (gen_stack_tie (stack_pointer_rtx,
 					 hard_frame_pointer_rtx));
-    }
 
+       if (arm_abi == ARM_ABI_APCS32 && !optimize)
+	{
+	  insn = emit_insn (gen_movsi (frame_pointer_rtx, stack_pointer_rtx));
+	  RTX_FRAME_RELATED_P (insn) = 1;
+	}
+    }
 
-  if (flag_pic)
+#ifdef TARGET_RISCOSELF
+  if (flag_pic != 2 || TARGET_MODULE)
+#endif
     arm_load_pic_register (0UL);
 
   /* If we are profiling, make sure no instructions are scheduled before
@@ -10873,6 +11258,7 @@
       emit_insn (gen_prologue_use (gen_rtx_REG (SImode, LR_REGNUM)));
       cfun->machine->lr_save_eliminated = 1;
     }
+
 }
 
 /* If CODE is 'd', then the X is a condition operand and the instruction
@@ -11252,14 +11638,20 @@
       if (NEED_GOT_RELOC && flag_pic && making_const_table &&
 	  (GET_CODE (x) == SYMBOL_REF || GET_CODE (x) == LABEL_REF))
 	{
-	  if (GET_CODE (x) == SYMBOL_REF
-	      && (CONSTANT_POOL_ADDRESS_P (x)
-		  || SYMBOL_REF_LOCAL_P (x)))
+	  if (TARGET_MODULE) /* -mmodule */
 	    fputs ("(GOTOFF)", asm_out_file);
-	  else if (GET_CODE (x) == LABEL_REF)
-	    fputs ("(GOTOFF)", asm_out_file);
-	  else
+	  else if (flag_pic == 2) /* -fPIC */
 	    fputs ("(GOT)", asm_out_file);
+	  else if (flag_pic == 1) /* -fpic */
+	    {
+	      if (GET_CODE (x) == SYMBOL_REF
+		  && (CONSTANT_POOL_ADDRESS_P (x) || SYMBOL_REF_LOCAL_P (x)))
+		fputs ("(GOTOFF)", asm_out_file);
+	      else if (GET_CODE (x) == LABEL_REF)
+		fputs ("(GOTOFF)", asm_out_file);
+	      else
+		fputs ("(GOT)", asm_out_file);
+            }
 	}
       fputc ('\n', asm_out_file);
       return true;
@@ -11947,7 +12339,7 @@
 
   /* If we are using the stack pointer to point at the
      argument, then an offset of 0 is correct.  */
-  if ((TARGET_THUMB || !frame_pointer_needed)
+  if ((TARGET_THUMB || !arm_apcs_frame_needed ())
       && REGNO (addr) == SP_REGNUM)
     return 0;
 
@@ -12714,7 +13106,7 @@
     case ARM_BUILTIN_WSHUFH:
       icode = CODE_FOR_iwmmxt_wshufh;
       arg0 = TREE_VALUE (arglist);
-      arg1 = TREE_VALUE (TREE_CHAIN (arglist));
+      arg1 = TREE_VALUE (TREE_CHAIN (arglist)); 
       op0 = expand_expr (arg0, NULL_RTX, VOIDmode, 0);
       op1 = expand_expr (arg1, NULL_RTX, VOIDmode, 0);
       tmode = insn_data[icode].operand[0].mode;
@@ -13503,17 +13895,33 @@
 #if ARM_FT_UNKNOWN != 0
   machine->func_type = ARM_FT_UNKNOWN;
 #endif
+  machine->leaf = -1;
   return machine;
 }
 
 /* Return an RTX indicating where the return address to the
    calling function can be found.  */
 rtx
-arm_return_addr (int count, rtx frame ATTRIBUTE_UNUSED)
+arm_return_addr (int count, rtx frame)
 {
   if (count != 0)
     return NULL_RTX;
 
+  if (arm_abi == ARM_ABI_APCS32)
+    {
+      /* On RISC OS, the story is more complicated because calls to
+	 stack-extension code, or alloca can directly alter the
+	 return address of the current function.  We rely on the
+         target's run-time C library to provide an appropriate solution.  */
+      rtx function_call = gen_rtx_SYMBOL_REF (Pmode,
+					      "__builtin_return_address");
+      frame = gen_reg_rtx (Pmode);
+      emit_library_call_value (function_call, frame, LCT_NORMAL,
+			       Pmode, 1,
+			       GEN_INT (count), SImode);
+      return frame;
+    }
+
   return get_hard_reg_initial_val (Pmode, LR_REGNUM);
 }
 
@@ -13619,7 +14027,7 @@
   if (flag_pic)
     arm_load_pic_register (live_regs_mask);
 
-  if (!frame_pointer_needed && CALLER_INTERWORKING_SLOT_SIZE > 0)
+  if (!arm_apcs_frame_needed () && CALLER_INTERWORKING_SLOT_SIZE > 0)
     emit_move_insn (gen_rtx_REG (Pmode, ARM_HARD_FRAME_POINTER_REGNUM),
 		    stack_pointer_rtx);
 
@@ -13653,7 +14061,7 @@
 	     it now.  */
 	  for (regno = LAST_ARG_REGNUM + 1; regno <= LAST_LO_REGNUM; regno++)
 	    if (live_regs_mask & (1 << regno)
-		&& !(frame_pointer_needed
+		&& !(arm_apcs_frame_needed ()
 		     && (regno == THUMB_HARD_FRAME_POINTER_REGNUM)))
 	      break;
 
@@ -13711,7 +14119,7 @@
 	}
     }
 
-  if (frame_pointer_needed)
+  if (arm_apcs_frame_needed ())
     {
       amount = offsets->outgoing_args - offsets->locals_base;
 
@@ -13768,7 +14176,7 @@
   offsets = arm_get_frame_offsets ();
   amount = offsets->outgoing_args - offsets->saved_regs;
 
-  if (frame_pointer_needed)
+  if (arm_apcs_frame_needed ())
     {
       emit_insn (gen_movsi (stack_pointer_rtx, hard_frame_pointer_rtx));
       amount = offsets->locals_base - offsets->saved_regs;
@@ -15057,7 +15465,7 @@
     emit_move_insn (gen_rtx_REG (Pmode, LR_REGNUM), source);
   else
     {
-      if (frame_pointer_needed)
+      if (arm_apcs_frame_needed ())
 	addr = plus_constant(hard_frame_pointer_rtx, -4);
       else
 	{
@@ -15100,7 +15508,7 @@
       offsets = arm_get_frame_offsets ();
 
       /* Find the saved regs.  */
-      if (frame_pointer_needed)
+      if (arm_apcs_frame_needed ())
 	{
 	  delta = offsets->soft_frame - offsets->saved_args;
 	  reg = THUMB_HARD_FRAME_POINTER_REGNUM;
@@ -15157,13 +15565,98 @@
   return mode == SImode ? 255 : 0;
 }
 
+void
+arm_expand_save_stack_block (rtx save_area, rtx stack_pointer ATTRIBUTE_UNUSED)
+{
+  emit_library_call_value (gen_rtx_SYMBOL_REF (Pmode, "__gcc_alloca_save"),
+                           save_area, LCT_NORMAL, GET_MODE (save_area), 0);
+}
+
+void
+arm_expand_restore_stack_block (rtx stack_pointer ATTRIBUTE_UNUSED, rtx save_area)
+{
+  emit_library_call (gen_rtx_SYMBOL_REF (Pmode, "__gcc_alloca_restore"),
+		     LCT_NORMAL, VOIDmode,
+		     2,
+		     gen_rtx_REG (SImode, FP_REGNUM), SImode,
+		     save_area, Pmode);
+}
+
+void
+arm_expand_alloca_epilogue (void)
+{
+  rtx sym = gen_rtx_SYMBOL_REF (Pmode, "__gcc_alloca_free");
+#if 1
+  emit_insn (gen_call_no_clobber (sym));
+#else
+  emit_library_call (sym,
+		     LCT_NORMAL, VOIDmode,
+		     0);
+#endif
+}
+
+void
+arm_expand_allocate_stack (rtx memptr, rtx size)
+{
+  emit_library_call_value (gen_rtx_SYMBOL_REF (Pmode, "__gcc_alloca"),
+                           memptr, LCT_NORMAL, GET_MODE (memptr),
+                      	   1,
+			   size, SImode);
+
+}
+
+void
+arm_expand_save_stack_nonlocal (rtx *operands)
+{
+  rtx sa = XEXP (operands[0], 0);
+  /*rtx sp = operands[1];*/
+
+  /*emit_move_insn (sa, gen_rtx_REG (Pmode, FP_REGNUM));*/
+  emit_move_insn (gen_rtx_MEM (Pmode, plus_constant (sa, 0)),
+                  gen_rtx_REG (Pmode, FP_REGNUM));
+
+  /*emit_move_insn (gen_rtx_MEM (Pmode, plus_constant (sa, 4)), sp); */
+}
+
+
+void
+arm_expand_restore_stack_nonlocal (rtx *operands)
+{
+  rtx sa = XEXP (operands[1], 0);
+  rtx sp = operands[0];
+  rtx fp = gen_rtx_REG (Pmode, FP_REGNUM);
+
+  emit_insn (gen_rtx_SET (VOIDmode, sp, fp));
+  emit_insn (gen_rtx_SET (VOIDmode, fp,
+                          gen_rtx_MEM (Pmode, plus_constant (sa, 0))));
+
+  /*  emit_move_insn (gen_rtx_REG (Pmode, FP_REGNUM),
+      gen_rtx_MEM (Pmode, plus_constant (sa, 0)));
+      emit_move_insn (sp, gen_rtx_MEM (Pmode, plus_constant (sa, 4)));*/
+
+  /* In non-optimising circumstances, ensure we make v6 = sp */
+  if (! optimize)
+    emit_move_insn (frame_pointer_rtx, sp);
+}
+
+void
+arm_expand_nonlocal_goto (rtx *operands)
+{
+  rtx sa = XEXP (operands[2], 0);
+  rtx fp = gen_rtx_REG (Pmode, FP_REGNUM);
+
+  emit_move_insn (gen_rtx_MEM (Pmode, plus_constant (sa, 0)), fp);
+  emit_move_insn (gen_rtx_MEM (Pmode, plus_constant (sa, 4)),
+                  stack_pointer_rtx);
+  emit_indirect_jump (operands[1]);
+}
 
 /* Map internal gcc register numbers to DWARF2 register numbers.  */
 
 unsigned int
 arm_dbx_register_number (unsigned int regno)
 {
-  if (regno < 16)
+  if (regno < 16 || (regno >= 24 && regno <= 26))
     return regno;
 
   /* TODO: Legacy targets output FPA regs as registers 16-23 for backwards
@@ -15171,9 +15664,12 @@
   if (IS_FPA_REGNUM (regno))
     return (TARGET_AAPCS_BASED ? 96 : 16) + regno - FIRST_FPA_REGNUM;
 
-  if (IS_VFP_REGNUM (regno))
+  if (IS_VFP_REGNUM (regno) || regno == 95)
     return 64 + regno - FIRST_VFP_REGNUM;
 
+  if (IS_CIRRUS_REGNUM (regno))
+    return 28 + regno - FIRST_CIRRUS_FP_REGNUM;
+
   if (IS_IWMMXT_GR_REGNUM (regno))
     return 104 + regno - FIRST_IWMMXT_GR_REGNUM;
 
