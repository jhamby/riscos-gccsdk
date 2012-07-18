Index: !RiscPkg/install_dbox.cc
===================================================================
--- !RiscPkg/install_dbox.cc	(revision 112)
+++ !RiscPkg/install_dbox.cc	(working copy)
@@ -3,6 +3,8 @@
 // Distribution and use are subject to the GNU General Public License,
 // a copy of which may be found in the file !RiscPkg.Copyright.
 
+#include <stdlib.h>
+
 #include "rtk/os/os.h"
 #include "rtk/os/wimp.h"
 #include "rtk/desktop/application.h"
