/* riscos/viwwer.c
 *
 * Launch just created draw or sprite block.
 *
 * In this version just *Filer_Run the file.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>
/* Use unixlib.  This could be changed to kernel.h, swix.h
   or oslib. */
#ifndef __GNUC__
#include <sys/os.h>
#include <sys/swis.h>
#else
#define __UNIXLIB_INTERNALS
#include <oslib/os.h>
#undef __UNIXLIB_INTERNALS
#ifndef os_file
#define os_file(a,b,c) __os_file(a,b,c)
#endif
#endif
/* But this could not... */
#include <unixlib/local.h> /* for _riscosify() */

/* Some definitions from plot.h (UnixLib's and Acorn/Norcroft's os_error
   conflicts with Gnuplot's one */

/*char *gp_alloc (unsigned long size, char *message);
void *gp_realloc(void *p, unsigned long size, char *message); */
void int_error(char str[], int t_num);
#define NO_CARET (-1)


/* If viewer failed before, don't throw another int_error or gnuplot will
   not be able to exit. */
static int previous_error = 0;

#if 0
static void start_from_taskwindow(char *);
static void start_as_task(char *);
#endif /* 0 */

void RISCOS_viewer (unsigned char * block, size_t size, int type)
{
/*  const char startcmd[] = "WimpTask Filer_Run ";*/
  const char startcmd[] = "Filer_Run ";
  char tempcmd [PATH_MAX + sizeof(startcmd) + 1];
  char un_tempname [L_tmpnam];
  int regs[10];
/*  int oerr, whandle; */
  int dummy = type;
#if 1
  time_t now;
#endif

  strcpy(tempcmd, startcmd);

  /* We need to riscosify the name ourselves. */
  tmpnam(un_tempname); /* something line /tmp/__1234ab */
#ifdef RISCOS_BB_DEBUG
  fprintf(stderr, "init tmpnam `%s\'\n", un_tempname);
#endif
  if (__riscosify_std(un_tempname, 1, tempcmd + sizeof(startcmd) - 1,
       sizeof(tempcmd) - sizeof(startcmd), &dummy)) {
    /* We have a valid temp file name, now it is something like
       <Wimp$ScrapDir>.__1234ab */
#ifdef RISCOS_BB_DEBUG
    fprintf(stderr, "riscosified: `%s\'\n", tempcmd + sizeof(startcmd) - 1);
#endif
    memset(regs, 0, sizeof(regs));
    regs[2] = type;
    switch (type) {
      case 0xaff:
        regs[4] = (int) block;
        regs[5] = (int) block + size;
        break;
      case 0xff9:
        regs[4] = (int) block + 4;
        regs[5] = (int) block + size;
        break;
      default:
        if (!previous_error) {
          previous_error = 1;
          int_error("RISC OS viewer: unknown file type", NO_CARET);
        }
        else {
          fprintf(stderr, "RISC OS viewer: unknown file type\n");
          return;
        }
        break;
    }
    if (!xosfile_save_stamped (tempcmd + sizeof(startcmd) - 1, regs[2], regs[4], regs[5])) {
/*    if (!os_file (10, tempcmd + sizeof(startcmd) - 1, regs)) {  */
      /* Created the temp file.
         Find out whether we're in a taskwindow or not. */

#if 0
      /* Without this, interaction between taskwindow
         and unixlib sometimes produces a bad display
         in the taskwindow. */
      fputc('\r',stderr); fflush(stderr);
#endif
#if 0
      memset(regs, 0, sizeof(regs));
      regs[0] = 5;
      os_swi(XOS_Bit | Wimp_ReadSysInfo, regs);
      whandle = regs[0];
      memset(regs, 0, sizeof(regs));
      regs[0] = (int) tempcmd;
      regs[1] = whandle;
      os_swi(XOS_Bit | Wimp_StartTask, regs);

#if 0
      os_swi(XOS_Bit | TaskWindow_TaskInfo, regs);
      if (regs[0])
        start_from_taskwindow(tempcmd);
      else
        start_as_task(tempcmd);
#endif
#else
      system(tempcmd);
#endif

      /* Wait a sec (or two)... */
#if 1
      /* sleep sleeps a bit too good... */
      now = time(NULL);
      while (time(NULL) < now + 2) ;
#else
      sleep(2);
#endif

      /* task got it (or not) */
      remove(tempcmd + sizeof(startcmd) - 1);
#ifdef RISCOS_BB_DEBUG
      fprintf(stderr, "removed `%s\'\n", tempcmd + sizeof(startcmd) - 1);
#endif

      return;
    }
  }

  /* If we get here, an error occurred. */
  if (!previous_error) {
    previous_error = 1;
    int_error("RISC OS viewer: cannot create temp file to send to viewer",
              NO_CARET);
  }
  else {
    fprintf(stderr,
            "RISC OS viewer: cannot create temp file to send to viewer\n");
    return;
  }
}


#if 0
/* When in a taskwindow, we're an implicit wimptask so it is not possible
   to become (aonther) one to use Wimp_StartTask. */

static void start_from_taskwindow(char * cmd) {
  int regs[10];
  _kernel_oserror * oe;

#ifdef RISCOS_BB_DEBUG
  fprintf(stderr, "cmd: `%s\'\n", cmd);
#endif
  memset(regs, 0, sizeof(regs));
  regs[0] = (int) cmd;

  oe = os_swi(XOS_Bit | Wimp_StartTask, regs);
  if (oe) {
    fprintf(stderr, "startask error %05x: `%s'\n",
                    *(int*) oe, (char*)(oe+4));
  }
}


static void start_as_task(char * cmd) {
  /* Become a task and launch file. */
  int regs[9];
  int wimphandle;
  _kernel_oserror * oe;

  memset(regs, 0, sizeof(regs));
  regs[0] = 200;
  regs[1] = 0x4B534154;
  oe = os_swi(XOS_Bit | Wimp_Initialise, regs);
  if (oe) fprintf(stderr, "wimp init error %05x %s",
                          *(int*)oe, (char*)(oe + 4));
  if (!oe) {
#ifdef RISCOS_BB_DEBUG
    fprintf(stderr, "wimp inited\n");
#endif
    wimphandle = regs[1];

    memset(regs, 0, sizeof(regs));
    regs[0] = (int) cmd;
    regs[1] = wimphandle;
#ifdef RISCOS_BB_DEBUG
    fprintf(stderr, "starting task `%s\'\n", cmd);
#endif
    oe = os_swi(XOS_Bit | Wimp_StartTask, regs);
    if (oe) fprintf(stderr, "starttask error %05x %s\n",
                            *(int*)oe, (char*)(oe + 4));
    memset(regs, 0, sizeof(regs));
    regs[0] = wimphandle;
    regs[1] = 0x4B534154;
    os_swi(XOS_Bit | Wimp_CloseDown, regs);
  }
}
#endif /* 0 */
