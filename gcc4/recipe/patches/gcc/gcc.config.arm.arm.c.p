Index: gcc/config/arm/arm.c
===================================================================
--- gcc/config/arm/arm.c	(revision 169388)
+++ gcc/config/arm/arm.c	(working copy)
@@ -123,6 +123,8 @@
 #if TARGET_DLLIMPORT_DECL_ATTRIBUTES
 static tree arm_handle_notshared_attribute (tree *, tree, tree, int, bool *);
 #endif
+static int arm_leaf_function_p (void);
+static void require_pic_register (void);
 static void arm_output_function_epilogue (FILE *, HOST_WIDE_INT);
 static void arm_output_function_prologue (FILE *, HOST_WIDE_INT);
 static void thumb1_output_function_prologue (FILE *, HOST_WIDE_INT);
@@ -1030,7 +1032,8 @@
   {"atpcs",   ARM_ABI_ATPCS},
   {"aapcs",   ARM_ABI_AAPCS},
   {"iwmmxt",  ARM_ABI_IWMMXT},
-  {"aapcs-linux",   ARM_ABI_AAPCS_LINUX}
+  {"aapcs-linux",   ARM_ABI_AAPCS_LINUX},
+  {"apcs-32",  ARM_ABI_APCS32}
 };
 
 /* Supported TLS relocations.  */
@@ -1332,6 +1335,8 @@
 
 /* Implement TARGET_HANDLE_OPTION.  */
 
+static int target_apcs_stack_check_explicit; /* HACK */
+
 static bool
 arm_handle_option (size_t code, const char *arg, int value ATTRIBUTE_UNUSED)
 {
@@ -1357,6 +1362,10 @@
       arm_selected_tune = arm_find_cpu(arg, all_cores, "-mtune");
       return true;
 
+    case OPT_mapcs_stack_check:
+      target_apcs_stack_check_explicit = 1;
+      return true;
+
     default:
       return true;
     }
@@ -1650,12 +1659,38 @@
   if (TARGET_ARM && TARGET_CALLEE_INTERWORKING)
     warning (0, "enabling callee interworking support is only meaningful when compiling for the Thumb");
 
-  if (TARGET_APCS_STACK && !TARGET_APCS_FRAME)
+  if (OPTION_APCS_STACK && !TARGET_APCS_FRAME)
     {
       warning (0, "-mapcs-stack-check incompatible with -mno-apcs-frame");
       target_flags |= MASK_APCS_FRAME;
     }
 
+#ifdef TARGET_RISCOSELF
+  /* FIXME: we better would have a test arm_abi == ARM_ABI_APCS32 instead but
+     arm_abi is only defined further on.  */
+  /* Unless the option -m(no-)apcs-stack-check is explicitely set, we have it
+     default set for APCS-32 code unless we're generating module code
+     (-mmodule) as this is generally using a non-chunk stack anyway.  */
+  if (!target_apcs_stack_check_explicit && !TARGET_MODULE)
+    target_apcs_stack_check |= OPTION_MASK_APCS_STACK;
+
+  /* For RISC OS module support, we need -fpic specified.  */
+  if (TARGET_MODULE)
+    {
+      if (flag_pic == 2)
+	error ("-mmodule and -fPIC are incompatible");
+      flag_pic = 1;
+    }
+#endif
+
+#if !defined(ARM_STKOVF_SPLIT_SMALL) || !defined(ARM_STKOVF_SPLIT_BIG)
+  if (OPTION_APCS_STACK)
+    {
+      warning (0, "APCS stack checking not supported.  Ignored");
+      target_apcs_stack_check &= ~OPTION_MASK_APCS_STACK;
+    }
+#endif
+
   if (TARGET_POKE_FUNCTION_NAME)
     target_flags |= MASK_APCS_FRAME;
 
@@ -1673,8 +1708,10 @@
       && (TARGET_DEFAULT & MASK_APCS_FRAME))
     warning (0, "-g with -mno-apcs-frame may not give sensible debugging");
 
+#ifndef TARGET_RISCOSELF
   if (TARGET_APCS_FLOAT)
     warning (0, "passing floating point arguments in fp regs not yet supported");
+#endif
 
   /* Initialize boolean versions of the flags, for use in the arm.md file.  */
   arm_arch3m = (insn_flags & FL_ARCH3M) != 0;
@@ -1850,6 +1887,17 @@
   if (TARGET_SOFT_FLOAT)
     arm_fpu_attr = FPU_NONE;
 
+#ifdef TARGET_RISCOSELF
+  if (TARGET_SOFT_FLOAT && !TARGET_UNIXLIB)
+    sorry ("-mlibscl and -mfloat-abi=soft");
+
+  if (TARGET_UNIXLIB && TARGET_MODULE)
+    sorry ("-munixlib and -mmodule");
+
+  if (TARGET_THUMB)
+    sorry ("arm-unknown-riscos and thumb");
+#endif
+
   if (TARGET_AAPCS_BASED)
     {
       if (arm_abi == ARM_ABI_IWMMXT)
@@ -1925,17 +1973,23 @@
       flag_pic = 0;
     }
 
+#ifdef TARGET_RISCOSELF
+  /* RISC OS Loader always expects r7 to be used.  For module code this is r8.  */
+  if (flag_pic)
+    arm_pic_register = TARGET_MODULE ? 8 : 7;
+#else
   /* If stack checking is disabled, we can use r10 as the PIC register,
      which keeps r9 available.  The EABI specifies r9 as the PIC register.  */
   if (flag_pic && TARGET_SINGLE_PIC_BASE)
     {
       if (TARGET_VXWORKS_RTP)
 	warning (0, "RTP PIC is incompatible with -msingle-pic-base");
-      arm_pic_register = (TARGET_APCS_STACK || TARGET_AAPCS_BASED) ? 9 : 10;
+      arm_pic_register = (OPTION_APCS_STACK || TARGET_AAPCS_BASED) ? 9 : 10;
     }
 
   if (flag_pic && TARGET_VXWORKS_RTP)
     arm_pic_register = 9;
