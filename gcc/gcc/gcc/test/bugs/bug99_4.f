      PROGRAM BUG99_4
C              divide by zero should give an error of some sort !!!!
      J = 0
      PRINT *,20/J
C        this gives the answer 20 with no error and reaches the next statement
      STOP 'OK'
      END
