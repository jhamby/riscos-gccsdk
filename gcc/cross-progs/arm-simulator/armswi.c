/* SWI driver for the ARM/RISC OS simulation environment.

   (c) Copyright 1998, 1999, 2000 Nick Burrett.  */


#include <ctype.h>
#include <errno.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fnmatch.h>

#include "armhdr.h"
#include "armswis.h"

#define ENV_EXIT 0xffee0001

struct riscos_object_detail
{
  int object_type;
  int load_address;
  int execution_address;
  int object_length;
  int object_attributes;
};

static void terminate_emulator (void);
static void cvt_unix_time (time_t unix_time,
			   unsigned int *high, unsigned int *low);
static time_t cvt_riscos_time (unsigned int high, unsigned int low);
static void cvt_unix_time1 (struct timeval *tv,
			    unsigned int *high, unsigned int *low);
static int set_protection (mode_t mode);
static mode_t get_protection (int attribute);
static struct riscos_object_detail *get_riscos_file_details (const char *name);

struct ticker_event
{
  clock_t call_time;
  WORD call_address;
  WORD r12;
  int delay;
  struct ticker_event *next;
};

#define ENVH_MEMORYLIMIT 0
#define ENVH_UNDEFINED_INSTRUCTION 1
#define ENVH_PREFETCH_ABORT 2
#define ENVH_DATA_ABORT 3
#define ENVH_ADDRESS_EXCEPTION 4
#define ENVH_OTHER_EXCEPTIONS 5
#define ENVH_ERROR 6
#define ENVH_CALLBACK 7
#define ENVH_BREAKPOINT 8
#define ENVH_ESCAPE 9
#define ENVH_EVENT 10
#define ENVH_EXIT 11
#define ENVH_UNUSED_SWI 12
#define ENVH_EXCEPTION_REGISTERS 13
#define ENVH_APPLICATION_SPACE 14
#define ENVH_CAO 15
#define ENVH_UPCALL 16

struct os_state
{
  /* RISC OS environment variables.  */
  volatile WORD environment[18][3];

  /* Keyboard escape key handler.  */
  int escape_state;
  int escape_key;

  /* DDEUtils command line extension emulation.  */
  void *dde_cmd_line;
  int dde_cmd_line_size;

  /* RISC OS file handles.  */
  WORD file_handles[2048];

  /* A singularly linked list of tickers for OS_CallAfter and OS_CallEvery.  */
  struct ticker_event *tickers;

  /* For locale support.  */
  unsigned int territory_table[9][9];
  unsigned char territory_trans[2][256];
} *process;

#define MIN_HANDLE 1500
#define MAX_HANDLE 1540

int free_handle (void)
{
  int x;

  for (x = MIN_HANDLE; x <= MAX_HANDLE; x++)
    {
      if (process->file_handles[x] == 0)
	return x;
    }

  return -1;
}

void init_handles (void)
{
  int x;

  for (x = MIN_HANDLE; x <= MAX_HANDLE; x++)
    process->file_handles[x] = 0;
}

/*****************************************************************************
 copy routines: For use with the PC (to centralize HUGE pointers)
 *****************************************************************************/

void 
blockcopy (void *to, void *from, register int size)
{
  register char *t, *f;

  t = (char *) to;
  f = (char *) from;

  while (size--)
    *t++ = *f++;
}

void 
stringcopy (void *to, void *from)
{
  register char *t, *f;

  t = (char *) to;
  f = (char *) from;

  while (*f && *f != 13)
    *t++ = *f++;
  *t = 0;
}

/*****************************************************************************
 generror: generate an error
 *****************************************************************************/

void 
generror (int x_bit_set, WORD num, char *text)
{
  WORD a;

  if (x_bit_set)
    {
      a = 0x600;

      stringcopy (a + bmem + 4, text);
      wwmem ((0 + a) >> 2, num);

      r15 |= V_BIT;
      r(0) = 0x600;
    }
  else
    {
      if (process->environment[ENVH_ERROR][0])
	{
	  r15 = process->environment[ENVH_ERROR][0];
	  PCwrite = TRUE;
	  a = process->environment[ENVH_ERROR][2];

	  wwmem ((0 + a) >> 2, *arm_regs[r15 & 3][15]);
	  wwmem ((4 + a) >> 2, num);
	  stringcopy (a + bmem + 8, text);
	  r (0) = process->environment[ENVH_ERROR][1];
	  r15 &= ~3; /* Enter in USR mode.  */
	}
    }
}

void c_error (int x_bit, int error_num)
{
  WORD a;

  if (x_bit)
    {
      a = 0x600;
      /* Convert a C error into RISC OS error format.  */
      stringcopy (a + bmem + 4, strerror (error_num));
      wwmem ((0 + a) >> 2, 0x10000 + error_num);
      r15 |= V_BIT;
      r(0) = 0x600;
    }
  else
    {
      if (process->environment[ENVH_ERROR][0])
	{
	  r15 = process->environment[ENVH_ERROR][0];
	  PCwrite = TRUE;
	  a = process->environment[ENVH_ERROR][2];

	  wwmem ((0 + a) >> 2, *arm_regs[r15 & 3][15]);
	  wwmem ((4 + a) >> 2, 0x10000 + error_num);
	  stringcopy (a + bmem + 8, strerror (error_num));
	  r (0) = process->environment[ENVH_ERROR][1];
	}
    }
}

/*****************************************************************************
 vdu: Archimedes VDU driver emulator
 *****************************************************************************/

static BYTE vduq[12];
static int vducount = 0;

void 
doplot (WORD c, WORD x, WORD y)
{
  static WORD x1 = 0, y1 = 0, x2 = 0, y2 = 0;
  int t;

  t = c & 7;
  c &= ~7;

  switch (c)
    {
    case 0:
      if (t == 5)
	line (x2, y2, x, y);
      break;
    case 80:
      trianglefill (x1, y1, x2, y2, x, y);
      break;
    case 96:
      rectanglefill (x1, y1, x2, y2, x, y);
      break;
    case 144:
      circleoutline (x2, y2, x, y);
      break;
    case 152:
      circlefill (x2, y2, x, y);
      break;
    }

  x1 = x2;
  y1 = y2;
  x2 = x;
  y2 = y;
}

void 
vdu (BYTE ch)
{
  int vdulen;

  if (vducount == 0 && ch >= 32)
    {
      writech (ch);
      return;
    }

  vduq[vducount++] = ch;

  vdulen = 1;
  if (vduq[0] == 1)
    vdulen = 2;
  else if (vduq[0] == 17)
    vdulen = 2;
  else if (vduq[0] == 18)
    vdulen = 3;
  else if (vduq[0] == 19)
    vdulen = 6;
  else if (vduq[0] == 22)
    vdulen = 2;
  else if (vduq[0] == 23)
    vdulen = 10;
  else if (vduq[0] == 24)
    vdulen = 9;
  else if (vduq[0] == 25)
    vdulen = 6;
  else if (vduq[0] == 28)
    vdulen = 5;
  else if (vduq[0] == 29)
    vdulen = 5;
  else if (vduq[0] == 31)
    vdulen = 3;

  if (vdulen == vducount)
    {
      vducount = 0;
      switch (vduq[0])
	{
	case 0:		/* Nothing */
	case 1:		/* Send next to printer only */
	case 2:		/* Printer on */
	case 3:		/* Printer off */
	case 4:		/* Split text and graphics cursors */
	case 5:		/* Join  text and graphics cursors */
	case 6:		/* Enable screen output */
	  break;

	case 7:		/* Bell */
	  writech (7);
	  break;
	case 8:		/* Back space */
	  writech (8);
	  break;
	case 9:		/* Horizontal tab */
	  writech (9);
	  break;
	case 10:		/* Line feed */
	  writech (10);
	  break;
	case 11:		/* Vertical tab */
	  writech (11);
	  break;
	case 12:		/* Form feed/Clear screen */
	  writech (12);
	  break;
	case 13:		/* Carriage return */
	  writech (13);
	  break;

	case 14:		/* Page mode on */
	case 15:		/* Page mode off */
	case 16:		/* Clear graphics window */
	case 17:		/* Set text colour */
	case 18:		/* Set graphics colour */
	case 19:		/* Set palette */
	case 20:		/* Restore default colours */
	case 21:		/* Disable screen display */
	  break;

	case 22:		/* Change display mode */
	  mode (vduq[1]);
	  break;

	case 23:		/* Miscellaneous commands */
	case 24:		/* Define graphics window */
	  break;

	case 25:		/* General plot command */
	  doplot (vduq[1], vduq[2] + 256 * vduq[3], vduq[4] + 256 * vduq[5]);
	  break;

	case 26:		/* Restore default windows */
	case 27:		/* No operation */
	  break;

	case 28:		/* Define text window */
	  window (vduq[4], vduq[2]);
	  break;

	case 29:		/* Set graphics origin */
	  break;

	case 30:		/* Home text cursor */
	  gotoxy (0, 0);
	  break;
	case 31:		/* Position text cursor */
	  gotoxy (vduq[1], vduq[2]);
	  break;
	}
    }
}

