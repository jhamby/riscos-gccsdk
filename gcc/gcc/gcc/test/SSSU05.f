C+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+
C+                                                                     +
C+                        FILE SSSU05.FOR                              +
C+                                                                     +
C+  CONTAINS UTILITY SUBPROGRAMS REQUIRED BY ALL PARTS OF THE PACKAGE  +
C+                                                                     +
C+                                                                     +
C+   HOST   : LINES    43 TO   298    ABORT  : LINES   527 TO   673    +
C+   IUNIT  : LINES   299 TO   375    APPCN  : LINES  1972 TO  2044    +
C+   LENTH  : LINES   376 TO   448    CHKFN  : LINES  1782 TO  1923    +
C+   DLETE  : LINES   449 TO   526    CHKFN1 : LINES  1924 TO  1971    +
C+   ABORT  : LINES   527 TO   673    DATTIM : LINES  2480 TO  2611    +
C+   TIDY   : LINES   674 TO   766    DLETE  : LINES   449 TO   526    +
C+   TIDYP  : LINES   767 TO   871    ER777  : LINES  2142 TO  2224    +
C+   WRCMN  : LINES   872 TO   975    FLNAM  : LINES  1328 TO  1471    +
C+   ZREAD  : LINES   976 TO  1203    HELP   : LINES  1649 TO  1781    +
C+   UCASE  : LINES  1204 TO  1241    HOST   : LINES    43 TO   298    +
C+   ZWRITE : LINES  1242 TO  1327    INITCM : LINES  2225 TO  2430    +
C+   FLNAM  : LINES  1328 TO  1471    INQIRE : LINES  2045 TO  2141    +
C+   RSERR  : LINES  1472 TO  1648    ISGN   : LINES  2431 TO  2479    +
C+   HELP   : LINES  1649 TO  1781    IUNIT  : LINES   299 TO   375    +
C+   CHKFN  : LINES  1782 TO  1923    LENTH  : LINES   376 TO   448    +
C+   CHKFN1 : LINES  1924 TO  1971    RSERR  : LINES  1472 TO  1648    +
C+   APPCN  : LINES  1972 TO  2044    TIDY   : LINES   674 TO   766    +
C+   INQIRE : LINES  2045 TO  2141    TIDYP  : LINES   767 TO   871    +
C+   ER777  : LINES  2142 TO  2224    UCASE  : LINES  1204 TO  1241    +
C+   INITCM : LINES  2225 TO  2430    UPDATE : LINES  2612 TO  2704    +
C+   ISGN   : LINES  2431 TO  2479    WRCMN  : LINES   872 TO   975    +
C+   DATTIM : LINES  2480 TO  2611    ZREAD  : LINES   976 TO  1203    +
C+   UPDATE : LINES  2612 TO  2704    ZWRITE : LINES  1242 TO  1327    +
C+                                                                     +
C+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
CCCCCCCCCCCCC                                         CCCCCCCCCCCCCCCCCC
CCCCCCCCCCCCC              NOTE                       CCCCCCCCCCCCCCCCCC
CCCCCCCCCCCCC                                         CCCCCCCCCCCCCCCCCC
CCCCCCCCCCCCC    HOST MAY NEED RPS(4), RPS(5)         CCCCCCCCCCCCCCCCCC
CCCCCCCCCCCCC             SETTING                     CCCCCCCCCCCCCCCCCC
CCCCCCCCCCCCC                                         CCCCCCCCCCCCCCCCCC
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C+*+* HOST   START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE HOST(MMARK)
C
C     ******************************************************************
C     *                                                                *
C     *   THIS SUBROUTINE DETERMINES THE HOST COMPUTER AND ALSO        *
C     *   RETURNS THE CURRENT MARK NUMBER FOR THE PACKAGE              *
C     *                                                                *
C     *          +++ HOST DEPENDENT SUBROUTINE +++                     *
C     *                                                                *
C     ******************************************************************
C
      IMPLICIT LOGICAL (A-Z)
C
C     ARGUMENTS
C     *********
      CHARACTER*(*) MMARK
C
C     COMMON BLOCKS
C     *************
C     CHARS   CNTRL  GKSDEV  GKSDSC
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
CCCC  GKSDEV COMMON BLOCK -- device workstation type numbers
C
      INTEGER NT,NP,GKTERM(10),GKPLOT(20),NDEV(20)
      COMMON /GKSDEV/ NT,NP,GKTERM,GKPLOT,NDEV
C
CCCC  GKSDSC COMMON BLOCK  -- device descriptions
C
      CHARACTER*30 GKTDSC(10),GKPDSC(20),GKPDDV(20,8)
      COMMON /GKSDSC/ GKTDSC,GKPDSC,GKPDDV
C
C     SUBPROGRAM HOST   CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     LENTH
C
C     SUBPROGRAM HOST   REFERENCES THE FOLLOWING FUNCTIONS
C     ****************************************************
      INTEGER IUNIT
C
C     SUBPROGRAM HOST   IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     HSU01   HSU02   HSU03
C
C     LOCAL VARIABLES
C     ***************
      CHARACTER*30 FNAME,TNAME,MARK*25
      LOGICAL LGV(6),LGVB,EXISTS
      INTEGER LH,I,J,I1,I2
C
C     DATA STATEMENTS
C     ***************
      DATA MARK/'Mark 7.1         14/08/91'/
C
      MMARK=MARK
C
CCC   initialise REPORT file unit to detect if REPORT file not yet open
C
      LR=-1
C
CCC   Set and store appropriate values for the terminal unit number and
CCC   also 'first' FORTRAN unit no for general use and number of units
CCC   to be made available
C
      IF (SALF .AND. PC386) THEN
C        Terminal unit number is 1 for both input and output
	 LL=1
	 LU=1
	 IN(31)=11
	 IN(32)=10
      ELSEIF (SALF .AND. PRIME) THEN
C        Terminal unit number is 1 for both input and output
	 LL=1
	 LU=1
	 IN(31)=111
	 IN(32)=10
      ELSEIF (LOW .AND. PRIME) THEN
C        Terminal unit number is 1 for both input and output
	 LL=1
	 LU=1
	 IN(31)=111
	 IN(32)=10
      ELSEIF (LOW .AND. HP9000) THEN
C        Terminal unit number is 5 for input and 6 for output
	 LL=6
	 LU=5
	 IN(31)=11
	 IN(32)=10
      ELSEIF (LOW .AND. VAX) THEN
C        Terminal unit number is 5 for input and 6 for output
	 LL=6
	 LU=5
	 IN(31)=11
	 IN(32)=10
      ELSE
C        Unexpected error -- host specified incorrectly?
	 GOTO 996
      ENDIF
C     Pack terminal unit numbers in IN(2) in control common block
      IN(2)=256*LL+LU
C
CCC   Find appropriate version of the file CTHST containing
CCC   the specific host and interface parameters
C
      FNAME='CTHST'
      CALL LENTH(FNAME,I1,I2)
C
      IF (PC386) THEN
C         Is there a file CTHST in the current directory?
	  INQUIRE (FILE=FNAME,EXIST=EXISTS)
	  IF (.NOT.EXISTS) THEN
C            Use the system version
	     TNAME='H:\\CONTOUR\\'//FNAME(I1:I2)
	     FNAME=TNAME
	  ENDIF
      ELSEIF (PRIME) THEN
C         see if in current ufd, if not find the 'system' version
	  INQUIRE(FILE=FNAME,EXIST=EXISTS)
	  IF (.NOT. EXISTS) THEN
	      TNAME='MA10>'//FNAME(I1:I2)
	      FNAME=TNAME
	  ENDIF
      ELSEIF (HP9000 .AND. LOW) THEN
	  INQUIRE (FILE=FNAME,EXIST=EXISTS)
	  IF (.NOT. EXISTS) THEN
	     TNAME='/users/ma10/'//FNAME(I1:I2)
	     FNAME=TNAME
	  ENDIF
      ELSEIF (VAX .AND. LOW) THEN
	  INQUIRE (FILE=FNAME,EXIST=EXISTS)
	  IF (.NOT. EXISTS) THEN
	     TNAME='contouring$'//FNAME(I1:I2)
	     FNAME=TNAME
	  ENDIF
      ELSE
C         unexpected error - host incorrectly specified?
	  GOTO 996
      ENDIF
C
CCC   Write FNAME to CNTRL common
C
      NAMES(10)=FNAME
C
      LH=IUNIT()
      OPEN(LH,FILE=FNAME,STATUS='OLD',ERR=997)
C     OPEN(LH,FILE=FNAME,STATUS='OLD',ERR=997)                          C-PRIME
C     OPEN(LH,FILE=FNAME,STATUS='OLD',ERR=997)                          C-HP9000
C     OPEN(LH,FILE=FNAME,STATUS='OLD',ERR=997,READONLY,SHARED)          C-VAX
C
      READ(LH,'(//6L1)') LGV
C
CCC   Check consistency of host information
C
      LGVB=           (   SALF.EQV.LGV(1) )
      LGVB=LGVB .AND. (    LOW.EQV.LGV(2) )
      LGVB=LGVB .AND. (  PC386.EQV.LGV(3) )
      LGVB=LGVB .AND. (  PRIME.EQV.LGV(4) )
      LGVB=LGVB .AND. ( HP9000.EQV.LGV(5) )
      LGVB=LGVB .AND. (    VAX.EQV.LGV(6) )
      IF( .NOT. LGVB ) GOTO 998
C
CCC   read tidying-up flag
C
      READ(LH,*) IN(20)
C
CCC   read name of HELP file
C
      READ(LH,'(A)') FNAME
      NAMES(13)=FNAME
C
CCC   read name of UPDATE file
C
      READ(LH,'(A)') FNAME
      NAMES(16)=FNAME
C
CCC   Read in number of GKS terminals, then read in corresponding
CCC   workstation ids and descriptions
C
      READ(LH,*) NT
      DO 210 I=1,NT
	 READ(LH,2010) GKTERM(I),GKTDSC(I)
 2010    FORMAT(I12,A30)
  210 CONTINUE
