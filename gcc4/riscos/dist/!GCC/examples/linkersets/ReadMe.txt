Linker sets example code
========================

Introduction
------------
Linker sets were introduced to drlink to provide a means by which structures
in different source files can be linked together at link-time. This feature
is required by the CFront C++ tool and most likely by other features. It may
be used in user-code to provide a simple extension mechanism. It is expected
that anyone reading this document and using the supplied source code has
read the relevant section of the drlink documentation and has understood
what the linker-sets provides.


Sources
-------
The source files supplied are taken from a program which required a simple
interactive command line for which a number of commands could be issued.
Example sources for linker-sets had been promised, and so the code was written
to be reusable for this purpose.

Each source file may have a linker set (__linkinteract) which describes its
features. For our purposes, we only have a pointer to a command table as
part of this structure. If necessary, we could name the group of commands by
including a name pointer within the structure.

The c.version source provides a simple example of the use of this library.
The only command it provides is a simple display of the tool's version
number, but any funtion can be used here. In order to extend this particular
library, more elements would be added to the 'commands' array, following the
"version" command.

The c.interact source provides the parser for the interactive shell, and
holds the head of the linker-set chain (__headinteract). The end of the file
is very similar to that given in the c.version source, and intentionally so.

The interact_help function, which displays the known command names and their
help, is a simple example of iterating through the linker sets (and
subsequently iterating through the commands array). The iteration sequence
is almost identical to that in interact_command, which processes the command
and dispatches it to the correct handler.

The h.interact header contains the structure definition for the linker-set
and the command array. Because the linker-set array type (interactlink_t) is
used by all the sources which use the linker-set, it would need to be
included by them. Alternatively the structure might be declared within each
source individually. Whilst this requires less dependencies between the
sources, it is much more prone to error and is not recommended.


License, warranty, etc
----------------------
The files supplied here are supplied completely free of license. They may be
used for any purposes whatsoever with no restrictions or requirements placed
upon them. Users may place restrictive licenses such as the GPL, or
proprietary licenses, or whatever. If supplied with drlink, this source is
not covered by the restrictive license (GPL) which drlink is placed under.

The sources and this documentation are supplied with no warranty. If the
world ends, don't come crying to me.


Author
------
The example sources were written by Justin Fletcher.
