C+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+
C+                                                                     +
C+                        FILE SSSU06.FOR                              +
C+                                                                     +
C+   CONTAINS UTILITY SUBPROGRAMS REQUIRED ONLY BY CONTOURING PROGRAM  +
C+                                                                     +
C+       ROUTINES IN FILE ORDER             ALPHABETICAL ORDER         +
C+   TRANF  : LINES    15 TO   120    HIGHT  : LINES   121 TO   394    +
C+   HIGHT  : LINES   121 TO   394    PHI    : LINES   395 TO   437    +
C+   PHI    : LINES   395 TO   437    TRANF  : LINES    15 TO   120    +
C+                                                                     +
C+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+
C+*+* TRANF  START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      REAL*8 FUNCTION TRANF(X,ISW,ITYPE,A,B)
C
C     ******************************************************************
C     *                                                                *
C     *   THIS FUNCTION IS USED TO PERFORM A DATA TRANSFORMATION, OR   *
C     *   THE CORRESPONDING INVERSE TRANSFORMATION                     *
C     *                                                                *
C     *   ARGUMENTS                                                    *
C     *   *********                                                    *
C     *   X      - DATA VALUE TO BE TRANSFORMED                        *
C     *   ISW    - SWITCH FOR TRANSF OR INVERSE TRANSF ( 1 OR -1 )     *
C     *   ITYPE  - TYPE OF TRANSFORMATION                              *
C     *   A,B    - COEFFICIENTS OF THE TRANSFORMATION                  *
C     *                                                                *
C     *                                                                *
C     ******************************************************************
C
      IMPLICIT LOGICAL (A-Z)
C
C     ARGUMENTS
C     *********
      INTEGER ISW,ITYPE
      REAL*8 X,A,B
C
C     COMMON BLOCKS
C     *************
C     CHARS   CNTRL
C
C-----INCLUDE STATEMENT FOR CNTRL COMMON BLOCKS ------------------------
C      INCLUDE 'H:\CONTOUR\MASTER\COMMON'
CCCC  CNTRL COMMON BLOCKS
C
      CHARACTER*80 TITLE
      CHARACTER*30 XTITLE,YTITLE,ZTITLE
      CHARACTER*5  XUNITS,YUNITS,ZUNITS
      CHARACTER*30 NAMES(20)
      COMMON /CHARS/ TITLE,XTITLE,YTITLE,ZTITLE,
     *               XUNITS,YUNITS,ZUNITS,NAMES
C
      LOGICAL SALF,LOW,PC386,PRIME,HP9000,VAX
      LOGICAL LGSV(30)
      INTEGER LU,LL,LR,QNO,IN(40)
      INTEGER IDD(10),ICS(10), IGG(30),IKN(10) ,IPF(10),IPS(40)
      REAL*8  RDD(10),RCS(130),RGG(50),RKN(350),RPF(20),RPS(30)
      COMMON /CNTRL/ SALF,LOW,PC386,PRIME,HP9000,VAX,
     *               LL,LU,LR, QNO,LGSV,IN,IDD,ICS,IGG,IKN,IPF,IPS,
     *                           RDD,RCS,RGG,RKN,RPF,RPS
C--
C--
C--
C--
C--
C--
C--
C--
C--
C--
C--
C--
C--
C--
C-----------------------------------------------------------------------
C
C     SUBPROGRAM TRANF  CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     ABORT
C
C     SUBPROGRAM TRANF  IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     BOXES   FNVOL   HIGHT1  HUCL7   LOHYP2  LPCTR   RDATA   SDATA
C     SSU20
C
      IF( ISW**2.NE.1 ) GOTO 998
C
C     LINEAR TRANSFORMATION?
C
      IF( ITYPE.GE.1 .AND. ITYPE.LE.3 ) THEN
	  IF( ISW.EQ.1 ) THEN
	      TRANF=A+B*X
	  ELSE
C             INVERSE
	      TRANF=(X-A)/B
	  ENDIF
C
C     LOGARITHMIC?
C
      ELSEIF( ITYPE.GE.4 .AND. ITYPE.LE.5 ) THEN
	 IF( ISW.EQ.1 ) THEN
	      TRANF=LOG(A+B*X)
	 ELSE
C             INVERSE
	      TRANF=(EXP(X)-A)/B
	 ENDIF
      ELSE
	  GOTO 999
      ENDIF
C
C
C
      RETURN
C
C
 998  CONTINUE
      WRITE(LL,2998)
