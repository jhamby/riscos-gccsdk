/*
Generic file handling routines and their support.

Copyright (C) 1991-2000 Free Software Foundation, Inc.

Authors: Jukka Virtanen <jtv@hut.fi>
         Frank Heckenbach <frank@pascal.gnu.de>
         Peter Gerwinski <peter@gerwinski.de>

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

int _p_filemode = 2;

/* Program standard input, output and error */
struct Fdr
  _p_stdin  = { 0, 0, 0, 0, 0, 0, 0, "uninitialized Input", 0, 0,
                0, FiNOP, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, { 0 } },
  _p_stdout = { 0, 0, 0, 0, 0, 0, 0, "uninitialized Output", 0, 0,
                0, FiNOP, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, { 0 } },
  _p_stderr = { 0, 0, 0, 0, 0, 0, 0, "uninitialized StdErr", 0, 0,
                0, FiNOP, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, { 0 } };

typedef struct FDRList
{
  struct FDRList *Next;
  FDR Item;
} TFDRList;

/* FDR list head pointer.
   Add an FDR to the list when reset/rewritten, remove it on close.
   List is needeed to flush buffered output to terminal
    1) when program crashes, dump everything before giving error message
    2) when something is read from terminal opened as a TEXT file
       (need to flush output from other TTY files before get)
*/
static TFDRList *_p_first_fdr = NULL;

void
_p_initialize_std_files ()
{
  static int init_std_files_done = 0;
  if (init_std_files_done) return;
  init_std_files_done++;
  _p_initfdr (&_p_stderr, "StdErr", 8, STD_FILE_FLAGS);
  _p_rewrite (&_p_stderr, NULL, 0);
  _p_initfdr (&_p_stdout, "Output", 8, STD_FILE_FLAGS);
  _p_rewrite (&_p_stdout, NULL, 0);
  _p_initfdr (&_p_stdin,  "Input",  8, STD_FILE_FLAGS);
  _p_reset   (&_p_stdin,  NULL, 0);
}

void
_p_clearbuffer (FDR File)
{
  File->BufSize = 0;
  File->BufPos = 0;
}

void
_p_flushbuffer (FDR File)
{
  /* empty -- will be needed when we add write buffers */
  (void) File;
}

static inline size_t
_p_f_read (FDR File, char *Buf, size_t Size)
{
  ssize_t result = read (File->Handle, Buf, Size > SSIZE_MAX ? SSIZE_MAX : Size);
  if (result < 0)
    {
      _p_inoutres = 464; /* error when reading from %s */
      return 0;
    }
  /* If we are reading from the options file and this is the end of
     _p_current_stdin, continue with the original stdin instead of quitting. */
  if (Size != 0 && result == 0 && _p_current_stdin && File != _p_current_stdin
      && File->Handle == _p_current_stdin->Handle)
    {
      _p_close (_p_current_stdin);
      _p_donefdr (_p_current_stdin);
      _p_dispose (_p_current_stdin);
      _p_current_stdin = NULL;
      File->Handle = 0; /* real stdin */
      _p_checkfiletype (File, TRUE);
      return _p_f_read (File, Buf, Size); /* read again */
    }
  return result;
}

static inline size_t
_p_f_write (int Handle, const char *Buf, size_t Size)
{
  ssize_t result = write (Handle, Buf, Size > SSIZE_MAX ? SSIZE_MAX : Size);
  if (result < 0)
    {
      if (errno != EPIPE && errno != ENOSPC && errno != EINTR)
        _p_inoutres = 466; /* error when writing to %s */
      return 0;
    }
  return result;
}

static inline void
_p_f_flush (int Handle)
{
  #ifdef HAVE_FSYNC
  fsync (Handle);
  #endif
}

/* Routine to flush files from Pascal */
void
_p_flush (FDR File)
{
  if (_p_inoutres) return;
  _p_flushbuffer (File);
  if (is_WRITABLE (File) && File->FlushProc)
    {
      if (File->FlushProc == DefaultFlushProc)
        _p_f_flush (File->Handle);
      else
        DO_RETURN_ADDRESS (File->FlushProc (File->PrivateData));
    }
}

void
_p_fflush (Boolean only_ttys)
{
  TFDRList *scan;
  /* flush buffers to synchronize output messages */
  for (scan = _p_first_fdr; scan; scan = scan->Next)
    if ((m_STATUS (scan->Item) & FiWRI) && (!only_ttys || tst_TTY (scan->Item)))
      _p_flushbuffer (scan->Item);
}

void
_p_done_files ()
{
  TFDRList *scan, *next;
  _p_fflush (FALSE);
  /* clean up all open files */
  scan = _p_first_fdr;
  while (scan)
    {
      next = scan->Next; /* scan->Next will be destroyed by _p_donefdr () */
      _p_donefdr (scan->Item);
      scan = next;
    }
}

int
_p_filehandle (const FDR File)
{
  return File->Handle;
}

void
_p_inittfdd (FDR File)
{
  File->PrivateData = NULL;
  File->OpenProc    = DefaultOpenProc;
  File->SelectFunc  = NULL;
  File->SelectProc  = NULL;
  File->ReadFunc    = DefaultReadFunc;
  File->WriteFunc   = DefaultWriteFunc;
  File->FlushProc   = DefaultFlushProc;
  File->CloseProc   = DefaultCloseProc;
  File->DoneProc    = NULL;
}

static void
_p_reinitfdr (FDR File)
{
  fil_clr (File, ~STATUS_KEEP);
  set_UND (File); /* Mark the file buffer contents undefined */
  File->BufPtr = File->InternalBuffer;
  File->NameToUnlink = NULL;
  m_STATUS (File) = FiNOP;
  File->Handle = - 1;
}

/* Name :internal name in program
   Size : file buffer size; in bits, if packed, else in bytes
   flags: see types.h */
void
_p_initfdr (FDR File, const char *Name, int Size, int flags)
{
  if (!Name)
    _p_internal_error (925); /* File has no internal name */
  m_FILSTA (File) = 0;
  _p_reinitfdr (File);
  if (flags & (1 << fkind_TEXT))
    set_TXT (File);
  if (flags & (1 << fkind_PACKED))
    set_PCK (File);
  if (flags & (1 << fkind_EXTERN))
    set_EXT (File);
  if (flags & (1 << fkind_DIRECT))
    set_DIRECT (File);
  if (flags & (1 << fkind_BYTE))
    set_BYTE (File);  /* Unused */
  if (flags & (1 << fkind_FILENAME))
    set_FILENAME (File);
  if (flags & (1 << fkind_BINDABLE))
    set_BINDABLE (File);
  _p_inittfdd (File);
  m_BINDING (File) = NULL;
  m_BNDNAM  (File) = NULL;
  m_EXTNAM  (File) = NULL;
  if (Size)
    {
      m_SIZ (File) = Size;
      if (tst_PCK (File))
        {
          /* Convert to unpacked file, since we don't yet support
             packed files. This may be done even if the compiler
             thinks we support them :-) */
          m_SIZ (File) = (m_SIZ (File) + 7) / 8;
          clr_PCK (File);
        }
#if 1
      /* Allocate file buffer */
      m_FILBPTR (File) = _p_malloc (m_SIZ (File));
#else
      /* @@ Why was this? */
      /* Always allocate at least one word for the file buffer */
      {
        int s = m_SIZ (File);
        if (s < BYTES_PER_WORD)
          s = BYTES_PER_WORD;
        /* Allocate file buffer */
        m_FILBPTR (File) = _p_malloc (s);
      }
#endif /* 1 */
    }
  m_NAM (File) = Name;
  m_BNDCHG (File) = 0;
}

static int
isStdFile (FDR File)
{
  return File == &_p_stdin  ||
         File == &_p_stdout ||
         File == &_p_stderr;
}

