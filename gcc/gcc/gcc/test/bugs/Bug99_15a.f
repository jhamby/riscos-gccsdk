      PROGRAM INQ1
C           INQUIRE problem 1 
C           Inaccurate reply given to query whether file is Direct accesss
C           when no file exists, two different queries here, both wrong
      CHARACTER DIR(2)*8
      DATA DIR/2*'********'/
C           UNKNOWN
      INQUIRE(10,DIRECT=DIR(1))
      INQUIRE(FILE='ABC_123',DIRECT=DIR(2))
      PRINT 101,'UNKNOWN ',DIR
  101 FORMAT(1X,3A8)
      END
C        responds with
*
* UNKNOWN NO      YES
*
C    despite the fact that neither unit 10 nor file 'ABC_123' are defined
C    from ANSI 12.10.3.3 it would seem best to reply with 'UNKNOWN' for both
C    cases since an answer is expected under any circumstances.


