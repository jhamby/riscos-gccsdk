/*
File Descriptor Record definition

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

/* Note: No files are actually packed */

/* FilSta bit definitions */

#define FiUnd      (1 << 0)  /* File buffer is totally undefined */
#define FiEof      (1 << 2)  /* End of file is true */
#define FiEln      (1 << 3)  /* End of line is true. Text files only */
#define FiTxt      (1 << 4)  /* It's a text file */
#define FiExt      (1 << 5)  /* External file */
#define FiExtB     (1 << 6)  /* External or bound file */
#define FiPck      (1 << 7)  /* Packed file */
#define FiUnread   (1 << 8)  /* Nothing read yet */
#define FiEofOK    (1 << 10) /* Internal flag if FiUnd is set: Accept EOF without EOLn */
#define FiDacc     (1 << 11) /* This is a direct access file */
#define FiLget     (1 << 12) /* Must do a get before buffer reference (lazy I/O) */
#define FiByte     (1 << 13) /* File buffer is actually one byte size */
#define FiFileName (1 << 14) /* Derive external file name from internal file name */
#define FiBindable (1 << 15) /* File is bindable */

/* RtsSta bit definitions */

#define FiNOP    0 /* File has not been opened */
#define FiRONLY  (1 << 0) /* File opened but is read only */
#define FiORE    (1 << 1) /* File open for reading */
#define FiWRI    (1 << 2) /* File open for writing */
#define FiRND    (1 << 3) /* File open for random access */
#define FiWONLY  (1 << 4) /* File opened but is write only */
/* RtsSta: Device specific bits */
#define FiTTY    (1 << 10) /* TTY: flush output before GET */
#define FiSEEK   (1 << 12) /* File is seekable */

/* m_STATUS bits altered by Seek* routines */
#define FiANY (FiORE | FiWRI | FiRND)

/* m_FILSTA bits that must be preserved by the run time system
 * when files are manipulated. */
#define STATUS_KEEP (FiTxt | FiExt | FiExtB | FiPck | FiDacc | FiByte | FiFileName | FiBindable)

/* some macro definitions */

#define m_FILBUF(file)  (*((file)->FilBuf))
#define m_FILBPTR(file) ((file)->FilBuf)
#define m_SIZ(file)     ((file)->FilSiz)
#define m_NAM(file)     ((file)->FilNam)
#define m_EXTNAM(file)  ((file)->ExtNam)
#define m_FILSTA(file)  ((file)->FilSta)
#define m_STATUS(file)  ((file)->RtsSta)
#define m_BINDING(file) ((file)->Binding)
#define m_BNDCHG(file)  ((file)->BindingChanged)
#define m_BNDNAM(file)  ((file)->BoundName)

#define fil_tst(File, bit) (!!(m_FILSTA (File) & (bit)))
#define fil_set(File, bit) (m_FILSTA (File) |= (bit))
#define fil_clr(File, bit) (m_FILSTA (File) &= ~(bit))

#define tst_UND(file)      fil_tst (file, FiUnd)
#define tst_EOF(file)      fil_tst (file, FiEof)
#define tst_EOLN(file)     fil_tst (file, FiEln)
#define tst_EOFOK(file)    fil_tst (file, FiEofOK)
#define tst_TXT(file)      fil_tst (file, FiTxt)
#define tst_EXT(file)      fil_tst (file, FiExt)
#define tst_EXTB(file)     fil_tst (file, FiExtB)
#define tst_PCK(file)      fil_tst (file, FiPck)
#define tst_UNREAD(file)   fil_tst (file, FiUnread)
#define tst_DIRECT(file)   fil_tst (file, FiDacc)
#define tst_LGET(file)     fil_tst (file, FiLget)
#define tst_BYTE(file)     fil_tst (file, FiByte)
#define tst_FILENAME(file) fil_tst (file, FiFileName)
#define tst_BINDABLE(file) fil_tst (file, FiBindable)

#define set_UND(file)      fil_set (file, FiUnd)
#define set_EOF(file)      fil_set (file, FiEof)
#define set_EOLN(file)     fil_set (file, FiEln)
#define set_EOFOK(file)    fil_set (file, FiEofOK)
#define set_TXT(file)      fil_set (file, FiTxt)
#define set_EXT(file)      fil_set (file, FiExt)
#define set_EXTB(file)     fil_set (file, FiExtB)
#define set_PCK(file)      fil_set (file, FiPck)
#define set_UNREAD(file)   fil_set (file, FiUnread)
#define set_DIRECT(file)   fil_set (file, FiDacc)
#define set_LGET(file)     fil_set (file, FiLget)
#define set_BYTE(file)     fil_set (file, FiByte)
#define set_FILENAME(file) fil_set (file, FiFileName)
#define set_BINDABLE(file) fil_set (file, FiBindable)

#define clr_UND(file)      fil_clr (file, FiUnd)
#define clr_EOF(file)      fil_clr (file, FiEof)
#define clr_EOLN(file)     fil_clr (file, FiEln)
#define clr_EOFOK(file)    fil_clr (file, FiEofOK)
#define clr_TXT(file)      fil_clr (file, FiTxt)
#define clr_EXT(file)      fil_clr (file, FiExt)
#define clr_EXTB(file)     fil_clr (file, FiExtB)
#define clr_PCK(file)      fil_clr (file, FiPck)
#define clr_UNREAD(file)   fil_clr (file, FiUnread)
#define clr_DIRECT(file)   fil_clr (file, FiDacc)
#define clr_LGET(file)     fil_clr (file, FiLget)
#define clr_BYTE(file)     fil_clr (file, FiByte)
#define clr_FILENAME(file) fil_clr (file, FiFileName)
#define clr_BINDABLE(file) fil_clr (file, FiBindable)

/* Test, clear and set status bits in file status word */
#define TST_STATUS(file, bit) (m_STATUS (file) & (bit))
#define CLR_STATUS(file, bit) (m_STATUS (file) &= ~(bit))
#define SET_STATUS(file, bit) (m_STATUS (file) |= (bit))

/* These fields are never cleared in an open file */
#define tst_TTY(file)   TST_STATUS (file, FiTTY)
#define tst_SEEK(file)  TST_STATUS (file, FiSEEK)

#define set_TTY(file)   SET_STATUS (file, FiTTY)
#define set_SEEK(file)  SET_STATUS (file, FiSEEK)

/* well... almost never... (only when restoring stdin)-: */
#define clr_TTY(file)   CLR_STATUS (file, FiTTY)
#define clr_SEEK(file)  CLR_STATUS (file, FiSEEK)

#define is_READABLE(file) TST_STATUS (file, FiORE | FiRONLY | FiRND)
#define is_WRITABLE(file) TST_STATUS (file, FiWRI | FiWONLY | FiRND)

#define is_RONLY(file) TST_STATUS (file, FiRONLY)
#define is_WONLY(file) TST_STATUS (file, FiWONLY)

/* BYTENUM calculates the byte where NumE'th element starts in file.
   First possible element is 1. */
#define BYTENUM(File, NumE) (tst_PCK (File) ? ((NumE) - 1) / (8 / m_SIZ (File)) : ((NumE) * m_SIZ (File)))

/* Opposite of BYTENUM. Calculates the number of the Pascal file component
   the byte is in. */
#define NUMBYTE(File, NumBytes) (tst_PCK (File) ? ((8 / m_SIZ (File)) * NumBytes) : (NumBytes / m_SIZ (File)))
