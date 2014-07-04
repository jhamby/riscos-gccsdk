#include "syscall.h"
#include "sysdep.h"

ATTRIBUTE_HIDDEN void
_dl_exit_internal (int status)
{
  _dl_exit (status);
}

ATTRIBUTE_HIDDEN const os_error *
_dl_canonicalise_filename (char *__filename,
			   char *__buffer,
			   int __buffer_size,
			   int *bytes_free)
{
  /* In */
  register const int fscontrol_canonicalise __asm ("r0") = 37;
  register const char *filename __asm ("r1") = __filename;
  register char *buffer __asm ("r2") = __buffer;
  register int buffer_size __asm ("r5") = __buffer_size;
  /* Out */
  register const os_error *err __asm ("r0");
  register int bytes_free_out __asm ("r5");
  
  asm volatile ("	mov	r3, #0;\n"
		"	mov	r4, #0;\n"
		"	swi	%[XOS_FSControl];\n"
		"	movvc	r0, #0;\n"
		: "=r" (err), "=r" (bytes_free_out)
		: "r" (fscontrol_canonicalise),
		  "r" (filename),
		  "r" (buffer),
		  "r" (buffer_size),
		  [XOS_FSControl] "i" ((1 << 17) | 0x29)
		: "r3", "r4", "cc");
  if (bytes_free)
    *bytes_free = bytes_free_out;
  return err;
}

/* If the filename contains a colon, then it may start with a path variable,
 * if it contains an angle bracket, then it starts with a system variable.
 * We canonicalise the filename in either of these cases, otherwise we assume
 * the filename is already expanded.  */
ATTRIBUTE_HIDDEN char *
_dl_normalise_filename (char *filename)
{
  const os_error *err;
  char *fn_ptr, *result;
  int buffer_size;

  for (fn_ptr = filename;
       *fn_ptr != '\0' && *fn_ptr != ':' && *fn_ptr != '<';
       fn_ptr++)
    /* Empty loop. */;

  /* We didn't find a path or system variable, so no need to process the
   * filename.  */
  if (*fn_ptr == '\0')
    return filename;

  err = _dl_canonicalise_filename (filename, 0, 0, &buffer_size);
  if (err)
    return filename;

  /* The returned buffer_size is negative, the number of bytes needed
   * is (1 - buffer_size).  */
  result = _dl_alloc (1 - buffer_size);
  if (!result)
    return filename;

  err = _dl_canonicalise_filename (filename, result, 1 - buffer_size, 0);
  if (err)
    return filename;

  return result;
}
