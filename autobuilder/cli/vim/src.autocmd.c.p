--- src/autocmd.c.orig	2019-02-04 12:14:45.000000000 -0800
+++ src/autocmd.c	2021-07-27 15:44:15.104528612 -0700
@@ -245,7 +245,7 @@
  * Relying on this value requires one to reset it prior calling
  * apply_autocmds_group.
  */
-int au_did_filetype INIT(= FALSE);
+extern int au_did_filetype;
 
 /*
  * The ID of the current group.  Group 0 is the default one.