C
CCC   read in no of GKS plotters
C
      READ(LH,*) NP
C
CCC   Read workstation ids, descriptions and device ids
C
      DO 220 I=1,NP
	 READ(LH,2010) GKPLOT(I),GKPDSC(I)
	 READ(LH,*) NDEV(I)
	 DO 221 J=1,NDEV(I)
	    READ(LH,2011) GKPDDV(I,J)
 2011       FORMAT(6X,A30)
  221    CONTINUE
  220 CONTINUE
C
CCC   close CTHST file
C
      CLOSE(LH)
C
      RETURN
C
C
  996 CONTINUE
      WRITE(LL,2996)
 2996 FORMAT(//2X,'UNEXPECTED ERROR -- INCORRECTLY SPECIFIED HOST?',
     *        /2X,'PLEASE REPORT THIS ERROR')
      GOTO 999
C
C
  997 CONTINUE
      WRITE(LL,2997) FNAME
 2997 FORMAT(//2X,'UNEXPECTED ERROR -- CANNOT OPEN FILE :  ',A,
     *        /2X,'PLEASE REPORT THIS ERROR')
      GOTO 999
C
C
  998 CONTINUE
      WRITE(LL,2998) FNAME
 2998 FORMAT(//2X,'UNEXPECTED ERROR IN SUBROUTINE HOST',
     *        /2X,'HOST PARAMETERS INCONSISTENT -- WRONG FILE? : ',A,
     *        /2X,'PLEASE REPORT THIS ERROR')
C
  999 CONTINUE
      STOP '**** STOP -- ERROR IN SUBROUTINE HOST ****'
C
C     **
C     **    END OF SUBROUTINE HOST
C     **
C
      END
C+*+* HOST   END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* IUNIT  START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      INTEGER FUNCTION IUNIT()
C
C     ******************************************************************
C     *                                                                *
C     *   THIS FUNCTION SELECTS A SUITABLE UNIT NUMBER FOR             *
C     *   FILE CONNECTIONS                                             *
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
C     SUBPROGRAM IUNIT  IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     APLOT   BTCH    CNCAT   CPLOT   CSTLN   DLETE   DPLOT   DVPLOT
C     HELP    HOST    HPLOT   HSU01   HSU02   HSU03   HUCD2   HUCL7
C     INQIRE  IPLOT   JPLOT   KPLOT   LOHYP2  LPCTR   QDD     RDATA
C     REFMT   REPLT   RPEAT   RPLOT   SCAN    SETPF   SPLOT   SSU10
C     SSU12   SSU20   SSU27   SSU35   TPLOT   UPDATE  WRCMN   WRMRC
C     XPLOT
C
C     LOCAL VARIABLES
C     ***************
      LOGICAL OPENED
      INTEGER I,L1,L2
C
      L1=IN(31)
      L2=L1+IN(32)-1
      DO 10 I=L1,L2
	  INQUIRE(I,OPENED=OPENED)
	  IF( .NOT.OPENED ) THEN
	      IUNIT=I
	      RETURN
	  ENDIF
   10 CONTINUE
C
C     ERROR DETECTED
C
      WRITE(LL,2991)
 2991 FORMAT(//'+++ FATAL ERROR IN IUNIT +++'/2X,
     1    ' UNIT NUMBERS EXHAUSTED -- CHECK CLOSING OF FILES')
C
CCC   CANNOT ABORT SINCE IT WILL CAUSE RECURSION
C
      STOP '**** STOP -- ERROR IN IUNIT ****'
C
C     **
C     **    END OF FUNCTION IUNIT
C     **
C
      END
C+*+* IUNIT  END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* LENTH  START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE LENTH(STRING,FIRST,LAST)
C
C     ******************************************************************
C     *                                                                *
C     *   SUBROUTINE LENTH(STRING,FIRST,LAST)                          *
C     *                                                                *
C     *   THIS SUBROUTINE DETERMINES THE POSITION OF THE FIRST         *
C     *   AND LAST NON-BLANK CHARACTERS IN A CHARACTER STRING          *
C     *                                                                *
C     *   ARGUMENTS                                                    *
C     *   *********                                                    *
C     *   STRING  -  CHARACTER STRING                                  *
C     *   FIRST   -  POSITION OF FIRST NON-BLANK CHARACTER             *
C     *   LAST    -  POSITION OF LAST  NON-BLANK CHARACTER             *
C     *                                                                *
C     ******************************************************************
C
      IMPLICIT LOGICAL (A-Z)
C
C     ARGUMENTS
C     *********
      CHARACTER STRING*(*)
      INTEGER FIRST,LAST
C
C     SUBPROGRAM LENTH  IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     AXES    CHKFN   DFLIST  HOST
C
C     LOCAL VARIABLES
C     ***************
      INTEGER LS,I
C
CCCC  DETERMINE THE LENGTH OF THE STRING
C
      LS=LEN(STRING)
C
CCCC  FIND THE FIRST NON-BLANK CHARACTER
C
      DO 10 I=1,LS
	 IF (STRING(I:I).NE.' ') THEN
	    FIRST=I
	    GOTO 20
	 ENDIF
   10 CONTINUE
C
CCCC  IF THE STRING IS ALL BLANKS, RETURN SIGNALLING A NON-BLANK STRING
CCCC  OF LENGTH ONE
C
      FIRST=1
      LAST=1
      RETURN
   20 CONTINUE
C
CCCC  DETERMINE THE LAST NON-BLANK CHARACTER
C
      DO 30 I=LS,1,-1
	 IF (STRING(I:I).NE.' ') THEN
	    LAST=I
	    RETURN
	 ENDIF
   30 CONTINUE
      RETURN
C
C     **
C     **  END OF SUBROUTINE LENTH
C     **
      END
C
C+*+* LENTH  END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* DLETE  START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE DLETE(FILE,LL)
C
C     ******************************************************************
C     *                                                                *
C     *   THIS SUBROUTINE DELETES A FILE                               *
C     *                                                                *
C     *   ARGUMENTS                                                    *
C     *   *********                                                    *
C     *   FILE  - CHARACTER VARIABLE CONTAINING A FILENAME             *
C     *   LL    - TERMINAL OUTPUT UNIT NO                              *
C     *                                                                *
C     ******************************************************************
C
      IMPLICIT LOGICAL (A-Z)
C
C     ARGUMENTS
C     *********
      CHARACTER*(*) FILE
      INTEGER LL
C
C     SUBPROGRAM DLETE  REFERENCES THE FOLLOWING FUNCTIONS
C     ****************************************************
      INTEGER IUNIT
C
C     SUBPROGRAM DLETE  IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     QPSC    TIDY    TIDYP
C
C     LOCAL VARIABLES
C     ***************
      INTEGER IOSTAT,UNIT
C
      UNIT=IUNIT()
      OPEN(UNIT,FILE=FILE,STATUS='OLD',ERR=991,IOSTAT=IOSTAT)
      CLOSE(UNIT,STATUS='DELETE',ERR=992,IOSTAT=IOSTAT)
      RETURN
C
C
C
  991 CONTINUE
C
C     ERROR DETECTED
C
      WRITE(LL,2991) IOSTAT,FILE
 2991 FORMAT(//'+++ FATAL ERROR DETECTED, IOSTAT =',I4,' +++'/2X,
     1    'IN SUBROUTINE DLETE -- WHILE OPENING ',
     2    'FILE < ',A,' >')
      GOTO 999
C
C
C
  992 CONTINUE
C
C     ERROR DETECTED
C
      WRITE(LL,2992) IOSTAT,FILE
 2992 FORMAT(//'+++ FATAL ERROR DETECTED, IOSTAT =',I4,' +++'/2X,
     1    'IN SUBROUTINE DLETE -- WHILE CLOSING ',
     2    'FILE < ',A,' >')
C
C
  999 CONTINUE
C
CCC   CANNOT ABORT SINCE IT WILL CAUSE RECURSION
C
      STOP '**** STOP -- ERROR IN DLETE ****'
C
C
C     **
C     **    END OF SUBROUTINE DLETE
C     **
C
      END
C+*+* DLETE  END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* ABORT  START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE ABORT
C
C     ******************************************************************
C     *                                                                *
C     *    THIS SUBROUTINE CLOSES ALL FILES , WRITES CNTRL COMMON      *
C     *    BLOCKS TO THE FILE 'CMNFIL/PMNFIL' AND STOPS                *
C     *                                                                *
C     ******************************************************************
C
      IMPLICIT LOGICAL (A-Z)
C
C     COMMON BLOCKS
C     *************
C     CHARS   CNTRL   RPCMN
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
CCCC  RPCMN COMMON BLOCK
C
      INTEGER RPUNIT,RPBTCH
      COMMON /RPCMN/ RPUNIT,RPBTCH
C
C     SUBPROGRAM ABORT  CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     JUMP@   TIDY    TIDYP   WRCMN
C
C     SUBPROGRAM ABORT  IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     AKIMA1  AKIMA2  APPCN   BTCH    CHKEDG  CREDR   ER777   ERRMES
C     FOLEY1  FOLEY2  GKINT1  HIGHT   HPLOT   HSU01   HSU02   HSU03
C     HUCL7   IPLOT   IPSCH1  IPSCH2  IREDR   JDMTH1  JDMTH2  LOHYP1
C     LOHYP2  LOSUP1  LOSUP2  MDQSM1  MDQSM2  NFQSM1  NFQSM2  QDD
C     QPS     QPSC    RDATA   REFMT   REPLT   RFTPS1  RFTPS2  RHMTH1
C     RHMTH2  RPEAT   RPLOT   RSERR   SAVED   SCAN    SDATA   SPLOT
C     SSU20   SSU27   TRANF   UMAP    VMAP    WRMRC   XPLOT
C
C     LOCAL VARIABLES
C     ***************
      INTEGER I,ILINE,ISTAT,L1,L2
C
C     EQUIVALENCED VARIABLES
C     **********************
      LOGICAL BATCH
      EQUIVALENCE (BATCH,LGSV(10))
