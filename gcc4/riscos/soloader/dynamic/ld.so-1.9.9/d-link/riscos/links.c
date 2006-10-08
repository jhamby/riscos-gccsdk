#include <kernel.h>
#include <sys/param.h>
#include <stdio.h>
#include "syscall.h"
#include "string.h"
#include "hash.h"

#define	LINK_FILETYPE	0x1C8

#define MAX_LINKS	64

#if 0
static _kernel_oserror *read_link(const char *file, int *regs)
{
  regs[0] = 23;
  regs[1] = (int)file;
  return (_kernel_swi(0x8, (_kernel_swi_regs *)regs, (_kernel_swi_regs *)regs));
}
#endif

#define READ_LINK(err, filename, type, status)	\
  asm volatile ("mov	a1, #23;\n\t"		\
		"mov	a2, %3;\n\t"		\
		"swi	0x20008;\n\t"		\
		"movvs	%0, r0;\n\t"		\
		"movvc	%0, #0;\n\t"		\
		"movvc	%1, r6;\n\t"		\
		"movvc	%2, r0;\n\t"		\
		: "=r" (err), "=r" (type), "=r" (status)	\
		: "r" (filename)				\
		: "a1", "a2", "a3", "a4", "v1", "v2", "v3")

static inline _kernel_oserror *__os_open(char *file, int *fd)
{
_kernel_oserror *err;

  asm volatile ("mov	a1, #0x4F;\n\t"
		"mov	a2, %2;\n\t"
		"swi	0x2000d;\n\t"
		"movvs	%0, r0;\n\t"
		"movvc	%0, #0;\n\t"
		"teq	r0, #0;\n\t"
		"moveq	r0, #-1;\n\t"
		"str	r0, [%1, #0]\n\t"
		: "=r" (err)
		: "r" (fd), "r" (file)
		: "a1", "a2", "cc", "memory");
  return err;
}

/*
  Assume input filename is RISC OS format
*/
int resolve_links(const char *file_in, char *file_out, int file_out_len)
{
int link_count = 0;
int fd = -1, size, type, status;
_kernel_oserror *err;
char buffer[MAXPATHLEN];

  if (_dl_strlen(file_in) + 1 > file_out_len)
    return -1;//__set_errno(EINVAL);

  _dl_strcpy(file_out, file_in);

  READ_LINK(err, file_out, type, status);
  if (err)
    goto os_err;

  while (status && type == LINK_FILETYPE)
  {
    if (link_count >= MAX_LINKS)
      return -1;//__set_errno(EMLINK);

    if ((err = __os_open(file_out, &fd)))
      goto os_err;

    if (_dl_read(fd, buffer, 4) < 0)
      goto err;

    if (buffer[0] != 'L' || buffer[1] != 'I' || buffer[2] != 'N' || buffer[3] != 'K')
    {
      _dl_close(fd);
      fd = -1;
      break;
    }

    if (_dl_read(fd, (void *)&size, 4) < 0)
      goto err;

    if (_dl_read(fd, buffer, size) < 0)
      goto os_err;

    _dl_close(fd);
    fd = -1;

    buffer[size] = '\0';

    if (_dl_strchr(buffer, ':') != 0)
    {
      if (_dl_strlen(buffer) + 1 > file_out_len)
        return -1;//__set_errno(EINVAL);

      _dl_strcpy(file_out, buffer);
    }
    else
    {
    char *sep;

      /* Strip the link leaf name */
      if ((sep = _dl_strrchr(file_out, '.')) != NULL)
        *(sep + 1)= '\0';
      else if ((sep = _dl_strrchr(file_out, ':')) != NULL)
        *(sep + 1) = '\0';
      else
        *file_out = '\0';

      if (_dl_strlen(file_out) + _dl_strlen(buffer) + 2 > file_out_len)
        return -1;//__set_errno(EINVAL);

      _dl_strcat(file_out, buffer);
    }

    READ_LINK(err, file_out, type, status);
    if (err)
      goto os_err;

    link_count++;
  }

  return 0;

os_err:
  if (fd >= 0)
    _dl_close(fd);
  _dl_fdprintf(2,"%s\n",err->errmess);
//  errno = EOPSYS;
  return -1;

err:
  if (fd >= 0)
    _dl_close(fd);
  return -1;
}

asm(".hidden resolve_links");

#if TEST

static const char in[] = "ADFS::Fireball.$.ProgDev.gccsdk.symlinks.ld-riscos/1/so";

int main(void)
{
char out[MAXPATHLEN];
int res;

  res = resolve_links(in, out, MAXPATHLEN);
  if (res < 0)
    printf("Error occurred (%d)\n",errno);
  else
    printf("%s\n",out);
  return 0;
}

#endif
