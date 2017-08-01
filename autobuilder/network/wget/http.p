--- src/http.c.orig	2017-02-11 23:53:50.000000000 +1300
+++ src/http.c	2017-08-01 04:16:32.930315088 +1200
@@ -3979,6 +3979,8 @@
   resp_free (&resp);
   request_free (&req);
 
+
+
   return retval;
 }
 
@@ -4524,6 +4526,10 @@
                          number_to_static_string (hstat.len),
                          number_to_static_string (hstat.contlen),
                          hstat.local_file, count);
+/* otherwise in RISC OS the -O option (file) gives a 'file
+already open' error when using with the -k option. */
+  if (output_stream)
+    fclose (output_stream);
             }
           ++numurls;
           total_downloaded_bytes += hstat.rd_size;
