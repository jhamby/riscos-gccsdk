/* sys/os.h extension header for the RISC OS SharedCLibrary.
   Copyright (c) 1996 P.J. Burwood
   Copyright (c) 1997-2005 Nick Burrett
   All rights reserved.
 
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
   1. Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
   3. The name of the author may not be used to endorse or promote products
      derived from this software without specific prior written permission.
 
   THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
   IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
   OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
   IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
   NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
   THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

#ifndef __SYS_OS_H
#define __SYS_OS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
  {
  unsigned int	err;
  char		mess[252];
  } os_error;

#define os_prhex __os_prhex
extern os_error	*os_prhex (int); 	/* %8x format hex output */
#define os_print __os_print
extern os_error	*os_print (char *);

#define os_file __os_file
extern os_error	*os_file (int, const char *,int * /* 6 reg */ );

/* OS_Find */

#define os_fopen __os_fopen
extern os_error	*os_fopen (int, const char *,int * /* 1 reg */ );
#define os_fclose __os_fclose
extern os_error	*os_fclose (int);

/* OS_GBPB */

#define os_fread __os_fread
extern os_error	*os_fread (int,void *,int,int * /* 5 reg */ );
#define os_fwrite __os_fwrite
extern os_error	*os_fwrite (int,void *,int,int * /* 5 reg */ );
#define os_gbpb567 __os_gbpb567
extern os_error	*os_gbpb567 (int,void *);

#define os_args __os_args
extern os_error	*os_args (int,int,int,int * /* 3 reg */ );

#define os_fsctrl __os_fsctrl
extern os_error	*os_fsctrl (int,char *,char *,int);

extern void __seterr(os_error *);	/* flag RISC OS error to UNIX */

#ifdef __cplusplus
	}
#endif

#endif