/*****************************************************************************
 readline: OS_ReadLine
 *****************************************************************************/

void 
readline ()
{
  WORD addr;
  int maxlen, low, high, key, len;

  if (process->escape_state)
    {
      r15 |= C_BIT;
      r (0) = 0x1b;
      return;
    }

  addr = r (0);
  maxlen = r (1);
  low = r (2);
  high = r (3);
  len = 0;

  while (1)
    {
      key = getkey ();

      if (key == process->escape_key)
	{
	  process->escape_state = TRUE;
	  r (1) = len;
	  r15 |= C_BIT;
	  return;
	}

      if (key == 13 || key == 10)
	{
	  wbmem (addr + len, 13);
	  vdu (13);
	  vdu (10);
	  break;
	}

      if (key == 127 || key == 8)
	{
	  if (len > 0)
	    {
	      len--;
	      vdu (8);
	      vdu (32);
	      vdu (8);
	    }
	  continue;
	}

      if (key == 21)
	{
	  while (len > 0)
	    {
	      vdu (8);
	      vdu (32);
	      vdu (8);
	      len--;
	    }
	  continue;
	}

      if (len >= maxlen)
	{
	  vdu (7);
	  continue;
	}

      vdu (key);

      if (key >= low && key <= high)
	{
	  wbmem (addr + len, key);
	  len++;
	}
    }

  r (1) = len;
  r15 &= ~C_BIT;
}

static void
build_territory_table (int code, int (*proc)(int))
{
  int x, pos, y;
  unsigned int t;

  pos = 0;
  x = 0;
  for (pos = 0; pos <= 8; pos++)
    {
      t = 0;
      for (y = 0; y <= 31; y++)
	{
	  t |= ((*proc) (x) ? 1 : 0) << y;
	  x++;
	}
      process->territory_table[code][pos] = t;
    }
}

static void
build_territory_tables (void)
{
  int x;

  build_territory_table (0, iscntrl);
  build_territory_table (1, isupper);
  build_territory_table (2, islower);
  build_territory_table (3, isalpha);
  build_territory_table (4, ispunct);
  build_territory_table (5, isspace);
  build_territory_table (6, isdigit);
  build_territory_table (7, isxdigit);
  for (x = 0; x <= 255; x++)
    {
      process->territory_trans[0][x] = tolower (x);
      process->territory_trans[1][x] = toupper (x);
    }
}

void process_command_line (int x_bit_set, WORD r0)
{
  int length;
  char *temp_buf;
  char *command_name_end, *command_name;
  int command_name_length;
  FILE *handle;
  struct stat file_stat;
  char *temp;

  length = strlen (&bmem[r0]);
  temp_buf = malloc (length + 1);

  /* Remove leading '/' since this as meant for the
     command line interpreter.  */
  if (bmem[r0] == '/')
    stringcopy (temp_buf, bmem + r0 + 1);
  else
    stringcopy (temp_buf, bmem + r0);

  /* Get command name.  */
  command_name_end = strchr (temp_buf, ' ');
  if (command_name_end == NULL)
    /* No arguments passed on the command line.  */
    command_name_length = length;
  else
    command_name_length = command_name_end - temp_buf;

  command_name = malloc (command_name_length + 1);
  strncpy (command_name, temp_buf, command_name_length);
  command_name[command_name_length] = '\0';

  printf ("command_line: '%s'\n", temp_buf);
  printf ("command_name: '%s'\n", command_name);

  /* Load command into 0x8000.  */
  if (stat (command_name, &file_stat) == 0)
    {
      printf ("Loading ...\n");
      handle = fopen (command_name, "r");
      fread (bmem + 0x8000, sizeof (char), file_stat.st_size, handle);
      fclose (handle);

      free (program_command_line);
      program_command_line = temp_buf;
      r15 = 0x8000;
      PCwrite = TRUE;
    }
  else
    {
      free (temp_buf);
      c_error (x_bit_set, errno);
    }

  free (command_name);

}

/*****************************************************************************
 swi: Software interrupt number SWI 1
 *****************************************************************************/

