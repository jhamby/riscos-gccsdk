/* os.h (c) Copyright 1996 P.J.Burwood */

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

extern void __seterr(os_error *);	/* flag RiscOS error to UNIX */

#ifdef __cplusplus
	}
#endif

#endif

