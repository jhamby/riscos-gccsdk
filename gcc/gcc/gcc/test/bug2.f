       PROGRAM TEST
       
       REAL A,B,C,D,E
       NAMELIST /FRED/ A,B,C,D,E
       
       OPEN(UNIT=1, FILE='BILL', READONLY)
       OPEN(UNIT=2,FILE='OUTPUT')
       READ(1,NML=FRED)
       WRITE(2,NML=FRED)
       A = 165
       B = 65.6
       C=3.54
       D=20.0
       E=88.0
       WRITE(2,NML=FRED)
       STOP
       
       END
