/*
The following code reliably (on my RISC OS 3.7 StrongARM RiscPC 64+2Mb RAM)
crashes gcc 2.96 when compiled with 'gcc -O -o testcomp -c test.c'
but when not optimized 'gcc -o testcomp -c test.c' the compilation succedes
(where this file is test.c)

  gcc -v gives
    Using builtin specs.
    gcc version 2.96 19990713 (experimental)

Error msg follows:
******************
Fatal signal received: Segmentation fault
A core dump will now follow ...
stack backtrace:

 pc:    b5b0c sp:   3df47c __unixlib_internal_post_signal()
 pc:    b9044 sp:   3df48c raise()
 pc:  1a171fc sp:   3df4dc
Invalid pc address  1a171fc


Register dump:

 a1:        0 a2:   1f44a4 a3:   1f64c8 a4:   1f64cc
 v1:        0 v2:        0 v3:   1d6444 v4: 1da16738
 v5: 1da2adb8 v6:        0 sl:   3df044 fp:   3df50c
 ip:   3df4a8 sp:   3df4dc lr: 1da171f8 pc: 11a171f8


 pc:    befe0 sp:   3df510 rest_of_compilation()
 pc:   18f5e0 sp:   3df534 finish_function()
 pc:    c9670 sp:   3dfa5c yyparse()
 pc:    bda38 sp:   3dfa94 compile_file()
 pc:    c18c4 sp:   3dfadc main()
 pc:   13e804 sp:   3dfaf8 _main()

gcc: Internal compiler error: program cc1 got fatal signal 75
******************
*/

typedef unsigned long ulong;
typedef ulong gx_color_index_data;
typedef gx_color_index_data gx_color_index;

static int
mask_clip_copy_mono( gx_color_index color0, gx_color_index color1 )
{
    gx_color_index color, mcolor0, mcolor1;

    int code;

    do {
	  if ( color1 != ((unsigned long)(-1)) ) {
	    if ( color0 != ((unsigned long)(-1)) ) {
/*	      code = (*((dev)->procs.fill_rectangle))(dev, x, y, w, h, color=
0);
	      if ( code < 0 )
	        return code; */
	    }
/*	    color = color1;
	    mcolor0 = 0;
	    mcolor1 = ((unsigned long)(-1)); */
	  }
	  else {
        return( 0 );
      }
    } while( 0 );

    return 0;
}