void 
swi (register WORD ins)
{
  /* register WORD num, length, a, b, c, pc; */
  unsigned long num, length, a, b, c, pc;
  char buf[256], buf1[256], *poin;
  FILE *file;
  int x_bit_set;

  num = ins & 0xfdffff;
  pc = (r15 & 0x03fffffc) - 8;

  x_bit_set = ins & 0x20000;

  /* Clear the V bit in the return address.  */
  r15 &= ~(1 << V_SHFT);

  /* Support the CallASWI veneer.  */
  if (num == OS_CallASWI)
    {
      num = r(10) & ~0x20000;
      x_bit_set = r(10) & 0x20000;
    }
  if (num == OS_CallASWIR12)
    {
      num = r(12) & ~0x20000;
      x_bit_set = r(12) & 0x20000;
    }

  if (num >= 256 && num <= 511)	/* OS_WriteI */
    {
      vdu (num - 256);
      return;
    }

  switch (num)
    {
    case OS_WriteC:
      vdu (r (0));
      break;

    case OS_WriteS:
      length = 1;
      r15 = addPC (r15, -4);
      a = r15 & 0x03fffffc;
      while (rbmem (a))
	{
	  vdu (rbmem (a++));
	  length++;
	}
      if (length & 3)
	length += 4 - (length & 3);
      r15 = addPC (r15, length);
      PCwrite = TRUE;
      break;

    case OS_Write0:
      a = r (0);
      while (rbmem (a))
	vdu (rbmem (a++));
      break;

    case OS_NewLine:
      vdu (10);
      vdu (13);
      break;

    case OS_ReadC:
      if (process->escape_state)
	{
	  r15 |= C_BIT;
	  r (0) = 0x1b;
	  break;
	}
      r (0) = getkey ();
      if (r (0) == process->escape_key)
	{
	  process->escape_state = TRUE;
	  r15 |= C_BIT;
	  r (0) = 0x1b;
	}
      else
	r15 &= ~C_BIT;
      break;

    case OS_CLI:
      process_command_line (x_bit_set, r(0));
      break;

    case OS_Byte:
      switch (r (0))
	{
	case 2:
	case 3:
	  break;

	case 124:
	  process->escape_state = FALSE;
	  break;

	case 125:
	  process->escape_state = TRUE;
	  break;

	case 126:
	  if (process->escape_state)
	    {
	      r (1) = 255;
	      process->escape_state = FALSE;
	    }
	  else
	    r (1) = 0;
	  break;

	case 128:
	  r (0) = r (1) = r (2) = 0;
	  break;

	case 129:
	  if (r(1) == 0 && r(2) == 0xff)
	    {
	      /* Read the OS version identifier.  */
	      r(1) = 0xa4; /* RISC OS version 3.11.  */
	      r(2) = 0;
	      break;
	    }

	  if (r(2) < 0x80)
	    {
	      if (process->escape_state)
		{
		  r(1) = 0;
		  r(2) = 0x1b;
		}
	      else
		{
		  r(1) = inkey (r(1), r(2));
		  if (r(1) == -1)
		    r(2) = 255;
		  else
		    r(2) = 0;
		}
	    }
	  else
	    r(1) = r(2) = 0;
	  break;

	case 218:
	  /* Read/write bytes in VDU queue.  */
	  r(1) = 0;
	  break;

	case 220:
	  /* Read/write escape character.  */
	  a = process->escape_key;
	  process->escape_key = (process->escape_key & r(2)) ^ r(1);
	  r(1) = a;
	  break;

	case 229:
	  /* Read/write escape character enable.  */
	  break;

	default:
	  printf ("UNKNOWN OS_BYTE: pc:%08lX a:%ld x:%ld y:%ld\n",
		  pc, r (0), r (1), r (2));
	  break;
	}
      break;

    case OS_Word:
      {
	char *bufptr;

	switch (r(0))
	  {
	  case 1: /* Read system clock.  */
	    {
	      unsigned long cl = (unsigned long) clock ();

	      bufptr = bmem + r(1);
	      bufptr[0] = cl & 0x000000ff;
	      bufptr[1] = (cl & 0x0000ff00) >> 8;
	      bufptr[2] = (cl & 0x00ff0000) >> 16;
	      bufptr[3] = (cl & 0xff000000) >> 24;
	      bufptr[4] = 0;
	    }
	    break;
	  case 14:
	    bufptr = bmem + r(1);
	    if (*bufptr == 3)
	      {
		int high, low;

		/* Read real-time in 5 byte format.  */
		cvt_unix_time (time(0), &high, &low);
		bufptr[0] = low & 0x000000ff;
		bufptr[1] = (low & 0x0000ff00) >> 8;
		bufptr[2] = (low & 0x00ff0000) >> 16;
		bufptr[3] = (low & 0xff000000) >> 24;
		bufptr[4] = high & 0xff;
#ifdef DEBUG
		printf ("OS_Word 14: time = %u, %u\n",
			high, low);
#endif
	      }
	    break;
	default:
	  printf ("Unknown OS_Word: r0 = %d\n", r(0));
	  break;
	}
      }
      break;

    case OS_File:
      switch (r (0))
	{
	case 0:
	case 10:
	  stringcopy (buf, bmem + r (1));
	  poin = buf;
	  while (*poin == ' ')
	    poin++;
	  file = fopen (poin, "wb");
	  if (file == NULL)
	    generror (x_bit_set, 1, "Can't open file for writing");
	  else
	    {
	      fwrite ((char *) bmem + r (4), r (5) - r (4), 1, file);
	      fclose (file);
	    }
	  break;

	case 4: /* Write catalogue information. */
	  {
	    /* R5 = object attributes.  */
	    int status = chmod ((char *) bmem + r(1),
				get_protection (r(5)));
	    if (status == -1)
	      c_error (x_bit_set, errno);
	  }
	  break;

	case 5:
	case 13:
	case 15:
	case 17: /* Read catalogue information.  */
	  {
	    struct stat f;
	    stringcopy (buf, bmem + r(1));
	    printf ("OS_File 5: buf = '%s'\n", buf);
	  
	    if (stat (buf, &f) == -1)
	      r(0) = 0;
	    else
	      {
		int attrib = 0, high, low;

		r(4) = f.st_size;
		if (S_ISDIR (f.st_rdev))
		  r(0) = 2;
		else if (S_ISREG (f.st_rdev))
		  r(0) = 1;
		r(2) = r(3) = 0;
		if (f.st_mode & S_IRUSR)
		  attrib |= 1;
		if (f.st_mode & S_IWUSR)
		  attrib |= (1 << 1);
		if (f.st_mode & S_IRWXG)
		  attrib |= (1 << 2);
		if (f.st_mode & S_IROTH)
		  attrib |= (1 << 4);
		if (f.st_mode & S_IWOTH)
		  attrib |= (1 << 5);
		r(5) = attrib;

		cvt_unix_time (f.st_mtime, &high, &low);
		/* Date stamped file, file type FFF */
		r(2) = (0xfff << 20) | (0xfff << 8) | high;
		r(3) = low;
	      }
	  }
	  break;

	case 6: /* Delete a named object.  */
	  {
	    /* R1 = pointer to non wildcarded filename.  */
	    struct stat f;
	    int status;

	    stringcopy (buf, bmem + r(1));
	  
	    if (stat (buf, &f) == -1)
	      r(0) = 0; /* Directory does not exist.  */
	    else
	      {
		int attrib = 0, high, low;

		r(4) = f.st_size;
		if (S_ISDIR (f.st_rdev))
		  r(0) = 2;
		else if (S_ISREG (f.st_rdev))
		  r(0) = 1;
		r(2) = r(3) = 0;
		if (f.st_mode & S_IRUSR)
		  attrib |= 1;
		if (f.st_mode & S_IWUSR)
		  attrib |= (1 << 1);
		if (f.st_mode & S_IRWXG)
		  attrib |= (1 << 2);
		if (f.st_mode & S_IROTH)
		  attrib |= (1 << 4);
		if (f.st_mode & S_IWOTH)
		  attrib |= (1 << 5);
		r(5) = attrib;

		cvt_unix_time (f.st_mtime, &high, &low);
		/* Date stamped file, file type FFF */
		r(2) = (0xfff << 20) | (0xfff << 8) | high;
		r(3) = low;
	      }

	    if (r(0) == 1)
	      {
		status = remove (bmem + r(1));
		if (status == -1)
		  c_error (x_bit_set, errno);
	      }
	    else if (r(0) == 2)
	      {
		status = rmdir (bmem + r(1));
		if (status == -1)
		  c_error (x_bit_set, errno);
	      }
	  }
	  break;

	case 7:
	case 11:
	  {
	    FILE *handle = fopen (bmem + r(1), "wb");
	    
	    if (handle == NULL)
	      generror (x_bit_set, 2, "Could not create file for writing");
	    else
	      {
		int length = r(5) - r(4);

		while (length >= 0)
		  {
		    fputc (0, handle);
		    length --;
		  }
		
		fclose (handle);
	      }
	  }
	  break;

	case 8: /* Creates a directory.  */
	  {
	    /* R1 = pointer to non-wild-leaf object name.
	       R4 = number of entries (ignored).  */
	    int status = mkdir (bmem + r(1), 0755);

	    if (status == -1)
	      c_error (x_bit_set, errno);
	  }
	  break;

	case 12:
	case 14:
	case 16:
	case 255:
	  stringcopy (buf, bmem + r (1));
	  poin = buf;
	  while (*poin == ' ')
	    poin++;
	  file = fopen (poin, "rb");
	  if (file == NULL)
	    generror (x_bit_set, 2, "Can't open file for reading");
	  else
	    {
	      int attrib = 0, high, low;
	      struct stat f;

	      if ((r (3) & 255))
		r (2) = r(3) = 0x8000;
	      fread ((char *) bmem + r (2), MEMSIZE - r (2), 1, file);
	      fclose (file);

	      stat (buf, &f);

	      r(4) = f.st_size;
	      if (S_ISDIR (f.st_rdev))
		r(0) = 2;
	      else if (S_ISREG (f.st_rdev))
		r(0) = 1;

	      if (f.st_mode & S_IRUSR)
		attrib |= 1;
	      if (f.st_mode & S_IWUSR)
		attrib |= (1 << 1);
	      if (f.st_mode & S_IRWXG)
		attrib |= (1 << 2);
	      if (f.st_mode & S_IROTH)
		attrib |= (1 << 4);
	      if (f.st_mode & S_IWOTH)
		attrib |= (1 << 5);
	      r(5) = attrib;

	      cvt_unix_time (f.st_mtime, &high, &low);
	      /* Date stamped file, file type FFF */
	      r(2) = (0xfff << 20) | (0xfff << 8) | high;
	      r(3) = low;
	    }
	  break;
     
	default:
	  printf ("OS_File: Unimplemented %d\n", r(0));
	  break;
	}

      break;

    case OS_Args:
#ifdef DEBUG
      printf ("OS_Args: r0 = %d, r1 = %x, handle = %x\n",
	      r(0), r(1), process->file_handles[r(1)]);
#endif
      switch (r(0))
	{
	case 0: /* Reads the temporary filing system number, or a
		   file's sequential file pointer.  */
	  if (r(1) == 0)
	    r(0) = 24; /* RISCiXFS  */
	  else
	    {
	      /* Get file sequential pointer.  */
	      r(2) = ftell ((FILE *) process->file_handles[r(1)]);
	    }
	case 1: /* Write an open file's sequential file pointer.  */
	  fseek ((FILE *) process->file_handles[r(1)], r(2), SEEK_SET);
	  break;
	case 2: /* Read an open file's extent.  */
	  {
	    /* Get current file position.  */
	    int temp = ftell ((FILE *) process->file_handles[r(1)]);
	    /* Move to end of file and read position there.  */
	    fseek ((FILE *) process->file_handles[r(1)], 0, SEEK_END);
	    r(2) = ftell ((FILE *) process->file_handles[r(1)]);
	    /* Return to original position.  */
	    fseek ((FILE *) process->file_handles[r(1)], temp, SEEK_SET);
	  }
	  break;
	case 5: /* Reads an open file's end-of-file status.  */
	  if (feof ((FILE *) process->file_handles[r(1)]))
	    r(2) = 1;
	  else
	    r(2) = 0;
	  break;
	case 7: /* Convert a file handle to a canonicalised name.  */
	  {
	    /* R1 = file handle
	       R2 = pointer to buffer to contain canonicalised name
	       R3 = size of buffer.  */
#ifdef P_LINUX
	    int filedes = fileno ((FILE *) process->file_handles[r(1)]);
	    char link[64], buffer[256];
	    int nchars;

	    /* Linux provides a very convenient way of finding the
	       canonicalised name though the use of the /proc filing
	       system.  */
	    sprintf (link, "/proc/self/fd/%d", filedes);
	    nchars = readlink (link, buffer, sizeof (buffer));
	    if (nchars == -1)
	      c_error (x_bit_set, errno);
	    else
	      {
		/* On exit, R5 = number of spare bytes in the buffer including
		   the null terminator.  */
		r(5) -= nchars + 1;
		if (r(5) >= 1)
		  {
		    strncpy (bmem + r(2), buffer, nchars);
		    bmem[r(2) + nchars + 1] = '\0'; /* Null terminate.  */
		  }
	      }
#endif
	  }
	  break;
	case 254: /* Reads information on a file handle.  */
	  if (process->file_handles[r(1)] == 0)
	    {
	      r(0) = 1 << 11;
	    }
	  else
	    {
	      r(0) = 0;
	      if (feof ((FILE *) process->file_handles[r(1)]))
		r(0) |= 1 << 9; /* Already read at EOF.  */
	      r(0) |= 1 << 6; /* Have read access.  */
	      r(0) |= 1 << 7; /* Have write access.  */
	      r(2) = 0; /* Filing system information word.  */
	    }
	  break;
	case 255: /* Ensures data has been written to a file, or all
		     files on the temporary filing system.  */
	  if (r(1) == 0)
	    {
	      int x;

	      for (x = MIN_HANDLE; x <= MAX_HANDLE; x++)
		if (process->file_handles[x] != 0)
		  fflush ((FILE *) process->file_handles[x]);
	    }
	  else
	    {
	      if (process->file_handles[r(1)] != 0)
		fflush ((FILE *) process->file_handles[r(1)]);
	    }
	default:
	  printf ("OS_Args: Unimplemented %d\n", r(0));
	  break;
	}
      break;

    case OS_BGet:
      r(0) = fgetc ((FILE *) process->file_handles[r(1)]);
      if (r(0) == EOF || feof ((FILE *) process->file_handles[r(1)]))
	r15 |= (1 << C_SHFT);
      else
	r15 &= ~(1 << C_SHFT);
      break;

    case OS_BPut:
      if (fputc (r(0) & 0xff, (FILE *) process->file_handles[r(1)]) == -1)
	c_error (x_bit_set, errno);
      break;

    case OS_GBPB:
#ifdef DEBUG
      printf ("OS_GBPB: r0 = %x, r1 = %x, r2 = %x, r3 = %x, r4 = %x\n",
	      r(0), r(1), r(2), r(3), r(4));
#endif
      switch (r(0))
	{
	case 1: /* Write bytes to an open file.  */
	  if (fseek ((FILE *) process->file_handles[r(1)], r(4), SEEK_SET) == -1)
	    {
	      c_error (x_bit_set, errno);
	      break;
	    }
	case 2: /* Write bytes to an open file.  */
	  {
	    int count;

	    count = fwrite (bmem + r(2), sizeof (char),
			    r(3), (FILE *) process->file_handles[r(1)]);
	    r(2) += count;
	    r(3) -= count;
	    r(4) += count;
	    r15 &= ~(1 << C_SHFT);
	  }
	  break;
	case 3: /* Read bytes from an open file.  */
	  if (fseek ((FILE *) process->file_handles[r(1)], r(4), SEEK_SET) == -1)
	    {
	      c_error (x_bit_set, errno);
	      break;
	    }
	case 4: /* Read bytes from an open file.  */
	  {
	    int count;

	    count = fread (bmem + r(2), sizeof (char),
			   r(3), (FILE *) process->file_handles[r(1)]);
	    r(2) += count;
	    r(3) -= count;
	    r(4) += count;
	    if (r(3) == 0)
	      r15 &= ~(1 << C_SHFT);
	    else
	      r15 |= (1 << C_SHFT);
	  }
	  break;

	case 6: /* Read information on a filing system.  */
	  {
	    char *ptr = (char *) bmem + r(2);
	    char buf[256];

	    /* R2 = start address of buffer in memory.  */

	    /* Format is:
	       <zero byte><name length byte><current dir name><priv byte>
	    */
	    if (getcwd (buf, sizeof (buf)) == NULL)
	      c_error (x_bit_set, errno);
	    else
	      {
		ptr[0] = 0; /* Zero byte.  */
		ptr[1] = strlen (buf); /* Name length byte.  */
		ptr = stpcpy (ptr, buf);
		ptr[0] = 0; /* Privilege byte.  */
	      }
	  }
	  break;

	case 9: /* Read entries from a specified directory.  */
	case 10: /* Read entries and file information */
	case 11: /* from a specified directory.  */
	case 12:
	  {
	    /* R1 = pointer to directory name
	       R2 = pointer to buffer
	       R3 = number of object names to read from directory
	       R4 = offset of first item to read in directory
	       R5 = buffer length
	       R6 = pointer to (wildcarded) name to match.  */
	    DIR *dir;

	    dir = opendir (bmem + r(1));
	    if (dir == NULL)
	      c_error (x_bit_set, errno);
	    else
	      {
		struct dirent *entry;
		int offset, objs_read;
		char *bufptr;
		int *recptr;
		struct riscos_object_detail *obj;

		/* Find first entry to read.  */
		for (offset = 0; offset < r(4); offset++)
		  {
		    entry = readdir (dir);
		    if (entry == NULL)
		      break;
		  }

		if (entry == NULL)
		  {
		    /* No more entries.  */
		    r(3) = 0;
		    r(4) = -1;
		    r15 &= ~(1 << C_SHFT); /* Clear C */
		    closedir (dir);
		    break;
		  }

		/* We are now at the starting offset so read the
		   number of entries requested.  */
		objs_read = 0;
		bufptr = (char *) bmem + r(2);
		recptr = (int *) (bmem + r(2));
		for (objs_read = 0; objs_read < r(3); objs_read++)
		  {
		    entry = readdir (dir);
		    if (entry == NULL)
		      break;
		    offset++;

		    /* Compare the filename against the wildcard.
		       Ignore case because RISC OS does as well.  */
		    if (fnmatch ((char *)bmem + r(6),
				 entry->d_name, 0) == 0)
		      {
			/* We have found a match.  Now output the result
			   according to the reason code for OS_GBPB.  */

			if (r(0) == 9)
			  {
			    /* Buffer is filled with a list of null-terminated
			       matched names.  */
			    bufptr = (char *) stpcpy (bufptr, entry->d_name);
			  }
			else if (r(0) == 10)
			  {
			    obj = get_riscos_file_details (entry->d_name);
			    recptr[0] = obj->load_address;
			    recptr[1] = obj->execution_address;
			    recptr[2] = obj->object_length;
			    recptr[3] = obj->object_attributes;
			    recptr[4] = obj->object_type;
			    recptr = (int *) stpcpy ((char *) (&recptr[5]),
					     entry->d_name);
			    recptr = (int *) (((int) recptr + 4) & ~3);
			  }
			else if (r(0) == 11)
			  {
			    obj = get_riscos_file_details (entry->d_name);
			    recptr[0] = obj->load_address;
			    recptr[1] = obj->execution_address;
			    recptr[2] = obj->object_length;
			    recptr[3] = obj->object_attributes;
			    recptr[4] = obj->object_type;
			    recptr[5] = 0;
			    recptr[6] = obj->load_address & 0xff;
			    recptr[7] = obj->execution_address;
			    recptr = (int *) stpcpy (((char *)recptr) + 29,
					     entry->d_name);
			    recptr = (int *) (((int) recptr + 4) & ~3);
			  }
			else if (r(0) == 12)
			  {
			    obj = get_riscos_file_details (entry->d_name);
			    recptr[0] = obj->load_address & 0xff;
			    recptr[1] = obj->execution_address;
			    recptr[2] = obj->object_length;
			    recptr[3] = obj->object_attributes;
			    recptr[4] = obj->object_type;
			    recptr[5] = 0xfff;
			    recptr = (int *) stpcpy ((char *)&recptr[6],
					     entry->d_name);
			    recptr = (int *) (((int) recptr + 4) & ~3);
			  }
		      }
		  }

		r(3) = objs_read;
		/* Set C if r(3) != 0.  */
		if (objs_read)
		  r15 |= (1 << C_SHFT);
		else
		  r15 &= ~(1 << C_SHFT);

		if (entry == NULL)
		  /* No more entries.  */
		  r(4) = -1;
		else
		  r(4) = offset;

		closedir (dir);
	      }
	  }
	  break;
	    
	default:
	  printf ("OS_GBPB: Unimplemented: %d\n", r(0));
	  break;
	}
      break;


    case OS_Find:
      {
	FILE *handle;
	char *attr;
	int riscos_handle;

	if (r(0) >= 0x40)
	  {
	    stringcopy (buf, bmem + r(1));
	    riscos_handle = free_handle ();
#ifdef DEBUG
	    printf ("OS_Find: r0 = %x, r1 = '%s', riscos_handle = %d\n",
		    r(0), buf, riscos_handle);
#endif
	    if (riscos_handle == -1)
	      {
		r(0) = 0;
		break;
	      }

	    if (r(0) & 0x40)
	      attr = "rb";
	    else if ((r(0) & 0x80) || (r(0) & 0xc0))
	      attr = "r+b";

	    if (r(0) & 0x80)
	      {
		struct stat f;
		
		if (stat (buf, &f) == 0
		    && S_ISREG (f.st_rdev))
		  remove (buf);
	      }

	    handle = fopen (buf, attr);
	    if (handle == NULL)
	      generror (x_bit_set, 2, "File not found");
	    else
	      {
		process->file_handles[riscos_handle] = (WORD) handle;
		r(0) = riscos_handle;
#ifdef DEBUG
		printf ("OS_Find: r0 = %d\n", r(0));
#endif
	      }
	  }
	else
	  {
#ifdef DEBUG
	    printf ("OS_Find: r0 = %x, r1 = %x\n", r(0), r(1));
#endif
	    if (r(1) == 0)
	      {
		for (riscos_handle = MIN_HANDLE;
		     riscos_handle <= MAX_HANDLE; riscos_handle++)
		  if (process->file_handles[riscos_handle])
		    {
		      fclose ((FILE *) process->file_handles[riscos_handle]);
		      process->file_handles[riscos_handle] = 0;
		    }
	      }
	    else
	      if (process->file_handles[r(1)])
		{
		  fclose ((FILE *) process->file_handles[r(1)]);
		  process->file_handles[r(1)] = 0;
		}
	  }
      }
      break;

    case OS_ReadLine:
      readline ();
      break;

    case OS_Control:
      {
	WORD error_handler, error_buffer;
	WORD escape_handler, event_handler;

	error_handler = process->environment[ENVH_ERROR][1];
	error_buffer = process->environment[ENVH_ERROR][2];
	escape_handler = process->environment[ENVH_ESCAPE][1];
	event_handler = process->environment[ENVH_EVENT][1];

	if (r(0) != 0)
	  process->environment[ENVH_ERROR][1] = r(0);
	if (r(1) != 0)
	  process->environment[ENVH_ERROR][2] = r(1);
	if (r(2) != 0)
	  process->environment[ENVH_ESCAPE][1] = r(2);
	if (r(3) != 0)
	  process->environment[ENVH_EVENT][1] = r(3);

	r(0) = error_handler;
	r(1) = error_buffer;
	r(2) = escape_handler;
	r(3) = event_handler;
      }
      break;

    case OS_GetEnv:
      r (0) = 0x800;
      stringcopy (bmem + r (0), program_command_line);
      /* Get the memory limit from the MemoryLimit handler.  */
      r (1) = process->environment[ENVH_MEMORYLIMIT][1];
      r (2) = 0x808;
#ifdef DEBUG
      printf ("OS_GetEnv: r0 = %x, r1 = %x, r2 = %x\n",
	      r(0), r(1), r(2));
#endif
      break;

    case OS_Exit:
#ifdef DEBUG
      printf ("OS_Exit: environment[ENVH_EXIT][0] = %x\n",
	      process->environment[ENVH_EXIT][0]);
#endif
      if (process->environment[ENVH_EXIT][0])
	{
	  /* If all user exit handlers have been restored then the main
	     process is now terminating.  Exit the emulator.  */
	  if (process->environment[ENVH_EXIT][0] == ENV_EXIT)
	    terminate_emulator ();

	  if (r (1) == 0x58454241) /* ABEX */
	    {
	      char buffer[16];

	      sprintf (buffer, "%d", r (2));

	      /* Set the return code.  */
	      setenv ("Sys_ReturnCode", buffer, 1);
#ifdef DEBUG
	      printf ("OS_Exit: setting return code %s\n", buffer);
#endif
	    }

	  /* Enter in USR mode */
	  r15 = process->environment[ENVH_EXIT][0] & ~0x3;
	  PCwrite = TRUE;
	  r (12) = process->environment[ENVH_EXIT][1];
	  printf ("OS_Exit: stating execution at 0x%x\n", r15); 
	}
      else
	{
	  terminate_emulator ();
	}
      break;

    case OS_SetEnv:
      {
	WORD a,b,c,d,e,f;

	a = process->environment[ENVH_EXIT][1];
	b = process->environment[ENVH_MEMORYLIMIT][1];
	c = process->environment[ENVH_UNDEFINED_INSTRUCTION][1];
	d = process->environment[ENVH_PREFETCH_ABORT][1];
	e = process->environment[ENVH_DATA_ABORT][1];
	f = process->environment[ENVH_ADDRESS_EXCEPTION][1];

	if (r(0) != 0)
	  process->environment[ENVH_EXIT][1] = r(0);
	if (r(1) != 0)
	  process->environment[ENVH_MEMORYLIMIT][1] = r(1);
	if (r(4) != 0)
	  process->environment[ENVH_UNDEFINED_INSTRUCTION][1] = r(4);
	if (r(5) != 0)
	  process->environment[ENVH_PREFETCH_ABORT][1] = r(5);
	if (r(6) != 0)
	  process->environment[ENVH_DATA_ABORT][1] = r(6);
	if (r(7) != 0)
	  process->environment[ENVH_ADDRESS_EXCEPTION][1] = r(7);

	r(0) = a;
	r(1) = b;
	r(4) = c;
	r(5) = d;
	r(6) = e;
	r(7) = f;
      }
      break;

    case OS_IntOn:
      r15 &= ~(1 << I_SHFT);
      break;

    case OS_IntOff:
      r15 |= (1 << I_SHFT);
      break;

    case OS_CallBack:
      {
	WORD save_block, handler;

	save_block = process->environment[ENVH_CALLBACK][2];
	handler = process->environment[ENVH_CALLBACK][1];

	if (r(0) != 0)
	  process->environment[ENVH_CALLBACK][2] = save_block;
	if (r(1) != 0)
	  process->environment[ENVH_CALLBACK][1] = handler;

	r(0) = save_block;
	r(1) = handler;
      }
      break;

    case OS_EnterOS:
      r15 |= SVC_MODE;
      break;

      /* case OS_BreakPt: */

    case OS_BreakCtrl:
      {
	WORD a;

	a = process->environment[ENVH_BREAKPOINT][1];
	if (r(0) != 0)
	  process->environment[ENVH_BREAKPOINT][1] = r(0);

	r(0) = a;
      }
      break;

    case OS_UnusedSWI:
      {
	WORD a;

	a = process->environment[ENVH_UNUSED_SWI][1];
	if (r(0) != 0)
	  process->environment[ENVH_UNUSED_SWI][1] = r(0);

	r(0) = a;
      }
      break;

    case OS_UpdateMEMC:
      break;

    case OS_ReadUnsigned:
      {
	char *buff, *endptr;
	unsigned long result;

	buff = (char *) (bmem + r (1));
	result = strtoul (buff, &endptr, r (0));
	r (1) = (int) endptr - (int) bmem;
	r (2) = result;
      }
      break;

    case OS_ReadVarVal: /* SWI 0x23 */
      {
	/* R0 = pointer to variable name.
	   R1 = pointer to buffer to hold variable value
	   R2 = maximum length of buffer
	   R3 = context pointer, or 0
	   R4 = 3 if an expanded string is to be converted.  */
	char *var;

	stringcopy (buf, bmem + r(0));
#ifdef DEBUG
	printf ("OS_ReadVarVal: '%s', r1 = %x, r2 = %x\n", buf, r(1), r(2));
#endif
	/* Replace '$' with an underscore.  */
	var = buf;
	while (*var)
	  {
	    if (*var == '$')
	      *var = '_';
	    var++;
	  }
	var = getenv (buf);

	if (var == NULL)
	  {
#ifdef DEBUG
	    printf ("OS_ReadVarVal: '%s' does not exist\n", buf);
#endif
	    r(2) = 0; /* Variable does not exist.  */
	    generror (x_bit_set, 1, "Environment variable does not exist.");
	  }
	else
	  {
	    /* Variable does exist.  */

	    r(4) = 0; /* Variable type is a string.  */
	    if (r(2) & (1 << 31))
	      {
		/* Just return the length it is bitwise NOT the length
		   of the string.  */
		r(2) = ~strlen (var);
	      }
	    else
	      {
		if (r(2) < strlen (var))
		  /* Not enough space for the variable.  */
		  generror (x_bit_set, 1, "Buffer too small");
		else
		  /* Place the environment variable into the buffer
		     pointed to by R1.  */
		  stringcopy (bmem + r(1), var);
	      }
	  }
      }
      break;

    case OS_SetVarVal: /* SWI 0x24 */
      {
	/* R0 = pointer to variable name.
	   R1 = pointer to variable value.
	   R2 = length of value, or negative to delete the variable.
	   R3 = context pointer
	   R4 = variable type.  */
	char *var;

	stringcopy (buf, bmem + r(0));

	/* Replace '$' with an underscore.  */
	var = buf;
	while (*var)
	  {
	    if (*var == '$')
	      *var = '_';
	    var++;
	  }

	if (r(4) == 0 || r(4) == 4)
	  /* Variable type: string or literal string  */
	  stringcopy (buf1, bmem + r(1));
	else if (r(4) == 1)
	  {
	    /* Variable type is a number.  */
	    sprintf (buf1, "%d", wmem[r(1) >> 2]);
	  }
	if (r(2) & (1 << 31))
	  {
	    /* Delete the variable from the environment.  */
	    unsetenv (buf);
	  }
	else
	  {
	    /* Set the environment variable. Create it, if it doesn't
	       exist, update it, if it does exist.  */
	    setenv (buf, buf1, 1);
	  }
      }
      break;

    case OS_BinaryToDecimal:
      {
	WORD a;

	sprintf (buf, "%d", r (0));
	r (0) = r (1);
	a = strlen (buf) + 1;
	if (a > r (2))
	  {
	    a = r (2);
	    generror (x_bit_set, 2, "Buffer overflow");
	  }
	blockcopy (bmem + r (1), buf, a);
	r (2) = a;
      }
      break;

    case OS_FSControl:
      switch (r(0))
	{
	case 0: /* Set the current directory.  */
	  if (chdir (bmem + r(1)) == -1)
	    c_error (x_bit_set, errno);
	  break;
	case 25: /* Rename object */
	  /* R1 = pointer to current pathname.
	     R2 = pointer to new pathname. */
	  if (rename (bmem + r(1), bmem + r(2)) == -1)
	    c_error (x_bit_set, errno);
	  break;
	case 33: /* Converts a filing system number to a name.  */
	  /* R1 = filing system number
	     R2 = pointer to buffer
	     R3 = length of buffer  */
	  strcpy (bmem + r(2), "ADFS");
	  break;
	case 37: /* Converts a pathname to a canonicalised name.  */
	  if (r(2) != 0)
	    {
	      if (r(5) >= strlen (bmem + r(1)))
		strcpy (bmem + r(2), bmem + r(1));
	    }
	  r(5) -= strlen (bmem + r(1));
	  break;
	default:
	  printf ("\nOS_FSControl: unimplemented %d\n", r(0));
	  break;
	}
      break;

    case OS_ReadEscapeState:
      if (process->escape_state)
	r15 |= C_BIT;
      else
	r15 &= ~C_BIT;
      break;

    case OS_GenerateError:
      if (x_bit_set)
	{
	  r15 |= (1 << V_SHFT);
	}
      else
	{
	  if (process->environment[ENVH_ERROR][0])
	    {
	      a = process->environment[ENVH_ERROR][2];
	      /* Set up the error block.  */
	      wwmem ((a + 0) >> 2, *arm_regs[r15 & 3][15]);
	      wwmem ((a + 4) >> 2, rwmem (r (0)));
	      stringcopy (a + bmem + 8, r (0) + bmem + 4);

	      /* Enter a error handler in USR mode.  */
	      r15 = process->environment[ENVH_ERROR][0] & ~3;
	      PCwrite = TRUE;
	      /* Set the workspace pointer.  */
	      r (0) = process->environment[ENVH_ERROR][1];
	    }
	  else
	    {
	      printf ("\nERROR: %08lX %s\n", rwmem (r (0) >> 2),
		      (char *) (r (0) + 4 + (int) bmem));
	      /* terminate_emulator (); */
	    }
	}
      break;

    case OS_ReadVduVariables:
      {
	/* R0 = pointer to input block.
	   R1 = pointer to output block.  */
	int *input, *output;

	input = (int *) (bmem + r (0));
	output = (int *) (bmem + r (1));
	while (*input != -1)
	  {
	    switch (*input)
	      {
	      case 128: /* Left hand column of the graphics window */
		*output++ = 0;
		break;
	      case 129: /* Bottom row of the graphics window */
		*output++ = 0;
		break;
	      case 130: /* Right hand column of the graphics window */
		*output++ = 1280;
		break;
	      case 131: /* Top row of the graphics window */
		*output++ = 1024;
		break;
	      case 132: /* Left hand column of the text window */
		*output++ = 0;
		break;
	      case 133: /* Bottom row of the text window */
		*output++ = 25;
		break;
	      case 134: /* Right hand column of the text window */
		*output++ = 80;
		break;
	      case 135: /* Top row of the text window */
		*output++ = 0;
		break;
	      case 155: /* Text foreground colour.  */
		*output++ = 7;
		break;
	      case 156: /* Text background colour.  */
		*output++ = 0;
		break;
	      default:
		printf ("OS_ReadVduVariables: unsupported code %d\n", *input);
		break;
	      }
	    input++;
	  }
      }
      break;

    case OS_ValidateAddress: /* SWI 0x3a */
      /* r0 = minimum address.
	 r1 = maximum address.  */
      if (r(1) > MEMSIZE || r(0) > MEMSIZE)
	{
	  r15 |= (1 << C_SHFT);
	}
      else
	r15 &= ~(1 << C_SHFT);
      break;

    case OS_CallAfter: /* SWI 0x3b */
      {
	/* R0 = time in centiseconds
	   R1 = address to call
	   R2 = value of R12 to call code with.  */
	struct ticker_event *tick = malloc (sizeof (struct ticker_event));

	tick->call_time = clock() + r(0);
	tick->call_address = r(1);
	tick->r12 = r(2);
	tick->delay = 0;
	tick->next = process->tickers;
	process->tickers = tick;
      }
      break;

    case OS_CallEvery: /* SWI 0x3c */
      {
	/* R0 = delay in centiseconds - 1
	   R1 = address to call
	   R2 = value of R12 to call code with.  */
	struct ticker_event *tick = malloc (sizeof (struct ticker_event));

	tick->call_time = clock() + r(0);
	tick->call_address = r(1);
	tick->r12 = r(2);
	tick->delay = r(0) + 1;
	tick->next = process->tickers;
	process->tickers = tick;
      }
      break;

    case OS_RemoveTickerEvent: /* SWI 0x3d */
      {
	struct ticker_event *tick, *prev;

	prev = NULL;
	tick = process->tickers;
	while (tick)
	  {
	    if (tick->call_address == r(0) && tick->r12 == r(12))
	      {
		/* Found it.  Unlink it.  */
		if (prev == NULL)
		  process->tickers = tick->next;
		else
		  prev->next = tick->next;

		free (tick);
	      }
	    prev = tick;
	    tick = tick->next;
	  }
      }
      break;

    case OS_ChangeEnvironment: /* SWI 0x40 */
      if (r (0) < 17)
	{
#ifdef DEBUG
	  printf ("OS_ChangeEnvironment: handle = %d, new = %x, %x, %x\n",
		  r(0), r(1), r(2), r(3));
#endif
	  a = process->environment[r (0)][0];
	  b = process->environment[r (0)][1];
	  c = process->environment[r (0)][2];
	  if (r(1) != 0)
	    process->environment[r (0)][0] = r (1);
	  if (r(2) != 0)
	    process->environment[r (0)][1] = r (2);
	  if (r(3) != 0)
	    process->environment[r (0)][2] = r (3);
	  r (1) = a;
	  r (2) = b;
	  r (3) = c;
	}
      break;

    case OS_ReadMonotonicTime:
      {
	/* Convert the value returned by clock into centiseconds.  */
	struct timeval tv;

	gettimeofday (&tv, NULL);

#ifdef DEBUG
	printf ("MonotonicTime: sec = %d, usec = %d\n",
		tv.tv_sec, tv.tv_usec);
#endif
	r(0) = (tv.tv_sec * 100) + (tv.tv_usec / 1000);
      }
      break;

    case OS_Plot:
      doplot (r (0), r (1), r (2));
      break;

    case OS_WriteN:
      a = r (0);
      for (length = r (1); length > 0; length--)
	vdu (rbmem (a++));
      break;

    case OS_ReadMemMapInfo:
      r (0) = 4096; /* 4Kbyte pages */
      r (1) = RAM / 4096;
      break;

    case OS_PlatformFeatures:
      if (r (0) == 0)
	r (0) = 0;
      break;

    case OS_SynchroniseCodeAreas:
    case OS_CallASWI:
    case OS_CallASWIR12:
      /* Don't do anything.  */
      break;

    case OS_ConvertHex1:
      sprintf (buf, "%01lX", r (0) & 0xf);
      r (0) = r (1);
      a = strlen (buf) + 1;
      if (a > r (2))
	a = r (2);
      blockcopy (bmem + r (1), buf, a);
      r(1) += a;
      r (2) -= a;
      break;

    case OS_ConvertHex2:
      sprintf (buf, "%02lX", r (0) & 0xff);
      r (0) = r (1);
      a = strlen (buf) + 1;
      if (a > r (2))
	a = r (2);
      blockcopy (bmem + r (1), buf, a);
      r(1) += a;
      r (2) -= a;
      break;

    case OS_ConvertHex4:
      sprintf (buf, "%04lX", r (0) & 0xffff);
      r (0) = r (1);
      a = strlen (buf) + 1;
      if (a > r (2))
	a = r (2);
      blockcopy (bmem + r (1), buf, a);
      r(1) += a;
      r (2) -= a;
      break;

    case OS_ConvertHex6:
      sprintf (buf, "%06lX", r (0) & 0xffffff);
      r (0) = r (1);
      a = strlen (buf) + 1;
      if (a > r (2))
	a = r (2);
      blockcopy (bmem + r (1), buf, a);
      r(1) += a;
      r (2) -= a;
      break;

    case OS_ConvertHex8:
      sprintf (buf, "%08lX", r (0));
      r (0) = r (1);
      a = strlen (buf) + 1;
      if (a > r (2))
	a = r (2);
      blockcopy (bmem + r (1), buf, a);
      r(1) += a;
      r (2) -= a;
      break;

    case OS_ConvertCardinal1:
      sprintf (buf, "%u", r (0) & 0xff);
      r (0) = r (1);
      a = strlen (buf) + 1;
      if (a > r (2))
	a = r (2);
      blockcopy (bmem + r (1), buf, a);
      r(1) += a;
      r (2) -= a;
      break;

    case OS_ConvertCardinal2:
      sprintf (buf, "%u", r (0) & 0xffff);
      r (0) = r (1);
      a = strlen (buf) + 1;
      if (a > r (2))
	a = r (2);
      blockcopy (bmem + r (1), buf, a);
      r(1) += a;
      r (2) -= a;
      break;

    case OS_ConvertCardinal3:
      sprintf (buf, "%u", r (0) & 0xffffff);
      r (0) = r (1);
      a = strlen (buf) + 1;
      if (a > r (2))
	a = r (2);
      blockcopy (bmem + r (1), buf, a);
      r(1) += a;
      r (2) -= a;
      break;

    case OS_ConvertCardinal4:
      sprintf (buf, "%u", r (0));
      r (0) = r (1);
      a = strlen (buf) + 1;
      if (a > r (2))
	a = r (2);
      blockcopy (bmem + r (1), buf, a);
      r(1) += a;
      r (2) -= a;
      break;

    case OS_ConvertInteger1:
      sprintf (buf, "%d", r (0) & 0xff);
      r (0) = r (1);
      a = strlen (buf) + 1;
      if (a > r (2))
	a = r (2);
      blockcopy (bmem + r (1), buf, a);
      r(1) += a;
      r (2) -= a;
      break;

    case OS_ConvertInteger2:
      sprintf (buf, "%d", r (0) & 0xffff);
      r (0) = r (1);
      a = strlen (buf) + 1;
      if (a > r (2))
	a = r (2);
      blockcopy (bmem + r (1), buf, a);
      r(1) += a;
      r (2) -= a;
      break;

    case OS_ConvertInteger3:
      sprintf (buf, "%d", r (0) & 0xffffff);
      r (0) = r (1);
      a = strlen (buf) + 1;
      if (a > r (2))
	a = r (2);
      blockcopy (bmem + r (1), buf, a);
      r(1) += a;
      r (2) -= a;
      break;

    case OS_ConvertInteger4:
      sprintf (buf, "%d", r (0));
      r (0) = r (1);
      a = strlen (buf) + 1;
      if (a > r (2))
	a = r (2);
      blockcopy (bmem + r (1), buf, a);
      r(1) += a;
      r (2) -= a;
      break;

    case Wimp_SlotSize:
      r(0) = RAM - 0x8000;
      break;

    case FPEmulator_Version:
      r (0) = 399;
      break;

    case DDEUtils_Prefix:
      break;

    case DDEUtils_SetCLSize:
      /* In R0 = size, or zero.
         Out - */
      if (process->dde_cmd_line)
	free (process->dde_cmd_line);
      process->dde_cmd_line_size = r (0);
      if (process->dde_cmd_line_size == 0)
	process->dde_cmd_line = NULL;
      else
	process->dde_cmd_line = malloc (process->dde_cmd_line_size);
      break;

    case DDEUtils_SetCL:
      /* In R0 = pointer to control-terminated string.
         Out - */
      blockcopy (process->dde_cmd_line, bmem + r (0),
		 process->dde_cmd_line_size);
      break;

    case DDEUtils_GetCLSize:
      /* In -
         Out R0 = size, or zero.  */
      r (0) = process->dde_cmd_line_size;
      break;

    case DDEUtils_GetCl:
      /* In R0 = pointer to buffer.
         Out - */
      blockcopy (bmem + r (0), process->dde_cmd_line,
		 process->dde_cmd_line_size);
      break;

    case DDEUtils_ThrowbackRegister:
      break;

    case DDEUtils_ThrowbackUnRegister:
      break;

    case DDEUtils_ThrowbackStart:
      break;

    case DDEUtils_ThrowbackSend:
      break;

    case DDEUtils_ThrowbackEnd:
      break;

    case TaskWindow_TaskInfo:
      /* State that we are executing in a Task Window. Not strictly
         true, but it turns off support for the setitimer functions
         that we don't need anyway.  */
      r (0) = 1;
      break;

    case Territory_Number:
      r (0) = 1;
      break;

    case Territory_NameToNumber:
      r (0) = 1;
      break;

    case Territory_ReadCurrentTimeZone:
      {
	struct tm t;
	char *buffer = (char *) (bmem + 0x1000);
	time_t c = time (NULL);

	localtime_r (&c, &t);
	strcpy (buffer, t.tm_zone);
	r (0) = 0x1000;
	r (1) = t.tm_gmtoff * 100;
      }
      break;

    case Territory_ReadTimeZones:
      {
	struct tm t;
	char *buffer = (char *) (bmem + 0x1000);
	time_t c = time (NULL);

	localtime_r (&c, &t);
	strcpy ((char *) (bmem + 0x1000), tzname[0]);
	strcpy ((char *) (bmem + 0x1010), tzname[1]);
	r (0) = 0x1000;
	r (1) = 0x1010;
	r (2) = (t.tm_gmtoff - 3600) * 100;
	r (3) = t.tm_gmtoff * 100;
      }
      break;
      
    case Territory_ConvertTimeToUTCOrdinals:
      {
	time_t unixtime;
	int high, low;
	char *buffer;
	int *ordinals;
	struct tm *t;

	/* Converts a 5-byte UTC time to UTC time ordinals.  */
	buffer = bmem + r(1);
	high = buffer[4];
	low = (buffer[3] << 24) | (buffer[2] << 16) | (buffer[1] << 8)
	  | buffer[0];
	unixtime = cvt_riscos_time (high, low);
	ordinals = (int *) (bmem + r(2));
	t = gmtime (&unixtime);

	ordinals[0] = 0; /* Centiseconds.  */
	ordinals[1] = t->tm_sec; /* Seconds.  */
	ordinals[2] = t->tm_min; /* Minutes.  */
	ordinals[3] = t->tm_hour; /* Hours.  */
	ordinals[4] = t->tm_mday; /* Day number in month.  */
	ordinals[5] = t->tm_mon + 1; /* Month number in year.  */
	ordinals[6] = t->tm_year; /* Year.  */
	ordinals[7] = t->tm_wday; /* Day of week.  */
	ordinals[8] = t->tm_yday; /* Day of year.  */
      }
      break;

    case Territory_ConvertTimeToOrdinals:
      {
	time_t unixtime;
	int high, low;
	char *buffer;
	int *ordinals;
	struct tm *t;

	/* Converts a 5-byte UTC time to local time ordinals.  */
	buffer = bmem + r(1);
	high = buffer[4];
	low = (buffer[3] << 24) | (buffer[2] << 16) | (buffer[1] << 8)
	  | buffer[0];
	unixtime = cvt_riscos_time (high, low);
	ordinals = (int *) (bmem + r(2));
	t = localtime (&unixtime);

	ordinals[0] = 0; /* Centiseconds.  */
	ordinals[1] = t->tm_sec; /* Seconds.  */
	ordinals[2] = t->tm_min; /* Minutes.  */
	ordinals[3] = t->tm_hour; /* Hours.  */
	ordinals[4] = t->tm_mday; /* Day number in month.  */
	ordinals[5] = t->tm_mon + 1; /* Month number in year.  */
	ordinals[6] = t->tm_year; /* Year.  */
	ordinals[7] = t->tm_wday; /* Day of week.  */
	ordinals[8] = t->tm_yday; /* Day of year.  */
      }
      break;

    case Territory_ConvertOrdinalsToTime:
      {
	time_t unixtime;
	int high, low;
	char *buffer;
	int *ordinals;
	struct tm t;

	/* Converts local time ordinals to a 5 byte UTC time.  */
	buffer = bmem + r(1);
	ordinals = (int *) (bmem + r(2));

	t.tm_sec = ordinals[1];
	t.tm_min = ordinals[2];
	t.tm_hour = ordinals[3];
	t.tm_mday = ordinals[4];
	t.tm_mon = ordinals[5] - 1;
	t.tm_year = ordinals[6];
	t.tm_wday = ordinals[7];
	t.tm_yday = ordinals[8];
	unixtime = mktime (&t);
	cvt_unix_time (unixtime, &high, &low);

	buffer[4] = high & 0xff;
	buffer[3] = (low & 0xff000000) >> 24;
	buffer[2] = (low & 0x00ff0000) >> 16;
	buffer[1] = (low & 0x0000ff00) >> 8;
	buffer[0] = low & 0xff;
      }
      break;

    case Territory_CharacterPropertyTable:
      blockcopy (bmem + 0x400,
		 &process->territory_table[r(1)][0], 32);
      r(0) = 0x400;
      break;
    case Territory_LowerCaseTable:
      blockcopy (bmem + 0x400, &process->territory_trans[0][0], 256);
      r(0) = 0x400;
      break;
    case Territory_UpperCaseTable:
      blockcopy (bmem + 0x400, &process->territory_trans[1][0], 256);
      r(0) = 0x400;
      break;

    case Territory_Collate:
      r(0) = strcoll ((char *) (bmem + r(1)), (char *) (bmem + r(2)));
      r15 &= ~0xfc000000;
      if (r(0) < 0)
	r15 |= (1 << N_SHFT);
      else if (r(0) == 0)
	r15 |= (1 << Z_SHFT);
      else if (r(0) > 0)
	r15 |= (1 << C_SHFT);
      break;

    default:
      printf ("\nUNDEFINED SWI &%08lX AT ADDRESS &%08lX\n", num,
      /*ins & 0xffffff, */ pc);
      r15 |= (1 << V_SHFT);
      break;
    }
}

