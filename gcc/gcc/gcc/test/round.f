      PROGRAM GNUBUG
C           GNUF77 is not able to print the floating number 12.0
      REAL A11,A12
      A11 = 11.0
      A12 = 12.0
      PRINT 101,A11,A11,A11,A12,A12,A12
  101 FORMAT(F6.0,F14.7,2X,Z8)
C            this gives:
C   11.    11.0000000  41300000
C   11.    11.9999999  41400000
C            so it is stored OK, it just can't print it.
      END
