
In message <internews46EC26147A@argonet.co.uk> Julian Pickering wrote:

> ie three different interpretations of the same number by different BASIC
> functions.

That's not as good as the infamous FORTRAN feature which allowed you to
alter the values of numeric constants like `4'. For example :

*type f.bdf
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
*f2c f.bdf
f.bdf:
   MAIN braindeadfortran:
   modify:
*cc -o bdf bdf.c -IF: F:o.f2c_lib
[...]
*bdf
 In main 4= 4
 On entry to modify number= 4
 On exit of modify number= 3
 In main 4= 3
 On entry to modify number= 3
 On exit of modify number= 2
 In main 4= 2

From memory, running the same program on SunOS gave :

 In main 4= 4
 On entry to modify number= 4
 On exit of modify number= 3
 In main 4= 4
 On entry to modify number= 3
 On exit of modify number= 2
 In main 4= 4

On Solaris, the program crashes with a segmentation violation and an
error to the effect of 'POSSIBLE ATTEMPT TO MODIFY CONSTANT' (yes, it
shouts at you).

Compared to this, BASIC's minor indiscretions are forgivable.

        - Steven

--- EchoMaker 0.43
 * Origin: DigiLink Usenet News Gateway (2:257/501.9)

