/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/termcap.h,v $
 * $Date: 2000/07/15 14:52:12 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifndef __TERMCAP_H
#define __TERMCAP_H

#ifdef __cplusplus
extern "C" {
#endif

extern char PC, *BC, *UP;
extern short ospeed;

extern int tgetent (char *__buffer, const char *__termtype);
extern int tgetflag (const char *__name);
extern int tgetnum (const char *__name);
extern char *tgetstr (const char *__name, char **__area);
extern char *tgoto (char *__cstring, int __hpos, int __vpos);
extern int tputs (const char *__string, int nlines, int ((*__outfun) (int)));

#ifdef __cplusplus
}
#endif

#endif
