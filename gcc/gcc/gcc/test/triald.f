	PROGRAM	TRIALD3
C TRIALD3.FOR    Version for SEL
C
C        This code contains a mix of instructions and subroutine/function
C         calls as found in our typical realtime simulation applications.
C        The actual coding is fairly meaningless although does give a crude
C         indication of round off errors.
C        A section of code is repeated 64 times with some variation of
C         variable names. This is to generate a program comparable in size
C         to a small simulation and to check the effects of cache.
C
C        A random number function generator is used, as a typical function
C         call and to give random accesses to an array. The version here reliCes
C         on 16 bit arithmetic and suppression of machine overflow checks
C        -- will your system accept this code ? If not I am sure you can
C        provide a similar routine which provides a pesudo real random
C        number in the range 0.0 to 1.0 (the argument is an integer seed).
C        ... see RANDOM at end of main program
C
C        IO Units 5,6 are used for interactively setting number of passes
C         and giving end message.
C        Timing is simply done by stop watch !
C
C        Results on a VAX11/780 (with Floating Point accelerator, 8KB cache):
C
C        Compile time    = approx. 38 secs
C        Link time       = approx. 4 secs
C        Run time        = 36 (+/- 1/2) secs  for two thousand passes.
C
C        Printout:       XX = 2561.88, XY = 0.0, TCNT = 128000.
C
C        If any queries contact:
C        John. V. Osborne
C        Design Simulation Lab.
C        British Aerospace
C        Aircraft Group, Hatfield, Herts         Hatfield 62345 ext 3210

      IMPLICIT DOUBLE PRECISION (A-H,O-Z)

      INTEGER   IR
      INTEGER  T1, T2
      REAL     DELTAT

          
      COMMON XK10,XK12,XK13,XK14,XK15,XK16,XK17,XK18,XK19
      COMMON XK20,XK22,XK23,XK24,XK25,XK26,XK27,XK28,XK29
      COMMON XK30,XK32,XK33,XK34,XK35,XK36,XK37,XK38,XK39
      COMMON XK40,XK42,XK43,XK44,XK45,XK46,XK47,XK48,XK49
      COMMON XK50,XK52,XK53,XK54,XK55,XK56,XK57,XK58,XK59
      COMMON XK60,XK62,XK63,XK64,XK65,XK66,XK67,XK68,XK69
      COMMON XK70,XK72,XK73,XK74,XK75,XK76,XK77,XK78,XK79
      COMMON XK80,XK82,XK83,XK84,XK85,XK86,XK87,XK88,XK89
      COMMON XK90,XK92,XK93,XK94,XK95,XK96,XK97,XK98,XK99
      COMMON XRY(3000)

C        Initial action: set critical variables.

      IX=1
      XX=0.
      XY=0.
      XKA= 0.
      XK1= 1.
      XK10= 0.1
      XK15= 0.5
      XK23= 3.3
      XK35= 100.0
      XK43= 2000.02
      XK47= 0.01
      XK53= 2000.
      XK57= 1.1
      XK61= 0.4
      XK68= 800.008
      XK76= 100.0
      XK85=1.
      XK92= 10.1
      XK98= 70.
      XK99= 30.
      CK2= .02
      IR= 12345
      DO 10  IC = 1, 3000
         XRY(IC) = 0.0
10    CONTINUE

      WRITE(6,103)
103   FORMAT(20H+INPUT NO. PASSES:I8)
      READ(5,100) IN  
100   FORMAT(I8)
      T1 = CLOCK()

      DO 1000 IC=1,IN

      XX= XX + CK2
      YABEF= DMOD(XX,XK1)*XK76
      IF(YABEF.GT. XK98) YABEF= XK98
      IF(YABEF.LT.XK99) YABEF= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WABEF= SQRT( XRY(IX) + XRY(IX+1) )
      ZABEF= (COS(YABEF*XK57 + XK15) + YABEF)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WABEF + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YABEF)
