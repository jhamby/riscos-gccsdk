      PROGRAM INQ5
C          to test error returns from INQUIRE
C               first call it with an illegal filename
      INQUIRE(FILE='ABC$123',IOSTAT=IERR,RECL=LRECL)
      IF(IERR.EQ.0) THEN
        PRINT *,'Error not detected by IOSTAT with illegal filename'
      ELSE
        PRINT 101,IERR
  101   FORMAT(' Error',I4,' detected by IOSTAT')
      ENDIF
C             secondly with an illegal unit number
      INQUIRE(UNIT=-10,ERR=10,RECL=LRECL)
      PRINT *,'Error not detected by ERR= when UNIt number illegal'
      RETURN
   10 PRINT *,'Error detected by ERR='
      RETURN
      END
C              gives:
* Error not detected by IOSTAT
* Error not detected by ERR=