static const char *
_p_nameit (FDR File, TOpenMode mode)
{
  int    tty;
  int    n, in, out, l;
  TFileAssociation *ap;
  char  *b, buf[512], *tmp;

  if (_p_inoutres) return "";
  if (!tst_EXTB (File))
    {
      b = _p_get_temp_file_name_cstring ();
      D (2, fprintf (stderr, "Opening internal file %s\n", b));
      return b;
    }
  for (ap = _p_FileAssociation; ap; ap = ap->Next)
    if (_p_strcasecmp (m_NAM (File), ap->int_name) == 0)
      {
        ap->int_name = "?KukkuuRESET?"; /* Allow Close (a); Reset (a) to access next one */
        m_EXTNAM (File) = _p_strdup (ap->ext_name);
        return m_EXTNAM (File);
      }
  if (isStdFile (File))
    return NULL;

  if (tst_FILENAME (File))
    {
      /* Derive the external file name from the internal one without
         asking the user. */
      _p_osdirseparator2slash_cstring (tmp = _p_strlower (_p_strdup (m_NAM (File))));
      return m_EXTNAM (File) = tmp;
    }

  /* Try to write filename prompts to /dev/tty and try to read responces
     from there also, to avoid mungling with stdin & stdout.

     However, if everything fails, try stdin & stdout, if they don't
     work, abort. You can also use -a switch to associate internal names
     to external ones. It only needs to be documented... */

  if ((tty = open ("/dev/tty", O_RDWR)) < 0)
    {
      _p_warning ("failed to open terminal for file name read, using stdin & stdout");
      in  = 0;
      out = 1;
    }
  else
    in = out = tty;

  sprintf (buf, "%s file `%s': ",
           (mode == foReset   || mode == foSeekRead)  ? "Input"  :
           (mode == foRewrite || mode == foSeekWrite) ? "Output" :
           (mode == foSeekUpdate) ? "Input/Output" : "Extend",
           m_NAM (File));
  l = _p_strlen (buf);
  if ((n = write (out, buf, l)) != l)
    {
      if (out != 1)
        _p_warning ("writing file name prompt to /dev/tty failed, using stdout");
      if (out == 1 || (n = write (1, buf, l)) < 0)
        {
          if (tty >= 0) close (tty);
          IOERROR_FILE (419, File, ""); /* cannot prompt user for external file name bindings for %s */
        }
    }
  if ((n = read (in, buf, sizeof (buf))) < 0)
    {
      if (in != 0)
        {
          _p_warning ("reading filename from /dev/tty failed, trying stdin");
          /* BUF should be ok still, since read failed. */
          (void) write (tty, buf, _p_strlen (buf));
        }
      if (in == 0 || (n = read (0, buf, sizeof (buf))) < 0)
        {
          if (tty >= 0) close (tty);
          IOERROR_FILE (420, File, ""); /* cannot query user for external file name bindings for %s */
        }
    }
  if (tty >= 0) close (tty);

  if (buf[0] == EOT)
    IOERROR_FILE (421, File, ""); /* EOT character given for query of file name for %s */

  buf[n] = '\0';
  if (n > 0 && buf[n-1] == '\n') buf[n-1] = '\0';
  _p_osdirseparator2slash_cstring (tmp = _p_strdup (buf));
  return m_EXTNAM (File) = tmp;
}

/* Check if File has a binding, and if so, set its external name */
static void
check_binding (FDR File)
{
  if (m_BINDING (File))
    {
      set_EXTB (File);
      if (m_BNDCHG (File))
        {
          m_BNDCHG (File) = 0;
          _p_close (File);
          if (_p_inoutres) return;
          m_EXTNAM (File) = m_BNDNAM (File);
        }
    }
  else
    if (tst_EXT (File))
      set_EXTB (File);
    else
      clr_EXTB (File);
}

void
_p_checkfiletype (FDR File, int stdinout)
{
  struct stat finfo;
  clr_TTY (File);
  clr_SEEK (File);
  if (fstat (File->Handle, &finfo) != 0) return;
  if (S_ISCHR (finfo.st_mode) && isatty (File->Handle)) set_TTY (File);
  /* Regular files and block devies are assumed seekable, nothing else.
     stdin, stdout, stderr are considered non-seekable (even if they're
     redirected to something seekable) to cause uniform behaviour for
     different kinds of input/output. */
  if (!stdinout && (S_ISREG (finfo.st_mode) || S_ISBLK (finfo.st_mode))) set_SEEK (File);
}

static void
_p_unlink (FDR File, const char *filename, int candelay)
{
  if (unlink (filename) != 0)
    {
      #if defined (MSDOS) || defined (_WIN32) || defined (__EMX__)
      /* Dos does not like unlinking an opened file in some circumstances,
         so remember the file name and unlink it later from _p_close().
         Check m_STATUS here, otherwise a real error (e.g., erasing a
         nonexisting file) could lead to later erasing or strange errors. */
      if (candelay && m_STATUS (File) != FiNOP)
        File -> NameToUnlink = _p_strdup (filename);
      else
      #else
      (void) candelay;
      #endif
        {
          switch (errno)
            {
              case ENOENT:
              case ENOTDIR: IOERROR_FILE (471, File,); /* `Erase': %s does not exist */
              case EACCES:
              case EPERM:
              case EISDIR:  IOERROR_FILE (472, File,); /* permission denied to erase %s */
              default:      IOERROR_FILE (474, File,); /* I/O error when trying to erase %s */
            }
        }
    }
}

static void
_p_close1 (FDR File)
{
  TFDRList *p, **pp;
  if (m_STATUS (File) == FiNOP)
    return;
  /* Don't check _p_inoutres here! We still want to close the file,
     even after an I/O error. */
  _p_flushbuffer (File);
  set_EOF (File);
  if (File->CloseProc == DefaultCloseProc)
    {
      if ((!_p_current_stdin || File == _p_current_stdin || File->Handle != _p_current_stdin->Handle) &&
          File->Handle > 2 /* not stdin, stdout, stderr */)
        {
          if (close (File->Handle) && !_p_inoutres)
            IOERROR_FILE (418, File,); /* error while closing %s */
          File->Handle = - 1;
        }
    }
  else if (File->CloseProc)
    {
      DO_RETURN_ADDRESS (File->CloseProc (File->PrivateData));
      if (!TST_STATUS (File, FiANY)) return;
    }
  for (pp = &_p_first_fdr; *pp && (*pp)->Item != File; pp = &((*pp)->Next));
  if (*pp)
    {
      p = *pp;
      *pp = p->Next;
      _p_free (p);
      D (4, fprintf (stderr, "Removed fdr %s from list\n", m_NAM (File)));
    }
}

void
_p_close (FDR File)
{
  if (m_STATUS (File) == FiNOP)
    return;
  /* Don't check _p_inoutres here! We still want to close the file,
     even after an I/O error. */
  _p_close1 (File);
  if (File -> NameToUnlink)
    {
      _p_unlink (File, File -> NameToUnlink, FALSE);
      _p_dispose (File -> NameToUnlink);
      File -> NameToUnlink = NULL;
    }
  if (m_EXTNAM (File))
    {
      if (! m_BINDING (File))
        _p_dispose (m_EXTNAM (File));
      m_EXTNAM (File) = NULL;
    }
  _p_reinitfdr (File);
  m_BNDCHG (File) = 1;
}

