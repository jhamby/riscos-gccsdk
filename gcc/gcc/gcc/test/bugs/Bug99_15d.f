      PROGRAM INQ4
C           INQUIRE problem 4 similar to previous ones but with UNFORMATTED
      CHARACTER UFOR(2)*8
      DATA UFOR/2*'********'/
C           UNKNOWN
      INQUIRE(10,UNFORMATTED=UFOR(1))
      INQUIRE(FILE='ABC_123',UNFORMATTED=UFOR(2))
      PRINT 101,'UNKNOWN ',UFOR
  101 FORMAT(1X,3A8)
      END
C        responds with
*
* UNKNOWN YES     UNKNOWN
*
C    despite the fact that unit 10 is not defined
C    from ANSI 12.10.3.3 it would seem best to reply with 'UNKNOWN'
C    since an answer is expected under any circumstances.
