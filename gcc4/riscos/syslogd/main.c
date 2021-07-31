/* Gateway for syslog msg via Inet to !Syslog
 * Written by John Tytgat / BASS <URL:mailto:John.Tytgat@aaug.net>
 * Contributions by Alex Waugh.
 */

#define _BSD_SOURCE /* for strdup() */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <swis.h>

#include <netdb.h> /* struct servent */
#include <sys/ioctl.h> /* socketioctl() */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/ioctl.h>

#include "syslog.h"
#include "modheader.h"

/* Function prototypes */
static _kernel_oserror *claim_eventv(void *pw);
static _kernel_oserror *release_eventv(void *pw);
static _kernel_oserror *enable_eventv(void);
static _kernel_oserror *disable_eventv(void);

static int sock = -1; /* Socket number we're listening too */
static int callback_queue = 0;

/* Buffers for reading from the socket, and preparing the log message. */
static char logbuffer[1024+32];
static char readbuffer[1024+1];

struct filenamemapping {
  char *ro;
  size_t rolen;
  char *unix;
  size_t unixlen;
  struct filenamemapping *next;
};

static struct filenamemapping *mappings = NULL;

#define kLogName "SysLogD"
#define kLogPrio 32

static const _kernel_oserror socket_failed_to_create = { 0, "Can not open datagram socket"};
static const _kernel_oserror socket_failed_to_nonblock = { 0, "Can not make socket non blocking"};
static const _kernel_oserror alloc_failure = { 0, "Unable to allocate memory"};

/* Nicked from syslog.h @ BSD 4.3 */
#define	LOG_EMERG	0	/* system is unusable */
#define	LOG_ALERT	1	/* action must be taken immediately */
#define	LOG_CRIT	2	/* critical conditions */
#define	LOG_ERR		3	/* error conditions */
#define	LOG_WARNING	4	/* warning conditions */
#define	LOG_NOTICE	5	/* normal but significant condition */
#define	LOG_INFO	6	/* informational */
#define	LOG_DEBUG	7	/* debug-level messages */

#define	LOG_PRIMASK	0x07	/* mask to extract priority part (internal) */

#define	LOG_PRI(p)	((p) & LOG_PRIMASK)

static const char * const prioritynames[LOG_PRIMASK + 1] =
{
  "emerg",
  "alert",
  "crit",
  "error",
  "warning",
  "notice",
  "info",
  "debug"
};

/* facility codes */
#define	LOG_KERN	(0<<3)	/* kernel messages */
#define	LOG_USER	(1<<3)	/* random user-level messages */
#define	LOG_MAIL	(2<<3)	/* mail system */
#define	LOG_DAEMON	(3<<3)	/* system daemons */
#define	LOG_AUTH	(4<<3)	/* security/authorization messages */
#define	LOG_SYSLOG	(5<<3)	/* messages generated internally by syslogd */
#define	LOG_LPR		(6<<3)	/* line printer subsystem */
#define	LOG_NEWS	(7<<3)	/* network news subsystem */
#define	LOG_UUCP	(8<<3)	/* UUCP subsystem */
#define	LOG_CRON	(9<<3)	/* clock daemon */
#define	LOG_AUTHPRIV	(10<<3)	/* security/authorization messages (private) */
#define	LOG_FTP		(11<<3)	/* ftp daemon */

	/* other codes through 15 reserved for system use */
#define	LOG_LOCAL0	(16<<3)	/* reserved for local use */
#define	LOG_LOCAL1	(17<<3)	/* reserved for local use */
#define	LOG_LOCAL2	(18<<3)	/* reserved for local use */
#define	LOG_LOCAL3	(19<<3)	/* reserved for local use */
#define	LOG_LOCAL4	(20<<3)	/* reserved for local use */
#define	LOG_LOCAL5	(21<<3)	/* reserved for local use */
#define	LOG_LOCAL6	(22<<3)	/* reserved for local use */
#define	LOG_LOCAL7	(23<<3)	/* reserved for local use */

#define	LOG_NFACILITIES	24	/* current number of facilities */
#define	LOG_FACMASK	0x03f8	/* mask to extract facility part */
				/* facility of pri */
#define	LOG_FAC(p)	(((p) & LOG_FACMASK) >> 3)

typedef struct _code
{
  const char	*c_name;
  int   	c_val;
} CODE;