/*
  Open a File in mode, depending on its bindings etc.

  foReset:
  pre-assertion: The components f0.L and f0.R are not undefined
  post-assertion: (f.L = S ()) and (f.R = (f0.L~f0.R~X))
                  and (f.M = Inspection)
                  and (if f.R = S () then (f^ is undefined) else (f^ = f^.R.first))

  foRewrite:
  pre-assertion: true.
  post-assertion: (f.L = f.R = S ()) and (f.M = Generation)
                  and (f^ is undefined)

  foAppend:
  pre-assertion: f0.L and f0.R are not undefined
  post-assertion: (f.M = Generation) and (f.L = f0.L~f0.R~X)
                  and (f.R = S ())
                  and (f^ is undefined)

  where, if F is of type TEXT, and f0.L~f0.R is not empty and
  if (f0.L~f0.R).last is not an end-of-line, then X shall be a sequence
  having an end-of-line component as its only component;
  otherwise X = S ().
*/
void
_p_open (FDR File, TOpenMode mode)
{
  if (_p_inoutres) return;
  SAVE_RETURN_ADDRESS;
  if (m_SIZ (File) == 0)
    _p_internal_error (914); /* _p_initfdr has not been called for file */

  fil_clr (File, ~STATUS_KEEP); /* Clear file status bits */
  set_UND (File); /* Mark the file buffer contents undefined */

  if (File->OpenProc != DefaultOpenProc)
    {
      if (m_STATUS (File) != FiNOP)
        {
          _p_close (File);
          if (_p_inoutres)
            {
              RESTORE_RETURN_ADDRESS;
              return;
            }
        }
      File->Handle = - 1;
      if (File->OpenProc) File->OpenProc (File->PrivateData, mode);
      if (_p_inoutres)
        {
          RESTORE_RETURN_ADDRESS;
          return;
        }
    }
  else
    {
      const char *filename = (char *) NULL;
      int stdinout = FALSE;
      if (m_BINDING (File) && m_BINDING (File) -> Directory)
        {
          RESTORE_RETURN_ADDRESS;
          IOERROR_STRING (401, m_BNDNAM (File),); /* cannot open directory `%s' */
        }
      check_binding (File);
      if (_p_inoutres)
        {
          RESTORE_RETURN_ADDRESS;
          return;
        }
      filename = m_EXTNAM (File);
      if (m_STATUS (File) != FiNOP)
        {
          /* File is currently open in Pascal program */
          int tempcloseflag = 0;
          /* Don't complain when, e.g., the file is "read only" and
             mode is foRewrite. "Read only" is set for text files on
             Reset regardless whether the file itself is writable.
             Furthermore, the permissions might have been changed
             since the last opening. */
          if (is_RONLY (File) || is_WONLY (File))
            tempcloseflag = 1;
          else if (mode == foAppend)
            _p_seek (File, 0, SEEK_END); /* Start appending */
          else if (mode != foRewrite)
            _p_seek (File, 0, SEEK_SET); /* Start reading or updating */
          else
            {
              #ifdef HAVE_FTRUNCATE
              /* We have ftruncate() */
              _p_seek (File, 0, SEEK_SET); /* Start writing */
              if (ftruncate (File->Handle, 0) == -1)
                {
                  RESTORE_RETURN_ADDRESS;
                  IOERROR_FILE (423, File,); /* ftruncate failed when re-opening %s with `Rewrite' */
                }
              #else
              /* If we don't have ftruncate() emulate the behaviour */
              tempcloseflag = 1;
              #endif
            }
          if (tempcloseflag)
            {
              _p_close1 (File);
              m_STATUS (File) = FiNOP;
              if (_p_inoutres)
                {
                  RESTORE_RETURN_ADDRESS;
                  return;
                }
              /* Let the code below re-open the same external file for writing */
              /* If the file is internal, it will not be the same, but who cares. */
            }
        }

      if (m_STATUS (File) == FiNOP)
        {
          int errcode = 0;

          if ((mode == foReset || mode == foSeekRead || mode == foSeekUpdate) && !tst_EXTB (File))
            {
              RESTORE_RETURN_ADDRESS;
              IOERROR_FILE (436, File,); /* `Reset', `SeekUpdate' or `SeekRead' to nonexistent %s */
            }

          if (m_BINDING (File) && m_BINDING (File) -> Handle >= 0 && m_BNDNAM (File) [0] == 0)
            File->Handle = m_BINDING (File) -> Handle;
          else
            {
              if (!filename)
                filename = _p_nameit (File, mode);
              if (_p_inoutres)
                {
                  RESTORE_RETURN_ADDRESS;
                  return;
                }

              if (!filename || filename[0] == 0 ||
                 (filename[0] == '-' && filename[1] == 0))
                {
                  stdinout = TRUE;
                  File->Handle = (mode == foReset
                                  ? (_p_current_stdin ? _p_current_stdin->Handle : 0 /* stdin */)
                                  : ((File == &_p_stderr) ? 2 /* stderr */ : 1 /* stdout */));
                }
              else
                {
                  int openmode = 0, binary =
                    #ifdef O_BINARY
                    (!tst_TXT (File) || (m_BINDING (File) && m_BINDING (File)->TextBinary)) ? O_BINARY :
                    #endif
                    0;
                  switch (mode)
                    {
                      case foReset:
                      case foSeekRead:
                        if (_p_filemode & 2 && !tst_TXT (File)) openmode = O_RDWR;
                        errcode = 442; /* %s cannot be opened for reading */
                        break;
                      case foRewrite:
                        if (!(_p_filemode & 4) || !tst_EXTB (File)) openmode = O_RDWR | O_CREAT | O_TRUNC;
                        errcode = 443; /* %s cannot be opened for writing */
                        break;
                      case foAppend:
                        /* do not use O_APPEND here because it does not allow writing
                           before the current end of file even after a seek */
                        if (!(_p_filemode & 8) || !tst_EXTB (File)) openmode = O_RDWR | O_CREAT;
                        errcode = 445; /* %s cannot be extended */
                        break;
                      case foSeekWrite:
                      case foSeekUpdate:
                        openmode = O_RDWR;
                        errcode = 444; /* %s cannot be opened for updating */
                        break;
                      default:
                        _p_internal_error (921); /* Unknown mode in _p_open */
                    }
                  if (openmode != 0)
                    File->Handle = open (filename, openmode | binary, 0666);
                  else
                    File->Handle = - 1;

                  /* Now, if the file could not be opened, but we want to only
                     read from or only write to it, check if that is possible */
                  if (File->Handle < 0)
                    {
                      switch (mode)
                        {
                          case foReset:
                          case foSeekRead:
                            File->Handle = open (filename, O_RDONLY | binary, 0666);
                            if (File->Handle >= 0)
                              {
                                SET_STATUS (File, FiRONLY);
                                _p_warning ("file is read only");
                              }
                            break;
                          case foRewrite:
                          case foSeekWrite:
                            File->Handle = open (filename, O_WRONLY | O_CREAT | O_TRUNC | binary, 0666);
                            if (File->Handle >= 0)
                              {
                                SET_STATUS (File, FiWONLY);
                                _p_warning ("file is write only");
                              }
                            break;
                          case foAppend:
                            File->Handle = open (filename, O_WRONLY | O_CREAT | binary, 0666);
                            if (File->Handle >= 0)
                              {
                                SET_STATUS (File, FiWONLY);
                                _p_warning ("file is write only");
                              }
                            break;
                          case foSeekUpdate:
                            /* nothing */
                            break;
                          default:
                            _p_internal_error (911); /* invalid file open mode */
                        }
                    }
                }
            }
          if (File->Handle < 0)
            {
              filename = (char *) NULL;
              RESTORE_RETURN_ADDRESS;
              IOERROR_FILE (errcode, File,);
            }
          else
            if (!tst_EXTB (File) && !_p_debug)
              _p_unlink (File, filename, TRUE);
        }
      _p_checkfiletype (File, stdinout);
    }
  if (mode == foRewrite || mode == foSeekWrite || mode == foAppend) SET_STATUS (File, FiWRI);
  if (mode == foReset || mode == foSeekRead  || mode == foSeekUpdate || (!tst_TXT (File) && !is_WONLY (File))) SET_STATUS (File, FiORE);
  if (!(is_RONLY (File) || is_WONLY (File) || tst_TXT (File))) SET_STATUS (File, FiRND);
  if (tst_TXT (File)) set_EOFOK (File);
  clr_UNREAD (File);
  _p_clearbuffer (File);
  File->Flags = 0;
  /* Add to FDR chain. Do it only when necessary, to speed up e.g. the
     string TFDD */
  if (File->FlushProc || File->CloseProc || File->DoneProc)
    {
      TFDRList *p;
      for (p = _p_first_fdr; p && p->Item != File; p = p->Next);
      if (!p) /* File not yet in list */
        {
          p = _p_malloc (sizeof (TFDRList));
          p->Next = _p_first_fdr;
          p->Item = File;
          _p_first_fdr = p;
          D (4, fprintf (stderr, "Added %s to fdr list\n", m_NAM (File)));
        }
    }
  switch (mode)
    {
      case foAppend:
        if (tst_TXT (File))
          {
            if (is_WONLY (File))
              _p_warning_string ("appending to write only text file `%s'; trailing EOLn not checked", m_NAM (File));
            else if (_p_seek (File, -1, SEEK_END) >= 0)
              {
                char nl = NEWLINE;
                _p_internal_getc (File);
                #ifdef __EMX__
                _p_internal_getc (File);
                #endif
                /* file pointer is now at EOF */
                if (!tst_EOLN (File))
                  _p_internal_write (&nl, sizeof (nl), File);
              }
          }
        set_EOF (File);
        clr_EOLN (File);
        set_LGET (File);
        if (_p_seek (File, 0, SEEK_END) < 0)
          {
            RESTORE_RETURN_ADDRESS;
            IOERROR_FILE (416, File,); /* `Extend'' could not seek to end of % */
          }
        break;
      case foRewrite:
        set_EOF (File);
        clr_EOLN (File);
        set_LGET (File);
        break;
      case foReset:
        clr_EOF (File);
        clr_EOLN (File);
        clr_UND (File);
        set_LGET (File);
        if (tst_TXT (File) && tst_TTY (File) && _p_eoln_reset_hack)
          set_UNREAD (File); /* Mark for EOLn; nothing has been read yet */
        /* Please do not remove this. It's here as an inside joke */
        D (1, fprintf (stderr, "Kukkuu RESET (TM)\n"));
        break;
      case foNone:
      case foSeekRead:
      case foSeekWrite:
      case foSeekUpdate:
        /* NOTHING */
    }
  RESTORE_RETURN_ADDRESS;
}

