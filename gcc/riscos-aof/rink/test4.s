; ***************** test4


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

  IMPORT printf

fn0
  STMFD R13!,{R14}

 ADR R0,string
 BL printf

  LDMFD R13!,{PC}

string
 DCB "fn0 called",10,13,0

  EXPORT fn0

  END

