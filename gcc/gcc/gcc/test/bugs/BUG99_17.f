      PROGRAM BUG17
      PARAMETER(NI=12,NJ=9)
      DIMENSION nn(ni,nj)
      DATA NN/
     1 3298,3304,4241,4291,4594,4710,4914,4942,4861,4914,4563,4892,
     2 1150,1034,1089,1126,1142,1100,1112,1099,1114,1079,999,1181,
     3 180,190,370,530,800,1130,1320,990,580,320,250,212,
     4 874,768,630,516,385,324,277,272,271,381,533,760,
     5 299,264,258,247,273,269,251,269,271,279,297,266,
     6 168,142,122,140,153,142,147,160,162,172,266,230,
     7 298,277,346,263,253,239,268,228,240,260,252,241,
     8 267,193,144,127,70,63,55,53,60,118,150,172,
     9 1264,1234,1172,1220,1547,1339,1419,1453,1359,1308,1264,1246/
      CALL SUB17(NN,NI,NJ)
      END
C
      SUBROUTINE SUB17(NN,NI,NJ)
      DIMENSION NN(NI,NJ)
      SUM=105321.
      H=0.
      DO 18 I=1,NI
        DO 17 J=1,NJ
          IF(NN(I,J).NE.0) THEN
            H=H-NN(I,J)/SUM
          ENDIF
17      CONTINUE
18    CONTINUE
      PRINT 101,SUM,H
  101 FORMAT(' SUM=',F10.0,' H=',F12.5)
      RETURN
      END
C
C the answers from this routine depend on the amount of optimisation used
C       optimisation off or level one results are
C  SUM=   105321. H=    -1.00000
C asking for level 2 optimisation gives 
C  SUM=   105321. H=    -1.13012
C running with RISCOS 3.70
C   Gets the right answer if the calculation is in the main program, but in
C   real code this is rarely possible, most calculations are not done in 
C   the main program but in the subroutines
C
C   this is only one of the optimisation errors found while attempting to
C  use GNUf77 2.95.1 to compile the standard code of the 'Numerical Recipes'
C  library. 

