      PROGRAM BUG99_11
C            problem in printing INTEGER*2 
C            list directed is OK, but not when using a Format statement I7 
      INTEGER*2 I2(4)
      DATA I2/63,1023,32765,-32767/
      PRINT *, ' values of I2 should be 63,1023,32765,-32767'
      PRINT *, ' they are ',I2
      PRINT 111,I2
  111 FORMAT('integer*2 printed with format statement are ',4I7)
      DO 10 J=1,4
      PRINT 113,I2(J)
C      PRINT *,' value of I2(J) =',I2(J)
C      PRINT 112,J,I2(J)
   10 CONTINUE
  112 FORMAT(' J=',I2,' I2(J)= with FORMAT I7 is:',I7)
  113 FORMAT(I7)
      END
