      PROGRAM BUG8
C         rounding problems when writing real variables
      DIMENSION R(2)
      R(1) = 1./3.
      R(2) = 2./3.
      PRINT *,R
      PRINT 101,R
C         THIS PRECISION SHOULD ONLY BE 7 DECIMAL PLACES
  101 FORMAT(1X,2 F14.7)
      END
C         gives:
c    0.333333343  0.666666687
C      0.3333333     0.6666667