static const CODE facilitynames[] =
{
  { "auth",	LOG_AUTH },
  { "authpriv",	LOG_AUTHPRIV },
  { "cron", 	LOG_CRON },
  { "daemon",	LOG_DAEMON },
  { "ftp",	LOG_FTP },
  { "kern",	LOG_KERN },
  { "lpr",	LOG_LPR },
  { "mail",	LOG_MAIL },
  { "mark", 	-1 },	             	/* INTERNAL */
  { "news",	LOG_NEWS },
  { "security",	LOG_AUTH },		/* DEPRECATED */
  { "syslog",	LOG_SYSLOG },
  { "user",	LOG_USER },
  { "uucp",	LOG_UUCP },
  { "local0",	LOG_LOCAL0 },
  { "local1",	LOG_LOCAL1 },
  { "local2",	LOG_LOCAL2 },
  { "local3",	LOG_LOCAL3 },
  { "local4",	LOG_LOCAL4 },
  { "local5",	LOG_LOCAL5 },
  { "local6",	LOG_LOCAL6 },
  { "local7",	LOG_LOCAL7 }
};

static const char *syslog_getfacility(int p);

/* The DDEUtils module throwback error category codes.  */
#define THROWBACK_INFORMATION         -1
#define THROWBACK_WARNING              0
#define THROWBACK_ERROR                1
#define THROWBACK_SERIOUS_ERROR        2

/* The DDEUtils module throwback reason codes.  */
#define THROWBACK_REASON_PROCESSING    0
#define THROWBACK_REASON_ERROR_DETAILS 1
#define THROWBACK_REASON_INFO_DETAILS  2

static inline _kernel_oserror *
xsyslog_logmessage (const char *__name, const char *__msg, int __priority)
{
  register const char *name __asm ("r0") = __name;
  register const char *msg __asm ("r1") = __msg;
  register int priority __asm ("r2") = __priority;
  register _kernel_oserror *err __asm ("r0");
  __asm__ volatile ("SWI\t%[SWI_XSysLog_LogMessage]\n\t"
		    "MOVVC\tr0, #0\n\t"
		    : "=r" (err)
                    : "r" (name), "r" (msg), "r" (priority),
		      [SWI_XSysLog_LogMessage] "i" (SysLog_LogMessage | (1<<17))
		    : "r14", "cc");
  return err;
}


/* Initialisation routine */
_kernel_oserror *
SysLogD_Init (const char *cmd_fail __attribute__ ((unused)),
	      int podule_base __attribute__ ((unused)), void *pw)
{
  xsyslog_logmessage(kLogName, "SysLogD begins...", kLogPrio);

  struct servent *servptr = getservbyname("syslog", "udp");
  int port;
  if (servptr == NULL)
    port = 514;
  else
    port = servptr->s_port;

  struct sockaddr_in name;
  name.sin_family = AF_INET;
  name.sin_addr.s_addr = htons(INADDR_ANY);
  name.sin_port = htons(port);
  
  /* Create socket */
  if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    return (_kernel_oserror *)&socket_failed_to_create;

  /* Make socket async & nonblocking and bind it */
  int on = 1;
  if (socketioctl(sock, FIOASYNC, &on) < 0
      || socketioctl(sock, FIONBIO, &on) < 0
      || bind(sock, (struct sockaddr *)&name, sizeof(name)) < 0)
    {
      socketclose(sock);
      sock = -1;
      return (_kernel_oserror *)&socket_failed_to_nonblock;
    }

  /* Read mappings list and create linked list of the filename mappings */
  FILE *file = fopen("Choices:syslogd.mappings", "r");
  if (file)
    {
      while (fgets(readbuffer, sizeof(readbuffer), file))
        {
          struct filenamemapping *mapping;
          char *rofilename;
          char *ch;

          mapping = malloc(sizeof(struct filenamemapping));
          if (mapping == NULL)
            {
              fclose(file);
              socketclose(sock);
              return (_kernel_oserror *)&alloc_failure;
            }

          ch = readbuffer;
          while (*ch && *ch != ' ' && *ch != '\n')
            ch++;

          if (*ch)
            *ch++ = '\0';

          rofilename = ch;
          while (*ch && *ch != ' ' && *ch != '\n')
            ch++;

          if (*ch)
            *ch = '\0';

          mapping->rolen = ch - rofilename;
          mapping->ro = strdup(rofilename);
          if (mapping->ro == NULL)
            {
              free(mapping);
              fclose(file);
              socketclose(sock);
              return (_kernel_oserror *)&alloc_failure;
            }

          mapping->unixlen = rofilename - readbuffer - 1;
          mapping->unix = strdup (readbuffer);
          if (mapping->unix == NULL)
            {
              free(mapping->ro);
              free(mapping);
              fclose(file);
              socketclose(sock);
              return (_kernel_oserror *)&alloc_failure;
            }

          /* Add on to linked list */
          mapping->next = mappings;
          mappings = mapping;
        }
    }

  fclose(file);

  /* Claim event */
  _kernel_oserror *e;
  if ((e = claim_eventv (pw)) != NULL)
    {
      socketclose(sock);
      return e;
    }
  if ((e = enable_eventv()) != NULL)
    {
      release_eventv(pw);
      socketclose(sock);
      return e;
    }

  return NULL;
}

