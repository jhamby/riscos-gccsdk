/*
Common defines for GNU Pascal compiler and the run time system

Copyright (C) 1991-2000 Free Software Foundation, Inc.

This file is part of GNU Pascal.

GNU Pascal is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU Pascal is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Pascal; see the file COPYING. If not, write to the
Free Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.

As a special exception, if you link this file with files compiled
with a GNU compiler to produce an executable, this does not cause
the resulting executable to be covered by the GNU General Public
License. This exception does not however invalidate any other
reasons why the executable file might be covered by the GNU General
Public License.
*/

#ifndef _RTS_TYPES_H_
#define _RTS_TYPES_H_

/*
 * The Pascal FILE_TYPE object is defined in fdr.h
 *
 * The things the compiler needs to know about the FILE_TYPE are:
 *  1) The size of the object. (Could be avoided if file buffers
 *                              were allocated from heap, but...)
 *  2) The file buffer address is in the first word of the object.
 *  3) The file status bits are in the second word of the file object
 *     The compiler can make inline code of EOF and EOLn by checking
 *     these bits. (_p_eof and _p_eoln also exist, but are slower)
 *     -- Not really, since _p_eof and _p_eoln do some more stuff... (fh)
 */

/* This file is also included in the compiler to keep the definition
 * unique (util.c file in the GCC compiler Pascal front end)
 */

typedef unsigned char Boolean;
typedef unsigned char Char;

typedef struct Fdr* FDR;

typedef enum { foNone, foReset, foRewrite, foAppend, foSeekRead, foSeekWrite, foSeekUpdate } TOpenMode;
typedef void      (*TOpenProc)   (void *, TOpenMode);
typedef int       (*TSelectFunc) (void *, Boolean);
typedef void      (*TSelectProc) (void *, Boolean *, Boolean *, Boolean *);
typedef size_t    (*TReadFunc)   (void *, char *, size_t);
typedef size_t    (*TWriteFunc)  (void *, const char *, size_t);
typedef void      (*TFileProc)   (void *);
typedef TFileProc TFlushProc;
typedef TFileProc TCloseProc;
typedef TFileProc TDoneProc;

#define DefaultOpenProc  ((TOpenProc)  -1)
#define DefaultReadFunc  ((TReadFunc)  -1)
#define DefaultWriteFunc ((TWriteFunc) -1)
#define DefaultFlushProc ((TFlushProc) -1)
#define DefaultCloseProc ((TCloseProc) -1)

typedef long long int UnixTimeType; /* This is hard-coded in the compiler. Do not change here. */
typedef long long int MicroSecondTimeType;

/*
  This is a RECORD_TYPE in the compiler, so it can be a struct. It
  works only if you pass a reference to this, because the string
  field is variable length.
*/
typedef struct {
  int  Capacity;
  int  length;
  char string [1];
} STRING;

/*
  This type must match the compiler definition of BindingType in
  util.c (initialize_world()). The name is copied to heap, so the
  length does not matter for the Run Time System.
*/
typedef struct
{
  char Bound;
  char Force;
  char Extensions_valid;
  char Readable;
  char Writable;
  char Executable;
  char Existing;
  char Directory;
  char Special;
  char SymLink;
  UnixTimeType AccessTime;
  UnixTimeType ModificationTime;
  UnixTimeType ChangeTime;
  int User;
  int Group;
  int Mode;
  int Device;
  int INode;
  Boolean TextBinary;
  int Handle;
  STRING Name;
} GPC_BINDING;

#define FILE_BUFSIZE 1024

struct Fdr {
  unsigned char *FilBuf; /* file buffer                                */
  int    FilSta;         /* status bits; see below                     */
  size_t FilSiz;         /* buffer size: if packed then in bits else bytes */
                         /* THE ABOVE FIELDS ACCESSED BY COMPILER      */
                         /* The fields below are used only by RTS      */

  /* Internal buffering and used for ReadStr/WriteStr */
  unsigned char *BufPtr; /* NOT the Standard Pascal file buffer, that is (*FilBuf) */
  size_t BufSize;
  size_t BufPos;
  int    Flags;

