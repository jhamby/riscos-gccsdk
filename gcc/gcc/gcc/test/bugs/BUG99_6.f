      PROGRAM BUG6
C           OPEN:  RECL must be positive
C          (ANSI page 12-20)
      OPEN(10,FILE='NORECL',ACCESS='DIRECT',STATUS='NEW',IOSTAT=IERR,
     + FORM='UNFORMATTED',RECL=-12)
      PRINT *,'opened direct access with -ve RECL, IOSTAT=',IERR
      IF(IERR.NE.0) STOP
      DO 10 I=1,10
        WRITE(10,REC=I)11-I
   10 CONTINUE
      CLOSE(10)
      END
c  gives:
c   opened direct access with no RECL, IOSTAT= 0
c  do_ud: off end of record
c  apparent state: unit 10 named NORECL
c  last format: list io
c  lately writing direct unformatted external IO
c
c  Fatal signal received: Aborted
