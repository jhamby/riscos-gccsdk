--- src/cairo-misc.c.orig	2019-11-04 11:00:55.136950757 +0000
+++ src/cairo-misc.c	2019-11-04 11:55:07.407939689 +0000
@@ -173,6 +173,10 @@
 	return "error occurred in the Windows Graphics Device Interface";
     case CAIRO_STATUS_TAG_ERROR:
 	return "invalid tag name, attributes, or nesting";
+#ifdef __riscos__
+    case CAIRO_STATUS_RISCOS_ERROR:
+	return "a RISC OS SWI returned an error";
+#endif
     default:
     case CAIRO_STATUS_LAST_STATUS:
 	return "<unknown error status>";
