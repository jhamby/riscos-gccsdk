/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/clib/h/fcrypt,v $
 * $Date: 1997/10/09 19:59:46 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

/* Fcrypt.h: Header file for fcrypt() DES encryption */

#ifndef __FCRYPT_H
#define __FCRYPT_H 1

#ifdef __cplusplus
extern "C" {
#endif

extern void init_des (void);
extern char *fcrypt (const char *, const char *);

#ifdef __cplusplus
}
#endif

#endif
