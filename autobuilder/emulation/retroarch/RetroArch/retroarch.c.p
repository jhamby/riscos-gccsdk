--- retroarch.c.orig	2020-02-15 12:13:54.665461581 +0000
+++ retroarch.c	2020-02-15 12:14:27.044805014 +0000
@@ -15760,6 +15760,7 @@
    return ret;
 }
 
+#pragma GCC optimize("O1")
 void menu_input_post_iterate(int *ret, unsigned action)
 {
    menu_input_t *menu_input  = &menu_input_state;
@@ -15798,7 +15799,7 @@
       *ret = menu_input_pointer_post_iterate(current_time, cbs, &entry, action);
    }
 }
-
+#pragma GCC optimize("O3")
 /**
  * input_menu_keys_pressed:
  *
