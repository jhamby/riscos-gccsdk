--- gcc/version.c.orig	2007-07-31 01:05:56.000000000 +0200
+++ gcc/version.c	2007-07-31 01:05:40.000000000 +0200
@@ -8,7 +8,7 @@
    in parentheses.  You may also wish to include a number indicating
    the revision of your modified compiler.  */
 
-#define VERSUFFIX ""
+#define VERSUFFIX " (GCCSDK GCC 4.1.1 Release 3 Development)"
 
 /* This is the location of the online document giving instructions for
    reporting bugs.  If you distribute a modified version of GCC,
@@ -17,7 +17,7 @@
    forward us bugs reported to you, if you determine that they are
    not bugs in your modifications.)  */
 
-const char bug_report_url[] = "<URL:http://gcc.gnu.org/bugs.html>";
+const char bug_report_url[] = "<URL:http://gccsdk.riscos.info/>";
 
 /* The complete version string, assembled from several pieces.
    BASEVER, DATESTAMP, and DEVPHASE are defined by the Makefile.  */
