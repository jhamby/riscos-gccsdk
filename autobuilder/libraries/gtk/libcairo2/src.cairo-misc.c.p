--- src/cairo-misc.c.orig	2012-03-29 11:05:32.000000000 +0100
+++ src/cairo-misc.c	2013-05-08 20:29:38.000000000 +0100
@@ -156,6 +156,10 @@
 	return "invalid operation during mesh pattern construction";
     case CAIRO_STATUS_DEVICE_FINISHED:
 	return "the target device has been finished";
+#ifdef __riscos__
+    case CAIRO_STATUS_RISCOS_ERROR:
+	return "a RISC OS SWI returned an error";
+#endif
     default:
     case CAIRO_STATUS_LAST_STATUS:
 	return "<unknown error status>";
