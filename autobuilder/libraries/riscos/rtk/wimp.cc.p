Index: !RTK/rtk/os/wimp.cc
===================================================================
--- !RTK/rtk/os/wimp.cc	(revision 430)
+++ !RTK/rtk/os/wimp.cc	(working copy)
@@ -3,6 +3,8 @@
 // Distribution and use are subject to the GNU Lesser General Public License,
 // a copy of which may be found in the file !RTK.Copyright.
 
+#include <string.h>
+
 #include "rtk/swi/wimp.h"
 #include "rtk/os/call_swi.h"
 #include "rtk/os/wimp.h"
