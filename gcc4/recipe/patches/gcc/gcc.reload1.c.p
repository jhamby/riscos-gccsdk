Index: gcc/reload1.c
===================================================================
--- gcc/reload1.c	(revision 209821)
+++ gcc/reload1.c	(working copy)
@@ -5566,6 +5566,14 @@
 	    || reg_mentioned_p (rld[r2].in, rld[r1].in)))
     return false;
 
+  /* The following loop assumes that r1 is the reload that feeds r2.  */
+  if (r1 > r2)
+    {
+      int tmp = r2;
+      r2 = r1;
+      r1 = tmp;
+    }
+
   for (i = 0; i < n_reloads; i ++)
     /* Look for input reloads that aren't our two */
     if (i != r1 && i != r2 && rld[i].in)