C
C     COMPLEX*16 LABEL
C     REAL*8 LABEL                                                      C-PRIME
      INTEGER LABEL
C     INTEGER LABEL                                                     C-VAX
      EQUIVALENCE (LABEL,IN(27))
C
CCCC  IN CASE TERMINAL OUTPUT HAS BEEN DIVERTED TO A SCRATCH FILE,
CCCC  RESET LL -- NOTE UNPACKING 1.7.87 -- RWP
C
      LL=IN(2)/256
      WRITE(LL,2000)
 2000 FORMAT(//'  ***  ROUTINE ABORT  ***')
      WRITE(LL,2002)
C
CCC   Check to see if REPORT file has been opened -- otherwise
CCC   an error may occur from cartridge no question
C
      IF( IN(10).NE.1 ) GOTO 999
C
CCC   check for line assigned to HP plotter
C
      ILINE=IN(21)
      IF( LOW .AND. PRIME .AND. ILINE.GE.0 ) THEN
C         Lowestoft -- UNASSIGN LINE
	  CONTINUE
C         CALL ASNLN$(0,ILINE,'TRAN  ',:2605,:20000,ISTAT)              C-PRIME
C         CONTINUE                                                      C-HP9000
C         CONTINUE                                                      C-VAX
      ENDIF
C
      WRITE(LR,2002)
      WRITE(LR,2002)
 2002 FORMAT(//2X,'****** RUN ABORTED ----- CLOSING ALL FILES ******')
C
C     WRITE THE CONTROL COMMON SO IT CAN BE CHECKED IF REQUIRED
C
      CALL WRCMN
C
CCC   make sure no files left open
C
      L1=IN(31)
      L2=L1+IN(32)-1
      DO 20 I=L1,L2
C         keep REPORT file open if in BATCH mode
	  IF( .NOT.(BATCH .AND. ( I.EQ.LR .OR. I.EQ.RPBTCH )) ) CLOSE(I)
   20 CONTINUE
C
C     TIDY UP WORK-FILES CREATED BY PACKAGE
C
      IF(IDD(1) .EQ. 4)THEN
	 CALL TIDYP
      ELSE
	 CALL TIDY
      ENDIF
C
CCC   if in BATCH mode, return to main program so that next c.f.
CCC   can be run  --  use non local GOTO
C
      IF( BATCH .AND. IN(25).GT.0 ) THEN
C         BATCH and reached the stage of reading in a c.f.
          GOTO LABEL
C	  CALL JUMP@(LABEL)
C         CALL PL1$NL(LABEL)                                            C-PRIME
C         GOTO LABEL                                                    C-HP9000
C         GOTO LABEL                                                    C-VAX
      ENDIF
C
CCC   in case REPORT file left open
C
      CLOSE(LR)
C
  999 CONTINUE
C
C     TERMINATE EXECUTION
C
      STOP '**** STOP -- IN ABORT ****'
C
C     **
C     **    END OF SUBROUTINE ABORT
C     **
C
      END
C+*+* ABORT  END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* TIDY   START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE TIDY
C
C     ******************************************************************
C     *                                                                *
C     *   THIS SUBROUTINE IS INTENDED TO DELETE ALL THE 'TEMPORARY'    *
C     *   FILES CREATED BY THE PACKAGE BUT AT SALFORD, OR AT LOWESTOFT *
C     *   ON CARTRIDGE 910, THE FILES ARE LEFT FOR DEBUGGING PURPOSES  *
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
C     SUBPROGRAM TIDY   CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     DLETE
C
C     SUBPROGRAM TIDY   IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     ABORT   HSU01   HSU02
C
C     LOCAL VARIABLES
C     ***************
      CHARACTER*6 NAME*30,FNAME(20)
      LOGICAL EXISTS
      INTEGER I
C
CCC   tidy up the temporary files (which may be useful
CCC   for debugging) unless the flag IN(20) is set to 1
C
      IF( IN(20).EQ.1 ) RETURN
C
      FNAME( 1)='RFIL'
      FNAME( 2)='SFIL'
      FNAME( 3)='PFCTR'
      FNAME( 4)='MFCTR'
      FNAME( 5)='PFAXS'
      FNAME( 6)='MFAXS'
      FNAME( 7)='PFCST'
      FNAME( 8)='MFCST'
      FNAME( 9)='PFDTA'
      FNAME(10)='MFDTA'
      FNAME(11)='PFADD'
      FNAME(12)='MFADD'
      FNAME(13)='PFKEY'
      FNAME(14)='DAFIL'
      FNAME(15)='CMNFIL'
      FNAME(16)='MFTMP'
      FNAME(17)='PFONE'
C
C
C
      DO 10 I=1,17
	  NAME=FNAME(I)
	  INQUIRE(FILE=NAME,EXIST=EXISTS)
	  IF( EXISTS ) CALL DLETE(NAME,LL)
   10 CONTINUE
C
      RETURN
C
C     **
C     **    END OF SUBROUTINE TIDY
C     **
C
      END
C+*+* TIDY   END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* TIDYP  START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE TIDYP
C
C     ******************************************************************
C     *                                                                *
C     *   THIS SUBROUTINE IS INTENDED TO DELETE ALL THE 'TEMPORARY'    *
C     *   FILES CREATED BY THE PACKAGE BUT AT SALFORD, OR AT LOWESTOFT *
C     *   ON CARTRIDGE 910, THE FILES ARE LEFT FOR DEBUGGING PURPOSES  *
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
C     SUBPROGRAM TIDYP  CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     DLETE
C
C     SUBPROGRAM TIDYP  IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     ABORT
C
C     LOCAL VARIABLES
C     ***************
      CHARACTER*6 NAME,FNAME(20)
      CHARACTER CN*3,NO*10
      LOGICAL EXISTS
      INTEGER I,D1,D2,D3
C
CCC   tidy up the temporary files (which may be useful
CCC   for debugging) unless the flag IN(20) is set to 1
C
      IF( IN(20).EQ.1 ) RETURN
C
      FNAME( 1)='PTEMP1'
      FNAME( 2)='PTEMP2'
      FNAME( 3)='PFIL'
      FNAME( 4)='PFX1A'
      FNAME( 5)='PFX1B'
      FNAME( 6)='PFP1'
      FNAME( 7)='PFP2A'
      FNAME( 8)='PFP2B'
      FNAME( 9)='PFP3'
      FNAME(10)='PFP4A'
      FNAME(11)='PFP4B'
      FNAME(12)='PFP5'
      IF( .FALSE. ) THEN
C
CCC       find default cartridge no, as a string, using
CCC       code similar to that used in CHKFN
C
	  NO='0123456789'
C
	  D1=IN(19)/100
	  D3=IN(19)-100*D1
	  D2=D3/10
	  D3=D3-10*D2
	  D1=D1+1
	  D2=D2+1
	  D3=D3+1
	  CN=NO(D1:D1)//NO(D2:D2)//NO(D3:D3)
      ENDIF
C
C
C
      DO 10 I=1,12
	  NAME=FNAME(I)
	  INQUIRE(FILE=NAME,EXIST=EXISTS)
	  IF( EXISTS ) CALL DLETE(NAME,LL)
   10 CONTINUE
C
      RETURN
C
C     **
C     **    END OF SUBROUTINE TIDYP
C     **
C
      END
C+*+* TIDYP  END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* WRCMN  START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE WRCMN
C
C     ******************************************************************
C     *                                                                *
C     *   THIS SUBROUTINE WRITES THE CNTRL COMMON BLOCKS TO THE        *
C     *   FILE 'CMNFIL/PMNFIL'                                         *
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
C     SUBPROGRAM WRCMN  CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     CHKFN1
C
C     SUBPROGRAM WRCMN  REFERENCES THE FOLLOWING FUNCTIONS
C     ****************************************************
      INTEGER IUNIT
C
C     SUBPROGRAM WRCMN  IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     ABORT   HSU01
C
C     LOCAL VARIABLES
C     ***************
      CHARACTER*30 NAME
      LOGICAL FLGV
      INTEGER LK
C
C     SET UP FILENAME , APPEND CARTRIDGE NO
C
      IF(IDD(1) .EQ. 4)THEN
	 NAME='PMNFIL'
      ELSE
	 NAME='CMNFIL'
      ENDIF
      CALL CHKFN1(NAME,FLGV)
C
CCCC  CHECK FOR UNEXPECTED ERROR IN CHKFN1 - STOP IF SO
C
      IF (.NOT.FLGV) THEN
	 WRITE(LL,2999) NAME
 2999    FORMAT(//'++++ UNEXPECTED ERROR IN WRCMN ++++'/
     *      'FILENAME IS ',A)
	 STOP '**** STOP -- ERROR IN WRCMN ****'
      ENDIF
C
C        WRITE CONTROL COMMONS
C
C
      LK=IUNIT()
      OPEN(LK,FILE=NAME,STATUS='UNKNOWN')
C     OPEN(LK,FILE=NAME,STATUS='UNKNOWN')                               C-PRIME
C     OPEN(LK,FILE=NAME,STATUS='UNKNOWN')                               C-HP9000
C     OPEN(LK,FILE=NAME,STATUS='NEW',CARRIAGECONTROL='LIST')            C-VAX
      WRITE(LK,8001) TITLE
      WRITE(LK,8001) XTITLE,YTITLE,ZTITLE
      WRITE(LK,8001) XUNITS,YUNITS,ZUNITS
      WRITE(LK,8001) NAMES
C
      WRITE(LK,8004) LGSV
      WRITE(LK,8002) IN,IDD,ICS,IGG,IKN,IPS
      WRITE(LK,8003) RDD,RCS,RGG,RKN,RPS
      CLOSE(LK)
C
 8001 FORMAT(A)
 8002 FORMAT(10I6)
 8003 FORMAT(5E16.8)
 8004 FORMAT(10(L1,2X))
C
      RETURN
C
C     **
C     **    END OF SUBROUTINE WRCMN
C     **
C
      END
C+*+* WRCMN  END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* ZREAD  START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE ZREAD(UPCASE,LINE,FLAG,*)
C
C     ******************************************************************
C     *                                                                *
C     *   THIS SUBROUTINE READS A LINE OF TEXT AND                     *
C     *     (a) CHECKS IF AN ERROR OR EOF OCCURS                       *
C     *     (b) STRIPS OFF LEADING BLANKS                              *
C     *     (c) CHECKS WHETHER QUIT OR HELP HAS BEEN REQUESTED         *
C     *                                                                *
C     *   IF OK       FLAG = 0     ORDINARY RETURN                     *
C     *   IF ERROR    FLAG = -1 )                                      *
C     *   IF EOF      FLAG = -2 )  ALTERNATE RETURN                    *
C     *   IF HELP     FLAG =  1 )                                      *
C     *   IF QUIT     FLAG =  2 )                                      *
C     *                                                                *
C     *   ARGUMENTS                                                    *
C     *   *********                                                    *
C     *   LINE  - 'PROCESSED' RECORD                                   *
C     *   FLAG  - RECORD STATUS FLAG, SEE ABOVE                        *
C     *                                                                *
C     ******************************************************************
C
      IMPLICIT LOGICAL (A-Z)
