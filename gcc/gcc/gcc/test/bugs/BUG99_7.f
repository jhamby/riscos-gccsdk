      PROGRAM BUG99_7
C     OPEN and CLOSE keywords are allowed trailing blanks in character strings
C           see 12.10.1 in ANSI standard
      OPEN(10,FILE='fred ',STATUS='NEW ', ACCESS='SEQUENTIAL ',
     + FORM='UNFORMATTED ',RECL=64)
      CLOSE(10,STATUS='DELETE  ')
      END
C      during comilation gives:
c Compiling bug7
c bug7.f: In program `bug7':
c bug7.f:4: 
c          OPEN(10,FILE='fred ',STATUS='NEW ', ACCESS='SEQUENTIAL ',
c                                      ^
c Unrecognized value for character constant at (^) -- expecting NEW, OLD, REPLACE, SCRATCH, or UNKNOWN
c bug7.f:4: 
c          OPEN(10,FILE='fred ',STATUS='NEW ', ACCESS='SEQUENTIAL ',
c                                                     ^
c Unrecognized value for character constant at (^) -- expecting APPEND, DIRECT, KEYED, or SEQUENTIAL
c bug7.f:5: 
c         + FORM='UNFORMATTED ',RECL=64)
c                ^
c Unrecognized value for character constant at (^) -- expecting FORMATTED or UNFORMATTED
c bug7.f:6: 
c          CLOSE(10,STATUS='DELETE  ')
c                          ^
c Unrecognized value for character constant at (^) -- expecting DELETE, KEEP, PRINT, PRINT/DELETE, SAVE, SUBMIT, or SUBMIT/DELETE