static void terminate_emulator (void)
{
#ifdef DEBUG
  extern int total_instructions;
  extern clock_t execution_start;
  clock_t total_time = clock () - execution_start;
  printf ("\nOS_EXIT with no handler - Halt!\n");
  printf ("Total instructions = %d\n", total_instructions);
  printf ("Total execution time = %d csecs\n",
	  total_time / 10000);
  printf ("Instructions per second = %f\n",
	  (double)total_instructions / ((double)total_time / 1000000));
#endif
  exit (0);
}

/*****************************************************************************
 signal_handler: to catch the interrupt
 *****************************************************************************/

void 
signal_handler (int type)
{
  if (type == SIGINT)
    {
      IRQint = INT_ESCAPE_PRESSED;
      signal (SIGINT, *signal_handler);
    }
}

/*****************************************************************************
 initswi: initialise swi driver
 *****************************************************************************/

void 
initswi ()
{
  int l;

  process = malloc (sizeof (struct os_state));
  memset (process, 0, sizeof (struct os_state));

  process->escape_state = FALSE;
  process->escape_key = ESCAPE_KEY;

  signal (SIGINT, *signal_handler);

  /* Initialise MemoryLimit handler. Allow 32K for the SVC stack.  */
  process->environment[ENVH_MEMORYLIMIT][1] = RAM - 0x8000;
  /* Application space handler.  */
  process->environment[ENVH_APPLICATION_SPACE][1] = RAM - 0x8000;

  /* Initialise Exit handler.  */
  process->environment[ENVH_EXIT][0] = ENV_EXIT;

  build_territory_tables ();
  init_handles ();
}

