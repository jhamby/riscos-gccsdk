--- gcc/diagnostic.c.orig	2006-09-24 22:57:01.000000000 +0200
+++ gcc/diagnostic.c	2006-09-24 23:02:34.000000000 +0200
@@ -364,6 +364,15 @@
       pp_format (context->printer, &diagnostic->message);
       (*diagnostic_starter (context)) (context, diagnostic);
       pp_output_formatted_text (context->printer);
+#ifdef ERROR_THROWBACK
+      {
+	const char *s = pp_formatted_text (context->printer);
+	ERROR_THROWBACK (diagnostic->location.file,
+			 diagnostic->location.line,
+			 (diagnostic->kind == DK_WARNING) ? "warning" : NULL,
+			 s);
+      }
+#endif
       (*diagnostic_finalizer (context)) (context, diagnostic);
       pp_flush (context->printer);
       diagnostic_action_after_output (context, diagnostic);
