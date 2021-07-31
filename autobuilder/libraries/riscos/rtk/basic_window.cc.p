Index: !RTK/rtk/desktop/basic_window.cc
===================================================================
--- !RTK/rtk/desktop/basic_window.cc	(revision 430)
+++ !RTK/rtk/desktop/basic_window.cc	(working copy)
@@ -481,9 +481,9 @@
 			{
 				ic->deliver_wimp_block(wimpcode,wimpblock);
 			}
-			else if (component* target=find_focus_target())
+			else if (component* target2=find_focus_target())
 			{
-				events::key_pressed ev(*target,wimpblock);
+				events::key_pressed ev(*target2,wimpblock);
 				ev.post();
 			}
 			else
