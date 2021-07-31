Index: gcc/opts-common.c
===================================================================
--- gcc/opts-common.c	(revision 181309)
+++ gcc/opts-common.c	(working copy)
@@ -761,7 +761,8 @@
   if (cl_options [next_opt_idx].neg_index == opt_idx)
     return true;
 
-  if (cl_options [next_opt_idx].neg_index != orig_next_opt_idx)
+  if (cl_options [next_opt_idx].neg_index >= 0
+      && cl_options [next_opt_idx].neg_index != orig_next_opt_idx)
     return cancel_option (opt_idx, cl_options [next_opt_idx].neg_index,
 			  orig_next_opt_idx);
 