/* Time conversion functions.  */

/* Unix time (time_t) represents the number of seconds elapsed since
   00:00:00 on January 1, 1970 Coordinated Universal Time.

   RISC OS time is a 5 byte number representing the number of
   centiseconds that have elapsed since 00:00:00 on January 1, 1900 CUT.

   The number of centiseconds that have elapsed between the starts
   of RISC OS and Unix times is 0x336e996a00.  */

/* Convert RISC OS format 5 byte time into Unix format time.  */

static time_t cvt_riscos_time (unsigned int high, unsigned int low)
{
  unsigned int tc;

  high = high & 0xff;

  /* Firstly, subtract 0x336e996a00 centiseconds from the RISC OS time.  */
  tc = 0x6e996a00U;
  if (low < tc) /* check for a carry.  */
    high--;
  low -= tc;

  /* Subtract 0x33 from the fifth byte. We assume that high > 0x33.  */
  high -= 0x33;

  /* Remove the centiseconds from the time.  */
  low = (low / 100) + (high * 42949673U); /* 0x1000000000 / 100 = 42949672.96 */
  low -= (high / 25); /* compensate for 0.04 error.  */

  /* Sorted.  */
  return low;
}

/* Convert Unix time into RISC OS five byte time.  */
static void cvt_unix_time (time_t unix_time,
			   unsigned int *high, unsigned int *low)
{
  unsigned long ra,rb,rc,rd,re;

  /* Multiply unix_time by 100.  */
  ra = unix_time;
  rb = 100; /* ls 16 bits of rb. */

  rd = ra >> 16; /* ms 16 bits of ra.  */
  ra = (ra << 16) >> 16;
  /* ra &= ~(rd << 16);*/ /* ls 16 bits of ra.  */

  rc = ra * rb; /* low partial product */
  rb = rd * rb; /* middle partial product */

  /* add middle partial product. look out for carry. */
  rd = rb >> 16;
  re = rc;
  rc += rb << 16;
  if (rc < re)
    rd += 1; /* carry occurred.  */

  /* Add the csecs from 1900 to 1970. */
/*  rd += 0x33; */
  /* look out for a carry.  */
  re = rc;
  rc += 0x6e996a00U;
  rd += (rc < re) ? 0x34 : 0x33;
#if 0
  if (rc < re)
    rd += 1; /* carry occurred */
#endif

  *high = rd & 0xff;
  *low = rc;
}