static void internal_assign (FDR File, char *filename, int length)
{
  _p_internal_assign (File, filename, length);
  if (!_p_inoutres && (!m_BINDING (File) || !m_BINDING (File)->Bound))
    {
      static char *buf = NULL;
      if (buf) _p_dispose (buf);
      buf = _p_malloc (length + 1);
      _p_strlcpy (buf, filename, length);
      buf [length] = 0;
      IOERROR_STRING (405, buf,); /* could not open `%s'' */
    }
}

void
_p_reset (FDR File, char *filename, int length)
{
  if (length) internal_assign (File, filename, length);
  _p_open (File, foReset);
}

void
_p_rewrite (FDR File, char *filename, int length)
{
  if (length) internal_assign (File, filename, length);
  _p_open (File, foRewrite);
}

void
_p_extend (FDR File, char *filename, int length)

{
  if (length) internal_assign (File, filename, length);
  _p_open (File, foAppend);
}

inline int
_p_ok_READ (FDR File)
{
  if (is_READABLE (File)) return 1;
  IOERROR_FILE (452, File, 0); /* %s is not open for reading */
}

inline int
_p_ok_EOF (FDR File)
{
  if (_p_inoutres || !_p_ok_READ (File)) return 0;
  if (!tst_EOF (File)) return 1;
  if (File->Flags & READ_WRITE_STRING_MASK)
    IOERROR (550, 0); /* Attempt to read past end of string in `ReadStr'*/
  else
    IOERROR_FILE (454, File, 0); /* attempt to read past end of %s */
}

inline int
_p_ok_WRITE (FDR File)
{
  if (is_WRITABLE (File)) return 1;
  IOERROR_FILE (450, File, 0); /* %s is not open for writing */
}

static void
_p_internal_blockwrite (FDR File, const char *Buf, size_t size, unsigned int *Result)
{
  size_t m = 0, n;
  if (Result)
    *Result = 0;
  if (_p_inoutres || !_p_ok_WRITE (File)) return;
  errno = 0;
  if (size > 0 && File->WriteFunc)
    do
      {
        if (File->WriteFunc == DefaultWriteFunc)
          n = _p_f_write (File->Handle, Buf + m, size);
        else
          n = File->WriteFunc (File->PrivateData, Buf + m, size);
        if (_p_inoutres && !_p_inoutres_str)
          IOERROR_FILE (_p_inoutres, File,);
        size -= n;
        m += n;
      }
    while ((n > 0 || errno == EINTR) && size > 0 && (m % m_SIZ (File) != 0 || !Result));
  if (Result)
    *Result = m / m_SIZ (File);
  else if (!_p_inoutres && size > 0)
    IOERROR_FILE ((m == 0) ? 466 : 467, File,); /* error when writing to `%s'; could not write all the data to `%s' */
}

void
_p_blockwrite (FDR File, const char *Buf, unsigned int Count, unsigned int *Result)
{
  DO_RETURN_ADDRESS (_p_internal_blockwrite (File, Buf, Count * m_SIZ (File), Result));
}

void
_p_internal_write (const char *ptr, size_t size, FDR File)
{
  _p_internal_blockwrite (File, ptr, size, NULL);
}

/* GET
 * pre-assertion: (f0.M = Inspection or f0.M = Update) and
 *                (neither f0.L nor f0.R is undefined) and
 *                (f0.R <> S ())
 * post-assertion: (f.M = f0.M) and (f.L = f0.L~S (f0.R.first)) and
 *                 (f.R = f0.R.rest)) and
 *                  (if (f.R = S ()) then
 *                     (f^ is undefined)
 *                   else
 *                     (f^ = f.R.first))
 */
void
_p_get (FDR File)
{
  if (_p_inoutres) return;
  _p_got (File);
  if (_p_inoutres) return;
  _p_lazyget (File);
  if (_p_inoutres) return;
}

void
_p_got (FDR File)
{
  if (_p_inoutres) return;
  _p_lazyget (File);
  if (_p_inoutres) return;
  set_LGET (File);
}

Boolean
_p_canread (FDR File)
{
  int result;
  InternalIOSelectType e;
  e.File = File;
  e.WantedRead = 1;
  e.WantedReadOrEOF = e.WantedEOF = e.WantedWrite = e.WantedException = e.WantedAlways = 0;
  DO_RETURN_ADDRESS (result = _p_select (&e, 1, 1, 0) > 0);
  return result;
}

#ifdef HAVE_SELECT
typedef struct timeval ttimeval;
#else
#undef fd_set
#define fd_set int
#undef FD_ZERO
#define FD_ZERO(s)
#undef FD_CLR
#define FD_CLR(f, s)
#undef FD_SET
#define FD_SET(f, s)
#undef FD_ISSET
#define FD_ISSET(f, s) ((f) >= 0 && 0)

typedef struct
{
  int tv_sec, tv_usec;
} ttimeval;

static int
select (int n, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, ttimeval *timeout)

{
  if (n) return - 1;
  /* systems without select() probably don't have usleep(), either.
     One could add a configure check for usleep(), however... */
  if (timeout) sleep (timeout->tv_sec + (timeout->tv_usec > 0));
  return 0;
}
#endif

static int
_p_select_occurred_read (InternalIOSelectType *p)
{
  p->OccurredReadOrEOF = 1;
  if (p->WantedRead || p->WantedEOF)
    p->OccurredRead = !((p->OccurredEOF = _p_eof (p->File))); /* assignment! */
  return p->WantedReadOrEOF || (p->WantedRead && p->OccurredRead) || (p->WantedEOF && p->OccurredEOF);
}

