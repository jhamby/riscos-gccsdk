--- generic/tclIOUtil.c.orig	2005-11-27 03:34:41.000000000 +0100
+++ generic/tclIOUtil.c	2006-02-25 14:54:26.000000000 +0100
@@ -20,6 +20,7 @@
  * RCS: @(#) $Id: tclIOUtil.c,v 1.77.2.23 2005/11/27 02:34:41 das Exp $
  */
 
+#include <sys/stat.h>
 #include "tclInt.h"
 #include "tclPort.h"
 #ifdef MAC_TCL
