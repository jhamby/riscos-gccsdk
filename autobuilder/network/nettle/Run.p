Index: !Nettle/!Run,feb
===================================================================
RCS file: /cvsroot/nettle/nettle/!Nettle/!Run,feb,v
retrieving revision 1.18
diff -u -r1.18 !Run,feb
--- !Nettle/!Run,feb	11 Apr 2010 21:39:02 -0000	1.18
+++ !Nettle/!Run,feb	31 May 2012 23:23:29 -0000
@@ -32,5 +32,5 @@
 
 RmEnsure WindowManager 3.98 Error Nettle requires the Nested WIMP window manager
 
-WimpSlot -min 256K -max 256K
+WimpSlot -min 1600K -max 1600K
 Run <Nettle$Dir>.!RunImage %*0 ><Nettle$Dir>.stdout 2><Nettle$Dir>.stderr
Index: c/sockwatch
===================================================================
RCS file: /cvsroot/nettle/nettle/c/sockwatch,v
retrieving revision 1.8
diff -u -r1.8 sockwatch
--- c/sockwatch	29 Aug 2010 19:45:29 -0000	1.8
+++ c/sockwatch	31 May 2012 23:23:29 -0000
@@ -19,7 +19,9 @@
 #include "sys/types.h"
 #include "sys/ioctl.h"
 #include "sys/socket.h"
-#ifndef __UNIXLIB_TYPES_H
+#ifdef __UNIXLIB_TYPES_H
+#include <unixlib/local.h>
+#else
 #include "socklib.h"
 #include "sys/filio.h"
 #endif
@@ -53,7 +55,7 @@
 
 #ifdef __UNIXLIB_TYPES_H
   /* get the underlying RISC OS socket handle from the Unixlib socket */
-  socket = __unixlib_getdev(socket);
+  socket = __get_ro_socket(socket);
 #endif
 
   if ( newinternet )
@@ -78,7 +80,7 @@
     return;
 
 #ifdef __UNIXLIB_TYPES_H
-  socket = __unixlib_getdev(socket);
+  socket = __get_ro_socket(socket);
 #endif
 
   if ( newinternet )