int
_p_select (InternalIOSelectType *Events, int Low, int Count, long long int MicroSeconds)
{
  ttimeval timeout, *ptimeout;
  if (MicroSeconds < 0)
    ptimeout = NULL;
  else
    {
      timeout.tv_sec  = MicroSeconds / 1000000;
      timeout.tv_usec = MicroSeconds % 1000000;
      ptimeout = &timeout;
    }
  if (!Events || Count <= 0)
    return (select (0, NULL, NULL, NULL, ptimeout) < 0) ? - 1 : 0;
  {
    int i, sresult, n = 0, result = 0, always = 0;
    fd_set readfds, writefds, exceptfds;
    FD_ZERO (&readfds);
    FD_ZERO (&writefds);
    FD_ZERO (&exceptfds);
    for (i = 0; i < Count; i++)
      {
        InternalIOSelectType *p = &Events [i];
        int f = 0, fa = 0;
        p->OccurredReadOrEOF = p->OccurredRead = p->OccurredEOF = p->OccurredWrite = p->OccurredException = 0;
        if (p->File && m_STATUS (p->File) != FiNOP)
          {
            int fn = p->File->SelectFunc ? p->File->SelectFunc (p->File->PrivateData, p->WantedWrite) : p->File->Handle;
            Boolean WantRead = (p->WantedReadOrEOF || p->WantedRead || p->WantedEOF) && is_READABLE (p->File),
                    WantWrite = p->WantedWrite && is_WRITABLE (p->File),
                    WantExcept = p->WantedException;
            int buffered = WantRead && (tst_EOF (p->File) || !tst_LGET (p->File) || p->File->BufPos < p->File->BufSize);
            if (buffered && _p_select_occurred_read (p))
              {
                WantRead = 0;
                f = 1;
              }
            if (fn >= 0)
              {
                if (fn >= n) n = fn + 1;
                if (WantRead && !buffered)
                  {
                    fa = 1;
                    FD_SET (fn, &readfds);
                  }
                if (WantWrite)
                  {
                    fa = 1;
                    FD_SET (fn, &writefds);
                  }
                if (WantExcept)
                  {
                    fa = 1;
                    FD_SET (fn, &exceptfds);
                  }
              }
            if ((WantRead || WantWrite || WantExcept) &&
                (p->File->SelectProc || fn < 0))
              {
                if (p->File->SelectProc)
                  p->File->SelectProc (p->File->PrivateData, &WantRead, &WantWrite, &WantExcept);
                else
                  {
                    if (!p->File->ReadFunc) WantRead = 0;
                    if (!p->File->WriteFunc) WantWrite = 0;
                  }
                if (WantRead && _p_select_occurred_read (p)) f = 1;
                if (WantWrite)  f = p->OccurredWrite = 1;
                if (WantExcept) f = p->OccurredWrite = 1;
              }
          }
        if (p->WantedAlways && fa) always = 1;
        if (f) result = Low + i;
      }
    if (result && !always) return result;
    sresult = select (n, &readfds, &writefds, &exceptfds, ptimeout);
    for (i = 0; i < Count; i++)
      {
        InternalIOSelectType *p = &Events [i];
        if (p->File && m_STATUS (p->File) != FiNOP)
          {
            Boolean WantRead = (p->WantedReadOrEOF || p->WantedRead || p->WantedEOF) && is_READABLE (p->File),
                    WantWrite = p->WantedWrite && is_WRITABLE (p->File),
                    WantExcept = p->WantedException;
            int f = 0, fn = p->File->SelectFunc ? p->File->SelectFunc (p->File->PrivateData, p->WantedWrite) : p->File->Handle;
            if (sresult > 0 && fn >= 0)
              {
                if (FD_ISSET (fn, &readfds))
                  {
                    WantRead = 0;
                    f = _p_select_occurred_read (p);
                  }
                if (FD_ISSET (fn, &writefds))
                  {
                    WantWrite = 0;
                    f = p->OccurredWrite = 1;
                  }
                if (FD_ISSET (fn, &exceptfds))
                  {
                    WantExcept = 0;
                    f = p->OccurredException = 1;
                  }
              }
            /* Call SelectProc even if select() returned an error --
               TFDDs might use signals to interrupt select() when ready. */
            if ((WantRead || WantWrite || WantExcept) && p->File->SelectProc)
              {
                p->File->SelectProc (p->File->PrivateData, &WantRead, &WantWrite, &WantExcept);
                if (WantRead && _p_select_occurred_read (p)) f = 1;
                if (WantWrite)  f = p->OccurredWrite = 1;
                if (WantExcept) f = p->OccurredWrite = 1;
              }
            if (f) result = Low + i;
          }
      }
    if (result == 0 && sresult < 0) result = - 1;
    return result;
  }
}

/* The standard requires that EOLn be set before EOF in text files.
 * Based on this we do not validate an undefined buffer for text files
 * when reading from a terminal if EOLn is not set. */
int
_p_eof (FDR File)
{
  if (_p_inoutres) return 1;
  if (m_STATUS (File) == FiNOP)
    IOERROR_FILE (455, File, 1); /* `EOF' tested for unopened %s */
  if (!tst_TXT (File) && tst_SEEK (File))
    {
      FileSizeType position = _p_position (File), size = _p_getsize (File);
      if (_p_inoutres) return 1;
      return position >= size;
    }
  /* If EOF is already set, don't try to read past EOF */
  if (tst_EOF (File))
    return 1;
  if (tst_LGET (File) && is_READABLE (File))
  {
    /* If we do not have EOLn or EOFOK when reading from terminal text file,
     * this can't be EOF */
    if (tst_TXT (File) && tst_TTY (File) && !(tst_EOLN (File) || tst_EOFOK (File)))
      return 0;
    else
      {
        _p_lazyget (File);
        if (_p_inoutres) return 1;
      }
  }
  return tst_EOF (File);
}

int
_p_eoln (FDR File)
{
  if (_p_inoutres) return 1;
  if (m_STATUS (File) == FiNOP)
    IOERROR_FILE (456, File, 1); /* `EOLn' tested for unopened %s */
  if (!tst_TXT (File))
    IOERROR_FILE (458, File, 1); /* `EOLn' applied to non-text %s */
  if (tst_LGET (File) && is_READABLE (File))
    {
      if (tst_UND (File))
        {
          /* If EOLn is tested in TERMINAL DEVICE where nothing has
             been read yet, return TRUE
             If it is not tested, it is FALSE.
             UNREAD is set before anything is read. */
          if (tst_TTY (File) && tst_UNREAD (File))
            {
              m_FILBUF (File) = ' ';
              set_EOLN (File);
              clr_LGET (File);
              clr_UND (File);
              clr_UNREAD (File);
              return 1;
            }
        }
      _p_lazyget (File);
      if (_p_inoutres) return 1;
    }
  if (tst_EOF (File))
    return TRUE; /*IOERROR_FILE (457, File, 1);*/ /* `EOLn' tested for %s when `EOF' is true */
  return tst_EOLN (File);
}

void
_p_read_buffer (FDR File)
{
  File->BufPos = 0;
  if (File->ReadFunc)
    {
      if (tst_TTY (File)) _p_fflush (TRUE);
      if (File->ReadFunc == DefaultReadFunc)
        File->BufSize = _p_f_read (File, File->BufPtr, FILE_BUFSIZE);
      else
        File->BufSize = File->ReadFunc (File->PrivateData, File->BufPtr, FILE_BUFSIZE);
      if (_p_inoutres && !_p_inoutres_str) IOERROR_FILE (_p_inoutres, File,);
    }
  else
    File->BufSize = 0;
}

static void
_p_internal_read (unsigned char *ptr, size_t size, size_t *presult, FDR File)
{
  size_t result = 0;
  if (tst_EOF (File)) return;
  while (_p_inoutres == 0 && result < size)
    {
      if (File->BufPos < File->BufSize)
        {
          size_t r = File->BufSize - File->BufPos;
          if (r > size - result) r = size - result;
          _p_move (File->BufPtr + File->BufPos, ptr, r);
          File->BufPos += r;
          ptr += r;
          result += r;
        }
      if (result < size)
        {
          _p_read_buffer (File);
          if (File->BufPos >= File->BufSize)
            {
              set_EOF (File);
              set_EOLN (File);
              break;
            }
        }
    }
  if (presult)
    *presult = result;
  else
    if (!_p_inoutres && result != size)
      IOERROR_FILE (465, File,); /* cannot read all the data from %s */
}

void
_p_blockread (FDR File, char *Buf, unsigned int Count, unsigned int *Result)
{
  size_t m = 0, n, r;
  if (Result)
    *Result = 0;
  if (_p_inoutres || !_p_ok_READ (File)) return;
  Count *= m_SIZ (File);
  if (!tst_EOF (File))
    {
      /* If something was read ahead (e.g. in _p_eof()), copy this to the
         destination buffer first */
      if (Count != 0 && !tst_LGET (File))
        {
          Buf [m++] = m_FILBUF (File);
          Count--;
          set_LGET (File);
        }
      if (File->BufPos < File->BufSize)
        {
          n = File->BufSize - File->BufPos;
          if (n > Count) n = Count;
          memmove (Buf + m, File->BufPtr + File->BufPos, n);
          File->BufPos += n;
          if (File->BufPos >= File->BufSize) _p_clearbuffer (File);
          Count -= n;
          m += n;
        }
      if (Count > 0)
        {
          int f = m < m_SIZ (File) || !Result;
          if (!f)
            {
              InternalIOSelectType e;
              e.File = File;
              e.WantedReadOrEOF = 1;
              e.WantedRead = e.WantedEOF = e.WantedWrite = e.WantedException = e.WantedAlways = 0;
              f = _p_select (&e, 1, 1, 0) > 0;
            }
          if (f)
            {
              if (tst_TTY (File)) _p_fflush (TRUE);
              errno = 0;
              do
                {
                  if (File->ReadFunc)
                    {
                      if (File->ReadFunc == DefaultReadFunc)
                        n = _p_f_read (File, Buf + m, Count);
                      else
                        n = File->ReadFunc (File->PrivateData, Buf + m, Count);
                      if (_p_inoutres && !_p_inoutres_str) IOERROR_FILE (_p_inoutres, File,);
                    }
                  else
                    n = 0;
                  Count -= n;
                  m += n;
                }
              while ((n > 0 || errno == EINTR) && Count > 0 && (m < m_SIZ (File) || !Result));
              if (n == 0) set_EOF (File);
            }
        }
    }
  r = m % m_SIZ (File);
  if (r)
    {
      memmove (File->BufPtr + File->BufSize, Buf + m - r, r);
      File->BufSize += r;
    }
  if (Result)
    *Result = m / m_SIZ (File);
  else
    if (Count > 0)
      IOERROR_FILE (415, File,); /* BlockRead: could not read all the data from `%s' */
}

