      PROGRAM BUG5
C           OPEN:  RECL must be specified for ACCESS='DIRECT'
C          (ANSI page 12-20)
      OPEN(10,FILE='NORECL',ACCESS='DIRECT',STATUS='NEW',IOSTAT=IERR,
     + FORM='UNFORMATTED')
      PRINT *,'opened direct access with no RECL, IOSTAT=',IERR
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
C        
C  real problem here is that after executing the OPEN statement
C the status return code, IOSTAT should not have been zero