/*
Extended Pascal binding routines.

Copyright (C) 1993-2000 Free Software Foundation, Inc.

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

/* BIND (file, b);
 *
 * Attempt to Bind file to b.Name.
 *
 * This routine must not update any fields in b
 *
 * Binding (object) is used to get the binding status info
 * of File after Bind has returned.
 */
void
_p_bind (FDR File, const GPC_BINDING *b)
{
  int permissions = 0, OK, ch;
  UnixTimeType atime = -1, mtime = -1, ctime = -1;
  int device = -1, inode = -1, mode = 0, user = -1, group = -1, onlydir = 0;
  struct stat st;
  char *name, *copy;
  GPC_BINDING *binding;
  int len = b->Name.length;

  if (_p_inoutres) return;

  if (!tst_BINDABLE (File))
    IOERROR_FILE (402, File,); /* `Bind' applied to non-bindable %s */

  if (m_BINDING (File))
    IOERROR_STRING (441, m_BNDNAM (File),); /* File already bound to `%s' */

  if (len < 0)
    IOERROR_FILE (424, File,); /* Invalid string length in `Bind' of `%s' */

  if (len >= BINDING_NAME_LENGTH)
    _p_warning_integer ("external names of bound objects must be shorter than %d characters", (long int) BINDING_NAME_LENGTH);

  /* strip trailing dir separators */
  while (len > 1 && _p_isdirseparator (b->Name.string[len - 1])
#if defined (MSDOS) || defined (_WIN32) || defined (__EMX__)
    && (len > 3 || b->Name.string[1] != ':')
#endif
    ) onlydir = 1, len--;

  /* Copy the name we are binding to (need it null terminated) */
  name = _p_malloc (len + 1);
  _p_strlcpy (name, &b->Name.string[0], len);
  name [len] = 0;

  _p_osdirseparator2slash_cstring (name);
  copy = _p_strdup (name);

  if (m_STATUS (File) != FiNOP)
    /* @@ Should we close it if it is opened instead of this? */
    _p_warning ("file already opened in `Bind'; binding takes effect with the next open");

  /* Unfortunately there is no knowledge if the file will be
   * reset, rewritten or extended, so I added some fields
   * to the bindingtype to let user have a control. */
  OK = TRUE;
  if (
#if defined (MSDOS) || defined (_WIN32) || defined (__EMX__)
           /* Write-only Dos devices */
           !_p_strcasecmp (copy, "prn")  ||
           !_p_strcasecmp (copy, "lpt1") ||
           !_p_strcasecmp (copy, "lpt2") ||
           !_p_strcasecmp (copy, "lpt3") ||
           !_p_strcasecmp (copy, "lpt4") ||
           !_p_strcasecmp (copy, "nul"))
    permissions = 32 | 2;
  /* Read-Write Dos devices */
  else if (!_p_strcasecmp (copy, "aux")  ||
           !_p_strcasecmp (copy, "com1") ||
           !_p_strcasecmp (copy, "com2") ||
           !_p_strcasecmp (copy, "com3") ||
           !_p_strcasecmp (copy, "com4") ||
           !_p_strcasecmp (copy, "con")  ||
#endif
           !_p_strcmp (copy, "") ||
           !_p_strcmp (copy, "-"))
    permissions = 32 | 4 | 2;
  else
    {
      permissions = ((access (copy, X_OK) == 0) * 1)  /* Execute permission? */
                  | ((access (copy, W_OK) == 0) * 2)  /* Write permission? */
                  | ((access (copy, R_OK) == 0) * 4)  /* Read permission? */
                  | ((access (copy, F_OK) == 0) * 8); /* File exists and directories allow file access */
      if (lstat (copy, &st) == 0)
        {
          if (S_ISLNK (st.st_mode))
            {
              permissions |= 64;
              stat (copy, &st);
            }
          atime  = (UnixTimeType) st.st_atime;
          mtime  = (UnixTimeType) st.st_mtime;
          ctime  = (UnixTimeType) st.st_ctime;
          user   = st.st_uid;
          group  = st.st_gid;
          mode   = _p_SysMode2Mode (st.st_mode);
          device = st.st_dev;
          inode  = st.st_ino;
          if (S_ISDIR (st.st_mode))
            {
              permissions = (permissions & ~8) | 16;
              OK = FALSE;
            }
          else if (S_ISCHR (st.st_mode) || S_ISBLK (st.st_mode) || S_ISFIFO (st.st_mode) || S_ISSOCK (st.st_mode))
            permissions = (permissions & ~8) | 32;
        }
      else if (!permissions)
        {
          /* Check for permissions to write/read the directory
             Only check the directory where the unexisting
             file would be created (not /tmp/non1/non2/non3) */
          char *slash = _p_strrscan (copy, '/'); /* dir separators have already been changed to slashes */
          if (!slash)
            {
              /* Nonexisting file in current directory */
              ch = '.';
              copy [0] = '.';
              copy [1] = 0;
            }
          else
            {
              ch = slash [1];
              if (slash == copy)
                slash [1] = 0; /* root directory */
              else
                slash [0] = 0; /* get rid of the file component, leave the path */
            }
          if (ch && /* not /directory/name/ending/with/slash/ */
              access (copy, W_OK) == 0 &&
              stat (copy, &st) == 0 &&
              S_ISDIR (st.st_mode))
            permissions = 2; /* Only write permissions are valid because the file did not exist. */
          else
            OK = FALSE; /* path is not valid */
        }
    }

  if (onlydir && !(permissions & 16))
    {
      permissions = 0;
      OK = FALSE;
    }

  _p_dispose (copy);

  if (!(OK || b->Force))
    {
      _p_dispose (name);
      return;
    }
  _p_inittfdd (File);
  m_BNDNAM (File) = name;
  m_BINDING (File) = binding = (GPC_BINDING *) _p_malloc (sizeof (GPC_BINDING));
  m_BNDCHG (File) = 1;
  _p_move (b, binding, sizeof (GPC_BINDING));
  binding->Extensions_valid = TRUE;
  binding->Readable         = !!(permissions&4);
  binding->Writable         = !!(permissions&2);
  binding->Executable       = !!(permissions&1);
  binding->Existing         = !!(permissions&8);
  binding->Directory        = !!(permissions&16);
  binding->Special          = !!(permissions&32);
  binding->SymLink          = !!(permissions&64);
  binding->AccessTime       = atime;
  binding->ModificationTime = mtime;
  binding->ChangeTime       = ctime;
  binding->User             = user;
  binding->Group            = group;
  binding->Mode             = mode;
  binding->Device           = device;
  binding->INode            = inode;
  binding->Bound            = TRUE; /* Standard flag */
}

