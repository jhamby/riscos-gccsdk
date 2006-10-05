/*
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#ifndef __TERMCAP_H
#define __TERMCAP_H

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

__BEGIN_DECLS

extern char PC, *BC, *UP;
extern short ospeed;

extern int tgetent (char *__buffer, const char *__termtype);
extern int tgetflag (const char *__name);
extern int tgetnum (const char *__name);
extern char *tgetstr (const char *__name, char **__area);
extern char *tgoto (char *__cstring, int __hpos, int __vpos);
extern int tputs (const char *__string, int nlines, int ((*__outfun) (int)));

__END_DECLS

#endif
