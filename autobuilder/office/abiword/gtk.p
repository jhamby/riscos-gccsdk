--- abi/src/wp/ap/unix/ap_UnixDialog_Tab.cpp.orig	2008-01-20 14:38:35.000000000 -0800
+++ abi/src/wp/ap/unix/ap_UnixDialog_Tab.cpp	2008-01-20 14:39:09.000000000 -0800
@@ -57,7 +57,7 @@
 /*!
 * FIXME remove this after we depend on gtk-2.6
 */
-gchar *
+/*gchar *
 gtk_combo_box_get_active_text (GtkComboBox *combo_box)
 {
   GtkTreeIter iter;
@@ -73,7 +73,7 @@
   }
   return text;
 }
-
+*/
 
 
 //! Event dispatcher for default tab width