/* Convert Unix time into RISC OS five byte time.  */
static void cvt_unix_time1 (struct timeval *tv,
			    unsigned int *high, unsigned int *low)
{
  unsigned long long ra;

  ra = tv->tv_sec * 100;
  ra += tv->tv_usec / 100;

  /* Add the csecs from 1900 to 1970.  */
  ra += 0x6e996a00U;

  *low = (unsigned int) (ra & 0x00000000ffffffffULL);
  *high = (unsigned int) (ra & 0x000000ff00000000ULL);
#if 0

  unsigned long ra,rb,rc,rd,re;

  /* Multiply unix_time by 100.  */
  ra = tv->tv_sec;
  rb = 100; /* ls 16 bits of rb. */

  rd = ra >> 16; /* ms 16 bits of ra.  */
  ra = (ra << 16) >> 16;
  /* ra &= ~(rd << 16);*/ /* ls 16 bits of ra.  */

  rc = ra * rb; /* low partial product */
  rb = rd * rb; /* middle partial product */

  /* add middle partial product. look out for carry. */
  rd = rb >> 16;
  re = rc;
  rc += rb << 16;
  if (rc < re)
    rd += 1; /* carry occurred.  */

  re = rc;
  rc += tv->tv_usec / 100;
  if (rc < re)
    rd += 1; /* carry occurred.  */

  /* Add the csecs from 1900 to 1970. */
/*  rd += 0x33; */
  /* look out for a carry.  */
  re = rc;
  rc += 0x6e996a00U;
  rd += (rc < re) ? 0x34 : 0x33;
#if 0
  if (rc < re)
    rd += 1; /* carry occurred */
#endif

  *high = rd & 0xff;
  *low = rc;
#endif
}

