C+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+
C+                                                                     +
C+                        FILE SSSU12.FOR                              +
C+                                                                     +
C+     SUBPROGRAMS FOR CREATING CONTROL FILES FROM CONTROL COMMON      +
C+                                                                     +
C+       ROUTINES IN FILE ORDER             ALPHABETICAL ORDER         +
C+   SSU12  : LINES    31 TO   150    EFOLEY : LINES  1417 TO  1550    +
C+   EQDD   : LINES   151 TO   281    EIPSCH : LINES  1326 TO  1416    +
C+   EREFM  : LINES   282 TO   379    ELOHYP : LINES   681 TO   792    +
C+   EQCS   : LINES   380 TO   535    ELOSUP : LINES   793 TO   900    +
C+   EQGG   : LINES   536 TO   680    EMDQSM : LINES  1015 TO  1127    +
C+   ELOHYP : LINES   681 TO   792    ENFQSM : LINES  1128 TO  1226    +
C+   ELOSUP : LINES   793 TO   900    EQCS   : LINES   380 TO   535    +
C+   ERFTPS : LINES   901 TO  1014    EQDD   : LINES   151 TO   281    +
C+   EMDQSM : LINES  1015 TO  1127    EQGG   : LINES   536 TO   680    +
C+   ENFQSM : LINES  1128 TO  1226    EQKN   : LINES  1551 TO  1693    +
C+   ERHMTH : LINES  1227 TO  1325    EQPOL  : LINES  2351 TO  2442    +
C+   EIPSCH : LINES  1326 TO  1416    EQPS   : LINES  1796 TO  1934    +
C+   EFOLEY : LINES  1417 TO  1550    EQPSC  : LINES  2056 TO  2350    +
C+   EQKN   : LINES  1551 TO  1693    EQPSI  : LINES  1935 TO  2055    +
C+   ETRAN  : LINES  1694 TO  1795    EREFM  : LINES   282 TO   379    +
C+   EQPS   : LINES  1796 TO  1934    ERFTPS : LINES   901 TO  1014    +
C+   EQPSI  : LINES  1935 TO  2055    ERHMTH : LINES  1227 TO  1325    +
C+   EQPSC  : LINES  2056 TO  2350    ETRAN  : LINES  1694 TO  1795    +
C+   EQPOL  : LINES  2351 TO  2442    SSU12  : LINES    31 TO   150    +
C+                                                                     +
C+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+
C+*+* SSU12  START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE SSU12
C
C     *************************************************************
C     *                                                           *
C     *  This subroutine calls the subroutines:                   *
C     *                                                           *
C     *         EQDD                                              *
C     *         EREFM                                             *
C     *         EQCS   -EQGG   -ETRAN                             *
C     *                -EQKN                                      *
C     *                -ETRAN                                     *
C     *         EQPS   -EQPSI                                     *
C     *                -EQPSC  -EQPOL                             *
C     *                                                           *
C     *************************************************************
C
      IMPLICIT LOGICAL(A-Z)
C
C     ARGUMENTS
C     *********
      CHARACTER*30 NAME
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
C     SUBPROGRAM SSU12  CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     EQCS    EQDD    EQPS    EREFM   FLNAM
C
C     SUBPROGRAM SSU12  REFERENCES THE FOLLOWING FUNCTIONS
C     ****************************************************
      INTEGER IUNIT
C
C     SUBPROGRAM SSU12  IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     HSU01   HSU02
C
C     LOCAL VARIABLES
C     ***************
      INTEGER LE
C
CCC   CREATE AND WRITE CONTROL FILE CORRESPONDING TO CURRENT
CCC   CONTROL COMMON
C
      WRITE(LL,2000)
      WRITE(LR,2000)
 2000 FORMAT(//
     *    /2X,'********************************************',
     *    /2X,'*                                          *',
     *    /2X,'*            SUITE SSSU12                  *',
     *    /2X,'*                                          *',
     *    /2X,'*       CREATION OF CONTROL FILE           *',
     *    /2X,'*                                          *',
     *    /2X,'********************************************')
C
CCC   ASK FOR THE NAME OF THE FILE
C
      CALL FLNAM(NAME,1)
C
      LE=IUNIT()
      IN(22)=LE
      OPEN(LE,FILE=NAME,STATUS='UNKNOWN')                               C-PC386
C     OPEN(LE,FILE=NAME,STATUS='UNKNOWN')                               C-PRIME
C     OPEN(LE,FILE=NAME,STATUS='UNKNOWN')                               C-HP9000
C     OPEN(LE,FILE=NAME,STATUS='NEW',CARRIAGECONTROL='LIST')            C-VAX
C
CCC   set default switches to force explicit non-defaults
C
      IPS(23)=1
      IPS(24)=1
      IPS(25)=1
C
C      QNO 0
C
      CALL EQDD
C
      CALL EREFM
C
      CALL EQCS
C
      CALL EQPS
C
      CLOSE(LE)
