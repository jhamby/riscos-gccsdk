      PROGRAM INQ3
C           INQUIRE problem 3
C          asking whether file is SEQUENTIAL, wrong replies again
C          when no file exists
      CHARACTER SEQ(2)*8
      DATA SEQ/2*'********'/
C           UNKNOWN
      INQUIRE(10,SEQUENTIAL=SEQ(1))
      INQUIRE(FILE='ABC_123',SEQUENTIAL=SEQ(2))
      PRINT 101,'UNKNOWN ',SEQ
  101 FORMAT(1X,3A8)
      END
C        responds with
*
* UNKNOWN YES     YES
*
C    despite the fact that neither unit 10 nor file 'ABC_123' are defined
C    from ANSI 12.10.3.3 it would seem best to reply with 'UNKNOWN' for both
C    cases since an answer is expected under any circumstances.


