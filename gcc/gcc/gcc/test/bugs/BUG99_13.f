      PROGRAM BUG99_13
C           illegal octal input accepted without warning
      CALL OCTAL('12345670')
      CALL OCTAL('8')
      CALL OCTAL('9')
      CALL OCTAL('A')
      END
C
      SUBROUTINE OCTAL(TEXT)
      CHARACTER TEXT*(*)
      PRINT 101,TEXT
  101 FORMAT('reading "',A,'" as an octal number')
      READ(TEXT,102,ERR=90)IOCT
  102 FORMAT(O8)
      PRINT 103,IOCT,IOCT
  103 FORMAT('interpreted as ',O8,'(octal),',I8,'(decimal)')
      RETURN
   90 PRINT *,'error interpreting as octal'
      RETURN
      END
C          this gives:
*
*reading "12345670" as an octal number
*interpreted as 12345670(octal), 2739128(decimal)
*reading "8" as an octal number
*interpreted as       10(octal),       8(decimal)
*reading "9" as an octal number
*interpreted as       11(octal),       9(decimal)
*reading "A" as an octal number
* error interpreting as octal
*
C  the analogous hexadecimal read of 'G' correctly gives an error
