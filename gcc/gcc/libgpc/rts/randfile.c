/*
Random access file routines.

Copyright (C) 1991-2000 Free Software Foundation, Inc.

Authors: Jukka Virtanen <jtv@hut.fi>
         Frank Heckenbach <frank@pascal.gnu.de>

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

#include "rts.h"
#include "fdr.h"

/* NOTE:
 *   Extended Pascal defined the following operations only to
 * Direct access file types:
 *
 *   SeekRead, SeekWrite, SeekUpdate, Empty, Position, LastPosition
 *   and Update (in not writing to end of file)
 *
 * Direct access file is defined by: File [ indextype ] of type
 * (the ord (a) in assertions means the smallest value of indextype)
 *
 * However, GPC does not currently implement direct access files, and
 * anyhow maybe we should allow the operations also to other
 * files capable of seeking. These non-direct access files may be
 * thought of the following direct access file type:
 *
 * type Natural = 0 .. MaxInt;
 *      gpcfiles = File [ Natural ] of <type>
 *
 * DefineSize is a GPC extension.
 */

static int
_p_direct_warn (FDR File, int n)
{
  if (!tst_DIRECT (File))
    {
      if (_p_force_direct_files)
        IOERROR_FILE (n, File, 1);
      else
        _p_warning_string (_p_errmsg (n), m_NAM (File));
    }
  return 0;
}

void
_p_truncate (FDR File)
{
  FileSizeType position, ByteNum;
  if (_p_inoutres) return;
  if (m_STATUS (File) == FiNOP)
    IOERROR_FILE (407, File,); /* % has not been opened */
  else if (is_RONLY (File))
    IOERROR_FILE (438, File,); /* `Truncate' or `DefineSize' applied to read only % */
  position = _p_position (File);
  if (_p_inoutres) return;
  _p_clearbuffer (File);
/*@@avoid superfluous warning under m68-linux (gcc-2.8.1 bug?)*/ ByteNum = 0;
  ByteNum = BYTENUM (File, position);
  D (3, fprintf (stderr, "truncating file to %Ld bytes\n", ByteNum));
#ifdef HAVE_FTRUNCATE
  if (ftruncate (File->Handle, /*@@*/ (size_t) ByteNum) < 0)
    IOERROR_FILE (425, File,); /* truncation failed for % */
#else
  /* Maybe this is not worth reporting?
     The call is not in 10206 Pascal anyway...
     @@ The way to go without FTRUNCATE might be to copy the
     POSITION first elements of the file to a temp file,
     then do some file renaming, but I won't do it now. */
  _p_warning ("ftruncate() needed by _p_truncate()");
#endif
}

/* DEFINESIZE (This is a GPC extension)
 *
 * May be applied only to random access files and files opened
 * for writing.
 *
 * Define files size as count of its component type units
 */
void
_p_definesize (FDR File, FileSizeType NewSize)
{
  if (_p_inoutres) return;
  _p_seekwrite (File, NewSize);
  if (_p_inoutres) return;
  _p_truncate (File);
  if (_p_inoutres) return;
}

void
_p_seekall (FDR File, FileSizeType NewPlace)
{
  if (_p_inoutres) return;
  if (is_WRITABLE (File))
    {
      if (is_READABLE (File))
        {
          _p_seekupdate (File, NewPlace);
          if (_p_inoutres) return;
        }
      else
        {
          _p_seekwrite (File, NewPlace);
          if (_p_inoutres) return;
        }
    }
  else
    {
      _p_seekread (File, NewPlace);
      if (_p_inoutres) return;
    }
}

/* SEEKREAD
 * pre-assertion: (neither f0.L nor f0.R is undefined) and
 *   (0 <= ord (n) - ord (a) <= length (f0.L~f0.R))
 * post-assertion: (f.M = Inspection) and (f.L~f.R = f0.L~f0.R) and
 *   (if length (f0.L~f0.R) > ord (n) - ord (a) then
 *     ((length (f.L) = ord (n) - ord (a)) and
 *      (f^ = f.R.first))
 *   else
 *     ((f.R = S () and f^ is undefined)))
 *
 * NEWPLACE is an offset from ZERO to the correct location.
 */
void
_p_seekread (FDR File, FileSizeType NewPlace)
{
  if (_p_direct_warn (File, 591) || _p_inoutres) /* Direct access routine `SeekRead' applied to non-direct % */
    return;
  if (is_WONLY (File))
    IOERROR_FILE (426, File,); /* `SeekRead' to write only % */
  else if (NewPlace < 0)
    IOERROR_FILE (410, File,); /* attempt to access elements before beginning of random access % */
  if (m_STATUS (File) == FiNOP)
    {
      _p_open (File, foSeekRead);
      if (_p_inoutres) return;
    }
  if (_p_seek (File, NewPlace, SEEK_SET) < 0)
    IOERROR_FILE (427, File,); /* SeekRead seek failed on % */
  /* Change the current status of file to INSPECTION */
  CLR_STATUS (File, FiANY);
  SET_STATUS (File, FiORE);
  clr_EOF (File);
  set_LGET (File);
}