C
C     ARGUMENTS
C     *********
      CHARACTER LINE*(*)
      INTEGER FLAG
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
C     SUBPROGRAM ZREAD  CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     UCASE
C
C     SUBPROGRAM ZREAD  IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     CREDR   DFAULT  DVPLOT  DVTERM  FLNAM   FOLEY1  HSU01   HSU02
C     HSU03   IPSCH1  IREDR   LOHYP1  LOSUP1  MDQSM1  NFQSM1  QCS
C     QDD     QGG     QKN     QPOLY   QPS     QPSC    QPSI    REFMT
C     RFTPS1  RHMTH1  RPEAT   SSU10   SSU13   SSU40   TRANS
C
C     LOCAL VARIABLES
C     ***************
      CHARACTER STR*4,ANS*2
      LOGICAL CHANGE,EDITCF
      INTEGER I,LENGTH,NBL,NCH,LLU
C
C     EQUIVALENCED VARIABLES
C     **********************
      LOGICAL OPEN,FIRST
      EQUIVALENCE (OPEN,LGSV(1)),(FIRST,LGSV(2)),(EDITCF,LGSV(11))
C
C
    1 CONTINUE
C     loop back point when skipping a record
      CHANGE=.FALSE.
C
      READ(LU,'(A)',ERR=40,END=50) LINE
      IF( IN(15) .EQ. 1 ) THEN
C        in interactive mode -- ignore any line starting with #
	 IF( LINE(1:1) .EQ. '#' ) GOTO 1
      ENDIF
C
C
    2 CONTINUE
C     loop back point when input from c.f. is 'changed' or
C     there is an 'insert'
C
      IF (LINE(1:1).EQ.' ') THEN
C
CCCC     FIRST CHARACTER IS BLANK, SO FIND NUMBER OF LEADING BLANKS
C
	 LENGTH=LEN(LINE)
	 DO 10 I=2,LENGTH
	    IF (LINE(I:I).NE.' ') THEN
	       NBL=I-1
	       GOTO 20
	    ENDIF
   10    CONTINUE
C        LINE COMPLETELY BLANK
	 FLAG=0
	 GOTO 60
   20    CONTINUE
C        STRIP OFF LEADING BLANKS
	 DO 30 I=1,LENGTH-NBL
	    NCH=NBL+I
	    LINE(I:I)=LINE(NCH:NCH)
   30    CONTINUE
	 LINE(I:)=' '
      ENDIF
C
CCCC  CHANGE INPUT TO UPPER CASE IF SPECIFIED
C
      IF (UPCASE) CALL UCASE(LINE)
C
CCCC  CHECK FOR QUIT OR HELP
C
      STR=LINE(1:4)
      IF(STR .EQ. 'QUIT')THEN
	 FLAG=2
      ELSEIF(STR .EQ. 'HELP')THEN
	 FLAG=1
      ELSE
	 FLAG=0
      ENDIF
      GOTO 60
C
C
   40 CONTINUE
C     ERROR IN READ
      FLAG=-1
      GOTO 61
C
C
   50 CONTINUE
C     EOF IN READ
      FLAG=-2
      GOTO 61
C
C
   60 CONTINUE
C
C
      IF( EDITCF .AND. ( .NOT. CHANGE ) .AND. IN(15).NE.0 ) THEN
C         Editing mode, not already changed on this entry
C         and non-interactive
C
    3     CONTINUE
C         loop back point in case of invalid response
	  WRITE(LL,2100) LINE
 2100     FORMAT(/2X,A/2X,'Type Accept, Change, Skip, Insert',
     *                ' or SWitch to input mode',
     *                /2X,'     -       -       -     -     ',
     *                '    --                  ')
C         Note unpacking 1.7.87 -- RWP
	  LLU=IN(2)-(IN(2)/256)*256
	  READ(LLU,'(A)') ANS
	  CALL UCASE(ANS)
	  IF( ANS.EQ.' ' ) ANS='A'
	  IF( ANS(1:1).EQ.'A' ) THEN
C             Accept
	      CONTINUE
	  ELSEIF( ANS(1:1).EQ.'C' ) THEN
C             Change input from c.f.
	      CHANGE=.TRUE.
	      WRITE(LL,2000)
 2000         FORMAT(//2X,'*** Input from control file ignored ***',
     *                /2X,'Please type in your alternative response')
	      READ(LLU,'(A)') LINE
	      GOTO 2
	  ELSEIF( ANS.EQ.'SW' ) THEN
C             revert to input by user
C             -- note that need to test for SW before S
	      WRITE(LL,2001)
 2001         FORMAT(//2X,'*** Input from control file ended ***',
     *                /2X,'You will now have to answer ',
     *                    'all remaining questions')
	      FLAG=-1
	      QNO=0
	      EDITCF=.FALSE.
	      IN(15)=0
	      CLOSE(LU)
C             Note unpacking 1.7.87 -- RWP
	      LU=IN(2)-(IN(2)/256)*256
	      RETURN 1
	  ELSEIF( ANS(1:1).EQ.'S' ) THEN
C             Skip this input record from c.f.
	      GOTO 1
	  ELSEIF( ANS(1:1).EQ.'I' ) THEN
C             Insert extra response, retain current input record
C             for next question by backspacing
	      CHANGE=.TRUE.
	      BACKSPACE LU
	      WRITE(LL,2002)
 2002         FORMAT(//2X,'*** Input from control file retained ',
     *                'for next question ***',
     *            /2X,'Please type in your response for this question')
	      READ(LLU,'(A)') LINE
	      GOTO 2
	  ELSE