C
      WRITE(LL,2003)
      WRITE(LR,2003)
 2003 FORMAT(//2X,'*********  SUITE  SSSU12 FINISHED  *********'////)
C
      RETURN
C
C
C     *
C     *  END OF SUBROUTINE SSU12
C     *
C
      END
C+*+* SSU12  END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* EQDD   START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE EQDD
C
C     *************************************************************
C     *                                                           *
C     *   SUBROUTINE EQDD                                         *
C     *                                                           *
C     *************************************************************
C
      IMPLICIT LOGICAL(A-Z)
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
C     SUBPROGRAM EQDD   CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     ZWRITE
C
C     SUBPROGRAM EQDD   IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     SSU12
C
C     LOCAL VARIABLES
C     ***************
      CHARACTER*80 LINE
      INTEGER LE
C
CCC   SET UP OUTPUT CHANNEL
C
      LE=IN(22)
C
C
C      QNO 201
C     Source data file name.
C
      WRITE(LINE,1003) NAMES(1),'SOURCE DATA FILE NAME'
      CALL ZWRITE(LL,LE,LINE)
C
C      QNO 202
C     Data-file-type.
C
      WRITE(LINE,1001) IDD(1),'DATA FILE TYPE'
      CALL ZWRITE(LL,LE,LINE)
C
C     If rectangular
C
      IF (IDD(1).EQ.1) RETURN
C
C     If tabular
C
      IF (IDD(1).EQ.3) THEN
C
C      QNO 203
C     Column number for tabular data
C
	    WRITE(LINE,1001) IDD(2),'COLUMN NUMBER'
	    CALL ZWRITE(LL,LE,LINE)
C
C      QNO 204
C     Parameter name for column from tabular data
C
	    WRITE(LINE,1003) TITLE(51:80),'COLUMN PARAMETER NAME'
	    CALL ZWRITE(LL,LE,LINE)
      ENDIF
C
C     If standardised
C
      IF(IDD(1).EQ.2) THEN
	    IF (IDD(3).EQ.1) THEN
C
C      QNO 205
C
		  WRITE(LINE,1003) 'Y','STANDARD FORMAT?'
		  CALL ZWRITE(LL,LE,LINE)
		  RETURN
	    ELSE
		  WRITE(LINE,1003) 'N','STANDARD FORMAT?'
		  CALL ZWRITE(LL,LE,LINE)
	    ENDIF
      ENDIF
C
C      QNO 206
C     LAT/LONG numeric format
C
      WRITE(LINE,1001) IDD(4),'LAT/LONG  NUMERIC FORMAT'
      CALL ZWRITE(LL,LE,LINE)
C
C
C      QNO 207
C     LAT/LONG NSEW format
C
      WRITE(LINE,1001) IDD(5),'LAT/LONG  NSEW FORMAT'
      CALL ZWRITE(LL,LE,LINE)
C
C
C
 1001 FORMAT(I6,T33,A)
 1003 FORMAT(A,T33,A)
C
C
      RETURN
C
C
C     *
C     *  END OF SUBROUTINE EQDD
C     *
C
      END
C+*+* EQDD   END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* EREFM  START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE EREFM
C
C     *************************************************************
C     *                                                           *
C     *   SUBROUTINE EREFM                                        *
C     *                                                           *
C     *************************************************************
C
      IMPLICIT LOGICAL(A-Z)
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
C     SUBPROGRAM EREFM  CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     ZWRITE
C
C     SUBPROGRAM EREFM  IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     SSU12
C
C     LOCAL VARIABLES
C     ***************
      CHARACTER*80 LINE
      INTEGER LE
C
CCC   SET UP OUTPUT CHANNEL
C
      LE=IN(22)
C
C     If standardised
C
      IF (IDD(3).EQ.1) THEN
C
C      QNO 211
C     If data in LAT/LONG,'DATA IN LAT/LONG'
C
	    IF (IDD(8).EQ.0) THEN
		WRITE(LINE,1003) 'Y','X-Y CORRESPOND TO LAT/LONG?'
		CALL ZWRITE(LL,LE,LINE)
	    ELSE
		WRITE(LINE,1003) 'N','X-Y CORRESPOND TO LAT/LONG?'
		CALL ZWRITE(LL,LE,LINE)
	    ENDIF
C
	    RETURN
C
      ENDIF
C
C      QNO 212
C     Working file name
C
      IF (NAMES(2).EQ.'SFIL') THEN
	  WRITE(LINE,1003) 'N','SAVE SCATTERED DATA?'
	  CALL ZWRITE(LL,LE,LINE)
      ELSE
	  WRITE(LINE,1003) 'Y','SAVE SCATTERED DATA?'
	  CALL ZWRITE(LL,LE,LINE)
	  WRITE(LINE,1003) NAMES(2),'SCATTERED DATA FILE NAME'
	  CALL ZWRITE(LL,LE,LINE)
      ENDIF
C
C
 1003 FORMAT(A,T33,A)
C
C
      RETURN
C
C
C     *
C     *  END OF SUBROUTINE EREFM
C     *
C
      END
C+*+* EREFM  END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* EQCS   START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE EQCS
C
C     *************************************************************
C     *                                                           *
C     *   SUBROUTINE EQCS                                         *
C     *                                                           *
C     *************************************************************
C
      IMPLICIT LOGICAL(A-Z)
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
C
C     SUBPROGRAM EQCS   CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     EQGG    EQKN    ETRAN   ZWRITE
C
C     SUBPROGRAM EQCS   IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     SSU12
C
C     LOCAL VARIABLES
C     ***************
      CHARACTER*80 LINE
      INTEGER LE,II
C
CCC   SET UP OUTPUT CHANNEL
C
      LE=IN(22)
C
C      QNO 221
C     Contouring type
C
      WRITE(LINE,1001) ICS(1),'CONTOURING TYPE'
      CALL ZWRITE(LL,LE,LINE)
C
C     If line-printer
C
      IF ((ICS(1).EQ.1).OR.(ICS(1).EQ.2)) THEN
C
C      QNO 222
C     Line-printer output file name
C
	    IF (NAMES(8).EQ.'LPCONT') THEN
		WRITE(LINE,1003) 'N','ALTERNATIVE FILE FOR LP OUTPUT?'
		CALL ZWRITE(LL,LE,LINE)
	    ELSE
		WRITE(LINE,1003) 'Y','ALTERNATIVE FILE FOR LP OUTPUT?'
		CALL ZWRITE(LL,LE,LINE)
C
		WRITE(LINE,1003) NAMES(8),'NAME OF LP OUTPUT FILE'
		CALL ZWRITE(LL,LE,LINE)
	    ENDIF
      ENDIF
C
C     If scattered or tabular
C
      IF ((IDD(1).EQ.2).OR.(IDD(1).EQ.3)) THEN
C
C      QNO 223
C     If not NAG-scattered and not pts without contours
C
	    IF (ICS(1).LE.3.OR.ICS(1).EQ.5.OR.ICS(1).EQ.6) THEN
		  WRITE(LINE,1003) 'Y','USE GRIDDING SUITE?'
		  CALL ZWRITE(LL,LE,LINE)
		  CALL EQGG
C
C     If contouring type is NAG-scattered
C
	    ELSEIF (ICS(1).EQ.4) THEN
C
		  CALL EQKN
C
	    ENDIF
      ENDIF
C
C      QNO 224
C     Definition of plotting region.
C
C     X min
      WRITE(LINE,1002) RCS(1),'X MIN FOR PLOTTING REGION'
      CALL ZWRITE(LL,LE,LINE)
C
C     X max
      WRITE(LINE,1002) RCS(2),'X MAX  "     "       "   '
      CALL ZWRITE(LL,LE,LINE)
C
C     Y min
      WRITE(LINE,1002) RCS(3),'Y MIN  "     "       "   '
      CALL ZWRITE(LL,LE,LINE)
C
C     Y max
      WRITE(LINE,1002) RCS(4),'Y MAX  "     "       "   '
      CALL ZWRITE(LL,LE,LINE)
C
C     If isometric or plotting of sample
C
      IF (ICS(1).EQ.7 .OR. ICS(1).EQ.6) RETURN
C
C      QNO 225
C     Number of contours
C
      WRITE(LINE,1001) ICS(4),'NUMBER OF CONTOURS'
      CALL ZWRITE(LL,LE,LINE)
C
C      QNO 226
C     Contour heights
C
      DO 10 II=11,10+ICS(4)
C
	  WRITE(LINE,1002) RCS(II),'CONTOUR HEIGHT NUMBER ',II-10
	  CALL ZWRITE(LL,LE,LINE)
C
   10 CONTINUE
C
      CALL ETRAN(ICS(7),RCS(21),RCS(22))
C
C
 1001 FORMAT(I6,T33,A)
 1002 FORMAT(G12.5,T33,A,I3)
 1003 FORMAT(A,T33,A)
C
C
      RETURN
C
C
C     *
C     *  END OF SUBROUTINE EQCS
C     *
C
      END
C+*+* EQCS   END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* EQGG   START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
      SUBROUTINE EQGG
C
C     *************************************************************
C     *                                                           *
C     *   SUBROUTINE EQGG                                         *
C     *                                                           *
C     *************************************************************
C
      IMPLICIT LOGICAL(A-Z)
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
C     SUBPROGRAM EQGG   CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     EFOLEY  EIPSCH  ELOHYP  ELOSUP  EMDQSM  ENFQSM  ERFTPS  ERHMTH
C     ETRAN   ZWRITE
C
C     SUBPROGRAM EQGG   IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     EQCS
C
C     LOCAL VARIABLES
C     ***************
      CHARACTER*80 LINE
      INTEGER LE
C
CCC   SET UP OUTPUT CHANNEL
C
      LE=IN(22)
C
C         Q130 -- Which gridder is to be used?
      WRITE (LINE,1001) IGG(2),'GRIDDING METHOD REQUIRED'
      CALL ZWRITE (LL,LE,LINE)
C
C         Call questions for appropriate gridder
      IF (IGG(2).EQ.1) THEN
C        Lowestoft hypergridder
	 CALL ELOHYP
      ELSEIF (IGG(2).EQ.2 .OR. IGG(2).EQ.3) THEN
C        Lowestoft supergridder / Lowestoft ordinary gridder
	 CALL ELOSUP
      ELSEIF (IGG(2).EQ.4) THEN
C        Franke's grid method using thin plate splines
	 CALL ERFTPS
      ELSEIF (IGG(2).EQ.5) THEN
C        Modified quadratic Shepard's method
	 CALL EMDQSM
      ELSEIF (IGG(2).EQ.6) THEN
C        Nielson-Franke quadratic Shepard's method
	 CALL ENFQSM
      ELSEIF (IGG(2).EQ.7 .OR. IGG(2).EQ.8) THEN
C        Duchon's thin plate method / Duchon's radial cubic method
C        These methods do not require parameter values to be given
	 CONTINUE
      ELSEIF (IGG(2).EQ.9 .OR. IGG(2).EQ.10) THEN
C        Hardy's multiquadratic / reciprocal multiquadratic method
	 CALL ERHMTH
      ELSEIF (IGG(2).EQ.11) THEN
C        Akima's method - does not require parameter values to be given
	 CONTINUE
      ELSEIF (IGG(2).EQ.12) THEN
C        Schagen's method
	 CALL EIPSCH
      ELSEIF (IGG(2).EQ.13) THEN
C        Foley's three stage method
	 CALL EFOLEY
      ENDIF
C
C         Q700 -- Region to be gridded
C                 Minimum value of X
      WRITE (LINE,1002) RGG(4),'X MIN FOR GRIDDING REGION '
      CALL ZWRITE (LL,LE,LINE)
C                 Maximum value of X
      WRITE (LINE,1002) RGG(5),'X MAX FOR GRIDDING REGION '
      CALL ZWRITE (LL,LE,LINE)
C                 Minimum value of Y
      WRITE (LINE,1002) RGG(6),'Y MIN FOR GRIDDING REGION '
      CALL ZWRITE (LL,LE,LINE)
C                 Maximum value of Y
      WRITE (LINE,1002) RGG(7),'Y MAX FOR GRIDDING REGION '
      CALL ZWRITE (LL,LE,LINE)
C
C         Q702 -- Grid size
C                 number of points 'across', M
      WRITE (LINE,1001) IGG(5),'GRID SIZE - ACROSS , M'
      CALL ZWRITE (LL,LE,LINE)
C                 number of points 'down', N
      WRITE (LINE,1001) IGG(6),'GRID SIZE - DOWN   , N'
      CALL ZWRITE (LL,LE,LINE)
C
C         Call transformation questions module
      CALL ETRAN(IGG(7),RGG(8),RGG(9))
C
C         Q703 -- Name of gridded data file
      IF (NAMES(3).EQ.'RFIL') THEN
C             Gridded data is not to be saved
	 WRITE (LINE,1003) 'N','SAVE GRIDDED DATA?'
	 CALL ZWRITE (LL,LE,LINE)
      ELSE
C             Gridded data is to be saved
	 WRITE (LINE,1003) 'Y','SAVE GRIDDED DATA?'
	 CALL ZWRITE (LL,LE,LINE)
C
C             Q121 -- Name of gridded data file
	 WRITE (LINE,1003) NAMES(3),'GRIDDED DATA FILE NAME'
	 CALL ZWRITE (LL,LE,LINE)
      ENDIF
C
C
 1001 FORMAT(I6,T33,A)
 1002 FORMAT(G12.5,T33,A,I3)
 1003 FORMAT(A,T33,A)
C
C
      RETURN
C
C     *
C     *  END OF SUBROUTINE EQGG
C     *
C
      END
C+*+* EQGG   END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* ELOHYP START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
      SUBROUTINE ELOHYP
C
C     *************************************************************
C     *                                                           *
C     *   SUBROUTINE ELOHYP                                       *
C     *                                                           *
C     *************************************************************
C
      IMPLICIT LOGICAL(A-Z)
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
C     SUBPROGRAM ELOHYP CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     ELOSUP  ZWRITE
C
C     SUBPROGRAM ELOHYP IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     EQGG
C
C     LOCAL VARIABLES
C     ***************
      CHARACTER*80 LINE
      INTEGER LE,COUNT
C
CCC   SET UP OUTPUT CHANNEL
C
      LE=IN(22)
C
      DO 10 COUNT=1,4
C
C            Q131 -- Change hypergridder default parameter values
	 WRITE (LINE,1001) COUNT,'CHANGE HYPERGRIDDER DEFAULTS'
	 CALL ZWRITE (LL,LE,LINE)
C
	 IF (COUNT.EQ.1) THEN
C
C               Q150 -- Starting values for hypergridder
	    WRITE (LINE,1001) IGG(9)+1,'HYPERGRIDDER STARTING VALUE'
	    CALL ZWRITE (LL,LE,LINE)
C
	    IF (IGG(9).EQ.2) THEN
C              Starting values are held in a file
C
C                  Q121 -- Name of file that holds the starting values
	       WRITE (LINE,1003) NAMES(14),'STARTING VALUES FILENAME'
	       CALL ZWRITE (LL,LE,LINE)
	    ENDIF
	 ELSEIF (COUNT.EQ.2) THEN
C
C               Q151 -- Relaxation parameter
	    WRITE (LINE,1002) RGG(3),'RELAXATION  PARAMETER'
	    CALL ZWRITE (LL,LE,LINE)
	 ELSEIF (COUNT.EQ.3) THEN
C
C               Q152 -- Accuracy criterion
	    WRITE (LINE,1002) RGG(10),'ACCURACY CRITERION'
	    CALL ZWRITE (LL,LE,LINE)
	 ELSE
C
C               Q153 -- Maximum number of iterations in hypergridder
	    WRITE(LINE,1001) IGG(4),'MAXIMUM NUMBER OF ITERATIONS'
	    CALL ZWRITE (LL,LE,LINE)
	 ENDIF
   10 CONTINUE
C
C         Q131 -- Accept hypergridder parameter values
      WRITE (LINE,1003) 'A','ACCEPT HYPERGRIDDER PARAMETERS'
      CALL ZWRITE (LL,LE,LINE)
C
      IF (IGG(9).EQ.1) THEN
C        Starting values set by ordinary gridder
	 CALL ELOSUP
      ENDIF
C
C
 1001 FORMAT(I6,T33,A)
 1002 FORMAT(G12.5,T33,A,I3)
 1003 FORMAT(A,T33,A)
C
C
      RETURN
C
C     *
C     *  END OF SUBROUTINE ELOHYP
C     *
C
      END
C+*+* ELOHYP END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* ELOSUP START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
      SUBROUTINE ELOSUP
C
C     *************************************************************
C     *                                                           *
C     *   SUBROUTINE ELOSUP                                       *
C     *                                                           *
C     *************************************************************
C
      IMPLICIT LOGICAL(A-Z)
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
C     SUBPROGRAM ELOSUP CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     ZWRITE
C
C     SUBPROGRAM ELOSUP IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     ELOHYP  EQGG
C
C     LOCAL VARIABLES
C     ***************
      CHARACTER*80 LINE
      INTEGER LE,COUNT
C
CCC   SET UP OUTPUT CHANNEL
C
      LE=IN(22)
C
      DO 10 COUNT=1,2
	 IF (IGG(2).EQ.2) THEN
C
C               Q132 -- Change supergridder default parameter values
	    WRITE (LINE,1001) COUNT,'CHANGE SUPERGRIDDER DEFAULTS'
	    CALL ZWRITE (LL,LE,LINE)
	 ELSE
C
C               Q133 -- Change ordinary gridder default parameter values
	    WRITE (LINE,1001) COUNT,'CHANGE ORDINARY GRIDDER DEFAULTS'
	    CALL ZWRITE (LL,LE,LINE)
	 ENDIF
C
	 IF (COUNT.EQ.1) THEN
C
C               Q156 -- Weighting function
	    WRITE (LINE,1001) IGG(3),'WEIGHTING FUNCTION'
	    CALL ZWRITE (LL,LE,LINE)
	 ELSE
C
C               Q157 -- Sample spacings
	    WRITE (LINE,1002) RGG(1),'SAMPLE SPACING IN X-DIRECTION'
	    CALL ZWRITE (LL,LE,LINE)
	    WRITE (LINE,1002) RGG(2),'SAMPLE SPACING IN Y-DIRECTION'
	    CALL ZWRITE (LL,LE,LINE)
	 ENDIF
C
C
   10 CONTINUE
C
      IF (IGG(2).EQ.2) THEN
C
C            Q132 -- Accept supergridder parameter values
	 WRITE (LINE,1003) 'A','ACCEPT SUPERGRIDDER DEFAULTS'
	 CALL ZWRITE (LL,LE,LINE)
      ELSE
C
C            Q133 -- Accept ordinary gridder parameter values
	 WRITE (LINE,1003) 'A','ACCEPT ORDINARY GRIDDER DEFAULTS'
	 CALL ZWRITE (LL,LE,LINE)
      ENDIF
C
C
 1001 FORMAT(I6,T33,A)
 1002 FORMAT(G12.5,T33,A,I3)
 1003 FORMAT(A,T33,A)
C
C
      RETURN
C
C     *
C     *  END OF SUBROUTINE ELOSUP
C     *
C
      END
C+*+* ELOSUP END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* ERFTPS START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
      SUBROUTINE ERFTPS
C
C     *************************************************************
C     *                                                           *
C     *   SUBROUTINE ERFTPS                                       *
C     *                                                           *
C     *************************************************************
C
      IMPLICIT LOGICAL(A-Z)
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
C     SUBPROGRAM ERFTPS CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     ZWRITE
C
C     SUBPROGRAM ERFTPS IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     EQGG
C
C     LOCAL VARIABLES
C     ***************
      CHARACTER*80 LINE
      INTEGER LE,COUNT,I
C
CCC   SET UP OUTPUT CHANNEL
C
      LE=IN(22)
C
      DO 10 COUNT=1,1
C
C            Q134 -- Change default parameter values for Franke's
C                    grid method using thin plate splines
	 WRITE (LINE,1001) COUNT,'CHANGE FRANKE''S GRID DEFAULTS'
	 CALL ZWRITE (LL,LE,LINE)
C
	 IF (COUNT.EQ.1) THEN
C            Q160 -- Average number of points per panel
	 WRITE (LINE,1001) IGG(11),'AVERAGE NUMBER OF POINTS PER PANEL'
	 CALL ZWRITE (LL,LE,LINE)
C
	    IF (IGG(11).EQ.0) THEN
C              User-defined panels
C
C                  Q161 -- Number of interior x-knots
	       WRITE (LINE,1001) IGG(12)-2,'NUMBER OF INTERIOR X-KNOTS'
	       CALL ZWRITE (LL,LE,LINE)
C
	       IF (IGG(12).GT.2) THEN
C
C                     Q163 -- Positions of x-knots
		  DO 20 I=12,IGG(12)+9
		     WRITE (LINE,1002) RGG(I),'POSITION OF X-KNOT ',I-11
		     CALL ZWRITE (LL,LE,LINE)
   20             CONTINUE
	       ENDIF
C
C                  Q162 -- Number of interior y-knots
	       WRITE (LINE,1001) IGG(13)-2,'NUMBER OF INTERIOR Y-KNOTS'
	       CALL ZWRITE (LL,LE,LINE)
C
	       IF (IGG(13).GT.2) THEN
C
C                     Q164 -- Positions of y-knots
		  DO 30 I=32,IGG(13)+29
		     WRITE (LINE,1002) RGG(I),'POSITION OF Y-KNOT ',I-31
		     CALL ZWRITE (LL,LE,LINE)
   30             CONTINUE
	       ENDIF
	    ENDIF
	 ENDIF
   10 CONTINUE
C
C         Q134 -- Accept parameter values for Franke's grid method
      WRITE (LINE,1003) 'A','ACCEPT FRANKE''S GRID PARAMETERS'
      CALL ZWRITE (LL,LE,LINE)
C
C
 1001 FORMAT(I6,T33,A)
 1002 FORMAT(G12.5,T33,A,I3)
 1003 FORMAT(A,T33,A)
C
C
      RETURN
C
C     *
C     *  END OF SUBROUTINE ERFTPS
C     *
C
      END
C+*+* ERFTPS END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* EMDQSM START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
      SUBROUTINE EMDQSM
C
C     *************************************************************
C     *                                                           *
C     *   SUBROUTINE EMDQSM                                       *
C     *                                                           *
C     *************************************************************
C
      IMPLICIT LOGICAL(A-Z)
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
C     SUBPROGRAM EMDQSM CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     ZWRITE
C
C     SUBPROGRAM EMDQSM IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     EQGG
C
C     LOCAL VARIABLES
C     ***************
      CHARACTER*80 LINE
      INTEGER LE,COUNT
C
CCC   SET UP OUTPUT CHANNEL
C
      LE=IN(22)
C
      DO 10 COUNT=1,3
C
C            Q134 -- Change default parameter values for modified
C                    quadratic Shepard's method
	 WRITE (LINE,1001) COUNT,'CHANGE MOD. QUAD. SHEPARD DEFAULTS'
	 CALL ZWRITE (LL,LE,LINE)
C
	 IF (COUNT.EQ.1) THEN
C
C               Q167 -- Methods for determining the radius of influence
	    WRITE (LINE,1001) IGG(14),'METHOD FOR RADIUS OF INFLUENCE'
	    CALL ZWRITE (LL,LE,LINE)
	 ELSEIF (COUNT.EQ.2) THEN
	    IF (IGG(14).EQ.1) THEN
C              Radius is program-determined
C
C                  Q168 -- Number of points for nodal functions
	       WRITE (LINE,1001) IGG(15),'NO. PTS. FOR NODAL FUNCTIONS'
	       CALL ZWRITE (LL,LE,LINE)
	    ELSE
C              Radius is user-determined
C
C                  Q170 -- Radius for nodal functions
	       WRITE (LINE,1002) RGG(38),'RADIUS FOR NODAL FUNCTIONS'
	       CALL ZWRITE (LL,LE,LINE)
	    ENDIF
	 ELSE
	    IF (IGG(14).EQ.1) THEN
C              Radius is program-determined
C
C                  Q169 -- Number of points for weight functions
	       WRITE (LINE,1001) IGG(16),'NO. PTS. FOR WEIGHT FUNCTIONS'
	       CALL ZWRITE (LL,LE,LINE)
	    ELSE
C              Radius is user-determined
C
C                  Q171 -- Radius for weight functions
	       WRITE (LINE,1002) RGG(39),'RADIUS FOR WEIGHT FUNCTIONS'
	       CALL ZWRITE (LL,LE,LINE)
	    ENDIF
	 ENDIF
   10 CONTINUE
C
C         Q134 -- Accept modified quadratic Shepard's method parameters
      WRITE (LINE,1003) 'A','ACCEPT MOD. QUAD. SHEPARD''S PARAMETERS'
      CALL ZWRITE (LL,LE,LINE)
C
C
 1001 FORMAT(I6,T33,A)
 1002 FORMAT(G12.5,T33,A,I3)
 1003 FORMAT(A,T33,A)
C
C
      RETURN
C
C     *
C     *  END OF SUBROUTINE EMDQSM
C     *
C
      END
C+*+* EMDQSM END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* ENFQSM START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
      SUBROUTINE ENFQSM
C
C     *************************************************************
C     *                                                           *
C     *   SUBROUTINE ENFQSM                                       *
C     *                                                           *
C     *************************************************************
C
      IMPLICIT LOGICAL(A-Z)
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
C     SUBPROGRAM ENFQSM CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     ZWRITE
C
C     SUBPROGRAM ENFQSM IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     EQGG
C
C     LOCAL VARIABLES
C     ***************
      CHARACTER*80 LINE
      INTEGER LE,COUNT
C
CCC   SET UP OUTPUT CHANNEL
C
      LE=IN(22)
C
      DO 10 COUNT=1,2
C
C            Q136 -- Change default parameter values for the
C                    Nielson-Franke quadratic Shepard's method
	 WRITE (LINE,1001) COUNT,'CHANGE NIELSON-FRANKE DEFAULTS'
	 CALL ZWRITE (LL,LE,LINE)
C
	 IF (COUNT.EQ.1) THEN
C
C               Q174 -- Methods for determining the radius of influence
	    WRITE (LINE,1001) IGG(14),'RADIUS OF INFLUENCE METHODS'
	    CALL ZWRITE (LL,LE,LINE)
	 ELSE
	    IF (IGG(14).EQ.1) THEN
C              Program-determined radius of influence
C
C                  Q175 -- Number of points for nodal functions
	       WRITE (LINE,1001) IGG(15),'NO. PTS. FOR NODAL FUNCTIONS'
	       CALL ZWRITE (LL,LE,LINE)
	    ELSE
C              User-determined radius of influence
C
C                  Q176 -- Radius for nodal functions
	       WRITE (LINE,1002) RGG(38),'RADIUS FOR NODAL FUNCTIONS'
	       CALL ZWRITE (LL,LE,LINE)
	    ENDIF
	 ENDIF
   10 CONTINUE
C
C         Q136 -- Accept Nielson-Franke parameter values
      WRITE (LINE,1003) 'A','ACCEPT NIELSON-FRANKE PARAMETERS'
      CALL ZWRITE (LL,LE,LINE)
C
C
 1001 FORMAT(I6,T33,A)
 1002 FORMAT(G12.5,T33,A,I3)
 1003 FORMAT(A,T33,A)
C
C
      RETURN
C
C     *
C     *  END OF SUBROUTINE ENFQSM
C     *
C
      END
C+*+* ENFQSM END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* ERHMTH START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
      SUBROUTINE ERHMTH
C
C     *************************************************************
C     *                                                           *
C     *   SUBROUTINE ERHMTH                                       *
C     *                                                           *
C     *************************************************************
C
      IMPLICIT LOGICAL(A-Z)
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
C     SUBPROGRAM ERHMTH CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     ZWRITE
C
C     SUBPROGRAM ERHMTH IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     EQGG
C
C     LOCAL VARIABLES
C     ***************
      CHARACTER*80 LINE
      INTEGER LE,COUNT
C
CCC   SET UP OUTPUT CHANNEL
C
      LE=IN(22)
C
      DO 10 COUNT=1,1
	 IF (IGG(2).EQ.9) THEN
C
C               Q139 -- Change default parameter values for Hardy's
C                       multiquadratic method
	    WRITE (LINE,1001) COUNT,'CHANGE HARDY MULTI. DEFAULTS'
	    CALL ZWRITE (LL,LE,LINE)
	 ELSE
C
C               Q140 -- Change default parameter values for Hardy's
C                       reciprocal multiquadratic method
	    WRITE (LINE,1001) COUNT,'CHANGE HARDY RECIPROCAL DEFAULTS'
	    CALL ZWRITE (LL,LE,LINE)
	 ENDIF
C
C            Q179 -- Value of NPPR
	 WRITE (LINE,1001) IGG(18),'VALUE OF NPPR'
	 CALL ZWRITE (LL,LE,LINE)
   10 CONTINUE
C
	 IF (IGG(2).EQ.9) THEN
C
C               Q139 -- Accept parameter values for Hardy's
C                       multiquadratic method
	    WRITE (LINE,1003) 'A','ACCEPT HARDY MULTI. PARAMETERS'
	    CALL ZWRITE (LL,LE,LINE)
	 ELSE
C
C               Q140 -- Accept parameter values for Hardy's
C                       reciprocal multiquadratic method
	    WRITE (LINE,1003) 'A','ACCEPT HARDY RECIPROCAL PARAMETERS'
	    CALL ZWRITE (LL,LE,LINE)
	 ENDIF
C                                                                    fil
 
 1001 FORMAT(I6,T33,A)
 1003 FORMAT(A,T33,A)
C
C
      RETURN
C
C     *
C     *  END OF SUBROUTINE ERHMTH
C     *
C
      END
C+*+* ERHMTH END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* EIPSCH START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
      SUBROUTINE EIPSCH
C
C     *************************************************************
C     *                                                           *
C     *   SUBROUTINE EIPSCH                                       *
C     *                                                           *
C     *************************************************************
C
      IMPLICIT LOGICAL(A-Z)
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
C     SUBPROGRAM EIPSCH CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     ZWRITE
C
C     SUBPROGRAM EIPSCH IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     EQGG
C
C     LOCAL VARIABLES
C     ***************
      CHARACTER*80 LINE
      INTEGER LE,COUNT
C
CCC   SET UP OUTPUT CHANNEL
C
      LE=IN(22)
C
      DO 10 COUNT=1,2
C
C            Q142 -- Change default parameters for Schagen's method
	 WRITE (LINE,1001) COUNT,'CHANGE SCHAGEN''S METHOD DEFAULTS'
	 CALL ZWRITE (LL,LE,LINE)
C
	 IF (COUNT.EQ.1) THEN
C
C               Q182 -- Value to which function tends as it moves away
C                       from areas of data (the Grand Mean)
	    WRITE (LINE,1002) RGG(11),'GRAND MEAN'
	    CALL ZWRITE (LL,LE,LINE)
	 ELSE
C
C               Q183 -- Distance over which correlation between two
C                       points is strong
	    WRITE (LINE,1002) RGG(12),'CORRELATION DISTANCE'
	    CALL ZWRITE (LL,LE,LINE)
	 ENDIF
   10 CONTINUE
C
C         Q142 -- Accept parameter values for Schagen's method
      WRITE (LINE,1003) 'A','ACCEPT SCHAGEN''S METHOD PARAMETERS'
      CALL ZWRITE (LL,LE,LINE)
C
C
 1001 FORMAT(I6,T33,A)
 1002 FORMAT(G12.5,T33,A,I3)
 1003 FORMAT(A,T33,A)
C
C
      RETURN
C
C     *
C     *  END OF SUBROUTINE EIPSCH
C     *
C
      END
C+*+* EIPSCH END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* EFOLEY START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
      SUBROUTINE EFOLEY
C
C     *************************************************************
C     *                                                           *
C     *   SUBROUTINE EFOLEY                                       *
C     *                                                           *
C     *************************************************************
C
      IMPLICIT LOGICAL(A-Z)
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
C     SUBPROGRAM EFOLEY CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     ZWRITE
C
C     SUBPROGRAM EFOLEY IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     EQGG
C
C     LOCAL VARIABLES
C     ***************
      CHARACTER*80 LINE
      INTEGER LE,COUNT,I
C
CCC   SET UP OUTPUT CHANNEL
C
      LE=IN(22)
C
      DO 10 COUNT=1,6
C
C            Q143 -- Change default parameter values for Foley's method
	 WRITE (LINE,1001) COUNT,'CHANGE FOLEY''S METHOD DEFAULTS'
	 CALL ZWRITE (LL,LE,LINE)
C
	 IF (COUNT.EQ.1) THEN
C
C               Q186 -- Mode of surface fitting
	    WRITE (LINE,1001) IGG(20)+1,'SURFACE FITTING MODE'
	    CALL ZWRITE (LL,LE,LINE)
	 ELSEIF (COUNT.EQ.2) THEN
C
C               Q187 -- Degree of fit on boundary
	    WRITE (LINE,1001) IGG(21),'DEGREE OF FIT ON BOUNDARY'
	    CALL ZWRITE (LL,LE,LINE)
	 ELSEIF (COUNT.EQ.3) THEN
C
C               Q188 -- Degree of fit on interior
	    WRITE (LINE,1001) IGG(22),'DEGREE OF FIT ON INTERIOR'
	    CALL ZWRITE (LL,LE,LINE)
	 ELSEIF (COUNT.EQ.4) THEN
C
C               Q189 -- No transformation / log transformation of
C                       function values
	    WRITE (LINE,1001) IGG(23)+1,'TRANSFORM OF FUNCTION VALUES'
	    CALL ZWRITE (LL,LE,LINE)
	 ELSEIF (COUNT.EQ.5) THEN
C
C               Q190 -- Choice of interior grid
	    WRITE (LINE,1001) IGG(24)+1,'CHOICE OF INTERIOR GRID'
	    CALL ZWRITE (LL,LE,LINE)
C
	    IF (IGG(24).EQ.1) THEN
C              User-determined panels
C
C                  Q637 -- Number of vertical grid lines
	       WRITE (LINE,1001) IGG(3),'NUMBER OF VERTICAL GRID LINES'
	       CALL ZWRITE (LL,LE,LINE)
C
C                  Q638 -- Positions of vertical grid lines
	       DO 20 I=12,IGG(3)+11
		  WRITE (LINE,1002) RGG(I),'X-COORDINATE ',I-11
		  CALL ZWRITE (LL,LE,LINE)
   20          CONTINUE
C
C                  Q639 -- Number of horizontal grid lines
	       WRITE (LINE,1001) IGG(4),'NUMBER OF HORIZ. GRID LINES'
	       CALL ZWRITE (LL,LE,LINE)
C
C                  Q640 -- Positions of horizontal grid lines
	       DO 30 I=32,IGG(4)+31
		  WRITE (LINE,1002) RGG(I),'Y COORDINATE ',I-31
		  CALL ZWRITE (LL,LE,LINE)
   30          CONTINUE
	    ENDIF
	 ELSE
C
C               Q191 -- Partial derivative definition
	    WRITE(LINE,1001) IGG(25)+1,'PARTIAL DERIVATIVE DEFINITION'
	    CALL ZWRITE (LL,LE,LINE)
	 ENDIF
   10 CONTINUE
C
C         Q143 -- Accept parameter values for Foley's method
      WRITE (LINE,1003) 'A','ACCEPT FOLEY''S METHOD PARAMETERS'
      CALL ZWRITE (LL,LE,LINE)
C
C
 1001 FORMAT(I6,T33,A)
 1002 FORMAT(G12.5,T33,A,I3)
 1003 FORMAT(A,T33,A)
C
C
      RETURN
C
C     *
C     *  END OF SUBROUTINE EFOLEY
C     *
C
      END
C+*+* EFOLEY END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* EQKN   START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE EQKN
C
C     *************************************************************
C     *                                                           *
C     *   SUBROUTINE EQKN                                         *
C     *                                                           *
C     *************************************************************
C
      IMPLICIT LOGICAL(A-Z)
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
C     SUBPROGRAM EQKN   CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     ZWRITE
C
C     SUBPROGRAM EQKN   IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     EQCS
C
C     LOCAL VARIABLES
C     ***************
      CHARACTER*80 LINE
      INTEGER LE,II
C
CCC   SET UP OUTPUT CHANNEL
C
      LE=IN(22)
C
C      QNO 261
C     Program attends to all details
C
      IF (IKN(1).EQ.1) THEN
	  WRITE(LINE,1003) 'Y','PROG ATTEND TO SPLINE FITTING DETAILS?'
	  CALL ZWRITE(LL,LE,LINE)
C
C     Else set all values
C
      ELSE
	  WRITE(LINE,1003) 'N','PROG ATTEND TO SPLINE FITTING DETAILS?'
	  CALL ZWRITE(LL,LE,LINE)
C
C      QNO 262
C     Default value of EPS?
C
	  WRITE(LINE,1003) 'N','USE DEFAULT VALUE OF EPS?'
	  CALL ZWRITE(LL,LE,LINE)
C
C      QNO 263
C     Value  of EPS
C
	  WRITE(LINE,1002) RKN(41),'VALUE OF EPS'
	  CALL ZWRITE(LL,LE,LINE)
C
C      QNO 264
C     Total number of knots in x direction
C
	  WRITE(LINE,1001) IKN(2)-8,'TOTAL NO OF KNOTS IN X-DIR'
	  CALL ZWRITE(LL,LE,LINE)
C
C      QNO 265
C     Total number of knots in y direction
C
	  WRITE(LINE,1001) IKN(3)-8,'TOTAL NO OF KNOTS IN Y-DIR'
	  CALL ZWRITE(LL,LE,LINE)
	  IF ((IKN(2)-8)**2+(IKN(3)-8)**2.EQ.0) RETURN
C
C      QNO 266
C     Program defined positions of knots
C
	  IF (IKN(1).EQ.2) THEN
	      WRITE(LINE,1003) 'Y','PROG DEF POSN OF KNOTS?'
	      CALL ZWRITE(LL,LE,LINE)
	   ELSEIF (IKN(1).EQ.3) THEN
	      WRITE(LINE,1003) 'N','PROG DEF POSN OF KNOTS?'
	      CALL ZWRITE(LL,LE,LINE)
C
C     Total number of knots in x direction
C
	      IF (IKN(2).GT.8) THEN
C
C      QNO 267
C     Positions of x knots
C
		  DO 50  II=5,5+IKN(2)-9
		     WRITE(LINE,1002) RKN(II),'POS OF X KNOT ',II-4
		     CALL ZWRITE(LL,LE,LINE)
   50             CONTINUE
	    ENDIF
C
C     Total number of knots in y direction
C
	    IF (IKN(3).GT.8) THEN
C
C      QNO 268
C     positions of y knots
C
		  DO 60  II=25,25+IKN(3)-9
		     WRITE(LINE,1002) RKN(II),'POS OF Y KNOT ',II-4
		     CALL ZWRITE(LL,LE,LINE)
   60             CONTINUE
		  ENDIF
	    ENDIF
      ENDIF
C
C
 1001 FORMAT(I6,T33,A)
 1002 FORMAT(G12.5,T33,A,I3)
 1003 FORMAT(A,T33,A)
C
C
      RETURN
C
C     *
C     *  END OF SUBROUTINE EQKN
C     *
C
      END
C+*+* EQKN   END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* ETRAN  START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE ETRAN(IETFG,A,B)
C
C     *************************************************************
C     *                                                           *
C     *   SUBROUTINE ETRAN                                        *
C     *                                                           *
C     *************************************************************
C
      IMPLICIT LOGICAL(A-Z)
C
C     ARGUMENTS
C     *********
      INTEGER IETFG
      REAL*8 A,B
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
C     SUBPROGRAM ETRAN  CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     ZWRITE
C
C     SUBPROGRAM ETRAN  IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     EQCS    EQGG
C
C     LOCAL VARIABLES
C     ***************
      CHARACTER*80 LINE
      INTEGER LE
C
CCC   SET UP OUTPUT CHANNEL
C
      LE=IN(22)
C
C      QNO 271
C     If no data transformation
C
      IF (IETFG.EQ.0) THEN
	    WRITE(LINE,1003) 'N','DATA TRANSFORMATION?'
	    CALL ZWRITE(LL,LE,LINE)
      ELSE
	    WRITE(LINE,1003) 'Y','DATA TRANSFORMATION?'
	    CALL ZWRITE(LL,LE,LINE)
C
C      QNO 272
C     Type of data transformation
C
	    WRITE(LINE,1001) IETFG,'TYPE OF DATA TRANSFORMATION'
	    CALL ZWRITE(LL,LE,LINE)
C
C      QNO 273
C     Change of origin
C
	    IF (IETFG.EQ.1) THEN
		  WRITE(LINE,1002) -A,'ORIGIN'
		  CALL ZWRITE(LL,LE,LINE)
	    ELSEIF (IETFG.EQ.3 .OR. IETFG.EQ.5) THEN
		  WRITE(LINE,1002) A,'VALUE OF A'
		  CALL ZWRITE(LL,LE,LINE)
		  WRITE(LINE,1002) B,'VALUE OF B'
		  CALL ZWRITE(LL,LE,LINE)
	    ENDIF
      ENDIF
C
 1001 FORMAT(I6,T33,A)
 1002 FORMAT(G12.5,T33,A,I3)
 1003 FORMAT(A,T33,A)
C
C
      RETURN
C
C
C     *
C     *   END OF SUBROUTINE ETRAN
C     *
C
      END
C+*+* ETRAN  END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* EQPS   START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE EQPS
C
C     *************************************************************
C     *                                                           *
C     *   SUBROUTINE EQPS                                         *
C     *                                                           *
C     *************************************************************
C
      IMPLICIT LOGICAL(A-Z)
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
C     SUBPROGRAM EQPS   CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     EQPSC   EQPSI   ZWRITE
C
C     SUBPROGRAM EQPS   IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     SSU12
C
C     LOCAL VARIABLES
C     ***************
      CHARACTER*80 LINE
      INTEGER LE
      REAL*8 XL,YL
C
CCC   SET UP OUTPUT CHANNEL
C
      LE=IN(22)
C
C      QNO 301
C     Is data title included
C
      IF (IPS(10).EQ.1) THEN
	    WRITE(LINE,1003) 'Y','PLOT TITLE INCLUDED?'
	    CALL ZWRITE(LL,LE,LINE)
      ELSE
	    WRITE(LINE,1003) 'N','PLOT TITLE INCLUDED?'
	    CALL ZWRITE(LL,LE,LINE)
      ENDIF
C
C     If LP contours, other plotting details not required
C
      IF ((ICS(1).EQ.1).OR.(ICS(1).EQ.2)) THEN
C
	    CONTINUE
C
C     Elseif isometric projection
C
      ELSEIF (ICS(1).EQ.6) THEN
C
	    CALL EQPSI
C
C     Else proper contourer required
C
      ELSE
C
	    CALL EQPSC
C
      ENDIF
C
C      QNO 302
C     Default plot size options
C
      IF (IPS(25).EQ.0) THEN
C           Default plot size specified
	    WRITE(LINE,1003) 'Y','DEFAULT PLOT SIZE ?'
	    CALL ZWRITE(LL,LE,LINE)
      ELSEIF (RPS(1).LT.1.0E-4 .OR. RPS(2).LT.1.0E-4) THEN
C           Actual plot size has not been set yet (set in SSSU40), so
C           assume that the default plot size has been specified
	    WRITE(LINE,1003) 'Y','DEFAULT PLOT SIZE ?'
	    CALL ZWRITE(LL,LE,LINE)
      ELSE
C           Actual plot size has been specified
	    WRITE(LINE,1003) 'N','DEFAULT PLOT SIZE?'
	    CALL ZWRITE(LL,LE,LINE)
C
C
	    IF ((ICS(1).EQ.1).OR.(ICS(1).EQ.2)) THEN
C               LINE-PRINTER CTRS
		XL=RPS(1)*25.4
		YL=RPS(2)*25.4
	    ELSE
		XL=RPS(1)
		YL=RPS(2)
	    ENDIF
C
C      QNO 303
C
	   WRITE(LINE,1002) XL,'X-LENGTH'
	   CALL ZWRITE(LL,LE,LINE)
C
C      QNO 304
C
	   IF (IPS(2).NE.1) THEN
	       WRITE(LINE,1002) YL,'Y-LENGTH'
	       CALL ZWRITE(LL,LE,LINE)
	   ENDIF
C
      ENDIF
C
C
 1002 FORMAT(G12.5,T33,A,I3)
 1003 FORMAT(A,T33,A)
C
C
      RETURN
C
C     *
C     *  END OF SUBROUTINE EQPS
C     *
C
      END
C+*+* EQPS   END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* EQPSI  START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE EQPSI
C
C     *************************************************************
C     *                                                           *
C     *   SUBROUTINE EQPSI                                        *
C     *                                                           *
C     *************************************************************
C
      IMPLICIT LOGICAL(A-Z)
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
C     SUBPROGRAM EQPSI  CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     ZWRITE
C
C     SUBPROGRAM EQPSI  IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     EQPS
C
C     LOCAL VARIABLES
C     ***************
      CHARACTER*80 LINE
      INTEGER LE
C
CCC   SET UP OUTPUT CHANNEL
C
      LE=IN(22)
C
      IF( IDD(8).EQ.1 ) THEN
C         abstract data -- need to specify X and Y titles
C         QNO 355
	  WRITE(LINE,1003) XTITLE,'X-AXIS TITLE'
	  CALL ZWRITE(LL,LE,LINE)
C
C         QNO 356
	  WRITE(LINE,1003) XUNITS,'UNITS FOR X-AXIS'
	  CALL ZWRITE(LL,LE,LINE)
C
C         QNO 357
	  WRITE(LINE,1003) YTITLE,'Y-AXIS TITLE'
	  CALL ZWRITE(LL,LE,LINE)
C
C         QNO 358
	  WRITE(LINE,1003) YUNITS,'UNITS FOR Y-AXIS'
	  CALL ZWRITE(LL,LE,LINE)
C
      ENDIF
C
C      QNO 359
      WRITE(LINE,1003) ZTITLE,'Z-AXIS TITLE'
      CALL ZWRITE(LL,LE,LINE)
C
C      QNO 360
      WRITE(LINE,1003) ZUNITS,'UNITS FOR Z-AXIS'
      CALL ZWRITE(LL,LE,LINE)
C
C
C      QNO 361
C     Rotation parameter
C
      WRITE(LINE,1001) IPS(13)+1,'VIEWPOINT PARAMETER'
      CALL ZWRITE(LL,LE,LINE)
C
C      QNO 362
C     Default plot options.
C
      WRITE(LINE,1003) 'N',
     *       'DEFAULT PLOT OPTIONS FOR COLOUR AND HEIGHT?'
      CALL ZWRITE(LL,LE,LINE)
C
C      QNO 363
C     Contour plotting colour
C
      WRITE(LINE,1001) IPS(5),'ISOMETRIC PLOT COLOUR'
      CALL ZWRITE(LL,LE,LINE)
C
C      QNO 364
C     Height factor
C
      WRITE(LINE,1002) RPS(10),'HEIGHT FACTOR'
      CALL ZWRITE(LL,LE,LINE)
C
C
 1001 FORMAT(I6,T33,A)
 1002 FORMAT(G12.5,T33,A,I3)
 1003 FORMAT(A,T33,A)
C
C
      RETURN
C
C     *
C     *  END OF SUBROUTINE EQPSI
C     *
C
      END
C+*+* EQPSI  END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* EQPSC  START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE EQPSC
C
C     *************************************************************
C     *                                                           *
C     *   SUBROUTINE EQPSC                                        *
C     *                                                           *
C     *************************************************************
C
      IMPLICIT LOGICAL(A-Z)
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
C     SUBPROGRAM EQPSC  CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     EQPOL   ZWRITE
C
C     SUBPROGRAM EQPSC  IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     EQPS
C
C     LOCAL VARIABLES
C     ***************
      CHARACTER*80 LINE
      INTEGER LE
C
CCC   SET UP OUTPUT CHANNEL
C
      LE=IN(22)
C
C
      IF( IDD(8).EQ.1 ) THEN
C         abstract data -- need to specify X and Y titles
C         QNO 319
	  WRITE(LINE,1003) XTITLE,'X-AXIS TITLE'
	  CALL ZWRITE(LL,LE,LINE)
C
C         QNO 320
C
CCC       note Y axis title is stored in ZTITLE
C
	  WRITE(LINE,1003) ZTITLE,'Y-AXIS TITLE'
	  CALL ZWRITE(LL,LE,LINE)
C
      ENDIF
C
C      QNO 321
C     If data of LAT/LONG type
C
      IF (IDD(8).EQ.0) THEN
C
C     If coastline is switched on
C
	    IF (IPS(3).EQ.1) THEN
		WRITE(LINE,1003) 'Y','COASTLINE PLOT?'
		CALL ZWRITE(LL,LE,LINE)
C
C      QNO 322
C     Coastline file name
C
		WRITE(LINE,1003) NAMES(5),'NAME OF COASTLINE FILE'
		CALL ZWRITE(LL,LE,LINE)
C
C      QNO 323
C
		  IF (NAMES(5).EQ.NAMES(6)) THEN
			WRITE(LINE,1003) 'N','EXTRACT RELEVANT DATA?'
			CALL ZWRITE(LL,LE,LINE)
		  ELSE
			WRITE(LINE,1003) 'Y','EXTRACT RELEVANT DATA?'
			CALL ZWRITE(LL,LE,LINE)
C
C      QNO 324
C     New coastline file name
C
			WRITE(LINE,1003) NAMES(6),'NEW COASTLINE FILE'
			CALL ZWRITE(LL,LE,LINE)
C
C      QNO 325
C
		  ENDIF
	    ELSE
		  WRITE(LINE,1003) 'N','COASTLINE PLOT?'
		  CALL ZWRITE(LL,LE,LINE)
	    ENDIF
      ENDIF
C
C      QNO 327
C     If default plot options for pen colours, labelling ,etc
C
      IF (IPS(23).EQ.0) THEN
	 WRITE(LINE,1003) 'Y',
     *           'DEFAULT PLOT OPTIONS FOR COLOURS,LABELS,ETC?'
	 CALL ZWRITE(LL,LE,LINE)
      ELSE
	 WRITE(LINE,1003) 'N',
     *           'DEFAULT PLOT OPTIONS FOR COLOURS,LABELS,ETC?'
	 CALL ZWRITE(LL,LE,LINE)
C
C     If NAG-regular OR NAG-scattered OR Harwell
C
	    IF ((ICS(1).EQ.3).OR.(ICS(1).EQ.4).OR.(ICS(1).EQ.5)) THEN
C
C      QNO 328
C     Contour plotting colour
C
		  WRITE(LINE,1001) IPS(5),'CONTOUR COLOUR'
		  CALL ZWRITE(LL,LE,LINE)
C
C      QNO 329
C     If contour labelled
C
		  IF (IPS(6).EQ.1) THEN
			WRITE(LINE,1003) 'Y','CONTOURS LABELLED?'
			CALL ZWRITE(LL,LE,LINE)
		  ELSE
			WRITE(LINE,1003) 'N','CONTOURS LABELLED?'
			CALL ZWRITE(LL,LE,LINE)
		  ENDIF
	    ENDIF
C
C      QNO 330
C     If coastline switched on, write plotting colour
C
	    IF (IPS(3).EQ.1) THEN
	       WRITE(LINE,1001) IPS(4),'COLOUR FOR COASTLINE'
	     CALL ZWRITE(LL,LE,LINE)
	    ENDIF
C
C      QNO 331
C     Source data point switch
C
	    WRITE(LINE,1001) IPS(7),'SOURCE DATA POINT SWITCH'
	    CALL ZWRITE(LL,LE,LINE)
	    IF (IPS(7).NE.0) THEN
C
C      QNO 332
C     Colour for source data points
C
		  WRITE(LINE,1001) IPS(9),'COLOUR FOR SOURCE POINTS'
      CALL ZWRITE(LL,LE,LINE)
C
C      QNO 333
C     Symbol for source data points
C
		  WRITE(LINE,1001) IPS(8),'SYMBOL FOR SOURCE POINTS'
		  CALL ZWRITE(LL,LE,LINE)
	    ENDIF
C
C      QNO 334
C
	    WRITE(LINE,1001) IPS(16),'ADDITIONAL DATA POINTS SWITCH'
	    CALL ZWRITE(LL,LE,LINE)
C
C     If additional points are to be plotted
C
	    IF (IPS(16).NE.0) THEN
C
C      QNO 335
C     Name of file containing additional data points.
C
		  WRITE(LINE,1003) NAMES(7),'ADDITIONAL POINTS FILENAME'
		  CALL ZWRITE(LL,LE,LINE)
C
C      QNO 336
C     Colour for the additional data points
C
		  WRITE(LINE,1001) IPS(18),'COLOUR FOR ADDITIONAL PTS'
		  CALL ZWRITE(LL,LE,LINE)
C
C      QNO 337
C     Symbol for the additional data points
C
		  WRITE(LINE,1001) IPS(17),'SYMBOL FOR ADDITIONAL PTS'
		  CALL ZWRITE(LL,LE,LINE)
	    ENDIF
      ENDIF
C
C      QNO 338
C     If default plot options for tick marks and annotation
C
      IF (IPS(24).EQ.0) THEN
	    WRITE(LINE,1003) 'Y','DEFAULT ANNOTATION?'
	    CALL ZWRITE(LL,LE,LINE)
      ELSE
	    WRITE(LINE,1003) 'N','DEFAULT ANNOTATION?'
	    CALL ZWRITE(LL,LE,LINE)
C
C      QNO 339
C     If no tick marks
C
	    IF (IPS(12).EQ.0) THEN
		  WRITE(LINE,1003) 'N','TICKS?'
		  CALL ZWRITE(LL,LE,LINE)
	    ELSE
		  WRITE(LINE,1003) 'Y','TICKS?'
		  CALL ZWRITE(LL,LE,LINE)
C
C      QNO 340
C     Number of tick marks on x-axis
C
		  WRITE(LINE,1001) IPS(19),'NO. OF TICKS ON X-AXIS'
		  CALL ZWRITE(LL,LE,LINE)
C
C      QNO 341
C     Number of tick marks on y-axis
C
		  WRITE(LINE,1001) IPS(20),'NO. OF TICKS ON Y-AXIS'
		  CALL ZWRITE(LL,LE,LINE)
C
C      QNO 342
C     Tick mark annotation
C
		  WRITE(LINE,1001) IPS(11),'TICK ANNOTATION TYPE'
		  CALL ZWRITE(LL,LE,LINE)
	    ENDIF
      ENDIF
C
C      QNO 343
C     Polygon plotting options
C
      IF (IPS(14).EQ.0) THEN
	    WRITE(LINE,1003) 'N','POLYGON PLOTTING OPTION?'
	    CALL ZWRITE(LL,LE,LINE)
      ELSE
	    WRITE(LINE,1003) 'Y','POLYGON PLOTTING OPTION?'
	    CALL ZWRITE(LL,LE,LINE)
C
	    CALL EQPOL
C
      ENDIF
C
C      QNO 344
C     Mercator projection
C
      IF (IDD(8).EQ.0) THEN
	    IF (IPS(2).EQ.0) THEN
		  WRITE(LINE,1003) 'N','MERCATOR PROJECTION?'
		  CALL ZWRITE(LL,LE,LINE)
	    ELSE
		  WRITE(LINE,1003) 'Y','MERCATOR PROJECTION?'
		  CALL ZWRITE(LL,LE,LINE)
	    ENDIF
      ENDIF
C
C      QNO 345
C     Are the results of the centre of mass calculation
C     to be sent to the report file?
C
      IF(ICS(1).NE.7) THEN
	    IF (IN(12).EQ.1) THEN
		  WRITE(LINE,1003) 'Y','AREA & C OF M?'
		  CALL ZWRITE(LL,LE,LINE)
	    ELSE
		  WRITE(LINE,1003) 'N','AREA & C OF M?'
		  CALL ZWRITE(LL,LE,LINE)
	    ENDIF
      ENDIF
C
C
 1001 FORMAT(I6,T33,A)
 1003 FORMAT(A,T33,A)
C
C
      RETURN
C
C     *
C     *  END OF SUBROUTINE EQPSC
C     *
C
      END
C+*+* EQPSC  END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* EQPOL  START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE EQPOL
C
C     *************************************************************
C     *                                                           *
C     *   SUBROUTINE EQPOL                                        *
C     *                                                           *
C     *************************************************************
C
      IMPLICIT LOGICAL(A-Z)
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
C     SUBPROGRAM EQPOL  CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     ZWRITE
C
C     SUBPROGRAM EQPOL  IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     EQPSC
C
C     LOCAL VARIABLES
C     ***************
      CHARACTER*80 LINE
      INTEGER LE,II
C
CCC   SET UP OUTPUT CHANNEL
C
      LE=IN(22)
C
C      QNO 371
C     Polygon plotting switch
C
      WRITE(LINE,1001) IPS(14),'POLYGON PLOTTING SWITCH'
      CALL ZWRITE(LL,LE,LINE)
C
C      QNO 372
C     Number of vertices of the polygon
C
      WRITE(LINE,1001) ICS(3),'NUMBER OF POLYGON VERTICES'
      CALL ZWRITE(LL,LE,LINE)
C
      DO 20 II=1,ICS(3)
C
C      QNO 373
C
	    WRITE(LINE,1002) RCS(II+ICS(8)-1),'X-COORD OF VERTEX ',II
	    CALL ZWRITE(LL,LE,LINE)
C
C      QNO 374
C
	    WRITE(LINE,1002) RCS(II+ICS(9)-1),'Y-COORD OF VERTEX ',II
	    CALL ZWRITE(LL,LE,LINE)
C
   20 CONTINUE
C
C
 1001 FORMAT(I6,T33,A)
 1002 FORMAT(G12.5,T33,A,I3)
C
C
      RETURN
C
C     *
C     *   END OF SUBROUTINE EQPOL
C     *
C
      END
C+*+* EQPOL  END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
