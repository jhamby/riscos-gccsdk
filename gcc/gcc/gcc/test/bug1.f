      program braindeadfortran

      print *,'In main 4=', 4
      call modify(4)
      print *,'In main 4=', 4
      call modify(4)
      print *,'In main 4=', 4
      end

      subroutine modify(number)
      integer number

      print *,'On entry to modify number=', number
      number=number-1
      print *,'On exit of modify number=', number
      return
      end
