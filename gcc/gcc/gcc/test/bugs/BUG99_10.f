      PROGRAM BUG10
C         problems with writing list-directed to a CHARACTER variable
C          which is too short, no warning given?
C         this should not hang the computer
C         although this is not in the ANSI standard it is a common extension
      CHARACTER A*20,B*40
      DIMENSION R(2)
      R(1) = 1./3.
      R(2) = 2./3.
      WRITE(B,*)R
      PRINT 101,40,B
  101 FORMAT('writing to a',I3,'-character variable'/A/)
      WRITE(A,102)R
  102 FORMAT(2F8.4)
      PRINT 101,20,A
      WRITE(A,*)R
      PRINT 101,20,A
      STOP 'OK'
      END
C     this gives when running:
C writing to a 40-character variable
C   0.333333343  0.666666687
C
C writing to a 20-character variable
C 0.3333  0.6667    
*         and then the computer hangs 
C   ESCAPE does nothing           

