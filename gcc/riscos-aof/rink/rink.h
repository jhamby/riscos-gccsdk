
/* ***************************************************/
/* function definitions for the rink run time system */
/* (c) Ben Summers 1995                              */

#include "kernel.h"

typedef void *rink_seghandle;

typedef void *(*_rink_fn) (void);

typedef struct
{
  char id[8];
  int main_version;
  int code_version;
}
rink_check;

typedef struct
{
  int main;
  int code;
}
rink_version;

_kernel_oserror *rink_load (char *segment_name, char *links_name,
			    rink_seghandle * sh, rink_check * ck);
  /* loads segment and it's links and performs necessary linking. Returns 0 if succesful,
     pointer to standard os error block otherwise. Internal errors translated - use
     rinkptr to set the function used.
     If values in segment do not match those in rink_check, an error is returned */


void rink_unload (rink_seghandle sh);
  /* unload code refered to by sh */


_rink_fn rink_fn (rink_seghandle sh, int entry);
  /* returns the address of function referenced by entry in the header */



_rink_fn rink_enum_named (rink_seghandle sh, int *ref, char **name);
  /* enumerates named functions in a segment. for first call, set *ref = 0
     preserve *ref between calls. *ref = -1 when finished.
     *name set to pointer to name of function.
     returns pointer to function  */


rink_version *rink_readversion (rink_seghandle sh);
  /* returns a pointer to a structure giving the version numbers of the segment */
