diff -r -u -N ./acorn/acornzip.c ../zip30-joty/acorn/acornzip.c
--- ./acorn/acornzip.c	2009-01-17 20:07:14.000000000 +0100
+++ ../zip30-joty/acorn/acornzip.c	2009-01-18 13:48:29.000000000 +0100
@@ -81,9 +81,9 @@
     if ((subwild = strchr(wildtail + 1, '.')) != NULL) {
         /* this "+ 1" dodges the   ^^^ hole left by *glue == 0 */
         *(subwild++) = 0;               /* wildtail = one component pattern */
-        newlen = strlen(whole) + strlen(subwild) + 32;
+        newlen = strlen(whole) + strlen(subwild) + MAXFILENAMELEN;
     } else
-        newlen = strlen(whole) + 31;
+        newlen = strlen(whole) + MAXFILENAMELEN;
     if (!dir || !(newwhole = malloc(newlen))) {
         if (glue)
             *glue = plug;
@@ -99,7 +99,7 @@
         goto ohforgetit;
     }
 
-    while (name = readd(dir)) {
+    while ((name = readd(dir)) != 0) {
         if (MATCH(wildtail, name, 0)) {
             strcpy(newwhole + newlen, name);
             if (subwild) {
diff -r -u -N ./acorn/makefile ../zip30-joty/acorn/makefile
--- ./acorn/makefile	2009-01-17 20:07:14.000000000 +0100
+++ ../zip30-joty/acorn/makefile	2009-01-18 15:30:53.000000000 +0100
@@ -15,9 +15,8 @@
 #   LFLAGS2   flags after obj file list (libraries, etc)
 #
 LIB      =
-CBASE    = -throwback -wn -DASMV -apcs 3/26
-CFLAGS   = $(CBASE) -IC:,@.
-ASMFLAGS = -Throwback -Stamp -NoCache -CloseExec -quit -apcs 3/26
+CFLAGS   = -throwback -wn -wd -DASMV -IC:,@.
+ASMFLAGS = -Throwback -Stamp -NoCache -CloseExec -quit
 LFLAGS1  =
 LFLAGS2  = $(LIB) C:o.Stubs
 
@@ -43,20 +42,20 @@
 
 install:  %.zip %.zipnote %.zipsplit %.zipcloak %.zipsfx \
           zip zipnote zipsplit zipcloak zipsfx
-        $(SQUEEZE) zip %.zip
-        $(SQUEEZE) zipnote %.zipnote
-        $(SQUEEZE) zipsplit %.zipsplit
-        $(SQUEEZE) zipcloak %.zipcloak
-        copy acorn.zipsfx %.zipsfx ~CVF
+	$(SQUEEZE) zip %.zip
+	$(SQUEEZE) zipnote %.zipnote
+	$(SQUEEZE) zipsplit %.zipsplit
+	$(SQUEEZE) zipcloak %.zipcloak
+	copy acorn.zipsfx %.zipsfx ~CVF
 
 # suffix rules
 .SUFFIXES:      _.o .o .c
 .c_.o:
-        $(CC) $(CFLAGS) -DUTIL -c $*.c -o $*_.o
+	$(CC) $(CFLAGS) -DUTIL -c $*.c -o $*_.o
 .c.o:
-        $(CC) $(CFLAGS) -c $<
+	$(CC) $(CFLAGS) -c $<
 .s.o:
-        $(ASM) $(ASMFLAGS) -from @*.s -to @*.o
+	$(ASM) $(ASMFLAGS) -from @*.s -to @*.o
 
 # rules for zip, zipnote, zipcloak and zipsplit
 $(OBJZ): $(ZIP_H)
@@ -71,45 +70,48 @@
 zip.o crypt.o ttyio.o zipcloak.o crypt_.o: ttyio.h
 
 crc32_.o: crc32.c
-        $(CC) $(CFLAGS) -DUTIL -c c.crc32 -o o.crc32_
+	$(CC) $(CFLAGS) -DUTIL -c crc32.c -o crc32_.o
 crypt_.o: crypt.c
-        $(CC) $(CFLAGS) -DUTIL -c c.crypt -o o.crypt_
+	$(CC) $(CFLAGS) -DUTIL -c crypt.c -o crypt_.o
 util_.o: util.c
-        $(CC) $(CFLAGS) -DUTIL -c c.util -o o.util_
+	$(CC) $(CFLAGS) -DUTIL -c util.c -o util_.o
 fileio_.o: fileio.c
-        $(CC) $(CFLAGS) -DUTIL -c c.fileio -o o.fileio_
+	$(CC) $(CFLAGS) -DUTIL -c fileio.c -o fileio_.o
 zipfile_.o: zipfile.c
-        $(CC) $(CFLAGS) -DUTIL -c c.zipfile -o o.zipfile_
+	$(CC) $(CFLAGS) -DUTIL -c zipfile.c -o zipfile_.o
 acornzip_.o: acorn/acornzip.c $(ZIP_H)
-        $(CC) $(CFLAGS) -DUTIL -c acorn/acornzip.c -o o.acornzip_
+	$(CC) $(CFLAGS) -DUTIL -c acorn/acornzip.c -o acornzip_.o
 
 riscos.o: acorn/riscos.c acorn/riscos.h
-        $(CC) $(CFLAGS) -c acorn/riscos.c
+	$(CC) $(CFLAGS) -c acorn/riscos.c
 
 acornzip.o: acorn/acornzip.c $(ZIP_H)
-        $(CC) $(CFLAGS) -c acorn/acornzip.c
+	$(CC) $(CFLAGS) -c acorn/acornzip.c
+
+swiven.o: acorn/swiven.c acorn/swiven.h
+	$(CC) $(CFLAGS) -c acorn/swiven.c
 
 match.o: acorn/match.s
-        $(ASM) $(ASMFLAGS) -from acorn.s.match -to o.match
+	$(ASM) $(ASMFLAGS) -from acorn.s.match -to match.o
 
 sendbits.o: acorn/sendbits.s
-        $(ASM) $(ASMFLAGS) -from acorn.s.sendbits -to o.sendbits
-
-swiven.o: acorn/swiven.s
-        $(ASM) $(ASMFLAGS) -from acorn.s.swiven -to o.swiven
+	$(ASM) $(ASMFLAGS) -from acorn.s.sendbits -to sendbits.o
 
-zip:      $(OBJZ) $(OBJI) $(OBJA)
-          $(BIND) -o zip$(E) $(LFLAGS1) $(OBJZ) $(OBJI) $(OBJA) $(LFLAGS2)
+zip:	$(OBJZ) $(OBJI) $(OBJA)
+	$(BIND) -o zip$(E) $(LFLAGS1) $(OBJZ) $(OBJI) $(OBJA) $(LFLAGS2)
 zipnote:  $(OBJN)
-          $(BIND) -o zipnote$(E) $(LFLAGS1) $(OBJN) $(LFLAGS2)
+	$(BIND) -o zipnote$(E) $(LFLAGS1) $(OBJN) $(LFLAGS2)
 zipcloak: $(OBJC)
-          $(BIND) -o zipcloak$(E) $(LFLAGS1) $(OBJC) $(LFLAGS2)
+	$(BIND) -o zipcloak$(E) $(LFLAGS1) $(OBJC) $(LFLAGS2)
 zipsplit: $(OBJS)
-          $(BIND) -o zipsplit$(E) $(LFLAGS1) $(OBJS) $(LFLAGS2)
+	$(BIND) -o zipsplit$(E) $(LFLAGS1) $(OBJS) $(LFLAGS2)
 
-clean:  ;remove zip; remove zipcloak;
-        remove zipsplit; remove zipnote;
-        create o.!fake! 0
-        wipe o.* ~cf
+clean:
+	remove zip
+	remove zipcloak
+	remove zipsplit
+	remove zipnote
+	create o.!fake! 0
+	wipe o.* ~cf
 
 # end of Makefile
diff -r -u -N ./acorn/match.s ../zip30-joty/acorn/match.s
--- ./acorn/match.s	2009-01-17 20:07:14.000000000 +0100
+++ ../zip30-joty/acorn/match.s	2009-01-18 13:48:29.000000000 +0100
@@ -1,3 +1,4 @@
+#if defined(__aof__)
 ;===========================================================================
 ; Copyright (c) 1990-1999 Info-ZIP.  All rights reserved.
 ;
@@ -29,10 +30,10 @@
 WMASK      EQU  32767
 MAX_MATCH  EQU  258
 
-                AREA    |C$$code|, CODE, READONLY
+        AREA    |C$$code|, CODE, READONLY
 
 
-; r1 = chain_lenght
+; r1 = chain_length
 ; r2 = scan
 ; r3 = match
 ; r4 = len (tmp)
@@ -44,62 +45,46 @@
 ; lr = window
 ; fp = prev
 
-|__max_chain_length|
-        IMPORT  max_chain_length
-        DCD     max_chain_length
-|__window|
+	IMPORT  max_chain_length
         IMPORT  window
-        DCD     window
-|__prev|
         IMPORT  prev
-        DCD     prev
-|__prev_length|
         IMPORT  prev_length
-        DCD     prev_length
-|__strstart|
         IMPORT  strstart
-        DCD     strstart
-|__good_match|
         IMPORT  good_match
-        DCD     good_match
-|__nice_match|
         IMPORT  nice_match
-        DCD     nice_match
-|__match_start|
         IMPORT  match_start
-        DCD     match_start
 
-        DCB     "longest_match"
-        DCB     &00,&00,&00
+        =       "longest_match", 0
+        ALIGN
         DCD     &ff000010
 
         EXPORT  longest_match
 longest_match
         STMFD   sp!, {r4-r9,fp,lr}
 
-        LDR     fp, [pc, #|__prev|-.-8]
+        LDR     fp, =prev
 
-        LDR     r1, [pc, #|__max_chain_length|-.-8]
+        LDR     r1, =max_chain_length
         LDR     r1, [r1]
-        LDR     lr, [pc, #|__window|-.-8]
+        LDR     lr, =window
 
-        LDR     ip, [pc, #|__strstart|-.-8]
+        LDR     ip, =strstart
         LDR     ip, [ip]
         ADD     r2, lr, ip
-        LDR     r5, [pc, #|__prev_length|-.-8]
+        LDR     r5, =prev_length
         LDR     r5, [r5]
-        SUBS    ip, ip, #MAX_DIST-250       ; if r6 > MAX_DIST
-        SUBCSS  r6, ip, #250                ; r6 = r6 - MAXDIST
-        MOVLS   r6, #0                      ; else r6 = 0
+        SUBS    ip, ip, #MAX_DIST-250    ; if r6 > MAX_DIST
+        SUBCSS  r6, ip, #250             ; r6 = r6 - MAXDIST
+        MOVLS   r6, #0                   ; else r6 = 0
 
         ADD     r7, r2, #MAX_MATCH-256
-        ADD     r7, r7, #256                ; r7 = r2 + MAX_MATCH (=258);
+        ADD     r7, r7, #256             ; r7 = r2 + MAX_MATCH (=258);
 
         SUB     ip, r5, #1
         LDRB    r8, [r2, ip]
         LDRB    r9, [r2, r5]
 
-        LDR     ip, [pc, #|__good_match|-.-8]
+        LDR     ip, =good_match
         LDR     ip, [ip]
         CMP     r5, ip
         MOVCS   r1, r1, LSR #2
@@ -183,18 +168,18 @@
         CMP     r4, r5                   ; if (len > best_len) {
         BLE     cycle_end
 
-        LDR     ip, [pc, #|__match_start|-.-8]  ; match_start = cur_match;
+        LDR     ip, =match_start         ; match_start = cur_match;
         STR     r0, [ip]
-        MOV     r5, r4                          ; best_len = len;
+        MOV     r5, r4                   ; best_len = len;
 
-        LDR     ip, [pc, #|__nice_match|-.-8]   ; if (len >= nice_match)
+        LDR     ip, =nice_match          ; if (len >= nice_match)
         LDR     ip, [ip]
         CMP     r4, ip
-        BGE     exit_match                      ;   break;
+        BGE     exit_match               ;   break;
 
-        SUB     ip, r5, #1                      ; scan_end1  = scan[best_len-1];
+        SUB     ip, r5, #1               ; scan_end1  = scan[best_len-1];
         LDRB    r8, [r2, ip]
-        LDRB    r9, [r2, r5]                    ; scan_end   = scan[best_len];
+        LDRB    r9, [r2, r5]             ; scan_end   = scan[best_len];
 
 cycle_end
         MOV     ip, r0, LSL #17          ; cur_match & WMASK
@@ -204,14 +189,186 @@
         MOV     r0, r0, ASL #16
         MOV     r0, r0, LSR #16
 
-        CMP     r0, r6                  ; cur_match > limit
+        CMP     r0, r6                   ; cur_match > limit
         BLS     exit_match
-        SUBS    r1, r1, #1              ; --chain_length
-        BNE     cycle                   ; chain_length != 0
+        SUBS    r1, r1, #1               ; --chain_length
+        BNE     cycle                    ; chain_length != 0
 
 exit_match
         MOV     r0, r5
 
-        LDMFD   sp!, {r4-r9,fp,pc}^
+        LDMFD   sp!, {r4-r9,fp,pc}
 
         END
+#elif defined(__ELF__)
+
+@ Defines from zip.h and deflate.c:
+#define MIN_MATCH  3
+#define MAX_MATCH  258
+#define WSIZE  (0x8000)
+#define MIN_LOOKAHEAD (MAX_MATCH+MIN_MATCH+1)
+#define MAX_DIST  (WSIZE-MIN_LOOKAHEAD)
+#define WMASK     (WSIZE-1)
+
+        .text
+
+@ r1 = chain_length
+@ r2 = scan
+@ r3 = match
+@ r4 = len (tmp)
+@ r5 = best_len
+@ r6 = limit
+@ r7 = strend
+@ r8 = scan_end1
+@ r9 = scan_end
+@ lr = window
+@ fp = prev
+
+        .asciz  "longest_match"
+        .align
+        .word   0xff000010
+
+        .global longest_match
+longest_match:
+        STMFD   sp!, {r4-r9,fp,lr}
+
+        LDR     fp, =prev
+
+        LDR     r1, =max_chain_length
+        LDR     r1, [r1]
+        LDR     lr, =window
+
+        LDR     ip, =strstart
+        LDR     ip, [ip]
+        ADD     r2, lr, ip
+        LDR     r5, =prev_length
+        LDR     r5, [r5]
+        SUBS    ip, ip, #MAX_DIST-250    @ if r6 > MAX_DIST
+        SUBCSS  r6, ip, #250             @ r6 = r6 - MAXDIST
+        MOVLS   r6, #0                   @ else r6 = 0
+
+        ADD     r7, r2, #MAX_MATCH-256
+        ADD     r7, r7, #256             @ r7 = r2 + MAX_MATCH (=258);
+
+        SUB     ip, r5, #1
+        LDRB    r8, [r2, ip]
+        LDRB    r9, [r2, r5]
+
+        LDR     ip, =good_match
+        LDR     ip, [ip]
+        CMP     r5, ip
+        MOVCS   r1, r1, LSR #2
+
+cycle:
+        ADD     r3, lr, r0
+
+        LDRB    ip, [r3, r5]
+        CMP     ip, r9
+        BNE     cycle_end
+
+        SUB     ip, r5, #1
+        LDRB    ip, [r3, ip]
+        CMP     ip, r8
+        BNE     cycle_end
+
+        LDRB    ip, [r2]
+        LDRB    r4, [r3]
+        CMP     ip, r4
+        BNE     cycle_end
+
+        LDRB    ip, [r3, #1]
+        LDRB    r4, [r2, #1]
+        CMP     ip, r4
+        BNE     cycle_end
+
+        ADD     r2, r2, #2
+        ADD     r3, r3, #2
+
+inn_cycle:
+        LDRB    ip, [r2, #1]!
+        LDRB    r4, [r3, #1]!
+        CMP     ip, r4
+        BNE     exit_inn_cycle
+
+        LDRB    ip, [r2, #1]!
+        LDRB    r4, [r3, #1]!
+        CMP     ip, r4
+        BNE     exit_inn_cycle
+
+        LDRB    ip, [r2, #1]!
+        LDRB    r4, [r3, #1]!
+        CMP     ip, r4
+        BNE     exit_inn_cycle
+
+        LDRB    ip, [r2, #1]!
+        LDRB    r4, [r3, #1]!
+        CMP     ip, r4
+        BNE     exit_inn_cycle
+
+        LDRB    ip, [r2, #1]!
+        LDRB    r4, [r3, #1]!
+        CMP     ip, r4
+        BNE     exit_inn_cycle
+
+        LDRB    ip, [r2, #1]!
+        LDRB    r4, [r3, #1]!
+        CMP     ip, r4
+        BNE     exit_inn_cycle
+
+        LDRB    ip, [r2, #1]!
+        LDRB    r4, [r3, #1]!
+        CMP     ip, r4
+        BNE     exit_inn_cycle
+
+        LDRB    ip, [r2, #1]!
+        LDRB    r4, [r3, #1]!
+        CMP     ip, r4
+        BNE     exit_inn_cycle
+
+        CMP     r2, r7
+        BCC     inn_cycle
+
+exit_inn_cycle:
+        SUB     r4, r2, r7               @ len = MAX_MATCH - (int)(strend - scan);
+        ADD     r4, r4, #MAX_MATCH-256
+        ADD     r4, r4, #256
+
+        SUB     r2, r2, r4               @ scan = strend - MAX_MATCH
+
+        CMP     r4, r5                   @ if (len > best_len) {
+        BLE     cycle_end
+
+        LDR     ip, =match_start         @ match_start = cur_match;
+        STR     r0, [ip]
+        MOV     r5, r4                   @ best_len = len;
+
+        LDR     ip, =nice_match          @ if (len >= nice_match)
+        LDR     ip, [ip]
+        CMP     r4, ip
+        BGE     exit_match               @   break;
+
+        SUB     ip, r5, #1               @ scan_end1  = scan[best_len-1];
+        LDRB    r8, [r2, ip]
+        LDRB    r9, [r2, r5]             @ scan_end   = scan[best_len];
+
+cycle_end:
+        MOV     ip, r0, LSL #17          @ cur_match & WMASK
+        MOV     ip, ip, LSR #17
+
+        LDR     r0, [fp, ip, ASL #1]     @ cur_match = prev[cur_match & WMASK]
+        MOV     r0, r0, ASL #16
+        MOV     r0, r0, LSR #16
+
+        CMP     r0, r6                   @ cur_match > limit
+        BLS     exit_match
+        SUBS    r1, r1, #1               @ --chain_length
+        BNE     cycle                    @ chain_length != 0
+
+exit_match:
+        MOV     r0, r5
+
+        LDMFD   sp!, {r4-r9,fp,pc}
+
+        .end
+#endif
+
diff -r -u -N ./acorn/ReadMe ../zip30-joty/acorn/ReadMe
--- ./acorn/ReadMe	2009-01-17 20:07:14.000000000 +0100
+++ ../zip30-joty/acorn/ReadMe	2009-01-18 13:51:15.000000000 +0100
@@ -1,15 +1,6 @@
 Acorn-specific compile instructions
 -----------------------------------
 
-Use the "RunMe1st" file (it is an Obey file) to convert all the files from
-"filename/[chs]" to "[chs].filename" (so that zip could be easily compiled
-under RISC OS). It will also set the correct makefile.
-
-To compile just set the CSD to the main zip directory and run 'amu'.
-
-Currently only the Acorn C V5 compiler has been tested but probably also
-Acorn C V4 and the Acorn Assembler V2 will be able to compile zip.
-
 The default makefile is configured without the support for the
 Extended Timestamp Extra Field. If you wan to enable it you have to
 add "-DUSE_EF_UT_TIME" to CFLAGS (see makefile). Without the Extended
@@ -26,6 +17,43 @@
 yourself, you can contact the authors at the Info-ZIP address who will do it
 for you.
 
+Compilation under GCC on RISC OS
+--------------------------------
+
+It is recommended that you use gcc 3.4.6 release 3 or higher and the corresponding
+'make' utility. Both are available from <URL:http://gccsdk.riscos.info/>.
+
+Use the "RunMe1st" file (it is an Obey file) to convert all the files from
+"filename/[chs]" to "[chs].filename" (so that zip could be easily compiled
+under RISC OS). It will also set the correct makefile.
+
+To compile just set the CSD to the main zip directory and run 'make'.
+
+Compilation under Acorn C/C++
+-----------------------------
+
+Currently only the Acorn C V5 compiler has been tested but probably also
+Acorn C V4 and the Acorn Assembler V2 will be able to compile zip.
+
+First check RunMe1st for a few lines that need uncommenting, in order to install
+the Acorn C/C++ makefile as opposed to the GCC one. Then run RunMe1st as normal
+(i.e. set type to 'Obey' and then double-click). The source tree should then be
+renamed and the makefile installed in the right place.
+
+To compile just set the CSD to the main zip directory and run 'amu'.
+
+Cross-compilation under GCCSDK
+------------------------------
+
+Use of RunMe1st is not required when using GCCSDK 3.4 or GCCSDK 4
+(<URL:http://gccsdk.riscos.info/). Instead copy the GCC makefile to the right
+place (e.g. 'cp acorn/GMakefile makefile') and then start the cross compiler:
+
+  $ /home/riscos/env/ro-make E=,ff8
+
+Or when using GCCSDK 4:
+
+  $ /home/riscos/env/ro-make E=,ff8 CC=arm-unknown-riscos-gcc
 
 Acorn-specific usage instructions
 ---------------------------------
@@ -39,7 +67,7 @@
 this second case will also be obtained (without the 'I' option) if SparkFS
 isn't loaded.
 
-When adding files to a zipfile; to maintain FileCore compliance, all
+When adding files to a zipfile; to maintain FileSwitch compliance, all
 files named "file/ext" will be added to the archive as "file.ext".
 This presents no problem if you wish to use unzip to extract them on any
 other machine, as the files are correctly named. This also presents no
@@ -53,7 +81,7 @@
 *zip new/zip newfile
 *zip new/zip NewFile
 *zip new/zip NEWFILE
-will create an archive containing 3 copies of the same Risc OS file 'newfile'
+will create an archive containing 3 copies of the same RISC OS file 'newfile'
 called 'newfile', 'NewFile' and 'NEWFILE'. Please be careful.
 
 The Acorn port conserves file attributes, including filetype, so if you
diff -r -u -N ./acorn/ReadMe.GMakefile ../zip30-joty/acorn/ReadMe.GMakefile
--- ./acorn/ReadMe.GMakefile	2009-01-17 20:07:14.000000000 +0100
+++ ../zip30-joty/acorn/ReadMe.GMakefile	1970-01-01 01:00:00.000000000 +0100
@@ -1,16 +0,0 @@
-GMakefile is for use with Acorn RISC OS and the forthcoming
-post-Acorn RISC OS for the compilation of both the current release and
-development versions of zip.
-
-It is recommended that you use gcc 2.95.4 or higher and you will need a 
-suitable 'make' utility. Both are available from 
-<URL:http://hard-mofo.dsvr.net/gcc/>.
-
-You will need the files gcc.zip and cc1.zip for the C compiler with the
-documentation available in the gccdoc.zip archive. GNU make can be 
-found in the utils.zip archive, although most versions of 'make' should be
-fine.
-
-When using gcc, check RunMe1st for two lines which need uncommenting.
-
-
diff -r -u -N ./acorn/riscos.c ../zip30-joty/acorn/riscos.c
--- ./acorn/riscos.c	2009-01-17 20:07:14.000000000 +0100
+++ ../zip30-joty/acorn/riscos.c	2009-01-18 13:48:29.000000000 +0100
@@ -1,7 +1,7 @@
 /*
-  Copyright (c) 1990-2002 Info-ZIP.  All rights reserved.
+  Copyright (c) 1990-2009 Info-ZIP.  All rights reserved.
 
-  See the accompanying file LICENSE, version 2000-Apr-09 or later
+  See the accompanying file LICENSE, version 2007-Mar-4 or later
   (the contents of which are also included in zip.h) for terms of use.
   If, for some reason, all these files are missing, the Info-ZIP license
   also may be found at:  ftp://ftp.info-zip.org/pub/infozip/license.html
@@ -14,8 +14,6 @@
 #include "zip.h"
 #include "riscos.h"
 
-#define MAXEXT 256
-
 /* External globals */
 extern int scanimage;
 
@@ -94,7 +92,6 @@
  DIR *thisdir;
  int type;
  int attr;
- os_error *er;
 
  thisdir=(DIR *)malloc(sizeof(DIR));
  if (thisdir==NULL)
@@ -110,7 +107,7 @@
  if (thisdir->dirname[strlen(thisdir->dirname)-1]=='.')
    thisdir->dirname[strlen(thisdir->dirname)-1]=0;
 
- if (er=SWI_OS_File_5(thisdir->dirname,&type,NULL,NULL,NULL,&attr),er!=NULL ||
+ if (SWI_OS_File_5(thisdir->dirname,&type,NULL,NULL,NULL,&attr) != NULL ||
      type<=1 || (type==3 && !scanimage))
  {
    free(thisdir->dirname);
@@ -172,13 +169,12 @@
  free(d);
 }
 
-int unlink(f)
-char *f;                /* file to delete */
+int unlink(char *f)
 /* Delete the file *f, returning non-zero on failure. */
 {
- os_error *er;
- char canon[256];
- int size=255;
+ _kernel_oserror *er;
+ char canon[MAXFILENAMELEN];
+ int size=MAXFILENAMELEN-1;
 
  er=SWI_OS_FSControl_37(f,canon,&size);
  if (er==NULL) {
@@ -195,7 +191,6 @@
  int objtype;
  char *s;
  int len;
- os_error *er;
 
  len = strlen(d);
  if ((s = malloc(len + 1)) == NULL)
@@ -205,7 +200,7 @@
  if (s[len-1]=='.')
    s[len-1]=0;
 
- if (er=SWI_OS_File_5(s,&objtype,NULL,NULL,NULL,NULL),er!=NULL) {
+ if (SWI_OS_File_5(s,&objtype,NULL,NULL,NULL,NULL) != NULL) {
    free(s);
    return -1;
  }
@@ -215,16 +210,13 @@
    return -1;
  }
 
- if (er=SWI_OS_File_6(s),er!=NULL) {
+ if (SWI_OS_File_6(s) != NULL) {
    /* maybe this is a problem with the DDEUtils module, try to canonicalise the path */
-   char canon[256];
-   int size=255;
+   char canon[MAXFILENAMELEN];
+   int size=MAXFILENAMELEN-1;
 
-   if (er=SWI_OS_FSControl_37(s,canon,&size),er!=NULL) {
-     free(s);
-     return -1;
-   }
-   if (er=SWI_OS_File_6(canon),er!=NULL) {
+   if (SWI_OS_FSControl_37(s,canon,&size) != NULL
+       || SWI_OS_File_6(canon) != NULL) {
      free(s);
      return -1;
    }
@@ -246,7 +238,7 @@
 
 void setfiletype(char *fname,int ftype)
 {
- char str[256];
+ char str[MAXFILENAMELEN+32];
  sprintf(str,"SetType %s &%3.3X",fname,ftype);
  SWI_OS_CLI(str);
 }
@@ -321,7 +313,7 @@
 
 void set_prefix(void)
 {
- char *pref;
+ char *pref=0;
  int size=0;
 
  if (SWI_OS_FSControl_37("@",pref,&size)!=NULL)
diff -r -u -N ./acorn/riscos.h ../zip30-joty/acorn/riscos.h
--- ./acorn/riscos.h	2009-01-17 20:07:14.000000000 +0100
+++ ../zip30-joty/acorn/riscos.h	2009-01-18 15:01:40.000000000 +0100
@@ -1,7 +1,7 @@
 /*
-  Copyright (c) 1990-2002 Info-ZIP.  All rights reserved.
+  Copyright (c) 1990-2009 Info-ZIP.  All rights reserved.
 
-  See the accompanying file LICENSE, version 2000-Apr-09 or later
+  See the accompanying file LICENSE, version 2007-Mar-4 or later
   (the contents of which are also included in zip.h) for terms of use.
   If, for some reason, all these files are missing, the Info-ZIP license
   also may be found at:  ftp://ftp.info-zip.org/pub/infozip/license.html
@@ -11,21 +11,14 @@
 #ifndef __riscos_h
 #define __riscos_h
 
+#include <limits.h>
 #include <time.h>
 #include <stdio.h>
 
-typedef struct {
-  int errnum;
-  char errmess[252];
-} os_error;
-
-#ifndef __swiven_h
-#  include "swiven.h"
-#endif
+#include "swiven.h"
 
-#define MAXPATHLEN 256
-#define MAXFILENAMELEN 64  /* should be 11 for ADFS, 13 for DOS, 64 seems a sensible value... */
-#define DIR_BUFSIZE 1024   /* this should be enough to read a whole E-Format directory */
+#define MAXFILENAMELEN 1024  /* RISC OS 4 has 1024 limit. 1024 is also the same as FNMAX in zip.h */
+#define DIR_BUFSIZE MAXFILENAMELEN   /* Ensure we can read at least one full-length RISC OS 4 filename */
 
 struct stat {
   unsigned int st_dev;
@@ -61,15 +54,6 @@
   char d_name[MAXFILENAMELEN]; /* name */
 };
 
-typedef struct {
-  unsigned int load_addr;
-  unsigned int exec_addr;
-  int lenght;
-  int attrib;
-  int objtype;
-  char name[13];
-} riscos_direntry;
-
 #define SPARKID   0x4341        /* = "AC" */
 #define SPARKID_2 0x30435241    /* = "ARC0" */
 
diff -r -u -N ./acorn/RunMe1st ../zip30-joty/acorn/RunMe1st
--- ./acorn/RunMe1st	2009-01-17 20:07:14.000000000 +0100
+++ ../zip30-joty/acorn/RunMe1st	2009-01-18 13:48:29.000000000 +0100
@@ -1,23 +1,18 @@
-| This Obey file prepares the zip port for a Desktop C re-compile.
+| This Obey file prepares the zip port for a GCC/Desktop C re-compile.
 | Run it and it will copy all the needed files into the correct
 | place.
 
-| Set the correct type of 'srcrename' so that the only requirement
-| for the user is to set 'RunMe1st' to Obey
-SetType <Obey$Dir>.srcrename FF8
-
 | Run 'srcrename' on the main zip directory with recursion enabled
-/<Obey$Dir>.srcrename -r -e c:h:s:o <Obey$Dir>.^
-
-| Create the 'o' directory
-CDir <Obey$Dir>.^.o
+BASIC -quit "<Obey$Dir>.srcrename" -r -e c:h:s:o <Obey$Dir>.^
 
 | Put the Makefile in its correct place and set the correct filetype
-Copy <Obey$Dir>.makefile <Obey$Dir>.^.makefile ~C ~V F
+Copy <Obey$Dir>.GMakefile <Obey$Dir>.^.makefile ~C~VF
 
-| Uncomment the following lines if you're using gcc
+| Uncomment the following lines if you're using Acorn C/C++
 || Put the Makefile in its correct place and set the correct filetype
-|Copy <Obey$Dir>.GMakefile <Obey$Dir>.^.makefile ~C~VF
+|Copy <Obey$Dir>.makefile <Obey$Dir>.^.makefile ~C ~V F
+|| Create the 'o' directory
+|CDir <Obey$Dir>.^.o
 
 SetType <Obey$Dir>.^.makefile FE1
 SetType <Obey$Dir>.zipsfx Obey
diff -r -u -N ./acorn/sendbits.s ../zip30-joty/acorn/sendbits.s
--- ./acorn/sendbits.s	2009-01-17 20:07:14.000000000 +0100
+++ ../zip30-joty/acorn/sendbits.s	2009-01-18 13:48:29.000000000 +0100
@@ -1,3 +1,4 @@
+#if defined(__aof__)
 ;===========================================================================
 ; Copyright (c) 1990-1999 Info-ZIP.  All rights reserved.
 ;
@@ -27,19 +28,17 @@
 
         AREA    |Asm$$Code|, CODE, READONLY
 
-        =       "send_bits",0
-        ALIGN
-        &       &FF00000C
-
-        IMPORT  __rt_stkovf_split_small
         IMPORT  flush_outbuf
-
         IMPORT  bi_valid
         IMPORT  bi_buf
         IMPORT  out_size
         IMPORT  out_offset
         IMPORT  out_buf
 
+        =       "send_bits",0
+        ALIGN
+        &       &FF00000C
+
         EXPORT  send_bits
 send_bits
         MOV     ip,sp
@@ -54,7 +53,7 @@
         CMP     r2,#&10
         STRLE   r2,[r3]         ; short? store & return
         STRLE   r4,[r5]
-        LDMLEDB fp,{r4,r5,fp,sp,pc}^
+        LDMLEDB fp,{r4,r5,fp,sp,pc}
 
         SUB     r2,r2,#&10      ; adjust bi_valid, bi_buf
         MOV     ip,r4,LSR #16   ;  (done early, keeping the old bi_buf
@@ -80,12 +79,7 @@
         ADD     r2,r2,#2
         STR     r2,[r1]
 
-        LDMDB   fp,{r4,r5,fp,sp,pc}^
-
-
-ptr_bi          &       bi_valid
-                &       bi_buf
-
+        LDMDB   fp,{r4,r5,fp,sp,pc}
 
         =       "bi_reverse",0
         ALIGN
@@ -99,7 +93,72 @@
         SUBS    r1,r1,#1
         BNE     loop
         MOV     r0,r2
-        MOVS    pc,lr
-
+        MOV     pc,lr
 
         END
+#elif defined(__ELF__)
+        .text
+
+        .asciz  "send_bits"
+        .align
+        .word   0xFF00000C
+
+        .global send_bits
+send_bits:
+        MOV     ip,sp
+        STMDB   sp!,{r4,r5,fp,ip,lr,pc}
+        SUB     fp,ip,#4
+        LDR     r5,=bi_buf
+        LDR     r3,=bi_valid
+        LDR     r4,[r5]
+        LDR     r2,[r3]
+        ORR     r4,r4,r0,LSL r2 @ |= value<<bi_valid
+        ADD     r2,r2,r1        @ += length
+        CMP     r2,#0x10
+        STRLE   r2,[r3]         @ short? store & return
+        STRLE   r4,[r5]
+        LDMLEDB fp,{r4,r5,fp,sp,pc}
+
+        SUB     r2,r2,#0x10     @ adjust bi_valid, bi_buf
+        MOV     ip,r4,LSR #16   @  (done early, keeping the old bi_buf
+        STR     r2,[r3]         @  in R4 for later storage)
+        STR     ip,[r5]
+
+        LDR     r0,=out_size
+        LDR     r1,=out_offset
+        LDR     r0,[r0]
+        LDR     r2,[r1]
+        SUB     r0,r0,#1
+        CMP     r2,r0           @ if out_offset >= out_size-1
+        LDRHS   r0,=out_buf
+        LDRHS   r0,[r0]
+        BLHS    flush_outbuf    @ then flush the buffer
+        LDR     r0,=out_buf
+        LDR     r1,=out_offset
+        LDR     r0,[r0]
+        LDR     r2,[r1]
+        MOV     r5,r4,LSR #8
+        STRB    r4,[r0,r2]!     @ store 'old' bi_buf
+        STRB    r5,[r0,#1]
+        ADD     r2,r2,#2
+        STR     r2,[r1]
+
+        LDMDB   fp,{r4,r5,fp,sp,pc}
+
+        .asciz  "bi_reverse"
+        .align
+        .word   0xFF00000C
+
+        .global bi_reverse
+bi_reverse:
+        MOV     r2,#0
+loop:
+        MOVS    r0,r0,LSR #1
+        ADCS    r2,r2,r2
+        SUBS    r1,r1,#1
+        BNE     loop
+        MOV     r0,r2
+        MOV     pc,lr
+
+        .end
+#endif
diff -r -u -N ./acorn/swiven.c ../zip30-joty/acorn/swiven.c
--- ./acorn/swiven.c	1970-01-01 01:00:00.000000000 +0100
+++ ../zip30-joty/acorn/swiven.c	2009-01-18 13:48:29.000000000 +0100
@@ -0,0 +1,127 @@
+/*
+  Copyright (c) 1990-2009 Info-ZIP.  All rights reserved.
+
+  See the accompanying file LICENSE, version 2007-Mar-4 or later
+  (the contents of which are also included in zip.h) for terms of use.
+  If, for some reason, all these files are missing, the Info-ZIP license
+  also may be found at:  ftp://ftp.info-zip.org/pub/infozip/license.html
+*/
+
+#include <stdlib.h>
+#include "kernel.h"
+#include "swis.h"
+
+#include "riscos.h"
+
+/* This is not defined in Acorn C swis.h header: */
+#ifndef DDEUtils_Prefix
+#define DDEUtils_Prefix 0x042580
+#endif
+
+_kernel_oserror *SWI_OS_FSControl_26(const char *source, const char *dest,
+				     int actionmask)
+{ /* copy */
+  return _swix(OS_FSControl,_INR(0,3),26,source,dest,actionmask);
+}
+
+_kernel_oserror *SWI_OS_FSControl_27(const char *filename, int actionmask)
+{ /* wipe */
+  return _swix(OS_FSControl,_INR(0,1)|_IN(3),27,filename,actionmask);
+}
+
+_kernel_oserror *SWI_OS_GBPB_9(const char *dirname, void *buf, int *number,
+			       int *offset, int size, const char *match)
+{ /* read dir */
+  return  _swix(OS_GBPB,_INR(0,6)|_OUTR(3,4),9,dirname,buf,
+		*number,*offset,size,match,number,offset);
+}
+
+_kernel_oserror *SWI_OS_File_1(const char *filename, unsigned int loadaddr,
+			       unsigned int execaddr, int attrib)
+{ /* write file attributes */
+  return _swix(OS_File,_INR(0,3)|_IN(5),1,filename,loadaddr,execaddr,attrib);
+}
+
+_kernel_oserror *SWI_OS_File_5(const char *filename, int *objtype,
+			       unsigned int *loadaddr,
+			       unsigned int *execaddr,
+			       int *length, int *attrib)
+{ /* read file info */
+  int _objtype,_length,_attrib;
+  unsigned int _loadaddr,_execaddr;
+  _kernel_oserror *err = _swix(OS_File,_INR(0,1)|_OUT(0)|_OUTR(2,5),5,
+			       filename,
+			       &_objtype,&_loadaddr,&_execaddr,&_length,
+			       &_attrib);
+  if (err)
+    return err;
+  if (objtype)
+    *objtype = _objtype;
+  if (loadaddr)
+    *loadaddr = _loadaddr;
+  if (execaddr)
+    *execaddr = _execaddr;
+  if (length)
+    *length = _length;
+  if (attrib)
+    *attrib = _attrib;
+  return NULL;
+}
+
+_kernel_oserror *SWI_OS_File_6(const char *filename)
+{ /* delete */
+  return _swix(OS_File,_INR(0,1),6,filename);
+}
+
+_kernel_oserror *SWI_OS_File_7(const char *filename, int loadaddr,
+			       int execaddr, int size)
+{ /* create an empty file */
+  return _swix(OS_File,_INR(0,5),7,filename,loadaddr,execaddr,0,size);
+}
+
+_kernel_oserror *SWI_OS_CLI(const char *cmd)
+{ /* execute a command */
+  return _swix(OS_CLI,_IN(0),cmd);
+}
+
+int SWI_OS_ReadC(void)
+{ /* get a key from the keyboard buffer */
+  int key;
+  _swix(OS_ReadC,_OUT(0),&key);
+  return key;
+}
+
+_kernel_oserror *SWI_OS_ReadVarVal(const char *var, char *buf, int len,
+				   int *bytesused)
+{ /* reads an OS varibale */
+  int _bytesused;
+  _kernel_oserror *err = _swix(OS_ReadVarVal,_INR(0,4)|_OUT(2),var,buf,len,
+			       0,0,&_bytesused);
+  if (err)
+    return err;
+  if (bytesused)
+    *bytesused = _bytesused;
+  return NULL;
+}
+
+_kernel_oserror *SWI_OS_FSControl_37(const char *pathname, char *buffer,
+				     int *size)
+{ /* canonicalise path */
+  return _swix(OS_FSControl,_INR(0,5)|_OUT(5),37,pathname,buffer,0,0,*size,
+	       size);
+}
+
+
+_kernel_oserror *SWI_DDEUtils_Prefix(const char *dir)
+{ /* sets the 'prefix' directory */
+  return _swix(DDEUtils_Prefix,_IN(0),dir);
+}
+
+int SWI_Read_Timezone(void)
+{ /* returns the timezone offset (centiseconds) */
+  int ofs;
+  _kernel_oserror *err = _swix(Territory_ReadCurrentTimeZone,_OUT(0),&ofs);
+  if (err)
+    return 0;
+  return ofs;
+}
diff -r -u -N ./acorn/swiven.h ../zip30-joty/acorn/swiven.h
--- ./acorn/swiven.h	2009-01-17 20:07:14.000000000 +0100
+++ ../zip30-joty/acorn/swiven.h	2009-01-18 13:48:29.000000000 +0100
@@ -1,5 +1,5 @@
 /*
-  Copyright (c) 1990-1999 Info-ZIP.  All rights reserved.
+  Copyright (c) 1990-2009 Info-ZIP.  All rights reserved.
 
   See the accompanying file LICENSE, version 1999-Oct-05 or later
   (the contents of which are also included in zip.h) for terms of use.
@@ -11,46 +11,51 @@
 #ifndef __swiven_h
 #define __swiven_h
 
-os_error *SWI_OS_FSControl_26(char *source, char *dest, int actionmask);
+#include "kernel.h"
+
+_kernel_oserror *SWI_OS_FSControl_26(const char *source, const char *dest,
+				     int actionmask);
 /* copy */
 
-os_error *SWI_OS_FSControl_27(char *filename, int actionmask);
+_kernel_oserror *SWI_OS_FSControl_27(const char *filename, int actionmask);
 /* wipe */
 
-os_error *SWI_OS_GBPB_9(char *dirname, void *buf, int *number,
-                        int *offset, int size, char *match);
+_kernel_oserror *SWI_OS_GBPB_9(const char *dirname, void *buf, int *number,
+			       int *offset, int size, const char *match);
 /* read dir */
 
-os_error *SWI_OS_File_1(char *filename, unsigned int loadaddr,
-                        unsigned int execaddr, int attrib);
+_kernel_oserror *SWI_OS_File_1(const char *filename, unsigned int loadaddr,
+			       unsigned int execaddr, int attrib);
 /* write file attributes */
 
-os_error *SWI_OS_File_5(char *filename, int *objtype, unsigned int *loadaddr,
-                        unsigned int *execaddr, int *length, int *attrib);
+_kernel_oserror *SWI_OS_File_5(const char *filename, int *objtype,
+			       unsigned int *loadaddr,
+			       unsigned int *execaddr,
+			       int *length, int *attrib);
 /* read file info */
 
-os_error *SWI_OS_File_6(char *filename);
+_kernel_oserror *SWI_OS_File_6(const char *filename);
 /* delete */
 
-os_error *SWI_OS_File_7(char *filename, int loadaddr, int execaddr, int size);
+_kernel_oserror *SWI_OS_File_7(const char *filename, int loadaddr,
+			       int execaddr, int size);
 /* create an empty file */
 
-os_error *SWI_OS_CLI(char *cmd);
+_kernel_oserror *SWI_OS_CLI(const char *cmd);
 /* execute a command */
 
 int SWI_OS_ReadC(void);
 /* get a key from the keyboard buffer */
 
-os_error *SWI_OS_ReadVarVal(char *var, char *buf, int len, int *bytesused);
+_kernel_oserror *SWI_OS_ReadVarVal(const char *var, char *buf, int len,
+				   int *bytesused);
 /* reads an OS varibale */
 
-os_error *SWI_OS_FSControl_54(char *buffer, int dir, char *fsname, int *size);
-/* reads the path of a specified directory */
-
-os_error *SWI_OS_FSControl_37(char *pathname, char *buffer, int *size);
+_kernel_oserror *SWI_OS_FSControl_37(const char *pathname, char *buffer,
+				     int *size);
 /* canonicalise path */
 
-os_error *SWI_DDEUtils_Prefix(char *dir);
+_kernel_oserror *SWI_DDEUtils_Prefix(const char *dir);
 /* sets the 'prefix' directory */
 
 int SWI_Read_Timezone(void);
diff -r -u -N ./fileio.c ../zip30-joty/fileio.c
--- ./fileio.c	2009-01-17 20:07:14.000000000 +0100
+++ ../zip30-joty/fileio.c	2009-01-18 15:31:54.000000000 +0100
@@ -3724,7 +3724,9 @@
 static ZCONST char Far long_op_ambig_err[] = "long option '%s' ambiguous";
 static ZCONST char Far long_op_not_sup_err[] = "long option '%s' not supported";
 
+#if 0
 static ZCONST char Far no_arg_files_err[] = "argument files not enabled\n";
+#endif
 
 
 /* below removed as only used for processing argument files */
diff -r -u -N ./zipfile.c ../zip30-joty/zipfile.c
--- ./zipfile.c	2009-01-17 20:07:14.000000000 +0100
+++ ../zip30-joty/zipfile.c	2009-01-18 15:33:01.000000000 +0100
@@ -444,7 +444,9 @@
       strcat(t, "_zip");
   }
 #    endif /* QDOS */
-#  endif /* !RISCOS */
+#  else /* !RISCOS */
+  q = q;
+#  endif /* ?RISCOS */
   return t;
 }
 #endif  /* ndef VMS */
