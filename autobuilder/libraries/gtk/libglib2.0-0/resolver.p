--- gio/gresolver.c.orig	2013-08-07 15:34:32.000000000 +0100
+++ gio/gresolver.c	2013-10-09 12:21:28.189944500 +0100
@@ -244,7 +244,7 @@
       if (st.st_mtime != resolver->priv->resolv_conf_timestamp)
         {
           resolver->priv->resolv_conf_timestamp = st.st_mtime;
-          res_init ();
+          //res_init ();
           g_signal_emit (resolver, signals[RELOAD], 0);
         }
     }
