C+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+
C+                                                                     +
C+                        FILE SSSU02.FOR                              +
C+                                                                     +
C+     CONTAINS THE DRIVER FOR THE CONTROL FILE GENERATOR PROGRAM      +
C+                                                                     +
C+       ROUTINES IN FILE ORDER             ALPHABETICAL ORDER         +
C+   HSU02  : LINES    13 TO   220    HSU02  : LINES    13 TO   220    +
C+                                                                     +
C+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+
C+*+* HSU02  START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      PROGRAM HSU02
C
C     ******************************************************************
C     *                                                                *
C     *    PROGRAM HSU02                                               *
C     *                                                                *
C     *        DRIVER FOR THE CONTROL FILE GENERATION PROGRAM          *
C     *                                                                *
C     ******************************************************************
C
      IMPLICIT LOGICAL (A-Z)
C
C     COMMON BLOCKS
C     *************
C     CHARS   CNTRL
C
C-----INCLUDE STATEMENT FOR CNTRL COMMON BLOCKS ------------------------
C     INCLUDE 'H:\CONTOUR\MASTER\COMMON'
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
C     PROGRAM HSU02  CALLS THE FOLLOWING SUBROUTINES
C     **********************************************
C     ABORT   APPCN   HOST    INITCM  INQIRE  RSERR   SSU10   SSU12
C     TIDY    ZREAD
C
C     PROGRAM HSU02  REFERENCES THE FOLLOWING FUNCTIONS
C     *************************************************
      INTEGER IUNIT
C
C     LOCAL VARIABLES
C     ***************
      CHARACTER*30 NAME
      CHARACTER LINE*80,IFORM*7,MARK*25
      LOGICAL OPEN,FIRST
      INTEGER FLAG
C
C     EQUIVALENCED VARIABLES
C     **********************
      LOGICAL BEOF,BATCH,EDITCF
      EQUIVALENCE (BEOF,LGSV(9)),(BATCH,LGSV(10)),(EDITCF,LGSV(11))
C
C     DATA STATEMENTS
C     ***************
      DATA IFORM/'(BN,I6)'/
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
CCCC  Set flag to indicate which driver is being used
C
      IN(13)=1
C
CCC   DETERMINE HOST-DEPENDENT PARAMETERS FROM FILE NAMES(10)
C
      CALL HOST(MARK)
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
     *     / 2X,'*         CONTROL FILE GENERATOR           *',
     *     / 2X,'*       FOR THE CONTOURING PACKAGE         *',
     *     / 2X,'*                  HSU01                   *',
     *     / 2X,'*                                          *',
     *     / 2X,'*        ',A,                    '         *',
     *     / 2X,'*                                          *',
     *     / 2X,'********************************************')
C
C     SET UP A REPORT FILE (ON DEFAULT CARTRIDGE)
C
      NAME='RPORT1'
      CALL APPCN(NAME)
C
CCC   CHECK THAT REPORT FILE IS NOT ALREADY OPEN
C
      CALL INQIRE(NAME,OPEN)
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
C
      WRITE(LR,'(''1'')')
      WRITE(LR,2001) MARK
C
C
  107 CONTINUE
      IF( QNO.EQ.107 ) CALL RSERR(FLAG)
      QNO=107
      WRITE(LL,2107)
 2107 FORMAT(//2X,'*** Q107 ***'/2X,'DO YOU WISH TO ',
     *  //7X,'1  CREATE A NEW CONTROL FILE ? ',
     *   /7X,'2  EDIT AN EXISTING CONTROL FILE ? ',
     *   /7X,'3  TEST AN EXISTING CONTROL FILE ? ',
     *  //7X,'TYPE 1,2 OR 3')
      CALL ZREAD(.TRUE.,LINE,FLAG,*107)
      READ(LINE,IFORM,ERR=107) IN(26)
      IF( IN(26).LT.1 .OR. IN(26).GT.3 ) GOTO 107
C
C
      IF( IN(26).EQ.1 ) THEN
	  IN(1)=1
	  EDITCF=.FALSE.
      ELSEIF( IN(26).EQ.2 ) THEN
	  IN(1)=2
	  EDITCF=.TRUE.
	  WRITE(LL,2002)
 2002     FORMAT(//
     *   2X,'*********************************************************',
     * / 2X,'*                                                       *',
     * / 2X,'*      Edit commands available are :                    *',
     * / 2X,'*      =============================                    *',
     * / 2X,'*                                                       *',
     * / 2X,'*  Accept (A)  -- accept record from control file       *',
     * / 2X,'*                                                       *',
     * / 2X,'*  Change (C)  -- type in an alternative response       *',
     * / 2X,'*                                                       *',
     * / 2X,'*  Skip   (S)  -- skip record, obtain next record from  *',
     * / 2X,'*                 the control file                      *',
     * / 2X,'*                                                       *',
     * / 2X,'*  Insert (I)  -- type in an alternative response,      *',
     * / 2X,'*    retain record from control file for next question  *'
     * / 2X,'*                                                       *',
     * / 2X,'*  SWitch (SW) -- switch from edit mode and type in     *',
     * / 2X,'*    responses for all further questions                *',
     * / 2X,'*                                                       *',
     * / 2X,'*********************************************************')
      ELSE
	  IN(1)=2
	  EDITCF=.FALSE.
      ENDIF
C
CCC   CALL INTERACTIVE CONTROL FILE CREATION OR INPUT SUITE
C
      IDD(1)=0
      CALL SSU10
C
C
      IF( IN(26).LT.3 ) THEN
C         Interactive or edit mode
	  CALL SSU12
      ELSE
C         Test mode
	  WRITE(LL,2004)
	  WRITE(LR,2004)
 2004     FORMAT(//2X,'*********  CONTROL FILE IS  VALID  *********'//)
      ENDIF
C
      WRITE(LL,2003)
      WRITE(LR,2003)
 2003 FORMAT(//2X,'*********  PROGRAM HSU02 FINISHED  *********'////)
C
      CLOSE(LR)
C
C     DELETE ALL UNWANTED FILES
      CALL TIDY
C
C     TERMINATE EXECUTION
      STOP '**** RUN COMPLETE ****'
C
C     **
C     **    END OF PROGRAM HSU02
C     **
C
      END
C+*+* HSU02  END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
 