C             invalid response
	      WRITE(LL,2003)
 2003         FORMAT(//2X,'*** INVALID RESPONSE -- TRY AGAIN ***')
	      GOTO 3
	  ENDIF
      ENDIF
C
   61 CONTINUE
C
      IF( FLAG.EQ.0 ) THEN
C         HELP,QUIT ERROR NOT DETECTED - 'ORDINARY' RETURN
	  RETURN
      ELSE
C         HELP,QUIT OR ERROR DETECTED - 'ALTERNATE' RETURN
	  RETURN 1
      ENDIF
C
C      **
C      **  END OF SUBROUTINE ZREAD
C      **
C
      END
C+*+* ZREAD  END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* UCASE  START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE UCASE(STRING)
C
C     ******************************************************************
C     *                                                                *
C     *   THIS SUBROUTINE CHANGES LOWER CASE LETTERS TO UPPER CASE     *
C     *                                                                *
C     *   ARGUMENTS                                                    *
C     *   *********                                                    *
C     *   STRING  -  CHARACTER STRING                                  *
C     *                                                                *
C     ******************************************************************
C
      IMPLICIT LOGICAL (A-Z)
C
C     ARGUMENTS
C     *********
      CHARACTER STRING*(*)
C
C     SUBPROGRAM UCASE  IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     RSERR   ZREAD
C
      IF (STRING(1:1).EQ.'y') STRING(1:1)='Y'
      IF (STRING(1:1).EQ.'n') STRING(1:1)='N'
      IF (STRING(1:1).EQ.'q') STRING(1:1)='Q'
C
      RETURN
C
C     **
C     **  END OF SUBROUTINE UCASE
C     **
      END
C
C+*+* UCASE  END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* ZWRITE START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE ZWRITE(LL,LE,LINE)
C
C     ******************************************************************
C     *                                                                *
C     *   THIS SUBROUTINE WRITES A LINE OF TEXT AFTER                  *
C     *   STRIPPING OFF LEADING BLANKS FROM THE FIRST FIELD OF         *
C     *   'NCOLS' CHARACTERS                                           *
C     *                                                                *
C     *   ARGUMENTS                                                    *
C     *   *********                                                    *
C     *   LL    - OUTPUT UNIT NO FOR TERMINAL, USED IN ERROR SITUATION *
C     *   LE    - OUTPUT UNIT NO FOR 'STRIPPED' LINES                  *
C     *   LINE  - RECORD TO BE PROCESSED                               *
C     *                                                                *
C     ******************************************************************
C
      IMPLICIT LOGICAL (A-Z)
C
C     ARGUMENTS
C     *********
      CHARACTER LINE*(*)
      INTEGER LL,LE
C
C     SUBPROGRAM ZWRITE IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     EFOLEY  EIPSCH  ELOHYP  ELOSUP  EMDQSM  ENFQSM  EQCS    EQDD
C     EQGG    EQKN    EQPOL   EQPS    EQPSC   EQPSI   EREFM   ERFTPS
C     ERHMTH  ETRAN
C
C     PARAMETER STATEMENT
C     *******************
      INTEGER NCOLS
      PARAMETER( NCOLS=30 )
C
C     LOCAL VARIABLES
C     ***************
      CHARACTER*30 CHCOLS
      INTEGER I,NBL,NCH
C
C
C
      CHCOLS=LINE(1:NCOLS)
      IF (CHCOLS(1:1).EQ.' ') THEN
C
CCCC     FIRST CHARACTER IS BLANK, SO FIND NUMBER OF LEADING BLANKS
C
	 DO 10 I=2,NCOLS
	    IF (CHCOLS(I:I).NE.' ') THEN
	       NBL=I-1
	       GOTO 20
	    ENDIF
   10    CONTINUE
C        CHCOLS COMPLETELY BLANK
C        allow blank record eg AXIS TITLE
	 GOTO 99
C
C
   20    CONTINUE
C        STRIP OFF LEADING BLANKS
	 DO 30 I=1,NCOLS-NBL
	    NCH=NBL+I
	    CHCOLS(I:I)=CHCOLS(NCH:NCH)
   30    CONTINUE
	 CHCOLS(I:)=' '
      ENDIF
C
C
C
   99 CONTINUE
C
C
      LINE(1:NCOLS)=CHCOLS
      WRITE(LE,'(A)') LINE
C
      RETURN
C
C      **
C      **  END OF SUBROUTINE ZWRITE
C      **
C
      END
C+*+* ZWRITE END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* FLNAM  START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE FLNAM(NAME,IOFLAG)
C
C     ******************************************************************
C     *                                                                *
C     *   THIS SUBROUTINE ASKS FOR A FILENAME AND CHECKS WHETHER THE   *
C     *   FILE EXISTS.                                                 *
C     *                                                                *
C     *   IF IOFLAG=0 THEN FILE IS FOR INPUT -- IF FILE DOES NOT EXIST *
C     *   --> ERROR SITUATION                                          *
C     *                                                                *
C     *                                                                *
C     *   IF IOFLAG=1 THEN FILE IS FOR OUTPUT -- IF FILE EXISTS A      *
C     *   CHECK IS MADE TO SEE WHETHER THE FILE MAY BE OVERWRITTEN     *
C     *                                                                *
C     *   ARGUMENTS                                                    *
C     *   *********                                                    *
C     *   NAME   -- CHARACTER VARIABLE CONTAINING FILENAME             *
C     *   IOFLAG -- INTEGER                                            *
C     *                                                                *
C     ******************************************************************
C
      IMPLICIT LOGICAL (A-Z)
C
C     ARGUMENT
C     ********
      CHARACTER NAME*(*)
      INTEGER IOFLAG
C
C     COMMON BLOCKS
C     *************
C     CHARS   CNTRL
C
C-----INCLUDE STATEMENT FOR CNTRL COMMON BLOCKS ------------------------
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
C      INCLUDE 'H:\CONTOUR\MASTER\COMMON'
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
C     SUBPROGRAM FLNAM  CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     CHKFN   RSERR   ZREAD
C
C     SUBPROGRAM FLNAM  IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     LOHYP1  QCS     QGG     QPSC    REFMT   SSU12
C
C     LOCAL VARIABLES
C     ***************
      CHARACTER ANS,LINE*80
      LOGICAL EXISTS
      INTEGER FLAG
C
CCCC  ASK FOR NAME OF FILE
C
      QNO=0
  121 CONTINUE
      IF (QNO.EQ.121) CALL RSERR(FLAG)
      QNO=121
      WRITE(LL,2121)
 2121 FORMAT(//2X,'*** Q121 ***'/2X,
     *       'TYPE IN A NAME FOR THE FILE')
      CALL ZREAD(.TRUE.,LINE,FLAG,*121)
      READ(LINE,'(A)',ERR=121)NAME
      CALL CHKFN(NAME,*121)
C
CCC   SEE IF FILE EXISTS
C
C     INQUIRE(FILE=NAME,EXIST=EXISTS,ERR=999)
      RETURN
CC
C      IF( IOFLAG.EQ.0 ) THEN
CC         FILE IS FOR INPUT
C	  IF( EXISTS ) THEN
C	     RETURN
C	  ELSE
C	     WRITE(LL,9001) NAME
C	     GOTO 121
C	  ENDIF
C      ELSE
CC         FILE IS FOR OUTPUT
C	  IF( .NOT. EXISTS .OR. IN(15).EQ.1 ) THEN
CC            OVERWRITE ANYWAY IF NON-INTERACTIVE
C	     RETURN
C	  ELSE
CC
CCCC          INTERACTIVE MODE AND FILE EXISTS, CAN IT BE OVERWRITTEN?
CC
C	     WRITE(LL,9002) NAME
CC
C  122        CONTINUE
C	     IF (QNO.EQ.122) CALL RSERR(FLAG)
C	     QNO=122
C	     WRITE(LL,2122)
C 2122        FORMAT(//2X,'*** Q122 ***',/2X,
C     *             'DO YOU WANT TO OVERWRITE THE FILE?')
C	     CALL ZREAD(.TRUE.,LINE,FLAG,*122)
C	     READ(LINE,'(A)')ANS
C	     IF (ANS .EQ. ' ')ANS='Y'
C	     IF (ANS .EQ. 'Y') THEN
CC               FILE MAY BE OVERWRITTEN
C		RETURN
C	     ELSEIF (ANS .EQ. 'N') THEN
CC               FILE MAY NOT BE OVERWRITTEN SO ASK FOR NEW FILE-NAME
C		GOTO 121
C	     ELSE
CC               ERROR IN RESPONSE
C		GOTO 122
C	     ENDIF
C	  ENDIF
C      ENDIF
CC
C  999 CONTINUE
CC     ERROR IN INQUIRE STATEMENT
C      WRITE(LL,9003)
C      QNO=0
C      GOTO 121
CC
C 9001 FORMAT(//2X,'+++ ERROR -- FILE ',A,' DOES NOT EXIST +++',
C     *   /2X,'- TRY AGAIN')
C 9002 FORMAT(//'+++ FILE ',A,' ALREADY EXISTS +++')
C 9003 FORMAT(//2X,'+++ ERROR IN CHECKING WHETHER FILE ALREADY EXISTS ',
C     *   '+++'/2X,'PROBABLY AN ILLEGAL FILENAME - TRY AGAIN')
C
C     **
C     **   END OF SUBROUTINE FLNAM
C     **
C
      END
C+*+* FLNAM  END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* RSERR  START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE RSERR(FLAG)
C
C     ******************************************************************
C     *                                                                *
C     *   THIS SUBROUTINE HANDLES 'HELP', 'QUIT', INCORRECT RESPONSES  *
C     *   AND ERROR CONDITIONS ARISING FROM INTERACTIVE QUESTIONING    *
C     *                                                                *
C     *   ARGUMENTS                                                    *
C     *   *********                                                    *
C     *   FLAG  -   0  ORDINARY ENTRY                                  *
C     *             1  ENTRY ASKING FOR HELP                           *
C     *             2  ENTRY ASKING FOR QUIT                           *
C     *            -1  ENTRY WITH READ ERROR                           *
C     *            -2  ENTRY WITH READ EOF                             *
C     *                                                                *
C     ******************************************************************
C
C     ARGUMENT
C     ********
      INTEGER FLAG
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
C     SUBPROGRAM RSERR  CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     ABORT   HELP    UCASE
C
C     SUBPROGRAM RSERR  IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     CREDR   DFAULT  DVPLOT  DVTERM  FLNAM   FOLEY1  HSU01   HSU02
C     HSU03   IPSCH1  IREDR   LOHYP1  LOSUP1  MDQSM1  NFQSM1  QCS
C     QDD     QGG     QKN     QPOLY   QPS     QPSC    QPSI    REFMT
C     RFTPS1  RHMTH1  SSU10   SSU13   SSU40   TRANS
C
C     LOCAL VARIABLES
C     ***************
      CHARACTER*4 ANS
C
C     EQUIVALENCED VARIABLES
C     **********************
      LOGICAL BATCH
      EQUIVALENCE (BATCH,LGSV(10))
C
C
      IF( FLAG.LE.0 ) WRITE(LL,2002)
 2002 FORMAT(//2X,'**** ERROR IN RESPONSE ****')
C
      IF( IN(15).NE.0 ) THEN
C         NON-INTERACTIVE -- ANSWERS MUST BE INPUT FROM A CONTROL FILE
	  CLOSE(LU)
	  CLOSE(LL)
	  LU=IN(2)-(IN(2)/256)*256
	  LL=IN(2)/256
	  WRITE(LL,2000) QNO
	  WRITE(LR,2000) QNO
 2000     FORMAT(//2X,'**** ERROR WHEN READING FROM CONTROL FILE -- ',
     *          'QUESTION NO ',I3,' ****'//)
	  IF( FLAG.EQ.-1 ) THEN
	      WRITE(LL,2005)
	      WRITE(LR,2005)
 2005         FORMAT(2X,'**** UNEXPECTED ERROR ****')
	  ELSEIF( FLAG.EQ.-2 ) THEN
	      WRITE(LL,2006)
	      WRITE(LR,2006)
 2006         FORMAT(2X,'**** END-OF-FILE DETECTED ****')
	  ENDIF
	  CALL HELP
	  WRITE(LL,2007)
	  WRITE(LR,2007)
 2007     FORMAT(//2X,'Note'/2X,'****'
     *      /2X,'   It is quite  possible  that the error  has  been '
     *      /2X,'caused by an incorrect ( but not invalid ) response '
     *      /2X,'to an earlier question.')
	  IF( BATCH ) THEN
C             batch mode
	      CALL ABORT
	  ELSE
C             non batch
C
	      IF( IN(26).EQ.3 ) THEN
C                 TEST mode in C.F.G. program -- control file must
C                 be invalid
		  WRITE(LL,2009)
		  WRITE(LR,2009)
 2009             FORMAT(//2X,'*********  CONTROL FILE IS INVALID',
     *                    ' *********'//)
		  CALL ABORT
	      ELSE
C                 allow control to return to the user,and ask
C                 him to continue answering the questions
		  WRITE(LL,2008)
 2008             FORMAT(//2X,'Please continue by typing in ',
     *                 'responses to the remaining questions')
C
C                 reset 'interactive' and 'control common
C                 creation' flags
		  IN(15)=0
		  IN(1)=1
C                 Set FLAG=0 in case error has  been caused by
C                 'answer' HELP or QUIT
		  FLAG=0
	      ENDIF
	  ENDIF
      ENDIF
C
C
C
    1 CONTINUE
C     POSSIBLE LOOP BACK IF QUIT OR HELP RESPONSES LATER
C
C
C
      IF( FLAG.EQ.2 ) THEN
C         QUIT
	  WRITE (LL,2001) QNO
 2001     FORMAT (//2X,'DO YOU REALLY WANT TO QUIT?',
     *            /2X,'TYPE Y FOR YES',
     *          /2X,'IF YOU TYPE ANY OTHER CHARACTER,',
     *          ' QUESTION ',I3,' WILL BE REPEATED')
	  READ(LU,'(A)') ANS
	  CALL UCASE(ANS)
	  IF( ANS(1:1).EQ.'Y' ) CALL ABORT
      ELSEIF( FLAG.EQ.1 ) THEN
	  CALL HELP
      ELSE
	  IF( FLAG.LT.0 ) WRITE(LL,2003)
 2003     FORMAT(2X,'**** UNEXPECTED ERROR -- PLEASE TRY AGAIN ****')
	  WRITE(LL,2004) QNO
 2004     FORMAT(//2X,'IF YOU WISH TO QUIT, TYPE QUIT',
     *            /2X,'IF YOU REQUIRE HELP, TYPE HELP',
     *            /2X,'IF YOU TYPE ANY OTHER CHARACTER,',
     *                ' QUESTION ',I3,' WILL BE REPEATED')
	  READ(LU,'(A)') ANS
	  CALL UCASE(ANS)
	  IF( ANS.EQ.'QUIT' ) THEN
	      FLAG=2
	      GOTO 1
	  ELSEIF( ANS.EQ.'HELP' ) THEN
	      FLAG=1
	      GOTO 1
	  ENDIF
      ENDIF
C
C
C
      RETURN
C
C     **
C     **    END OF SUBROUTINE RSERR
C     **
C
      END
C+*+* RSERR  END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* HELP   START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE HELP
C
C     ******************************************************************
C     *                                                                *
C     *   THIS SUBROUTINE IS USED TO OBTAIN HELP INFORMATION FROM      *
C     *   THE HELP FILE                                                *
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
C     SUBPROGRAM HELP   REFERENCES THE FOLLOWING FUNCTIONS
C     ****************************************************
      INTEGER IUNIT
C
C     SUBPROGRAM HELP   IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     RSERR
C
C     LOCAL VARIABLES
C     ***************
      CHARACTER RECORD*72
      CHARACTER*30 NAME
      INTEGER DUNIT,RECNO,I,RECS,Q(18)
C
CCCC  WRITE ONE BLANK RECORD TO THE TERMINAL
C
      WRITE(LL,9996)
      WRITE(LR,9996)
C
CCCC  OPEN THE DIRECT ACCESS FILE DHELP WHICH HAS BEEN CREATED BY
CCCC  THE PROGRAM HCRLP
C
      NAME=NAMES(13)
C
      DUNIT=IUNIT()
      OPEN(DUNIT,FILE=NAME,STATUS='OLD',ERR=990,
C    *                      READONLY,SHARED,                            C-VAX
     *                      ACCESS='DIRECT',RECL=72)
C
CCCC  FIND WHICH RECORD CONTAINS THE START OF THE HELP INFORMATION
CCCC  FROM THE KEY RECORDS AT THE BEGINNING OF THE FILE
C
      RECNO=(QNO-100)/18+2
      READ (DUNIT,REC=RECNO) (Q(I),I=1,18)
      RECNO=Q(QNO-RECNO*18-63)
C
CCCC  IF THIS IS ZERO, NO HELP IS PROVIDED
C
      IF (RECNO.EQ.0) THEN
	 WRITE (LL,9999)
	 WRITE (LR,9999)
	 CLOSE (DUNIT)
	 RETURN
      ENDIF
C
CCCC  READ THE TOTAL NUMBER OF RECORDS IN THE FILE AND THE HEADER RECORD
CCCC  FOR THIS QUESTION NUMBER.
C
      READ (DUNIT,REC=1) RECS
      READ (DUNIT,REC=RECNO) RECORD
   10 CONTINUE
C
CCCC  INCREASE THE RECORD NUMBER BY 1.  IF IT IS NOW LARGER THAN THE
CCCC  NUMBER OF RECORDS IN THE FILE, CONTROL IS RETURNED TO THE CALLING
CCCC  PROGRAM UNIT
C
      RECNO=RECNO+1
      IF (RECNO.GT.RECS) THEN
	 CLOSE (DUNIT)
	 RETURN
      ENDIF
C
CCCC  READ THE NEXT RECORD.  IF IT IS A HEADER, THE HELP INFORMATION
CCCC  FOR THE CURRENT QUESTION IS OVER.
C
      READ (DUNIT,REC=RECNO) RECORD
      IF (RECORD(1:5).EQ.'*** Q') THEN
	 CLOSE (DUNIT)
	 RETURN
      ENDIF
      WRITE (LL,9998) RECORD
      WRITE (LR,9998) RECORD
      GOTO 10
C
C
C
  990 CONTINUE
      WRITE(LL,9997) NAME
      WRITE(LR,9997) NAME
      RETURN
C
CCCC  FORMAT STATEMENTS FOR SUBROUTINE HELP
C
 9996 FORMAT(/)
 9997 FORMAT('  ***** CANNOT OPEN HELP FILE ',A,' *****')
 9998 FORMAT(A)
 9999 FORMAT('  ***** NO HELP IS AVAILABLE FOR THIS QUESTION *****')
C
C     **
C     **    END OF SUBROUTINE HELP
C     **
C
      END
C+*+* HELP   END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* CHKFN  START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE CHKFN(NAME,*)
C
C     ******************************************************************
C     *                                                                *
C     *   AT SALFORD AN IMMEDIATE RETURN IS MADE BUT AT LOWESTOFT      *
C     *   THIS SUBROUTINE CHECKS THE VALIDITY OF A FILENAME AND,       *
C     *   IF NECESSARY, APPENDS THE DEFAULT CARTRIDGE NUMBER           *
C     *                                                                *
C     *   ARGUMENTS                                                    *
C     *   *********                                                    *
C     *   NAME - CHARACTER VARIABLE CONTAINING A FILENAME              *
C     *                                                                *
C     *          +++ HOST DEPENDENT SUBROUTINE +++                     *
C     *                                                                *
C     ******************************************************************
C
      IMPLICIT LOGICAL (A-Z)
C
C     ARGUMENT
C     ********
      CHARACTER NAME*(*)
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
C     SUBPROGRAM CHKFN  CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     LENTH
C
C     SUBPROGRAM CHKFN  IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     APPCN   CHKFN1  CREDR   FLNAM   HSU01   HSU03   QDD     QPSC
C     REFMT   SSU10
C
C     LOCAL VARIABLES
C     ***************
      CHARACTER*30 TEMP
      CHARACTER CN*3,NO*10
      INTEGER I,J,IND,L,D1,D2,D3,N
C
C     checks only on HP1000
C
      IF( .TRUE. ) RETURN
C
CCCC  INITIALISE VARIABLES
C
      TEMP=' '
      NO='0123456789'
C
CCCC  REMOVE ALL EXCEPT TRAILING BLANKS FROM NAME
C
      J=0
      DO 10 I=1,LEN(NAME)
	 IF (NAME(I:I).EQ.' ') GOTO 10
	 J=J+1
	 TEMP(J:J)=NAME(I:I)
   10 CONTINUE
C
CCCC  IF THE NAME IS NULL, PRINT AN ERROR MESSAGE AND RETURN
C
      IF (J.EQ.0) THEN
	 WRITE (LL,'(A)') 'BLANK FILE NAMES ARE NOT ALLOWED'
	 RETURN 1
      ENDIF
      NAME=TEMP
C
CCCC  IS THERE A CARTRIDGE NUMBER IN THE FILENAME
C
      IND=INDEX(NAME,'::')
      IF (IND.EQ.0) THEN
C
CCCC     IF NOT, APPEND THE DEFAULT CARTRIDGE NUMBER TO THE FILENAME
C
	 CALL LENTH(NAME,I,L)
	 D1=IN(19)/100
	 D3=IN(19)-100*D1
	 D2=D3/10
	 D3=D3-10*D2
	 D1=D1+1
	 D2=D2+1
	 D3=D3+1
	 CN=NO(D1:D1)//NO(D2:D2)//NO(D3:D3)
	 NAME(L+1:L+5)='::'//CN
      ELSE
C
CCCC     OTHERWISE, VALIDATE THE CARTRIDGE NUMBER
C
	 TEMP=NAME(IND+2:LEN(NAME))
	 N=INDEX(TEMP,' ')
	 DO 20 I=1,N-1
C
CCCC        PRINT AN ERROR MESSAGE AND RETURN IF THE CARTRIDGE NUMBER
CCCC        CONTAINS A NON-NUMERIC CHARACTER
C
	    IF (INDEX(NO,TEMP(I:I)).EQ.0) THEN
	       WRITE (LL,'(A)') 'NON-NUMERIC CHARACTER IN CARTRIDGE NO'
	       RETURN 1
	    ENDIF
   20    CONTINUE
C
CCCC     PRINT AN ERROR MESSAGE AND RETURN IF THE CARTRIDGE NUMBER IS
CCCC     MORE THAN THREE DIGITS IN LENGTH
C
	 IF (TEMP(4:4).NE.' ') THEN
	    WRITE (LL,'(A)') 'CARTRIDGE NUMBER MUST BE LESS THAN 1000'
	    RETURN 1
	 ENDIF
      ENDIF
      IF (INDEX(NAME,'::').GT.7) THEN
	 WRITE (LL,'(A)') 'FILE NAME MUST NOT BE MORE THAN 6 CHARACTERS'
	 RETURN 1
      ENDIF
C
C     **
C     **    END OF SUBROUTINE CHKFN
C     **
C
      END
C+*+* CHKFN  END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* CHKFN1 START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE CHKFN1(NAME1,FLGV)
C
C     ******************************************************************
C     *                                                                *
C     *   THIS SUBROUTINE                                              *
C     *                                                                *
C     ******************************************************************
C
      IMPLICIT LOGICAL (A-Z)
C
C     ARGUMENTS
C     *********
      LOGICAL FLGV
      CHARACTER NAME1*(*)
C
C     SUBPROGRAM CHKFN1 CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     CHKFN
C
C     SUBPROGRAM CHKFN1 IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     BTCH    WRCMN
C
C     LOCAL VARIABLES
C     ***************
      CHARACTER*30 NAME
C
      NAME=NAME1
      CALL CHKFN(NAME,*999)
C
      FLGV=.TRUE.
      RETURN
C
  999 CONTINUE
C
      FLGV=.FALSE.
      RETURN
C
C     **
C     **    END OF SUBROUTINE CHKFN1
C     **
C
      END
C+*+* CHKFN1 END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* APPCN  START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE APPCN(NAME)
C
C     ******************************************************************
C     *                                                                *
C     *   THIS SUBROUTINE IS USED TO APPEND THE DEFAULT CARTRIDGE NO   *
C     *   TO A FILENAME BY A CALL TO CHKFN. A DIRECT CALL TO CHKFN     *
C     *   IS NOT MADE TO AVOID THE ALTERNATE RETURN CONSTRUCTION       *
C     *                                                                *
C     ******************************************************************
C
      IMPLICIT LOGICAL (A-Z)
C
C     ARGUMENT
C     ********
      CHARACTER NAME*(*)
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
C     SUBPROGRAM APPCN  CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     ABORT   CHKFN
C
C     SUBPROGRAM APPCN  IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     APLOT   CPLOT   DPLOT   HPLOT   HSU01   HSU02   HSU03   HUCD2
C     IPLOT   JPLOT   KPLOT   QCS     QGG     REFMT   RPLOT   SPLOT
C     SSU35   TPLOT   XPLOT
C
      CALL CHKFN(NAME,*999)
C
C
      RETURN
C
C
  999 CONTINUE
C
C
      WRITE(LL,2999) NAME
 2999 FORMAT(//'++++ UNEXPECTED ERROR IN APPCN ++++'/'FILENAME IS ',A)
C
      CALL ABORT
C
C
C     **
C     **    END OF SUBROUTINE APPCN
C     **
C
      END
C+*+* APPCN  END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* INQIRE START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE INQIRE(NAME,OPEN)
C
C     ******************************************************************
C     *                                                                *
C     *   THIS SUBROUTINE IS USED TO DETERMINE WHETHER A FILE IS       *
C     *   ALREADY OPEN - NOTE THAT THE FORTRAN77 INQUIRE, AS           *
C     *   IMPLEMENTED AT SALFORD AND LOWESTOFT, IS ONLY CONCERNED      *
C     *   WITH FILE CONNECTIONS TO THE 'CURRENT' PROGRAM AND DOES      *
C     *   NOT INDICATE WHETHER A FILE HAS BEEN OPENED BY ANOTHER       *
C     *   PROGRAM                                                      *
C     *                                                                *
C     *          +++ HOST DEPENDENT SUBROUTINE +++                     *
C     *                                                                *
C     ******************************************************************
C
      IMPLICIT LOGICAL (A-Z)
C
C     ARGUMENTS
C     *********
      CHARACTER NAME*(*)
      LOGICAL OPEN
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
C     SUBPROGRAM INQIRE CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     ER777
C
C     SUBPROGRAM INQIRE REFERENCES THE FOLLOWING FUNCTIONS
C     ****************************************************
      INTEGER IUNIT
C
C     SUBPROGRAM INQIRE IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     HSU01   HSU02   HSU03
C
C     LOCAL VARIABLES
C     ***************
      INTEGER LO,IOSTAT,IOP
C
      LO=IUNIT()
      OPEN(LO,FILE=NAME,STATUS='UNKNOWN',ERR=777,IOSTAT=IOSTAT)
      CLOSE(LO)
C
      OPEN=.FALSE.
      RETURN
C
  777 CONTINUE
C
CCC   SET UP IOSTAT VALUE FOR 'FILE ALREADY OPEN'
C
      IF( PRIME ) THEN
	  IOP=124
      ELSEIF( VAX ) THEN
C         VAX error is "open failure" rather than "unit alreay open"
	  IOP=30
      ENDIF
C
      IF( IOSTAT.EQ.IOP ) THEN
C         FILE ALREADY OPEN?
	  OPEN=.TRUE.
	  RETURN
      ENDIF
C
      CALL ER777(NAME,'INQIRE',IOSTAT)
C
C
C     **
C     **    END OF SUBROUTINE INQIRE
C     **
C
      END
C+*+* INQIRE END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* ER777  START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE ER777(FNAME,SUBR,IOSTAT)
C
C     ******************************************************************
C     *                                                                *
C     *   THIS SUBROUTINE TRAPS UNEXPECTED ERRORS WHEN OPENING AN      *
C     *   OLD FILE.                                                    *
C     *                                                                *
C     *   ARGUMENTS                                                    *
C     *   *********                                                    *
C     *   FNAME  - CHARACTER VARIABLE WHOSE VALUE, ON ENTRY, IS THE    *
C     *            NAME OF THE FILE WHOSE ATTEMPTED OPENING HAS        *
C     *            CAUSED AN ERROR.  UNCHANGED ON EXIT.                *
C     *   SUBR   - CHARACTER VARIABLE WHOSE VALUE, ON ENTRY, IS THE    *
C     *            NAME OF THE SUBROUTINE IN WHICH THE OPENING OF      *
C     *            THE FILE WAS ATTEMPTED.  UNCHANGED ON EXIT.         *
C     *   IOSTAT - INTEGER VARIABLE WHOSE VALUE, ON ENTRY, IS THE      *
C     *            IOSTAT VALUE RETURNED WHEN THE OPENING OF THE       *
C     *            FILE WAS ATTEMPTED.  UNCHANGED ON EXIT.             *
C     *                                                                *
C     ******************************************************************
C
      IMPLICIT LOGICAL (A-Z)
C
C     ARGUMENTS
C     *********
      CHARACTER FNAME*(*),SUBR*(*)
      INTEGER IOSTAT
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
C     SUBPROGRAM ER777  CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     ABORT
C
C     SUBPROGRAM ER777  IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     APLOT   CPLOT   DPLOT   HUCD2   INQIRE  OUTSEG  REPLT   SAVED
C     UPDATE  WRMRC
C
      WRITE(LL,2000) FNAME,IOSTAT,SUBR
      IF( LR.GE.IN(31) ) THEN
C        LR must have been allocated and report file open
	 WRITE(LR,2000) FNAME,IOSTAT,SUBR
      ENDIF
 2000 FORMAT(//2X,'**** I-O ERROR ****',
     *       /6X,'FILE IS         ',A,
     *       /6X,'IOSTAT    =     ',I4,
     *       /6X,'IN SUBROUTINE   ',A)
C
C
      CALL ABORT
      RETURN
C
C     **
C     **    END OF SUBROUTINE ER777
C     **
C
      END
C+*+* ER777  END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* INITCM START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE INITCM(FIRST)
C
C     ******************************************************************
C     *                                                                *
C     *   THIS SUBROUTINE INITIALISES COMMON BLOCK VARIABLES           *
C     *                                                                *
C     ******************************************************************
C
      IMPLICIT LOGICAL (A-Z)
C
C     ARGUMENTS
C     *********
      LOGICAL FIRST
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
C     SUBPROGRAM INITCM  IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     HSU01   HSU02   HSU03
C
C     PARAMETERS
C     **********
      INTEGER CVALS,LVALS,IVALS,RVALS
      PARAMETER( CVALS=12,LVALS=30,IVALS=130,RVALS=520 )
C
C     LOCAL VARIABLES
C     ***************
      CHARACTER*30 NAMS2,NAMS3,NAMS9,NAMS10,NAMS11
      LOGICAL LGSV3,LGSV8,LGSV9,LGSV10,LGSV12,LGSV13,LGSV14
      INTEGER I,IN2,IN6,IN10,IN16,IN19,IN25,
     *        IN27,IN28,IN29,IN30,IN31,IN32
C
C     EQUIVALENCED VARIABLES
C     **********************
      INTEGER IARRAY(IVALS)
      REAL*8 RARRAY(RVALS)
      EQUIVALENCE (IARRAY(1),IN(1)),(RARRAY(1),RDD(1))
C
      IF( .NOT. FIRST ) THEN
C
CCC       set REPEAT flag
C
	  IF( IDD(1).EQ.3 .OR. LGSV(8) ) THEN
C             if tabular data, assume REPEAT -- or if already REPEATing.
C             Either condition alone is insufficient -- the first is
C             needed to 'trigger' the REPEATing but IDD(1) may be unset
C             if c.f. contains incorrect **REPEAT modifications. If
C             serious error occurs in setting up mofifications, then
C             IDD(1) will be -9999 and LGSV(8) false.
C
	      LGSV8=.TRUE.
	  ELSE
	      LGSV8=.FALSE.
	  ENDIF
C
CCC       Following entries always need 'preserving'
C
	  NAMS9 =NAMES(9)
	  NAMS10=NAMES(10)
	  NAMS11=NAMES(11)
C
	  LGSV9 =LGSV(9)
	  LGSV10=LGSV(10)
	  LGSV12=LGSV(12)
	  LGSV13=LGSV(13)
	  LGSV14=LGSV(14)
C
	  IN2 =IN(2)
	  IN10=IN(10)
	  IN19=IN(19)
	  IN25=IN(25)
	  IN27=IN(27)
	  IN28=IN(28)
	  IN29=IN(29)
	  IN30=IN(30)
	  IN31=IN(31)
	  IN32=IN(32)
C
	  IF( LGSV8 ) THEN
C             if REPEAT also need saving
	      NAMS2 =NAMES(2)
	      NAMS3 =NAMES(3)
C
	      LGSV3=LGSV(3)
C
	      IN6 =IN(6)
	      IN16=IN(16)
C
	  ENDIF
C
      ENDIF
C
C
C
CCC   Initialise cntrl common
C
      QNO=-999
C
      DO 10 I=1,CVALS
	  NAMES(I)='undefined'
   10 CONTINUE
C
      DO 20 I=1,LVALS
	  LGSV(I)=.TRUE.
   20 CONTINUE
      LGSV(1) =.FALSE.
      LGSV(11) =.FALSE.
C
      DO 30 I=1,IVALS
	  IARRAY(I)=-9999
   30 CONTINUE
C
      DO 40 I=1,RVALS
	  RARRAY(I)=-1.0E+37
   40 CONTINUE
C
C
C
      IF( FIRST ) THEN
C
CCC       Following need 'setting' on first entry
C
	  IN(10)=0
	  IN(15)=0
	  IN(25)=0
C
	  LGSV(8)=.FALSE.
	  LGSV(9)=.FALSE.
      ELSE
C
CCC       Following entries need 're-setting'
C
	  NAMES(9) =NAMS9
	  NAMES(10)=NAMS10
	  NAMES(11)=NAMS11
C
	  LGSV(8) =LGSV8
	  LGSV(9) =LGSV9
	  LGSV(10)=LGSV10
	  LGSV(12)=LGSV12
	  LGSV(13)=LGSV13
	  LGSV(14)=LGSV14
C
	  IN(2) =IN2
	  IN(10)=IN10
	  IN(19)=IN19
	  IN(25)=IN25
	  IN(27)=IN27
	  IN(28)=IN28
	  IN(29)=IN29
	  IN(30)=IN30
	  IN(31)=IN31
	  IN(32)=IN32
C
	  IF( LGSV8 ) THEN
C             if REPEAT also need re-setting
	      NAMES(2) =NAMS2
	      NAMES(3) =NAMS3
C
	      LGSV(3)=LGSV3
C
	      IN(6) =IN6
	      IN(16)=IN16
C
	  ENDIF
C
      ENDIF
C
CCC   special effect for CFILE
C
      IDD(1)=0
C
C
C
      RETURN
C
C     **
C     **    END OF SUBROUTINE INITCM
C     **
C
      END
C+*+* INITCM END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* ISGN   START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      INTEGER FUNCTION ISGN(I1,I2,I3)
C
C     ******************************************************************
C     *                                                                *
C     *    THIS FUNCTION IS USED TO FIND 'SIGN' OF LAT/LONG IN         *
C     *    CONVERSION OF LAT/LONG TO DECIMAL                           *
C     *                                                                *
C     *    ARGUMENTS                                                   *
C     *    *********                                                   *
C     *    II - DEGREES                                                *
C     *    I2 - MINUTES                                                *
C     *    I3 - SECS                                                   *
C     *                                                                *
C     ******************************************************************
C
      IMPLICIT LOGICAL (A-Z)
C
C     ARGUMENTS
C     *********
      INTEGER I1,I2,I3
C
C     SUBPROGRAM ISGN   IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     REFMT
C
C     LOCAL VARIABLES
C     ***************
C     NONE
C
      IF( I1.NE.0 ) THEN
	  ISGN=SIGN(1,I1)
      ELSEIF( I2.NE.0 ) THEN
	  ISGN=SIGN(1,I2)
      ELSE
	  ISGN=SIGN(1,I3)
      ENDIF
C
      RETURN
C
C     **
C     **    END OF FUNCTION ISGN
C     **
C
      END
C+*+* ISGN   END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* DATTIM START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE DATTIM(CDATE,CTIME)
C
C     ******************************************************************
C     *                                                                *
C     *        THIS SUBROUTINE DETERMINES THE CURRENT DATE AND TIME,   *
C     *        AND RETURNS THEM AS TWO CHARACTER STRINGS IN THE FORMAT *
C     *           day/month/year   hours.minutes.secs                  *
C     *             dd/mm/yy            hh.mm.ss                       *
C     *                                                                *
C     *          +++ HOST DEPENDENT SUBROUTINE +++                     *
C     *                                                                *
C     ******************************************************************
C
      IMPLICIT LOGICAL (A-Z)
C
C     ARGUMENTS
C     *********
      CHARACTER*8 CDATE,CTIME
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
C     SUBPROGRAM DATTIM REFERENCES THE FOLLOWING FUNCTIONS
C     ****************************************************
      CHARACTER*8 DATE, TIME
C
C     SUBPROGRAM DATTIM IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     HSU01   RPORT   TTABL
C
C     LOCAL VARIABLES
C     ***************
      INTEGER*2 WORK(28),NUM,IHOUR,IMIN,IDAY,IMONTH,IYEAR
      CHARACTER*2 BUFFER,DAY,MONTH,YEAR,HOUR,MIN,SEC
      CHARACTER*8 VTIME,PCDATE,PCTIME
C
      IF (PRIME) THEN
CCC      see PRIMOS  Sub Ref Guide, Rev19.0  p10-45
CCC      PRIMOS ROUTINE TIMDAT
	 NUM=28
C        CALL TIMDAT(WORK,NUM)                                          C-PRIME
C
	 WRITE(BUFFER,'(A2)') WORK(2)
	 READ(BUFFER,'(A)') DAY
	 WRITE(BUFFER,'(A2)') WORK(1)
	 READ(BUFFER,'(A)') MONTH
	 WRITE(BUFFER,'(A2)') WORK(3)
	 READ(BUFFER,'(A)') YEAR
C
	 IMIN=WORK(4)
	 IHOUR=IMIN/60
	 IMIN=IMIN-IHOUR*60
C
	 WRITE(BUFFER,'(I2.2)') IHOUR
	 READ(BUFFER,'(A)') HOUR
	 WRITE(BUFFER,'(I2.2)') IMIN
	 READ(BUFFER,'(A)') MIN
	 WRITE(BUFFER,'(I2.2)') WORK(5)
	 READ(BUFFER,'(A)') SEC
C
      ELSEIF (VAX) THEN
CCC      see VAX FORTRAN manual p C28 ?
CCC      IDAY etc initialised to suppress compiler warning
	 IDAY=0
	 IMONTH=0
	 IYEAR=0
	 VTIME=' '
C        CALL IDATE(IMONTH,IDAY,IYEAR)                                  C-VAX
C
	 WRITE(BUFFER,'(I2.2)') IDAY
	 READ(BUFFER,'(A)') DAY
	 WRITE(BUFFER,'(I2.2)') IMONTH
	 READ(BUFFER,'(A)') MONTH
	 WRITE(BUFFER,'(I2.2)') IYEAR
	 READ(BUFFER,'(A)') YEAR
C
C        CALL TIME(VTIME)                                               C-VAX
	 HOUR=VTIME(1:2)
	 MIN =VTIME(4:5)
	 SEC =VTIME(7:8)
C
      ELSEIF (PC386) THEN
CCC      see FTN77/386 Reference Manual
CCC      PC Routines DATE and TIME (NB American date, not European date)
	 PCDATE = DATE()
	 DAY = PCDATE(4:5)
	 MONTH = PCDATE(1:2)
	 YEAR = PCDATE(7:8)
C
	 PCTIME = TIME()
	 HOUR = PCTIME(1:2)
	 MIN = PCTIME(4:5)
	 SEC = PCTIME(7:8)
C
      ENDIF
C
      CDATE=DAY//'/'//MONTH//'/'//YEAR
      CTIME=HOUR//'.'//MIN//'.'//SEC
C
CCCC  WRITE DATE AND TIME TO CONTROL COMMON
C
      NAMES(15)='    '//CDATE//'      '//CTIME//'    '
C
C     **
C     **    END OF SUBROUTINE DATTIM
C     **
C
      END
C+*+* DATTIM END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C+*+* UPDATE START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      SUBROUTINE UPDATE
C
C     ******************************************************************
C     *                                                                *
C     *        THIS SUBROUTINE LISTS A 'UPDATE' FILE AT THE TERMINAL   *
C     *        TO KEEP USERS INFORMED OF ANY CHANGES TO THE PACKAGE    *
C     *                                                                *
C     ******************************************************************
C
      IMPLICIT LOGICAL (A-Z)
C
C     ARGUMENTS
C     *********
C     NONE
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
C     SUBPROGRAM UPDATE CALLS THE FOLLOWING SUBROUTINES
C     *************************************************
C     ER777
C
C     SUBPROGRAM UPDATE REFERENCES THE FOLLOWING FUNCTIONS
C     ****************************************************
      INTEGER IUNIT
C
C     SUBPROGRAM UPDATE IS CALLED BY THE FOLLOWING SUBPROGRAMS
C     ********************************************************
C     HSU01
C
C     LOCAL VARIABLES
C     ***************
      CHARACTER FNAME*30,RECORD*72
      INTEGER IOSTAT,L1,I
C
      FNAME=NAMES(16)
C     return if facility switched off
      IF( FNAME .EQ. 'OFF' ) RETURN
C
      L1=IUNIT()
      OPEN(L1,FILE=FNAME,STATUS='OLD',ERR=777,IOSTAT=IOSTAT)
C     OPEN(LK,FILE=NAME,STATUS='UNKNOWN')                               C-PRIME
C     OPEN(LK,FILE=NAME,STATUS='UNKNOWN')                               C-HP9000
C     OPEN(L1,FILE=FNAME,STATUS='OLD',ERR=777,IOSTAT=IOSTAT,READONLY,   C-VAX
C    *   SHARED)                                                        C-VAX
C
      WRITE(LL,'(//)')
      DO 10 I=1,20000
	 READ(L1,'(A)',END=11) RECORD
	 WRITE(LL,'(1X,A)') RECORD
   10 CONTINUE
   11 CONTINUE
      WRITE(LL,'(//)')
      CLOSE(L1)
C
      RETURN
C
C
C
  777 CONTINUE
C
      CALL ER777(FNAME,'UPDATE',IOSTAT)
C
C
C     **
C     **    END OF SUBROUTINE UPDATE
C     **
C
      END
C+*+* UPDATE END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