/* SEEKWRITE
 * pre-assertion: (neither f0.L nor f0.R is undefined) and
 *   (0 <= ord (n) - ord (a) <= length (f0.L~f0.R))
 * post-assertion: (f.M = Generation) and (f.L~f.R = f0.L~f0.R) and
 *   (length (f.L) = ord (n) - ord (a)) and (f^ is undefined)
 *
 * Note: this definition DOES NOT WRITE anything. It just moves the
 * file pointer and changes the MODE to GENERATION.
 *
 * NEWPLACE is an offset from ZERO to the correct location.
 */
void
_p_seekwrite (FDR File, FileSizeType NewPlace)
{
  if (_p_direct_warn (File, 592) || _p_inoutres) /* Direct access routine `SeekWrite' applied to non-direct % */
    return;
  if (is_RONLY (File))
    IOERROR_FILE (411, File,); /* attempt to modify read only % */
  else if (NewPlace < 0)
    IOERROR_FILE (410, File,); /* attempt to access elements before beginning of random access % */
  if (m_STATUS (File) == FiNOP)
    {
      _p_open (File, foSeekWrite);
      if (_p_inoutres) return;
    }
  if (_p_seek (File, NewPlace, SEEK_SET) < 0)
    IOERROR_FILE (429, File,); /* SeekWrite seek failed on % */
  /* Change the mode to generation */
  CLR_STATUS (File, FiANY);
  SET_STATUS (File, FiWRI);
}

/* SEEKUPDATE
 * pre-assertion: (neither f0.L nor f0.R is undefined) and
 *   (0 <= ord (n) - ord (a) <= length (f0.L~f0.R))
 * post-assertion: (f.M = Update) and (f.L~f.R = f0.L~f0.R) and
 *   (if length (f0.L~f0.R) > ord (n) - ord (a) then
 *     ((length (f.L) =  ord (n) - ord (a)) and
 *      (f^ = f.R.first))
 *   else
 *     ((f.R = S ()) and (f^ is undefined)))
 *
 * The (only) difference with SEEKREAD is that this leaves f.M to
 * UPDATE mode.
 */
void
_p_seekupdate (FDR File, FileSizeType NewPlace)
{
  if (_p_direct_warn (File, 593) || _p_inoutres) /* Direct access routine `SeekUpdate' applied to non-direct % */
    return;
  if (is_RONLY (File) || is_WONLY (File))
    IOERROR_FILE (430, File,); /* `SeekUpdate' to read-only or write-only % */
  else if (NewPlace < 0)
    IOERROR_FILE (410, File,); /* attempt to access elements before beginning of random access % */
  if (m_STATUS (File) == FiNOP)
    {
      _p_open (File, foSeekUpdate);
      if (_p_inoutres) return;
    }
  if (_p_seek (File, NewPlace, SEEK_SET) < 0)
    IOERROR_FILE (431, File,); /* `SeekUpdate' seek failed on % */
  CLR_STATUS (File, FiANY);
  if (!(is_RONLY (File) || is_WONLY (File)))
    SET_STATUS (File, FiRND);
  clr_EOF (File);
  set_LGET (File);
}

/* UPDATE
 * pre-assertion: (f0.M = Generation or f0.M = Update) and
 *   (neither f0.L nor f0.R is undefined) and
 *   (f is a direct access file type) and
 *   (f0^ is not undefined)
 * post-assertion: (f.M = f0.M) and (f.L = f0.L) and
 *   (if f0.R = S () then
 *     (f.R = S (f0^))
 *   else
 *     (f.R = S (f0^)~f0.R.rest)) and
 *   (f^ = f0^)
 *
 * i.e. Write the stuff in, and leave it also in the file buffer.
 * don't advance the file pointer from the pre-assert state!
 */
