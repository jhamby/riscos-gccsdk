Index: !LibPkg/libpkg/zipfile.cc
===================================================================
--- !LibPkg/libpkg/zipfile.cc	(revision 128)
+++ !LibPkg/libpkg/zipfile.cc	(working copy)
@@ -3,6 +3,7 @@
 // Distribution and use are subject to the GNU Lesser General Public License,
 // a copy of which may be found in the file !LibPkg.Copyright.
 
+#include <string.h>
 #include "zlib.h"
 
 #include "libpkg/zipfile.h"
