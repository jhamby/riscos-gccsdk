;; Implementation for module code
(define_insn "module_load_addr_based"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
        (unspec:SI [(match_operand:SI 1 "" "mX")] UNSPEC_PIC_SYM))]
  "TARGET_ARM && TARGET_MODULE"
  "ldr%?\\t%0, [pc, #%1-.-8]"
  [(set_attr "type" "load")
   (set (attr "pool_range")     (const_int 4096))
   (set (attr "neg_pool_range") (const_int 4084))]
)

;; Implementations for dyamic allocation off the stack.

;; These patterns say how to perform an equivalent to dynamic allocation
;; off the stack. We use library routines to return malloced memory
;; then alter the frame pointer so that on function exit, all chunks
;; will be freed. This system also works for different blocks within a
;; function.

;; We do not need to worry about the outermost block in a function since
;; function exit will tidy up all unalloced chunks.

(define_expand "save_stack_function"
  [(use (match_operand:SI 0 "memory_operand" ""))
   (use (match_operand:SI 1 "s_register_operand" ""))]
  "TARGET_ARM && TARGET_APCS_STACK"
{
  /* Calls to __arm_save_stack_block increase the internal counter of a
     nesting level number.  It is this level number that we choose to store
     in the 'save area' that GCC is giving us in operands[0].  */
  emit_library_call_value (gen_rtx_SYMBOL_REF (Pmode,
                                               \"__arm_save_stack_function\"),
                           operands[0], LCT_NORMAL, GET_MODE (operands[0]), 0);
  DONE;
})


(define_expand "restore_stack_function"
  [(set (match_dup 2) (mem:SI (match_operand:SI 0 "s_register_operand" "")))
   (set (match_dup 0) (match_operand:SI 1 "s_register_operand" ""))
   (set (mem:SI (match_dup 0)) (match_dup 2))]
  "TARGET_ARM && TARGET_APCS_STACK"
{
  /* Here we pass the value in the 'save area'.  This is the level number
     that we can use to work out which dynamic allocations to free.  */
  emit_library_call (gen_rtx_SYMBOL_REF (Pmode,
                                         \"__arm_restore_stack_function\"),
                     LCT_NORMAL, VOIDmode, 1,
                     operands[1], Pmode);
  DONE;
})

;; On entrance to a block, with __builtin_alloca, create a unique key
;; suitable for the free code.

(define_expand "save_stack_block"
  [(use (match_operand:SI 0 "memory_operand" ""))
   (use (match_operand:SI 1 "s_register_operand" ""))]
  "TARGET_ARM && TARGET_APCS_STACK"
{
  /* Calls to __arm_save_stack_block increase the internal counter of a
     nesting level number.  It is this level number that we choose to store
     in the 'save area' that GCC is giving us in operands[0].  */
  emit_library_call_value (gen_rtx_SYMBOL_REF (Pmode,
                                               \"__arm_save_stack_block\"),
                           operands[0], LCT_NORMAL, GET_MODE (operands[0]), 0);
  DONE;
})

(define_expand "save_stack_nonlocal"
  [(use (match_operand:SI 0 "memory_operand" ""))
   (use (match_operand:SI 1 "s_register_operand" ""))]
  "TARGET_ARM && TARGET_APCS_STACK"
{
  rtx fp_rtx = gen_rtx_REG (Pmode, FP_REGNUM);
  rtx sp_rtx = gen_rtx_REG (Pmode, SP_REGNUM);
  emit_move_insn (operands[0], fp_rtx);
  emit_move_insn (gen_rtx_MEM (Pmode,
			       plus_constant (XEXP (operands[0], 0), 4)),
			       sp_rtx);
  DONE;
})

;; Use the unique key produced earlier to free chunks created under
;; this key.

(define_expand "restore_stack_block"
  [(set (match_dup 2) (mem:SI (match_operand:SI 0 "s_register_operand" "")))
   (set (match_dup 0) (match_operand:SI 1 "s_register_operand" ""))
   (set (mem:SI (match_dup 0)) (match_dup 2))]
  "TARGET_ARM && TARGET_APCS_STACK"
{
  /* Here we pass the value in the 'save area'.  This is the level number
     that we can use to work out which dynamic allocations to free.  */
  emit_library_call (gen_rtx_SYMBOL_REF (Pmode,
                                         \"__arm_restore_stack_block\"),
                     LCT_NORMAL, VOIDmode, 1,
                     operands[1], Pmode);
  DONE;
})

(define_expand "restore_stack_nonlocal"
  [(use (match_operand:SI 0 "s_register_operand" ""))
   (use (match_operand:SI 1 "memory_operand" ""))]
  "TARGET_ARM && TARGET_APCS_STACK"
{
  rtx fp_rtx = gen_rtx_REG (Pmode, FP_REGNUM);
  rtx sp_rtx = gen_rtx_REG (Pmode, SP_REGNUM);
  emit_move_insn (fp_rtx, operands[1]);
  emit_move_insn (sp_rtx,
		  gen_rtx_MEM (Pmode,
			       plus_constant (XEXP (operands[1], 0), 4)));
  DONE;
})

;; Call the function that will reserve the necessary amount of memory
(define_expand "nonlocal_goto"
 [(use (match_operand 0 "general_operand" ""))
  (use (match_operand 1 "general_operand" ""))
  (use (match_operand 2 "general_operand" ""))
  (use (match_operand 3 "general_operand" ""))]
  "TARGET_ARM && TARGET_APCS_STACK"
{
  rtx chain = operands[0];
  rtx handler = operands[1];
  rtx stack = operands[2];
  rtx label = operands[3];

  emit_library_call (gen_rtx_SYMBOL_REF (Pmode, \"__arm_nonlocal_goto\"),
                     LCT_NORETURN, VOIDmode, 4,
                     chain, Pmode,
                     handler, Pmode,
                     stack, Pmode,
                     label, Pmode);

  emit_barrier ();
  DONE;
})


(define_expand "allocate_stack"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
        (minus:SI (reg:SI 13) (match_operand:SI 1 "reg_or_int_operand" "")))
   (set (reg:SI 13)
        (minus:SI (reg:SI 13) (match_dup 1)))
   (clobber (reg:SI 0))
   (clobber (reg:SI 14))]
  "TARGET_APCS_STACK"
  "
{
  emit_library_call_value (gen_rtx_SYMBOL_REF (Pmode,
                                               \"___arm_alloca_alloc\"),
                           operands[0], LCT_NORMAL, GET_MODE (operands[0]),
                           1, operands[1], SImode);
  DONE;
}")

;(define_insn "call_no_clobber"
;  [(unspec:SI [(match_operand 0 "" "")] UNSPEC_CALL)
;   (clobber (reg:SI LR_REGNUM))]
;  ""
;  "bl%?\\t%a0"
;[(set_attr "length" "4")])

(define_insn "rt_stkovf"
  [(unspec:SI [(match_operand:SI 0 "s_register_operand" "r")
               (match_operand:SI 1 "s_register_operand" "r")
               (match_operand 2 "" "")] UNSPEC_STK)
   (clobber (reg:SI SL_REGNUM))
   (clobber (reg:SI IP_REGNUM))
   (clobber (reg:SI LR_REGNUM))
   (clobber (reg:CC CC_REGNUM))]
  ""
  "cmp\\t%0, %1\;bllt\\t%a2"
[(set_attr "conds" "clob")
 (set_attr "length" "8")])

; Software Interrupt
;(define_insn "swi"
;  [(unspec:SI [(match_operand 0 "" "")] UNSPEC_SWI)
;   (use (reg:SI 0))]
;  ""
;  "swi%?\\t%a0"
;[(set_attr "length" "4")])