/* Finalisation routine */
_kernel_oserror *
SysLogD_Final (int fatal __attribute__ ((unused)),
	       int podule __attribute__ ((unused)), void *pw)
{
  xsyslog_logmessage(kLogName, "SysLogD ends...", kLogPrio);

  /* Disable the Internet Event handling */
  disable_eventv();
  release_eventv(pw);

  /* Remove pending AddCallBack request, if necessary */
  if (callback_queue)
    {
      _kernel_swi_regs r;
  
      r.r[0] = (int)callback_handler_entry;
      r.r[1] = (int)pw;
      (void)_kernel_swi(XOS_Bit | OS_RemoveCallBack, &r, &r);
    }
  
  /* Close the socket we've asked */
  if (sock >= 0)
    {
      socketclose(sock);
      sock = -1;
    }

  /* Free the mappings list */
  while (mappings)
    {
      struct filenamemapping *next = mappings->next;

      free(mappings->ro);
      free(mappings->unix);
      free(mappings);
      mappings = next;
    }

  return NULL;
}

/* Event routines */
#define EventV 16
#define Event_Enable 14
#define Event_Disable 13
#define Internet_Event 19
#define Socket_Async_Event 1

static _kernel_oserror *
claim_eventv (void *pw)
{
  _kernel_swi_regs r;
  r.r[0] = EventV;
  r.r[1] = (int)eventv_handler_entry;
  r.r[2] = (int)pw;
  return _kernel_swi(XOS_Bit | OS_Claim, &r, &r);
}

static _kernel_oserror *
release_eventv (void *pw)
{
  _kernel_swi_regs r;
  r.r[0] = EventV;
  r.r[1] = (int)eventv_handler_entry;
  r.r[2] = (int)pw;
  return _kernel_swi(XOS_Bit | OS_Release, &r, &r);
}

static _kernel_oserror *
enable_eventv (void)
{
  _kernel_swi_regs r;
  r.r[0] = Event_Enable;
  r.r[1] = Internet_Event;
  return _kernel_swi(XOS_Bit | OS_Byte, &r, &r);
}

static _kernel_oserror *
disable_eventv (void)
{
  _kernel_swi_regs r;
  r.r[0] = Event_Disable;
  r.r[1] = Internet_Event;
  return _kernel_swi(XOS_Bit | OS_Byte, &r, &r);
}

/* 0 to claim event, non-0 if not to claim the event */
int
eventv_handler (_kernel_swi_regs *r, void *pw)
{
  /* Check for Internet Event & our socket has input, if not, pass it on */
  if (r->r[0] != Internet_Event
      || r->r[1] != Socket_Async_Event
      || r->r[2] != sock)
    return 1;
  
  /* Don't know if IRQs are enabled or disabled here.  We play it extremely
   * save here.
   */
  const int irqs_disabled = _kernel_irqs_disabled();
  if (irqs_disabled == 0)
    _kernel_irqs_off();
  if (callback_queue == 0)
    {
      _kernel_swi_regs r;
  
      r.r[0] = (int)callback_handler_entry;
      r.r[1] = (int)pw;
      (void)_kernel_swi(XOS_Bit | OS_AddCallBack, &r, &r);
    
      callback_queue = 1;
    }
  if (irqs_disabled == 0)
    _kernel_irqs_on();
  
  /* Claim the event, it was for us */
  return 0;
}

static void
translate (char *dst, const char *src, int len)
{
  while (len--)
    {
      switch (*src)
        {
          case '.':
            *dst++ = '/';
            break;
          case '/':
            *dst++ = '.';
            break;
          default:
            *dst++ = *src;
        }
      src++;
    }
  *dst = '\0';
}

