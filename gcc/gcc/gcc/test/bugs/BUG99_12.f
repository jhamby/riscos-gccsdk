      PROGRAM BUG99_12
C            hexadecimal format input has problems
      INTEGER IHEX(4)
      CHARACTER*8 TEXT(4)
C            first prove that output is OK
      DATA IHEX/'12345678'X,'87654321'X,'FEDCBA98'X,'89ABCDEF'X/
      DATA TEXT/'12345678' ,'87654321' ,'FEDCBA98' ,'89ABCDEF' /
      PRINT 101,IHEX,TEXT
  101 FORMAT('the next two rows should be identical'/4Z10/4(2X,A8))
C             clear the array
      DO 10 I=1,4
        IHEX(I) = 0
   10 CONTINUE
      PRINT *,'now input the hex data'
C            now try to input HEX data from the TEXT array
      READ(TEXT,102)IHEX
  102 FORMAT(Z8)
      PRINT 101,IHEX,TEXT
      END
C           this gives:
*
*the next two rows should be identical
*  12345678  87654321  FEDCBA98  89ABCDEF
*  12345678  87654321  FEDCBA98  89ABCDEF
* now input the hex data
*the next two rows should be identical
*    780000    210000    980000    EF0000
*  12345678  87654321  FEDCBA98  89ABCDEF
*
C         showing that it can output but not input in HEX format

