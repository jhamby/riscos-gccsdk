#include <stdio.h>
#include <fcntl.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/os.h>

int open (const char *filename, int oflag, ...)
{
  va_list ap;
  int mode;
  FILE *fp;
  char cmode[4];
  int regs[6];
  os_error *err;

  /* mode is actually ignored */
  if (oflag & O_CREAT)
    {
      va_start (ap, oflag);
      mode = va_arg (ap, int);
      va_end (ap);
    }
  else
    mode = 0777;

  filename = __uname (filename, oflag);

  if ((err = os_file (0x05, filename, regs)) != NULL)
    {
      __seterr (err);
      return -1;
    }

  if (regs[0])
    {
      /* if the file already exists and exclusive access requested, then error */
      if ((oflag & (O_EXCL | O_CREAT)) == (O_EXCL | O_CREAT))
        {
          errno = EEXIST;
          return -1;
        }
      /* directory ? */
      if (regs[0] == 2)
        {
	  errno = EISDIR;
	  return -1;
        }
    }

  cmode[1] = '\0';
  cmode[2] = '\0';
  cmode[3] = '\0';
  switch (oflag)
    {
      case O_RDONLY:
	cmode[0] = 'r';      
        break;
      case O_WRONLY:
        if (oflag & O_APPEND)
          cmode[1] = 'a';
        else
          cmode[1] = 'w';
        break;
      case O_RDWR:
	cmode[1] = '+';
        if (oflag & O_APPEND)
          cmode[0] = 'a';
        else if (oflag & O_TRUNC)
          cmode[0] = 'w';
        else
          cmode[0] = 'r';
        break;
      default:
        errno = EINVAL;
        return -1;
    }

  if (oflag & O_BINARY)
    strcat (cmode, "b");

  fp = fopen (filename, cmode);
  if (fp == NULL)
    {
      __seterr ((os_error*)_kernel_last_oserror());
      return -1;
    }
  return fileno(fp);
}

