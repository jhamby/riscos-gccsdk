--- gas/stabs.c.orig	2008-12-10 00:22:37.000000000 +0100
+++ gas/stabs.c	2008-12-10 00:23:11.000000000 +0100
@@ -666,8 +666,9 @@ stabs_generate_asm_func (const char *fun
     }
 
   as_where (&file, &lineno);
-  asprintf (&buf, "\"%s:F1\",%d,0,%d,%s",
-	    funcname, N_FUN, lineno + 1, startlabname);
+  if (asprintf (&buf, "\"%s:F1\",%d,0,%d,%s",
+		funcname, N_FUN, lineno + 1, startlabname) == -1)
+    as_fatal ("%s", xstrerror (errno));
   input_line_pointer = buf;
   s_stab ('s');
   free (buf);
@@ -692,7 +693,8 @@ stabs_generate_asm_endfunc (const char *
   ++label_count;
   colon (sym);
 
-  asprintf (&buf, "\"\",%d,0,0,%s-%s", N_FUN, sym, startlabname);
+  if (asprintf (&buf, "\"\",%d,0,0,%s-%s", N_FUN, sym, startlabname) == -1)
+    as_fatal ("%s", xstrerror (errno));
   input_line_pointer = buf;
   s_stab ('s');
   free (buf);
