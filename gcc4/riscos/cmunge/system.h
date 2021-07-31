/*
 * Copyright (C) 2003 Justin Fletcher
 */

#ifndef SYSTEM_H
#define SYSTEM_H

/* This function is here to provide escaping for the invoked comand when used
   under linux. Under RISC OS we can happily invoke the command directly
   without any problems.

   Basically this gets around the issue of using 'system variables' under
   linux. Trying to issue a command with (for example) -I<Lib$Dir> in its
   path will fail on linux because the < is taken as a redirection. Such
   arguments will be quoted and $'s escaped where necessary.
 */

int our_system(const char *cmdtxt);

#endif