/* Convert a file mode into a attribute mask suitable for
   passing to RISC OS.  */
static int set_protection (mode_t mode)
{
  int attribute = 0;

  if (mode & S_IREAD) /* Owner read access.  */
    attribute |= (1 << 0);
  if (mode & S_IWRITE) /* Owner write access.  */
    attribute |= (1 << 1);
  if (mode & S_IROTH) /* Public read access.  */
    attribute |= (1 << 4);
  if (mode & S_IWOTH) /* Public write access.  */
    attribute |= (1 << 5);

  return attribute;
}

/* Convert a RISC OS attribute mask into a 'stat' compatible
   file access mode.  */
static mode_t get_protection (int attribute)
{
  mode_t mode = 0;

  /* Do the file attributes.  */
  if (attribute & (1 << 0)) /* Owner read access */
    mode |= S_IREAD;
  if (attribute & (1 << 1)) /* Owner write access */
    mode |= S_IWRITE;

  if (attribute & (1 << 4)) /* Public read access */
    mode |= S_IROTH;
  if (attribute & (1 << 5)) /* Public write access */
    mode |= S_IWOTH;

  return mode;
}

static struct riscos_object_detail *get_riscos_file_details (const char *name)
{
  static struct riscos_object_detail obj;
  int attrib, high, low;
  struct stat f;

  if (stat (name, &f) == -1)
    {
      /* File/directory does not exist. */
      obj.object_type = 0;
      return &obj;
    }

  obj.object_length = f.st_size;
  if (S_ISDIR (f.st_rdev))
    obj.object_type = 2; /* Object is a directory.  */
  else if (S_ISREG (f.st_rdev))
    obj.object_type = 1; /* Object is a file.  */

  /* Get object attributes.  */
  if (f.st_mode & S_IRUSR)
    attrib |= 1;
  if (f.st_mode & S_IWUSR)
    attrib |= (1 << 1);
  if (f.st_mode & S_IRWXG)
    attrib |= (1 << 2);
  if (f.st_mode & S_IROTH)
    attrib |= (1 << 4);
  if (f.st_mode & S_IWOTH)
    attrib |= (1 << 5);
  obj.object_attributes = attrib;

  cvt_unix_time (f.st_mtime, &high, &low);
  /* Date stamped file, file type FFF */
  obj.load_address = (0xfff << 20) | (0xfff << 8) | high;
  obj.execution_address = low;

  return &obj;
}
