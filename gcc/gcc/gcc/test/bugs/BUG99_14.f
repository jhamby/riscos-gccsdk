      PROGRAM BUG14
C            problems reading integers which are too large, no errors seen
C            on INTEGER *2, INTEGER *4 and BYTE
C            first the biggest/smallest integers INTEGR *2
      CALL RINT1('127')
      CALL RINT1('-128')
      PRINT *,'             legal range BYTE above, illegal below'
C           now some outside legal range of BYTE
      CALL RINT1('200')
      CALL RINT1('-500')
C
      CALL RINT2('32767')
      CALL RINT2('-32768')
      PRINT *,'             legal range INTEGER *2 above, illegal below'
C           now some outside legal range
      CALL RINT2('40000')
      CALL RINT2('-500000')
C           next the same for integer*4
C            first the biggest/smallest integers
      CALL RINT4('2147483647')
      CALL RINT4('-2147483648')
      PRINT *,'             legal range INTEGER *4 above, illegal below'
C           now some outside legal range
      CALL RINT4('4000000000')
      CALL RINT4('-5000000000')
      END
C
      SUBROUTINE RINT1(TEXT)
      CHARACTER TEXT*(*),TEXT10*10
C           read BYTE variable from TEXT
      BYTE I
      TEXT10=TEXT
      READ(TEXT10,101,IOSTAT=IERR)I
  101 FORMAT(I10)
      IF(IERR.EQ.0) THEN
        PRINT 102,TEXT,I
  102   FORMAT('Read ',A,' as BYTE',I7)
      ELSE
        PRINT 103,IERR,TEXT
  103   FORMAT('Error',I4,' reading ',A,' as BYTE')
      ENDIF
      RETURN
      END    
C
      SUBROUTINE RINT2(TEXT)
      CHARACTER TEXT*(*),TEXT10*10
C           read integer*2 variable from TEXT
      INTEGER*2 I
      TEXT10=TEXT
      READ(TEXT10,101,IOSTAT=IERR)I
  101 FORMAT(I10)
      IF(IERR.EQ.0) THEN
        PRINT 102,TEXT,I
  102   FORMAT('Read ',A,' as integer*2',I7)
      ELSE
        PRINT 103,IERR,TEXT
  103   FORMAT('Error',I4,' reading ',A,' as integer*2')
      ENDIF
      RETURN
      END    
C
      SUBROUTINE RINT4(TEXT)
      CHARACTER TEXT*(*),TEXT12*12
C           read integer*4 variable from TEXT
      INTEGER I
      TEXT12=TEXT
      READ(TEXT12,101,IOSTAT=IERR)I
  101 FORMAT(I12)
      IF(IERR.EQ.0) THEN
        PRINT 102,TEXT,I
  102   FORMAT('Read ',A,' as integer*4',I12)
      ELSE
        PRINT 103,IERR,TEXT
  103   FORMAT('Error',I4,' reading ',A,' as integer*4')
      ENDIF
      RETURN
      END
C           this gives:
*
*Read 32767 as integer*2  32767
*Read -32768 as integer*2 -32768
*Read 40000 as integer*2 -25536
*Read -500000 as integer*2  24288
*Read 2147483647 as integer*4  2147483647
*Read -2147483648 as integer*4 -2147483648
*Read 4000000000 as integer*4  -294967296
*Read -5000000000 as integer*4  -705032704
*
    
