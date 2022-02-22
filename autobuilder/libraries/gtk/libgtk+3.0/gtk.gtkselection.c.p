--- gtk/gtkselection.c.orig	2020-06-11 03:45:39.000000000 +0100
+++ gtk/gtkselection.c	2020-11-26 15:34:17.192018636 +0000
@@ -116,6 +116,10 @@
 #include "broadway/gdkbroadway.h"
 #endif
 
+#ifdef GDK_WINDOWING_RISCOS
+#include "riscos/gdkriscos.h"
+#endif
+
 #undef DEBUG_SELECTION
 
 /* Maximum size of a sent chunk, in bytes. Also the default size of
@@ -1149,6 +1153,42 @@
     owner_window = NULL;
 #endif
 
+#ifdef GDK_WINDOWING_RISCOS
+  if (target == gdk_atom_intern_static_string ("text/plain;charset=utf-8"))
+    {
+      gchar *data;
+      if (!gdk_riscos_clipboard_is_text_available(display) ||
+	  (data = gdk_riscos_clipboard_request_text(display)) == NULL)
+	{
+	  gtk_selection_retrieval_report (info,
+					  target,
+					  8,
+					  "",
+					  0,
+					  time_);
+	  return FALSE;
+	}
+
+      gtk_selection_retrieval_report (info,
+				      text_plain_locale_atom,
+				      8,
+				      (guchar *)data,
+				      strlen(data) + 1,
+				      time_);
+      g_free (data);
+	  
+      g_slice_free (GtkRetrievalInfo, info);
+      return TRUE;
+    }
+
+  gtk_selection_retrieval_report (info,
+				  target,
+				  8,
+				  "",
+				  0,
+				  time_);
+  return TRUE;
+#else
   if (owner_window != NULL)
     {
       GtkWidget *owner_widget;
@@ -1213,6 +1253,7 @@
   g_source_set_name_by_id (id, "[gtk+] gtk_selection_retrieval_timeout");
   
   return TRUE;
+#endif
 }
 
 /**