  const char *FilNam;    /* internal name of the file                  */
  const char *ExtNam;    /* external name of the file                  */
  const char *NameToUnlink;
  int   Handle;          /* file handle                                */
  int   RtsSta;          /* run time system status bits                */
  GPC_BINDING *Binding;  /* binding of the file                        */
  const char *BoundName; /* name the binding refers to as a CString    */
  int   BindingChanged;
  void       *PrivateData;
  TOpenProc   OpenProc;
  TSelectFunc SelectFunc;
  TSelectProc SelectProc;
  TReadFunc   ReadFunc;
  TWriteFunc  WriteFunc;
  TFlushProc  FlushProc;
  TCloseProc  CloseProc;
  TDoneProc   DoneProc;
  unsigned char InternalBuffer [FILE_BUFSIZE]; /* NOT the Standard Pascal file buffer, that is (*FilBuf) */
};

#define FDR_Size (sizeof (struct Fdr))

/* File kind: shift counts for the bits in the fourth parameter to _p_initfdr */
#define fkind_TEXT      0
#define fkind_PACKED    1
#define fkind_EXTERN    2
#define fkind_DIRECT    4
#define fkind_BYTE      5
#define fkind_FILENAME  6
#define fkind_BINDABLE  7
#define STD_FILE_FLAGS ((1 << fkind_TEXT) | (1 << fkind_PACKED) | (1 << fkind_EXTERN) | (1 << fkind_BINDABLE))

/* The following defines specify the argument types of the run time system
 * routines _p_read and _p_write that get variable number and type
 * of arguments. */

#define P_S_BYTE             1  /* READ: signed char *                                   */
#define P_S_SHORT            2  /* READ: signed short *                                  */
#define P_S_INT              3  /* READ: signed int *          WRITE: signed int         */
#define P_S_LONG             4  /* READ: signed long *                                   */
#define P_S_LONGLONG         5  /* READ: signed long long *    WRITE: signed long long   */
#define P_U_BYTE             6  /* READ: unsigned char *                                 */
#define P_U_SHORT            7  /* READ: unsigned short *                                */
#define P_U_INT              8  /* READ: unsigned int *        WRITE: unsigned int       */
#define P_U_LONG             9  /* READ: unsigned long *                                 */
#define P_U_LONGLONG        10  /* READ: unsigned long long *  WRITE: unsigned long long */
#define P_SHORT_REAL        11  /* READ: float *                                         */
#define P_REAL              12  /* READ: double *                                        */
#define P_LONG_REAL         13  /* READ: long double *         WRITE: long double        */
#define P_CHAR              14  /* READ: char *                WRITE: int                */
#define P_BOOL              15  /* READ: Boolean *             WRITE: int                */
#define P_ENUM              16  /* READ: char **, int, int*    WRITE: char **, int, int  */
#define P_ANY_STRING        17  /*                             WRITE: (char *), int      */
#define P_STRING            18  /* READ: char *, int *, int                              */
#define P_SHORT_STRING      19  /* READ: char *, char *, int                             */
#define P_FIXED_STRING      20  /* READ: char *, int                                     */
#define P_CSTRING           21  /* READ: char *, int                                     */
#define P_LINE              22  /* READ: void                  WRITE: void               */

#define VAL_MASK (1 << 6)

#define READ_WRITE_STRING_MASK (1 << 7)

/* Keep the following masks as different bits, and bigger than all the type
   specifiers until "End of input masks" */

/* OR to any integer input type specifier to allow the base specifier (n#) */
#define INT_READ_BASE_SPEC_MASK (1 << 9)

/* OR to any integer input type specifier to allow the hex specifier ($) */
#define INT_READ_HEX_MASK       (1 << 10)

/* OR to any integer or real type specifier to require whitespace after
   the number */
#define NUM_READ_CHK_WHITE_MASK (1 << 11)

/* OR to any real type specifier if you don't want to accept Extended
   Pascal real number format extension:
   [ sign ] (digit-sequence [ "." ] | "." fractional-part) [ "e" scale-factor ]
   By default it's accepted. */
#define REAL_READ_ISO7185_ONLY_MASK (1 << 12)

/* End of input masks */

/* Keep the following masks as different bits, and bigger than all the type
   specifiers until "End of output masks" */

/* OR to any type specifier for output with a minimum width.
   If given, another parameter of type int is expected. */
#define FIX_WIDTH_MASK        (1 << 8)

