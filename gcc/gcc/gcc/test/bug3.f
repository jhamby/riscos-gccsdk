      program  run2
      
      real a,b,c,d,e,f
      integer i
      
      open(unit=1,status='scratch')
      
      a=1.0
      b=2.0
      c=3.0
      
      do i=1,5
C      rewind 1
      write(1,99) a,b,c
C      rewind 1
      read(1,99)d,e,f
      rewind 1
      enddo
      
      stop

  99  FORMAT(A)

      end
