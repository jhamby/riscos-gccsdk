      PROGRAM INQ2
C           INQUIRE problem 2
C     asking whether a file is FORMATTED, when file does not exist
C        and no file OPENed for that UNIT
      CHARACTER FOR(2)*8
      DATA FOR/2*'********'/
C           UNKNOWN
      INQUIRE(10,FORMATTED=FOR(1))
      INQUIRE(FILE='ABC_123',FORMATTED=FOR(2))
      PRINT 101,'UNKNOWN ',FOR
  101 FORMAT(1X,3A8)
      END
C        responds with
*
* UNKNOWN NO      YES
*
C    despite the fact that neither unit 10 nor file 'ABC_123' are defined
C    from ANSI 12.10.3.3 it would seem best to reply with 'UNKNOWN' for both
C    cases since an answer is expected under any circumstances.