/* Gets one byte from the file.

   First get from the terminal input device
   This is done to take care of an EOLn test
   before anything is read in. Otherwise we
   would have to read in a character to test.

   @@ Document: If INPUT is RESET, the file buffer
   is set UNDEFINED and when nothing is read in yet:

   EOF (Input) = False

     (This is according to standard, because EOLn must be on
      before EOF can be TRUE).

   EOLn (Input)

     If it is TESTED it is TRUE.
     If it is NOT TESTED it is FALSE */
void
_p_getbyte (FDR File)
{
  int eof_now = tst_EOFOK (File), temp;
  if (!_p_ok_EOF (File)) return;
  clr_UNREAD (File);
  clr_UND (File);
  clr_LGET (File);
  clr_EOFOK (File);
  m_FILBUF (File) = temp = _p_internal_getc (File);
  if (temp != EOF) return;
  if (tst_TXT (File) && !eof_now && !tst_EOLN (File))
    {
      /* When reading from a Text file EOLn is always true just before
         EOF, even if there is no end of line at the end of the file */
      set_EOLN (File);
      m_FILBUF (File) = ' ';
    }
  else
    {
      set_EOF (File);
      set_UND (File);
    }
}

/* Get m_SIZ bytes (> 1) from the file */
static void
_p_get_n (FDR File)
{
  size_t n;
  if (!_p_ok_EOF (File)) return;
  clr_UND (File);
  _p_internal_read (m_FILBPTR (File), m_SIZ (File), &n, File);
  if (_p_inoutres) return;
  if (n < m_SIZ (File))
    {
      if (n != 0)
        _p_warning ("read partial record in `Get'");
      else
        {
          set_EOF (File);
          clr_EOLN (File);
        }
      set_UND (File);
    }
  else
    clr_LGET (File);
}

/* This is the buffer referencing routine, that the compiler
 * should do inline. Nothing is actually done, if tst_LGET (File)
 * is not on. Compiler should dereference the file buffer address
 * to get the value from the buffer. */
void
_p_lazytryget (FDR File)
{
  if (_p_inoutres) return;
#if 0
  /* @@ This is called also for `Buffer^ := Val;'
   * So it must not blindly trap the reference
   *
   * Compiler should clear the FiUnd bit for these...
   */
  if (tst_UND (File) && !tst_LGET (File))
    IOERROR_FILE (440, File,); /* reference to buffer variable of %s with undefined value */
#endif

  /* If the file buffer contents is lazy, validate it */
  if (tst_LGET (File))
    {
      if (!is_READABLE (File) || tst_EOF (File))
        {
          /* Buffer cannot be read in. But perhaps someone only wants to
             write to it, who knows? (This routine doesn't know, and that's
             the problem!)-: . So we zero it and mark it undefined. :-*/
          set_UND (File);
        }
      else
        if (m_SIZ (File) == 1) /* No files are packed yet. */
          {
            _p_getbyte (File);
            if (_p_inoutres) return;
          }
        else
          {
            _p_get_n (File);
            if (_p_inoutres) return;
          }
    }
}

/* This is the buffer referencing routine for read-only access. */
void
_p_lazyget (FDR File)
{
  if (_p_inoutres) return;

  /* If the file buffer contents is lazy, validate it */
  if (tst_LGET (File))
    {
      if (!_p_ok_READ (File)) return;
      if (m_SIZ (File) == 1) /* No files are packed yet. */
        {
          _p_getbyte (File);
          if (_p_inoutres) return;
        }
      else
        {
          _p_get_n (File);
          if (_p_inoutres) return;
        }
    }
}

/* Empty a file buffer before writing to it */
void
_p_lazyunget (FDR File)
{
  if (_p_inoutres) return;
  /* If the file buffer content is filled, clear it and seek back */
  if (!tst_LGET (File))
    {
      _p_seek (File, -1, SEEK_CUR);
      clr_EOF (File);
      clr_EOLN (File);
      set_LGET (File);
      set_UND (File);
    }
}

void
_p_page (FDR File)
{
  char c = NEWPAGE;
  _p_internal_write (&c, sizeof (c), File);
}

/* PUT
 * pre-assertion: (f0.M = Generation or f0.M = Update) and
 *    (neither f0.L nor f0.R is undefined) and
 *    (f0.R = S () or f is a direct access file type) and
 *    (f0^ is not undefined)
 * post-assertion: (f.M = f0.M) and (f.L = f0.L~S (f0^)) and
 *    (if f0.R = S () then
 *       (f.R = S ())
 *    else
 *      (f.R = f0.R.rest)) and
 *      (if (f.R = S ()) or (f0.M = Generation) then
 *        (f^ is undefined)
 *      else
 *        (f^ = f.R.first))
 */
void
_p_put (FDR File)
{
  if (tst_DIRECT (File))
    _p_flushbuffer (File);
  _p_internal_write (m_FILBPTR (File), m_SIZ (File), File);
  if (_p_inoutres) return;
  /* f^ set undefined if EOF or mode is generation */
  if (tst_EOF (File) || !TST_STATUS (File, FiRND))
    set_UND (File);
}

void
_p_donefdr (FDR File)
{
  SAVE_RETURN_ADDRESS;
  _p_close (File);
  if (File->DoneProc)
    {
      File->DoneProc (File->PrivateData);
      File->DoneProc = NULL;
    }
  RESTORE_RETURN_ADDRESS;
}

/* get external file name */
const char *
_p_filename (FDR File)
{
  return m_EXTNAM (File);
}

/* get internal or external file name with a description -- currently used
   for error messages. NOTE: result is only valid until the function gets
   called again */
const char *
_p_get_file_name (const FDR File)
{
  static char *buf = NULL;
  if (buf) _p_dispose (buf);
  if (isStdFile (File))
    buf = _p_strdup (m_NAM (File));
  else if (File -> ReadFunc != DefaultReadFunc)
    {
      buf = (char *) _p_malloc (26 + _p_strlen (m_NAM (File)));
      if (buf) sprintf (buf, "TFDD file `%s'", m_NAM (File));
    }
  else if (m_BINDING (File) && m_BINDING (File) -> Handle >= 0 && m_BNDNAM (File) [0] == 0)
    {
      buf = (char *) _p_malloc (80 + _p_strlen (m_NAM (File)));
      if (buf) sprintf (buf, "file `%s' bound to file handle #%i", m_NAM (File), m_BINDING (File) -> Handle);
    }
  else if (tst_EXTB (File))
    {
      buf = (char *) _p_malloc (18 + _p_strlen (m_EXTNAM (File)));
      if (buf) sprintf (buf, "file `%s'", m_EXTNAM (File));
    }
  else
    {
      buf = (char *) _p_malloc (27 + _p_strlen (m_NAM (File)));
      if (buf) sprintf (buf, "internal file `%s'", m_NAM (File));
    }
  if (buf)
    return buf;
  else
    return m_NAM (File);
}

Boolean
_p_write_error_message (char *s, Boolean StdErrFlag)
{
  if (StdErrFlag)
    {
      write (_p_stderr.Handle >= 0 ? _p_stderr.Handle : 2 /*stderr*/, s, _p_strlen (s));
      if (_p_error_fd < 0)
        {
          if (_p_error_file_name)
            _p_error_fd = open (_p_error_file_name, O_WRONLY | O_CREAT | O_TRUNC, 0666);
          if (_p_error_fd < 0)
            return 0;
        }
    }
  else
    write (_p_error_fd, s, _p_strlen (s));
  return 1;
}