+#endif
 
   if (arm_pic_register_string != NULL)
     {
@@ -2271,11 +2325,14 @@
 
   /* So do interrupt functions that use the frame pointer and Thumb
      interrupt functions.  */
-  if (IS_INTERRUPT (func_type) && (frame_pointer_needed || TARGET_THUMB))
+  if (IS_INTERRUPT (func_type) && (arm_apcs_frame_needed () || TARGET_THUMB))
     return 0;
 
   offsets = arm_get_frame_offsets ();
-  stack_adjust = offsets->outgoing_args - offsets->saved_regs;
+  if (arm_abi == ARM_ABI_APCS32)
+    stack_adjust = abs (- offsets->frame_size - offsets->outgoing_args);
+  else
+    stack_adjust = offsets->outgoing_args - offsets->saved_regs;
 
   /* As do variadic functions.  */
   if (crtl->args.pretend_args_size
@@ -2283,10 +2340,10 @@
       /* Or if the function calls __builtin_eh_return () */
       || crtl->calls_eh_return
       /* Or if the function calls alloca */
-      || cfun->calls_alloca
+      || (cfun->calls_alloca && ! TARGET_APCS_FRAME)
       /* Or if there is a stack adjustment.  However, if the stack pointer
 	 is saved on the stack, we can use a pre-incrementing stack load.  */
-      || !(stack_adjust == 0 || (TARGET_APCS_FRAME && frame_pointer_needed
+      || !(stack_adjust == 0 || (TARGET_APCS_FRAME && arm_apcs_frame_needed ()
 				 && stack_adjust == 4)))
     return 0;
 
@@ -4544,6 +4601,7 @@
       pcum->aapcs_partial = 0;
       pcum->aapcs_arg_processed = false;
       pcum->aapcs_cprc_slot = -1;
+      pcum->fpa_nregs = 0;
       pcum->can_split = true;
 
       if (pcum->pcs_variant != ARM_PCS_AAPCS)
@@ -4564,6 +4622,7 @@
   /* On the ARM, the offset starts at 0.  */
   pcum->nregs = 0;
   pcum->iwmmxt_nregs = 0;
+  pcum->fpa_nregs = 0;
   pcum->can_split = true;
 
   /* Varargs vectors are treated the same as long long.
@@ -4646,6 +4705,16 @@
 	}
     }
 
+  if (TARGET_FPA && TARGET_APCS_FLOAT
+      && (mode == SFmode || mode == DFmode))
+    {
+      if (pcum->fpa_nregs <= 3)
+	return gen_rtx_REG (mode, pcum->fpa_nregs + FIRST_FPA_REGNUM);
+
+      pcum->can_split = false;
+      return NULL_RTX;
+    }
+
   /* Put doubleword aligned quantities in even register pairs.  */
   if (pcum->nregs & 1
       && ARM_DOUBLEWORD_ALIGN
@@ -4689,6 +4758,10 @@
   if (TARGET_IWMMXT_ABI && arm_vector_mode_supported_p (mode))
     return 0;
 
+  else if (TARGET_FPA && TARGET_APCS_FLOAT
+	   && (mode == DFmode || mode == SFmode))
+    return 0;
+
   if (NUM_ARG_REGS > nregs
       && (NUM_ARG_REGS < nregs + ARM_NUM_REGS2 (mode, type))
       && pcum->can_split)
@@ -4729,6 +4802,9 @@
 	  && pcum->named_count > pcum->nargs
 	  && TARGET_IWMMXT_ABI)
 	pcum->iwmmxt_nregs += 1;
+      else if (TARGET_FPA && TARGET_APCS_FLOAT
+	       && (mode == SFmode || mode == DFmode))
+	pcum->fpa_nregs += 1;
       else
 	pcum->nregs += ARM_NUM_REGS2 (mode, type);
     }
@@ -5077,6 +5153,25 @@
   if (IS_STACKALIGN (func_type))
     return false;
 
+  /* When stack checking we can only allow a sibcall when there is
+     exactly one function call, and that is the candidate for the
+     sibcall.  In other words, we can prevent a full stack-frame
+     being setup.  */
+  if (OPTION_APCS_STACK
+      && (arm_apcs_frame_needed () || crtl->outgoing_args_size))
+    return false;
+
+#ifdef TARGET_RISCOSELF
+  /* When compiling PIC don't tail-call an external function. The PLT
+     requires that the PIC register be loaded by the caller, but the caller
+     must restore the PIC register before the call.  */
+  if (flag_pic == 2 && TREE_PUBLIC (decl))
+    {
+      require_pic_register ();
+      return false;
+    }
+#endif
+
   /* Everything else is ok.  */
   return true;
 }
@@ -5171,6 +5266,75 @@
 	  reg = gen_reg_rtx (Pmode);
 	}
 
