--- src/ariarm_base.d	2004-03-08 21:21:13.000000000 +0000
+++ src/ariarm.d	2006-08-13 13:21:21.000000000 +0100
@@ -55,9 +55,9 @@
 
 #define C(x) x
 #define EXPORT(x) EXPORT x
-# The leading underscore will be munged away by asmfilter.sed.
-#define GLABEL(x) _##x
-#define LABEL(x) _##x
+
+#define GLABEL(x) x
+#define LABEL(x) x
 
 
         AREA    |C$$code|,CODE,READONLY
@@ -187,7 +187,7 @@
         LDR     a3,[pc,#ptr_mulu32_high-.-8]
         STR     a2,[a3,#0]
 #endif
-        MOVS    pc,lr
+        MOV     pc,lr
 
 # extern uint16 divu_3216_1616_ (uint32 x, uint16 y);
 #       entry
@@ -278,7 +278,7 @@
         LDR     a4,[pc,#ptr_divu_32_rest-.-8]
         STR     a2,[a4,#0]              # divu_32_rest = remainder
 #endif
-        LDMFD   sp!, {v1,v2,v3,v4,v5,v6,pc}^
+        LDMFD   sp!, {v1,v2,v3,v4,v5,v6,pc} 
 
 LABEL(divu_6432_3232_l1)
         MOV     v3, #0                  # s = 0
@@ -346,7 +346,7 @@
         LDR     a3,[pc,#ptr_divu_32_rest-.-8]
         STR     a2,[a3,#0]              # divu_32_rest = remainder
 #endif
-        LDMFD   sp!, {v1,v2,v3,v4,v5,v6,pc}^
+        LDMFD   sp!, {v1,v2,v3,v4,v5,v6,pc} 
 
 # extern uintD* copy_loop_up (uintD* sourceptr, uintD* destptr, uintC count);
 #       entry
@@ -370,7 +370,7 @@
 LABEL(copy_loop_up_l1)
         BICS    a4,a3,#3                # set counter to multiple of 4
         MOVEQ   a1,a2                   # return addr of last word stored
-        MOVEQS  pc,lr                   # if zero then we're done
+        MOVEQ   pc,lr                   # if zero then we're done
         STMFD   sp!,{v1,lr}             # save work regs
 LABEL(copy_loop_up_l2)
         LDMIA   a1!,{a3,v1,ip,lr}       # copy 4 words in one go
@@ -380,7 +380,7 @@
         STMGEIA a2!,{a3,v1,ip,lr}       # 4 more words
         BGT     copy_loop_up_l2         # and loop
         MOV     a1,a2                   # return addr of last word stored
-        LDMFD   sp!,{v1,pc}^            # restore work regs and return
+        LDMFD   sp!,{v1,pc}             # restore work regs and return
 
 # extern uintD* copy_loop_down (uintD* sourceptr, uintD* destptr, uintC count);
 #       entry
@@ -404,7 +404,7 @@
 LABEL(copy_loop_down_l1)
         BICS    a4,a3,#3                # set counter to multiple of 4
         MOVEQ   a1,a2                   # return addr of last word stored
-        MOVEQS  pc,lr                   # if zero then we're done
+        MOVEQ   pc,lr                   # if zero then we're done
         STMFD   sp!,{v1,lr}             # save work regs
 LABEL(copy_loop_down_l2)
         LDMDB   a1!,{a3,v1,ip,lr}       # copy 4 words in one go
@@ -414,7 +414,7 @@
         STMGEDB a2!,{a3,v1,ip,lr}       # 4 more words
         BGT     copy_loop_down_l2       # and loop
         MOV     a1,a2                   # return addr of last word stored
-        LDMFD   sp!,{v1,pc}^            # restore work regs and return
+        LDMFD   sp!,{v1,pc}             # restore work regs and return
 
 # extern uintD* clear_loop_up (uintD* destptr, uintC count);
 #       entry
@@ -446,7 +446,7 @@
         STRGT   a3,[a1],#4
 LABEL(fill_loop_up_l1)
         BICS    a4,a2,#3                # set counter to multiple of 4
-        MOVEQS  pc,lr                   # if zero then we're done
+        MOVEQ   pc,lr                   # if zero then we're done
         STMFD   sp!,{v1,lr}             # save work regs
         MOV     v1,a3                   # copy filler to three other
         MOV     ip,a3                   # registers
@@ -456,7 +456,7 @@
         SUBS    a4,a4,#8                # decrement counter by 8
         STMGEIA a1!,{a3,v1,ip,lr}       # if count still positive then store 4
         BGT     fill_loop_up_l2         # more and loop
-        LDMFD   sp!,{v1,pc}^            # restore work regs and return
+        LDMFD   sp!,{v1,pc}             # restore work regs and return
 
 
 # extern uintD* clear_loop_down (uintD* destptr, uintC count);
@@ -489,7 +489,7 @@
         STRGT   a3,[a1,#-4]!
 LABEL(fill_loop_down_l1)
         BICS    a4,a2,#3                # set counter to multiple of 4
-        MOVEQS  pc,lr                   # if zero then we're done
+        MOVEQ   pc,lr                   # if zero then we're done
         STMFD   sp!,{v1,lr}             # save work regs
         MOV     v1,a3                   # copy filler to three other
         MOV     ip,a3                   # registers
@@ -499,7 +499,7 @@
         SUBS    a4,a4,#8                # decrement counter by 8
         STMGEDB a1!,{a3,v1,ip,lr}       # if count still positive then store 4
         BGT     fill_loop_down_l2       # more and loop
-        LDMFD   sp!,{v1,pc}^            # restore work regs and return
+        LDMFD   sp!,{v1,pc}             # restore work regs and return
 
 # extern void or_loop_up (uintD* xptr, uintD* yptr, uintC count);
 #       entry
@@ -529,7 +529,7 @@
         STRGT   ip,[a1],#4
 LABEL(or_loop_up_l1)
         BICS    a4,a3,#3                # set counter to multiple of 4
-        MOVEQS  pc,lr                   # if zero then we're done
+        MOVEQ   pc,lr                   # if zero then we're done
         STMFD   sp!,{v1-v5,lr}          # save work regs
 LABEL(or_loop_up_l2)
         LDMIA   a2!,{a3,v1,v2,ip}       # load 4 words in one go
@@ -541,7 +541,7 @@
         STMIA   a1!,{v3,v4,v5,lr}       # store 4 results
         SUBS    a4,a4,#4                # decrement counter by 4
         BGT     or_loop_up_l2           # if count still positive then loop
-        LDMFD   sp!,{v1-v5,pc}^         # restore work regs and return
+        LDMFD   sp!,{v1-v5,pc}          # restore work regs and return
 
 # extern void xor_loop_up (uintD* xptr, uintD* yptr, uintC count);
 #       entry
@@ -571,7 +571,7 @@
         STRGT   ip,[a1],#4
 LABEL(xor_loop_up_l1)
         BICS    a4,a3,#3                # set counter to multiple of 4
-        MOVEQS  pc,lr                   # if zero then we're done
+        MOVEQ   pc,lr                   # if zero then we're done
         STMFD   sp!,{v1-v5,lr}          # save work regs
 LABEL(xor_loop_up_l2)
         LDMIA   a2!,{a3,v1,v2,ip}       # load 4 words in one go
@@ -583,7 +583,7 @@
         STMIA   a1!,{v3,v4,v5,lr}       # store 4 results
         SUBS    a4,a4,#4                # decrement counter by 4
         BGT     xor_loop_up_l2          # if count still positive then loop
-        LDMFD   sp!,{v1-v5,pc}^         # restore work regs and return
+        LDMFD   sp!,{v1-v5,pc}          # restore work regs and return
 
 # extern void and_loop_up (uintD* xptr, uintD* yptr, uintC count);
 #       entry
@@ -613,7 +613,7 @@
         STRGT   ip,[a1],#4
 LABEL(and_loop_up_l1)
         BICS    a4,a3,#3                # set counter to multiple of 4
-        MOVEQS  pc,lr                   # if zero then we're done
+        MOVEQ   pc,lr                   # if zero then we're done
         STMFD   sp!,{v1-v5,lr}          # save work regs
 LABEL(and_loop_up_l2)
         LDMIA   a2!,{a3,v1,v2,ip}       # load 4 words in one go
@@ -625,7 +625,7 @@
         STMIA   a1!,{v3,v4,v5,lr}       # store 4 results
         SUBS    a4,a4,#4                # decrement counter by 4
         BGT     and_loop_up_l2          # if count still positive then loop
-        LDMFD   sp!,{v1-v5,pc}^         # restore work regs and return
+        LDMFD   sp!,{v1-v5,pc}          # restore work regs and return
 
 # extern void eqv_loop_up (uintD* xptr, uintD* yptr, uintC count);
 #       entry
@@ -659,7 +659,7 @@
         STRGT   ip,[a1],#4
 LABEL(eqv_loop_up_l1)
         BICS    a4,a3,#3                # set counter to multiple of 4
-        MOVEQS  pc,lr                   # if zero then we're done
+        MOVEQ   pc,lr                   # if zero then we're done
         STMFD   sp!,{v1-v5,lr}          # save work regs
 LABEL(eqv_loop_up_l2)
         LDMIA   a2!,{a3,v1,v2,ip}       # load 4 words in one go
@@ -675,7 +675,7 @@
         STMIA   a1!,{v3,v4,v5,lr}       # store 4 results
         SUBS    a4,a4,#4                # decrement counter by 4
         BGT     eqv_loop_up_l2          # if count still positive then loop
-        LDMFD   sp!,{v1-v5,pc}^         # restore work regs and return
+        LDMFD   sp!,{v1-v5,pc}          # restore work regs and return
 
 # extern void nand_loop_up (uintD* xptr, uintD* yptr, uintC count);
 #       entry
@@ -709,7 +709,7 @@
         STRGT   ip,[a1],#4
 LABEL(nand_loop_up_l1)
         BICS    a4,a3,#3                # set counter to multiple of 4
-        MOVEQS  pc,lr                   # if zero then we're done
+        MOVEQ   pc,lr                   # if zero then we're done
         STMFD   sp!,{v1-v5,lr}          # save work regs
 LABEL(nand_loop_up_l2)
         LDMIA   a2!,{a3,v1,v2,ip}       # load 4 words in one go
@@ -725,7 +725,7 @@
         STMIA   a1!,{v3,v4,v5,lr}       # store 4 results
         SUBS    a4,a4,#4                # decrement counter by 4
         BGT     nand_loop_up_l2         # if count still positive then loop
-        LDMFD   sp!,{v1-v5,pc}^         # restore work regs and return
+        LDMFD   sp!,{v1-v5,pc}          # restore work regs and return
 
 # extern void nor_loop_up (uintD* xptr, uintD* yptr, uintC count);
 #       entry
@@ -759,7 +759,7 @@
         STRGT   ip,[a1],#4
 LABEL(nor_loop_up_l1)
         BICS    a4,a3,#3                # set counter to multiple of 4
-        MOVEQS  pc,lr                   # if zero then we're done
+        MOVEQ   pc,lr                   # if zero then we're done
         STMFD   sp!,{v1-v5,lr}          # save work regs
 LABEL(nor_loop_up_l2)
         LDMIA   a2!,{a3,v1,v2,ip}       # load 4 words in one go
@@ -775,7 +775,7 @@
         STMIA   a1!,{v3,v4,v5,lr}       # store 4 results
         SUBS    a4,a4,#4                # decrement counter by 4
         BGT     nor_loop_up_l2          # if count still positive then loop
-        LDMFD   sp!,{v1-v5,pc}^         # restore work regs and return
+        LDMFD   sp!,{v1-v5,pc}          # restore work regs and return
 
 # extern void andc2_loop_up (uintD* xptr, uintD* yptr, uintC count);
 #       entry
@@ -805,7 +805,7 @@
         STRGT   ip,[a1],#4
 LABEL(andc2_loop_up_l1)
         BICS    a4,a3,#3                # set counter to multiple of 4
-        MOVEQS  pc,lr                   # if zero then we're done
+        MOVEQ   pc,lr                   # if zero then we're done
         STMFD   sp!,{v1-v5,lr}          # save work regs
 LABEL(andc2_loop_up_l2)
         LDMIA   a2!,{a3,v1,v2,ip}       # load 4 words in one go
@@ -817,7 +817,7 @@
         STMIA   a1!,{v3,v4,v5,lr}       # store 4 results
         SUBS    a4,a4,#4                # decrement counter by 4
         BGT     andc2_loop_up_l2        # if count still positive then loop
-        LDMFD   sp!,{v1-v5,pc}^         # restore work regs and return
+        LDMFD   sp!,{v1-v5,pc}          # restore work regs and return
 
 # extern void orc2_loop_up (uintD* xptr, uintD* yptr, uintC count);
 #       entry
@@ -851,7 +851,7 @@
         STRGT   ip,[a1],#4
 LABEL(orc2_loop_up_l1)
         BICS    a4,a3,#3                # set counter to multiple of 4
-        MOVEQS  pc,lr                   # if zero then we're done
+        MOVEQ   pc,lr                   # if zero then we're done
         STMFD   sp!,{v1-v5,lr}          # save work regs
 LABEL(orc2_loop_up_l2)
         LDMIA   a2!,{a3,v1,v2,ip}       # load 4 words in one go
@@ -867,7 +867,7 @@
         STMIA   a1!,{v3,v4,v5,lr}       # store 4 results
         SUBS    a4,a4,#4                # decrement counter by 4
         BGT     orc2_loop_up_l2         # if count still positive then loop
-        LDMFD   sp!,{v1-v5,pc}^         # restore work regs and return
+        LDMFD   sp!,{v1-v5,pc}          # restore work regs and return
 
 # extern void not_loop_up (uintD* xptr, uintC count);
 #       entry
@@ -893,7 +893,7 @@
         STRGT   a3,[a1],#4
 LABEL(not_loop_up_l1)
         BICS    a4,a2,#3                # set counter to multiple of 4
-        MOVEQS  pc,lr                   # if zero then we're done
+        MOVEQ   pc,lr                   # if zero then we're done
         STMFD   sp!,{lr}                # save work regs
 LABEL(not_loop_up_l2)
         LDMIA   a1,{a2,a3,ip,lr}        # load 4 words in one go,NO writeback
@@ -904,7 +904,7 @@
         STMIA   a1!,{a2,a3,ip,lr}       # store 4 results
         SUBS    a4,a4,#4                # decrement counter by 4
         BGT     not_loop_up_l2          # if count still positive then loop
-        LDMFD   sp!,{pc}^               # restore work regs and return
+        LDMFD   sp!,{pc}                # restore work regs and return
 
 # extern void and_test_loop_up (uintD* xptr, uintD* yptr, uintC count);
 #       entry
@@ -923,13 +923,13 @@
         LDR     ip,[a1],#4              # to align the total to a multiple
         TST     ip,a4                   # of 4 words
         MOVNE   a1,#1                   # return true if AND_TEST ok
-        MOVNES  pc,lr
+        MOVNE   pc,lr
         BCC     and_test_loop_up_l1     # better to branch than skip instrs.
         LDRGE   a4,[a2],#4
         LDRGE   ip,[a1],#4
         TSTGE   ip,a4
         MOVNE   a1,#1
-        MOVNES  pc,lr
+        MOVNE   pc,lr
         ANDS    a4,a3,#3
         CMP     a4,#2
         BLE     and_test_loop_up_l1     # better to branch than skip instrs.
@@ -937,11 +937,11 @@
         LDRGT   ip,[a1],#4
         TSTGT   ip,a4
         MOVNE   a1,#1
-        MOVNES  pc,lr
+        MOVNE   pc,lr
 LABEL(and_test_loop_up_l1)
         BICS    a4,a3,#3                # set counter to multiple of 4
         MOVEQ   a1,#0                   # return false
-        MOVEQS  pc,lr                   # if zero then we're done
+        MOVEQ   pc,lr                   # if zero then we're done
         STMFD   sp!,{v1-v6,lr}          # save work regs
         MOV     v6,a1                   # move xptr to v6
         MOV     a1,#1                   # set result to true
@@ -952,11 +952,11 @@
         TSTEQ   v4,v1
         TSTEQ   v5,v2
         TSTEQ   lr,ip
-        LDMNEFD sp!,{v1-v6,pc}^
+        LDMNEFD sp!,{v1-v6,pc} 
         SUBS    a4,a4,#4                # decrement counter by 4
         BGT     and_test_loop_up_l2     # if count still positive then loop
         MOV     a1,#0
-        LDMFD   sp!,{v1-v6,pc}^         # restore work regs and return
+        LDMFD   sp!,{v1-v6,pc}          # restore work regs and return
 
 # extern void test_loop_up (uintD* xptr, uintC count);
 #       entry
@@ -973,21 +973,21 @@
         BEQ     test_loop_up_l1         # yup, so branch
         LDR     a4,[ip],#4              # TEST the first 1-3 words
         TEQ     a4,#0                   # align the total to a multiple of 4
-        MOVNES  pc,lr                   # return true if AND_TEST ok
+        MOVNE   pc,lr                   # return true if AND_TEST ok
         CMP     a3,#2
         BLT     test_loop_up_l1         # need to branch 'cos PSR set
         LDRGE   a4,[ip],#4              # when checking against zero
         TEQGE   a4,#0
-        MOVNES  pc,lr
+        MOVNE   pc,lr
         CMP     a3,#2
         BLE     test_loop_up_l1         # need to branch 'cos PSR set
         LDRGT   a4,[ip],#4              # when checking against zero
         TEQGT   a4,#0
-        MOVNES  pc,lr
+        MOVNE   pc,lr
 LABEL(test_loop_up_l1)
         BICS    a4,a2,#3                # set counter to multiple of 4
         MOVEQ   a1,#0                   # return false
-        MOVEQS  pc,lr                   # if zero then we're done
+        MOVEQ   pc,lr                   # if zero then we're done
         STMFD   sp!,{v1,lr}             # save work regs
 LABEL(test_loop_up_l2)
         LDMIA   ip!,{a2,a3,v1,lr}       # load 4 words in one go
@@ -995,11 +995,11 @@
         TEQEQ   a3,#0
         TEQEQ   v1,#0
         TEQEQ   lr,#0
-        LDMNEFD sp!,{v1,pc}^
+        LDMNEFD sp!,{v1,pc} 
         SUBS    a4,a4,#4                # decrement counter by 4
         BGT     test_loop_up_l2         # if count still positive then loop
         MOV     a1,#0
-        LDMFD   sp!,{v1,pc}^            # restore work regs and return
+        LDMFD   sp!,{v1,pc}             # restore work regs and return
 
 # extern void compare_loop_up (uintD* xptr, uintD* yptr, uintC count);
 #       entry
@@ -1021,7 +1021,7 @@
         CMP     ip,a4                   # of 4 words
         MVNLO   a1,#0                   # x < y -> -1
         MOVHI   a1,#1                   # x > y -> +1
-        MOVNES  pc,lr                   # and return result if not equal
+        MOVNE   pc,lr                   # and return result if not equal
         ANDS    a4,a3,#3
         CMP     a4,#2
         BLT     compare_loop_up_l1      # need to branch 'cos PSR used
@@ -1030,7 +1030,7 @@
         CMP     ip,a4
         MVNLO   a1,#0
         MOVHI   a1,#1
-        MOVNES  pc,lr
+        MOVNE   pc,lr
         ANDS    a4,a3,#3
         CMP     a4,#2
         BLE     compare_loop_up_l1      # need to branch 'cos PSR used
@@ -1039,11 +1039,11 @@
         CMP     ip,a4
         MVNLO   a1,#0
         MOVHI   a1,#1
-        MOVNES  pc,lr
+        MOVNE   pc,lr
 LABEL(compare_loop_up_l1)
         BICS    a4,a3,#3                # set counter to multiple of 4
         MOVEQ   a1,#0                   # xptr[] == yptr[] -> 0
-        MOVEQS  pc,lr                   # if zero then we're done
+        MOVEQ   pc,lr                   # if zero then we're done
         STMFD   sp!,{v1-v6,lr}          # save work regs
         MOV     v6,a1                   # move xptr to v6
         MOV     a1,#1                   # set result to +1
@@ -1055,11 +1055,11 @@
         CMPEQ   v5,v2
         CMPEQ   lr,ip
         MVNLO   a1,#0                   # x < y -> -1 (a1 already holds +1)
-        LDMNEFD sp!,{v1-v6,pc}^
+        LDMNEFD sp!,{v1-v6,pc} 
         SUBS    a4,a4,#4                # decrement counter by 4
         BGT     compare_loop_up_l2      # if count still positive then loop
         MOV     a1,#0
-        LDMFD   sp!,{v1-v6,pc}^         # restore work regs and return
+        LDMFD   sp!,{v1-v6,pc}          # restore work regs and return
 
 # extern uintD addto_loop_down (uintD* sourceptr, uintD* destptr, uintC count);
 #       entry
@@ -1111,11 +1111,11 @@
         BICS    a4,a4,#3                # set counter to multiple of 4
         BNE     add_loop_down_l3        # branch if more adds to do
         ADCEQ   a1,a4,a4                # set result to Carry (a4 is 0)
-        LDMEQFD sp!,{v6,pc}^            # and return
+        LDMEQFD sp!,{v6,pc}             # and return
 LABEL(add_loop_down_l1)
         BICS    a4,a4,#3                # set counter to multiple of 4
         MOVEQ   a1,#0                   # no adds, so C = 0
-        MOVEQS  pc,lr                   # if zero then we're done
+        MOVEQ   pc,lr                   # if zero then we're done
         CMN     a4,#0                   # clear carry bit
         STMFD   sp!,{v6,lr}
 LABEL(add_loop_down_l3)
@@ -1132,7 +1132,7 @@
         TEQ     a4,#0                   # are we done ?
         BNE     add_loop_down_l2        # if count non-zero then loop
         ADC     a1,a4,a4                # set result to Carry (a4 is 0)
-        LDMFD   sp!,{v1-v6,pc}^         # restore work regs and return
+        LDMFD   sp!,{v1-v6,pc}          # restore work regs and return
 
 # extern uintD inc_loop_down (uintD* ptr, uintC count);
 #       entry
@@ -1150,11 +1150,11 @@
         ADDS    a4,a4,#1                # align the total to a multiple of 2
         STR     a4,[a1]
         MOVNE   a1,#0                   # set result to 0
-        MOVNES  pc,lr                   # return 0 if non-zero result
+        MOVNE   pc,lr                   # return 0 if non-zero result
 LABEL(inc_loop_down_l1)
         BICS    a4,a2,#1                # set counter to multiple of 2
         MOVEQ   a1,#1                   # return 1
-        MOVEQS  pc,lr                   # if zero then we're done
+        MOVEQ   pc,lr                   # if zero then we're done
         MOV     ip,a1                   # move ptr to ip
         MOV     a1,#0                   # set result to 0
         ANDS    a3,a4,#3
@@ -1163,10 +1163,10 @@
         ADDS    a3,a3,#1                # INC the two words
         ADDEQS  a2,a2,#1                # stopping when first word non-zero
         STMDB   ip!,{a2,a3}             # store 2 results
-        MOVNES  pc,lr                   # return 0 if any result non-zero
+        MOVNE   pc,lr                   # return 0 if any result non-zero
         SUBS    a4,a4,#2                # decrement counter by 2
         MOVEQ   a1,#1                   # if finished loop then
-        MOVEQS  pc,lr                   # return 1
+        MOVEQ   pc,lr                   # return 1
 LABEL(inc_loop_down_l3)                 # now a multiple of 4 words
         STMFD   sp!,{v1,lr}             # save work regs
 LABEL(inc_loop_down_l2)
@@ -1176,11 +1176,11 @@
         ADDEQS  a3,a3,#1
         ADDEQS  a2,a2,#1
         STMDB   ip!,{a2,a3,v1,lr}       # store 4 results
-        LDMNEFD sp!,{v1,pc}^            # return 0 if any result non-zero
+        LDMNEFD sp!,{v1,pc}             # return 0 if any result non-zero
         SUBS    a4,a4,#4                # decrement counter by 4
         BGT     inc_loop_down_l2        # if count still positive then loop
         MOV     a1,#1
-        LDMFD   sp!,{v1,pc}^            # restore work regs and return 1
+        LDMFD   sp!,{v1,pc}             # restore work regs and return 1
 
 # extern uintD sub_loop_down (uintD* sourceptr1, uintD* sourceptr2, uintD* destptr, uintC count);
 #       entry
@@ -1206,7 +1206,7 @@
 LABEL(sub_loop_down_l4)                 # drop through for better instr. timings
         BICS    a4,a4,#3                # set counter to multiple of 4
         SBCEQ   a1,a4,a4                # set result to Carry (a4 is 0)
-        LDMEQFD sp!,{v6,pc}^            # and return
+        LDMEQFD sp!,{v6,pc}             # and return
         STMFD   sp!,{v1-v5}             # save work regs
         B       sub_loop_down_l2        # branch if more subtracts to do
 LABEL(sub_loop_down_l0)
@@ -1224,7 +1224,7 @@
 LABEL(sub_loop_down_l1)
         BICS    a4,a4,#3                # set counter to multiple of 4
         MOVEQ   a1,#0                   # no subtracts, so C = 0
-        MOVEQS  pc,lr                   # if zero then we're done
+        MOVEQ   pc,lr                   # if zero then we're done
         CMP     a4,#0                   # set carry bit, since a4 > 0
         STMFD   sp!,{v1-v6,lr}          # save work regs
 LABEL(sub_loop_down_l2)
@@ -1239,7 +1239,7 @@
         TEQ     a4,#0                   # are we done ?
         BNE     sub_loop_down_l2        # if count non-zero then loop
         SBC     a1,a4,a4                # set result to Carry (a4 is 0)
-        LDMFD   sp!,{v1-v6,pc}^         # restore work regs and return
+        LDMFD   sp!,{v1-v6,pc}          # restore work regs and return
 
 # extern uintD subx_loop_down (uintD* sourceptr1, uintD* sourceptr2, uintD* destptr, uintC count, uintD carry);
 #       entry
@@ -1269,7 +1269,7 @@
 LABEL(subx_loop_down_l4)                # drop through for better instr. timings
         BICS    a4,a4,#3                # set counter to multiple of 4
         SBCEQ   a1,a4,a4                # set result to Carry (a4 is 0)
-        LDMEQFD sp!,{v6,pc}^            # and return
+        LDMEQFD sp!,{v6,pc}             # and return
         STMFD   sp!,{v1-v5}             # save work regs
         B       subx_loop_down_l2       # branch if more subtracts to do
 LABEL(subx_loop_down_l0)
@@ -1287,7 +1287,7 @@
 LABEL(subx_loop_down_l1)
         BICS    a4,a4,#3                # set counter to multiple of 4
         SBCEQ   a1,a4,a4                # set result to Carry (a4 is 0)
-        MOVEQS  pc,lr                   # if zero then we're done
+        MOVEQ   pc,lr                   # if zero then we're done
         STMFD   sp!,{v1-v6,lr}          # save work regs
 LABEL(subx_loop_down_l2)
         LDMDB   a2!,{v1,v2,v3,ip}       # load 4 words in one go
@@ -1301,7 +1301,7 @@
         TEQ     a4,#0                   # are we done ?
         BNE     subx_loop_down_l2       # if count non-zero then loop
         SBC     a1,a4,a4                # set result to Carry (a4 is 0)
-        LDMFD   sp!,{v1-v6,pc}^         # restore work regs and return
+        LDMFD   sp!,{v1-v6,pc}          # restore work regs and return
 
 # extern uintD subfrom_loop_down (uintD* sourceptr, uintD* destptr, uintC count);
 #       entry
@@ -1326,7 +1326,7 @@
 LABEL(subfrom_loop_down_l4)             # drop through for better instr. timings
         BICS    a4,a3,#3                # set counter to multiple of 4
         SBCEQ   a1,a4,a4                # set result to Carry (a4 is 0)
-        LDMEQFD sp!,{pc}^               # and return
+        LDMEQFD sp!,{pc}                # and return
         STMFD   sp!,{v1-v5}             # save work regs
         B       subfrom_loop_down_l2    # branch if more subtracts to do
 LABEL(subfrom_loop_down_l0)
@@ -1344,7 +1344,7 @@
 LABEL(subfrom_loop_down_l1)
         BICS    a4,a3,#3                # set counter to multiple of 4
         MOVEQ   a1,#0                   # no subtracts, so C = 0
-        MOVEQS  pc,lr                   # if zero then we're done
+        MOVEQ   pc,lr                   # if zero then we're done
         CMP     a4,#0                   # set carry bit, since a4 > 0
         STMFD   sp!,{v1-v5,lr}          # save work regs
 LABEL(subfrom_loop_down_l2)
@@ -1359,7 +1359,7 @@
         TEQ     a4,#0                   # are we done ?
         BNE     subfrom_loop_down_l2    # if count non-zero then loop
         SBC     a1,a4,a4                # set result to Carry (a4 is 0)
-        LDMFD   sp!,{v1-v5,pc}^         # restore work regs and return
+        LDMFD   sp!,{v1-v5,pc}          # restore work regs and return
 
 # extern uintD dec_loop_down (uintD* ptr, uintC count);
 #       entry
@@ -1377,11 +1377,11 @@
         SUBS    a4,a4,#1                # align the total to a multiple of 2
         STR     a4,[a1]
         MOVCS   a1,#0                   # set result to 0
-        MOVCSS  pc,lr                   # return 0 if non-zero result
+        MOVCS   pc,lr                   # return 0 if non-zero result
 LABEL(dec_loop_down_l1)
         BICS    a4,a2,#1                # set counter to multiple of 2
         MVNEQ   a1,#0                   # return -1
-        MOVEQS  pc,lr                   # if zero then we're done
+        MOVEQ   pc,lr                   # if zero then we're done
         MOV     ip,a1                   # move ptr to ip
         MOV     a1,#0                   # set result to 0
         ANDS    a3,a4,#3
@@ -1390,10 +1390,10 @@
         SUBS    a3,a3,#1                # DEC the two words
         SUBCCS  a2,a2,#1                # stopping when first word non-zero
         STMDB   ip!,{a2,a3}             # store 2 results
-        MOVCSS  pc,lr                   # return 0 if any result non-zero
+        MOVCS   pc,lr                   # return 0 if any result non-zero
         SUBS    a4,a4,#2                # decrement counter by 2
         MVNEQ   a1,#0                   # if finished loop then
-        MOVEQS  pc,lr                   # return -1
+        MOVEQ   pc,lr                   # return -1
 LABEL(dec_loop_down_l3)                 # now a multiple of 4 words
         STMFD   sp!,{v1,lr}             # save work regs
 LABEL(dec_loop_down_l2)
@@ -1403,11 +1403,11 @@
         SUBCCS  a3,a3,#1
         SUBCCS  a2,a2,#1
         STMDB   ip!,{a2,a3,v1,lr}       # store 4 results
-        LDMCSFD sp!,{v1,pc}^            # return 0 if any carry
+        LDMCSFD sp!,{v1,pc}             # return 0 if any carry
         SUBS    a4,a4,#4                # decrement counter by 4
         BGT     dec_loop_down_l2        # if count still positive then loop
         MVN     a1,#0
-        LDMFD   sp!,{v1,pc}^            # restore work regs and return -1
+        LDMFD   sp!,{v1,pc}             # restore work regs and return -1
 
 # extern void neg_loop_down (uintD* ptr, uintC count);
 #       entry
@@ -1421,7 +1421,7 @@
 GLABEL(neg_loop_down)
         CMPS    a2,#0                   # count = 0 ?
         MOVEQ   a1,#0                   # yup, so return 0
-        MOVEQS  pc,lr
+        MOVEQ   pc,lr
 LABEL(neg_loop_down_l1)                 # skip all the zero words first
         LDR     a3,[a1,#-4]!            # compare words against zero
         CMPS    a3,#0                   # downwards in memory
@@ -1429,13 +1429,13 @@
         SUBS    a2,a2,#1                # reduce count of words
         BNE     neg_loop_down_l1        # more ?, so loop
         MOV     a1,#0                   # return 0
-        MOVS    pc,lr
+        MOV     pc,lr
 LABEL(neg_loop_down_l2)
         RSB     a3,a3,#0                # first non-zero word = -word
         STR     a3,[a1]
         SUBS    a2,a2,#1
         MVNEQ   a1,#0                   # done ? -> return -1
-        MOVEQS  pc,lr
+        MOVEQ   pc,lr
                                         # now NOT rest of the words
         ANDS    a3,a2,#3                # multiple of 4 words ?
         BEQ     neg_loop_down_l3        # yup, so branch
@@ -1453,7 +1453,7 @@
 LABEL(neg_loop_down_l3)
         BICS    a4,a2,#3                # set counter to multiple of 4
         MVNEQ   a1,#0                   # set result to -1
-        MOVEQS  pc,lr                   # if zero then we're done
+        MOVEQ   pc,lr                   # if zero then we're done
         STMFD   sp!,{lr}                # save work regs
 LABEL(neg_loop_down_l4)
         LDMDB   a1,{a2,a3,ip,lr}        # load 4 words in one go,NO writeback
@@ -1465,7 +1465,7 @@
         SUBS    a4,a4,#4                # decrement counter by 4
         BGT     neg_loop_down_l4        # if count still positive then loop
         MVN     a1,#0                   # set result to -1
-        LDMFD   sp!,{pc}^               # restore work regs and return -1
+        LDMFD   sp!,{pc}                # restore work regs and return -1
 
 # extern uintD shift1left_loop_down (uintD* ptr, uintC count);
 #       entry
@@ -1485,7 +1485,7 @@
 LABEL(shift1left_loop_down_l1)
         BICS    a4,a2,#1                # set counter to multiple of 2
         ADCEQ   a1,a4,a4                # if zero set result to C (a4 is 0)
-        MOVEQS  pc,lr                   # and return
+        MOVEQ   pc,lr                   # and return
         ANDS    a3,a4,#3                # multiple of 4 words ?
         BEQ     shift1left_loop_down_l3 # yup, so branch
         LDMDB   a1,{a2,a3}              # load 2 words in one go
@@ -1494,7 +1494,7 @@
         STMDB   a1!,{a2,a3}             # store 2 results
         BICS    a4,a4,#2                # decrement counter by 2
         ADCEQ   a1,a4,a4                # set result to Carry (a4 is 0)
-        MOVEQS  pc,lr                   # and return
+        MOVEQ   pc,lr                   # and return
 LABEL(shift1left_loop_down_l3)          # now a multiple of 4 words
         STMFD   sp!,{lr}                # save work regs
 LABEL(shift1left_loop_down_l2)
@@ -1508,7 +1508,7 @@
         TEQ     a4,#0                   # are we done ?
         BNE     shift1left_loop_down_l2 # if count non-zero then loop
         ADC     a1,a4,a4                # set result to Carry (a4 is 0)
-        LDMFD   sp!,{pc}^               # restore work regs and return 1
+        LDMFD   sp!,{pc}                # restore work regs and return 1
 
 # extern uintD shiftleft_loop_down (uintD* ptr, uintC count, uintC i, uintD carry);
 #       entry
@@ -1542,7 +1542,7 @@
 LABEL(shiftleft_loop_down_l1)
         BICS    ip,a2,#3                # set counter to multiple of 4
         MOVEQ   a1,a4                   # if zero then we're done
-        LDMEQFD sp!,{v6,pc}^            # so return last shift out
+        LDMEQFD sp!,{v6,pc}             # so return last shift out
         STMFD   sp!,{v1-v3}             # save work regs
 LABEL(shiftleft_loop_down_l2)
         LDMDB   a1,{a2,v1,v2,v3}        # load 4 words in one go
@@ -1558,7 +1558,7 @@
         SUBS    ip,ip,#4                # decrement counter by 4
         BGT     shiftleft_loop_down_l2  # if count still positive then loop
         MOV     a1,a4                   # result = last shift out
-        LDMFD   sp!,{v1-v3,v6,pc}^      # restore work regs and return
+        LDMFD   sp!,{v1-v3,v6,pc}       # restore work regs and return
 
 # extern uintD shiftleftcopy_loop_down (uintD* sourceptr, uintD* destptr, uintC count, uintC i);
 #       entry
@@ -1593,7 +1593,7 @@
 LABEL(shiftleftcopy_loop_down_l1)
         BICS    ip,a3,#3                # set counter to multiple of 4
         MOVEQ   a1,v5                   # if zero then we're done
-        LDMEQFD sp!,{v5,v6,pc}^         # so return last shift out
+        LDMEQFD sp!,{v5,v6,pc}          # so return last shift out
         STMFD   sp!,{v1-v3}             # save work regs
 LABEL(shiftleftcopy_loop_down_l2)
         LDMDB   a1!,{a3,v1,v2,v3}       # load 4 words in one go
@@ -1609,7 +1609,7 @@
         SUBS    ip,ip,#4                # decrement counter by 4
         BGT     shiftleftcopy_loop_down_l2      # if count still positive then loop
         MOV     a1,v5                   # result = last shift out
-        LDMFD   sp!,{v1-v3,v5,v6,pc}^   # restore work regs and return
+        LDMFD   sp!,{v1-v3,v5,v6,pc}    # restore work regs and return
 
 # extern uintD shift1right_loop_up (uintD* ptr, uintC count, uintD carry);
 #       entry
@@ -1630,7 +1630,7 @@
 LABEL(shift1right_loop_up_l1)
         BICS    a4,a2,#1                # set counter to multiple of 2
         MOVEQ   a1,a4,RRX               # if zero set result to C (a4 is 0)
-        MOVEQS  pc,lr                   # and return
+        MOVEQ   pc,lr                   # and return
         ANDS    a3,a4,#3                # multiple of 4 words ?
         BEQ     shift1right_loop_up_l3  # yup, so branch
         LDMIA   a1,{a2,a3}              # load 2 words in one go
@@ -1639,7 +1639,7 @@
         STMIA   a1!,{a2,a3}             # store 2 results
         BICS    a4,a4,#2                # decrement counter by 2
         ADCEQ   a1,a4,a4                # set result to Carry (a4 is 0)
-        MOVEQS  pc,lr                   # and return
+        MOVEQ   pc,lr                   # and return
 LABEL(shift1right_loop_up_l3)           # now a multiple of 4 words
         STMFD   sp!,{lr}                # save work regs
 LABEL(shift1right_loop_up_l2)
@@ -1653,7 +1653,7 @@
         TEQ     a4,#0                   # are we done ?
         BNE     shift1right_loop_up_l2  # if count non-zero then loop
         MOV     a1,a4,RRX               # set result to Carry (a4 is 0)
-        LDMFD   sp!,{pc}^               # restore work regs and return 1
+        LDMFD   sp!,{pc}                # restore work regs and return 1
 
 # extern uintD shiftright_loop_up (uintD* ptr, uintC count, uintC i);
 #       entry
@@ -1688,7 +1688,7 @@
 LABEL(shiftright_loop_up_l1)
         BICS    ip,a2,#3                # set counter to multiple of 4
         MOVEQ   a1,a4                   # if zero then we're done
-        LDMEQFD sp!,{v6,pc}^            # so return last shift out
+        LDMEQFD sp!,{v6,pc}             # so return last shift out
         STMFD   sp!,{v1-v3}             # save work regs
 LABEL(shiftright_loop_up_l2)
         LDMIA   a1,{v1,v2,v3,lr}        # load 4 words in one go
@@ -1704,7 +1704,7 @@
         SUBS    ip,ip,#4                # decrement counter by 4
         BGT     shiftright_loop_up_l2   # if count still positive then loop
         MOV     a1,a4                   # result = last shift out
-        LDMFD   sp!,{v1-v3,v6,pc}^      # restore work regs and return
+        LDMFD   sp!,{v1-v3,v6,pc}       # restore work regs and return
 
 # extern uintD shiftrightsigned_loop_up (uintD* ptr, uintC count, uintC i);
 #       entry
@@ -1759,7 +1759,7 @@
 LABEL(shiftrightcopy_loop_up_l1)
         BICS    ip,a3,#3                # set counter to multiple of 4
         MOVEQ   a1,v5                   # if zero then we're done
-        LDMEQFD sp!,{v5,v6,pc}^         # so return last shift out
+        LDMEQFD sp!,{v5,v6,pc}          # so return last shift out
         STMFD   sp!,{v1-v3}             # save work regs
 LABEL(shiftrightcopy_loop_up_l2)
         LDMIA   a1!,{v1,v2,v3,lr}       # load 4 words in one go
@@ -1775,7 +1775,7 @@
         SUBS    ip,ip,#4                # decrement counter by 4
         BGT     shiftrightcopy_loop_up_l2       # if count still positive then loop
         MOV     a1,v5                   # result = last shift out
-        LDMFD   sp!,{v1-v3,v5,v6,pc}^   # restore work regs and return
+        LDMFD   sp!,{v1-v3,v5,v6,pc}    # restore work regs and return
 
 #ifndef HAVE_umull
 # mulu32_64_vregs
@@ -1800,7 +1800,7 @@
         ADDCS   v2,v2,#0x10000          # carry from above add
         ADDS    v1,v4,ip,LSL #16        # x is now bottom 32 bits of result
         ADC     ip,v2,ip,LSR #16        # hi is top 32 bits
-        MOVS    pc,lr
+        MOV     pc,lr
 #endif # HAVE_umull
 
 # extern uintD mulusmall_loop_down (uintD digit, uintD* ptr, uintC len, uintD newdigit);
@@ -1816,7 +1816,7 @@
 GLABEL(mulusmall_loop_down)
         CMP     a3,#0
         MOVEQ   a1,a4
-        MOVEQS  pc,lr
+        MOVEQ   pc,lr
 #ifdef HAVE_umull
         STMFD   sp!,{v1,lr}
 LABEL(mulusmall_loop_down_l1)
@@ -1828,7 +1828,7 @@
         SUBS    a3,a3,#1                # len--
         BNE     mulusmall_loop_down_l1  # until len==0
         MOV     a1,a4                   # return carry
-        LDMFD   sp!,{v1,pc}^
+        LDMFD   sp!,{v1,pc} 
 #else
         STMFD   sp!,{v1-v2,lr}
 LABEL(mulusmall_loop_down_l1)
@@ -1850,7 +1850,7 @@
         SUBS    a3,a3,#1                # len--
         BNE     mulusmall_loop_down_l1  # until len==0
         MOV     a1,a4                   # return carry
-        LDMFD   sp!,{v1-v2,pc}^
+        LDMFD   sp!,{v1-v2,pc} 
 #endif
 
 # extern void mulu_loop_down (uintD digit, uintD* sourceptr, uintD* destptr, uintC len);
@@ -1875,7 +1875,7 @@
         SUBS    a4,a4,#1                # len--
         BNE     mulu_loop_down_l1       # until len==0
         STR     v5,[a3,#-4]!            # *--destptr = carry
-        LDMFD   sp!,{v1,v5,pc}^
+        LDMFD   sp!,{v1,v5,pc} 
 #else
         STMFD   sp!,{v1-v5,lr}
         MOV     v5,#0
@@ -1888,7 +1888,7 @@
         SUBS    a4,a4,#1                # len--
         BNE     mulu_loop_down_l1       # until len==0
         STR     v5,[a3,#-4]!            # *--destptr = carry
-        LDMFD   sp!,{v1-v5,pc}^
+        LDMFD   sp!,{v1-v5,pc} 
 #endif
 
 # extern void muluadd_loop_down (uintD digit, uintD* sourceptr, uintD* destptr, uintC len);
@@ -1916,7 +1916,7 @@
         SUBS    a4,a4,#1                # len--
         BNE     muluadd_loop_down_l1    # until len==0
         MOV     a1,v5                   # return carry
-        LDMFD   sp!,{v1,v5,pc}^
+        LDMFD   sp!,{v1,v5,pc} 
 #else
         STMFD   sp!,{v1-v5,lr}
         MOV     v5,#0
@@ -1932,7 +1932,7 @@
         SUBS    a4,a4,#1                # len--
         BNE     muluadd_loop_down_l1    # until len==0
         MOV     a1,v5                   # return carry
-        LDMFD   sp!,{v1-v5,pc}^
+        LDMFD   sp!,{v1-v5,pc} 
 #endif
 
 # extern void mulusub_loop_down (uintD digit, uintD* sourceptr, uintD* destptr, uintC len);
@@ -1960,7 +1960,7 @@
         SUBS    a4,a4,#1                # len--
         BNE     mulusub_loop_down_l1    # until len==0
         MOV     a1,v5                   # return carry
-        LDMFD   sp!,{v1,v5,pc}^
+        LDMFD   sp!,{v1,v5,pc} 
 #else
         STMFD   sp!,{v1-v5,lr}
         MOV     v5,#0
@@ -1976,7 +1976,7 @@
         SUBS    a4,a4,#1                # len--
         BNE     mulusub_loop_down_l1    # until len==0
         MOV     a1,v5                   # return carry
-        LDMFD   sp!,{v1-v5,pc}^
+        LDMFD   sp!,{v1-v5,pc} 
 #endif
 
         END