void
_p_erase (FDR File)
{
  if (_p_inoutres) return;
  if (m_BINDING (File) && m_BINDING (File) -> Directory)
    IOERROR_STRING (473, m_BNDNAM (File),); /* `Erase' cannot erase directory `%s' */
  DO_RETURN_ADDRESS (check_binding (File));
  if (_p_inoutres) return;
  if (!tst_EXTB (File))
    IOERROR_FILE (468, File,); /* cannot erase %s */
  if (!m_EXTNAM (File))
    IOERROR_STRING (469, m_NAM (File),); /* `Erase': external file `%s' has no external name */
/*  if (m_STATUS (File) != FiNOP)
    IOERROR_FILE (470, File,); *//* cannot erase opened %s */
  _p_unlink (File, m_EXTNAM (File), TRUE);
}

void
_p_rename (FDR File, char *NewName)
{
  if (_p_inoutres) return;
  DO_RETURN_ADDRESS (check_binding (File));
  if (_p_inoutres) return;
  if (!tst_EXTB (File))
    IOERROR_FILE (475, File,); /* cannot rename %s */
  if (!m_EXTNAM (File))
    IOERROR_STRING (476, m_NAM (File),); /* `Rename': external file `%s' has no external name */
  if (m_STATUS (File) != FiNOP)
    IOERROR_FILE (477, File,); /* cannot rename opened %s */
  if (access (NewName, F_OK) == 0)
    IOERROR_STRING (482, NewName,); /* `Rename': cannot overwrite file `%s' */
  if (rename (m_EXTNAM (File), NewName) != 0)
    {
      switch (errno)
        {
          case ENOENT:
          case ENOTDIR: IOERROR_FILE (478, File,); /* `Rename': %s does not exist */
          case EBUSY:
          case EACCES:
          case EPERM:   IOERROR_FILE (479, File,); /* permission denied to rename %s */
          case EISDIR:  IOERROR_STRING (480, NewName,); /* `Rename': cannot overwrite directory `%s' */
          default:      IOERROR_FILE (481, File,); /* I/O error when trying to rename %s */
        }
    }
  _p_dispose (m_EXTNAM (File));
  m_EXTNAM (File) = _p_strdup (NewName);
  if (m_BINDING (File)) m_BNDNAM (File) = m_EXTNAM (File);
}

void
_p_chdir (char *path)
{
  if (_p_inoutres) return;
  if (chdir (path) != 0)
    IOERROR_STRING (483, path,); /* cannot change to directory `%s' */
}

void
_p_mkdir (char *path)
{
  if (_p_inoutres) return;
  if (mkdir (path, 0777) != 0)
    IOERROR_STRING (484, path,); /* cannot make directory `%s' */
}

void
_p_rmdir (char *path)
{
  if (_p_inoutres) return;
  if (rmdir (path) != 0)
    IOERROR_STRING (485, path,); /* cannot remove directory `%s' */
}

DIR *
_p_opendir_c (char *Name)
{
  return opendir (Name);
}

char *
_p_readdir_c (DIR *dir)
{
  struct dirent *d = readdir (dir);
  if (d)
    return d -> d_name;
  else
    return NULL;
}

void
_p_closedir (DIR *dir)
{
  if (dir && closedir (dir)) IOERROR (496,); /* cannot close directory */
}

/* mode constants */
#define fmSetUID           04000
#define fmSetGID           02000
#define fmSticky           01000
#define fmUserReadable     00400
#define fmUserWritable     00200
#define fmUserExecutable   00100
#define fmGroupReadable    00040
#define fmGroupWritable    00020
#define fmGroupExecutable  00010
#define fmOthersReadable   00004
#define fmOthersWritable   00002
#define fmOthersExecutable 00001

/* Dos systems only know permissions (partly) for user, not for group and others */
#ifndef S_ISUID
#define S_ISUID 0
#endif
#ifndef S_ISGID
#define S_ISGID 0
#endif
#ifndef S_ISVTX
#define S_ISVTX 0
#endif
#ifndef S_IRGRP
#define S_IRGRP S_IRUSR
#endif
#ifndef S_IROTH
#define S_IROTH S_IRUSR
#endif
#ifndef S_IWGRP
#define S_IWGRP S_IWUSR
#endif
#ifndef S_IWOTH
#define S_IWOTH S_IWUSR
#endif
#ifndef S_IXGRP
#define S_IXGRP S_IXUSR
#endif
#ifndef S_IXOTH
#define S_IXOTH S_IXUSR
#endif

#define ALLMODES (S_ISUID | S_ISGID | S_ISVTX | S_IRUSR | S_IWUSR | S_IXUSR | \
                  S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH)

/* convert the constants on "strange" OSs */
#if S_ISUID != fmSetUID         || S_ISGID != fmSetGID         || S_ISVTX != fmSticky          || \
    S_IRUSR != fmUserReadable   || S_IWUSR != fmUserWritable   || S_IXUSR != fmUserExecutable  || \
    S_IRGRP != fmGroupReadable  || S_IWGRP != fmGroupWritable  || S_IXGRP != fmGroupExecutable || \
    S_IROTH != fmOthersReadable || S_IWOTH != fmOthersWritable || S_IXOTH != fmOthersExecutable
int
_p_Mode2SysMode (int Mode)
{
  return
    ((Mode & fmSetUID)           ? S_ISUID : 0) |
    ((Mode & fmSetGID)           ? S_ISGID : 0) |
    ((Mode & fmSticky)           ? S_ISVTX : 0) |
    ((Mode & fmUserReadable)     ? S_IRUSR : 0) |
    ((Mode & fmUserWritable)     ? S_IWUSR : 0) |
    ((Mode & fmUserExecutable)   ? S_IXUSR : 0) |
    ((Mode & fmGroupReadable)    ? S_IRGRP : 0) |
    ((Mode & fmGroupWritable)    ? S_IWGRP : 0) |
    ((Mode & fmGroupExecutable)  ? S_IXGRP : 0) |
    ((Mode & fmOthersReadable)   ? S_IROTH : 0) |
    ((Mode & fmOthersWritable)   ? S_IWOTH : 0) |
    ((Mode & fmOthersExecutable) ? S_IXOTH : 0);
}

int
_p_SysMode2Mode (int Mode)
{
  return
    ((Mode & S_ISUID) ? fmSetUID           : 0) |
    ((Mode & S_ISGID) ? fmSetGID           : 0) |
    ((Mode & S_ISVTX) ? fmSticky           : 0) |
    ((Mode & S_IRUSR) ? fmUserReadable     : 0) |
    ((Mode & S_IWUSR) ? fmUserWritable     : 0) |
    ((Mode & S_IXUSR) ? fmUserExecutable   : 0) |
    ((Mode & S_IRGRP) ? fmGroupReadable    : 0) |
    ((Mode & S_IWGRP) ? fmGroupWritable    : 0) |
    ((Mode & S_IXGRP) ? fmGroupExecutable  : 0) |
    ((Mode & S_IROTH) ? fmOthersReadable   : 0) |
    ((Mode & S_IWOTH) ? fmOthersWritable   : 0) |
    ((Mode & S_IXOTH) ? fmOthersExecutable : 0);
}
#else
int
_p_Mode2SysMode (int Mode)
{
  return Mode & ALLMODES;
}

int
_p_SysMode2Mode (int Mode)
{
  return Mode & ALLMODES;
}
#endif

void
_p_chmod (FDR File, int Mode)
{
  if (_p_inoutres) return;
  DO_RETURN_ADDRESS (check_binding (File));
  if (_p_inoutres) return;
  /* @@ TFDD */
  if (!m_EXTNAM (File))
    IOERROR_STRING (491, m_NAM (File),); /* `ChMod': file `%s' has no external name */
  if (chmod (m_EXTNAM (File), _p_Mode2SysMode (Mode)) != 0)
    {
      switch (errno)
        {
          case ENOENT:
          case ENOTDIR: IOERROR_FILE (492, File,); /* `ChMod': %s does not exist */
          case EACCES:
          case EPERM:   IOERROR_FILE (493, File,); /* permission denied to change mode of %s */
          default:      IOERROR_FILE (494, File,); /* I/O error when trying to change mode of %s */
        }
    }
}

