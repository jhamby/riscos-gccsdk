--- src/http.c.orig	2017-02-11 23:53:50.000000000 +1300
+++ src/http.c	2017-07-26 22:03:16.359624115 +1200
@@ -3979,6 +3979,11 @@
   resp_free (&resp);
   request_free (&req);
 
+/* otherwise in RISC OS the -O option (file) gives a 'file
+already open' error when using with the -k option. */
+  if (output_stream)
+    fclose (output_stream);
+
   return retval;
 }
 
