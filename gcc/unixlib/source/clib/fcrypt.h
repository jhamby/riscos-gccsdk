/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/fcrypt.h,v $
 * $Date: 2000/07/15 14:52:11 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
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
