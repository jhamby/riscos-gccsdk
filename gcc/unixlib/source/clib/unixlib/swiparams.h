/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/unixlib/swiparams.h,v $
 * $Date: 2003/04/12 11:31:39 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#ifndef __UNIXLIB_SWIPARAMS_H
#define __UNIXLIB_SWIPARAMS_H

#ifdef __UNIXLIB_INTERNALS

/* Limited set of SWI parameters needed in UnixLib itself */

#define OSFILE_READCATINFO_FILEPATH  5
#define OSFILE_READCATINFO_PATH     13
#define OSFILE_READCATINFO_PATHVAR  15
#define OSFILE_READCATINFO_NOPATH   17

#define OSFILE_WRITECATINFO_ATTR     4
#define OSFILE_WRITECATINFO_FILETYPE 18

#define OSFIND_OPEN_OPENIN            (4<<4)
#define OSFIND_OPEN_OPENOUT           (8<<4)
#define OSFIND_OPEN_OPENUP            (12<<4)

#define OSFIND_OPEN_PATH              1
#define OSFIND_OPEN_PATHVAR           2
#define OSFIND_OPEN_NOPATH	      3
#define OSFIND_OPEN_ERRORIFABSENT     8
#define OSFIND_OPEN_ERRORIFDIR	      4

#ifdef __UNIXLIB_USEFILEPATH
#define OSFILE_READCATINFO (OSFILE_READCATINFO_FILEPATH)
#define OSFILE_OPENIN	   (OSFIND_OPEN_OPENIN | OSFIND_OPEN_NOPATH)
#define OSFILE_OPENOUT	   (OSFIND_OPEN_OPENOUT | OSFIND_OPEN_NOPATH)
#define OSFILE_OPENUP	   (OSFIND_OPEN_OPENUP | OSFIND_OPEN_NOPATH)
#else
#define OSFILE_READCATINFO (OSFILE_READCATINFO_NOPATH)
#define OSFILE_OPENIN	   (OSFIND_OPEN_OPENIN)
#define OSFILE_OPENOUT	   (OSFIND_OPEN_OPENOUT)
#define OSFILE_OPENUP	   (OSFIND_OPEN_OPENUP)
#endif

#define MMM_TYPE_RISCOS               0
#define MMM_TYPE_RISCOS_STRING        1
#define MMM_TYPE_MIME                 2
#define MMM_TYPE_DOT_EXTN             3

#endif  /* __UNIXLIB_INTERNALS */

#endif
