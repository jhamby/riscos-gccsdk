;; Implementations for dyamic allocation off the stack.
;; Copyright (C) 2005-2010 Free Software Foundation, Inc.
;; Contributed by Nick Burrett (nick@sqrt.co.uk>)
;; and Lee Noar (leenoar@sky.com).

;; This file is part of GCC.

;; GCC is free software; you can redistribute it and/or modify it
;; under the terms of the GNU General Public License as published
;; by the Free Software Foundation; either version 3, or (at your
;; option) any later version.

;; GCC is distributed in the hope that it will be useful, but WITHOUT
;; ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
;; or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
;; License for more details.

;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING3.  If not see
;; <http://www.gnu.org/licenses/>.

;; These patterns say how to perform an equivalent to dynamic allocation
;; off the stack. We use library routines to return malloced memory
;; then alter the frame pointer so that on function exit, all chunks
;; will be freed. This system also works for different blocks within a
;; function.

;; We do not need to worry about the outermost block in a function since
;; function exit will tidy up all unalloced chunks.

(define_insn "call_no_clobber"
  [(unspec:SI [(match_operand 0 "" "")] UNSPEC_CALL)
   (clobber (reg:SI LR_REGNUM))
   (clobber (reg:CC CC_REGNUM))]
  ""
  "bl%?\\t%a0"
[(set_attr "conds" "clob")
 (set_attr "length" "4")])


;; On entrance to a block, with __builtin_alloca, create a unique key
;; suitable for the free code.
(define_expand "save_stack_block"
  [(use (match_operand:SI 0 "s_register_operand" "=r"))
   (match_operand:SI 1 "s_register_operand" "")
   (clobber (reg:SI 14))]
  "TARGET_ARM && TARGET_APCS_STACK"
{
  arm_expand_save_stack_block (operands[0], operands[1]);
  DONE;
})

;; Use the unique key produced earlier to free chunks created under
;; this key.
(define_expand "restore_stack_block"
  [(match_operand:SI 0 "s_register_operand" "")
   (use (match_operand:SI 1 "s_register_operand" "=r"))
   (clobber (reg:SI 14))]
  "TARGET_ARM && TARGET_APCS_STACK"
{
  arm_expand_restore_stack_block (operands[0], operands[1]);
  DONE;
})

(define_expand "save_stack_nonlocal"
  [(use (match_operand:SI 0 "memory_operand" ""))
   (use (match_operand:SI 1 "s_register_operand" ""))]
  "TARGET_ARM && TARGET_APCS_STACK"
{
  arm_expand_save_stack_nonlocal (operands);
  DONE;
})

(define_expand "restore_stack_nonlocal"
  [(use (match_operand:SI 0 "s_register_operand" ""))
   (use (match_operand:SI 1 "memory_operand" ""))]
  "TARGET_ARM && TARGET_APCS_STACK"
{
  arm_expand_restore_stack_nonlocal (operands);
  DONE;
})

(define_expand "allocate_stack"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
        (minus:SI (reg:SI 13) (match_operand:SI 1 "reg_or_int_operand" "")))
   (set (reg:SI 13)
        (minus:SI (reg:SI 13) (match_dup 1)))
   (clobber (reg:SI 0))
   (clobber (reg:SI 14))]
  "TARGET_ARM && TARGET_APCS_STACK"
  "
{
  arm_expand_allocate_stack (operands[0], operands[1]);
  DONE;
}")

(define_expand "nonlocal_goto"
  [(match_operand:SI 0 "general_operand" "")
   (match_operand:SI 1 "general_operand" "")
   (match_operand:SI 2 "general_operand" "")
   (match_operand:SI 3 "" "")]
  "TARGET_ARM && TARGET_APCS_STACK"
{
  arm_expand_nonlocal_goto (operands);
  DONE;
})

(define_insn "rt_stkovf"
  [(unspec:SI [(match_operand:SI 0 "s_register_operand" "")
               (match_operand:SI 1 "s_register_operand" "")
               (match_operand 2 "" "")] UNSPEC_STK)
   (clobber (reg:SI SL_REGNUM))
   (use (reg:SI 11))
   (clobber (reg:SI IP_REGNUM))
   (clobber (reg:SI LR_REGNUM))
   (clobber (reg:CC CC_REGNUM))]
  ""
  "cmp\\t%0, %1\;bllt\\t%a2"
[(set_attr "conds" "clob")
 (set_attr "length" "8")])

; 4th operand is ignored and included to make pattern different from rt_stkovf
(define_insn "rt_stkovf_v5_clobbered"
  [(unspec:SI [(match_operand:SI 0 "s_register_operand" "")
               (match_operand:SI 1 "s_register_operand" "")
               (match_operand 2 "" "")
	       (match_operand 3 "" "")] UNSPEC_STK)
   (clobber (reg:SI 8))
   (clobber (reg:SI SL_REGNUM))
   (use (reg:SI 11))
   (clobber (reg:SI IP_REGNUM))
   (clobber (reg:SI LR_REGNUM))
   (clobber (reg:CC CC_REGNUM))]
  ""
  "cmp\\t%0, %1\;bllt\\t%a2"
[(set_attr "conds" "clob")
 (set_attr "length" "8")])

(define_insn "rt_loadpic"
  [(unspec:SI [(match_operand:SI 0 "s_register_operand" "r")
               (match_operand:SI 1 "s_register_operand" "r")] UNSPEC_PIC_BASE)]
  ""
  "ldmia\\t%0, {%0, %1}\\n\\tldr\\t%1, [%1, #0]\\n\\tldr\\t%0, [%1, %0, lsl#4]"
[(set_attr "length" "12")])