2998  FORMAT(//2X,'******  TRANSF SWITCH .NE. 1 OR -1 ******')
      CALL ABORT
C
C
 999  CONTINUE
      WRITE(LL,2999)
2999  FORMAT(//2X,'******  ITYPE < 1  OR  > 5 IN TRANF ******')
      CALL ABORT
C
C     **
C     **   END OF FUNCTION TRANF
C     **
C
      END
C+*+* TRANF  END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* HIGHT  START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE HIGHT(XX,YY,ZZ)
C
C     ******************************************************************
C     *                                                                *
C     *   THIS SUBROUTINE IS USED TO DETERMINE THE HEIGHT OF THE       *
C     *   BICUBIC SPLINE, USED TO REPRESENT THE SOURCE DATA, AT THE    *
C     *   POINT WITH COORDINATES XX AND YY. THE ARRAYS LAMDA AND MU    *
C     *   SPECIFY THE  X-INTERCEPTS AND Y-INTERCEPTS RESPECTIVELY.     *
C     *                                                                *
C     *   ARGUMENTS                                                    *
C     *   *********                                                    *
C     *   XX,YY - COORDINATES OF A POINT                               *
C     *   ZZ    - HEIGHT OF BICUBIC SPLINE AT THE POINT                *
C     *                                                                *
C     ******************************************************************
C
      IMPLICIT LOGICAL (A-Z)
C
C     ARGUMENTS
C     *********
      REAL*8 XX,YY,ZZ
C
C     COMMON BLOCKS
C     *************
C     CHARS   CNTRL
C
C-----INCLUDE STATEMENT FOR CNTRL COMMON BLOCKS ------------------------
C      INCLUDE 'H:\CONTOUR\MASTER\COMMON'
CCCC  CNTRL COMMON BLOCKS
C
      CHARACTER*80 TITLE
      CHARACTER*30 XTITLE,YTITLE,ZTITLE
      CHARACTER*5  XUNITS,YUNITS,ZUNITS
      CHARACTER*30 NAMES(20)
      COMMON /CHARS/ TITLE,XTITLE,YTITLE,ZTITLE,
     *               XUNITS,YUNITS,ZUNITS,NAMES
C
      LOGICAL SALF,LOW,PC386,PRIME,HP9000,VAX
      LOGICAL LGSV(30)
      INTEGER LU,LL,LR,QNO,IN(40)
      INTEGER IDD(10),ICS(10), IGG(30),IKN(10) ,IPF(10),IPS(40)
      REAL*8  RDD(10),RCS(130),RGG(50),RKN(350),RPF(20),RPS(30)
      COMMON /CNTRL/ SALF,LOW,PC386,PRIME,HP9000,VAX,
     *               LL,LU,LR, QNO,LGSV,IN,IDD,ICS,IGG,IKN,IPF,IPS,
     *                           RDD,RCS,RGG,RKN,RPF,RPS
C--
C--
C--
C--
C--
C--
C--
C--
C--
C--
C--
C--
C--
C--
C-----------------------------------------------------------------------
C
C     SUBPROGRAM HIGHT  CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     ABORT
C
C     SUBPROGRAM HIGHT  REFERENCES THE FOLLOWING FUNCTIONS
C     ****************************************************
      REAL*8 X02ABF
C
C     SUBPROGRAM HIGHT  IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     FNVOL   HIGHT1  SDATA   SPLIN
C
C     LOCAL VARIABLES
C     ***************
      INTEGER XNOT,YNOT,NC,NPTS,NADRES,NPOINT
      PARAMETER (XNOT=17,YNOT=17,NC=(XNOT-4)*(YNOT-4),NPTS=2002,
     *   NADRES=(XNOT-7)*(YNOT-7),NPOINT=NPTS+NADRES)
      INTEGER PX,PY,QX,QY,NAD,I,IADRES,ITEMP,JJ,JX,JXOLD,JY,K4,K7,KK,L,
     *   L4,L7,LLL,NP,PANEL,U,ADRES(NADRES),POINT1(NPOINT)
      REAL*8 DX4,DX5,DX6,DX7,DX8,DX9,DY4,DY5,DY6,DY7,DY8,DY9,E2,E3,E4,
     *   E5,LAMDA1,LAMDA2,LAMDA3,LAMDA4,LAMDA5,LAMDA6,M1,M2,M3,MU1,MU2,
     *   MU3,MU4,MU5,MU6,MXL,TEMP,WI,MX(4),MY(4),DX,DY,DEL
C
C
C     EQUIVALENCED VARIABLES
C     **********************
      REAL*8 LAMDA(XNOT),MU(YNOT),C(NC)
      EQUIVALENCE (RKN(1), LAMDA(1)),
     *            (RKN(21),MU(1)),
     *            (RKN(51),C(1))
C
      PX=IKN(2)
      PY=IKN(3)
      QX=PX-4
      QY=PY-4
      NAD=(PX-7)*(PY-7)
C
      K7=PX-7
      L7=PY-7
      NP=K7*L7
      K4=PX-4
      L4=PY-4
      IF (PX.LT.8 .OR. PY.LT.8 .OR. NAD.NE.NP) THEN
	 WRITE (LR,9999)
 9999    FORMAT (///' *** ERROR IN SUBROUTINE HIGHT ***'
     *      /5X,'NUMBER OF X-INTERCEPTS IS LESS THAN 8 OR ...'
     *      /5X,'NUMBER OF Y-INTERCEPTS IS LESS THAN 8 OR ...'
     *      /5X,'TOTAL NUMBER OF PANELS IS INCORRECT'
     *      /' +++ PLEASE REPORT THIS FAULT +++')
	  CALL ABORT
      ENDIF
C
CCCC  CHECK THAT DATA POINT IS NOT OUTSIDE THE RECTANGLE DEFINED BY THE
CCCC  EXTREME KNOTS.
C
      LAMDA1=LAMDA(4)
      LAMDA2=LAMDA(PX-3)
      DX=1.05*(LAMDA2-LAMDA1)/2.0
      LAMDA1=(LAMDA1+LAMDA2)/2.0-DX
      LAMDA2=LAMDA1+2.0*DX
      MU1=MU(4)
      MU2=MU(PY-3)
      DY=1.05*(MU2-MU1)/2.0
      MU1=(MU1+MU2)/2.0-DY
      MU2=MU1+2.0*DY
      IF (XX.LT.LAMDA1 .OR. XX.GT.LAMDA2 .OR. YY.LT.MU1 .OR.
     *                                              YY.GT.MU2 ) THEN
	 WRITE (LR,9998)XX,YY
 9998    FORMAT (///' *** ERROR IN SUBROUTINE HIGHT ***'
     *      /5X,'THE POINT (',2(1X,1PE12.5),' ) IS OUTSIDE THE DATA'
     *      /5X,'   AREA DEFINED BY THE PANELS'
     *      /' +++ PLEASE REPORT THIS FAULT +++')
	  CALL ABORT
      ENDIF
C
CCCC  DETERMINE PANEL IN WHICH POINT (XX,YY) LIES
C
      DO 100 PANEL=1,NAD
	 ITEMP=PANEL+1
	 ADRES(PANEL)=ITEMP
	 POINT1(ITEMP)=0
  100 CONTINUE
      L=4
      U=PX-3
  120 I=(L+U)/2
      IF ((U-L).GT.1) THEN
	 IF (XX.LT.LAMDA(I)) THEN
	    U=I
	 ELSE
	    L=I
	 ENDIF
	 GOTO 120
      ENDIF
      PANEL=U-5
      L=4
      U=PY-3
  130 I=(U+L)/2
      IF ((U-L).GT.1) THEN
	 IF (YY.LT.MU(I)) THEN
	    U=I
	 ELSE
	    L=I
	 ENDIF
	 GOTO 130
      ENDIF
      PANEL=PANEL*L7+U-4
      ITEMP=ADRES(PANEL)
      ADRES(PANEL)=1
      POINT1(ITEMP)=1
      POINT1(1)=0
C
CCCC
C
      JXOLD=0
      DO 200 I=1,NP
	 IADRES=I+1
	 IF (POINT1(IADRES).GT.0) THEN
	    JX=(I-1)/L7
	    JY=I-JX*L7
	    JX=JX+1
	    IF (JX.NE.JXOLD) THEN
	       LAMDA1=LAMDA(JX+1)
	       LAMDA2=LAMDA(JX+2)
	       LAMDA3=LAMDA(JX+3)
	       LAMDA4=LAMDA(JX+4)
	       LAMDA5=LAMDA(JX+5)
	       LAMDA6=LAMDA(JX+6)
	       DX4=1.0/(LAMDA4-LAMDA1)
	       DX5=1.0/(LAMDA5-LAMDA2)
	       DX6=1.0/(LAMDA6-LAMDA3)
	       DX7=1.0/(LAMDA4-LAMDA2)
	       DX8=1.0/(LAMDA5-LAMDA3)
	       DX9=1.0/(LAMDA4-LAMDA3)
	       JXOLD=JX
	    ENDIF
	    MU1=MU(JY+1)
	    MU2=MU(JY+2)
	    MU3=MU(JY+3)
	    MU4=MU(JY+4)
	    MU5=MU(JY+5)
	    MU6=MU(JY+6)
	    DY4=1.0/(MU4-MU1)
	    DY5=1.0/(MU5-MU2)
	    DY6=1.0/(MU6-MU3)
	    DY7=1.0/(MU4-MU2)
	    DY8=1.0/(MU5-MU3)
	    DY9=1.0/(MU4-MU3)
  140       IADRES=POINT1(IADRES)
	    IF (IADRES.GT.0) THEN
C
CCCC           CHECK THAT DATA POINTS ARE ACCESSED IN PANEL ORDER
C
	       DEL=X02ABF(XX)
	       IF (XX.LT.LAMDA3-DEL .OR. XX.GT.LAMDA4+DEL
     *            .OR. (ABS(XX-LAMDA4).LT.DEL .AND. JX.NE.K7)
     *            .OR. YY.LT.MU3-DEL .OR. YY.GT.MU4+DEL
     *              .OR. (ABS(YY-MU4).LT.DEL .AND. JY.NE.L7)) THEN
		  WRITE (LR,9997) XX,YY
 9997             FORMAT (///' *** ERROR IN SUBROUTINE HIGHT ***'
     *               /5X,'DATA POINT NOT ACCESSED IN PANEL ORDER'
     *               /5X,' X = ',F12.4,'   Y = ',F12.4
     *               /' +++ PLEASE REPORT THIS FAULT +++')
		   CALL ABORT
	       ENDIF
C
CCCC           EVALUATE THE FOUR NON-ZERO CUBIC B-SPLINES AT XX, AND
CCCC           STORE IN MX(1) TO MX(4).
C
	       E5=LAMDA5-XX
	       E4=LAMDA4-XX
	       E3=XX-LAMDA3
	       E2=XX-LAMDA2
	       M2=E3*DX8
	       M1=E4*DX7
	       M3=E3*M2*DX6
	       M2=(E2*M1+E5*M2)*DX5
	       M1=E4*M1*DX4
	       WI=DX9
	       MX(4)=WI*E3*M3
	       MX(3)=WI*(E2*M2+(LAMDA6-XX)*M3)
	       MX(2)=WI*((XX-LAMDA1)*M1+E5*M2)
	       MX(1)=WI*E4*M1
C
CCCC           EVALUATE THE FOUR NON-ZERO CUBIC B-SPLINES AT YY, AND
CCCC           STORE IN MY(1) TO MY(4).
C
	       E5=MU5-YY
	       E4=MU4-YY
	       E3=YY-MU3
	       E2=YY-MU2
	       M2=E3*DY8
	       M1=E4*DY7
	       M3=E3*M2*DY6
	       M2=(E2*M1+E5*M2)*DY5
	       M1=E4*M1*DY4
	       WI=DY9
	       MY(4)=WI*E3*M3
	       MY(3)=WI*(E2*M2+(MU6-YY)*M3)
	       MY(2)=WI*((YY-MU1)*M1+E5*M2)
	       MY(1)=WI*E4*M1
C
CCCC           EVALUATE THE BICUBIC SPLINE FUNCTION.
C
	       KK=(JX-1)*L4+JY
	       TEMP=0
	       DO 180 LLL=1,4
		  MXL=MX(LLL)
		  DO 160 JJ=1,4
		     ITEMP=KK+JJ-1
		     TEMP=TEMP+MXL*MY(JJ)*C(ITEMP)
  160             CONTINUE
		  KK=KK+L4
  180          CONTINUE
	       ZZ=TEMP
	       GO TO 140
	    ENDIF
	 ENDIF
  200 CONTINUE
      RETURN
C
C     **
C     **    END OF SUBROUTINE HIGHT
C     **
C
      END
C+*+* HIGHT  END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* PHI    START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      REAL*8 FUNCTION PHI(X)
C
C     ******************************************************************
C     *                                                                *
C     *   THIS FUNCTION                                                *
C     *                                                                *
C     *   ARGUMENTS                                                    *
C     *   *********                                                    *
C     *   X  -                                                         *
C     *                                                                *
C     ******************************************************************
C
      IMPLICIT LOGICAL (A-Z)
C
C     ARGUMENT
C     ********
      REAL*8 X
C
C     SUBPROGRAM PHI    IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     INTTO   SSATT
C
      X=ABS(X)
      IF (X.LT.1.0) THEN
	 PHI=1.0-0.5*X*X*(5.0-3.0*X)
      ELSEIF (X.GT.2.0) THEN
	 PHI=0.0
      ELSE
	 X=2.0-X
	 PHI=-0.5*X*X*(1.0-X)
      ENDIF
      RETURN
C
C     **
C     **    END OF FUNCTION PHI
C     **
C
      END
C+*+* PHI    END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