C      PRINT *,'XK61    := ',XK61
C      PRINT *,'XK43    := ',XK43
C      PRINT *,'WX*XK61 := ',WX*XK61
C      PRINT *,'XK68    := ',XK68
      XX= XX + CK2
      YEFAB= DMOD(XX,XK1)*XK76
      IF(YEFAB.GT.XK98) YEFAB= XK98
      IF(YEFAB.LT.XK99) YEFAB= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WEFAB= SQRT( XRY(IX) + XRY(IX+1) )
      ZEFAB= (COS(YEFAB*XK57 + XK15) + YEFAB)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WEFAB + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YEFAB)
      XX= XX + CK2
      YABFE= DMOD(XX,XK1)*XK76
      IF(YABFE.GT.XK98) YABFE= XK98
      IF(YABFE.LT.XK99) YABFE= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WABFE= SQRT( XRY(IX) + XRY(IX+1) )
      ZABFE= (COS(YABFE*XK57 + XK15) + YABFE)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WABFE + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YABFE)
      XX= XX + CK2
      YFEAB= DMOD(XX,XK1)*XK76
      IF(YFEAB.GT.XK98) YFEAB= XK98
      IF(YFEAB.LT.XK99) YFEAB= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WFEAB= SQRT( XRY(IX) + XRY(IX+1) )
      ZFEAB= (COS(YFEAB*XK57 + XK15) + YFEAB)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WFEAB + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YFEAB)
      XX= XX + CK2
      YBAEF= DMOD(XX,XK1)*XK76
      IF(YBAFE.GT.XK98) YBAFE= XK98
      IF(YBAFE.LT.XK99) YBAFE= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WBAEF= SQRT( XRY(IX) + XRY(IX+1) )
      ZBAEF= (COS(YBAEF*XK57 + XK15) + YBAEF)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WBAEF + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YBAEF)
      XX= XX + CK2
      YEFBA= DMOD(XX,XK1)*XK76
      IF(YEFBA.GT.XK98) YEFBA= XK98
      IF(YEFBA.LT.XK99) YEFBA= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WEFBA= SQRT( XRY(IX) + XRY(IX+1) )
      ZEFBA= (COS(YEFBA*XK57 + XK15) + YEFBA)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WEFBA + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YEFBA)
      XX= XX + CK2
      YBAFE= DMOD(XX,XK1)*XK76
      IF(YBAFE.GT.XK98) YBAFE= XK98
      IF(YBAFE.LT.XK99) YBAFE= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WBAFE= SQRT( XRY(IX) + XRY(IX+1) )
      ZBAFE= (COS(YBAFE*XK57 + XK15) + YBAFE)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WBAFE + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YBAFE)
      XX= XX + CK2
      YFEBA= DMOD(XX,XK1)*XK76
      IF(YFEBA.GT.XK98) YFEBA= XK98
      IF(YFEBA.LT.XK99) YFEBA= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WFEBA= SQRT( XRY(IX) + XRY(IX+1) )
      ZFEBA= (COS(YFEBA*XK57 + XK15) + YFEBA)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WFEBA + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YFEBA)
      XX= XX + CK2
      YPBEF= DMOD(XX,XK1)*XK76
      IF(YPBEF.GT.XK98) YPBEF= XK98
      IF(YPBEF.LT.XK99) YPBEF= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WPBEF= SQRT( XRY(IX) + XRY(IX+1) )
      ZPBEF= (COS(YPBEF*XK57 + XK15) + YPBEF)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WPBEF + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YPBEF)
      XX= XX + CK2
      YEFPB= DMOD(XX,XK1)*XK76
      IF(YEFPB.GT.XK98) YEFPB= XK98
      IF(YEFPB.LT.XK99) YEFPB= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WEFPB= SQRT( XRY(IX) + XRY(IX+1) )
      ZEFPB= (COS(YEFPB*XK57 + XK15) + YEFPB)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WEFPB + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YEFPB)
      XX= XX + CK2
      YPBFE= DMOD(XX,XK1)*XK76
      IF(YPBFE.GT.XK98) YPBFE= XK98
      IF(YPBFE.LT.XK99) YPBFE= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WPBFE= SQRT( XRY(IX) + XRY(IX+1) )
      ZPBFE= (COS(YPBFE*XK57 + XK15) + YPBFE)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WPBFE + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YPBFE)
      XX= XX + CK2
      YFEPB= DMOD(XX,XK1)*XK76
      IF(YFEPB.GT.XK98) YFEPB= XK98
      IF(YFEPB.LT.XK99) YFEPB= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WFEPB= SQRT( XRY(IX) + XRY(IX+1) )
      ZFEPB= (COS(YFEPB*XK57 + XK15) + YFEPB)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WFEPB + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YFEPB)
      XX= XX + CK2
      YBPEF= DMOD(XX,XK1)*XK76
      IF(YBPEF.GT.XK98) YBPEF= XK98
      IF(YBPEF.LT.XK99) YBPEF= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WBPEF= SQRT( XRY(IX) + XRY(IX+1) )
      ZBPEF= (COS(YBPEF*XK57 + XK15) + YBPEF)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WBPEF + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YBPEF)
      XX= XX + CK2
      YEFBP= DMOD(XX,XK1)*XK76
      IF(YEFBP.GT.XK98) YEFBP= XK98
      IF(YEFBP.LT.XK99) YEFBP= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WEFBP= SQRT( XRY(IX) + XRY(IX+1) )
      ZEFBP= (COS(YEFBP*XK57 + XK15) + YEFBP)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WEFBP + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YEFBP)
      XX= XX + CK2
      YBPFE= DMOD(XX,XK1)*XK76
      IF(YBPFE.GT.XK98) YBPFE= XK98
      IF(YBPFE.LT.XK99) YBPFE= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WBPFE= SQRT( XRY(IX) + XRY(IX+1) )
      ZBPFE= (COS(YBPFE*XK57 + XK15) + YBPFE)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WBPFE + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YBPFE)
      XX= XX + CK2
      YFEBP= DMOD(XX,XK1)*XK76
      IF(YFEBP.GT.XK98) YFEBP= XK98
      IF(YFEBP.LT.XK99) YFEBP= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WFEBP= SQRT( XRY(IX) + XRY(IX+1) )
      ZFEBP= (COS(YFEBP*XK57 + XK15) + YFEBP)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WFEBP + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YFEBP)
      XX= XX + CK2
      YAQEF= DMOD(XX,XK1)*XK76
      IF(YAQEF.GT. XK98) YAQEF= XK98
      IF(YAQEF.LT.XK99) YAQEF= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WAQEF= SQRT( XRY(IX) + XRY(IX+1) )
      ZAQEF= (COS(YAQEF*XK57 + XK15) + YAQEF)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WAQEF + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YAQEF)
      XX= XX + CK2
      YEFAQ= DMOD(XX,XK1)*XK76
      IF(YEFAQ.GT.XK98) YEFAQ= XK98
      IF(YEFAQ.LT.XK99) YEFAQ= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WEFAQ= SQRT( XRY(IX) + XRY(IX+1) )
      ZEFAQ= (COS(YEFAQ*XK57 + XK15) + YEFAQ)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WEFAQ + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YEFAQ)
      XX= XX + CK2
      YAQFE= DMOD(XX,XK1)*XK76
      IF(YAQFE.GT.XK98) YAQFE= XK98
      IF(YAQFE.LT.XK99) YAQFE= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WAQFE= SQRT( XRY(IX) + XRY(IX+1) )
      ZAQFE= (COS(YAQFE*XK57 + XK15) + YAQFE)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WAQFE + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YAQFE)
      XX= XX + CK2
      YFEAQ= DMOD(XX,XK1)*XK76
      IF(YFEAQ.GT.XK98) YFEAQ= XK98
      IF(YFEAQ.LT.XK99) YFEAQ= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WFEAQ= SQRT( XRY(IX) + XRY(IX+1) )
      ZFEAQ= (COS(YFEAQ*XK57 + XK15) + YFEAQ)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WFEAQ + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YFEAQ)
      XX= XX + CK2
      YQAEF= DMOD(XX,XK1)*XK76
      IF(YQAFE.GT.XK98) YQAFE= XK98
      IF(YQAFE.LT.XK99) YQAFE= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WQAEF= SQRT( XRY(IX) + XRY(IX+1) )
      ZQAEF= (COS(YQAEF*XK57 + XK15) + YQAEF)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WQAEF + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YQAEF)
      XX= XX + CK2
      YEFQA= DMOD(XX,XK1)*XK76
      IF(YEFQA.GT.XK98) YEFQA= XK98
      IF(YEFQA.LT.XK99) YEFQA= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WEFQA= SQRT( XRY(IX) + XRY(IX+1) )
      ZEFQA= (COS(YEFQA*XK57 + XK15) + YEFQA)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WEFQA + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YEFQA)
      XX= XX + CK2
      YQAFE= DMOD(XX,XK1)*XK76
      IF(YQAFE.GT.XK98) YQAFE= XK98
      IF(YQAFE.LT.XK99) YQAFE= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WQAFE= SQRT( XRY(IX) + XRY(IX+1) )
      ZQAFE= (COS(YQAFE*XK57 + XK15) + YQAFE)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WQAFE + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YQAFE)
      XX= XX + CK2
      YFEQA= DMOD(XX,XK1)*XK76
      IF(YFEQA.GT.XK98) YFEQA= XK98
      IF(YFEQA.LT.XK99) YFEQA= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WFEQA= SQRT( XRY(IX) + XRY(IX+1) )
      ZFEQA= (COS(YFEQA*XK57 + XK15) + YFEQA)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WFEQA + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YFEQA)
      XX= XX + CK2
      YPQEF= DMOD(XX,XK1)*XK76
      IF(YPQEF.GT.XK98) YPQEF= XK98
      IF(YPQEF.LT.XK99) YPQEF= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WPQEF= SQRT( XRY(IX) + XRY(IX+1) )
      ZPQEF= (COS(YPQEF*XK57 + XK15) + YPQEF)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WPQEF + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YPQEF)
      XX= XX + CK2
      YEFPQ= DMOD(XX,XK1)*XK76
      IF(YEFPQ.GT.XK98) YEFPQ= XK98
      IF(YEFPQ.LT.XK99) YEFPQ= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WEFPQ= SQRT( XRY(IX) + XRY(IX+1) )
      ZEFPQ= (COS(YEFPQ*XK57 + XK15) + YEFPQ)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WEFPQ + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YEFPQ)
      XX= XX + CK2
      YPQFE= DMOD(XX,XK1)*XK76
      IF(YPQFE.GT.XK98) YPQFE= XK98
      IF(YPQFE.LT.XK99) YPQFE= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WPQFE= SQRT( XRY(IX) + XRY(IX+1) )
      ZPQFE= (COS(YPQFE*XK57 + XK15) + YPQFE)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WPQFE + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YPQFE)
      XX= XX + CK2
      YFEPQ= DMOD(XX,XK1)*XK76
      IF(YFEPQ.GT.XK98) YFEPQ= XK98
      IF(YFEPQ.LT.XK99) YFEPQ= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WFEPQ= SQRT( XRY(IX) + XRY(IX+1) )
      ZFEPQ= (COS(YFEPQ*XK57 + XK15) + YFEPQ)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WFEPQ + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YFEPQ)
      XX= XX + CK2
      YQPEF= DMOD(XX,XK1)*XK76
      IF(YQPEF.GT.XK98) YQPEF= XK98
      IF(YQPEF.LT.XK99) YQPEF= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WQPEF= SQRT( XRY(IX) + XRY(IX+1) )
      ZQPEF= (COS(YQPEF*XK57 + XK15) + YQPEF)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WQPEF + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YQPEF)
      XX= XX + CK2
      YEFQP= DMOD(XX,XK1)*XK76
      IF(YEFQP.GT.XK98) YEFQP= XK98
      IF(YEFQP.LT.XK99) YEFQP= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WEFQP= SQRT( XRY(IX) + XRY(IX+1) )
      ZEFQP= (COS(YEFQP*XK57 + XK15) + YEFQP)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WEFQP + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YEFQP)
      XX= XX + CK2
      YQPFE= DMOD(XX,XK1)*XK76
      IF(YQPFE.GT.XK98) YQPFE= XK98
      IF(YQPFE.LT.XK99) YQPFE= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WQPFE= SQRT( XRY(IX) + XRY(IX+1) )
      ZQPFE= (COS(YQPFE*XK57 + XK15) + YQPFE)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WQPFE + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YQPFE)
      XX= XX + CK2
      YFEQP= DMOD(XX,XK1)*XK76
      IF(YFEQP.GT.XK98) YFEQP= XK98
      IF(YFEQP.LT.XK99) YFEQP= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WFEQP= SQRT( XRY(IX) + XRY(IX+1) )
      ZFEQP= (COS(YFEQP*XK57 + XK15) + YFEQP)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WFEQP + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YFEQP)
      XX= XX + CK2
      YABRF= DMOD(XX,XK1)*XK76
      IF(YABRF.GT. XK98) YABRF= XK98
      IF(YABRF.LT.XK99) YABRF= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WABRF= SQRT( XRY(IX) + XRY(IX+1) )
      ZABRF= (COS(YABRF*XK57 + XK15) + YABRF)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WABRF + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YABRF)
      XX= XX + CK2
      YRFAB= DMOD(XX,XK1)*XK76
      IF(YRFAB.GT.XK98) YRFAB= XK98
      IF(YRFAB.LT.XK99) YRFAB= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WRFAB= SQRT( XRY(IX) + XRY(IX+1) )
      ZRFAB= (COS(YRFAB*XK57 + XK15) + YRFAB)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WRFAB + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YRFAB)
      XX= XX + CK2
      YABFR= DMOD(XX,XK1)*XK76
      IF(YABFR.GT.XK98) YABFR= XK98
      IF(YABFR.LT.XK99) YABFR= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WABFR= SQRT( XRY(IX) + XRY(IX+1) )
      ZABFR= (COS(YABFR*XK57 + XK15) + YABFR)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WABFR + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YABFR)
      XX= XX + CK2
      YFRAB= DMOD(XX,XK1)*XK76
      IF(YFRAB.GT.XK98) YFRAB= XK98
      IF(YFRAB.LT.XK99) YFRAB= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WFRAB= SQRT( XRY(IX) + XRY(IX+1) )
      ZFRAB= (COS(YFRAB*XK57 + XK15) + YFRAB)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WFRAB + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YFRAB)
      XX= XX + CK2
      YBARF= DMOD(XX,XK1)*XK76
      IF(YBAFR.GT.XK98) YBAFR= XK98
      IF(YBAFR.LT.XK99) YBAFR= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WBARF= SQRT( XRY(IX) + XRY(IX+1) )
      ZBARF= (COS(YBARF*XK57 + XK15) + YBARF)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WBARF + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YBARF)
      XX= XX + CK2
      YRFBA= DMOD(XX,XK1)*XK76
      IF(YRFBA.GT.XK98) YRFBA= XK98
      IF(YRFBA.LT.XK99) YRFBA= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WRFBA= SQRT( XRY(IX) + XRY(IX+1) )
      ZRFBA= (COS(YRFBA*XK57 + XK15) + YRFBA)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WRFBA + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YRFBA)
      XX= XX + CK2
      YBAFR= DMOD(XX,XK1)*XK76
      IF(YBAFR.GT.XK98) YBAFR= XK98
      IF(YBAFR.LT.XK99) YBAFR= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WBAFR= SQRT( XRY(IX) + XRY(IX+1) )
      ZBAFR= (COS(YBAFR*XK57 + XK15) + YBAFR)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WBAFR + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YBAFR)
      XX= XX + CK2
      YFRBA= DMOD(XX,XK1)*XK76
      IF(YFRBA.GT.XK98) YFRBA= XK98
      IF(YFRBA.LT.XK99) YFRBA= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WFRBA= SQRT( XRY(IX) + XRY(IX+1) )
      ZFRBA= (COS(YFRBA*XK57 + XK15) + YFRBA)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WFRBA + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YFRBA)
      XX= XX + CK2
      YPBRF= DMOD(XX,XK1)*XK76
      IF(YPBRF.GT.XK98) YPBRF= XK98
      IF(YPBRF.LT.XK99) YPBRF= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WPBRF= SQRT( XRY(IX) + XRY(IX+1) )
      ZPBRF= (COS(YPBRF*XK57 + XK15) + YPBRF)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WPBRF + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YPBRF)
      XX= XX + CK2
      YRFPB= DMOD(XX,XK1)*XK76
      IF(YRFPB.GT.XK98) YRFPB= XK98
      IF(YRFPB.LT.XK99) YRFPB= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WRFPB= SQRT( XRY(IX) + XRY(IX+1) )
      ZRFPB= (COS(YRFPB*XK57 + XK15) + YRFPB)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WRFPB + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YRFPB)
      XX= XX + CK2
      YPBFR= DMOD(XX,XK1)*XK76
      IF(YPBFR.GT.XK98) YPBFR= XK98
      IF(YPBFR.LT.XK99) YPBFR= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WPBFR= SQRT( XRY(IX) + XRY(IX+1) )
      ZPBFR= (COS(YPBFR*XK57 + XK15) + YPBFR)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WPBFR + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YPBFR)
      XX= XX + CK2
      YFRPB= DMOD(XX,XK1)*XK76
      IF(YFRPB.GT.XK98) YFRPB= XK98
      IF(YFRPB.LT.XK99) YFRPB= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WFRPB= SQRT( XRY(IX) + XRY(IX+1) )
      ZFRPB= (COS(YFRPB*XK57 + XK15) + YFRPB)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WFRPB + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YFRPB)
      XX= XX + CK2
      YBPRF= DMOD(XX,XK1)*XK76
      IF(YBPRF.GT.XK98) YBPRF= XK98
      IF(YBPRF.LT.XK99) YBPRF= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WBPRF= SQRT( XRY(IX) + XRY(IX+1) )
      ZBPRF= (COS(YBPRF*XK57 + XK15) + YBPRF)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WBPRF + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YBPRF)
      XX= XX + CK2
      YRFBP= DMOD(XX,XK1)*XK76
      IF(YRFBP.GT.XK98) YRFBP= XK98
      IF(YRFBP.LT.XK99) YRFBP= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WRFBP= SQRT( XRY(IX) + XRY(IX+1) )
      ZRFBP= (COS(YRFBP*XK57 + XK15) + YRFBP)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WRFBP + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YRFBP)
      XX= XX + CK2
      YBPFR= DMOD(XX,XK1)*XK76
      IF(YBPFR.GT.XK98) YBPFR= XK98
      IF(YBPFR.LT.XK99) YBPFR= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WBPFR= SQRT( XRY(IX) + XRY(IX+1) )
      ZBPFR= (COS(YBPFR*XK57 + XK15) + YBPFR)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WBPFR + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YBPFR)
      XX= XX + CK2
      YFRBP= DMOD(XX,XK1)*XK76
      IF(YFRBP.GT.XK98) YFRBP= XK98
      IF(YFRBP.LT.XK99) YFRBP= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WFRBP= SQRT( XRY(IX) + XRY(IX+1) )
      ZFRBP= (COS(YFRBP*XK57 + XK15) + YFRBP)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WFRBP + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YFRBP)
      XX= XX + CK2
      YAQRF= DMOD(XX,XK1)*XK76
      IF(YAQRF.GT. XK98) YAQRF= XK98
      IF(YAQRF.LT.XK99) YAQRF= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WAQRF= SQRT( XRY(IX) + XRY(IX+1) )
      ZAQRF= (COS(YAQRF*XK57 + XK15) + YAQRF)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WAQRF + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YAQRF)
      XX= XX + CK2
      YRFAQ= DMOD(XX,XK1)*XK76
      IF(YRFAQ.GT.XK98) YRFAQ= XK98
      IF(YRFAQ.LT.XK99) YRFAQ= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WRFAQ= SQRT( XRY(IX) + XRY(IX+1) )
      ZRFAQ= (COS(YRFAQ*XK57 + XK15) + YRFAQ)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WRFAQ + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YRFAQ)
      XX= XX + CK2
      YAQFR= DMOD(XX,XK1)*XK76
      IF(YAQFR.GT.XK98) YAQFR= XK98
      IF(YAQFR.LT.XK99) YAQFR= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WAQFR= SQRT( XRY(IX) + XRY(IX+1) )
      ZAQFR= (COS(YAQFR*XK57 + XK15) + YAQFR)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WAQFR + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YAQFR)
      XX= XX + CK2
      YFRAQ= DMOD(XX,XK1)*XK76
      IF(YFRAQ.GT.XK98) YFRAQ= XK98
      IF(YFRAQ.LT.XK99) YFRAQ= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WFRAQ= SQRT( XRY(IX) + XRY(IX+1) )
      ZFRAQ= (COS(YFRAQ*XK57 + XK15) + YFRAQ)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WFRAQ + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YFRAQ)
      XX= XX + CK2
      YQARF= DMOD(XX,XK1)*XK76
      IF(YQAFR.GT.XK98) YQAFR= XK98
      IF(YQAFR.LT.XK99) YQAFR= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WQARF= SQRT( XRY(IX) + XRY(IX+1) )
      ZQARF= (COS(YQARF*XK57 + XK15) + YQARF)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WQARF + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YQARF)
      XX= XX + CK2
      YRFQA= DMOD(XX,XK1)*XK76
      IF(YRFQA.GT.XK98) YRFQA= XK98
      IF(YRFQA.LT.XK99) YRFQA= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WRFQA= SQRT( XRY(IX) + XRY(IX+1) )
      ZRFQA= (COS(YRFQA*XK57 + XK15) + YRFQA)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WRFQA + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YRFQA)
      XX= XX + CK2
      YQAFR= DMOD(XX,XK1)*XK76
      IF(YQAFR.GT.XK98) YQAFR= XK98
      IF(YQAFR.LT.XK99) YQAFR= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WQAFR= SQRT( XRY(IX) + XRY(IX+1) )
      ZQAFR= (COS(YQAFR*XK57 + XK15) + YQAFR)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WQAFR + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YQAFR)
      XX= XX + CK2
      YFRQA= DMOD(XX,XK1)*XK76
      IF(YFRQA.GT.XK98) YFRQA= XK98
      IF(YFRQA.LT.XK99) YFRQA= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WFRQA= SQRT( XRY(IX) + XRY(IX+1) )
      ZFRQA= (COS(YFRQA*XK57 + XK15) + YFRQA)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WFRQA + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YFRQA)
      XX= XX + CK2
      YPQRF= DMOD(XX,XK1)*XK76
      IF(YPQRF.GT.XK98) YPQRF= XK98
      IF(YPQRF.LT.XK99) YPQRF= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WPQRF= SQRT( XRY(IX) + XRY(IX+1) )
      ZPQRF= (COS(YPQRF*XK57 + XK15) + YPQRF)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WPQRF + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YPQRF)
      XX= XX + CK2
      YRFPQ= DMOD(XX,XK1)*XK76
      IF(YRFPQ.GT.XK98) YRFPQ= XK98
      IF(YRFPQ.LT.XK99) YRFPQ= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WRFPQ= SQRT( XRY(IX) + XRY(IX+1) )
      ZRFPQ= (COS(YRFPQ*XK57 + XK15) + YRFPQ)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WRFPQ + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YRFPQ)
      XX= XX + CK2
      YPQFR= DMOD(XX,XK1)*XK76
      IF(YPQFR.GT.XK98) YPQFR= XK98
      IF(YPQFR.LT.XK99) YPQFR= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WPQFR= SQRT( XRY(IX) + XRY(IX+1) )
      ZPQFR= (COS(YPQFR*XK57 + XK15) + YPQFR)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WPQFR + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YPQFR)
      XX= XX + CK2
      YFRPQ= DMOD(XX,XK1)*XK76
      IF(YFRPQ.GT.XK98) YFRPQ= XK98
      IF(YFRPQ.LT.XK99) YFRPQ= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WFRPQ= SQRT( XRY(IX) + XRY(IX+1) )
      ZFRPQ= (COS(YFRPQ*XK57 + XK15) + YFRPQ)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WFRPQ + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YFRPQ)
      XX= XX + CK2
      YQPRF= DMOD(XX,XK1)*XK76
      IF(YQPRF.GT.XK98) YQPRF= XK98
      IF(YQPRF.LT.XK99) YQPRF= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WQPRF= SQRT( XRY(IX) + XRY(IX+1) )
      ZQPRF= (COS(YQPRF*XK57 + XK15) + YQPRF)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WQPRF + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YQPRF)
      XX= XX + CK2
      YRFQP= DMOD(XX,XK1)*XK76
      IF(YRFQP.GT.XK98) YRFQP= XK98
      IF(YRFQP.LT.XK99) YRFQP= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WRFQP= SQRT( XRY(IX) + XRY(IX+1) )
      ZRFQP= (COS(YRFQP*XK57 + XK15) + YRFQP)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WRFQP + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YRFQP)
      XX= XX + CK2
      YQPFR= DMOD(XX,XK1)*XK76
      IF(YQPFR.GT.XK98) YQPFR= XK98
      IF(YQPFR.LT.XK99) YQPFR= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WQPFR= SQRT( XRY(IX) + XRY(IX+1) )
      ZQPFR= (COS(YQPFR*XK57 + XK15) + YQPFR)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WQPFR + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YQPFR)
      XX= XX + CK2
      YFRQP= DMOD(XX,XK1)*XK76
      IF(YFRQP.GT.XK98) YFRQP= XK98
      IF(YFRQP.LT.XK99) YFRQP= XK99
      IX= INT(RAND()*XK53) + 1
      XRY(IX)= XRY(IX) + XK47
      WFRQP= SQRT( XRY(IX) + XRY(IX+1) )
      ZFRQP= (COS(YFRQP*XK57 + XK15) + YFRQP)*2. + XK23
      WX= XK43 + XKA*XRY(IX+100)
      IF (( XX +XK92).GT.( WFRQP + XK10 ))  TCNT= TCNT+1.
      XY= XY +(WX*XK61 - XK68)*XK35/(XK85+YFRQP)

1000  CONTINUE
      T2 = CLOCK()
      DELTAT = (T2 - T1) / (0.1 * IN )
      PRINT *,' '
      PRINT *,'Average frame time: ',DELTAT,' Milliseconds.'
  
      WRITE(6,101) XX,XY,TCNT
101   FORMAT(4H XX ,G14.6,4H XY ,G14.6,4HTCNT,G14.6)
      STOP
      END

C       Note some non standard code in following routine:
C        ! for inline comment
C        OPTIONS as compiler command to suppress runtime overflow checks.
C
C       OPTIONS  /NOCHECK
C       FUNCTION  RANDOM(IR)
C        Real function producing value in range 0.0, 1.0
C        IR is integer seed which is updated by routine.
C       INTEGER*2   IR            ! 16 bit integer
C       IR = 14579*IR + 1
C       RANDOM = (IR/2 + 16383.0)/32767.0
C       RETURN
C       END