+#ifdef TARGET_RISCOSELF
+      if (TARGET_MODULE
+	  && GET_CODE (orig) == SYMBOL_REF)
+	{
+	  /* We ignore LABEL_REFs here because the SYMBOL_REF_DECL macro
+	     gives erratic results, ie:
+	     a) It may return a seemingly valid looking pointer that causes
+		a segmentation fault when accessed.
+	     b) We get an obviously wrong value like 0x2 which causes an
+		ICE at the next gcc_assert().
+	     c) or sometimes it works correctly giving a pointer that can
+		be accessed without error.
+	     Multiple runs on the same code can give different results, as above.
+	     This may be an indication of a bug somewhere else or we are
+	     making incorrect assumptions.  */
+	  tree ftree;
+
+	  insn = emit_insn (gen_pic_load_addr_32bit (reg, orig));
+
+	  /* Don't do reallocation based on PIC register when our
+	     symbol is a function or read-only data, i.e. do this only for
+	     global/local writable variables.  */
+	  ftree = SYMBOL_REF_DECL (orig);
+
+	  /* If ftree is NULL, then we have a compiler generated symbol
+	     with no declaration. These always seem to be anchor symbols,
+	     ie, those named ".LANCHOR". The first symbol in the
+	     corresponding block appears to be the symbol the anchor
+	     refers to, so we can get the declaration from that.  */
+	  if (!ftree && SYMBOL_REF_ANCHOR_P (orig))
+	    {
+	      struct object_block *block = SYMBOL_REF_BLOCK (orig);
+	      
+	      if (block)
+	        {
+		  rtx decl_object = VEC_index (rtx, block->objects, 0);
+		  if (decl_object)
+		    ftree = SYMBOL_REF_DECL (decl_object);
+	        }
+	    }
+#if 0
+	  printf ("legitimize_pic_address: sym=%s, ftree=%p\n",
+		  XSTR (orig, 0), (void *)ftree);
+	  if (ftree != NULL)
+	    printf ("  tree code=%d (func %d, var %d), RO data=%d, const data=%d %d\n",
+		    TREE_CODE (ftree), FUNCTION_DECL, VAR_DECL,
+		    decl_readonly_section (ftree, 1),
+		    TREE_READONLY (ftree),
+		    TREE_CONSTANT (ftree));
+#endif
+	  gcc_assert (ftree == NULL || TREE_CODE (ftree) == FUNCTION_DECL || TREE_CODE (ftree) == VAR_DECL);
+	  if (ftree != NULL
+	      && TREE_CODE (ftree) == VAR_DECL
+	      && ! decl_readonly_section (ftree, 1)
+	      && ! (TREE_READONLY (ftree) || TREE_CONSTANT (ftree)))
+	    {
+#if 0
+	      printf("  -> applied OFFSET\n");
+#endif
+
+	      /* If this function doesn't have a pic register, create one now.  */
+	      require_pic_register ();
+
+	      insn = emit_insn (gen_addsi3 (reg, reg, pic_offset_table_rtx));
+	    }
+	}
+      else
+	{
+#endif
       /* VxWorks does not impose a fixed gap between segments; the run-time
 	 gap can be different from the object-file gap.  We therefore can't
 	 use GOTOFF unless we are absolutely sure that the symbol is in the
@@ -5201,6 +5365,7 @@
 
 	  insn = emit_insn (pat);
 	}
+	}
 
       /* Put a REG_EQUAL note on this insn, so that it can be optimized
 	 by loop.  */
@@ -5350,6 +5515,17 @@
   gcc_assert (flag_pic);
 
   pic_reg = cfun->machine->pic_reg;
+  if (TARGET_MODULE)
+    {
+      rtx sl_reg, sb_ref;
+
+      sl_reg = gen_rtx_REG (Pmode, SL_REGNUM);
+      sb_ref = gen_rtx_MEM (Pmode,
+			    gen_rtx_PLUS (Pmode, sl_reg, GEN_INT (-536)));
+      MEM_READONLY_P (sb_ref) = 1;
+      emit_insn (gen_rtx_SET (SImode, pic_reg, sb_ref));
+    }
+  else
   if (TARGET_VXWORKS_RTP)
     {
       pic_rtx = gen_rtx_SYMBOL_REF (Pmode, VXWORKS_GOTT_BASE);
@@ -5372,7 +5548,7 @@
 
       /* On the ARM the PC register contains 'dot + 8' at the time of the
 	 addition, on the Thumb it is 'dot + 4'.  */
-      pic_rtx = plus_constant (l1, TARGET_ARM ? 8 : 4);
+      pic_rtx = plus_constant (l1, TARGET_ARM && flag_pic != 2 ? 8 : 4);
       pic_rtx = gen_rtx_UNSPEC (Pmode, gen_rtvec (1, pic_rtx),
 				UNSPEC_GOTSYM_OFF);
       pic_rtx = gen_rtx_CONST (Pmode, pic_rtx);
@@ -5381,7 +5557,15 @@
 	{
 	  emit_insn (gen_pic_load_addr_32bit (pic_reg, pic_rtx));
 	  if (TARGET_ARM)
-	    emit_insn (gen_pic_add_dot_plus_eight (pic_reg, pic_reg, labelno));
+	    {
+	      emit_insn (gen_pic_add_dot_plus_eight (pic_reg, pic_reg, labelno));
+	      if (flag_pic == 2) /* -fPIC */
+		{
+		  emit_insn (gen_rt_loadpic (pic_reg,
+					     gen_rtx_REG (SImode, PIC_PLT_SCRATCH_REGNUM)));
+		  emit_insn (gen_blockage ());
+		}
+	    }
 	  else
 	    emit_insn (gen_pic_add_dot_plus_four (pic_reg, pic_reg, labelno));
 	}
@@ -13944,7 +14128,7 @@
 	 compromise save just the frame pointers.  Combined with the link
 	 register saved elsewhere this should be sufficient to get
 	 a backtrace.  */
-      if (frame_pointer_needed)
+      if (arm_apcs_frame_needed ())
 	save_reg_mask |= 1 << HARD_FRAME_POINTER_REGNUM;
       if (df_regs_ever_live_p (ARM_HARD_FRAME_POINTER_REGNUM))
 	save_reg_mask |= 1 << ARM_HARD_FRAME_POINTER_REGNUM;
@@ -13960,7 +14144,7 @@
 	  save_reg_mask |= (1 << reg);
 
       /* Handle the frame pointer as a special case.  */
-      if (frame_pointer_needed)
+      if (arm_apcs_frame_needed ())
 	save_reg_mask |= 1 << HARD_FRAME_POINTER_REGNUM;
 
       /* If we aren't loading the PIC register,
@@ -13969,7 +14153,8 @@
 	  && !TARGET_SINGLE_PIC_BASE
 	  && arm_pic_register != INVALID_REGNUM
 	  && (df_regs_ever_live_p (PIC_OFFSET_TABLE_REGNUM)
-	      || crtl->uses_pic_offset_table))
+	      || crtl->uses_pic_offset_table
+	      || !arm_leaf_function_p ()))
 	save_reg_mask |= 1 << PIC_OFFSET_TABLE_REGNUM;
 
       /* The prologue will copy SP into R0, so save it.  */
@@ -14004,7 +14189,7 @@
   unsigned long func_type = arm_current_func_type ();
   int static_chain_stack_bytes = 0;
 
-  if (TARGET_APCS_FRAME && frame_pointer_needed && TARGET_ARM &&
+  if (TARGET_APCS_FRAME && arm_apcs_frame_needed () && TARGET_ARM &&
       IS_NESTED (func_type) &&
       df_regs_ever_live_p (3) && crtl->args.pretend_args_size == 0)
     static_chain_stack_bytes = 4;
@@ -14030,7 +14215,7 @@
 
   /* If we are creating a stack frame, then we must save the frame pointer,
      IP (which will hold the old stack pointer), LR and the PC.  */
-  if (TARGET_APCS_FRAME && frame_pointer_needed && TARGET_ARM)
+  if (TARGET_APCS_FRAME && arm_apcs_frame_needed () && TARGET_ARM)
     save_reg_mask |=
       (1 << ARM_HARD_FRAME_POINTER_REGNUM)
       | (1 << IP_REGNUM)
@@ -14120,7 +14305,7 @@
     mask |= 1 << PIC_OFFSET_TABLE_REGNUM;
 
   /* See if we might need r11 for calls to _interwork_r11_call_via_rN().  */
-  if (!frame_pointer_needed && CALLER_INTERWORKING_SLOT_SIZE > 0)
+  if (!arm_apcs_frame_needed () && CALLER_INTERWORKING_SLOT_SIZE > 0)
     mask |= 1 << ARM_HARD_FRAME_POINTER_REGNUM;
 
   /* LR will also be pushed if any lo regs are pushed.  */
@@ -14281,7 +14466,7 @@
 	     corrupted it, or 3) it was saved to align the stack on
 	     iWMMXt.  In case 1, restore IP into SP, otherwise just
 	     restore IP.  */
-	  if (frame_pointer_needed)
+	  if (arm_apcs_frame_needed ())
 	    {
 	      live_regs_mask &= ~ (1 << IP_REGNUM);
 	      live_regs_mask |=   (1 << SP_REGNUM);
@@ -14314,8 +14499,18 @@
 
 	  /* Generate the load multiple instruction to restore the
 	     registers.  Note we can get here, even if
-	     frame_pointer_needed is true, but only if sp already
+	     arm_apcs_frame_needed () is true, but only if sp already
 	     points to the base of the saved core registers.  */
+	  if (arm_abi == ARM_ABI_APCS32)
+	    {
+	      if (arm_apcs_frame_needed ())
+		sprintf (instr, "ldm%sea\t%%|fp, {", conditional);
+	      else if (live_regs_mask & (1 << SP_REGNUM))
+		sprintf (instr, "ldm%sfd\t%%|sp, {", conditional);
+	      else
+		sprintf (instr, "ldm%sfd\t%%|sp!, {", conditional);
+	    }
+	  else
 	  if (live_regs_mask & (1 << SP_REGNUM))
 	    {
 	      unsigned HOST_WIDE_INT stack_adjust;
@@ -14516,12 +14711,13 @@
   if (IS_STACKALIGN (func_type))
     asm_fprintf (f, "\t%@ Stack Align: May be called with mis-aligned SP.\n");
 
-  asm_fprintf (f, "\t%@ args = %d, pretend = %d, frame = %wd\n",
+  asm_fprintf (f, "\t%@ args = %d, pretend = %d, frame = %wd, outgoing = %d\n",
 	       crtl->args.size,
-	       crtl->args.pretend_args_size, frame_size);
+	       crtl->args.pretend_args_size, frame_size,
+	       crtl->outgoing_args_size);
 
   asm_fprintf (f, "\t%@ frame_needed = %d, uses_anonymous_args = %d\n",
-	       frame_pointer_needed,
+	       arm_apcs_frame_needed (),
 	       cfun->machine->uses_anonymous_args);
 
   if (cfun->machine->lr_save_eliminated)
@@ -14588,7 +14784,7 @@
     if (saved_regs_mask & (1 << reg))
       floats_offset += 4;
 
-  if (TARGET_APCS_FRAME && frame_pointer_needed && TARGET_ARM)
+  if (TARGET_APCS_FRAME && arm_apcs_frame_needed () && TARGET_ARM)
     {
       /* This variable is for the Virtual Frame Pointer, not VFP regs.  */
       int vfp_offset = offsets->frame;
@@ -14712,6 +14908,10 @@
       else
 	saved_regs_mask &= ~ (1 << PC_REGNUM);
 
+      if (arm_abi == ARM_ABI_APCS32)
+	print_multi_reg (f, "ldmea\t%r, ", FP_REGNUM, saved_regs_mask, 0);
+      else
+	{
       /* We must use SP as the base register, because SP is one of the
          registers being restored.  If an interrupt or page fault
          happens in the ldm instruction, the SP might or might not
@@ -14726,6 +14926,7 @@
 	asm_fprintf (f, "\tsub\t%r, %r, #%d\n", SP_REGNUM, FP_REGNUM,
 		     4 * bit_count (saved_regs_mask));
       print_multi_reg (f, "ldmfd\t%r, ", SP_REGNUM, saved_regs_mask, 0);
+	}
 
       if (IS_INTERRUPT (func_type))
 	/* Interrupt handlers will have pushed the
@@ -14754,8 +14955,19 @@
       unsigned HOST_WIDE_INT amount;
       int rfe;
       /* Restore stack pointer if necessary.  */
-      if (TARGET_ARM && frame_pointer_needed)
+      if (arm_abi == ARM_ABI_APCS32)
 	{
+	  if (abs (offsets->frame_size) + offsets->outgoing_args)
+	    {
+	      operands[0] = operands[1] = stack_pointer_rtx;
+	      operands[2] = GEN_INT (abs (offsets->frame_size)
+					  + offsets->outgoing_args);
+	      output_add_immediate (operands);
+	    }
+	}
+      else
+      if (TARGET_ARM && arm_apcs_frame_needed ())
+	{
 	  operands[0] = stack_pointer_rtx;
 	  operands[1] = hard_frame_pointer_rtx;
 	  
@@ -14764,7 +14976,7 @@
 	}
       else
 	{
-	  if (frame_pointer_needed)
+	  if (arm_apcs_frame_needed ())
 	    {
 	      /* For Thumb-2 restore sp from the frame pointer.
 		 Operand restrictions mean we have to incrememnt FP, then copy
@@ -14812,7 +15024,7 @@
 	      operands[2] = GEN_INT (amount);
 	      output_add_immediate (operands);
 	    }
-	  if (frame_pointer_needed)
+	  if (arm_apcs_frame_needed ())
 	    asm_fprintf (f, "\tmov\t%r, %r\n",
 			 SP_REGNUM, HARD_FRAME_POINTER_REGNUM);
 	}
@@ -15013,8 +15225,11 @@
 
       gcc_assert (!use_return_insn (FALSE, NULL)
 		  || (cfun->machine->return_used_this_function != 0)
-		  || offsets->saved_regs == offsets->outgoing_args
-		  || frame_pointer_needed);
+		  || (arm_abi != ARM_ABI_APCS32
+		      && offsets->saved_regs == offsets->outgoing_args)
+		  || (arm_abi == ARM_ABI_APCS32
+		      && abs (offsets->frame_size + offsets->outgoing_args) <= 256)
+		  || arm_apcs_frame_needed ());
 
       /* Reset the ARM-specific per-function variables.  */
       after_arm_reorg = 0;
@@ -15354,15 +15569,23 @@
 
   /* Initially this is the size of the local variables.  It will translated
      into an offset once we have determined the size of preceding data.  */
+#ifndef TARGET_RISCOSELF
   frame_size = ROUND_UP_WORD (get_frame_size ());
+#else
+  offsets->frame_size = frame_size = ROUND_UP_WORD (get_frame_size ());
+#endif
 
+#ifndef TARGET_RISCOSELF
   leaf = leaf_function_p ();
+#else
+  leaf = arm_leaf_function_p ();
+#endif
 
   /* Space for variadic functions.  */
   offsets->saved_args = crtl->args.pretend_args_size;
 
   /* In Thumb mode this is incorrect, but never used.  */
-  offsets->frame = offsets->saved_args + (frame_pointer_needed ? 4 : 0) +
+  offsets->frame = offsets->saved_args + (arm_apcs_frame_needed () ? 4 : 0) +
                    arm_compute_static_chain_stack_bytes();
 
   if (TARGET_32BIT)
@@ -15415,7 +15638,7 @@
   offsets->soft_frame = offsets->saved_regs + CALLER_INTERWORKING_SLOT_SIZE;
   /* A leaf function does not need any stack alignment if it has nothing
      on the stack.  */
-  if (leaf && frame_size == 0)
+  if (leaf && frame_size == 0 && arm_abi != ARM_ABI_APCS32)
     {
       offsets->outgoing_args = offsets->soft_frame;
       offsets->locals_base = offsets->soft_frame;
@@ -15461,7 +15684,10 @@
 	}
     }
 
-  offsets->locals_base = offsets->soft_frame + frame_size;
+  if (arm_abi == ARM_ABI_APCS32)
+    offsets->locals_base = 0;
+  else
+    offsets->locals_base = offsets->soft_frame + frame_size;
   offsets->outgoing_args = (offsets->locals_base
 			    + crtl->outgoing_args_size);
 
@@ -15487,6 +15713,29 @@
 
   offsets = arm_get_frame_offsets ();
 
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
+
+      /* return (offsets->outgoing_args + offsets->saved_regs
+	 + offsets->frame - 4); */
+  
+      if (from == FRAME_POINTER_REGNUM && to == STACK_POINTER_REGNUM)
+	return 0;
+  
+      abort ();
+    }
+
   /* OK, now we have enough information to compute the distances.
      There must be an entry in these switch tables for each pair
      of registers in ELIMINABLE_REGS, even if some of the entries
@@ -15563,13 +15812,116 @@
 bool
 arm_can_eliminate (const int from, const int to)
 {
+  if (arm_abi == ARM_ABI_APCS32)
+    {
+      /* We can eliminate ARGP to STACKP if no alloca, no stack checks needed
+	 and frame not needed.  */
+      if (from == ARG_POINTER_REGNUM && to == STACK_POINTER_REGNUM
+	  && ! arm_apcs_frame_needed ())
+	return true;
+  
+      /* FRAMEP can be eliminated to STACKP.  */
+      if (from == FRAME_POINTER_REGNUM && to == STACK_POINTER_REGNUM)
+	return true;
+  
+      /* Can't do any other eliminations.  */
+      return false;
+    }
+
   return ((to == FRAME_POINTER_REGNUM && from == ARG_POINTER_REGNUM) ? false :
-          (to == STACK_POINTER_REGNUM && frame_pointer_needed) ? false :
+          (to == STACK_POINTER_REGNUM && arm_apcs_frame_needed ()) ? false :
           (to == ARM_HARD_FRAME_POINTER_REGNUM && TARGET_THUMB) ? false :
           (to == THUMB_HARD_FRAME_POINTER_REGNUM && TARGET_ARM) ? false :
            true);
 }
+ 
+/* Return non-zero if this is a leaf function.  */
+static int
+arm_leaf_function_p (void)
+{
+#ifdef TARGET_RISCOSELF
+  /* This function is designed to cache the result of leaf_function_p()
+     on the first call and then return the cached result on future calls.
+     However, it would seem that the first call isn't always correct and
+     as this result is used to determine whether stack checking is
+     required, we end up with non-leaf functions without any stack
+     checking at all. If a number of these functions call each other, it
+     leads to a corrupt stack.
+     We could forget the cached result (as newer versions of GCC do) and
+     just call leaf_function_p() every time, but the result tends to change
+     constantly for the same function while GCC rearranges code and
+     optimises things away, etc, and of course right at the crucial point
+     where the need for stack checking is being determined, we get the
+     wrong result.
+     So I've used a more conservative approach. After initially deciding
+     that a function is leaf, GCC can change its mind and change a leaf
+     function into a non-leaf function, but once it becomes a non-leaf
+     function, it stays like that. This may lead to some leaf functions
+     gaining a stack frame and stack check when they don't need them, but
+     at least we shouldn't get non-leaf functions without stack checks.  */
+  int leaf = leaf_function_p ();
+  
+  if (cfun->machine->leaf < 0 || (cfun->machine->leaf == 1 && leaf == 0))
+    cfun->machine->leaf = leaf;
+#else
+  if (cfun->machine->leaf < 0)
+    cfun->machine->leaf = leaf_function_p ();
+#endif
+  return cfun->machine->leaf;
+}
 
+/* Return 1 if the function prologue should contain an explicit
+   stack check.  */
+static bool
+arm_stack_check_needed (void)
+{
+  int frame_size;
+
+  /* Don't do any stack checking if it was not asked for.  */
+  if (!OPTION_APCS_STACK)
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
+  if (cfun->calls_alloca
+      || crtl->has_nonlocal_goto
+      || cfun->has_nonlocal_label
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
 /* Emit RTL to save coprocessor registers on function entry.  Returns the
    number of bytes pushed.  */
 
@@ -15706,7 +16058,9 @@
 void
 arm_expand_prologue (void)
 {
+#ifndef TARGET_RISCOSELF
   rtx amount;
+#endif
   rtx insn;
   rtx ip_rtx;
   unsigned long live_regs_mask;
@@ -15714,6 +16068,7 @@
   int fp_offset = 0;
   int saved_pretend_args = 0;
   int saved_regs = 0;
+  int frame_size;
   unsigned HOST_WIDE_INT args_to_push;
   arm_stack_offsets *offsets;
 
@@ -15723,6 +16078,16 @@
   if (IS_NAKED (func_type))
     return;
 
+#ifdef TARGET_RISCOSELF
+  /* FIXME: this cause an assert in require_pic_register().  */
+  if (0 && flag_pic == 2 && arm_apcs_frame_needed())
+    {
+      /* If there's a stack extension call, then we need the PIC
+         register setting up before it for a call via the PLT.  */
+      require_pic_register ();
+    }
+#endif
+
   /* Make a copy of c_f_p_a_s as we may need to modify it locally.  */
   args_to_push = crtl->args.pretend_args_size;
 
@@ -15767,7 +16132,7 @@
   /* For APCS frames, if IP register is clobbered
      when creating frame, save that register in a special
      way.  */
-  if (TARGET_APCS_FRAME && frame_pointer_needed && TARGET_ARM)
+  if (TARGET_APCS_FRAME && arm_apcs_frame_needed () && TARGET_ARM)
     {
       if (IS_INTERRUPT (func_type))
 	{
@@ -15874,7 +16239,7 @@
      can be done with a single instruction.  */
   if ((func_type == ARM_FT_ISR || func_type == ARM_FT_FIQ)
       && (live_regs_mask & (1 << LR_REGNUM)) != 0
-      && !(frame_pointer_needed && TARGET_APCS_FRAME)
+      && !(arm_apcs_frame_needed () && TARGET_APCS_FRAME)
       && TARGET_ARM)
     {
       rtx lr = gen_rtx_REG (SImode, LR_REGNUM);
@@ -15885,7 +16250,7 @@
   if (live_regs_mask)
     {
       saved_regs += bit_count (live_regs_mask) * 4;
-      if (optimize_size && !frame_pointer_needed
+      if (optimize_size && !arm_apcs_frame_needed ()
 	  && saved_regs == offsets->saved_regs - offsets->saved_args)
 	{
 	  /* If no coprocessor registers are being pushed and we don't have
@@ -15912,7 +16277,17 @@
   if (! IS_VOLATILE (func_type))
     saved_regs += arm_save_coproc_regs ();
 
-  if (frame_pointer_needed && TARGET_ARM)
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
+  if (arm_apcs_frame_needed () && TARGET_ARM)
     {
       /* Create the new frame pointer.  */
       if (TARGET_APCS_FRAME)
@@ -15921,6 +16296,48 @@
 	  insn = emit_insn (gen_addsi3 (hard_frame_pointer_rtx, ip_rtx, insn));
 	  RTX_FRAME_RELATED_P (insn) = 1;
 
+#if defined(ARM_STKOVF_SPLIT_SMALL) && defined(ARM_STKOVF_SPLIT_BIG)
+      /* Explicit stack checks.  */
+      if (arm_stack_check_needed ())
+	{
+	  rtx last = get_last_insn ();
+	  rtx sl_reg = gen_rtx_REG (GET_MODE (stack_pointer_rtx), SL_REGNUM);
+	  if (frame_size <= -256)
+	    {
+	      rtx stkovf = gen_rtx_SYMBOL_REF (Pmode, ARM_STKOVF_SPLIT_BIG);
+	      emit_insn (gen_addsi3 (ip_rtx, stack_pointer_rtx,
+				     GEN_INT (frame_size)));
+	      if (call_used_regs[8])
+		{
+		  rtx null = gen_rtx_SYMBOL_REF(Pmode, "__ABC__");
+		  insn = emit_insn (gen_rt_stkovf_v5_clobbered (ip_rtx, sl_reg, stkovf, null));
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
+		  insn = emit_insn (gen_rt_stkovf_v5_clobbered (stack_pointer_rtx, sl_reg, stkovf, null));
+		}
+	      else
+	        insn = emit_insn (gen_rt_stkovf (stack_pointer_rtx, sl_reg, stkovf));
+	    }
+	  /* Create barrier to prevent real stack adjustment from being
+	     scheduled before call to stack checker.  */
+	  emit_insn (gen_blockage ());
+	  do
+	    {
+	      last = last ? NEXT_INSN(last) : get_insns ();
+	      RTX_FRAME_RELATED_P (last) = 1;
+	    }
+	  while (last != insn);
+	}
+#endif
+
 	  if (IS_NESTED (func_type))
 	    {
 	      /* Recover the static chain register.  */
@@ -15950,17 +16367,14 @@
     current_function_static_stack_size
       = offsets->outgoing_args - offsets->saved_args;
 
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
@@ -15971,16 +16385,29 @@
       /* If the frame pointer is needed, emit a special barrier that
 	 will prevent the scheduler from moving stores to the frame
 	 before the stack adjustment.  */
-      if (frame_pointer_needed)
+      if (arm_apcs_frame_needed ())
 	insn = emit_insn (gen_stack_tie (stack_pointer_rtx,
 					 hard_frame_pointer_rtx));
+
+      if (arm_abi == ARM_ABI_APCS32
+	  && (!optimize
+	      || (cfun->calls_alloca && !OPTION_APCS_STACK)))
+	{
+	  /* These are the cases when we still have an uneliminated
+	     FRAME_POINTER_REGNUM usage: either in an unoptimized case, either
+	     when the function calls alloca and we don't have APCS stack
+	     checking which results in directly allocating alloca memory
+	     on the stack.  */
+	  insn = emit_insn (gen_movsi (frame_pointer_rtx, stack_pointer_rtx));
+	  RTX_FRAME_RELATED_P (insn) = 1;
+	}
     }
 
 
-  if (frame_pointer_needed && TARGET_THUMB2)
+  if (arm_apcs_frame_needed () && TARGET_THUMB2)
     thumb_set_frame_pointer (offsets);
 
-  if (flag_pic && arm_pic_register != INVALID_REGNUM)
+  if ((flag_pic != 2 || TARGET_MODULE) && arm_pic_register != INVALID_REGNUM)
     {
       unsigned long mask;
 
@@ -16884,6 +17311,11 @@
       if (NEED_GOT_RELOC && flag_pic && making_const_table &&
 	  (GET_CODE (x) == SYMBOL_REF || GET_CODE (x) == LABEL_REF))
 	{
+	  if (TARGET_MODULE) /* -mmodule */
+ 	    fputs ("(GOTOFF)", asm_out_file);
+	  else if (flag_pic == 2) /* -fPIC */
+ 	    fputs ("(GOT)", asm_out_file);
+	  else /* -fpic */
 	  /* See legitimize_pic_address for an explanation of the
 	     TARGET_VXWORKS_RTP check.  */
 	  if (TARGET_VXWORKS_RTP
@@ -16972,7 +17404,11 @@
 static void
 arm_elf_asm_constructor (rtx symbol, int priority)
 {
+#ifdef TARGET_RISCOSELF
+  default_named_section_asm_out_constructor (symbol, priority);
+#else
   arm_elf_asm_cdtor (symbol, priority, /*is_ctor=*/true);
+#endif
 }
 
 /* Add a function to the list of static destructors.  */
@@ -17722,7 +18158,7 @@
   /* If we are using the stack pointer to point at the
      argument, then an offset of 0 is correct.  */
   /* ??? Check this is consistent with thumb2 frame layout.  */
-  if ((TARGET_THUMB || !frame_pointer_needed)
+  if ((TARGET_THUMB || !arm_apcs_frame_needed ())
       && REGNO (addr) == SP_REGNUM)
     return 0;
 
@@ -20402,7 +20838,7 @@
   unsigned long high_regs_pushed = bit_count (live_regs_mask & 0x0f00);
   int n_free, reg_base;
 
-  if (!for_prologue && frame_pointer_needed)
+  if (!for_prologue && arm_apcs_frame_needed ())
     amount = offsets->locals_base - offsets->saved_regs;
   else
     amount = offsets->outgoing_args - offsets->saved_regs;
@@ -20432,7 +20868,7 @@
      between the push and the stack frame allocation.  */
   if (for_prologue
       && ((flag_pic && arm_pic_register != INVALID_REGNUM)
-	  || (!frame_pointer_needed && CALLER_INTERWORKING_SLOT_SIZE > 0)))
+	  || (!arm_apcs_frame_needed () && CALLER_INTERWORKING_SLOT_SIZE > 0)))
     return 0;
 
   reg_base = 0;
@@ -20642,6 +21078,7 @@
 #if ARM_FT_UNKNOWN != 0
   machine->func_type = ARM_FT_UNKNOWN;
 #endif
+  machine->leaf = -1;
   return machine;
 }
 
@@ -20653,6 +21090,21 @@
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
 
@@ -20758,7 +21210,7 @@
   if (flag_pic && arm_pic_register != INVALID_REGNUM)
     arm_load_pic_register (live_regs_mask);
 
-  if (!frame_pointer_needed && CALLER_INTERWORKING_SLOT_SIZE > 0)
+  if (!arm_apcs_frame_needed () && CALLER_INTERWORKING_SLOT_SIZE > 0)
     emit_move_insn (gen_rtx_REG (Pmode, ARM_HARD_FRAME_POINTER_REGNUM),
 		    stack_pointer_rtx);
 
@@ -20815,7 +21267,7 @@
 	}
     }
 
-  if (frame_pointer_needed)
+  if (arm_apcs_frame_needed ())
     thumb_set_frame_pointer (offsets);
 
   /* If we are profiling, make sure no instructions are scheduled before
@@ -20848,7 +21300,7 @@
   offsets = arm_get_frame_offsets ();
   amount = offsets->outgoing_args - offsets->saved_regs;
 
-  if (frame_pointer_needed)
+  if (arm_apcs_frame_needed ())
     {
       emit_insn (gen_movsi (stack_pointer_rtx, hard_frame_pointer_rtx));
       amount = offsets->locals_base - offsets->saved_regs;
@@ -22164,7 +22616,7 @@
     emit_move_insn (gen_rtx_REG (Pmode, LR_REGNUM), source);
   else
     {
-      if (frame_pointer_needed)
+      if (arm_apcs_frame_needed ())
 	addr = plus_constant(hard_frame_pointer_rtx, -4);
       else
 	{
@@ -22207,7 +22659,7 @@
     {
       limit = 1024;
       /* Find the saved regs.  */
-      if (frame_pointer_needed)
+      if (arm_apcs_frame_needed ())
 	{
 	  delta = offsets->soft_frame - offsets->saved_args;
 	  reg = THUMB_HARD_FRAME_POINTER_REGNUM;
@@ -22336,7 +22788,92 @@
   return mode == SImode ? 255 : 0;
 }
 
+void
+arm_expand_save_stack_block (rtx save_area, rtx stack_pointer ATTRIBUTE_UNUSED)
+{
+  emit_library_call_value (gen_rtx_SYMBOL_REF (Pmode, "__gcc_alloca_save"),
+                           save_area, LCT_NORMAL, GET_MODE (save_area), 0);
+}
 
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
+
+
 /* Map internal gcc register numbers to DWARF2 register numbers.  */
 
 unsigned int
@@ -22350,6 +22887,9 @@
   if (IS_FPA_REGNUM (regno))
     return (TARGET_AAPCS_BASED ? 96 : 16) + regno - FIRST_FPA_REGNUM;
 
+  if (IS_CIRRUS_REGNUM (regno))
+    return 28 + regno - FIRST_CIRRUS_FP_REGNUM;
+
   if (IS_VFP_REGNUM (regno))
     {
       /* See comment in arm_dwarf_register_span.  */
@@ -22365,7 +22905,13 @@
   if (IS_IWMMXT_REGNUM (regno))
     return 112 + regno - FIRST_IWMMXT_REGNUM;
 
-  gcc_unreachable ();
+  /* FIXME: GCCSDK: It is very odd we have to make an exception on pseudo registers
+     24, 25, 26 and anything >= FIRST_HI_VFP_REGNUM as otherwise we reach
+     gcc_unreachable() when being called from expand_builtin_init_dwarf_reg_sizes()
+     which is enumerating all registers from 0 until FIRST_PSEUDO_REGISTER.  Why
+     don't we have this problem in other ARM targets ? */
+  /* gcc_unreachable (); */
+  return 0;
 }
 
 /* Dwarf models VFPv3 registers as 32 64-bit registers.
@@ -22542,6 +23088,31 @@
       break;
 
     case REG:
+#ifdef TARGET_RISCOSELF
+      if (REGNO (e0) == IP_REGNUM
+	      && GET_CODE (e1) == PLUS
+	      && GET_CODE (XEXP (e1, 0)) == REG
+	      && (REGNO (XEXP (e1, 0)) == IP_REGNUM
+		  || REGNO (XEXP (e1, 0)) == SP_REGNUM)
+	      && GET_CODE (XEXP (e1, 1)) == CONST_INT)
+	{
+	  /* Ignore any prologue related instructions that we inserted for
+	     stack extension.  */
+	  break;
+	}
+      else if (REGNO (e0) == 9
+	       && GET_CODE (e1) == REG
+	       && REGNO (e1) == SP_REGNUM)
+	{
+	  /* A "mov r9, sp" instruction is emitted when not optimising or
+	     when alloca is called with no stack checking (which may be possible
+	     in module code). This causes another ".movsp" unwind directive to be
+	     generated besides the one at the top of the function. The assembler
+	     generates an error because of it, so we suppress it here. */
+	  break;
+	}
+      else
+#endif
       if (REGNO (e0) == SP_REGNUM)
 	{
 	  /* A stack increment.  */
@@ -22644,6 +23215,13 @@
       arm_unwind_emit_sequence (asm_out_file, pat);
       break;
 
+#ifdef TARGET_RISCOSELF
+    case PARALLEL:
+      /* The rt_stkovf instruction pattern is interpreted as PARALLEL.
+         As it doesn't actually affect the stack pointer, do nothing.  */
+      break;
+#endif
+
     default:
       abort();
     }
@@ -23089,6 +23667,10 @@
 bool
 arm_frame_pointer_required (void)
 {
+  /* The APCS-32 ABI never requires a frame pointer. */
+  if (arm_abi == ARM_ABI_APCS32)
+    return false;
+
   return (cfun->has_nonlocal_label
           || SUBTARGET_FRAME_POINTER_REQUIRED
           || (TARGET_ARM && TARGET_APCS_FRAME && ! leaf_function_p ()));
@@ -23627,10 +24209,10 @@
       fixed_regs[PIC_OFFSET_TABLE_REGNUM] = 1;
       call_used_regs[PIC_OFFSET_TABLE_REGNUM] = 1;
     }
-  else if (TARGET_APCS_STACK)
+  if (OPTION_APCS_STACK)
     {
-      fixed_regs[10]     = 1;
-      call_used_regs[10] = 1;
+      fixed_regs[SL_REGNUM]     = 1;
+      call_used_regs[SL_REGNUM] = 1;
     }
   /* -mcaller-super-interworking reserves r11 for calls to
      _interwork_r11_call_via_rN().  Making the register global
