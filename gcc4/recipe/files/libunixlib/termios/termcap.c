/* tgetent (), tgetnum (), tgetflag (), tgetstr(), tgoto (), tputs ()
 * Copyright (c) 2000-2010 UnixLib Developers
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <termcap.h>

#include <pthread.h>
#include <internal/unix.h>

#define T_BUILTIN		/* builtin termcap */

static char *t_bname = "acorn0";
static unsigned char *t_btenv = (unsigned char *)
  "av|acorn0|Acorn VDU Driver Mode 0:"
  ":li#32:co#80:am:cl=^L:bs:cm=^_%r%.%.:up=^K:ho=^^:bl=^G:bw:"
  ":ce=^W^H^E^F\\200\\200\\200\\200\\200\\200:"
  ":so=^W^Q^E\\200\\200\\200\\200\\200\\200\\200:"
  ":se=^W^Q^E\\200\\200\\200\\200\\200\\200\\200:"
  ":sb=^W^G^A^B\\200\\200\\200\\200\\200\\200:"
  ":sf=^W^G^A^C\\200\\200\\200\\200\\200\\200:"
  ":is=^C^F^D^O^V\\200:";

#define T_IOCTL			/* use TIOCGWINSIZ to get LI,CO */

#ifdef T_IOCTL
#include <termios.h>
#endif

/* #define T_TEST */
/* test main() */

/* #define T_DEBUG */
/* debugging output to t_debug */

#define T_FILE "/etc/termcap"	/* the database file */

static int t_tgetnam (unsigned char **, unsigned char **);
static int t_tgetln (FILE *, unsigned char *);
static int t_tentcp (unsigned char *);
static unsigned char *t_tgetid (const char *);
static unsigned char t_tcoord (int, unsigned int *);

/* max. tc indirections */
#define MAXTC 64

static char __PC, *__BC, *__UP;

static char *t_tbp;
static unsigned char *t_tbpstart;
static int t_recurs, t_tbpspace;
static unsigned char *t_tenv;
short ospeed;

#ifdef T_DEBUG
static FILE *t_debug;
#endif

/* tgetent() */

int
tgetent (char *bp, const char *name)
{
  unsigned char *tenv, *tent, *tentbuf, tc;
  unsigned char *tp1, *tp2;
  char *fnam;
  FILE *tfile = NULL;
  int nbyt, rval;

  PTHREAD_UNSAFE

  if (bp == NULL)
    return __set_errno (EINVAL);

  t_tbp = bp;

#ifdef T_DEBUG
  if (!t_recurs)
    {
      t_debug = fopen ("t_debug", "w");
      if (t_debug == NULL)
	return -1;
      setvbuf (t_debug, NULL, _IONBF, BUFSIZ);
    }
#endif

  if (!name)
#ifdef T_BUILTIN
    {
#ifdef T_DEBUG
      fputs ("tgetent(\"(null)\")\n", t_debug);
#endif
      name = t_bname;
    }
#else
    return -1;
#endif

  fnam = T_FILE;

  if (!t_recurs)
    t_tbpspace = 1024;

  if (!t_recurs)
    {
      char *t = getenv ("TERM");

      if (t == NULL)
	t = "";
      if (strcmp (name, t) == 0)
	t_tenv = (unsigned char *) getenv ("TERMCAP");
      else
	t_tenv = NULL;
    }

  tenv = t_tenv;
  if (tenv != NULL)
    {
      if (!t_recurs && *tenv != '/')
	{
#ifdef T_BUILTIN
	builtin:
#endif

	  while (isspace (*tenv))
	    tenv++;
	  tp1 = tp2 = tenv;

#ifdef T_DEBUG
	  fprintf (t_debug, "tgetent(\"%s\") < $TERMCAP\n", name);
#endif

	  while (t_tgetnam (&tp1, &tp2))
	    {
	      tc = *tp2;
	      *tp2 = 0;

#ifdef T_DEBUG
	      fprintf (t_debug, "t_tgetnam(): %s\n", (char *) tp1);
#endif

	      if (!strcmp ((char *) tp1, name))
		{
		  *tp2 = tc;

#ifdef T_DEBUG
		  fputs ("t_tgetnam() - MATCH -\n", t_debug);
#endif

		  if (strlen ((char *) tenv) > t_tbpspace)
		    return -1;
		  return t_tentcp (tenv) ? -1 : 1;
		}
	      else
		*tp2 = tc;
	    }
	}
      else if (*tenv == '/')
	{

#ifdef T_DEBUG
	  fprintf (t_debug, "tgetent(\"%s\") < %s\n", name, (char *) tenv);
#endif

	  fnam = (char *) tenv;
	}
    }

#ifdef T_DEBUG
  if (fnam != (char *) t_tenv)
    fprintf (t_debug, "tgetent(\"%s\") < /etc/termcap\n", name);
#endif

  tfile = fopen (fnam, "r");
  if (tfile == NULL)
#ifdef T_BUILTIN
    {
      if (fnam != (char *) t_tenv && !strcmp (name, t_bname))
	{
	  tenv = t_btenv;
	  goto builtin;
	}
      else
	return -1;
    }
#else
    return -1;
#endif

  tent = tentbuf = (unsigned char *) malloc (1024);
  /* Not enough space for malloc call.  */
  if (tent == NULL)
    return -1;

  while ((nbyt = t_tgetln (tfile, tent)))
    {
      if (*tent != '#')
	{
	  while (isspace (*tent))
	    tent++;
	  tp1 = tp2 = tent;
	  while (t_tgetnam (&tp1, &tp2))
	    {
	      tc = *tp2;
	      *tp2 = 0;

#ifdef T_DEBUG
	      fprintf (t_debug, "t_tgetnam(): %s\n", (char *) tp1);
#endif

	      if (!strcmp ((char *) tp1, name))
		{
		  *tp2 = tc;

#ifdef T_DEBUG
		  fprintf (t_debug, "t_tgetnam() - MATCH -\n");
		  fprintf (t_debug, "t_tgetnam(): [%d (%d)] %s\n", nbyt,
		  	   t_tbpspace, tent);
#endif

		  if (nbyt > t_tbpspace || nbyt < 0)
		    {
		      free ((char *) tentbuf);
		      return -1;
		    }
		  if (tfile)
		    {
		      fclose (tfile);
		      tfile = 0;
		    }
		  rval = t_tentcp (tent);
		  free ((char *) tentbuf);
		  return rval ? -1 : 1;
		}
	      else
		*tp2 = tc;
	    }
	}
    }

  if (tfile)
    {
      fclose (tfile);
      tfile = 0;
    }

  free ((char *) tentbuf);

  return 0;
}