/* 0 to claim event, non-0 if not to claim the event */
int
callback_handler (_kernel_swi_regs *r __attribute__ ((unused)),
		  void *pw __attribute__ ((unused)))
{
  int bytesread;
  while ((bytesread = socketread(sock, readbuffer, sizeof(readbuffer) - 1)) > 0)
    {
      int from = 0;
      int to = 0;
      int priority = kLogPrio;
      int facility = LOG_KERN;
      char *tag;

      readbuffer[bytesread] = '\0';

      /* Detect if the strings begins with "<number>" and if so, extract
       * the priority & facility from it.
       */
      if (readbuffer[from] == '<')
        {
	  int i;
          for (i = from + 1; i < bytesread && isdigit(readbuffer[i]); i++)
            /* no body */;

          if (i < bytesread && readbuffer[i] == '>')
            {
              /* priority & facility detected */
              readbuffer[i] = '\0';
              priority = atoi(readbuffer + from + 1);
              from = i + 1;

              to = snprintf(logbuffer, sizeof(logbuffer), "(%s, %s) ", prioritynames[LOG_PRI(priority)], syslog_getfacility(priority));

              facility = LOG_FAC(priority);
              priority = LOG_PRI(priority);
            }
        }

      tag = strstr(readbuffer + from, "throwback ");
      if (facility == LOG_FAC(LOG_USER) && tag != NULL)
        {
          char *filename = tag + sizeof("throwback");
          char *linenum;
          char *msg;
          int seriousness;
          struct filenamemapping *mapping = mappings;
          static char newfilename[1024];

          /* Extract the filename, line number and message */
          while (isspace(*filename))
            filename++;

          linenum = filename;
          while ((*linenum != '\0') && (*linenum != ':'))
            linenum++;

          if (*linenum)
            *linenum++ = '\0';

          msg = linenum;
          while ((*msg != '\0') && (*msg != ':'))
            msg++;

          if (*msg)
            *msg++ = '\0';

          while (isspace(*msg))
            msg++;

          if (filename[0] == '/')
            {
              /* Absolute unix path */
              while (mapping)
                {
                  /* Search through the mappings to find a match for the unix format pathname */
                  if (strncmp(filename, mapping->unix, mapping->unixlen) == 0)
                    {
                      char *oldfilename = filename + mapping->unixlen;
                      int oldlen = strlen(oldfilename);

                      if (oldlen + mapping->rolen < sizeof(newfilename))
                        {
                          memcpy(newfilename, mapping->ro, mapping->rolen);
                          filename = newfilename + mapping->rolen;
    
                          /* Translate the rest of the unix pathname to RISC OS format */
                          translate(filename, oldfilename, oldlen);
    
                          filename = newfilename;
                        }
                      break;
                    }
    
                  mapping = mapping->next;
                }
            }
          else
            {
              /* A relative path */
              char *dot = strrchr(filename, '.');
              char *slash = strrchr(filename, '/');
              char *suffixes = getenv("UnixEnv$gcc$sfix");

              /* Translate everything up to the last slash */
              char *dst;
              if (slash)
                {
                  translate(newfilename, filename, (slash + 1) - filename);
                  dst = newfilename + ((slash + 1) - filename);
                  filename = slash + 1;
                }
              else
                dst = newfilename;

              if (dot && suffixes)
                {
                  suffixes = strdup(suffixes);

                  /* Search suffix list for a match */
                  char *suffix = strtok(suffixes, ":");
                  while (suffix)
                    {
                      if (strcmp(suffix, dot + 1) == 0)
                        {
                          *dot++ = '\0';
                          strcpy(dst, dot);
                          dst += strlen(dot);
                          *dst++ = '.';
                          break;
                        }
                      suffix = strtok(NULL, ":");
                    }
                }

              /* Translate whatever is left */
              translate(dst, filename, strlen(filename));
              filename = newfilename;
            }

          if (_swix(DDEUtils_ThrowbackStart, 0))
            break;

          if (_swix(DDEUtils_ThrowbackSend, _IN(0) | _IN(2),
	      THROWBACK_REASON_PROCESSING, filename))
            break;

          seriousness = (priority == LOG_ERR) ? THROWBACK_ERROR : THROWBACK_WARNING;
          if (_swix(DDEUtils_ThrowbackSend, _IN(0) | _INR(2,5),
		    THROWBACK_REASON_ERROR_DETAILS, filename,
		    atoi(linenum), seriousness, msg))
            break;

          if (_swix(DDEUtils_ThrowbackEnd, 0))
            break;
        }
      else
        {
          snprintf(logbuffer + to, sizeof(logbuffer) - to, "%s", readbuffer + from);
          xsyslog_logmessage(kLogName, logbuffer, priority);
        }
    }

  callback_queue = 0;
  /* Never claim the callback handler, wild things might happen */
  return 1;
}

/* syslog facility retrieving code */
static const char *
syslog_getfacility(int p)
{
  int facility = LOG_FAC(p);
  for (int i = 0; i < sizeof(facilitynames) / sizeof(CODE); i++)
    {
      if (facility == (facilitynames[i].c_val >> 3))
        return facilitynames[i].c_name;
    }
  
  return "unknown";
}
