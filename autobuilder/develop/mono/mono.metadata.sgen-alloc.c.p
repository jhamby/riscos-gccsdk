--- mono/metadata/sgen-alloc.c.orig	2013-07-30 02:50:25.000000000 +0100
+++ mono/metadata/sgen-alloc.c	2013-09-11 20:14:23.864543236 +0100
@@ -377,6 +377,18 @@
 		if (G_LIKELY (new_next < real_end)) {
 			TLAB_NEXT = new_next;
 
+#ifdef __riscos__
+			// I added this because a call to a SWI from managed code which marshalled
+			// results into a struct containing a null reference to a class would dump
+			// the results over (previously) zero'd memory that would later become the
+			// null reference exception object.
+			// Obviously, the offending C# code would be written so that the struct
+			// would not contain a null reference, however, the null reference should
+			// still be reported correctly rather than crash.
+			// I don't consider this to be a bug, more an unfortunate consquence of the
+			// fact that the SWI is called before Mono can check the destination.
+			memset (p, 0, size);
+#endif
 			/* Second case, we overflowed temp end */
 			if (G_UNLIKELY (new_next >= TLAB_TEMP_END)) {
 				sgen_set_nursery_scan_start (new_next);