/* t_tgetnam() */

static int
t_tgetnam (unsigned char **t1, unsigned char **t2)
{
  unsigned char *tp2 = *t2;
  while (*tp2 == '|' || isspace (*tp2))
    tp2++;
  if (*tp2 == ':')
    return 0;
  unsigned char *tp1 = tp2;
  while (*tp2 != '|' && *tp2 != ':')
    tp2++;

  *t1 = tp1;
  *t2 = tp2;
  return -1;
}

/* t_tentcp() */

static int
t_tentcp (unsigned char *s)
{
  unsigned char *s1, *s2, *sp;
  char *t_tbp_;
  char *tcnam, *tcn;

  PTHREAD_UNSAFE

#ifdef T_DEBUG
  fputs ("t_tentcp()\n", t_debug);
#endif

  tcnam = (char *) malloc (256);
  /* Not enough space for malloc.  */
  if (tcnam == NULL)
    return -1;

  s1 = (unsigned char *) t_tbp;
  s2 = s;

  while (*s2)
    {
      if (*s2 == ':')		/* strips out empty capabilities :: */
	{
	  sp = s2 + 1;
	  while (isspace (*sp))
	    sp++;
	  if (*sp == ':')
	    s2 = sp;
	}
      *s1++ = *s2++;
    }

  *s1 = 0;

  if (!t_recurs)
    {
      sp = (unsigned char *) t_tbp;
      while (*sp++ != ':');
      t_tbpstart = sp;
    }

  sp = t_tgetid ("tc");
  if (sp)
    {
#ifdef T_DEBUG
      fprintf (t_debug, "tgetent()# %d\n", t_recurs);
#endif

      if (++t_recurs > (MAXTC - 1))
	{
	  free (tcnam);
	  return -1;
	}
      t_tbp_ = t_tbp;

      tcn = tcnam;
      s1 = sp = sp - 2;
      t_tbpspace -= (int) (s1 - (unsigned char *) t_tbp);
      if (tgetent ((char *) s1, tgetstr ("tc", &tcn)) < 1)
	{
	  free (tcnam);
	  return -1;
	}

      while (*sp++ != ':')
        ;
      while ((*s1++ = *sp++))
        ;

      t_tbp = t_tbp_;
      t_recurs--;
    }

#ifdef T_DEBUG
  fprintf (t_debug, "tgetent(): %s\n", t_tbp);
#endif

  if (!t_recurs)
    {
      __PC = 0;
      __BC = "\b";
      __UP = "\v";
    }

  free (tcnam);

  return 0;
}

