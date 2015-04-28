--- flexdef.h.orig	2014-03-27 01:46:44.000000000 +1300
+++ flexdef.h	2015-04-15 20:22:35.759992000 +1200
@@ -1188,6 +1188,7 @@
 extern bool filter_apply_chain PROTO((struct filter * chain));
 extern int filter_truncate (struct filter * chain, int max_len);
 extern int filter_tee_header PROTO((struct filter *chain));
+extern int filter_empty PROTO((struct filter *chain));
 extern int filter_fix_linedirs PROTO((struct filter *chain));
 
 
