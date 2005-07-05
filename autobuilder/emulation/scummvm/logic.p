Index: queen/logic.cpp
===================================================================
RCS file: /cvsroot/scummvm/scummvm/queen/logic.cpp,v
retrieving revision 1.218
diff -u -r1.218 logic.cpp
--- queen/logic.cpp	24 Jun 2005 15:23:02 -0000	1.218
+++ queen/logic.cpp	5 Jul 2005 21:50:24 -0000
@@ -1232,7 +1232,7 @@
 		// update screen scrolling
 		_vm->display()->horizontalScroll(mx);
 
-		// update bobs position / frame
+		// update Bob's position / frame
 		joe->x = piton->x = 3 * mx / 4 + 200;
 		joe->frameNum = mx / 36 + 45;
 
