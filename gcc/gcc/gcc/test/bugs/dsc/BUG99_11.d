Dear Nick, 
   Here is another set of problems for you, our Bug99_11 to 14, mostly I/O.
Bug99_15 is a set of file programs, 15a to 15e, all failures of INQUIRE
to report state of non-existent files.
The test programs are attached.
Kate
---------------------------------------------------------------------------
running on a RISC PC StrongARM RISCOS 3.7 
 (same result on an A7000+ RISCOS 3.71)
 commands lines to compile and link

g77 -c -fno-backslash f.Bug99_4 
drlink -rescan -o image.%*0 o.%*0 g77lib:OSlibN g77lib:libg2c g77lib:libgcc
g77lib:unixlib

----------------------------------------------------
Bug99_11 7 Oct 99
    Printing INTEGER *2 variables list directed  gets a different result
    from printing the variables with a format I7

Bug99_12 7 Oct 99
    Haxadecimal printing appears to be OK, but not hexadecimal input

Bug99_13 7 Oct 99
    illegal octal input format is accepted without error message or warning

Bug99_14 7 Oct 99
     No error messages or warnings given when integers outside the alloed
     range are stored (for BYTE, INTEGER *2 and INTEGER *4)

Bug99_15 10 Oct 99
    set of 5 bugs in INQUIRE
   a)Wrong replies to INQUIRE asking whether file is direct access
      2 forms of query used, both answers wrong, neither file exists
   b)Two wrong replies to INQUIRE asking whether file is FORMATTED. 
         file does not exist
   c)same as a,b but asking whether file is SEQUENTIAL
   d)same as a,b,c but asking whether file is UNFORMATTED
   e)Illegal unit number and filename not detected

-- 
 __  __  __  __      __ ___   _____________________________________________
|__||__)/ __/  \|\ ||_   |   /...Internet access for all Acorn RISC machines
|  ||  \\__/\__/| \||__  |  /  fortran@argonet.co.uk
___________________________/web site http://www.argonet.co.uk/users/fortran/

