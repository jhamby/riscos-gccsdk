; ***************** rink run-time system ****************


; *******************************************************

r0 RN 0
r1 RN 1
r2 RN 2
r3 RN 3
r4 RN 4
r5 RN 5
r6 RN 6
r7 RN 7
r8 RN 8
r9 RN 9
a1 RN 0
a2 RN 1
a3 RN 2
a4 RN 3
v1 RN 4
v2 RN 5
v3 RN 6
v4 RN 7
v5 RN 8
v6 RN 9
sl RN 10
fp RN 11
ip RN 12
sp RN 13
lr RN 14
pc RN 15

  GET h.asmswis

  AREA |rink$$code|, CODE, READONLY

 ; rink_codehandle rink_load(char *code_name, char *links_name, char *error, rink_check ck);
 ; void *rink_call(rink_codehandle ch, int entry, ...);
 ; void rink_unload(rink_codehandle ch);
 ; void *rink_fn(rink_seghandle cd, int entry);


  EXPORT |rink_load|
  EXPORT |rink_unload|
  EXPORT |rink_fn|
  EXPORT |rink_enum_named|
  EXPORT |_rink_default_trans|

  IMPORT |_rink_pointerblock|
  IMPORT |_rink_alloc|
  IMPORT |_rink_free|
  IMPORT |_rink_errtrans|

codehdr_id             EQU 0
codehdr_main_version   EQU 8
codehdr_code_version   EQU 12
codehdr_fn_entries     EQU 16
codehdr_code_size      EQU 20
codehdr_zero_init_size EQU 24
codehdr_named_entries  EQU 28
codehdr_named_offset   EQU 32
codehdr_res1           EQU 36
codehdr_res2           EQU 40
codehdr_size           EQU 44

nameden_fn   EQU 0
nameden_name EQU 4

linkshdr_id      EQU 0
linkshdr_version EQU 4
linkshdr_n_links EQU 8
linkshdr_res     EQU 12
linkshdr_size    EQU 16

rink_load_x
  DCB "rink_load",0
  ALIGN
rink_load_y DCD &ff000000 + rink_load_y - rink_load_x

  ; a1 = pointer to segement name
  ; a2 = pointer to links name
  ; a3 = pointer to segment handle
  ; a4 = rink check block
rink_load
  STMFD sp!,{v1-v6,lr}

    ; a1 = pointer to code filename
    ; a2 = pointer to links filename
    ; a3 = pointer to segment handle for return
    ; a4 = pointer to four words which the first four words of the code must match
  MOV   v6,a4
  MOV   v5,a3
  MOV   v4,a2
  MOV   r1,a1
  MOV   r0,#&4f
  SWI   XOS_Find
  BVS   load_error
  MOV   v3,r0             ; file handle
  MOV   r1,r0
  MOV   r0,#4
  LDR   r2,read_hdr_addr
  MOV   r3,#codehdr_size
  SWI   XOS_GBPB
  BVS   load_close_error

  LDR   r0,read_hdr_addr
  LDMIA r0,{r0-r3}
  LDMIA v6,{v1-v2}
  CMP   r0,v1
  CMPEQ r1,v2
  MOVNE r0,#-2           ; not a rink segment
  BNE   load_close_error
  LDMIA v6,{r0,r1,v1,v2}
