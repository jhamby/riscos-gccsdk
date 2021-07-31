--- gcc/diagnostic.c.orig	2008-12-13 20:42:10.000000000 +0100
+++ gcc/diagnostic.c	2008-12-13 20:41:42.000000000 +0100
@@ -41,6 +41,9 @@ Software Foundation, 51 Franklin Street,
 #include "langhooks.h"
 #include "langhooks-def.h"
 #include "opts.h"
+#ifdef TARGET_ERROR_THROWBACK
+#include "cpplib.h"
+#endif
 
 
 /* Prototypes.  */
@@ -324,6 +327,19 @@ default_diagnostic_finalizer (diagnostic
   pp_destroy_prefix (context->printer);
 }
 
+#ifdef TARGET_ERROR_THROWBACK
+static void ATTRIBUTE_PRINTF_4
+diagnostic_throwback_trampoline (int lvl, const char *file, int line,
+                                 const char *s, ...)
+{
+  va_list va;
+
+  va_start (va, s);
+  TARGET_ERROR_THROWBACK (lvl, file, line, s, &va);
+  va_end (va);
+}
+#endif
+
 /* Report a diagnostic message (an error or a warning) as specified by
    DC.  This function is *the* subroutine in terms of which front-ends
    should implement their specific diagnostic handling modules.  The
@@ -364,6 +380,14 @@ diagnostic_report_diagnostic (diagnostic
       pp_format (context->printer, &diagnostic->message);
       (*diagnostic_starter (context)) (context, diagnostic);
       pp_output_formatted_text (context->printer);
+#ifdef TARGET_ERROR_THROWBACK
+      diagnostic_throwback_trampoline ((diagnostic->kind == DK_WARNING)
+				         ? CPP_DL_WARNING : CPP_DL_ERROR,
+		                       diagnostic->location.file,
+		                       diagnostic->location.line,
+		                       "%s",
+				       pp_formatted_text (context->printer));
+#endif
       (*diagnostic_finalizer (context)) (context, diagnostic);
       pp_flush (context->printer);
       diagnostic_action_after_output (context, diagnostic);