void
_p_update (FDR File)
{
  int is_random;
  if (_p_direct_warn (File, 595) || _p_inoutres) /* Direct access routine `Update' applied to non-direct % */
    return;
  /* If the file buffer contents is lazy, validate it */
  if (tst_LGET (File))
    {
      clr_LGET (File);
      _p_get (File);
      if (_p_inoutres) return;
    }
#if 0
  /* @@ Ooops: Currently assigning a value to a file buffer
     does not clear the FiUnd bit in the status word.
     Disable this check -> Undefined file buffers may be written with
     update... */
  if (tst_UND (File))
    IOERROR_FILE (439, File,); /* `Update' with an undefined file buffer in % */
#endif
  is_random = TST_STATUS (File, FiRND);
  if (is_random)
    {
      /* Change the mode to generation, prevents implicit GET
       * Yes, PUT in UPDATE mode gets the next element by default.
       */
      CLR_STATUS (File, FiANY);
      SET_STATUS (File, FiWRI);
    }
  _p_put (File); /* Write to the current location. _p_put does not clobber file buffer. */
  if (_p_inoutres) return;
  if (is_random)
    {
      /* Change the mode back to random access */
      CLR_STATUS (File, FiANY);
      if (!(is_RONLY (File) || is_WONLY (File)))  SET_STATUS (File, FiRND);
    }
  clr_UND (File); /* The file buffer is still f0^ */
  /* Seek back to the place where we were before the PUT.
     It's m_SIZ (File) before the place we are now at */
  if (_p_seek (File, -1, SEEK_CUR) < 0)
    IOERROR_FILE (433, File,); /* `Update' failed to reset the position of % */
}

/* LASTPOSITION: LastPosition (f) = succ (a, length (f.L~f.R) - 1) */
FileSizeType
_p_lastposition (FDR File)
{
  return _p_getsize (File) - 1;
}

/* EMPTY: Returns True if file is Empty, otherwise False */
int
_p_empty (const FDR File)
{
  if (_p_direct_warn (File, 594) || _p_inoutres) /* Direct access routine `Empty' applied to non-direct % */
    return 1;
  return _p_getsize (File) == 0;
}

#ifdef HAVE_LLSEEK
extern loff_t llseek (int, loff_t, int);
#endif

static FileSizeType
do_seek (int fildes, FileSizeType offset, int whence)
{
  #ifdef HAVE_LLSEEK
  FileSizeType result = llseek (fildes, offset, whence);
  if (result >= 0) return result;
  #endif
  return lseek (fildes, offset, whence);
}

FileSizeType
_p_getsize (FDR File)
{
  FileSizeType OrigPos, LastPos = - 1;
  if (_p_direct_warn (File, 590) || _p_inoutres) /* Direct access routine `GetSize' applied to non-direct % */
    return 0;
  if (m_STATUS (File) == FiNOP)
    IOERROR_FILE (407, File, 0); /* % has not been opened */
  _p_flushbuffer (File);
  OrigPos = do_seek (File->Handle, 0, SEEK_CUR);
  if (OrigPos >= 0)
    {
      LastPos = do_seek (File->Handle, 0, SEEK_END);
      do_seek (File->Handle, OrigPos, SEEK_SET);
    }
  if (LastPos >= 0)
    return NUMBYTE (File, LastPos);
  else
    IOERROR_FILE (446, File, 0); /* cannot get the size of % */
}

/* POSITION
 * Position (f) = succ (a, length (f.L));
 *
 * This is the element number always counting from ZERO
 *
 * (Since the run time system does not know the low bound
 *  of the direct access file type)
 *
 * The returned value is an offset from A, so the compiler needs to
 * adjust the value before it is returned to the user.
 */
FileSizeType
_p_position (FDR File)
{
  FileSizeType NumBytes, pos;
  if (_p_direct_warn (File, 596) || _p_inoutres) /* Direct access routine `Position' applied to non-direct % */
     return 0;
  if (m_STATUS (File) == FiNOP)
    IOERROR_FILE (407, File, 0); /* % has not been opened */
  NumBytes = do_seek (File->Handle, 0, SEEK_CUR);
  if (NumBytes < 0) /* Error */
    IOERROR_FILE (417, File, 0); /* `FilePos'' could not get file position of % */
  if (File->BufPos < File->BufSize)
    NumBytes -= File->BufSize - File->BufPos;
/*@@avoid superfluous warning under m68-linux (gcc-2.8.1 bug?)*/ pos = 0;
  pos = NUMBYTE (File, NumBytes);
  if (!tst_UND (File) && !tst_LGET (File))
    pos--;
  return pos;
}

/* Move the file pointer to the requested Pascal record of the file.
   record specifies how much to move, negative is backward, positive is
   forward. whence corresponds to the `whence' parameter to `lseek'. */
FileSizeType
_p_seek (FDR File, FileSizeType record, int whence)
{
  FileSizeType bytenum, result;
  _p_clearbuffer (File);
  set_LGET (File);
/*@@avoid superfluous warning under m68-linux (gcc-2.8.1 bug?)*/ bytenum = 0;
  if (whence != SEEK_SET)
    bytenum = record * m_SIZ (File);
  else
    bytenum = BYTENUM (File, record);
  D (3, fprintf (stderr, "seek to element %Ld, byte %Ld, whence=%d\n", record, bytenum, whence));
  result = do_seek (File->Handle, bytenum, whence);
  return result;
}
