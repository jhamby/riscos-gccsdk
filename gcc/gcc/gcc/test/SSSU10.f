C+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+
C+                                                                     +
C+                        FILE SSSU10.FOR                              +
C+                                                                     +
C+     DRIVER FOR INITIAL USER INTERFACE AND AUXILIARY SUBPROGRAMS     +
C+                                                                     +
C+       ROUTINES IN FILE ORDER             ALPHABETICAL ORDER         +
C+   SSU10  : LINES    30 TO   276    ANTCLK : LINES  3693 TO  3790    +
C+   BTCH   : LINES   277 TO   431    BTCH   : LINES   277 TO   431    +
C+   RPEAT  : LINES   432 TO   580    CSTLN  : LINES  4194 TO  4417    +
C+   RPFIND : LINES   581 TO   724    PLYCRS : LINES  3891 TO  3996    +
C+   QDD    : LINES   725 TO   985    PLYOK  : LINES  3791 TO  3890    +
C+   REFMT  : LINES   986 TO  1428    QCS    : LINES  1429 TO  1836    +
C+   QCS    : LINES  1429 TO  1836    QDD    : LINES   725 TO   985    +
C+   TRANS  : LINES  1837 TO  2036    QKN    : LINES  2037 TO  2306    +
C+   QKN    : LINES  2037 TO  2306    QPOLY  : LINES  3521 TO  3692    +
C+   QPS    : LINES  2307 TO  2580    QPS    : LINES  2307 TO  2580    +
C+   QPSC   : LINES  2581 TO  3286    QPSC   : LINES  2581 TO  3286    +
C+   QPSI   : LINES  3287 TO  3520    QPSI   : LINES  3287 TO  3520    +
C+   QPOLY  : LINES  3521 TO  3692    REFMT  : LINES   986 TO  1428    +
C+   ANTCLK : LINES  3693 TO  3790    RPEAT  : LINES   432 TO   580    +
C+   PLYOK  : LINES  3791 TO  3890    RPFIND : LINES   581 TO   724    +
C+   PLYCRS : LINES  3891 TO  3996    SCAN   : LINES  3997 TO  4193    +
C+   SCAN   : LINES  3997 TO  4193    SSU10  : LINES    30 TO   276    +
C+   CSTLN  : LINES  4194 TO  4417    TRANS  : LINES  1837 TO  2036    +
C+                                                                     +
C+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+
C+*+* SSU10  START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE SSU10
C
C     ******************************************************************
C     *                                                                *
C     *    SUBROUTINE SSU10                                            *
C     *                                                                *
C     *        DRIVER FOR EITHER                                       *
C     *           (a) INTERACTIVE CREATION OF CONTROL COMMON           *
C     *           (b) INPUT OF CONTROL FILE FROM DISC                  *
C     *           (c) INPUT OF CONTROL FILE FROM PAPER TAPE            *
C     *                                                                *
C     *          +++ HOST DEPENDENT SUBROUTINE +++                     *
C     *                                                                *
C     ******************************************************************
C
      IMPLICIT LOGICAL (A-Z)
C
C     COMMON BLOCKS
C     *************
C     CHARS   CNTRL   MODS
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
CCC   MODS COMMON BLOCK
C
      CHARACTER*80 MDLINE(5)
      COMMON /MODS/ MDLINE
C
C     SUBPROGRAM SSU10  CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     BTCH    CHKFN   QCS     QDD     QPS     REFMT   RPEAT   RSERR
C     ZREAD
C
C     SUBPROGRAM SSU10  REFERENCES THE FOLLOWING FUNCTIONS
C     ****************************************************
      INTEGER IUNIT
C
C     SUBPROGRAM SSU10  IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     HSU01   HSU02
C
C     LOCAL VARIABLES
C     ***************
      LOGICAL EXISTS,STEP
      CHARACTER LINE*80,IFORM*7
      INTEGER FLAG
C
C     EQUIVALENCED VARIABLES
C     **********************
      LOGICAL REPEAT,BEOF,BATCH
      EQUIVALENCE (REPEAT,LGSV(8)),(BEOF,LGSV(9)),(BATCH,LGSV(10))
     *           ,(STEP,LGSV(11))
C
C     ******************************************************************
C     *                                                                *
C     *         QNO = QUESTION NO                                      *
C     *         ANS = REPLY TO YES/NO TYPE QUESTION                    *
C     *                                                                *
C     *     NOTE                                                       *
C     *         'Y' = YES        'N' = NO                              *
C     *                                                                *
C     ******************************************************************
C
      IFORM='(BN,I6)'
C
C
C
      IF( IN(1).EQ.1 ) THEN
C         CREATE CONTROL COMMON INTERACTIVELY
C
	  WRITE(LL,2000) '  CONTROL COMMON CREATED INTERACTIVELY'
	  WRITE(LR,2000) '  CONTROL COMMON CREATED INTERACTIVELY'
 2000     FORMAT(//
     *    /2X,'********************************************',
     *    /2X,'*                                          *',
     *    /2X,'*             SUITE SSSU10                 *',
     *    /2X,'*                                          *',
     *    /2X,'* ',                    A38           ,'   *',
     *    /2X,'*                                          *',
     *    /2X,'********************************************')
C
C
      ELSEIF( IN(1).EQ.2 ) THEN
C         CONTROL FILE(S) ON DISC -- SET FLAG TO INDICATE NON-INTERACTIVE
C
C
C
	  IF( .NOT.BATCH ) THEN
C             SINGLE JOB MODE
C
C
C
  104       CONTINUE
	    IF( QNO.EQ.104 ) CALL RSERR(FLAG)
	    QNO=104
	    WRITE(LL,2104)
 2104       FORMAT(//2X,'*** Q104 ***'/
     *      2X,'TYPE IN THE NAME OF THE CONTROL FILE')
	    CALL ZREAD(.TRUE.,LINE,FLAG,*104)
	    READ(LINE,'(A)',ERR=104) NAMES(9)
	    CALL CHKFN(NAMES(9),*104)
	    INQUIRE(FILE=NAMES(9),EXIST=EXISTS,ERR=104)
	    IF( .NOT.EXISTS ) THEN
		WRITE(LL,2901) NAMES(9)
		GOTO 104
	    ENDIF
C
C
C
C
C
	  ELSEIF( BATCH ) THEN
C             BATCH MODE
C
	      IF( REPEAT ) THEN
C
CCC               If current c.f. is tabular, attempt to set up
CCC               'repeat' c.f. using '**REPEAT ' modifications.
CCC               If c.f. exhausted or unexpected error occurs,
CCC               REPEAT will be set false.
C
		  CALL RPEAT
C
	      ENDIF
C
C
	      IF( .NOT. REPEAT ) THEN
C
CCC               need to find next control file from batch file
CCC               --  no more REPEATs in current c.f.
C
		  CALL BTCH
C
CCC               if BATCH file exhausted, return
C
		  IF( BEOF ) RETURN
C
	      ENDIF
C
C
C
	  ENDIF
C
CCC       set non-interactive flag
C
	  IN(15)=1
C
	  WRITE(LL,2001) NAMES(9)
	  WRITE(LR,2001) NAMES(9)
 2001     FORMAT(//
     *    /2X,'********************************************',
     *    /2X,'*                                          *',
     *    /2X,'*             SUITE SSSU10                 *',
     *    /2X,'*                                          *',
     *    /2X,'* CONTROL COMMON CREATED FROM CONTROL FILE *',
     *    /2X,'*                                          *',
     *    /2X,'* ',A,                         '           *',
     *    /2X,'*                                          *',
     *    /2X,'********************************************')
C
CCC       OPEN CONTROL FILE
C
	  LU=IUNIT()
	  OPEN(LU,FILE=NAMES(9),STATUS='OLD')
C         OPEN(LU,FILE=NAMES(9),STATUS='OLD')                           C-PRIME
C         OPEN(LU,FILE=NAMES(9),STATUS='OLD')                           C-HP9000
C         OPEN(LU,FILE=NAMES(9),STATUS='OLD',READONLY,SHARED)           C-VAX
C
C
C
      ENDIF
C
C
C     ASK THE QUESTIONS , DIVERT INTERACTIVE QUERIES TO A SCRATCH
C     FILE FOR INPUT OF CONTROL FILE FROM DISC OR PAPER TAPE
C     ( unless in STEP mode from SSSU01 )
C
      IF( IN(15).GT.0 .AND. ( .NOT. STEP ) ) THEN
	  LL=IUNIT()
	  OPEN(LL,STATUS='SCRATCH')
      ENDIF
C
      CALL QDD
      CALL REFMT
      CALL QCS
      CALL QPS
C
C
C
      IF( IN(15).GT.0 ) THEN
C         CLOSE UNIT AND RESET LU AFTER INPUT FROM EXISTING
C         CONTROL FILE
	  CLOSE(LU)
	  LU=IN(2)-(IN(2)/256)*256
	  IF( .NOT. STEP ) THEN
	      CLOSE(LL)
	      LL=IN(2)/256
	  ENDIF
C
CCC       IF NOT BATCH, RESET INTERACTIVE FLAG
C
	  IF( .NOT. BATCH ) IN(15)=0
      ENDIF
C
C
C
      IF( .NOT. BATCH ) THEN