#ifndef MAXPATHLEN
#ifdef PATH_MAX
#define MAXPATHLEN PATH_MAX
#elif defined (MAX_PATH)
#define MAXPATHLEN MAX_PATH
#else
#define MAXPATHLEN 2048
#endif
#endif

#if !(defined (HAVE_REALPATH) || defined (realpath))

/* Must not be a macro, because it must evalute its argument only once */
static inline int isslash (char ch)
{
  return ch == '/' || ch == _p_dirseparatorvar;
}

#define DOSDRIVE(name) (isslash ('\\') && (name)[0] != 0 && (name)[1] == ':')

char *realpath (char *, char *);
char *realpath (char *name, char *resolved_path)
{
  char *dest, *start, *end, *rpath_limit;
  rpath_limit = resolved_path + MAXPATHLEN;
#if defined (MSDOS) || defined (_WIN32) || defined (__EMX__)
  if (DOSDRIVE (name))
    {
      if (isslash (name [2]))
        {
          dest = resolved_path;
          *dest++ = *name++;
          *dest++ = *name++;
          if (*name) *dest++ = '/';
        }
      else
        {
          char tmp [3];
          tmp [0] = *name++;
          tmp [1] = *name++;
          tmp [2] = 0;

          /* @@@@@ FIXME: _fullpath is a private mingw routine, and
             _fixpath a private DJGPP routine (which will not be
             necessary in DJGPP versions after 2.02 because DJGPP
             will have realpath then -- or so they said...). I suppose they don't exist
             under EMX, but I don't know this system. There is
             probably something equivalent, to expand a path like
             `c:' to `c:\foo\bar', i.e., the current directory on
             some drive (but not necessarily the current drive, so
             we can't use getcwd() ). */
          /*#if (DJGPP == 2) && (DJGPP_MINOR <= 2)*/
          #ifdef DJGPP
          #define _fullpath(resolved_path, path, size) _fixpath (path, resolved_path)
          #endif
          _fullpath (resolved_path, tmp, MAXPATHLEN);

          dest = _p_strscan (resolved_path, '\0');
        }
    }
  else if (isslash (*name) && getcwd (resolved_path, MAXPATHLEN))
    dest = resolved_path + 3;
  else
#endif
       if (!isslash (*name) && getcwd (resolved_path, MAXPATHLEN))
    dest = _p_strscan (resolved_path, '\0');
  else
    {
      dest = resolved_path;
      *dest++ = '/';
    }
  for (start = end = name; *start; start = end)
    {
      while (isslash (*start)) ++start;
      for (end = start; *end && !isslash (*end); ++end);
      if (end == start)
        break;
      else if (strncmp (start, ".", end - start) == 0)
        /* nothing */;
      else if (strncmp (start, "..", end - start) == 0) {
        if (dest > resolved_path + 1 + 2 * DOSDRIVE (resolved_path))
          while (!isslash ((--dest)[-1]));
      } else {
        if (!isslash (dest[-1])) *dest++ = '/';
        if (dest + (end - start) >= rpath_limit) return NULL;
        _p_move (start, dest, end - start);
        dest += end - start;
      }
    }
  if (dest > resolved_path + 1 + 2 * DOSDRIVE (resolved_path) && isslash (dest[-1])) --dest;
  *dest = '\0';
  return resolved_path;
}
#endif

char *
_p_realpath (char *Path)
{
  /* some libc's (Linux libc5) don't seem to like static buffers!?! */
  static/*!!!*/ char Buf [MAXPATHLEN];
  char TempBuf [MAXPATHLEN];
  _p_strlcpy (Buf, realpath (Path, TempBuf), MAXPATHLEN - 1);
  return Buf;
}

const char
*_p_c_executable_path (char *Buffer)
{
  #if defined (HAVE_EXECUTABLE_PATH) || defined (executable_path)
  char *result = executable_path (Buffer);
  if (result)
    return result;
  else
  #endif
    return _p_argv [0];
  (void) Buffer;
}

#if (defined (HAVE_UTIME_H) || defined (HAVE_SYS_UTIME_H)) && defined (HAVE_UTIME)
#ifdef HAVE_UTIME_H
#include <utime.h>
#else
#include <sys/utime.h>
#endif
#else
#ifndef time_t
#define time_t int
#endif
struct utimbuf
{
  time_t actime, modtime;
};
int utime (const char *, struct utimbuf *);
int utime (const char *filename, struct utimbuf *buf)
{
  return -1;
}
#endif
void
_p_set_file_time (FDR File, UnixTimeType Time)
{
  struct utimbuf utim;
  if (_p_inoutres) return;
  DO_RETURN_ADDRESS (check_binding (File));
  if (_p_inoutres) return;
  if (!tst_EXTB (File) || !m_EXTNAM (File))
    IOERROR_STRING (486, m_NAM (File),); /* `SetFTime': file `%s' has no external name */
  utim.actime = utim.modtime = (time_t) Time;
  if (utime (m_EXTNAM (File), &utim))
    IOERROR_FILE (487, File,); /* cannot set time for %s */
}

#if defined (HAVE_STATVFS) || defined (HAVE_STATFS)
void
_p_statfs (char *path, statfsbuf *buf)
{
  int result;
  if (_p_inoutres) return;
  {
#ifdef HAVE_STATVFS
    struct statvfs b;
    result = statvfs (path, &b);
    buf -> f_block_size    = (LongestInt) b.f_frsize;
    buf -> f_blocks_total  = (LongestInt) b.f_blocks;
    buf -> f_blocks_free   = (LongestInt) b.f_bavail;
    buf -> f_files_total   = (int) b.f_files;
    buf -> f_files_free    = (int) b.f_favail;
#else
    struct statfs b;
    result = statfs (path, &b);
    buf -> f_block_size    = (LongestInt) b.f_bsize;
    buf -> f_blocks_total  = (LongestInt) b.f_blocks;
    buf -> f_blocks_free   = (LongestInt) b.f_bavail;
    buf -> f_files_total   = (int) b.f_files;
    buf -> f_files_free    = (int) b.f_ffree;
#endif
    if (result != 0)
      {
        buf -> f_block_size = 0;
        buf -> f_blocks_total = 0;
        buf -> f_blocks_free = 0;
        buf -> f_files_total = 0;
        buf -> f_files_free = 0;
        switch (errno)
          {
            case ENOSYS : IOERROR (489,); /* `StatFS': function not supported */
            default     : IOERROR_STRING (490, path,); /* cannot lstat file system `%s' */
          }
      }
  }
}
#endif

#if defined (HAVE_FNMATCH_H) && defined (HAVE_FNMATCH)
#include <fnmatch.h>
int _p_have_fnmatch = 1;
int
_p_fnmatch (const char *pattern, const char *name)
{
  return fnmatch (pattern, name, FNM_PATHNAME | FNM_PERIOD);
}
#else
int _p_have_fnmatch = 0;
int
_p_fnmatch (const char *pattern, const char *name)
{
  return -1;
}
#endif

#if defined (HAVE_GLOB_H) && defined (HAVE_GLOB)
#include <glob.h>
void
_p_globon (globbuffer *gbuf, char *pattern)
{
  int first = gbuf->result == NULL;
  if (!gbuf->buf)
    {
      gbuf->buf = _p_malloc (sizeof (glob_t));
      gbuf->count = 0;
    }
  if (glob (pattern, GLOB_MARK | (first ? 0 : GLOB_APPEND), NULL, (glob_t *) gbuf->buf) != 0 && first)
    {
      _p_free (gbuf->buf);
      gbuf->buf = 0;
      gbuf->count = 0;
      gbuf->result = 0;
    }
  else
    {
      gbuf->result = ((glob_t *) gbuf->buf)->gl_pathv;
      gbuf->count  = ((glob_t *) gbuf->buf)->gl_pathc;
    }
}

void
_p_globfree (globbuffer *gbuf)
{
  if (gbuf->buf)
    {
      globfree ((glob_t *) gbuf->buf);
      _p_free (gbuf->buf);
      gbuf->buf = NULL;
    }
  gbuf->count = 0;
  gbuf->result = NULL;
}
#else
void
_p_globon (globbuffer *gbuf, char *pattern)
{
  gbuf->count = 0;
}

void
_p_globfree (globbuffer *gbuf)
{
  gbuf->buf = NULL;
  gbuf->count = 0;
  gbuf->result = NULL;
}
#endif