/* t_tgetln() */

static int
t_tgetln (FILE * tfile, unsigned char *buf)
{
  unsigned char *bufp;
  int nbyt;

  bufp = buf;
  nbyt = 0;

  PTHREAD_UNSAFE

  while (-1)
    {
      if (!fgets ((char *) bufp, t_tbpspace - nbyt, tfile))
	return nbyt;
      while (*bufp++)
        ;
      bufp--;
      if (*--bufp != '\n')
	++bufp;
      nbyt += (bufp - buf);
      if (!nbyt)
	continue;
      if (*--bufp != '\\')
	{
	  *++bufp = 0;
	  return nbyt;
	}
      buf = bufp;
      nbyt--;
    }
}

/* t_tgetid() */

static unsigned char *
t_tgetid (const char *id)
{
  unsigned char *bptr;
  int found;

  PTHREAD_UNSAFE

  if (id == NULL || t_tbpstart == NULL)
    return NULL;

  bptr = t_tbpstart;
  while (*bptr)
    {
      found = (((unsigned char) *id++ == *bptr++) ? \
	       ((unsigned char) *id == *bptr) : 0);
      bptr++;
      id--;
      if (found)
	{
	  if (*bptr == '@')
	    return NULL;
	  else
	    return bptr;
	}
      while (*bptr != ':' && *bptr != 0)
	bptr++;
      if (*bptr == ':')
	bptr++;
    }
  return NULL;
}

/* tgetnum() */

int
tgetnum (const char *id)
{
  unsigned char *eptr;
  int rval;

#ifdef TIOCGWINSZ
  if (id[0] == 'l' && id[1] == 'i' && id[2] == '\0')
    rval = 1;
  else if (id[0] == 'c' && id[1] == 'o' && id[2] == '\0')
    rval = -1;
  else
    rval = 0;

  if (rval)
    {
      struct winsize w[1];

      ioctl (2, TIOCGWINSZ, w);
      return (rval > 0) ? w->ws_row : w->ws_col;
    }
#endif

  eptr = t_tgetid (id);
  if (eptr == NULL)
    return -1;

  if (*eptr++ != '#')
    return 0;
  rval = atoi ((const char *) eptr);

#ifdef T_DEBUG
  fprintf (t_debug, "tgetnum(\"%s\"): %d\n", id, rval);
#endif

  return rval;
}

/* tgetflag() */

int
tgetflag (const char *id)
{
  unsigned char *idp;

#ifdef T_DEBUG
  int rval;

  idp = t_tgetid (id);
  rval = (idp ? ((*idp != '@') ? 1 : 0) : 0);
  fprintf (t_debug, "tgetflag(\"%s\"): %d\n", id, rval);
  return rval;
#else
  idp = t_tgetid (id);
  return idp ? ((*idp != '@') ? 1 : 0) : 0;
#endif
}

/* tgetstr() */

