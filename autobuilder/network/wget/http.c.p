--- src/http.c.orig	2018-01-21 01:42:05.000000000 +1300
+++ src/http.c	2018-03-27 01:43:22.464808791 +1300
@@ -4688,6 +4688,8 @@
                          number_to_static_string (hstat.len),
                          number_to_static_string (hstat.contlen),
                          hstat.local_file, count);
+  if (output_stream)
+    fclose (output_stream);
             }
           ++numurls;
           total_downloaded_bytes += hstat.rd_size;
@@ -4721,6 +4723,8 @@
                              "%s URL:%s [%s] -> \"%s\" [%d]\n",
                              tms, u->url, number_to_static_string (hstat.len),
                              hstat.local_file, count);
+  if (output_stream)
+    fclose (output_stream);
                 }
               ++numurls;
               total_downloaded_bytes += hstat.rd_size;