/* OR to any real type specifier for output with a fixed number of decimals
   and a minimum width. Only effective in combination with FIX_WIDTH_MASK.
   If given, one more parameter of type int are expected. */
#define FIX2_REAL_MASK        (1 << 9)

/* OR to any type specifier with FIX_WIDTH_MASK given to cause an error
   when the field width is negative */
#define NEG_WIDTH_ERROR_MASK  (1 << 10)

/* OR to any type specifier with FIX_WIDTH_MASK given to cause left
   adjusted output when the field width is negative */
#define NEG_WIDTH_LEFT_MASK   (1 << 11)

/* OR to any type specifier with FIX_WIDTH_MASK given to cause centered
   adjusted output when the field width is negative */
#define NEG_WIDTH_CENTER_MASK (NEG_WIDTH_ERROR_MASK | NEG_WIDTH_LEFT_MASK)

#define NEG_WIDTH_MASKS (NEG_WIDTH_ERROR_MASK | NEG_WIDTH_LEFT_MASK | NEG_WIDTH_CENTER_MASK)

/* OR to any real type specifier for output without a leading blank */
#define REAL_NOBLANK_MASK     (1 << 12)

/* OR to any real type specifier for output with a capital `E' for the
   exponent (the default is a lower case `e') */
#define REAL_CAPITAL_EXP_MASK (1 << 13)

/* OR to P_ANY_STRING to clip strings that exceed the given width.
   Only efffective in combination with FIX_WIDTH_MASK. */
#define CLIP_STRING_MASK      (1 << 14)

/* OR to the destination string type specifier in _p_writestr if too long
   ouput is to be truncated. Otherwise, an error is generated when the string
   capacity is exceeded. */
#define TRUNCATE_STRING_MASK  (1 << 15)
/* End of output masks */

/* Masks for the bits that must be used to distinguish the type in the read
   and write routines, i.e. the inverse of those masks that can be ORed to
   several types and are therefore not used for type distinction in the
   first place */
#define INPUT_TYPE_MASK  (~( INT_READ_BASE_SPEC_MASK     |\
                             INT_READ_HEX_MASK           |\
                             NUM_READ_CHK_WHITE_MASK     |\
                             REAL_READ_ISO7185_ONLY_MASK ))
#define OUTPUT_TYPE_MASK (~( FIX_WIDTH_MASK              |\
                             FIX2_REAL_MASK              |\
                             NEG_WIDTH_ERROR_MASK        |\
                             NEG_WIDTH_LEFT_MASK         |\
                             NEG_WIDTH_CENTER_MASK       |\
                             REAL_NOBLANK_MASK           |\
                             REAL_CAPITAL_EXP_MASK       |\
                             CLIP_STRING_MASK            |\
                             TRUNCATE_STRING_MASK        ))

/* Run time _p_string() opcodes */
/* Lexicographic string relations: EQ, NE, LT, LE, GE, GT */
#define R_EQ            1
#define R_NE            2
#define R_LT            3
#define R_LE            4
#define R_GE            5
#define R_GT            6
#define R_TRIM          7
#define R_INDEX         8
#define R_SUBSTR        10
#define R_COPY          9
/* String relations padding with spaces: =, !=, <, <=, >=, > */
#define R_eq            11
#define R_ne            12
#define R_lt            13
#define R_le            14
#define R_ge            15
#define R_gt            16

/* rts string argument type codes: flags for CHAR_TYPE parameters */
#define P_STR_FIRST_IS_CHAR     1
#define P_STR_SECOND_IS_CHAR    2

/* Length of the NAME field of the BindingType required record
 *
 * Any length should work, as long as the name fits in. Passed as
 * VAR parameter to the run time system. NAME is copied to HEAP
 * in the RTS (length (name) bytes)
 *
 * This should be a reasonable length.
 */
#define BINDING_NAME_LENGTH 255

/* Implementation dependent length of the canonical-string-type
 * that is returned by the required function date
 *
 * '14 Nov 1993'
 */
#define GPC_DATE_LENGTH 11

/* Implementation dependent length of the canonical-string-type
 * that is returned by the required function time
 *
 * '22:55:26'
 */
#define GPC_TIME_LENGTH  8

#endif /* _RTS_TYPES_H_ */