char *
tgetstr (const char *id, char **area)
{
  unsigned char *eptr, *aptr;
  unsigned char obuf[4];
  char *rval;

  PTHREAD_UNSAFE
  
#ifdef T_DEBUG
  fprintf (t_debug, "tgetstr(\"%s\")\n", id);
#endif

  if (!(area ? *area : 0))
    {
      rval = 0;
      goto tgetstr_ret;
    }

  aptr = (unsigned char *) *area;

  eptr = t_tgetid (id);
  if (eptr == NULL)
    return 0;

  if (*eptr++ != '=')
    {
      *aptr = 0;
      return 0;
    }

  if (*eptr >= '0' && *eptr <= '9')
    {
      while (*eptr >= '0' && *eptr <= '9')
	*aptr++ = *eptr++;
      if (*eptr == '*')
	*aptr++ = *eptr++;
    }

  obuf[3] = 0;
  while (*eptr != ':' && *eptr != 0)
    {
      switch (*eptr)
	{
	case '\\':
	  switch (*++eptr)
	    {
	    case 'E':
	      *aptr = 0x1b;
	      break;
	    case 'n':
	      *aptr = 0x0a;
	      break;
	    case 'r':
	      *aptr = 0x0d;
	      break;
	    case 't':
	      *aptr = 0x09;
	      break;
	    case 'b':
	      *aptr = 0x08;
	      break;
	    case 'f':
	      *aptr = 0x0c;
	      break;
	    case '0':
	    case '1':
	    case '2':
	    case '3':
	      obuf[0] = *eptr++;
	      obuf[1] = *eptr++;
	      obuf[2] = *eptr;
	      *aptr = (char) (strtol ((const char *) obuf, 0, 8) & 0xff);
	      break;
	    default:
	      *aptr = *eptr;
	    }
	  break;
	case '^':
	  *aptr = ((*++eptr) & 0x1f);
	  break;
	default:
	  *aptr = *eptr;
	  break;
	}
      eptr++;
      aptr++;
    }

  *aptr++ = 0;
  rval = *area;
  *area = (char *) aptr;

tgetstr_ret:

#ifdef T_DEBUG
  fprintf (t_debug, "tgetstr(\"%s\"): %s\n", id, rval);
#endif

  if (rval)
    {
      if (id[1] == 'c')
	{
	  if (id[0] == 'b')
	    __BC = rval;
	  else if (id[0] == 'p')
	    __PC = *rval & 0x7f;
	}
      else if (id[0] == 'u' && id[1] == 'p')
	__UP = rval;
      else if (id[0] == 'l' && id[1] == 'e')	/* terminfo compatibility */
	__BC = rval;
    }

  return rval;
}

static int tg_aoff, tg_coff, tg_clev;

#define TG_revxy	0001
#define TG_incxy	0002
#define TG_eorxy	0004
#define TG_bcdxy	0010
#define TG_revcod	0020
#define TG_bc		0040
#define TG_up		0100
#define TG_chkout	0200

/* tgoto() */

char *
tgoto (char *cm, int destcol, int destline)
{
  static char rstr[256];
  char *cmp, *rstrp, *cp;
  unsigned int f;

  PTHREAD_UNSAFE
  
#define TC(f) (((f) & TG_revxy) ? destcol : destline)

#ifdef T_DEBUG
  fprintf (t_debug, "tgoto(\"%s\",%d,%d): ", cm, destcol, destline);

  if (cm == NULL)
    {
      putc ('\n', t_debug);
      return 0;
    }
#else
  if (!cm)
    return 0;
#endif

  tg_aoff = tg_coff = tg_clev = 0;
  f = 0;

  cmp = cm;
  rstrp = rstr;
  while (*cmp)
    {
      if (*cmp == '%')
	{
	  int d;
	  char *e;

	  cmp++;
	  switch (*cmp++)
	    {
	    case 'r':
	      f |= TG_revxy;
	      break;
	    case 'i':
	      f |= TG_incxy;
	      break;
	    case 'n':
	      f |= TG_eorxy;
	      break;
	    case 'B':
	      f |= TG_bcdxy;
	      break;
	    case 'D':
	      f |= TG_revcod;
	      break;
	    case '>':
	      tg_clev = *cmp++;
	      tg_coff = *cmp++;
	      break;
	    case '+':
	      tg_aoff = *cmp++;	/* fallthru */
	    case '.':
	      f |= TG_chkout;
	      *rstrp++ = t_tcoord (TC (f), &f);
	      break;
	    case 'd':
	      d = (int) t_tcoord (TC (f), &f);
	      e = rstrp + 1;
	      if (d > 9)
		e++;
	      if (d > 99)
		e++;
	      rstrp = e;
	      *e = 0;
	      do
		{
		  *--e = (d % 10) + '0';
		  d /= 10;
		}
	      while (d);
	      break;
	    case '2':
	      d = (int) t_tcoord (TC (f), &f);
	      *rstrp = ' ';
	      if (d > 99)
		*++rstrp = ' ';
	      e = rstrp = rstrp + 2;
	      *e = 0;
	      do
		{
		  *--e = (d % 10) + '0';
		  d /= 10;
		}
	      while (d);
	      break;
	    case '3':
	      d = (int) t_tcoord (TC (f), &f);
	      *rstrp = ' ';
	      *++rstrp = ' ';
	      e = rstrp = rstrp + 2;
	      *e = 0;
	      do
		{
		  *--e = (d % 10) + '0';
		  d /= 10;
		}
	      while (d);
	      break;
	    case '%':
	      *rstrp++ = '%';
	      break;
	    default:
	      rstrp = "OOPS";
	      goto tgoto_ret;
	      break;
	    }
	}
      else
	*rstrp++ = *cmp++;
    }

  if (f & TG_bc)
    {
      cp = __BC;
      while ((*rstrp = *cp))
	rstrp++, cp++;
    }
  if (f & TG_up)
    {
      cp = __UP;
      while ((*rstrp = *cp))
	rstrp++, cp++;
    }

  *rstrp = 0;

  rstrp = rstr;

tgoto_ret:

#ifdef T_DEBUG
  fprintf (t_debug, "%s\n", rstrp);
#endif

  return rstrp;
}

