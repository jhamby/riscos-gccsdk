      PROGRAM BUG9
C         insufficient precision writing DOUBLE PRECISION list-directed
C         at least 15 decimal places should be printed in this example
      DOUBLE PRECISION D(2)
      D(1) = 1.D0/3.D0
      D(2) = 2.D0/3.D0
      PRINT *,D
C            these variables should be accurate to 15 places
      PRINT 101,D
  101 FORMAT(1X, 2F20.15 )
      END
C        gives:
c  0.333333333  0.666666667
C    0.333333333333333   0.666666666666667