void
_p_clearbinding (GPC_BINDING *b)
{
  b->Bound            = FALSE;
  b->Force            = FALSE;
  b->Extensions_valid = FALSE;
  b->Readable         = FALSE;
  b->Writable         = FALSE;
  b->Executable       = FALSE;
  b->Existing         = FALSE;
  b->Directory        = FALSE;
  b->Special          = FALSE;
  b->SymLink          = FALSE;
  b->AccessTime       = -1;
  b->ModificationTime = -1;
  b->ChangeTime       = -1;
  b->User             = -1;
  b->Group            = -1;
  b->Mode             = 0;
  b->Device           = -1;
  b->INode            = -1;
  b->TextBinary       = FALSE;
  b->Handle           = -1;
  b->Name.length      = 0;
  b->Name.string[0]   = 0;
}

void
_p_binding (const FDR File, GPC_BINDING *b)
{
  int len;

  _p_clearbinding (b);

  if (_p_inoutres) return;
  if (!tst_BINDABLE (File))
    IOERROR_FILE (403, File,); /* `Binding' applied to non-bindable %s */

  if (!m_BINDING (File)) return;

  /* Copy all fields except the Name field */
  *b = *m_BINDING (File);
  len = strlen (m_BNDNAM (File));
  if (len >= BINDING_NAME_LENGTH)
    {
      len = BINDING_NAME_LENGTH-1;
      _p_warning_integer ("bound name truncated to %d characters in `Binding'", (long int) len);
    }

  /* Now copy the name, does not matter if null terminated or not */
  b->Name.length = len;
  _p_strlcpy (&b->Name.string[0], m_BNDNAM (File), len);
}

void
_p_unbind (FDR File)
{
  if (_p_inoutres) return;
  if (!tst_BINDABLE (File))
    IOERROR_FILE (404, File,); /* `Unbind' applied to non-bindable %s */
  if (m_BINDING (File))
    {
      DO_RETURN_ADDRESS (_p_close (File));
      _p_inittfdd (File);
      if (_p_inoutres) return;
      _p_dispose (m_BNDNAM (File));
      _p_dispose (m_BINDING (File));
      m_BINDING (File) = NULL;
      m_EXTNAM (File) = NULL;
      m_BNDCHG (File) = 1;
    }
}

void _p_assign_tfdd (FDR File, TOpenProc OpenProc, TSelectFunc SelectFunc, TSelectProc SelectProc, TReadFunc ReadFunc,
                     TWriteFunc WriteFunc, TFlushProc FlushProc, TCloseProc CloseProc, TDoneProc DoneProc, void *PrivateData)
{
  _p_internal_assign (File, "", 0);
  _p_set_tfdd (File, OpenProc, SelectFunc, SelectProc, ReadFunc, WriteFunc, FlushProc, CloseProc, DoneProc, PrivateData);
}

void _p_set_tfdd (FDR File, TOpenProc OpenProc, TSelectFunc SelectFunc, TSelectProc SelectProc, TReadFunc ReadFunc,
                  TWriteFunc WriteFunc, TFlushProc FlushProc, TCloseProc CloseProc, TDoneProc DoneProc, void *PrivateData)
{
  File->OpenProc    = OpenProc;
  File->SelectFunc  = SelectFunc;
  File->SelectProc  = SelectProc;
  File->ReadFunc    = ReadFunc;
  File->WriteFunc   = WriteFunc;
  File->FlushProc   = FlushProc;
  File->CloseProc   = CloseProc;
  File->DoneProc    = DoneProc;
  File->PrivateData = PrivateData;
}

void _p_get_tfdd (FDR File, TOpenProc *OpenProc, TSelectFunc *SelectFunc, TSelectProc *SelectProc, TReadFunc *ReadFunc,
                  TWriteFunc *WriteFunc, TFlushProc *FlushProc, TCloseProc *CloseProc, TDoneProc *DoneProc, void **PrivateData)
{
  if (OpenProc)    *OpenProc    = File->OpenProc;
  if (SelectFunc)  *SelectFunc  = File->SelectFunc;
  if (SelectProc)  *SelectProc  = File->SelectProc;
  if (ReadFunc)    *ReadFunc    = File->ReadFunc;
  if (WriteFunc)   *WriteFunc   = File->WriteFunc;
  if (FlushProc)   *FlushProc   = File->FlushProc;
  if (CloseProc)   *CloseProc   = File->CloseProc;
  if (DoneProc)    *DoneProc    = File->DoneProc;
  if (PrivateData) *PrivateData = File->PrivateData;
}