/* t_tcoord() */

static unsigned char
t_tcoord (int x, unsigned int *_f)
{
  unsigned int f;

  PTHREAD_UNSAFE
  
t_tccalc:

  f = (*_f);

  if (x > tg_clev)
    x += tg_coff;
  x += tg_aoff;
  if (f & TG_incxy)
    x++;
  if (f & TG_eorxy)
    x ^= 0140;
  if (f & TG_bcdxy)
    x = ((x / 10) << 4) | (x % 10);
  if (f & TG_revcod)
    x = (x - ((x & 15) << 1));

  if (f & TG_chkout)
    {
      (*_f) &= (~TG_chkout);
      if (!x || x == '\004' || x == '\n' || x == '\r')
	{
	  if (f & TG_revxy)
	    (*_f) |= TG_bc;
	  else
	    (*_f) |= TG_up;
	  x++;
	  goto t_tccalc;
	}
    }

  (*_f) ^= TG_revxy;
  tg_aoff = 0;
  return x;
}

/* tputs() */

int
tputs (const char *cp, int affcnt, int (*outc) (int))
{
  int i;
  char *cpp;
  int delay;

#ifdef T_DEBUG
  fprintf (t_debug, "tputs(\"%s\",%d)\n", cp, affcnt);
#endif

  if (cp == NULL || outc == NULL)
    return 0;

  if (isdigit (*cp))
    {
      delay = (int) strtol (cp, &cpp, 0);
      cp = cpp;
    }
  else
#ifdef T_DEBUG
    {
      cpp = cp;
      delay = 0;
    }
#else
    delay = 0;
#endif

  i = *cp;

  if (i == '.')
    while (isdigit (*++cp));
  if (i == '*')
    {
      delay *= affcnt;
      cp++;
    }

  if (ospeed > 13)
    delay <<= (ospeed - 13);
  else
    delay >>= (13 - ospeed);

#ifdef T_DEBUG
  fprintf (t_debug, "tputs() (pad %d): %s\n", delay, cpp);
#endif

  while ((i = *cp++))
    __funcall ((*outc), (i & 0x7f));

  if (ospeed)
    while (delay-- > 0)
      __funcall ((*outc), (__PC));

  return 0;
}

#ifdef T_TEST

char PC, *BC, *UP;

static int out (char);
static void dump (char *);

/* main() */

int
main (void)
{
  static char bp[1024], tbuf[512];
  char *tbufp, *cm;
  int rval, i;

  rval = tgetent (bp, getenv ("TERM"));
  if (rval < 1)
    {
      fprintf (stderr, "termcap: tgetent() returned %d\n", rval);
      exit (1);
    }

  tbufp = tbuf;

  if (tgetstr ("pc", &tbufp))
    PC = *tbuf;

  BC = tgetstr ("bc", &tbufp);
  if (BC == NULL)
    BC = "\b";

  UP = tgetstr ("up", &tbufp);

  tputs (tgetstr ("cl", &tbufp), 1, out);

  printf ("tgetent(): %s\n", bp);

  printf ("tgetflag(\"cm\"): %d\n", tgetflag ("cm"));

  printf ("tgetnum(\"li\"): %d\n", tgetnum ("li"));

  tbufp = tbuf;

  printf ("tgetstr(\"cm\"): ");
  dump (cm = tgetstr ("cm", &tbufp));

  for (i = 0; i < 10; i++)
    {
      tputs (tgoto (cm, i << 1, i + 20), 1, out);
      printf ("%d", i);
    }

  printf ("\n");
}

static int
out (char c)
{
  return putc (c, stdout);
}

static void
dump (char *s)
{
  if (s == NULL)
    {
      printf ("(null)\n");
      return;
    }
  while (*s)
    {
      if ((*s < ' ') || (*s > '\176'))
	printf ("\\%03o", (int) *s);
      else
	putc (*s, stdout);
      s++;
    }
  putc ('\n', stdout);
}

#endif /* T_TEST */
