C+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+
C+                                                                     +
C+                        FILE SSSU03.FOR                              +
C+                                                                     +
C+                   DRIVER FOR THE PLOTTER PROGRAM                    +
C+                                                                     +
C+       ROUTINES IN FILE ORDER             ALPHABETICAL ORDER         +
C+   HSU03  : LINES    13 TO   207    HSU03  : LINES    13 TO   207    +
C+                                                                     +
C+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+
C+*+* HSU03  START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      PROGRAM HSU03
C
C     ******************************************************************
C     *                                                                *
C     *    PROGRAM HSU03                                               *
C     *                                                                *
C     *            DRIVER FOR THE PLOTTING PROGRAM                     *
C     *                                                                *
C     ******************************************************************
C
      IMPLICIT LOGICAL (A-Z)
C
C     COMMON BLOCKS
C     *************
C     CHARS   CNTRL   PF
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
CCCC  PF COMMON BLOCK
C
      CHARACTER*32 PFLIST(10)
      COMMON /PF/ PFLIST
C
C     PROGRAM HSU03  CALLS THE FOLLOWING SUBROUTINES
C     **********************************************
C     ABORT   APPCN   CHKFN   HOST    INITCM  INQIRE  RSERR   SSU40
C     ZREAD
C
C     PROGRAM HSU03  REFERENCES THE FOLLOWING FUNCTIONS
C     *************************************************
      INTEGER IUNIT
C
C     LOCAL VARIABLES
C     ***************
      LOGICAL FIRST,OPEN,EXISTS
      CHARACTER*30 NAME
      CHARACTER LINE*80,ANS*1,MARK*25
      CHARACTER*30 PFNAME
      INTEGER FLAG
C
C     EQUIVALENCED VARIABLES
C     **********************
      LOGICAL BEOF,BATCH,EDITCF
      EQUIVALENCE (BEOF,LGSV(9)),(BATCH,LGSV(10)),(EDITCF,LGSV(11))
C
CCC   initialise control common
C
      FIRST=.TRUE.
      CALL INITCM(FIRST)
C
CCC   Specify host and location
C
      SALF=.TRUE.
      PC386=.TRUE.                                                      C-PC386
C     PRIME=.TRUE.                                                      C-PRIME
C     HP9000=.TRUE.                                                     C-HP9000
C     VAX=.TRUE.                                                        C-VAX
C
CCC   DETERMINE HOST-DEPENDENT PARAMETERS
C
      CALL HOST(MARK)
C
CCCC  Set flag to indicate which driver is being used
C
      IN(13)=2
C
CCC   set up necessary flags
C
      BATCH=.FALSE.
C
      WRITE(LL,2001) MARK
 2001 FORMAT(2X,'********************************************',
     *     / 2X,'*                                          *',
     *     / 2X,'*          UNIVERSITY OF SALFORD           *',
     *     / 2X,'*        DEPARTMENT OF MATHEMATICS         *',
     *     / 2X,'*                   AND                    *',
     *     / 2X,'*            COMPUTER  SCIENCE             *',
     *     / 2X,'*                                          *',
     *     / 2X,'*            PLOTTING  PROGRAM             *',
     *     / 2X,'*       FOR THE CONTOURING PACKAGE         *',
     *     / 2X,'*                  HSU02                   *',
     *     / 2X,'*                                          *',
     *     / 2X,'*        ',A,                    '         *',
     *     / 2X,'*                                          *',
     *     / 2X,'********************************************')
C
C
C     SET UP A REPORT FILE
C
      NAME='RPORT2'
      CALL APPCN(NAME)
C
CCC   CHECK THAT REPORT FILE IS NOT ALREADY OPEN
C
      CALL INQIRE(NAME,OPEN)
C
      IF (OPEN) THEN
	  WRITE(LL,'(//2X,''REPORT FILE IS ALREADY OPEN '')')
	  CALL ABORT
      ENDIF
C
      LR=IUNIT()
      OPEN(LR,FILE=NAME,STATUS='UNKNOWN')                               C-PC386
C     OPEN(LR,FILE=NAME,STATUS='UNKNOWN')                               C-PRIME
C     OPEN(LR,FILE=NAME,STATUS='UNKNOWN')                               C-HP9000
C     OPEN(LR,FILE=NAME,STATUS='NEW',CARRIAGECONTROL='LIST')            C-VAX
      IN(10)=1
      WRITE(LR,2001) MARK
C
   87 CONTINUE
C
  480 CONTINUE
      IF( QNO.EQ.480 ) CALL RSERR(FLAG)
      QNO=480
      WRITE(LL,2480)
 2480 FORMAT(//2X,'*** Q480 ***'/2X,'TYPE IN THE NAME OF THE ',
     *        'PLOTTING FILE',
     *        /'( OR PRESS RETURN FOR DEFAULT PFBTC )')
      CALL ZREAD(.TRUE.,LINE,FLAG,*480)
      READ(LINE,'(A)',ERR=480) PFNAME
      IF( PFNAME.EQ.' ' ) PFNAME='PFBTC'
      CALL CHKFN(PFNAME,*480)
C
      INQUIRE(FILE=PFNAME,EXIST=EXISTS)
      IF( .NOT.EXISTS ) THEN
	  WRITE(LL,2901) PFNAME
 2901     FORMAT(//'+++ PLOTTING FILE ',A,' DOES NOT EXIST +++')
	  GOTO 480
      ENDIF
C
CCC   set up PFLIST for a batch job
C
      PFLIST(1)(1:30)=PFNAME
      PFLIST(1)(32:32)='1'
      PFLIST(2)(1:30)='ZZZZZ'
      PFLIST(2)(32:32)='1'
C
      CALL SSU40
C
CCCC  CHECK THAT PLOTTING IS REQUIRED
C
  481 CONTINUE
      IF (QNO.EQ.481) CALL RSERR(FLAG)
      QNO=481
      WRITE (LL,1001) QNO
 1001 FORMAT (//' *** Q',I3,' ***')
      WRITE (LL,2481)
 2481 FORMAT (2X,'ANY MORE FILES FOR PLOTTING?')
      CALL ZREAD(.TRUE.,LINE,FLAG,*481)
      READ (LINE,'(A)') ANS
      IF (ANS.EQ.' ') ANS='Y'
      IF (ANS.EQ.'Y') THEN
	  GOTO 87
      ELSEIF (ANS.EQ.'N') THEN
	  CONTINUE
      ELSE
	  GOTO 481
      ENDIF
C
C
      WRITE(LL,2003)
      WRITE(LR,2003)
 2003 FORMAT(//2X,'*********  PROGRAM HSU03 FINISHED  *********'////)
C
      CLOSE(LR)
C
C     TERMINATE EXECUTION
      STOP '**** RUN COMPLETE ****'
C
C     **
C     **    END OF PROGRAM HSU03
C     **
C
      END
C+*+* HSU03  END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
 