C         NOT BATCH MODE
	  WRITE(LL,2003)
	  WRITE(LR,2003)
      ENDIF
 2003 FORMAT(//2X,'*********  SUITE  SSSU10 FINISHED  *********'////)
C
C
      RETURN
C
C
C
 2901 FORMAT(//'+++ CONTROL FILE ',A,' DOES NOT EXIST +++')
 
C
C     **
C     **    END OF SUBROUTINE SSU10
C     **
C
      END
C+*+* SSU10  END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* BTCH   START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE BTCH
C
C     ******************************************************************
C     *                                                                *
C     *    SUBROUTINE BTCH                                             *
C     *                                                                *
C     *                                                                *
C     *                                                                *
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
C     SUBPROGRAM BTCH   CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     ABORT   CHKFN1
C
C     SUBPROGRAM BTCH   REFERENCES THE FOLLOWING FUNCTIONS
C     ****************************************************
      INTEGER IUNIT
C
C     SUBPROGRAM BTCH   IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     SSU10
C
C     LOCAL VARIABLES
C     ***************
      LOGICAL EXISTS
      CHARACTER IFORM*7
      INTEGER LB,I,IOSTAT
C
C     EQUIVALENCED VARIABLES
C     **********************
      LOGICAL BEOF,FLGV
      EQUIVALENCE (BEOF,LGSV(9))
C
C
      IFORM='(BN,I6)'
C
C
C
C
C
C
CCC   660 IS A LOOP-BACK POINT IN CASE OF ERRORS IN BATCH FILE
C
  660 CONTINUE
C
CCC   OPEN BATCH FILE AND FIND NAME OF NEXT CONTROL FILE
C
      LB=IUNIT()
      OPEN(LB,FILE=NAMES(11),STATUS='OLD',ERR=771,IOSTAT=IOSTAT)
C     OPEN(LB,FILE=NAMES(11),STATUS='OLD',ERR=771,IOSTAT=IOSTAT)        C-PRIME
C     OPEN(LB,FILE=NAMES(11),STATUS='OLD',ERR=771,IOSTAT=IOSTAT)        C-HP9000
C     OPEN(LB,FILE=NAMES(11),STATUS='OLD',ERR=771,IOSTAT=IOSTAT,        C-VAX
C    *        READONLY,SHARED)                                          C-VAX
C
C
      DO 10 I=1,IN(25)+1
	  READ(LB,'(A)',END=999,ERR=772,IOSTAT=IOSTAT) NAMES(9)
   10 CONTINUE
C
C
      CLOSE(LB)
C
CCC   INCREMENT C.F. COUNTER
C
      IN(25)=IN(25)+1
C
      IF( NAMES(9)(1:1).EQ.'#' ) THEN
C        line commented out in batch file, loop back
	 GOTO 660
      ENDIF
C
C
      CALL CHKFN1(NAMES(9),FLGV)
C
C
      IF( .NOT. FLGV ) THEN
	  WRITE(LR,2902) NAMES(9)
C         LOOP BACK AND READ NEXT C.F. NAME
	  GOTO 660
      ENDIF
C
C
      INQUIRE(FILE=NAMES(9),EXIST=EXISTS,ERR=773, IOSTAT=IOSTAT)
      IF( .NOT.EXISTS ) THEN
	  WRITE(LR,2901) NAMES(9)
C         LOOP BACK AND READ NEXT C.F. NAME
	  GOTO 660
      ENDIF
C
      RETURN
C
  999 CONTINUE
C
CCC   BATCH FILE EXHAUSTED, SET FLAG BEOF
C
      BEOF=.TRUE.
C
      RETURN
C
 771  CONTINUE
      WRITE(LL,'(//2X,A)') 'Passing label 771'
      WRITE(LR,'(//2X,A)') 'Passing label 771'
 772  CONTINUE
      WRITE(LL,'(//2X,A)') 'Passing label 772'
      WRITE(LR,'(//2X,A)') 'Passing label 772'
 773  CONTINUE
      WRITE(LL,'(//2X,A)') 'Passing label 773'
      WRITE(LR,'(//2X,A)') 'Passing label 773'
C     temporary code
      WRITE(LL,2777) IOSTAT
      WRITE(LR,2777) IOSTAT
 2777 FORMAT(//2X,'UNEXPECTED ERROR -- IOSTAT = ',I4,
     *  / '**** ERROR IN BTCH ****')
      CALL ABORT
C
C
 2901 FORMAT('1',//2X,'+++ CONTROL FILE ',A,' DOES NOT EXIST +++')
 2902 FORMAT('1',//2X,'+++ CONTROL FILE NAME ',A,' IS INVALID +++')
 
C
C     **
C     **    END OF SUBROUTINE BTCH
C     **
C
      END
C+*+* BTCH   END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* RPEAT  START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE RPEAT
C
C     ******************************************************************
C     *                                                                *
C     *   THIS SUBROUTINE SETS UP APPROPRIATE CONTROL FILES FOR        *
C     *   EACH INDIVIDUAL 'JOB' WHEN WORKING IN REPEAT MODE            *
C     *                                                                *
C     ******************************************************************
C
      IMPLICIT LOGICAL (A-Z)
C
C     COMMON BLOCKS
C     *************
C     CHARS   CNTRL   MODS
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
CCC   MODS COMMON BLOCK
C
      CHARACTER*80 MDLINE(5)
      COMMON /MODS/ MDLINE
C
C     SUBPROGRAM RPEAT  CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     ABORT   RPFIND  ZREAD
C
C     SUBPROGRAM RPEAT  REFERENCES THE FOLLOWING FUNCTIONS
C     ****************************************************
      INTEGER IUNIT
C
C     SUBPROGRAM RPEAT  IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     SSU10
C
C     LOCAL VARIABLES
C     ***************
      INTEGER FLAG,I,I1,I2,NMODS
C
C     EQUIVALENCED VARIABLES
C     **********************
      LOGICAL FIRST,REPEAT
      EQUIVALENCE (FIRST,LGSV(3)),(REPEAT,LGSV(8))
C
C
C
C
CCC   open current c.f. on unit LU
C
      LU=IUNIT()
      OPEN(LU,FILE=NAMES(9),STATUS='OLD')
C
CCC   find occurrences of next two **REPEATs
C
      I1=IN(16)
      CALL RPFIND(I1,I2)
      IF( .NOT. REPEAT ) THEN
	 CLOSE(LU)
	 RETURN
      ENDIF
      IN(16)=I2
C
CCC   check that the minimum modifications are present
C
      NMODS=I2-I1-1
      IF( NMODS.LT.3 ) GOTO 991
C
CCC   only consider first 5 modification lines
C
      IF( NMODS.GT.5 ) NMODS=5
C
      REWIND LU
C
      DO 10 I=1,I1
C         Skip records, down to REPEAT mods
	  READ(LU,'(1X)',END=990)
   10 CONTINUE
C
CCC   possible modifications are :
CCC          1    new datafile name
CCC          2    column no
CCC          3    column title
CCC          4    scattered datafile name (optional)
CCC          5    gridded datafile name   (optional)
CCC
CCC
CCC   set up 'defaults' for optional 4 and 5 (see below)
C
      MDLINE(4)=NAMES(2)
      MDLINE(5)=NAMES(3)
C
CCC   now read modifications :
C
      DO 20 I=1,NMODS
	  CALL ZREAD(.TRUE.,MDLINE(I),FLAG,*990)
   20 CONTINUE
C
C
      CLOSE(LU)
C
C
      RETURN
C
  990 CONTINUE
C
C
      WRITE(LL,2990)
      WRITE(LR,2990)
 2990 FORMAT(//2X,'++++ UNEXPECTED ERROR IN SUBR RPEAT WHEN READING ',
     *           'A RECORD FROM CONTROL FILE ++++')
C     force abandonment of REPEAT mode for current c.f.
      REPEAT=.FALSE.
      RETURN
C
  991 CONTINUE
C
C
      WRITE(LL,2991)
      WRITE(LR,2991)
 2991 FORMAT(//2X,'++++  ERROR IN SUBR RPEAT WHEN READING ',
     *           'MODIFICATIONS FROM CONTROL FILE ++++')
C     calling ABORT will allow REPEAT mode to continue
      CALL ABORT
C
C     **
C     **    END OF SUBROUTINE RPEAT
C     **
C
      END
C+*+* RPEAT  END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* RPFIND START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE RPFIND(I1,I2)
C
C     ******************************************************************
C     *                                                                *
C     *    SUBROUTINE RPFIND                                           *
C     *      I1 = first  occurrence of **REPEAT                        *
C     *      I2 = second occurrence  "    "     or EOF                 *
C     *                                                                *
C     ******************************************************************
C
      IMPLICIT LOGICAL (A-Z)
C
C     ARGUMENTS
C     *********
      INTEGER I1,I2
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
C     SUBPROGRAM RPFIND IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     RPEAT
C
C     LOCAL VARIABLES
C     ***************
      CHARACTER LINE*80,CH8*8
      INTEGER I
C
C     EQUIVALENCED VARIABLES
C     **********************
      LOGICAL FIRST,REPEAT
      EQUIVALENCE (FIRST,LGSV(3)),(REPEAT,LGSV(8))
C
CCC   assumes current c.f. is already open on unit LU
C
      IF( FIRST ) I1=0
C
C
      DO 10 I=1,I1
C         skip past first '**REPEAT'
	  READ(LU,'(1X)',END=991,ERR=990)
   10 CONTINUE
C
C
      DO 20 I=I1+1,1000
C     ALLOWS FOR REPEAT OF ABOUT 300
C
CCC       find next '**REPEAT'
C
	  READ(LU,'(A)',END=992,ERR=990) LINE
C
C
	  CH8=LINE(1:8)
	  IF( CH8.EQ.'**REPEAT' .OR. CH8.EQ.'** REPEA' ) THEN
C             found !
	      IF( FIRST ) THEN
		  I1=I
		  FIRST=.FALSE.
	      ELSE
		  I2=I
		  RETURN
	      ENDIF
	  ENDIF
C
   20 CONTINUE
C
C
      WRITE(LL,2999)
      WRITE(LR,2999)
 2999 FORMAT(//2X,'++++ DO 10 LOOP IN SUBR RPFIND NOT ',
     *                         'LARGE ENOUGH ++++')
C     force abandonment of REPEAT mode for current c.f.
      REPEAT=.FALSE.
      RETURN
C
C
  990 CONTINUE
C
C
      WRITE(LL,2995)
      WRITE(LR,2995)
 2995 FORMAT(//2X,'++++ UNEXPECTED ERROR IN SUBR RPFIND WHEN READING ',
     *           'A RECORD FROM CONTROL FILE ++++')
C     force abandonment of REPEAT mode for current c.f.
      REPEAT=.FALSE.
      RETURN
C
  991 CONTINUE
C
CCC   end-of-file in 10 loop -- file exhausted
C
      REPEAT=.FALSE.
C
      RETURN
C
C
  992 CONTINUE
C
CCC   end-of-file in 20 loop
C
      IF( FIRST ) THEN
C         no **REPEATs at all
	  REPEAT=.FALSE.
      ELSEIF( I.EQ.I1+1 ) THEN
C         file exhausted
	  REPEAT=.FALSE.
      ELSE
C         treat as last **REPEAT
	  I2=I
      ENDIF
C
C
      RETURN
C
C
C     **
C     **    END OF SUBROUTINE RPFIND
C     **
C
      END
C+*+* RPFIND END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* QDD    START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE QDD
C
C     ******************************************************************
C     *                                                                *
C     *   THIS SUBROUTINE ASKS QUESTIONS ABOUT THE SOURCE DATA         *
C     *                                                                *
C     ******************************************************************
C
      IMPLICIT LOGICAL (A-Z)
C
C     COMMON BLOCKS
C     *************
C     CHARS   CNTRL   MODS
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
CCC   MODS COMMON BLOCK
C
      CHARACTER*80 MDLINE(5)
      COMMON /MODS/ MDLINE
C
C     SUBPROGRAM QDD    CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     ABORT   CHKFN   RSERR   ZREAD
C
C     SUBPROGRAM QDD    REFERENCES THE FOLLOWING FUNCTIONS
C     ****************************************************
      INTEGER IUNIT
C
C     SUBPROGRAM QDD    IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     SSU10
C
C     LOCAL VARIABLES
C     ***************
      CHARACTER ANS,LINE*80,IFORM*7
      LOGICAL EXISTS
      INTEGER FLAG,LI,NPAR,IOS
C
C     EQUIVALENCED VARIABLES
C     **********************
      LOGICAL REPEAT
      EQUIVALENCE (REPEAT,LGSV(8))
C
      IFORM='(BN,I6)'
C
C
C
      WRITE(LL,2000)
 2000 FORMAT(///2X,'**************************************'/
     *          2X,'* QUESTIONS RELATED TO THE DATA FILE *'/
     *          2X,'**************************************'/)
C
C
C
  201 CONTINUE
      IF( QNO.EQ.201 ) CALL RSERR(FLAG)
      QNO=201
      WRITE(LL,2201)
 2201 FORMAT(//2X,'*** Q201 ***',/2X,'TYPE IN THE NAME OF THE ',
     *            'DATA FILE')
      CALL ZREAD(.TRUE.,LINE,FLAG,*201)
C
CCC   special REPEAT modification
C
      IF( REPEAT ) LINE=MDLINE(1)
C
      READ(LINE,'(A)',ERR=201) NAMES(1)
      CALL CHKFN(NAMES(1),*201)
      INQUIRE(FILE=NAMES(1),EXIST=EXISTS,IOSTAT=IOS)
      IF( IOS.GT.0 ) THEN
C         error occurred
	  WRITE(LL,2903) IOS
 2903     FORMAT(//2X,'+++ ERROR IN INQUIRE +++',
     *             /2X,'+++ IOSTAT = ',I6,'  +++')
	  GOTO 201
      ENDIF
      IF( .NOT.EXISTS ) THEN
	  WRITE(LL,2901)
 2901     FORMAT(//2X,'+++ FILE DOES NOT EXIST +++')
	  GOTO 201
      ENDIF
C
C
C
  202 CONTINUE
      IF( QNO.EQ.202 ) CALL RSERR(FLAG)
      QNO=202
      WRITE(LL,2202)
 2202 FORMAT(//2X,'*** Q202 ***'/2X,'IS THE DATA'/
     *   /7X,'1  REGULAR GRIDDED?'
     *   /7X,'2  SCATTERED?  '
     *   /7X,'3  TABULAR?    '
     *   //7X,'TYPE 1,2 OR 3')
      CALL ZREAD(.TRUE.,LINE,FLAG,*202)
      READ(LINE,IFORM,ERR=202) IDD(1)
      IF( IDD(1).LT.1 .OR. IDD(1).GT.3 ) GOTO 202
C
C
C
      IF( IDD(1).EQ.1 ) THEN
C         *** REGULAR DATA  ***
	  NAMES(3)=NAMES(1)
	  IDD(3)=1
	  RETURN
      ENDIF
C
C
C
      IF( IDD(1).EQ.3 ) THEN
C         *** TABULAR DATA ***
	  IDD(3)=2
C
CCC       FIND OUT HOW MANY COLUMNS THERE ARE
C
	  LI=IUNIT()
	  OPEN(LI,FILE=NAMES(1),STATUS='OLD')
C         OPEN(LI,FILE=NAMES(1),STATUS='OLD')                           C-PRIME
C         OPEN(LI,FILE=NAMES(1),STATUS='OLD')                           C-HP9000
C         OPEN(LI,FILE=NAMES(1),STATUS='OLD',READONLY,SHARED)           C-VAX
	  READ(LI,*,ERR=999)
	  READ(LI,*,ERR=999) NPAR
	  CLOSE(LI)
C
C
C
  203     CONTINUE
	  IF( QNO.EQ.203 ) CALL RSERR(FLAG)
	  QNO=203
	  WRITE(LL,2203) NPAR
 2203     FORMAT(//2X,'*** Q203 ***'/2X,'WHICH COLUMN OF THE TABULAR',
     *            ' DATA DO YOU WISH TO CONTOUR?',
     *            /7X,'( THE TOTAL NUMBER OF COLUMNS IS ',I2,' )',
     *            /7X,'TYPE THE COLUMN NUMBER')
	  CALL ZREAD(.TRUE.,LINE,FLAG,*203)
C
CCC       special REPEAT modification
C
	  IF( REPEAT ) LINE=MDLINE(2)
C
	  READ(LINE,IFORM,ERR=203) IDD(2)
	  IF( IDD(2).LT.1 ) GOTO 203
	  IF( IDD(2).GT.NPAR ) THEN
	      WRITE(LL,2902)
 2902         FORMAT(/2X,'COLUMN NUMBER IS TOO LARGE')
	      QNO=0
	      GOTO 203
	  ENDIF
C
  204     CONTINUE
	  IF( QNO.EQ.204 ) CALL RSERR(FLAG)
	  QNO=204
	  WRITE(LL,2204)
 2204     FORMAT(//2X,'*** Q204 ***',/2X,'TYPE IN THE NAME OF THE',
     *            ' COLUMN PARAMETER YOU ARE USING')
	  CALL ZREAD(.FALSE.,LINE,FLAG,*204)
C
CCC       special REPEAT modification
C
	  IF( REPEAT ) LINE=MDLINE(3)
C
	  READ(LINE,'(A)',ERR=204) TITLE(51:80)
      ENDIF
C
C
      IF( IDD(1).EQ.2 ) THEN
C         SCATTERED DATA
C
  205     CONTINUE
	  IF( QNO.EQ.205 ) CALL RSERR(FLAG)
	  QNO=205
	  WRITE(LL,2205) NAMES(1)
 2205     FORMAT(//2X,'*** Q205 ***'/2X,'IS THE SCATTERED DATA FILE ',
     *          A,'IN THE STANDARD FORMAT?')
	  CALL ZREAD(.TRUE.,LINE,FLAG,*205)
	  READ(LINE,'(A)') ANS
	  IF( ANS.EQ.' ' ) ANS='Y'
	  IF( ANS.EQ.'Y' ) THEN
	      IDD(3)=1
	      RETURN
	  ELSEIF( ANS.EQ.'N' ) THEN
	      IDD(3)=2
	  ELSE
	      GOTO 205
	  ENDIF
      ENDIF
C
C
C
  206 CONTINUE
      IF( QNO.EQ.206 ) CALL RSERR(FLAG)
      QNO=206
      WRITE(LL,2206)
 2206 FORMAT(//2X,'*** Q206 ***'/2X,'IS THE LAT/LONG SPECIFIED IN '/
     *   /7X,'1  DECIMAL DEGREES?'
     *   /7X,'2  DEGREES - DECIMAL MINUTES?  '
     *   /7X,'3  DEGREES - MINUTES - SECONDS?'
     *   //7X,'TYPE 1,2 OR 3')
      CALL ZREAD(.TRUE.,LINE,FLAG,*206)
      READ(LINE,IFORM,ERR=206) IDD(4)
      IF( IDD(4).LT.1 .OR. IDD(4).GT.3 ) GOTO 206
C
C
C
  207 CONTINUE
      IF( QNO.EQ.207 ) CALL RSERR(FLAG)
      QNO=207
      WRITE(LL,2207)
 2207 FORMAT(//2X,'*** Q207 ***'/
     *        2X,'HOW IS NORTH/SOUTH AND EAST/WEST SPECIFIED?'/
     *       /7X,'1  USING ''0'' = N AND E , ''1'' = S AND W?  '
     *       /7X,'2  USING ''+'' = N AND E , ''-'' = S AND W?  '
     *       /7X,'3  USING N,S,E,W?'
     *       //7X,'TYPE 1,2 OR 3')
      CALL ZREAD(.TRUE.,LINE,FLAG,*207)
      READ(LINE,IFORM,ERR=207) IDD(5)
      IF( IDD(5).LT.1 .OR. IDD(5).GT.3 ) GOTO 207
C
C
C
      RETURN
C
C
C
  999 CONTINUE
C
C     ERROR DETECTED
C
      WRITE(LL,2999)
      WRITE(LR,2999)
 2999 FORMAT(//'+++ FATAL ERROR DETECTED +++',/
     *    ' IN SUBROUTINE QDD -- WHILE DETERMINING ',
     *    '<NPAR> FROM DATA-FILE')
      CALL ABORT
C
C     **
C     **    END OF SUBROUTINE QDD
C     **
C
      END
C+*+* QDD    END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* REFMT  START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE REFMT
C
C     ******************************************************************
C     *                                                                *
C     *   THIS SUBROUTINE READS IN THE SOURCE DATA IN ONE OF THE       *
C     *   MANY POSSIBLE FORMATS                                        *
C     *                                                                *
C     ******************************************************************
C
      IMPLICIT LOGICAL (A-Z)
C
C     COMMON BLOCKS
C     *************
C     CHARS   CNTRL   MODS
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
CCC   MODS COMMON BLOCK
C
      CHARACTER*80 MDLINE(5)
      COMMON /MODS/ MDLINE
C
C     SUBPROGRAM REFMT  CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     ABORT   APPCN   CHKFN   FLNAM   RSERR   ZREAD
C
C     SUBPROGRAM REFMT  REFERENCES THE FOLLOWING FUNCTIONS
C     ****************************************************
      INTEGER ISGN,IUNIT
C
C     SUBPROGRAM REFMT  IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     SSU10
C
C     LOCAL VARIABLES
C     ***************
      CHARACTER ANS,CHAR,LINE*80,LINE1*84,IFORM*7,CLAT,CLONG
      INTEGER NS,EW,FLAG,LKI,LKO,IFST,ICOUNT,NCOL,II,NPAR,ISTAT,ILAT,
     *   ILAT1,ILAT2,ILAT3,ILONG,ILONG1,ILONG2,ILONG3,I,I1,LS
      REAL*8 RLAT,RLONG,X,Y,VALUE(50)
C
C     EQUIVALENCED VARIABLES
C     **********************
      LOGICAL REPEAT
      EQUIVALENCE (REPEAT,LGSV(8))
C
C     DATA STATEMENT
C     **************
C
      IFORM='(BN,I6)'
C
C
C
      IF( IDD(3).EQ.1 ) THEN
C         SCATTERED X-Y  OR  REGULAR IN STANDARD FORMAT
C         NO NEED TO REFORMAT THE DATA
	  NAMES(2)=NAMES(1)
C
C
  211     CONTINUE
	  IF( QNO.EQ.211 ) CALL RSERR(FLAG)
	  QNO=211
	  WRITE(LL,2211)
 2211     FORMAT(//2X,'*** Q211 ***'/2X,'DO THE X-Y COORDINATES IN',
     *         ' YOUR STANDARD FORMAT DATAFILE ',
     *         /2X,'CORRESPOND TO LONGITUDE/LATITUDE?')
	  CALL ZREAD(.TRUE.,LINE,FLAG,*211)
	  READ(LINE,'(A)') ANS
	  IF( ANS.EQ.'Y' ) ANS='Y'
	  IF( ANS.EQ.'Y' ) THEN
	      IDD(8)=0
	  ELSEIF( ANS.EQ.'N' ) THEN
	      IDD(8)=1
	  ELSE
	      GOTO 211
	  ENDIF
C
C
C
	  RETURN
C
      ELSE
C         MUST BE LAT/LONG
	  IDD(8)=0
C
      ENDIF
C
C
  212 CONTINUE
      IF( QNO.EQ.212 ) CALL RSERR(FLAG)
      QNO=212
      WRITE(LL,2212) NAMES(1)
 2212 FORMAT(//2X,'A SCATTERED, X-Y TYPE DATA FILE WILL BE ',
     *    'CREATED FROM < ',A,' >')
      WRITE(LL,4212)
 4212 FORMAT(/2X,'*** Q212 ***',/2X,
     *   'DO YOU WISH TO SAVE THE FILE TO USE AGAIN?')
      CALL ZREAD(.TRUE.,LINE,FLAG,*212)
      READ(LINE,'(A)') ANS
      IF( ANS.EQ.' ' ) ANS='Y'
      IF( ANS.EQ.'Y' ) THEN
C
CCC       ASK FOR THE NAME OF THE FILE
C
	  CALL FLNAM(NAMES(2),1)
C
CCC       special REPEAT modification
C
	  IF( REPEAT ) THEN
	      NAMES(2)=MDLINE(4)
	      CALL CHKFN(NAMES(2),*212)
C             If filename error, control passed first to RSERR, to
C             produce some diagnostic info, then ABORT.
	  ENDIF
C
C
C
      ELSEIF( ANS.EQ.'N' ) THEN
	  NAMES(2)='SFIL'
	  CALL APPCN(NAMES(2))
      ELSE
C         ERROR IN RESPONSE TO Q212
	  GOTO 212
      ENDIF
C
C
C
      LKI=IUNIT()
      OPEN(LKI,FILE=NAMES(1),STATUS='OLD')
C     OPEN(LKI,FILE=NAMES(1),STATUS='OLD')                              C-PRIME
C     OPEN(LKI,FILE=NAMES(1),STATUS='OLD')                              C-HP9000
C     OPEN(LKI,FILE=NAMES(1),STATUS='OLD',READONLY,SHARED)              C-VAX
C
      LKO=IUNIT()
      OPEN(LKO,FILE=NAMES(2),STATUS='UNKNOWN')
C     OPEN(LKO,FILE=NAMES(2),STATUS='UNKNOWN')                          C-PRIME
C     OPEN(LKO,FILE=NAMES(2),STATUS='UNKNOWN')                          C-HP9000
C     OPEN(LKO,FILE=NAMES(2),STATUS='NEW',CARRIAGECONTROL='LIST')       C-VAX
C
C
C
      READ(LKI,'(A)') TITLE(1:50)
C
C     ***        IS THE DATA TABULAR?             ***
C     *** NPAR = TOTAL NUMBER OF COLUMNS IN TABLE ***
C     *** NCOL = NUMBER OF COLUMN TO BE EXTRACTED ***
C     *** IFST = FLAG = 0/1 FOR SCATTERED/TABULAR ***
C
      IF( IDD(1).EQ.3 ) THEN
	  IFST=1
	  NCOL=IDD(2)
	  READ(LKI,*,ERR=991,IOSTAT=IN(18)) NPAR
      ELSE
	  IFST=0
	  NCOL=1
      ENDIF
C
C
      WRITE(LKO,'(A)') TITLE
C
C
C
      IF( IDD(5).EQ.3 ) THEN
C
CCC       FOR NORTH/SOUTH/EAST/WEST/ SPECIFIED BY CHARACTERS N,S,E,W
CCC       NEED TO PRE-PROCESS THE INPUT FILE SO THAT IT IS SUITABLE
CCC       FOR LIST-DIRECTED INPUT I.E. CHANGE  N  TO  'N'  ,ETC AND
CCC       WRITE TO A SCRATCH FILE
C
	  LS=IUNIT()
	  OPEN(LS,STATUS='SCRATCH')
C
C
    5     CONTINUE
C         PRE-PROCESSING LOOP
	  READ(LKI,'(A)',END=34,ERR=994) LINE
C
C
C
	  LINE1=' '
	  I1=1
	  DO 33 I=1,80
	      CHAR=LINE(I:I)
	      IF( CHAR.EQ.'N' .OR. CHAR.EQ.'S' .OR.
     *            CHAR.EQ.'E' .OR. CHAR.EQ.'W' ) THEN
		  IF( I1.GT.82 ) GOTO 994
		  LINE1(I1:I1+2)=''''//CHAR//''''
		  I1=I1+3
	      ELSE
		  IF( I1.GT.84 ) GOTO 994
		  LINE1(I1:I1)=CHAR
		  I1=I1+1
	      ENDIF
   33     CONTINUE
C
	  WRITE(LS,'(A)') LINE1
C
	  GOTO 5
C
C
   34     CONTINUE
C
CCC       INPUT EXHAUSTED, SCRATCH FILE SET UP -- NOW REWIND
CCC       AND ARRANGE FOR IT TO BE USED AS INPUT
C
	  CLOSE(LKI)
	  REWIND LS
	  LKI=LS
C
C
      ENDIF
C
C     SET UP LOOP
      ICOUNT=0
   10 CONTINUE
      IF( IDD(5).EQ.1 ) THEN
C         0/1 FOR N/S , E/W
	  IF( IDD(4).EQ.1 ) THEN
C             DECIMAL DEGREES  AND  0/1
	      READ(LKI,*,END=990,ERR=992,IOSTAT=IN(18))
     *            (ISTAT,II=1,IFST),RLAT,NS,RLONG,EW,
     *            (VALUE(II),II=1,NCOL)
	      X=RLONG
	      Y=RLAT
	  ELSEIF( IDD(4).EQ.2 ) THEN
C             DEGREES -- DECIMAL MINUTES  AND  0/1
	      READ(LKI,*,END=990,ERR=992,IOSTAT=IN(18))
     *            (ISTAT,II=1,IFST),
     *            ILAT,RLAT,NS,ILONG,RLONG,EW,(VALUE(II),II=1,NCOL)
	      Y=ILAT+RLAT/60.0
	      X=ILONG+RLONG/60.0
	  ELSE
C             DEGREES -- MINUTES -- SECONDS  AND  0/1
	      READ(LKI,*,END=990,ERR=992,IOSTAT=IN(18))
     *            (ISTAT,II=1,IFST),ILAT1,ILAT2,ILAT3,NS,
     *            ILONG1,ILONG2,ILONG3,EW,(VALUE(II),II=1,NCOL)
	      Y=ILAT1+ILAT2/60.0+ILAT3/3600.0
	      X=ILONG1+ILONG2/60.0+ILONG3/3600.0
	  ENDIF
	  X=X*(1-2*EW)
	  Y=Y*(1-2*NS)
      ELSEIF( IDD(5).EQ.2 ) THEN
C         +/- FOR N/S , E/W
	  IF( IDD(4).EQ.1 ) THEN
C             DECIMAL DEGREES  AND  +/-
	      READ(LKI,*,END=990,ERR=992,IOSTAT=IN(18))
     *          (ISTAT,II=1,IFST),RLAT,RLONG,(VALUE(II),II=1,NCOL)
	      X=RLONG
	      Y=RLAT
	  ELSEIF( IDD(4).EQ.2 ) THEN
C             DEGREES -- DECIMAL MINUTES  AND  +/-
	      READ(LKI,*,END=990,ERR=992,IOSTAT=IN(18))
     *            (ISTAT,II=1,IFST),ILAT,RLAT,ILONG,RLONG,
     *            (VALUE(II),II=1,NCOL)
	      IF( ILAT.GT.0 ) THEN
		  Y=ILAT+RLAT/60.0
	      ELSEIF( ILAT.EQ.0 ) THEN
		  Y=RLAT/60.0
	      ELSEIF( ILAT.LT.0 ) THEN
		  Y=ILAT-RLAT/60.0
	      ENDIF
	      IF( ILONG.GT.0 ) THEN
		  X=ILONG+RLONG/60.0
	      ELSEIF( ILONG.EQ.0 ) THEN
		  X=RLONG/60.0
	      ELSEIF( ILONG.LT.0 ) THEN
		  X=ILONG-RLONG/60.0
	      ENDIF
	  ELSE
C             DEGREES -- MINUTES -- SECONDS  AND  +/-
	      READ(LKI,*,END=990,ERR=992,IOSTAT=IN(18))
     *            (ISTAT,II=1,IFST),ILAT1,ILAT2,ILAT3,
     *            ILONG1,ILONG2,ILONG3,(VALUE(II),II=1,NCOL)
	      Y=ABS(ILAT1)+ABS(ILAT2)/60.0+ABS(ILAT3)/3600.0
	      Y=ISGN(ILAT1,ILAT2,ILAT3)*Y
	      X=ABS(ILONG1)+ABS(ILONG2)/60.0+ABS(ILONG3)/3600.0
	      X=ISGN(ILONG1,ILONG2,ILONG3)*X
	  ENDIF
      ELSE
C         N S E OR W
	  IF( IDD(4).EQ.1 ) THEN
C             DECIMAL DEGREES  AND  NSEW
	      READ(LKI,*,END=990,ERR=992,IOSTAT=IN(18))
     *          (ISTAT,II=1,IFST),RLAT,CLAT,RLONG,CLONG,
     *          (VALUE(II),II=1,NCOL)
	      X=RLONG
	      Y=RLAT
	  ELSEIF( IDD(4).EQ.2 ) THEN
C             DEGREES -- DECIMAL MINUTES  AND  NSEW
	      READ(LKI,*,END=990,ERR=992,IOSTAT=IN(18))
     *            (ISTAT,II=1,IFST),ILAT,RLAT,CLAT,ILONG,RLONG,CLONG,
     *            (VALUE(II),II=1,NCOL)
	      Y=ILAT+RLAT/60.0
	      X=ILONG+RLONG/60.0
	  ELSE
C             DEGREES -- MINUTES -- SECONDS  AND  NSEW
	      READ(LKI,*,END=990,ERR=992,IOSTAT=IN(18))
     *            (ISTAT,II=1,IFST),ILAT1,ILAT2,ILAT3,CLAT,
     *            ILONG1,ILONG2,ILONG3,CLONG,(VALUE(II),II=1,NCOL)
	      Y=ILAT1+ILAT2/60.0+ILAT3/3600.0
	      X=ILONG1+ILONG2/60.0+ILONG3/3600.0
	  ENDIF
C
C
	  IF( CLAT.EQ.'N' ) THEN
C             NORTH
	      CONTINUE
	  ELSEIF( CLAT.EQ.'S' ) THEN
C             SOUTH
	      Y=-Y
	  ELSE
C             ERROR
	      GOTO 993
	  ENDIF
C
C
	  IF( CLONG.EQ.'E' ) THEN
C             EAST
	      CONTINUE
	  ELSEIF( CLONG.EQ.'W' ) THEN
C             WEST
	      X=-X
	  ELSE
C             ERROR
	      GOTO 993
	  ENDIF
      ENDIF
C
C    X,Y AND VALUE EXTRACTED FROM DATAFILE
C
C    IF VALUE=-9.0 ,REGARD AS INVALID DATA
C    (SEE LINE 00710-00730 OF LOWESTOFT SOURCE SUCHA )
C
      IF(VALUE(NCOL).EQ.-9.0 ) GOTO 10
C
      ICOUNT=ICOUNT+1
      WRITE(LKO,'(3E16.8)') X,Y,VALUE(NCOL)
C
C
      GOTO 10
C
C
C
  990 CONTINUE
C
C     END-OF-FILE DETECTED
C
      CLOSE(LKI)
      CLOSE(LKO)
      IDD(7)=ICOUNT
C
      IF (IDD(7).GT.2000) THEN
C        NUMBER OF SOURCE DATA POINTS IS ABOVE THAT WHICH CAN BE DEALT
C        WITH BY THE PACKAGE
	 WRITE (LL,2990) IDD(7)
	 WRITE (LR,2990) IDD(7)
 2990    FORMAT (//' +++ FATAL ERROR DETECTED +++'/
     *      5X,'NUMBER OF SOURCE DATA POINTS IS ',I8/
     *      5X,'THIS IS TOO LARGE - MAXIMUM ALLOWED IS 2000')
	 CALL ABORT
      ENDIF
C
C
      RETURN
C
C
C
  991 CONTINUE
C
C     ERROR DETECTED
C
      WRITE(LL,2991) IN(18)
      WRITE(LR,2991) IN(18)
 2991 FORMAT(//'+++ FATAL ERROR DETECTED, IOSTAT =',I4,' +++'/2X,
     * 'IN SUBROUTINE REFMT -- WHILE',
     * ' READING <NPAR> FROM DATA-FILE')
      CALL ABORT
C
C
C
  992 CONTINUE
C
C     ERROR DETECTED
C
      WRITE(LL,2992) IN(18)
      WRITE(LR,2992) IN(18)
 2992 FORMAT(//'+++ FATAL ERROR DETECTED, IOSTAT =',I4,' +++'/2X,
     *    'IN SUBROUTINE REFMT -- WHILE READING ',
     *    '<LAT,LONG,VALUE(S)> FROM DATA-FILE')
      CALL ABORT
C
C
C
C
  993 CONTINUE
C
C     ERROR DETECTED
C
      WRITE(LL,2993)
      WRITE(LR,2993)
 2993 FORMAT('+++ AN ERROR IN REFMT HAS OCCURRED WHILST READING ',
     *   'N S E W FORMATTED VALUES +++' )
      CALL ABORT
C
C
C
  994 CONTINUE
C
C     ERROR DETECTED
C
      WRITE(LL,2994)
      WRITE(LR,2994)
 2994 FORMAT('+++ AN ERROR IN REFMT HAS OCCURRED WHILST SETTING UP ',
     *   'THE SCRATCH FILE FOR NSEW LAT/LONG +++' )
      CALL ABORT
C
C
C     **
C     **    END OF SUBROUTINE REFMT
C     **
C
      END
C+*+* REFMT  END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* QCS    START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE QCS
C
C     ******************************************************************
C     *                                                                *
C     *   THIS SUBROUTINE ASKS QUESTIONS ABOUT THE CONTOURER           *
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
C     SUBPROGRAM QCS    CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     APPCN   FLNAM   QGG     QKN     RSERR   SCAN    TRANS   ZREAD
C
C     SUBPROGRAM QCS    IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     SSU10
C
C     LOCAL VARIABLES
C     ***************
      CHARACTER ANS,LINE*80,IFORM*7,RFORM*10,STRNG*25
      INTEGER FLAG,II
      REAL*8 XPMIN,XPMAX,YPMIN,YPMAX
C
      IFORM='(BN,I6)'
      RFORM='(BN,F16.0)'
C
CCC   scan source datafile to find max and min values of x,y,value
C
      CALL SCAN
C
      WRITE(LL,2000)
 2000 FORMAT(///2X,'***************************************'/
     *          2X,'* QUESTIONS RELATED TO THE CONTOURING *'/
     *          2X,'***************************************'/)
C
C
C
C     SET GRIDDING AND SPLINE SWITCHES OFF
      IGG(1)=0
      IKN(1)=0
C
C
C
  221 CONTINUE
      IF( QNO.EQ.221 ) CALL RSERR(FLAG)
      QNO=221
      WRITE(LL,2221)
 2221 FORMAT(//2X,'*** Q221 ***'/2X,'DO YOU REQUIRE'/
     *      /7X,'1  LINE PRINTER CONTOURER (BI-LINEAR)?'
     *      /7X,'2  LINE PRINTER CONTOURER (BI-CUBIC)?'
     *      /7X,'3  NAG REGULAR CONTOURER?'
     *      /7X,'4  NAG SCATTERED CONTOURER?'
     *      /7X,'5  HARWELL CONTOURER?'
     *      /7X,'6  ISOMETRIC PROJECTION?'
     *      /13X,'OR'
     *      /7X,'7  PLOTTING OF SAMPLE POINTS WITHOUT CONTOURING?'
     *      //7X,'TYPE 1,2,3,4,5,6 OR 7')
      CALL ZREAD(.TRUE.,LINE,FLAG,*221)
      READ(LINE,IFORM,ERR=221) ICS(1)
      IF( ICS(1).LT.1 .OR. ICS(1).GT.7 ) GOTO 221
C
C
C
      IF( ICS(1).EQ.1 .OR. ICS(1).EQ.2 ) THEN
C         LINE PRINTER CONTOURERS
C
C
C
  222     CONTINUE
	  IF( QNO.EQ.222 ) CALL RSERR(FLAG)
	  QNO=222
	  WRITE(LL,2222)
 2222     FORMAT(//2X,'*** Q222 ***',
     *       /2X,'BY DEFAULT, THE LINEPRINTER CONTOURS WILL BE WRITTEN '
     *       /2X,'TO THE FILE NAMED LPCONT (ON THE DEFAULT CARTRIDGE). '
     *       //2X,'DO YOU WISH TO SPECIFY SOME ALTERNATIVE FILE?')
	  CALL ZREAD(.TRUE.,LINE,FLAG,*222)
	  READ(LINE,'(A)') ANS
	  IF( ANS.EQ.' ' ) ANS='Y'
	  IF( ANS.EQ.'Y' ) THEN
C
C
C             ASK FOR THE NAME OF THE FILE
	      CALL FLNAM(NAMES(8),1)
	  ELSEIF( ANS.EQ.'N' ) THEN
C             DEFAULT FILENAME
	      NAMES(8)='LPCONT'
	      CALL APPCN(NAMES(8))
	  ELSE
CCC           ERROR
	      GOTO 222
	  ENDIF
C
C
C
      ENDIF
C
C
C
      IF( IDD(1).EQ.1 .AND. ICS(1).EQ.4 ) THEN
C         REGULAR DATA AND NAG-SCATTERED CONTOURER
	  WRITE(LL,2900)
 2900     FORMAT(//7X,'THE NAG-SCATTERED CONTOURER IS NOT AVAILABLE',
     *         ' WITH REGULAR DATA'/7X,'PLEASE RESELECT')
	  QNO=0
	  GOTO 221
      ENDIF
C
C
C
      IF( IDD(1).EQ.2 .OR. IDD(1).EQ.3 ) THEN
C         SCATTERED OR TABULAR
	  IF( ICS(1).LE.3 .OR. ICS(1).EQ.5 .OR. ICS(1).EQ.6 ) THEN
C             LP CONTOURERS,NAG-REGULAR,HARWELL OR ISOMETRIC PROJ
C
C
C
  223         CONTINUE
	      IF( QNO.EQ.223 ) CALL RSERR(FLAG)
	      QNO=223
	      WRITE(LL,2223)
 2223         FORMAT(//2X,'*** Q223 ***',/2X,
     *           'THE SCATTERED ( OR TABULAR ) DATA NEEDS TO BE GRIDDED'
     *        ,/2X,'FOR USE WITH THIS CONTOURER OPTION',//2X,
     *           'DO YOU WISH TO USE THE GRIDDING SUITE?')
	      CALL ZREAD(.TRUE.,LINE,FLAG,*223)
	      READ(LINE,'(A)') ANS
	      IF( ANS.EQ.' ' ) ANS='Y'
	      IF( ANS.EQ.'Y' ) THEN
C                 SET GRIDDING SWITCH ON, ASK GRIDDING QUESTIONS
		  IGG(1)=1
		  CALL QGG
C
C
C
		  WRITE(LL,2001)
 2001             FORMAT(///
     *       2X,'***************************************************'/
     *       2X,'* QUESTIONS RELATED TO THE CONTOURING (CONTINUED) *'/
     *       2X,'***************************************************'/)
C
	      ELSEIF( ANS.EQ.'N' ) THEN
CCC               RESELECT
		  WRITE(LL,'(/A)') 'PLEASE RE-SELECT THE CONTOURER'
		  GOTO 221
	      ELSE
CCC               ERROR
		  GOTO 223
	      ENDIF
C
C
C
	  ELSEIF( ICS(1).EQ.4 ) THEN
C             NAG-SCATTERED CONTOURER
	      CALL QKN
	      WRITE(LL,2001)
	  ENDIF
      ENDIF
C
C
C
      IF( ICS(1).LE.2 ) THEN
	  STRNG='LINE PRINTER CONTOURER '
      ELSEIF( ICS(1).EQ.3 ) THEN
	  STRNG='NAG REGULAR CONTOURER '
      ELSEIF( ICS(1).EQ.4 ) THEN
	  STRNG='NAG SCATTERED CONTOURER '
      ELSEIF( ICS(1).EQ.5 ) THEN
	  STRNG='HARWELL CONTOURER '
      ELSEIF( ICS(1).EQ.6 ) THEN
	  STRNG='NAG ISOMETRIC '
      ELSE
	  STRNG='DATA POINTS ONLY '
      ENDIF
C
      WRITE(LR,2002) STRNG
 2002 FORMAT(//2X,'CONTOURING OPTION SELECTED : ',A)
C
C
C
  224 CONTINUE
      IF( QNO.EQ.224 ) CALL RSERR(FLAG)
      QNO=224
      WRITE(LL,2224)
 2224 FORMAT(//2X,'*** Q224 ***'/2X,'DEFINITION OF PLOTTING REGION')
C
C
      IF( IDD(1).EQ.1 ) THEN
C         SOURCE DATA IS GRIDDED
	  XPMIN=RDD(3)
	  XPMAX=RDD(4)
	  YPMIN=RDD(5)
	  YPMAX=RDD(6)
	  WRITE(LL,2901) XPMIN,XPMAX,YPMIN,YPMAX
 2901     FORMAT(/2X,' YOUR  GRIDDED  POINTS ARE',
     *       /3X,'        WITHIN          ',
     *       /3X,1PG10.3,' < X < ',1PG10.3,
     *       /3X,1PG10.3,' < Y < ',1PG10.3)
C
      ELSEIF( IGG(1).EQ.0 ) THEN
C         SOURCE DATA IS SCATTERED(OR TABULAR),GRIDDING NOT
	  XPMIN=RDD(3)
	  XPMAX=RDD(4)
	  YPMIN=RDD(5)
	  YPMAX=RDD(6)
	  WRITE(LL,2902) XPMIN,XPMAX,YPMIN,YPMAX
 2902     FORMAT(/2X,' YOUR SCATTERED POINTS ARE',
     *       /3X,'        WITHIN          ',
     *       /3X,1PG10.3,' < X < ',1PG10.3,
     *       /3X,1PG10.3,' < Y < ',1PG10.3)
C
      ELSE
C         SOURCE DATA IS SCATTERED(OR TABULAR),GRIDDING HAS TAKEN PLACE
	  XPMIN=RGG(4)
	  XPMAX=RGG(5)
	  YPMIN=RGG(6)
	  YPMAX=RGG(7)
	  WRITE(LL,2903) RDD(3),RDD(4),XPMIN,XPMAX,
     *                   RDD(5),RDD(6),YPMIN,YPMAX
 2903     FORMAT(/2X,' YOUR SCATTERED POINTS ARE',14X,
     *               ' YOUR  GRIDDED  POINTS ARE',
     *     /3X,'        WITHIN          ',22X,'WITHIN',
     *     /3X,1PG10.3,' < X < ',1PG10.3,11X,
     *         1PG10.3,' < X < ',1PG10.3,
     *     /3X,1PG10.3,' < Y < ',1PG10.3,11X,
     *         1PG10.3,' < Y < ',1PG10.3)
C
      ENDIF
C
      IF( ICS(1).EQ.1 .OR. ICS(1).EQ.2 ) WRITE(LL,2904)
 2904 FORMAT(/2X,'FOR LINE PRINTER CONTOURING, THE PLOTTING REGION',
     *        /2X,'MUST LIE WITHIN THE GRIDDED REGION' )
C
      WRITE(LL,2905)
 2905 FORMAT(/2X,'DEFINE THE PLOTTING REGION BY ENTERING',
     *   /2X,'MINIMUM AND MAXIMUM X AND Y VALUES')
      WRITE(LL,2906)
 2906 FORMAT(/2X,'MINIMUM X?')
      CALL ZREAD(.TRUE.,LINE,FLAG,*224)
      READ(LINE,RFORM,ERR=224) RCS(1)
      WRITE(LL,2907)
 2907 FORMAT(/2X,'MAXIMUM X?')
      CALL ZREAD(.TRUE.,LINE,FLAG,*224)
      READ(LINE,RFORM,ERR=224) RCS(2)
      IF( RCS(2).LE.RCS(1) ) GOTO 224
      WRITE(LL,2908)
 2908 FORMAT(/2X,'MINIMUM Y?')
      CALL ZREAD(.TRUE.,LINE,FLAG,*224)
      READ(LINE,RFORM,ERR=224) RCS(3)
      WRITE(LL,2909)
 2909 FORMAT(/2X,'MAXIMUM Y?')
      CALL ZREAD(.TRUE.,LINE,FLAG,*224)
      READ(LINE,RFORM,ERR=224) RCS(4)
C####
C####
C     commented out to allow reverse plots e.g. ymax=0, ymin=-1 (?? DE )
C     which would fail with this test : G Bunn 27/1/87 at request of John
C     Shepard
C      IF( RCS(4).LE.RCS(3) ) GOTO 224
C####
C####
CCC
CCC   TEST TO SEE IF PLOTTING REGION IS SENSIBLY CHOSEN
CCC
      IF( ICS(1).EQ.1 .OR. ICS(1).EQ.2 ) THEN
C
C         LINE PRINTER CONTOURS REQUIRED
C         TEST PLOTTING AND GRIDDED AREA COINCIDENCE
C
	  IF( RCS(1).LT.XPMIN .OR.
     *        RCS(2).GT.XPMAX .OR.
     *        RCS(3).LT.YPMIN .OR.
     *        RCS(4).GT.YPMAX ) THEN
	      WRITE(LL,2910)
 2910         FORMAT(//2X,'WHEN THE LINE PRINTER CONTOURER IS ',
     *          'SELECTED,'/2X,' THE PLOTTING REGION MUST NOT ',
     *          'EXCEED THE GRIDDED REGION' )
	      GOTO 224
	  ENDIF
C
C
C
      ELSEIF( ICS(1).EQ.3 .OR. ICS(1).EQ.5 .OR. ICS(1).EQ.6 ) THEN
C
C         CONTOURERS USE GRIDDED DATA
C         CHECK THAT PLOTTING REGION CONTAINS SOME GRIDDED POINTS
C         MORE DETAILED CHECKS ARE PERFORMED IN SUBR RDATA IN SSSU30
C
	  IF( RCS(1).GT.XPMAX .OR.
     *        RCS(2).LT.XPMIN .OR.
     *        RCS(3).GT.YPMAX .OR.
     *        RCS(4).LT.YPMIN ) THEN
	      WRITE(LL,2911)
	      WRITE(LR,2911)
 2911         FORMAT(//2X,'THE PLOTTING REGION AND THE GRIDDED REGION ',
     *          'DO NOT OVERLAP ',
     *          /'PLEASE RE-DEFINE THE PLOTTING REGION')
	      GOTO 224
	  ENDIF
C
C
C
      ELSEIF( ICS(1).EQ.4 ) THEN
C
C         CONTOURER USES SCATTERED DATA
C         CHECK THAT PLOTTING REGION CONTAINS SOME OF THE SPINED REGION
C         MORE DETAILED CHECKS ARE PERFORMED IN SUBR SDATA IN SSSU30
C
	  IF( RCS(1).GT.XPMAX .OR.
     *        RCS(2).LT.XPMIN .OR.
     *        RCS(3).GT.YPMAX .OR.
     *        RCS(4).LT.YPMIN ) THEN
	      WRITE(LL,2912)
 2912         FORMAT(//2X,'THE PLOTTING REGION AND THE SPLINED REGION ',
     *          'DO NOT OVERLAP ',
     *          /'PLEASE RE-DEFINE THE PLOTTING REGION')
	      GOTO 224
	  ENDIF
C
C
C
      ENDIF
C
CCC
CCC   RETURN IF ISOMETRIC PROJECTION OR DATA POINTS ONLY
CCC
      IF( ICS(1).EQ.7 .OR. ICS(1).EQ.6 ) RETURN
C
C
C
  225 CONTINUE
      IF( QNO.EQ.225 ) CALL RSERR(FLAG)
      QNO=225
      WRITE(LL,2225) RDD(7),RDD(8)
 2225 FORMAT(//2X,'*** Q225 ***'/
     *  2X,'ENTER THE NUMBER OF CONTOUR HEIGHTS (MAXIMUM = 10)'/
C??    *   2X,'(FUNCTION VALUES ARE IN THE RANGE ',F8.3,' TO ',F8.3,' )')
     *  2X,'(FUNCTION VALUES ARE IN THE RANGE ',E10.3,' TO ',E10.3,' )')
      CALL ZREAD(.TRUE.,LINE,FLAG,*225)
      READ(LINE,IFORM,ERR=225) ICS(4)
      IF( ICS(4).LT.1 .OR. ICS(4).GT.10 ) GOTO 225
      IF( ICS(1).EQ.1 .OR. ICS(1).EQ.2 ) THEN
	  IF( ICS(4).NE.(ICS(4)/2)*2 ) THEN
	      WRITE(LL,2913)
 2913         FORMAT(//7X,'LINEPRINTER CONTOURS REQUIRE AN EVEN ',
     *           'NUMBER OF HEIGHTS')
	      GOTO 225
	  ENDIF
      ENDIF
C
C
C
  226 CONTINUE
      IF( QNO.EQ.226 ) CALL RSERR(FLAG)
      QNO=226
      WRITE(LL,2226)
 2226 FORMAT(//2X,'*** Q226 ***',
     *   /2X,'ENTER THE CONTOUR HEIGHTS ')
C
      DO 10 II=11,10+ICS(4)
	  WRITE(LL,2914) II-10
 2914     FORMAT(/2X,'HEIGHT NO.',I2,'?')
	  CALL ZREAD(.TRUE.,LINE,FLAG,*226)
	  READ(LINE,RFORM,ERR=226) RCS(II)
   10 CONTINUE
C
CCCC
CCCC  SEE IF A TRANSFORMATION IS REQUESTED
CCCC
      CALL TRANS(2,ICS(7),RCS(21),RCS(22))
C
C
C
      RETURN
C
C     **
C     **    END OF SUBROUTINE QCS
C     **
C
      END
C+*+* QCS    END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* TRANS  START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE TRANS(INDIC,TFLAG,A,B)
C
C
C     ******************************************************************
C     *                                                                *
C     *   THIS SUBROUTINE IS USED TO ASK QUESTIONS ABOUT ANY           *
C     *   TRANSFORMATION THAT IS TO BE PERFORMED                       *
C     *                                                                *
C     *   ARGUMENTS                                                    *
C     *   *********                                                    *
C     *   INDIC   -  1/2 WHEN CALLED FROM QGG/QCS                      *
C     *   TFLAG   -  INDICATES TYPE OF TRANSFORMATION                  *
C     *   A,B     -  TRANSFORMATION CONSTANTS                          *
C     *                                                                *
C     ******************************************************************
C
      IMPLICIT LOGICAL (A-Z)
C
C     ARGUMENTS
C     *********
      INTEGER INDIC,TFLAG
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
C     SUBPROGRAM TRANS  CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     RSERR   ZREAD
C
C     SUBPROGRAM TRANS  IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     QCS     QGG
C
C     LOCAL VARIABLES
C     ***************
      CHARACTER ANS,LINE*80,STRING*10,IFORM*7,RFORM*10
      INTEGER FLAG
      REAL*8 E,V1,V2
C
      IFORM='(BN,I6)'
      RFORM='(BN,F16.0)'
C
C
      IF( INDIC.EQ.1 ) THEN
	  STRING='GRIDDING'
      ELSE
	  STRING='CONTOURING'
      ENDIF
C
C
C
  271 CONTINUE
      IF( QNO.EQ.271 ) CALL RSERR(FLAG)
      QNO=271
      WRITE(LL,2271) STRING,RDD(7),RDD(8)
 2271 FORMAT(//2X,'*** Q271 ***'/2X,'DO YOU WANT TO TRANSFORM ',
     *    'BEFORE ',A,'?',
     *     //2X,'YOUR DATA VALUES ARE SUCH THAT ',
     *     /10X,1PE10.2,' < DATA VALUE < ',1PE10.2)
      CALL ZREAD(.TRUE.,LINE,FLAG,*271)
      READ(LINE,'(A)') ANS
      IF( ANS.EQ.' ' ) ANS='Y'
      IF( ANS.EQ.'Y' ) THEN
C
C
C
  272     CONTINUE
	  IF( QNO.EQ.272 ) CALL RSERR(FLAG)
	  QNO=272
	  WRITE(LL,2272)
 2272     FORMAT(//2X,'*** Q272 ***',/2X,'CHOOSE A TRANSFORMATION'/
     *       /7X,'1  CHANGE OF ORIGIN?',
     *       /7X,'2  STANDARD LINEAR TRANSFORMATION?',
     *       /7X,'3  USER-DEFINED LINEAR TRANSFORMATION v''=a+b*v?',
     *       /7X,'4  STANDARD LOG TRANSFORMATION?',
     *       /7X,'5  USER-DEFINED LOG TRANSFORMATION v''=log(a+b*v)?',
     *       //7X,'TYPE 1,2,3,4 OR 5')
	  CALL ZREAD(.TRUE.,LINE,FLAG,*272)
	  READ(LINE,IFORM,ERR=272) TFLAG
	  IF( TFLAG.EQ.1 ) THEN
C             LINEAR TRANF V'=A+B*V WITH B = 1,A = USER SELECTED
	      B=1.0
C
C
C
  273         CONTINUE
	      IF( QNO.EQ.273 ) CALL RSERR(FLAG)
	      QNO=273
	      WRITE(LL,2273)
 2273         FORMAT(//2X,'*** Q273 ***',/2X,'TYPE IN THE VALUE OF THE',
     *            ' CONSTANT TO BE SUBTRACTED FROM THE DATA VALUES')
	      CALL ZREAD(.TRUE.,LINE,FLAG,*273)
	      READ(LINE,RFORM,ERR=273) A
	      A=-A
C
C
C
	  ELSEIF( TFLAG.EQ.2 ) THEN
C             STANDARD LINEAR TRANF V'=A+B*V TO RANGE 1.0 TO 2.0
	      B=1.0/(RDD(8)-RDD(7))
	      A=1.0-B*RDD(7)
	  ELSEIF( TFLAG.EQ.4 ) THEN
C             STANDARD LOGARITHMIC TRANF V'=LOG(A+B*V) TO RANGE 1.0 TO 2
	      E=EXP(1.0)
	      B=(E**2-E)/(RDD(8)-RDD(7))
	      A=E-B*RDD(7)
	  ELSEIF( TFLAG.EQ.3 .OR. TFLAG.EQ.5 ) THEN
C             TFLAG=3 : LINEAR TRANF V'=A+B*V
C             TFLAG=5 : LOGARITHMIC TRANF V'=LOG(A+B*V)
C               -- BOTH A,B USER SELECTED
C
C
C
  274         CONTINUE
	      IF( QNO.EQ.274 ) CALL RSERR(FLAG)
	      QNO=274
	      WRITE(LL,2274)
 2274         FORMAT(//2X,'*** Q274 ***',/2X,'DEFINE THE CONSTANTS A',
     *            ' AND B')
	      WRITE(LL,2901)
C
 2901         FORMAT(//2X,'TYPE IN THE VALUE OF A')
	      CALL ZREAD(.TRUE.,LINE,FLAG,*274)
	      READ(LINE,RFORM,ERR=274) A
	      WRITE(LL,2902)
C
 2902         FORMAT(//2X,'TYPE IN THE VALUE OF B')
	      CALL ZREAD(.TRUE.,LINE,FLAG,*274)
	      READ(LINE,RFORM,ERR=274) B
C
	      IF( B.EQ.0 ) THEN
C                 CHECK FOR B=0
		  WRITE(LL,2903)
 2903             FORMAT(//2X,'YOU HAVE SELECTED B EQUAL TO EQUAL ZERO',
     *                   ' -- THIS IS NOT PERMITTED',
     *                  /2X,'PLEASE RE-SELECT')
		  QNO=0
		  GOTO 274
	      ELSEIF( TFLAG.EQ.5 ) THEN
C                 CHECK FOR -VE ARGUMENT TO LOG
		  V1=A+B*RDD(7)
		  V2=A+B*RDD(8)
		  IF( V1.LT.1E-05 .OR. V2.LT.1E-05 ) THEN
		      WRITE(LL,2904)
 2904                 FORMAT(//2X,'YOUR CHOICE OF A AND B WILL LEAD ',
     *                       'TO NEGATIVE ARGUMENT IN LOGARITHM',
     *                      /2X,'PLEASE RE-SELECT')
		      QNO=0
		      GOTO 274
		  ENDIF
	      ENDIF
C
	  ELSE
C             ERROR
	      GOTO 272
	  ENDIF
C
C
C
      ELSEIF( ANS.EQ.'N' ) THEN
	  TFLAG=0
      ELSE
C         ERROR
	  GOTO 271
      ENDIF
C
C
C
      RETURN
C
C     **
C     **    END OF SUBROUTINE TRANS
C     **
C
      END
C+*+* TRANS  END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* QKN    START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE QKN
C
C     ******************************************************************
C     *                                                                *
C     *      THIS SUBROUTINE ASKS QUESTIONS ABOUT THE SPLINES          *
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
C     SUBPROGRAM QKN    CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     RSERR   ZREAD
C
C     SUBPROGRAM QKN    IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     QCS
C
C     LOCAL VARIABLES
C     ***************
      CHARACTER ANS,LINE*80,IFORM*7,RFORM*10
      INTEGER FLAG,II,II0,KNMAX
C
      IFORM='(BN,I6)'
      RFORM='(BN,F16.0)'
C
C
C
      WRITE(LL,2000)
 2000 FORMAT(///2X,'**************************************'/
     *          2X,'* QUESTIONS RELATED TO THE SPLINES   *'/
     *          2X,'**************************************'/)
C
C
C
C     SET THE DEFAULT VALUE OF EPS
      RKN(41)=1E-4
C     SET IKN(4) FOR NO WEIGHTS
      IKN(4)=0
C
C
C
  261 CONTINUE
      IF( QNO.EQ.261 ) CALL RSERR(FLAG)
      QNO=261
      WRITE(LL,2261)
 2261 FORMAT(//2X,'*** Q261 ***'/2X,'THE NAG SCATTERED CONTOURER',
     *        ' REQUIRES BICUBIC SPLINE FITTING'/
     *        /2X,'DO YOU WANT THE PROGRAM TO ATTEND TO ',
     *        'ALL THE DETAILS?')
C
C     IF REQUESTED, THE DEFAULT NUMBER AND POSITION OF KNOTS ARE
C     DETERMINED IN THE SPLINE PROGRAM. HOWEVER, THE DEFAULT VALUE
C     OF EPS=RKN(41) IS SET ABOVE
C
      CALL ZREAD(.TRUE.,LINE,FLAG,*261)
      READ(LINE,'(A)') ANS
      IF( ANS.EQ.' ' ) ANS='Y'
      IF( ANS.EQ.'Y' ) THEN
	  IKN(1)=1
      ELSEIF( ANS.EQ.'N' ) THEN
C
C
C
  262     CONTINUE
	  IF( QNO.EQ.262 ) CALL RSERR(FLAG)
	  QNO=262
	  WRITE(LL,2262) RKN(41)
 2262     FORMAT(//2X,'*** Q262 ***'/2X,'THE DEFAULT VALUE OF EPS, ',
     *            'USED IN THE NAG SUBROUTINE E02DAF, IS ',E12.6,
     *            /2X,'IS THIS VALUE ACCEPTABLE?')
	  CALL ZREAD(.TRUE.,LINE,FLAG,*262)
	  READ(LINE,'(A)') ANS
	  IF( ANS.EQ.'Y' ) ANS='Y'
	  IF( ANS.EQ.'Y' ) THEN
	      CONTINUE
	  ELSEIF( ANS.EQ.'N' ) THEN
C
C
C
  263         CONTINUE
	      IF( QNO.EQ.263 ) CALL RSERR(FLAG)
	      QNO=263
	      WRITE(LL,2263)
 2263         FORMAT(//2X,'*** Q263 ***'/2X,'TYPE IN THE REQUIRED ',
     *                                   'VALUE OF EPS')
	      CALL ZREAD(.TRUE.,LINE,FLAG,*263)
	      READ(LINE,RFORM,ERR=263) RKN(41)
	      IF ( RKN(41).LE.0 ) GOTO 263
C
C
C
	  ELSE
	      GOTO 262
	  ENDIF
C
C
C
C         SET MAX NO OF INTERIOR KNOTS
	  KNMAX=12
C
C
C
  264     CONTINUE
	  IF( QNO.EQ.264 ) CALL RSERR(FLAG)
	  QNO=264
	  WRITE(LL,2264) KNMAX
 2264     FORMAT(//2X,'*** Q264 ***'/2X,'TYPE IN THE NUMBER OF ',
     *            'INTERIOR KNOTS IN THE X-DIRECTION ',
     *            /2X,'(MAXIMUM OF ',I2,')')
	  CALL ZREAD(.TRUE.,LINE,FLAG,*264)
	  READ(LINE,IFORM,ERR=264) IKN(2)
	  IF ( IKN(2).LT.0 .OR. IKN(2).GT.KNMAX  ) GOTO 264
C         SHOULD HAVE TOTAL NO OF KNOTS
	  IKN(2)=IKN(2)+8
C
C
C
  265     CONTINUE
	  IF( QNO.EQ.265 ) CALL RSERR(FLAG)
	  QNO=265
	  WRITE(LL,2265) KNMAX
 2265     FORMAT(//2X,'*** Q265 ***'/2X,'TYPE IN THE NUMBER OF ',
     *            'INTERIOR KNOTS IN THE Y-DIRECTION ',
     *            /2X,'(MAXIMUM OF ',I2,')')
	  CALL ZREAD(.TRUE.,LINE,FLAG,*265)
	  READ(LINE,IFORM,ERR=265) IKN(3)
	  IF ( IKN(3).LT.0 .OR. IKN(3).GT.KNMAX  ) GOTO 265
C         SHOULD HAVE TOTAL NO OF KNOTS
	  IKN(3)=IKN(3)+8
C
C
C
	  IF( (IKN(2)-8)**2+(IKN(3)-8)**2 .EQ.0 ) THEN
C             NO INTERIOR KNOTS IN EITHER DIMENSION
	      IKN(1)=3
	      RETURN
	  ENDIF
C
C
  266     CONTINUE
	  IF( QNO.EQ.266 ) CALL RSERR(FLAG)
	  QNO=266
	  WRITE(LL,2266)
 2266     FORMAT(//2X,'*** Q266 ***'/2X,'DO YOU WANT THE PROGRAM TO ',
     *            'DETERMINE THE POSITION OF THE KNOTS?')
	  CALL ZREAD(.TRUE.,LINE,FLAG,*266)
	  READ(LINE,'(A)') ANS
	  IF( ANS.EQ.' ' ) ANS='Y'
	  IF( ANS.EQ.'Y' ) THEN
	      IKN(1)=2
	  ELSEIF( ANS.EQ.'N' ) THEN
	      IKN(1)=3
C
C
C
	      IF( IKN(2).GT.8 ) THEN
C               AT LEAST 1 INTERIOR KNOT IN THE X-DIRECTION
C
C
  267           CONTINUE
		IF( QNO.EQ.267 ) CALL RSERR(FLAG)
		QNO=267
		WRITE(LL,2267)
 2267           FORMAT(//2X,'*** Q267 ***'/
     *           2X,'ENTER INTERIOR X-KNOT VALUES '/
     *           2X,'(NOTE: VALUES MUST BE ENTERED IN INCREASING ORDER',
     *           ' AND MUST NOT LIE',/2X,'OUTSIDE THE DATA REGION)')
C               PUT INTERIOR X-KNOTS INTO RKN(5),...
		II0=5
		DO 10 II=II0,II0+IKN(2)-9
		    WRITE(LL,2911) II-II0+1
 2911               FORMAT(/2X,'X-KNOT NO.',I2,'?')
		    CALL ZREAD(.TRUE.,LINE,FLAG,*267)
		    READ(LINE,RFORM,ERR=267) RKN(II)
		    IF(RKN(II).LT.RDD(3).OR.RKN(II).GT.RDD(4))THEN
			WRITE(LL,2913)
			GOTO 267
		    ENDIF
		    IF(RKN(II).LT.RKN(II-1))THEN
			WRITE(LL,2914)
			GOTO 267
		    ENDIF
   10           CONTINUE
C
	       ENDIF
C
C
C
	      IF( IKN(3).GT.8 ) THEN
C               AT LEAST 1 INTERIOR KNOT IN THE Y-DIRECTION
C
C
  268           CONTINUE
		IF( QNO.EQ.268 ) CALL RSERR(FLAG)
		QNO=268
		WRITE(LL,2268)
 2268           FORMAT(//2X,'*** Q268 ***'/
     *           2X,'ENTER INTERIOR Y-KNOT VALUES '/
     *           2X,'(NOTE: VALUES MUST BE ENTERED IN INCREASING ORDER',
     *           ' AND MUST NOT LIE',/2X,'OUTSIDE THE DATA REGION)')
C               PUT INTERIOR Y-KNOTS INTO RKN(25),...
		II0=25
		DO 20 II=II0,II0+IKN(3)-9
		    WRITE(LL,2912) II-II0+1
 2912               FORMAT(/2X,'Y-KNOT NO.',I2,'?')
		    CALL ZREAD(.TRUE.,LINE,FLAG,*268)
		    READ(LINE,RFORM,ERR=268) RKN(II)
		    IF(RKN(II).LT.RDD(5).OR.RKN(II).GT.RDD(6))THEN
			WRITE(LL,2913)
			GOTO 268
		    ENDIF
		    IF(RKN(II).LT.RKN(II-1))THEN
			WRITE(LL,2914)
			GOTO 268
		    ENDIF
   20           CONTINUE
C
	      ENDIF
C
C
C
	  ELSE
	      GOTO 266
	  ENDIF
C
C
C
      ELSE
	  GOTO 261
      ENDIF
C
      RETURN
C
 2913 FORMAT(//2X,'+++ THIS KNOT LIES OUTSIDE THE DATA REGION')
 2914 FORMAT(//2X,'+++ THE KNOT POSITIONS MUST BE ENTERED IN ',
     *            'INCREASING ORDER')
C
C     **
C     **    END OF SUBROUTINE QKN
C     **
C
      END
C+*+* QKN    END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* QPS    START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE QPS
C
C     ******************************************************************
C     *                                                                *
C     *      THIS SUBROUTINE ASKS QUESTIONS ABOUT THE PLOTTING         *
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
C     SUBPROGRAM QPS    CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     ABORT   QPSC    QPSI    RSERR   ZREAD
C
C     SUBPROGRAM QPS    IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     SSU10
C
C     LOCAL VARIABLES
C     ***************
      CHARACTER ANS,LINE*80,IFORM*7,RFORM*10
      INTEGER FLAG
      REAL*8 SCALE,SFACT,PHI,XLEN,YLEN,PIBY4
C
C     EQUIVALENCED VARIABLES
C     **********************
      LOGICAL REPEAT,BATCH
      EQUIVALENCE (REPEAT,LGSV(8)),(BATCH,LGSV(10))
C
      IFORM='(BN,I6)'
      RFORM='(BN,F16.0)'
C
CCC   SET FLAGS FOR PFCTR,ETC -- if in REPEAT mode and
CCC   PFCST exists, retain it by leaving IN(6) unchanged
C
      IN(4)=0
      IN(5)=0
      IF( .NOT. REPEAT ) IN(6)=0
      IN(7)=0
      IN(8)=0
      IN(9)=0
C@@ C
C@@ C     SET PLOTTING DEVICE
C@@ C
C@@       IF( ICS(1).LE.2 ) THEN
C@@ C         BAND CONTOURS ON LINEPRINTER -- NO DEVICE SELECTED
C@@           IPS(1)=0
C@@       ELSEIF( .NOT. BATCH ) THEN
C@@ C         SINGLE JOB MODE
C@@ C         PLOTS TO TEKTRONIX INITIALLY (AT SALFORD)
C@@           IPS(1)=3
C@@       ELSE
C@@ C         BATCH MODE
C@@ C         PLOTS DIRECT TO CALCOMP
C@@           IPS(1)=2
C@@       ENDIF
C@@C
C
C     SET PLOTTING DEVICE FLAG
C
      IF( ICS(1).LE.2 ) THEN
C         BAND CONTOURS ON LINEPRINTER -- NO DEVICE SELECTED
	  IPS(1)=1
      ELSE
C         NOT BAND CONTOURS ON LINEPRINTER
	  IPS(1)=2
      ENDIF
C@@ C
C
C
 301  CONTINUE
      IF(QNO.EQ.301)CALL RSERR(FLAG)
      QNO=301
      WRITE(LL,2301)
 2301 FORMAT(//2X,'*** Q301 ***'/2X,'DO YOU WANT TO USE THE TITLE ',
     *    'FROM THE SOURCE FILE AS THE PLOT TITLE?')
      CALL ZREAD(.TRUE.,LINE,FLAG,*301)
      READ(LINE,'(A)') ANS
      IF( ANS.EQ.' ' ) ANS='Y'
      IF( ANS.EQ.'Y' ) THEN
	  IPS(10)=1
      ELSEIF( ANS.EQ.'N' ) THEN
	  IPS(10)=0
      ELSE
	  GOTO 301
      ENDIF
C
      IF( ICS(1).EQ.1 .OR. ICS(1).EQ.2 ) THEN
C         LP CONTOURERS , OTHER PLOTTING DETAILS NOT REQUIRED
	  CONTINUE
      ELSEIF( ICS(1).EQ.6 ) THEN
C         ISOMETRIC PROJ
	  CALL QPSI
      ELSE
C         PROPER CONTOURER
	  CALL QPSC
      ENDIF
C
C
C
      IF( ICS(1).EQ.1 .OR. ICS(1).EQ.2 ) THEN
C         LINE-PRINTER CONTOURERS
	  IPS(2)=0
	  SFACT=1.0
C         set up defaults for x and y axis lengths
	  RPS(1)=150.0
	  RPS(2)=120.0
      ELSE
C         allow for margin round plot
	  SFACT=1.0/1.15
C         set default x/y axis lengths to zero, reset in SSSU41
C         if still default
	  RPS(1)=0
	  RPS(2)=0
      ENDIF
C
C
      XLEN=RCS(2)-RCS(1)
      YLEN=RCS(4)-RCS(3)
C
C
 302  CONTINUE
      IF( QNO.EQ.302 ) CALL RSERR(FLAG)
      QNO=302
      WRITE(LL,2302)
 2302 FORMAT(//2X,'*** Q302 ***'/2X,'DO YOU WANT THE DEFAULT PLOT',
     *      ' SIZE?')
      CALL ZREAD(.TRUE.,LINE,FLAG,*302)
      READ(LINE,'(A)') ANS
      IF( ANS.EQ.' ' ) ANS='Y'
      IF( ANS.EQ.'Y' ) THEN
C
CCC      DEFAULT PLOT SIZE
C
	 IPS(25)=0
	 PIBY4=ATAN(1.0)
C
C
	 IF( ICS(1).NE.6 ) THEN
C           CONTOURS OR DATA PTS ONLY (NOT ISO)
C
C
	    IF( IDD(8).EQ.0 .AND. IPS(2).EQ.0 ) THEN
C               LAT-LONG COORDS , NO PROJECTION
C               CHOOSE SCALE FACTOR BASED ON AVERAGE LATITUDE AND
C               SUCH THAT LINEAR DISTANCES CORRESPOND
		PHI=(RCS(3)+RCS(4))/2.0
		SCALE=1.0/COS(PHI*PIBY4/45.0)
		SCALE=1.0/SCALE
	    ELSEIF( IDD(8).EQ.0 .AND. IPS(2).EQ.1 ) THEN
C               LAT-LONG COORDS , MERCATOR PROJECTION
		SCALE=45.0/PIBY4*LOG( TAN(PIBY4*(1.0+0.5*RCS(4)/45.0))/
     *               TAN(PIBY4*(1.0+0.5*RCS(3)/45.0)) )/YLEN
		SCALE=1.0/SCALE
	    ELSEIF( IDD(8).EQ.1 .AND. IPS(2).EQ.0 ) THEN
C               X-Y COORDS
C               CHOOSE EQUAL SCALING
		SCALE=1
	    ELSE
		WRITE(LL,2999)
		WRITE(LR,2999)
 2999           FORMAT(//2X,'+++ UNEXPECTED ERROR -- INCONSISTENT ',
     *                  'COORDS/MERCATOR PROJECTION IN QPS +++')
		CALL ABORT
	    ENDIF
C
C
C
C
C
	 ELSE
C           ISOMETRIC -- GIVE SQUARE REGION
	    SCALE=XLEN/YLEN
	 ENDIF
C
      ELSEIF( ANS.EQ.'N' ) THEN
C
CCC      USER SELECTED PLOT SIZE
C
	 IPS(25)=1
C
C
C
 303     CONTINUE
	 IF( QNO.EQ.303 ) CALL RSERR(FLAG)
	 QNO=303
	 WRITE(LL,2303)
 2303    FORMAT(//2X,'*** Q303 ***'/2X,'TYPE IN X-LENGTH(MM)')
	 CALL ZREAD(.TRUE.,LINE,FLAG,*303)
	 READ(LINE,RFORM,ERR=303) RPS(1)
	 IF( RPS(1).LT.1E-6 ) GOTO 303
C
	 IF( IPS(2).EQ.1 ) THEN
C            MERCATOR PROJECTION - RELATIVE SCALES IMPOSED
	     PIBY4=ATAN(1.0)
	     SCALE=45.0/PIBY4*LOG( TAN(PIBY4*(1.0+0.5*RCS(4)/45.0)) /
     *            TAN(PIBY4*(1.0+0.5*RCS(3)/45.0)) )/(RCS(4)-RCS(3))
	     SCALE=1.0/SCALE
	 ENDIF
C
C
	 IF( IPS(2).EQ.1 ) THEN
C            MERCATOR
	     RPS(2)=YLEN*SCALE*RPS(1)/XLEN
	 ELSE
C
C
C
 304         CONTINUE
	     IF( QNO.EQ.304 ) CALL RSERR(FLAG)
	     QNO=304
	     WRITE(LL,2304)
 2304        FORMAT(//2X,'*** Q304 ***'/2X,'TYPE IN Y-LENGTH(MM)')
	     CALL ZREAD(.TRUE.,LINE,FLAG,*304)
	     READ(LINE,RFORM,ERR=304) RPS(2)
	     IF( RPS(2).LT.1E-6 ) GOTO 304
C             SCALE=RPS(1)/RPS(2)
CCC          SET SCALE TO GIVE USER SPECIFIED RPS(3)
C
	     SCALE=RPS(1)/RPS(2)*(RCS(4)-RCS(3))/(RCS(2)-RCS(1))
	 ENDIF
C
C
C
      ELSE
	  GOTO 302
      ENDIF
C
C
C
      IF( ICS(1).EQ.1 .OR. ICS(1).EQ.2 ) THEN
C         convert from mm to inches
	  RPS(1)=RPS(1)/25.4
	  RPS(2)=RPS(2)/25.4
      ENDIF
C
CCCC  SET RATIO XLENGTH/YLENGTH OF MAIN PLOTTING BOX
C
      RPS(3)=(RCS(2)-RCS(1))*SCALE/(RCS(4)-RCS(3))
C
      RETURN
C
C     **
C     **    END OF SUBROUTINE QPS
C     **
C
      END
C+*+* QPS    END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* QPSC   START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE QPSC
C
C     ******************************************************************
C     *                                                                *
C     *      THIS SUBROUTINE ASKS QUESTIONS ABOUT PLOTTING OPTIONS     *
C     *      RELEVANT TO CONTOURING ( RATHER THAN ISOMETRIC PROJ)      *
C     *                                                                *
C     *          +++ HOST DEPENDENT SUBROUTINE +++                     *
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
C     SUBPROGRAM QPSC   CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     ABORT   CHKFN   CSTLN   DLETE   FLNAM   QPOLY   RSERR   ZREAD
C
C     SUBPROGRAM QPSC   IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     QPS
C
C     LOCAL VARIABLES
C     ***************
      CHARACTER*30 NAME
      CHARACTER ANS,LINE*80,IFORM*7,RFORM*10
      LOGICAL EXISTS,EMPTY
      INTEGER FLAG,IANS
C
      IFORM='(BN,I6)'
      RFORM='(BN,F16.0)'
C
CCC   SET UP X-AXIS(HORIZONTAL) AND Y-AXIS(VERTICAL) TITLES
C
      IPS(27)=1
      IPS(26)=1
C
      IF( IDD(8).EQ.0 ) THEN
C         DATA IS LAT/LONG TYPE
	  XTITLE='LONGITUDE'
	  ZTITLE='LATITUDE'
      ELSE
C         ABSTRACT X-Y DATA
C
C
C
 319      CONTINUE
	  IF( QNO.EQ.319 ) CALL RSERR(FLAG)
	  QNO=319
	  WRITE(LL,2319)
 2319     FORMAT(//2X,'*** Q319 ***'/2X,
     *               'TYPE IN A TITLE FOR THE X-AXIS')
	  CALL ZREAD(.FALSE.,LINE,FLAG,*319)
	  READ(LINE,'(A)') XTITLE
C
C
C
 320      CONTINUE
	  IF( QNO.EQ.320 ) CALL RSERR(FLAG)
	  QNO=320
	  WRITE(LL,2320)
 2320     FORMAT(//2X,'*** Q320 ***'/2X,
     *               'TYPE IN A TITLE FOR THE Y-AXIS')
	  CALL ZREAD(.FALSE.,LINE,FLAG,*320)
	  READ(LINE,'(A)') ZTITLE
C
      ENDIF
C
C
C
      IF( IDD(8).EQ.0 ) THEN
C           LAT/LONG TYPE DATA
C
C
C
 321      CONTINUE
	  IF( QNO.EQ.321 ) CALL RSERR(FLAG)
	  QNO=321
	  WRITE(LL,2321)
 2321     FORMAT(//2X,'*** Q321 ***'/2X,
     *               'DO YOU WANT THE COASTLINE PLOT?')
	  CALL ZREAD(.TRUE.,LINE,FLAG,*321)
	  READ(LINE,'(A)') ANS
	  IF( ANS.EQ.' ' ) ANS='Y'
	  IF( ANS.EQ.'Y' ) THEN
	      IPS(3)=1
C
C
C
  322         CONTINUE
	      IF( QNO.EQ.322 ) CALL RSERR(FLAG)
	      QNO=322
	      WRITE(LL,2322)
 2322         FORMAT(//2X,'*** Q322 ***',/2X,'TYPE IN THE NAME OF THE ',
     *                'COASTLINE FILE'
     *               /2X,'THE COASTLINE DATA SHOULD BE IN THE STANDARD',
     *                ' MAFF FORMAT')
	      CALL ZREAD(.TRUE.,LINE,FLAG,*322)
	      READ(LINE,'(A)') NAMES(5)
	      CALL CHKFN(NAMES(5),*322)
	      INQUIRE(FILE=NAMES(5),EXIST=EXISTS,ERR=322)
	      IF( .NOT.EXISTS ) THEN
		  WRITE(LL,2901)
 2901             FORMAT(//'+++ FILE DOES NOT EXIST +++')
C                 return to Q321 so that can choose to abandon
C                 coastline plot
		  QNO=321
		  GOTO 321
	      ENDIF
C
C
C
  323         CONTINUE
	      IF( QNO .EQ. 323 ) CALL RSERR(FLAG)
	      QNO=323
	      WRITE(LL,2323)
 2323         FORMAT(//2X,'*** Q323 ***'/2X,'DO YOU WANT TO EXTRACT '
     *          ,'COASTLINE DATA RELEVANT TO '
     *          /2X,' YOUR CONTOURING/PLOTTING REGION?')
	      CALL ZREAD(.TRUE.,LINE,FLAG,*323)
	      READ(LINE,'(A)') ANS
	      IF( ANS.EQ.' ' ) ANS='Y'
C---- temp fix
	      IF( ANS.EQ.'Y' ) THEN
		 IF( IN(15).EQ. 0 ) THEN
C                   interactive input
		    WRITE(LL,'(//2X,A)') 'This option is not available',
     *                 ' at present, response NO assumed'
		    ANS='N'
		 ELSE
C                   input from CF file -- abandon with a message
		    WRITE(LR,6635)
 6635               FORMAT(//2X,'Coastline extraction not currently',
     *                ' available,'//2X,'Job abandoned')
		    CALL ABORT
		 ENDIF
	      ENDIF
C---- end of temp fix
	      IF( ANS.EQ.'Y' ) THEN
C
CCCC              ASK FOR THE NAME OF THE FILE
C
		  CALL FLNAM(NAMES(6),1)
C
C
		  CALL CSTLN(EMPTY)
C
C
		  IF( EMPTY ) THEN
		      CALL DLETE(NAMES(6),LL)
C
C
C
  326                 CONTINUE
		      IF( QNO.EQ.326 ) CALL RSERR(FLAG)
		      QNO=326
		      WRITE(LL,2326)
 2326                 FORMAT(//2X,'+++ ERROR -- COASTLINE FILE HAS ',
     *                   'NOT PROVIDED ANY COASTLINE SEGMENTS '/6X,
     *                   ' WITHIN THE REGION OF INTEREST',
     *                    //2X,'*** Q326 ***'/2X,'DO YOU WANT TO RE-',
     *                   'SPECIFY THE SOURCE COASTLINE FILE?')
		      CALL ZREAD(.TRUE.,LINE,FLAG,*326)
		      READ(LINE,'(A)') ANS
		      IF( ANS.EQ.' ' ) ANS='Y'
		      IF( ANS.EQ.'Y' ) THEN
			  GOTO 322
		      ELSEIF( ANS.EQ.'N' ) THEN
			  IPS(3)=0
		      ELSE
			  GOTO 326
		      ENDIF
		  ENDIF
C
C
C
	      ELSEIF ( ANS.EQ.'N' ) THEN
		  NAMES(6)=NAMES(5)
	      ELSE
		  GOTO 323
	      ENDIF
C
C
C
	  ELSEIF( ANS.EQ.'N' ) THEN
	      IPS(3)=0
	  ELSE
	      GOTO 321
	  ENDIF
C
C
C
      ENDIF
C
C
C
  327 CONTINUE
      IF(QNO.EQ.327)CALL RSERR(FLAG)
      QNO=327
      WRITE(LL,2327)
 2327 FORMAT(//2X,'*** Q327 ***'/2X,
     *     'DO YOU ACCEPT THE DEFAULT PLOT OPTIONS FOR :',
     *     /'   PEN COLOURS, LABELLING, SAMPLE VALUES AND SYMBOLS?')
      CALL ZREAD(.TRUE.,LINE,FLAG,*327)
      READ(LINE,'(A)') ANS
      IF( ANS.EQ.' ' ) ANS='Y'
      IF( ANS.EQ.'Y' ) THEN
	  IPS(23)=0
C
C         SET DEFAULTS
C
C         ***  PEN COLOUR  ***
C              COASTLINE,CONTOUR,SAMPLE POSN(AND VALUE),ISO PROJ
	  IPS(4)=2
	  IPS(5)=1
	  IPS(9)=3
C
C         ***  SAMPLE PTS -- POSN, VALUE AND SYMBOL  ***
	  IF( ICS(1).EQ.7 ) THEN
C             NO CONTOURING
	      IPS(7)=2
	  ELSE
	      IPS(7)=0
	  ENDIF
C
	  IPS(8)=2
C
C         ***  MISCELLANEOUS  ***
C         SET DEFAULTS FOR ADDITIONAL DATA PTS
	  IPS(16)=0
	  IPS(17)=5
	  IPS(18)=3
C         CONTOUR LABELLING
	  IPS(6)=1
C
      ELSEIF( ANS.EQ.'N' ) THEN
	  IPS(23)=1
C
C
C
CCC       CHECK TO SEE IF CONTOURS ARE PLOTTED
	  IF( ICS(1).EQ.3 .OR. ICS(1).EQ.4 .OR. ICS(1).EQ.5 ) THEN
C
C
C
  328         CONTINUE
	      IF( QNO.EQ.328 ) CALL RSERR(FLAG)
	      QNO=328
	      WRITE(LL,2328)
 2328         FORMAT(//2X,'*** Q328 ***'
     *               /2X,'CHOOSE THE COLOUR FOR THE CONTOURS'
     *              //7X,'1  PEN 1 (BLACK)'
     *               /7X,'2  PEN 2 (GREEN)'
     *               /7X,'3  PEN 3 (RED)'
     *              //7X,'TYPE 1,2 OR 3 ')
	      CALL ZREAD(.TRUE.,LINE,FLAG,*328)
	      READ(LINE,IFORM,ERR=329) IPS(5)
	      IF( IPS(5).LT.1 .OR. IPS(5).GT.3 ) GOTO 328
C
C
C
  329         CONTINUE
	      IF(QNO.EQ.329)CALL RSERR(FLAG)
	      QNO=329
	      WRITE(LL,2329)
 2329         FORMAT(//2X,'*** Q329 ***'/2X,'DO YOU WANT TO LABEL THE',
     *              ' CONTOURS?')
	      CALL ZREAD(.TRUE.,LINE,FLAG,*329)
	      READ(LINE,'(A)') ANS
	      IF( ANS.EQ.'Y' ) THEN
		  IPS(6)=1
	      ELSEIF( ANS.EQ.'N' ) THEN
		  IPS(6)=0
	      ELSE
		  GOTO 329
	      ENDIF
C
C
	  ENDIF
C
C
C         IF COASTLINE IS TO BE PLOTTED
	  IF( IPS(3).EQ.1 ) THEN
C
C
  330         CONTINUE
	      IF(QNO.EQ.330)CALL RSERR(FLAG)
	      QNO=330
	      WRITE(LL,2330)
 2330         FORMAT(//2X,'*** Q330 ***'
     *           /2X,'CHOOSE THE COLOUR FOR THE COASTLINE'
     *          //7X,'1  PEN 1 (BLACK)'
     *           /7X,'2  PEN 2 (GREEN)'
     *           /7X,'3  PEN 3 (RED)'
     *          //7X,'TYPE 1,2 OR 3 ')
	      CALL ZREAD(.TRUE.,LINE,FLAG,*330)
	      READ(LINE,IFORM,ERR=330)IPS(4)
	      IF( IPS(4).LT.1 .OR. IPS(4).GT.3 ) GOTO 330
C
C
	  ENDIF
C
C
C
C         ************************************
C         ***  SOURCE DATA POINTS OPTIONS  ***
C         ************************************
C
CCCC      SELECT THE METHOD OF PLOTTING THE SOURCE DATA POINTS
C
  331     CONTINUE
	  IF (QNO.EQ.331) CALL RSERR(FLAG)
	  QNO=331
	  WRITE (LL,1000) QNO
	  WRITE (LL,2331)
 2331     FORMAT (2X,'SET THE SOURCE DATA POINT SWITCH'
     *      //7X,'0  NO SOURCE DATA POINTS ARE TO BE PLOTTED'
     *       /7X,'1  POSITION ONLY'
     *       /7X,'2  POSITION AND VALUE'
     *      //7X,'TYPE 0,1 OR 2')
	  CALL ZREAD(.TRUE.,LINE,FLAG,*331)
	  READ (LINE,IFORM,ERR=331) IANS
	  IF (IANS.LT.0 .OR. IANS.GT.2) GOTO 331
	  IF (IANS.EQ.0) THEN
	      IPS(7)=IANS
	      IPS(8)=0
	      IPS(9)=0
	      IN(7)=0
	  ELSE
	     IF (IPS(7).NE.IANS) THEN
		IN(7)=0
		IPS(7)=IANS
	     ENDIF
C
CCCC         SELECT THE COLOUR FOR THE SOURCE DATA POINTS
C
  332        CONTINUE
	     IF (QNO.EQ.332) CALL RSERR(FLAG)
	     QNO=332
	     WRITE (LL,1000) QNO
	     WRITE (LL,2332)
 2332        FORMAT (2X,'CHOOSE THE COLOUR FOR THE SOURCE DATA POINTS')
	     WRITE (LL,1001)
	     CALL ZREAD(.TRUE.,LINE,FLAG,*332)
	     READ (LINE,IFORM,ERR=332) IANS
	     IF (IANS.LT.1 .OR. IANS.GT.3) GOTO 332
	     IF (IANS.NE.IPS(9)) THEN
		IPS(9)=IANS
		IN(7)=0
	     ENDIF
C
CCCC         SELECT THE SYMBOL FOR THE SOURCE DATA POINTS
C
  333        CONTINUE
	     IF (QNO.EQ.333) CALL RSERR(FLAG)
	     QNO=333
	     WRITE (LL,1000) QNO
	     WRITE (LL,2333)
 2333        FORMAT (2X,'CHOOSE THE SYMBOL FOR THE SOURCE DATA POINTS',
     *          //7X,'TYPE 1,2,3,4 OR 5')
	     CALL ZREAD(.TRUE.,LINE,FLAG,*333)
	     READ (LINE,IFORM,ERR=333) IANS
	     IF (IANS.LT.1 .OR. IANS.GT.5) GOTO 333
	     IF (IANS.NE.IPS(8)) THEN
		IPS(8)=IANS
		IN(7)=0
	     ENDIF
	  ENDIF
C
C         *******************************************
C         ***  END OF SOURCE DATA POINTS OPTIONS  ***
C         *******************************************
C
C
C         ****************************************
C         ***  ADDITIONAL DATA POINTS OPTIONS  ***
C         ****************************************
C
CCCC      SELECT THE METHOD OF PLOTTING THE ADDITIONAL DATA POINTS
C
  334     CONTINUE
	  IF (QNO.EQ.334) CALL RSERR(FLAG)
	  QNO=334
	  WRITE (LL,1000) QNO
	  WRITE (LL,2334)
 2334     FORMAT (2X,'SET SWITCH FOR SUPERIMPOSING ADDITIONAL ',
     *                                             'DATA POINTS'
     *      //7X,'0  NO ADDITIONAL DATA POINTS ARE TO BE PLOTTED'
     *       /7X,'1  POSITION ONLY'
     *       /7X,'2  POSITION AND VALUE'
     *      //7X,'TYPE 0, 1 OR 2')
	  CALL ZREAD(.TRUE.,LINE,FLAG,*334)
	  READ (LINE,IFORM,ERR=334) IANS
	  IF (IANS.LT.0 .OR. IANS.GT.2) GOTO 334
	  IF (IANS.EQ.0) THEN
	      IPS(16)=IANS
	      IPS(17)=0
	      IPS(18)=0
	      IN(8)=0
	  ELSE
	     IF (IPS(16).NE.IANS) THEN
		IN(8)=0
		IPS(16)=IANS
	     ENDIF
C
CCCC         SPECIFY THE NAME OF THE FILE WHICH IS TO HOLD THE
CCCC         ADDITIONAL DATA POINTS.
CCCC
CCCC         +++ NOTE +++
CCCC         IT IS ASSUMED THAT THE RECORDS IN THIS FILE EACH CONTAIN AN
CCCC         X-VALUE FOLLOWED BY A Y-VALUE FOLLOWED BY A FUNCTION VALUE
C
  335        CONTINUE
	     IF (QNO.EQ.335) CALL RSERR(FLAG)
	     QNO=335
	     WRITE (LL,1000) QNO
	     WRITE (LL,2335)
 2335        FORMAT (2X,'TYPE THE NAME OF THE FILE WHICH HOLDS THE',
     *          ' ADDITIONAL DATA POINTS'//10X,'*** NOTE ***'
     *          /10X,'THE DATA IN THIS FILE MUST BE HELD IN THE FORM'
     *          /15X,'X-VALUE   Y-VALUE   FUNCTION-VALUE')
	     CALL ZREAD(.TRUE.,LINE,FLAG,*335)
	     READ (LINE,'(A)',ERR=335) NAME
	     CALL CHKFN(NAME,*335)
	     INQUIRE (FILE=NAME,EXIST=EXISTS)
	     IF (.NOT. EXISTS) THEN
		WRITE (LL,1007)
		GOTO 335
	     ENDIF
	     NAMES(7)=NAME
C
CCCC         SELECT THE COLOUR FOR THE ADDITIONAL DATA POINTS
C
  336        CONTINUE
	     IF (QNO.EQ.336) CALL RSERR(FLAG)
	     QNO=336
	     WRITE (LL,1000) QNO
	     WRITE (LL,2336)
 2336        FORMAT (2X,'CHOOSE THE COLOUR FOR THE ',
     *                              'ADDITIONAL DATA POINTS')
	     WRITE (LL,1001)
	     CALL ZREAD(.TRUE.,LINE,FLAG,*336)
	     READ (LINE,IFORM,ERR=336) IANS
	     IF (IANS.LT.1 .OR. IANS.GT.3) GOTO 336
	     IF (IANS.NE.IPS(18)) IPS(18)=IANS
C
CCCC         SELECT THE SYMBOL FOR THE ADDITIONAL DATA POINTS
C
  337        CONTINUE
	     IF (QNO.EQ.337) CALL RSERR(FLAG)
	     QNO=337
	     WRITE (LL,1000) QNO
	     WRITE (LL,2337)
 2337        FORMAT (2X,'CHOOSE THE SYMBOL FOR THE ',
     *          'ADDITIONAL DATA POINTS'
     *          /7X,'TYPE 1,2,3,4 OR 5')
	     CALL ZREAD(.TRUE.,LINE,FLAG,*337)
	     READ (LINE,IFORM,ERR=337) IANS
	     IF (IANS.LT.1 .OR. IANS.GT.5) GOTO 337
	     IF (IANS.NE.IPS(17)) THEN
		IPS(17)=IANS
		IN(8)=0
	     ENDIF
	  ENDIF
C
C         ***********************************************
C         ***  END OF ADDITIONAL DATA POINTS OPTIONS  ***
C         ***********************************************
C
      ELSE
	  GOTO 327
      ENDIF
C
C
C
  338 CONTINUE
      IF(QNO.EQ.338)CALL RSERR(FLAG)
      QNO=338
      WRITE(LL,2338)
 2338 FORMAT(//2X,'*** Q338 ***'/2X,
     *     'DO YOU ACCEPT THE DEFAULT PLOT OPTIONS FOR :',
     *     /'   TICK MARKS AND ANNOTATION?')
      CALL ZREAD(.TRUE.,LINE,FLAG,*338)
      READ(LINE,'(A)') ANS
      IF( ANS.EQ.' ' ) ANS='Y'
      IF( ANS.EQ.'Y' ) THEN
	  IPS(24)=0
C
C         SET DEFAULTS
C
C         ***  TICK MARKS AND FREQ  ***
	  IPS(12)=1
	  IPS(19)=0
	  IPS(20)=0
C
C         ***  ANNOTATION  ***
	  IF( IDD(8).EQ.1 ) THEN
C             ABSTRACT X-Y DATA
	      IPS(11)=2
	  ELSE
C             LAT-LONG DATA
	      IPS(11)=3
	  ENDIF
C
C
      ELSEIF( ANS.EQ.'N' ) THEN
	  IPS(24)=1
C
C
C
  339     CONTINUE
	  IF(QNO.EQ.339)CALL RSERR(FLAG)
	  QNO=339
	  WRITE(LL,2339)
 2339     FORMAT(//2X,'*** Q339 ***'/2X,'DO YOU WANT TICK MARKS?')
	  CALL ZREAD(.TRUE.,LINE,FLAG,*339)
	  READ(LINE,'(A)') ANS
	  IF( ANS.EQ.' ' ) ANS='Y'
	  IF( ANS.EQ.'Y' ) THEN
	      IPS(12)=1
C
C
C
  340         CONTINUE
	      IF(QNO.EQ.340)CALL RSERR(FLAG)
	      QNO=340
	      WRITE(LL,2340)
 2340         FORMAT(//2X,'*** Q340 ***'
     *            /2X,'ENTER THE NUMBER OF TICK MARKS ON THE X-AXIS'
     *            /10X,'ENTER 0 IF THIS IS TO BE PROGRAM-SELECTED')
	      CALL ZREAD(.TRUE.,LINE,FLAG,*340)
	      READ(LINE,IFORM,ERR=340) IPS(19)
	      IF (IPS(19).LT.0) GOTO 340
C
C
C
  341         CONTINUE
	      IF(QNO.EQ.341)CALL RSERR(FLAG)
	      QNO=341
	      WRITE(LL,2341)
 2341         FORMAT(//2X,'*** Q341 ***'
     *            /2X,'ENTER THE NUMBER OF TICK MARKS ON THE Y-AXIS'
     *            /10X,'ENTER 0 IF THIS IS TO BE PROGRAM-SELECTED')
	      CALL ZREAD(.TRUE.,LINE,FLAG,*341)
	      READ(LINE,IFORM,ERR=341) IPS(20)
	      IF (IPS(20).LT.0) GOTO 341
C
C
C
  342         CONTINUE
	      IF(QNO.EQ.342)CALL RSERR(FLAG)
	      QNO=342
	      WRITE(LL,2342)
 2342         FORMAT(//2X,'*** Q342 ***'
     *            /2X,'CHOOSE THE TICK MARK ANNOTATION'
     *           //7X,'1  NO ANNOTATION '
     *            /7X,'2  NORMAL X-Y COORDINATES'
     *            /7X,'3  DECIMAL DEGREES'
     *            /7X,'4  DEGREES, DECIMAL MINUTES'
     *            /7X,'5  DEGREES, MINUTES, SECONDS'
     *           //7X,'TYPE 1,2,3,4 OR 5')
	      CALL ZREAD(.TRUE.,LINE,FLAG,*342)
	      READ(LINE,IFORM,ERR=342) IPS(11)
	      IF( IPS(11).LT.1 .OR. IPS(11).GT.5) GOTO 342
C
C
C
	  ELSEIF( ANS.EQ.'N' ) THEN
	      IPS(12)=0
	  ELSE
	      GOTO 339
	  ENDIF
C
C
C
      ELSE
C         ERROR
	  GOTO 338
      ENDIF
C
C
C
 343  CONTINUE
      IF(QNO.EQ.343) CALL RSERR(FLAG)
      QNO=343
      WRITE(LL,2343)
 2343 FORMAT(//2X,'*** Q343 ***'/2X,'DO YOU WANT TO USE THE POLYGON ',
     *      'PLOTTING OPTION?')
      CALL ZREAD(.TRUE.,LINE,FLAG,*343)
      READ(LINE,'(A)') ANS
      IF( ANS.EQ.' ' ) ANS='Y'
      IF( ANS.EQ.'Y' ) THEN
C         POLYGON OPTION SELECTED
	  CALL QPOLY
      ELSEIF( ANS.EQ.'N' ) THEN
C         POLYGON OPTION NOT SELECTED
	  IPS(14)=0
      ELSE
C         ERROR
	  GOTO 343
      ENDIF
C
C
C
      IF( IDD(8).EQ.0 ) THEN
C         LAT/LONG COORDS
C
C
C
  344     CONTINUE
	  IF( QNO.EQ.344 ) CALL RSERR(FLAG)
	  QNO=344
	  WRITE(LL,2344)
 2344     FORMAT(//2X,'*** Q344 ***'/2X,
     *         'DO YOU REQUIRE A MERCATOR PROJECTION?')
	  CALL ZREAD(.TRUE.,LINE,FLAG,*344)
	  READ(LINE,'(A)') ANS
	  IF( ANS.EQ.' ' ) ANS='Y'
	  IF( ANS.EQ.'Y' ) THEN
	      IPS(2)=1
	  ELSEIF( ANS.EQ.'N' ) THEN
	      IPS(2)=0
	  ELSE
C             ERROR
	      GOTO 344
	  ENDIF
      ELSE
C         ABSTRACT COORDS -- ASSUME NO PROJECTION
	  IPS(2)=0
      ENDIF
C
C
C
      IF( ICS(1).NE.7 )THEN
C
CCC       WRITE THE RESULT OF THE CENTRE OF MASS CALCULATION TO THE
CCC       REPORT FILE IF THIS OPTION IS SPECIFIED. IT IS ONLY AVAILABLE
CCC       FOR THE NAG-REGULAR, THE NAG-SCATTERED OR THE HARWELL
CCC       CONTOURERS. IT IS NOT AVAILABLE IF PLOTTING IS WITHIN A
CCC       POLYGON PLOTTING BOX
C
C
  345     CONTINUE
	  IF (QNO.EQ.345) CALL RSERR(FLAG)
	  QNO=345
	  WRITE (LL,1000) QNO
	  WRITE (LL,2345)
 2345     FORMAT (2X,'DO YOU WANT THE RESULT OF THE AREA AND ',
     *       'CENTRE OF MASS',
     *       /2X,'CALCULATION SENT TO THE REPORT FILE?')
	  CALL ZREAD(.TRUE.,LINE,FLAG,*345)
	  READ (LINE,'(A)') ANS
	  IF (ANS.EQ.' ') ANS='Y'
	  IF (ANS.EQ.'Y') THEN
	      IN(12)=1
	  ELSEIF (ANS.EQ.'N') THEN
	      IN(12)=0
	  ELSE
	      GOTO 345
	  ENDIF
C
C
      ENDIF
C
C
      RETURN
C
C
 1000 FORMAT (//2X,'*** Q',I3,' ***')
 1001 FORMAT (//7X,'1  PEN 1 (BLACK)'
     *         /7X,'2  PEN 2 (GREEN)'
     *         /7X,'3  PEN 3 (RED)'
     *        //7X,'TYPE 1,2 OR 3')
 1007 FORMAT (//' +++ THIS FILE DOES NOT EXIST +++')
C
C     **
C     **    END OF SUBROUTINE QPSC
C     **
C
      END
C+*+* QPSC   END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* QPSI   START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE QPSI
C
C     ******************************************************************
C     *                                                                *
C     *      THIS SUBROUTINE ASKS QUESTIONS ABOUT PLOTTING OPTIONS     *
C     *      RELEVANT TO ISOMETRIC PROJ'NS ( RATHER THAN CONTOURING)   *
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
C     SUBPROGRAM QPSI   CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     RSERR   ZREAD
C
C     SUBPROGRAM QPSI   IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     QPS
C
C     LOCAL VARIABLES
C     ***************
      CHARACTER ANS,LINE*80,IFORM*7,RFORM*10
      INTEGER FLAG
C
      IFORM='(BN,I6)'
      RFORM='(BN,F16.0)'
C
CCC   SET UP X-AXIS AND Y-AXIS TITLES
C
      IPS(28)=1
      IPS(29)=1
C
      IF( IDD(8).EQ.0 ) THEN
C         DATA IS LAT/LONG TYPE
	  XTITLE='LONGITUDE'
	  YTITLE='LATITUDE'
C
	  XUNITS='DEG'
	  YUNITS='DEG'
      ELSE
C         ABSTRACT X-Y DATA
C
C
C
 355      CONTINUE
	  IF( QNO.EQ.355 ) CALL RSERR(FLAG)
	  QNO=355
	  WRITE(LL,2355)
 2355     FORMAT(//2X,'*** Q355 ***'/2X,
     *               'TYPE IN A TITLE FOR THE X-AXIS')
	  CALL ZREAD(.FALSE.,LINE,FLAG,*355)
	  READ(LINE,'(A)') XTITLE
C
C
C
 356      CONTINUE
	  IF( QNO.EQ.356 ) CALL RSERR(FLAG)
	  QNO=356
	  WRITE(LL,2356)
 2356     FORMAT(//2X,'*** Q356 ***'/2X,
     *               'SPECIFY THE UNITS FOR THE X COORDINATE')
	  CALL ZREAD(.FALSE.,LINE,FLAG,*356)
	  READ(LINE,'(A)') XUNITS
C
C
C
 357      CONTINUE
	  IF( QNO.EQ.357 ) CALL RSERR(FLAG)
	  QNO=357
	  WRITE(LL,2357)
 2357     FORMAT(//2X,'*** Q357 ***'/2X,
     *               'TYPE IN A TITLE FOR THE Y-AXIS')
	  CALL ZREAD(.FALSE.,LINE,FLAG,*357)
	  READ(LINE,'(A)') YTITLE
C
C
C
 358      CONTINUE
	  IF( QNO.EQ.358 ) CALL RSERR(FLAG)
	  QNO=358
	  WRITE(LL,2358)
 2358     FORMAT(//2X,'*** Q358 ***'/2X,
     *               'SPECIFY THE UNITS FOR THE Y COORDINATE')
	  CALL ZREAD(.FALSE.,LINE,FLAG,*358)
	  READ(LINE,'(A)') YUNITS
C
      ENDIF
C
CCC   SET UP Z-AXIS(VERTICAL) TITLE
C
      IPS(26)=1
C
 359  CONTINUE
      IF( QNO.EQ.359 ) CALL RSERR(FLAG)
      QNO=359
      WRITE(LL,2359)
 2359 FORMAT(//2X,'*** Q359 ***'/2X,
     *           'TYPE IN A TITLE FOR THE Z-AXIS')
      CALL ZREAD(.FALSE.,LINE,FLAG,*359)
      READ(LINE,'(A)') ZTITLE
C
C
C
 360  CONTINUE
      IF( QNO.EQ.360 ) CALL RSERR(FLAG)
      QNO=360
      WRITE(LL,2360)
 2360 FORMAT(//2X,'*** Q360 ***'/2X,
     *           'SPECIFY THE UNITS FOR THE Z COORDINATE')
      CALL ZREAD(.FALSE.,LINE,FLAG,*360)
      READ(LINE,'(A)') ZUNITS
C
C
C
  361 CONTINUE
      IF( QNO .EQ. 361 ) CALL RSERR(FLAG)
      QNO=361
      WRITE(LL,2361)
 2361 FORMAT(//2X,'*** Q361 ***'/2X,'DO YOU WANT TO VIEW THE ',
     *   'PROJECTION FROM '/
     *   /7X,'1  THE NORTH EAST?'
     *   /7X,'2  THE NORTH WEST?'
     *   /7X,'3  THE SOUTH WEST?'
     *   /7X,'4  THE SOUTH EAST?'
     *   //7X,'TYPE 1,2,3 OR 4')
      CALL ZREAD(.TRUE.,LINE,FLAG,*361)
      READ(LINE,IFORM,ERR=361) IPS(13)
      IF ( IPS(13).LT.1 .OR. IPS(13).GT.4 ) GOTO 361
      IPS(13)=IPS(13)-1
C
C
  362 CONTINUE
      IF(QNO.EQ.362)CALL RSERR(FLAG)
      QNO=362
      WRITE(LL,2362)
 2362 FORMAT(//2X,'*** Q362 ***'/2X,
     *     'DO YOU ACCEPT THE DEFAULT PLOT OPTIONS FOR :',
     *     /'   PEN COLOUR, HEIGHT FACTOR AND MIN/MAX HEIGHT?')
      CALL ZREAD(.TRUE.,LINE,FLAG,*362)
      READ(LINE,'(A)') ANS
      IF( ANS.EQ.' ' ) ANS='Y'
C
CCC   NEED TO SET MIN/MAX HT SINCE USER-SPECIFIED ONLY IN THE REDRAW
C
C     ***  MIN/MAX HEIGHT  ***
C          SET TO ZERO -- VALID OPTION
      RPS(11)=0.0
      RPS(12)=0.0
C
      IF( ANS.EQ.'Y' ) THEN
C
C         SET DEFAULTS
C
C         ***  PEN COLOUR  ***
	  IPS(5)=1
C         ***  HEIGHT FACTOR  ***
	  RPS(10)=2.0
C
      ELSEIF( ANS.EQ.'N' ) THEN
C
CCCC      SELECT THE PEN COLOUR
C
  363     CONTINUE
	  IF (QNO.EQ.363) CALL RSERR(FLAG)
	  QNO=363
	  WRITE (LL,2363)
 2363     FORMAT(//2X,'*** Q363 ***'/2X,
     *          'CHOOSE THE COLOUR FOR THE ISOMETRIC PLOT'
     *          //7X,'1  PEN 1 (BLACK)'
     *           /7X,'2  PEN 2 (GREEN)'
     *           /7X,'3  PEN 3 (RED)'
     *          //7X,'TYPE 1,2 OR 3 ')
	  CALL ZREAD(.TRUE.,LINE,FLAG,*363)
	  READ(LINE,IFORM,ERR=363) IPS(5)
	  IF( IPS(5).LT.1 .OR. IPS(5).GT.3 ) GOTO 363
C
C
C
  364     CONTINUE
	  IF( QNO.EQ.364 ) CALL RSERR(FLAG)
	  QNO=364
	  WRITE(LL,2364)
 2364     FORMAT(//2X,'*** Q364 ***'/2X,
     *       'THE STANDARD DEFAULT VALUE FOR THE HEIGHT FACTOR IS 2.0',
     *      /'DECREASING THE HEIGHT FACTOR',
     *       ' EXAGGERATES THE APPEARANCE OF THE PLOT',
     *     //'TYPE IN A VALUE FOR THE ',
     *       ' HEIGHT FACTOR ')
	  CALL ZREAD(.TRUE.,LINE,FLAG,*364)
	  READ(LINE,RFORM,ERR=364) RPS(10)
	  IF ( RPS(10).LT.0 ) GOTO 364
C
C
C
      ELSE
C         ERROR
	  GOTO 362
      ENDIF
C
C
      RETURN
C
C     **
C     **    END OF SUBROUTINE QPSI
C     **
C
      END
C+*+* QPSI   END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* QPOLY  START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE QPOLY
C
C     ******************************************************************
C     *                                                                *
C     *      THIS SUBROUTINE ASKS QUESTIONS ABOUT THE POLYGON OPTION   *
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
C     SUBPROGRAM QPOLY  CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     ANTCLK  RSERR   ZREAD
C
C     SUBPROGRAM QPOLY  REFERENCES THE FOLLOWING FUNCTIONS
C     ****************************************************
      LOGICAL PLYOK
C
C     SUBPROGRAM QPOLY  IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     CREDR   QPSC
C
C     LOCAL VARIABLES
C     ***************
      CHARACTER LINE*80,IFORM*7,RFORM*10
      INTEGER FLAG,IANS,I,ICS8,ICS9,VTMAX
      REAL*8 RANS
C
      IFORM='(BN,I6)'
      RFORM='(BN,F16.0)'
C
CCCC  SET POINTERS TO FIRST X & Y VERTEX VALUES
C
      ICS8=31
      ICS9=81
C
      ICS(8)=ICS8
      ICS(9)=ICS9
C
CCCC  FIND MAX NO OF VERTICES ALLOWED
C
      VTMAX=ICS9-ICS8
C
CCCC  SELECT THE PLOTTING POLYGON SWITCH
C
  371 CONTINUE
      IF (QNO.EQ.371) CALL RSERR(FLAG)
      QNO=371
      WRITE (LL,2371)
 2371 FORMAT(//2X,'*** Q371 ***'/2X,
     *   'SET THE SWITCH FOR PLOTTING WITHIN A POLYGON'
     *  //7X,'0  PLOT CONTOURS WITHIN CONTOURING REGION'
     *   /7X,'1  PLOT CONTOURS WITHIN A SPECIFIED POLYGON'
     *   /7X,'2  AS 1, BUT DRAW THE POLYGON BOUNDARY AS WELL'
     *  //7X,'TYPE  0,1 OR 2')
      IF (IN(3).EQ.1) WRITE (LL,1002) IPS(14)
      CALL ZREAD(.TRUE.,LINE,FLAG,*371)
      READ (LINE,IFORM,ERR=371) IANS
      IF (IANS.LT.0 .OR. IANS.GT.2) GOTO 371
      IPS(14)=IANS
C
      IF (IPS(14).GT.0) THEN
C
CCCC     SPECIFY THE NUMBER OF VERTICES OF THE POLYGON
C
  372    CONTINUE
	 IF (QNO.EQ.372) CALL RSERR(FLAG)
	 QNO=372
	 WRITE (LL,2372) VTMAX
 2372    FORMAT(//2X,'*** Q372 ***'/2X,
     *      'TYPE IN THE NUMBER OF VERTICES OF THE POLYGON',
     *      ' (MINIMUM=3,MAXIMUM=',I2,')' )
	 CALL ZREAD(.TRUE.,LINE,FLAG,*372)
	 READ (LINE,IFORM,ERR=372) IANS
	 IF (IANS.LT.3 .OR. IANS.GT.VTMAX) GOTO 372
	 ICS(3)=IANS
C
CCCC     SELECT THE VERTICES FOR THE PLOTTING POLYGON.  IT IS ASSUMED
CCCC     THAT THE VERTICES ARE READ IN ORDER, THAT ALL VERTICES ARE
CCCC     IN THE PLOTTING AREA, AND THAT NO TWO SIDES OF THE POLYGON
CCCC     INTERSECT BETWEEN THEIR VERTICES
C
	 WRITE(LL,1008)
 1008    FORMAT(/2X,'SELECT THE VERTICES FOR THE PLOTTING POLYGON',
C    *      /2X,'THE VERTICES SHOULD BE :',
C    *      /2X,'  (A)   IN ANTICLOCKWISE ORDER',
C    *      /2X,'  (B)   WITHIN THE PLOTTING AREA'//)
     *      /2X,'THE VERTICES SHOULD BE WITHIN THE PLOTTING AREA'//)
C
CCCC     LOOP BACK TO 11 IF POLYGON FAILS TO CONFORM TO ABOVE
C
   11    CONTINUE
C
	 DO 12 I=1,ICS(3)
  373       CONTINUE
	    IF (QNO.EQ.373) CALL RSERR(FLAG)
	    QNO=373
	    WRITE (LL,2373) I
 2373       FORMAT (5X,'TYPE THE X-COORDINATE OF VERTEX ',I2)
	    CALL ZREAD(.TRUE.,LINE,FLAG,*373)
	    READ (LINE,RFORM,ERR=373) RANS
	    IF(RANS.LT.RCS(1).OR.RANS.GT.RCS(2))GOTO 11
	    RCS(I+ICS8-1)=RANS
C
  374       CONTINUE
	    IF (QNO.EQ.374) CALL RSERR(FLAG)
	    QNO=374
	    WRITE (LL,2374) I
 2374       FORMAT (5X,'TYPE THE Y-COORDINATE OF VERTEX ',I2)
	    CALL ZREAD(.TRUE.,LINE,FLAG,*374)
	    READ (LINE,RFORM,ERR=374) RANS
	    IF(RANS.LT.RCS(3).OR.RANS.GT.RCS(4))GOTO 11
	    RCS(I+ICS9-1)=RANS
   12    CONTINUE
C
CCCC     CHECK TO SEE IF 'VALID' POLYGON
C
	 IF (.NOT.PLYOK()) THEN
	    WRITE (LL,1009)
	    WRITE (LR,1009)
	    GOTO 11
	 ENDIF
C
CCCC     CHECK ORIENTATION OF POLYGON
C
	 CALL ANTCLK
      ENDIF
C
      RETURN
C
 1002 FORMAT (7X,'CURRENT VALUE IS ',I2)
 1009 FORMAT (//' +++ WARNING +++'
     *   /5X,' THE PACKAGE ASSUMES THAT'
C    *   /10X,'1. THE VERTICES ARE READ IN ANTICLOCKWISE ORDER'
     *   /10X,'1. ALL VERTICES LIE INSIDE THE PLOTTING AREA'
     *   /10X,'2. NO SIDES OF THE POLYGON INTERSECT BETWEEN VERTICES'
     *   /10X,'3. THE COORDINATES OF THE VERTICES ARE READ AS DECIMAL',
     *   ' NUMBERS'/5X,'IT IS THE RESPONSIBILITY OF THE USER TO MAKE',
     *   ' SURE THAT THIS IS SO')
C
C     **
C     **    END OF SUBROUTINE QPOLY
C     **
C
      END
C+*+* QPOLY  END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* ANTCLK START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
      SUBROUTINE ANTCLK
C
C     ******************************************************************
C     *                                                                *
C     *     SUBROUTINE ANTCLK                                          *
C     *           DETERMINES IF THE USER-SPECIFIED POLYGON IS          *
C     *           ORIENTATED IN A CLOCKWISE OR ANTICLOCKWISE MANNER.   *
C     *           THE ORDER OF THE VERTICES IS REVERSED IF THEY        *
C     *           HAVE BEEN ENTERED CLOCKWISE.                         *
C     *                                                                *
C     ******************************************************************
C
      IMPLICIT LOGICAL ( A-Z )
C
C     COMMON BLOCKS
C     ***************
C     CHARS  CNTRL
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
C------------------------------------------------------------------------
C
C     SUBPROGRAM ANTCLK IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     QPOLY
C
C     LOCAL VARIABLES
C     ***************
      INTEGER I,L,M,N,IX,IY
      REAL*8 TA,X1,Y1,X2,Y2,XF,YF
C
CCCC  INITIALISE POINTERS
C
      N =ICS(3)
      IX=ICS(8)
      IY=ICS(9)
C
CCCC  SUM CONTRIBUTION FROM EACH EDGE
C
      X1=RCS(IX)
      XF=X1
      Y1=RCS(IY)
      YF=Y1
C
      TA=0.0
      DO 10 I=1,N-1
	 X2=RCS(IX+I)
	 Y2=RCS(IY+I)
	 TA=TA+0.5*(X1+X2)*(Y2-Y1)
	 X1=X2
	 Y1=Y2
  10  CONTINUE
      X2=XF
      Y2=YF
      TA=TA+0.5*(X1+X2)*(Y2-Y1)
C
CCCC  TA > 0   --->   ANTICLOCKWISE
CCCC  TA < 0   --->   CLOCKWISE
C
      IF(TA.GT.0)RETURN
C
CCCC  REVERSE THE ORDER OF THE VERTICES
C
      M=N/2
      L=N-1
      DO 20 I=0,M-1
	 X1=RCS(IX+I)
	 Y1=RCS(IY+I)
	 RCS(IX+I)=RCS(IX+L)
	 RCS(IY+I)=RCS(IY+L)
	 RCS(IX+L)=X1
	 RCS(IY+L)=Y1
	 L=L-1
  20  CONTINUE
      RETURN
C
C     ***
C     ***   END OF SUBROUTINE ANTCLK
C     ***
C
      END
C
C
C+*+* ANTCLK END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* PLYOK  START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      LOGICAL FUNCTION PLYOK()
C
C     ******************************************************************
C     *                                                                *
C     *   THIS FUNCTION CHECKS THAT THE SIDES OF A POLYGON DO          *
C     *   NOT INTERSECT INTERNALLY. IF THEY DO, PLYOK IS RETURNED      *
C     *   AS .FALSE.                                                   *
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
C     SUBPROGRAM PLYOK  REFERENCES THE FOLLOWING FUNCTIONS
C     ****************************************************
      LOGICAL PLYCRS
C
C     SUBPROGRAM PLYOK  IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     QPOLY
C
C     LOCAL VARIABLES
C     ***************
      INTEGER I,J,JPL1,XJP1,YJP1,ICS8,ICS9
      LOGICAL ERROR
C
      ICS8=ICS(8)
      ICS9=ICS(9)
C
      ERROR = .FALSE.
      DO 20 I=1,ICS(3)-1
	 DO 10 J=I+1,ICS(3)
	    IF (PLYCRS(I,J)) THEN
	       ERROR=.TRUE.
	       IF (J.EQ.ICS(3)) THEN
		  YJP1=ICS9
		  XJP1=ICS8
	       ELSE
		  YJP1=J+ICS9
		  XJP1=J+ICS8
	       ENDIF
C
	       IF (J.EQ.ICS(3)) THEN
		   JPL1 = 1
	       ELSE
		   JPL1 = J + 1
	       ENDIF
	       WRITE (LL,1000)
	       WRITE (LL,1001) I
	       WRITE (LL,1002) I,I,I+1,I+1
	       WRITE (LL,1003) RCS(I+ICS8-1),RCS(I+ICS9-1),
     *                         RCS(I+ICS8),RCS(I+ICS9)
	       WRITE (LL,1001) J
	       WRITE (LL,1002) J,J,JPL1,JPL1
	       WRITE (LL,1003) RCS(J+ICS8),RCS(J+ICS9),
     *                         RCS(XJP1),RCS(YJP1)
	    ENDIF
   10    CONTINUE
   20 CONTINUE
      IF ( ERROR ) WRITE (LL,1004)
      PLYOK=.NOT.ERROR
      RETURN
 1000 FORMAT (//' ++++ INTERSECTING SEGMENTS ++++')
 1001 FORMAT (/' SEGMENT',I2)
 1002 FORMAT (5X,'X',I2,9X,'Y',I2,9X,'X',I2,9X,'Y',I2)
 1003 FORMAT (1X,4F12.6)
 1004 FORMAT (//'   PLEASE RETYPE ALL THE VERTICES')
C
C     **
C     **    END OF FUNCTION PLYOK
C     **
C
      END
C+*+* PLYOK  END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* PLYCRS START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      LOGICAL FUNCTION PLYCRS(I,J)
C
C     ******************************************************************
C     *                                                                *
C     *    THIS FUNCTION RETURNS THE VALUE .TRUE. IF THE POLYGON       *
C     *    SIDES I AND J  CROSS INTERNALLY, .FALSE. OTHERWISE          *
C     *                                                                *
C     *   ARGUMENTS                                                    *
C     *   *********                                                    *
C     *   I,J    - INDICATE THE TWO SIDES OF THE POLYGON               *
C     *                                                                *
C     ******************************************************************
C
      IMPLICIT LOGICAL (A-Z)
C
C     ARGUMENTS
C     *********
      INTEGER I,J
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
C     SUBPROGRAM PLYCRS IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     PLYOK
C
C     LOCAL VARIABLES
C     ***************
      INTEGER YI,YJ,XI,XJ,XJP1,YJP1,ICS8,ICS9
      LOGICAL L1,L2,L3
      REAL*8 DETRMT,P,Q,R,L,M,S,T1,T2
C
      ICS8=ICS(8)
      ICS9=ICS(9)
C
      YI=I+ICS9-1
      YJ=J+ICS9-1
      XI=I+ICS8-1
      XJ=J+ICS8-1
      IF (J.EQ.ICS(3)) THEN
	 YJP1=ICS9
	 XJP1=ICS8
      ELSE
	 YJP1=J+ICS9
	 XJP1=J+ICS8
      ENDIF
C
      P=RCS(YJ)-RCS(YI)
      Q=RCS(YI+1)-RCS(YI)
      R=RCS(YJP1)-RCS(YJ)
      L=RCS(XJ)-RCS(XI)
      M=RCS(XI+1)-RCS(XI)
      S=RCS(XJP1)-RCS(XJ)
C
      DETRMT=S*Q-R*M
      IF (ABS(DETRMT).LT.0.5E-8) THEN
C        LINES ARE PARALLEL
	 PLYCRS=.FALSE.
	 RETURN
      ENDIF
C
CCCC  DETRMT IS NOT ZERO
C
C     T1 AND T2 ARE THE PARAMETRIC VARIABLES OF THE 2 LINES
      T1=(P*S-R*L)/DETRMT
      T2=(P*M-L*Q)/DETRMT
C
CCCC  CHECK WHERE THE LINES CROSS AND RETURN RESULT
C
      L1=(ABS(T1).LT.0.5E-4).OR.(ABS(T1-1).LT.0.5E-4)
      L2=(ABS(T2).LT.0.5E-4).OR.(ABS(T2-1).LT.0.5E-4)
      L3=L1 .AND. L2 .OR. T1.LT.0 .OR. T1.GT.1 .OR. T2.LT.0 .OR. T2.GT.1
      PLYCRS=.NOT.L3
C
C
      RETURN
C
C     **
C     **    END OF FUNCTION PLYCRS
C     **
C
      END
C+*+* PLYCRS END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* SCAN   START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE SCAN
C
C     ******************************************************************
C     *                                                                *
C     *   THIS SUBROUTINE IS USED TO SCAN THE DATA TO DETERMINE        *
C     *   THE RANGE OF X,Y AND FUNCTION VALUES OF THE SOURCE DATA      *
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
C     SUBPROGRAM SCAN   CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     ABORT
C
C     SUBPROGRAM SCAN   REFERENCES THE FOLLOWING FUNCTIONS
C     ****************************************************
      INTEGER IUNIT
C
C     SUBPROGRAM SCAN   IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     QCS
C
C     LOCAL VARIABLES
C     ***************
      INTEGER LKI,I,J,M,N,ICOUNT
      REAL*8 X,XMIN,XMAX,Y,YMIN,YMAX,V,VMIN,VMAX,VALUE(50)
C
C
C
      LKI=IUNIT()
      OPEN(LKI,FILE=NAMES(2),STATUS='OLD')
C     OPEN(LKI,FILE=NAMES(2),STATUS='OLD')                              C-PRIME
C     OPEN(LKI,FILE=NAMES(2),STATUS='OLD')                              C-HP9000
C     OPEN(LKI,FILE=NAMES(2),STATUS='OLD',READONLY,SHARED)              C-VAX
      READ(LKI,'(A)') TITLE
C
CCC     Working datafile is REGULAR or SCATTERED X-Y
C
      IF( IDD(1).EQ.1 ) THEN
C         REGULAR DATA-FILE (UP TO 50 PTS ACROSS)
	  READ(LKI,*,ERR=991,IOSTAT=IN(18)) N,M
	  IGG(5)=N
	  IGG(6)=M
	  ICOUNT=N*M
	  READ(LKI,*,ERR=991,IOSTAT=IN(18)) XMIN,XMAX
	  READ(LKI,*,ERR=991,IOSTAT=IN(18)) YMIN,YMAX
C
CCC       read in first value from first row to set min and max
C
	  READ(LKI,*,ERR=991,IOSTAT=IN(18)) V
	  BACKSPACE LKI
	  VMIN=V
	  VMAX=V
C
	  DO 10 J=1,M
	      READ(LKI,*,ERR=991,IOSTAT=IN(18))
     *               (VALUE(I),I=1,N)
	      DO 20 I=1,N
C                  ONLY REQUIRE VMIN,VMAX
	      V=VALUE(I)
	      IF( V.LT.VMIN ) VMIN=V
	      IF( V.GT.VMAX ) VMAX=V
   20         CONTINUE
   10     CONTINUE
C
C
      ELSE
C
C
C     X-Y SCATTERED
	  ICOUNT=0
   30     CONTINUE
	  READ(LKI,*,END=990,ERR=992,IOSTAT=IN(18)) X,Y,V
	  ICOUNT=ICOUNT+1
	  IF( ICOUNT.EQ.1 ) THEN
C             initialise max and min
	      XMIN=X
	      XMAX=X
	      YMIN=Y
	      YMAX=Y
	      VMIN=V
	      VMAX=V
	  ELSE
	      IF( X.LT.XMIN ) XMIN=X
	      IF( X.GT.XMAX ) XMAX=X
	      IF( Y.LT.YMIN ) YMIN=Y
	      IF( Y.GT.YMAX ) YMAX=Y
	      IF( V.LT.VMIN ) VMIN=V
	      IF( V.GT.VMAX ) VMAX=V
	  ENDIF
	  GOTO 30
      ENDIF
C
C
C
  990 CONTINUE
C
C      END-OF-FILE DETECTED
C
      CLOSE(LKI)
      IDD(7)=ICOUNT
C
      IF (IDD(7).GT.2000) THEN
C        NUMBER OF SOURCE DATA POINTS IS ABOVE THAT WHICH CAN BE DEALT
C        WITH BY THE PACKAGE
	 WRITE (LL,2990) IDD(7)
	 WRITE (LR,2990) IDD(7)
 2990    FORMAT (//' +++ FATAL ERROR DETECTED +++'/
     *      5X,'NUMBER OF SOURCE DATA POINTS IS ',I8/
     *      5X,'THIS IS TOO LARGE - MAXIMUM ALLOWED IS 2000')
	 CALL ABORT
      ENDIF
C
      RDD(3)=XMIN
      RDD(4)=XMAX
      RDD(5)=YMIN
      RDD(6)=YMAX
      RDD(7)=VMIN
      RDD(8)=VMAX
C
C     WRITE SUMMARY TO REPORT FILE
C
      WRITE(LR,2001) NAMES(1),TITLE,XMIN,XMAX,YMIN,YMAX,VMIN,VMAX
 2001 FORMAT(//2X,'SOURCE DATAFILE IS   ',A
     *       //2X,'TITLE OF DATAFILE IS :'/2X,A
     *       //2X,'   X     VALUES RANGE FROM ',F10.3,' TO ',F10.3,
     *        /2X,'   Y     VALUES RANGE FROM ',F10.3,' TO ',F10.3,
     *        /2X,'FUNCTION VALUES RANGE FROM ',E12.3,' TO ',E12.3)
      IF( IDD(1).EQ.1 ) THEN
	  WRITE(LR,'(/2X,A)') 'SOURCE DATA IS REGULAR GRIDDED'
      ELSEIF( IDD(1).EQ.2 ) THEN
	  WRITE(LR,'(/2X,A)') 'SOURCE DATA IS SCATTERED'
      ELSE
	  WRITE(LR,'(/2X,A)') 'SOURCE DATA IS TABULAR'
      ENDIF
C
C
      RETURN
C
C
  991 CONTINUE
C
C     ERROR DETECTED
C
      WRITE(LL,2991) IN(18)
      WRITE(LR,2991) IN(18)
 2991 FORMAT(//'+++ FATAL ERROR DETECTED, IOSTAT =',I4,' +++'/2X,
     *    'IN SUBROUTINE SCAN -- WHILE READING ',
     *    'FROM REGULAR DATA-FILE')
      CALL ABORT
C
C
C
  992 CONTINUE
C
C     ERROR DETECTED
C
      WRITE(LL,2992) IN(18)
      WRITE(LR,2992) IN(18)
 2992 FORMAT(//'+++ FATAL ERROR DETECTED, IOSTAT =',I4,' +++'/2X,
     *    'IN SUBROUTINE SCAN -- WHILE READING ',
     *    'FROM SCATTERED X-Y DATAFILE')
      CALL ABORT
C
C     **
C     **    END OF SUBROUTINE SCAN
C     **
C
      END
C+*+* SCAN   END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* CSTLN  START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE CSTLN(EMPTY)
C
C     ******************************************************************
C     *                                                                *
C     *   THIS SUBROUTINE IS USED TO PROCESS A COASTLINE DATAFILE      *
C     *   TO PRODUCE A NEW COASTLINE FILE CONTAINING DATA RELEVANT     *
C     *   ONLY TO THE PARTICULAR JOB - THIS MAY BE CONVENIENT TO       *
C     *   USE IF THE JOB IS RE-RUN                                     *
C     *                                                                *
C     *   ARGUMENTS                                                    *
C     *   *********                                                    *
C     *   EMPTY - RETURNED AS .TRUE. IF THE SPECIFIED COASTLINE DATA   *
C     *           FILE DOES NOT CONTAIN ANY POINTS WITHIN THE PLOTTING *
C     *           REGION                                               *
C     *                                                                *
C     ******************************************************************
C
      IMPLICIT LOGICAL (A-Z)
C
C     ARGUMENTS
C     *********
      LOGICAL EMPTY
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
C     SUBPROGRAM CSTLN  REFERENCES THE FOLLOWING FUNCTIONS
C     ****************************************************
      INTEGER IUNIT
C
C     SUBPROGRAM CSTLN  IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     QPSC
C
C     LOCAL VARIABLES
C     ***************
      LOGICAL OUT
      INTEGER LCST,LCST1
      REAL*8 DX,DY,DX1,DY1,DX2,DY2,DXC,DYC,DDX,DDY,DXMIN,DXMAX,DYMIN,
     *   DYMAX,X1,Y1,X2,Y2,XSCAL,YSCAL,XMIN,XMAX,YMIN,YMAX,MY1,MY2,
     *   MYMIN,MYMAX,PIBY4
C
C     OUT   SET .TRUE.  IF POINTS OUTPUT FROM CURRENT COASTLINE SEGMENT.
C     EMPTY SET .TRUE.  IF NO 'ORDINARY' POINTS ARE EVER OUTPUT.
C
      EMPTY=.TRUE.
C
CCC   OPEN GIVEN COASTLINE FILE
C
      LCST=IUNIT()
      OPEN(LCST,FILE=NAMES(5),STATUS='OLD')
C     OPEN(LCST,FILE=NAMES(5),STATUS='OLD')                             C-PRIME
C     OPEN(LCST,FILE=NAMES(5),STATUS='OLD')                             C-HP9000
C     OPEN(LCST,FILE=NAMES(5),STATUS='OLD',READONLY,SHARED)             C-VAX
C
CCC   OPEN NEW COASTLINE FILE
C
      LCST1=IUNIT()
      OPEN(LCST1,FILE=NAMES(6),STATUS='UNKNOWN')
C     OPEN(LCST1,FILE=NAMES(6),STATUS='UNKNOWN')                        C-PRIME
C     OPEN(LCST1,FILE=NAMES(6),STATUS='UNKNOWN')                        C-HP9000
C     OPEN(LCST1,FILE=NAMES(6),STATUS='NEW',CARRIAGECONTROL='LIST')     C-VAX
C
C     READ AND WRITE LAT-LONG REFERENCE POINTS
C
      READ(LCST,*)Y1,X1
      READ(LCST,*)Y2,X2
      WRITE(LCST1,*)Y1,X1
      WRITE(LCST1,*)Y2,X2
C
C     COMPUTE CORRESPONDING MERCATOR PROJECTIONS
C
      PIBY4=ATAN(1.0)
      MY1=LOG( TAN( PIBY4*(1+Y1/90.0) ) )
      MY2=LOG( TAN( PIBY4*(1+Y2/90.0) ) )
C
C     READ AND WRITE DIGITISED X-Y REFERENCE POINTS
C
      READ(LCST,*)DX1,DY1
      READ(LCST,*)DX2,DY2
      WRITE(LCST1,*)DX1,DY1
      WRITE(LCST1,*)DX2,DY2
C
C     SET SCALING FACTORS
C     (COAST ASSUMED TO BE IN MERCATOR PROJECTION)
C
      XSCAL=(X2-X1)/(DX2-DX1)
      YSCAL=(MY2-MY1)/(DY2-DY1)
C
C     FIND DIGITISED DATA REGION CORRESPONDING TO XMIN,XMAX,YMIN,YMAX
C
      XMIN=RCS(1)
      XMAX=RCS(2)
      YMIN=RCS(3)
      YMAX=RCS(4)
C
      DXMIN=DX1+(XMIN-X1)/XSCAL
      DXMAX=DX1+(XMAX-X1)/XSCAL
C
C     COMPUTE MERCATOR PROJECTIONS CORRESPONDING TO YMIN,YMAX
C
      MYMIN=LOG( TAN( PIBY4*(1+YMIN/90.0) ) )
      MYMAX=LOG( TAN( PIBY4*(1+YMAX/90.0) ) )
C
      DYMIN=DY1+(MYMIN-MY1)/YSCAL
      DYMAX=DY1+(MYMAX-MY1)/YSCAL
C
C     INCREASE DIGITISED DATA REGION BY 20% TO  ALLOW
C     FOR MINOR CHANGES IN PLOTTING AREA
C
      DDX=DXMAX-DXMIN
      DDY=DYMAX-DYMIN
      DXC=(DXMIN+DXMAX)/2
      DYC=(DYMIN+DYMAX)/2
      DXMIN=DXC-DDX*0.6
      DXMAX=DXC+DDX*0.6
      DYMIN=DYC-DDY*0.6
      DYMAX=DYC+DDY*0.6
C
    1 CONTINUE
C
C     START PROCESSING NEW COASTLINE SEGMENT FROM INPUT FILE
C
      OUT=.FALSE.
C
C
    2 CONTINUE
C
C     CONTINUE PROCESSING COASTLINE SEGMENT FROM INPUT FILE
C     READ DIGITISED X,Y
C
      READ(LCST,*,END=99) DX,DY
      IF( DX.EQ.-9999 ) THEN
C         END-OF-FILE MARKER
C         ( BUT WITH F77 LIST-DIRECTED INPUT WILL NEED END=99 ABOVE )
	  GOTO 99
      ELSEIF( DX.EQ.-8888 ) THEN
C         END-OF-COASTLINE-SEGMENT MARKER
C                        BUT
C         F77 LIST-DIRECTED INPUT WILL ADVANCE TO NEXT RECORD IF NO
C         LINE NUMBERS IN COLS73-80 -- NEED TO ALLOW FOR ABSENCE OR
C         PRESENCE OF LINE NOS
C
	  IF( OUT ) WRITE(LCST1,*) -8888,-8888
	  BACKSPACE LCST
	  READ(LCST,*) DX
	  IF( DX.EQ.-8888 ) THEN
C             THE VALUE FOR DY WAS FOUND ON SAME LINE AS DX
C             SHOULD BE IN CORRECT POSN NOW
	      CONTINUE
	  ELSE
C             THE VALUE FOR DY WAS FOUND ON NEXT LINE
C             NEED TO GO BACK AGAIN
	      BACKSPACE LCST
	  ENDIF
	  GOTO 1
      ELSE
C         ORDINARY DIGITISED X-Y POINT
C         CHECK TO SEE IF INSIDE REGION
	  IF( DX.GT.DXMIN .AND.
     *        DX.LT.DXMAX .AND.
     *        DY.GT.DYMIN .AND.
     *        DY.LT.DYMAX ) THEN
C             INSIDE
	      OUT=.TRUE.
	      EMPTY=.FALSE.
	      WRITE(LCST1,*) DX,DY
	  ELSE
C             OUTSIDE
	      IF( OUT ) THEN
C                  TERMINATE OUTPUT COASTLINE SEGMENT
		   DX=-8888
		   DY=-8888
		   WRITE(LCST1,*) DX,DY
		   OUT=.FALSE.
	       ENDIF
	  ENDIF
C
C     CONTINUE PROCESSING OF INPUT COASTLINE SEGMENT
C
	  GOTO 2
      ENDIF
C
C
C
   99 CONTINUE
C
C     END OF INPUT FILE REACHED
C
      WRITE(LCST1,*) -9999,-9999
      CLOSE(LCST)
      CLOSE(LCST1)
C
      RETURN
C
C     **
C     **    END OF SUBROUTINE CSTLN
C     **
C
      END
C+*+* CSTLN  END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
