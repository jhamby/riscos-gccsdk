--- gio/gresolver.c.orig	2009-09-28 14:57:04.000000000 -0700
+++ gio/gresolver.c	2009-09-28 14:57:13.000000000 -0700
@@ -205,7 +205,7 @@
       if (st.st_mtime != resolver->priv->resolv_conf_timestamp)
         {
           resolver->priv->resolv_conf_timestamp = st.st_mtime;
-          res_init ();
+          //res_init ();
           g_signal_emit (resolver, signals[RELOAD], 0);
         }
     }
