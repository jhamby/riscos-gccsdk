/*
Support routine for pipe.pas

Copyright (C) 1998-2000 Free Software Foundation, Inc.

Author: Frank Heckenbach <frank@pascal.gnu.de>

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

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

/* Keep this consistent with the one in pipe.pas */
#if !(defined(MSDOS) || defined(__MINGW32__))

int _p_pipe (char *path, char *argv [], char *envp [], int *pipe_stdin, int *pipe_stdout, int *pipe_stderr, void (*ChildProc) ());
int _p_pipe (char *path, char *argv [], char *envp [], int *pipe_stdin, int *pipe_stdout, int *pipe_stderr, void (*ChildProc) ())
{
  int executable = *path && access (path, X_OK) == 0, fd_stdout [2], fd_stderr [2], fd_stdin [2];
  pid_t pid;
  if ((*path && !executable)
      || (pipe_stdout && pipe (fd_stdout))
      || (pipe_stderr && pipe_stderr != pipe_stdout && pipe (fd_stderr))
      || (pipe_stdin && pipe (fd_stdin))
      || ((pid = fork ()) < 0))
    return - 1;
  if (pid)
    {
      if (pipe_stdout)
        {
          close (fd_stdout [1]);
          *pipe_stdout = fd_stdout [0];
        }
      if (pipe_stderr && pipe_stderr != pipe_stdout)
        {
          close (fd_stderr [1]);
          *pipe_stderr = fd_stderr [0];
        }
      if (pipe_stdin)
        {
          close (fd_stdin [0]);
          *pipe_stdin = fd_stdin [1];
        }
      return pid;
    }
  else
    {
      if (pipe_stdout)
        {
          dup2  (fd_stdout [1], 1);
          close (fd_stdout [0]);
          close (fd_stdout [1]);
        }
      if (pipe_stderr)
        {
          if (pipe_stderr != pipe_stdout)
            {
              dup2  (fd_stderr [1], 2);
              close (fd_stderr [0]);
              close (fd_stderr [1]);
            }
          else
            dup2 (1, 2);
        }
      if (pipe_stdin)
        {
          dup2  (fd_stdin [0], 0);
          close (fd_stdin [0]);
          close (fd_stdin [1]);
        }
      if (ChildProc) ChildProc ();
      if (executable)
        {
          execve (path, argv, envp);
          _exit (127);
        }
      else
        _exit (0);
      return - 1;
    }
}

#else

#include <fcntl.h>
#include <process.h>

int _p_pipe (char *path, char *argv [], char *envp [], char *name_stdin, char *name_stdout, char *name_stderr, void (*ChildProc) ());
int _p_pipe (char *path, char *argv [], char *envp [], char *name_stdin, char *name_stdout, char *name_stderr, void (*ChildProc) ())
{
  int executable = *path && !access (path, X_OK), save_stdin = - 1, save_stdout = - 1, save_stderr = - 1, h, r;
  if (*path && !executable) return - 1;
  if (name_stdin)
    {
      save_stdin = dup (0);
      h = open (name_stdin, O_RDONLY, 0777);
      dup2 (h, 0);
      close (h);
    }
  if (name_stdout)
    {
      save_stdout = dup (1);
      h = open (name_stdout, O_WRONLY | O_CREAT, 0777);
      dup2 (h, 1);
      close (h);
    }
  if (name_stderr)
    {
      save_stderr = dup (2);
      if (name_stderr != name_stdout)
        {
          h = open (name_stderr, O_WRONLY | O_CREAT, 0777);
          dup2 (h, 2);
          close (h);
        }
      else
        dup2 (1, 2);
    }
  if (ChildProc) ChildProc ();
  if (executable)
    r = spawnve (P_WAIT, path, argv, envp);
  else
    r = 0;
  if (name_stdin)
    {
      dup2 (save_stdin, 0);
      close (save_stdin);
    }
  if (name_stdout)
    {
      dup2 (save_stdout, 1);
      close (save_stdout);
    }
  if (name_stderr)
    {
      dup2 (save_stderr, 2);
      close (save_stderr);
    }
  return r;
}

#endif
