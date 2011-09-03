Index: gcc/ada/raise-gcc.c
===================================================================
--- gcc/ada/raise-gcc.c	(revision 178499)
+++ gcc/ada/raise-gcc.c	(working copy)
@@ -41,6 +41,7 @@
    we include it.  */
 #ifndef FIRST_PSEUDO_REGISTER
 #include "coretypes.h"
+#include <stdio.h>
 #include "tm.h"
 #endif
 #include <sys/stat.h>
@@ -1102,7 +1103,11 @@
 	return __gnat_handle_vms_condition (version_arg, phases_arg);
       #endif
 
+#ifdef __ARM_EABI_UNWINDER__
+      return _URC_FAILURE;
+#else
       return _URC_FATAL_PHASE1_ERROR;
+#endif
     }
 
   db_indent (DB_INDENT_RESET);