;  CMP   r2,v1           ; code only allows current version
;  CMPNE r3,v2
;  MOVNE r0,#-2
;  BNE   load_close_error
  CMP   r2,v1            ; major version is greater than given version
  MOVGT r0,#-4           ; version too recent
  BGT   load_close_error

  LDR   a2,read_hdr_addr
  LDR   a1,[a2,#codehdr_code_size]
  LDR   a2,[a2,#codehdr_zero_init_size]
  ADD   a1,a1,a2
  BL    |_rink_alloc|
  CMP   a1,#0
  MOVEQ r0,#-1
  BEQ   load_close_error
  MOV   v6,a1              ; code file at v6

  MOV   r2,a1
  MOV   r0,#3
  MOV   r1,v3
  LDR   r3,read_hdr_addr
  LDR   r3,[r3,#codehdr_code_size]
  MOV   r4,#0
  SWI   XOS_GBPB           ; load code into memory
  BVS   load_close_error

  MOV   r0,#0
  MOV   r1,v3
  SWI   XOS_Find           ; close file
  BVS   load_error

    ; initialise the zero initialised data to zeros...
  LDR   r2,[v6,#codehdr_zero_init_size]
  CMP   r2,#0
  BLE   Arl_l1
  MOV   r0,#0
  LDR   r1,[v6,#codehdr_code_size]
  ADD   r1,r1,v6
rl_l1
    STR   r0,[r1],#4
    SUBS  r2,r2,#4
  BGT   rl_l1
Arl_l1

    ; load links
  MOV   r0,#17
  MOV   r1,v4      ; links filename
  SWI   XOS_File
  BVS   load_free_error
  CMP   r0,#1      ; file
  BNE   links_not_found
  MOV   a1,r4
  BL    |_rink_alloc|
  CMP   a1,#0
  MOVEQ r0,#-1
  BEQ   load_free_error
  MOV   v3,a1               ; pointer to links block

  MOV   r2,a1
  MOV   r0,#16
  MOV   r1,v4
  MOV   r3,#0
  SWI   XOS_File
  BVS   load_free2_error

    ; test file
  LDMIA v3,{a1-a2}
  ADR   a3,rink_links_check
  LDMIA a3,{a3-a4}
  CMP   a1,a3
  CMPEQ a2,a4
  MOVNE r0,#-3
  BNE   load_free2_error

    ; perform links
  LDR   v2,rink_pointerblock_addr
  ADD   v1,v3,#linkshdr_size
  LDR   a4,[v3,#linkshdr_n_links]
  CMP   a4,#0
  BEQ   no_links
    ; v3 = pointer to links file
    ; v6 = pointer to code file
    ; v2 = pointer to index of function addresses
    ; v1 = pointer to current link
    ; a4 = number of links to go
    ; v4 = temporary scrap register
link_loop
    LDR   a3,[v1],#4
    AND   a2,a3,#&C0000000
    BIC   a3,a3,#&C0000000
      ; a3 = offset of link
      ; a2 = link type
    CMP   a2,#&C0000000
    BEQ   link_type3

    LDR   a1,[v6,a3]
      ; a1 = contents of word to relocate

    CMP   a2,#&40000000
    BEQ   link_type1
    CMP   a2,#&00000000
    BEQ   link_type0
;    CMP   a2,#&80000000
;    BEQ   link_type2

link_type2               ; add absolute address of symbol
    LDR   a2,[v1],#4     ; load index of symbol
    LDR   a2,[v2,a2,LSL#2]   ; load address of symbol
    ADD   a1,a1,a2
    B     link_done

link_type0               ; add base of code
    ADD   a1,a1,v6
    B     link_done

link_type1               ; add offset of symbol from code base to B instruction
    LDR   a2,[v1],#4     ; load index of symbol
    B     link_type1_unpacked

link_type3               ; unpack type 1 link
    MOV   a2,a3,LSR#16   ; retrieve symbol index
    MOV   a3,a3,LSL#16
    MOV   a3,a3,LSR#16   ; clear symbol index bits (quicker than LDR and BIC)
    LDR   a1,[v6,a3]
link_type1_unpacked
    LDR   a2,[v2,a2,LSL#2]   ; load address of symbol
    SUB   a2,a2,v6
    MOV   v4,a1,LSL#8
    ADD   v4,v4,a2,LSL#6     ; offset / 4
    AND   a1,a1,#&ff000000
    ORR   a1,a1,v4,LSR#8
link_done
    STR   a1,[v6,a3]

    SUBS  a4,a4,#1
  BGT   link_loop

no_links

  MOV   a1,v3               ; free links block
  BL    |_rink_free|

;  MOV   a1,v6
  STR   v6,[v5]
  MOV   a1,#0
  LDMFD sp!,{v1-v6,pc}^

rink_pointerblock_addr
  DCD |_rink_pointerblock|

links_not_found
  MOV   r2,r0
  MOV   r0,#19
  SWI   XOS_File
  B     load_free_error

load_free2_error
  STMFD sp!,{r0}
  MOV   a1,v3
  BL    |_rink_free|
  LDMFD sp!,{r0}
load_free_error
  STMFD sp!,{r0}
  MOV   a1,v6
  BL    |_rink_free|
  LDMFD sp!,{r0}
  B     load_error

load_close_error
  STMFD sp!,{r0}
  MOV   r0,#0
  MOV   r1,v3
  SWI   XOS_Find
  LDMFD sp!,{r0}
load_error
  CMP   r0,#0
  BLLT  |_rink_errtrans|
  LDMFD sp!,{v1-v6,pc}^

; *******************************************************

rink_fn_x
  DCB "rink_fn",0
  ALIGN
rink_fn_y DCD &ff000000 + rink_fn_y - rink_fn_x

rink_fn
    ; a1 = pointer to block
    ; a2 = number of function to call
  LDR   a4,[a1,#codehdr_fn_entries]    ; number of entries in the header
  CMP   a4,a2
  BLE   rink_fn_no_fn                  ; return now if header doesn't contain enough functions
  ADD   a2,a1,a2,LSL#2                 ; a2 = a1 + a2 * 4
  ADD   a2,a2,#codehdr_size            ; a2 = pointer to header entry of function
  LDR   a3,[a2]                        ; load fn offset
  CMP   a3,#0
  BEQ   rink_fn_no_fn
  ADD   a1,a3,a1                       ; a1 = address of function to call
  MOV   pc,lr
    ; a4 = address of function

rink_fn_no_fn
  MOV   a1,#0
  MOV   pc,lr

; *******************************************************

; ***********
;    obsolete function - if replacing, modify rink to include number of arguements in header

; rink_call_x
;   DCB "rink_call",0
;   ALIGN
; rink_call_y DCD &ff000000 + rink_call_y - rink_call_x

; rink_call
;     ; a1 = pointer to block
;     ; a2 = number of function to call
;     ; a3, a4 = first two arguments of call
;   STMFD sp!,{a3-a4}
;   LDR   a4,[a1,#codehdr_fn_entries]    ; number of entries in the header
;   CMP   a4,a2
;   ADDLE sp,sp,#8
; ;  LDMLEFD sp!,{a3-a4}
;   MOVLE pc,lr                          ; return now if header doesn't contain enough functions
;   ADD   a2,a1,a2,LSL#3                 ; a2 = a1 + a2 * 8
;   ADD   a2,a2,#codehdr_size            ; a2 = pointer to header entry of function
;   LDR   a3,[a2,#4]                     ; load fn arguments
;   LDR   a4,[a2]                        ; load fn offset
;   CMP   a4,#0
;   ADDEQ sp,sp,#8
; ;  LDMEQFD sp!,{a3-a4}
;   MOVEQ pc,lr                          ; return now if not defined in header
;   ADD   a4,a4,a1                       ; a3 = address of function to call
;     ; a4 = address of function
;     ; a3 = number of arguments of function
;     ; a2 = address of header entry     - no longer needed
;     ; a1 = pointer to code block       - no longer needed
;   CMP   a3,#2
;   LDMLEFD sp!,{a1-a2}
;   MOVLE pc,a4                          ; call function

;   CMP   a3,#3
;   BEQ   call_three_arguments
;   CMP   a3,#4
;   BEQ   call_four_arguments
;     ; right then, there are rather a lot of arguments to worry about...
;     ; a return address will be necesary to sort out the stack pointer...

;     ; all arguments are on the stack
;   LDR   a1,rink_fn_addr_addr
;   STR   a4,[a1]
;   MOV   a4,sp
;   LDR   a2,rink_reg_store_addr
;   MOV   a1,v1            ; make sure stored bits are stored in the right order
;   ADD   v1,sp,#8         ; store old stack pointer
;   STMIA a2,{a1,v1,lr}    ; and link register
;   ADD   a2,sp,a3,LSL#2   ; point after last entry on stack
;   SUB   a3,a3,#4         ; four arguments in registers
; copy_arg_loop
;     LDR   a1,[a2,#-4]!
;     STR   a1,[sp,#-4]!
;     SUBS  a3,a3,#1
;   BGT   copy_arg_loop
;   LDMIA a4,{a1-a4}
;   LDR   v1,rink_fn_addr_addr
;   LDR   v1,[v1]
;   MOV   lr,pc
;   MOV   pc,v1
;   LDR   a2,rink_reg_store_addr
;   LDMIA a2,{v1,sp,pc}^

; call_three_arguments
;   LDMIA sp,{a1-a3}       ; load arguments
;   STMFD sp!,{lr}
;   MOV   lr,pc
;   MOV   pc,a4            ; call function
;   LDMFD sp!,{lr}
;   ADD   sp,sp,#8         ; skip over first two arguments to restore stack pointer
;   MOV   pc,lr

; call_four_arguments
;   STMFD sp!,{v1,lr}
;   MOV   v1,a4
;   ADD   a1,sp,#8         ; get address of arguments on stack
;   LDMIA a1,{a1-a4}       ; load arguments
;   MOV   lr,pc
;   MOV   pc,v1            ; call function
;   LDMFD sp!,{v1,lr}
;   ADD   sp,sp,#8         ; skip over first two arguments to restore stack pointer
;   MOV   pc,lr

; *******************************************************

rink_enum_named_x
  DCB "rink_enum_named",0
  ALIGN
rink_enum_named_y DCD &ff000000 + rink_enum_named_y - rink_enum_named_x

  ; a1 = pointer to code
  ; a2 = pointer to entry number
  ; a3 = pointer to pointer to function name
  ; returns
  ; a1 = pointer to function
rink_enum_named
  STMFD sp!,{v1-v3,lr}

  LDR   v1,[a2]
  CMP   v1,#0
  BLT   rink_enum_named_end
  LDR   v3,[a1,#codehdr_named_entries]
  CMP   v1,v3
  BGE   rink_enum_named_end
    ; entry number valid - find out about it all
  LDR   a4,[a1,#codehdr_named_offset]
  ADD   a4,a4,a1
  ADD   a4,a4,v1,LSL#3      ; * 8
  LDR   v2,[a4,#nameden_name]
  ADD   v2,v2,a1
  STR   v2,[a3]             ; store pointer to name of function
  LDR   a4,[a4,#nameden_fn]
  ADD   a1,a1,a4
  ADD   v1,v1,#1
  CMP   v1,v3
  MOVGE v1,#-1
  STR   v1,[a2]
  LDMFD sp!,{v1-v3,pc}^

rink_enum_named_end
  MOV   a1,#-1
  STR   a1,[a2]
  MOV   a1,#0
  STR   a1,[a3]
  LDMFD sp!,{v1-v3,pc}^

; *******************************************************

rink_readversion
  ADD   a1,a1,#codehdr_main_version
  MOV   pc,lr

  EXPORT rink_readversion

; *******************************************************

e1
  DCD -1
  DCB "No room",0
  ALIGN
e2
  DCD -2
  DCB "Not a rink segment",0
  ALIGN
e3
  DCD -3
  DCB "Not a rink links file",0
  ALIGN
e4
  DCD -4
  DCB "Segment is too recent",0
  ALIGN

errors
  DCD e1,e2,e3,e4

|_rink_default_trans|
  RSB   a1,a1,#0          ; a1 = 0 - a1  : make number positive
  SUB   a1,a1,#1          ; subtract one to make it the number of the error in the list
  ADR   a2,errors
  LDR   a1,[a2,a1,LSL#2]  ; load address of error block to return
  MOV   pc,lr

; *******************************************************

rink_unload_x
  DCB "rink_unload",0
  ALIGN
rink_unload_y DCD &ff000000 + rink_unload_y - rink_unload_x

rink_unload
  B     |_rink_free|

; *******************************************************

rink_links_check
  DCB "rink"
  DCD 100
  ALIGN

read_hdr_addr
  DCD read_hdr

; *******************************************************

  AREA |rink$$data|, DATA, NOINIT

read_hdr
  % codehdr_size

; rink_reg_store
;   DCD 0, 0, 0, 0

;   ALIGN
; rink_fn_addr
;   DCD 0

; *******************************************************


  END

