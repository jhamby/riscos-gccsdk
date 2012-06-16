This is an autobuilder package for CryptRandom, which is now hosted on
riscos.info subversion.

IMPORTANT NOTE:
CryptRandom is a module, and needs a module version of OSLib to build.  That
means OSLib needs building in the autobuilder with ELFOBJECTTYPE=MODULE
set.  It won't build with standard settings - you get errors about floating
point:
http://www.riscos.info/pipermail/gcc/2011-November/005560.html

Until the autobuilder supports multiple library targets, this means it won't
build by default.
