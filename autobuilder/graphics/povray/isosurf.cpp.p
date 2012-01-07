--- source/isosurf.cpp.orig	2004-08-03 00:11:36.000000000 +0100
+++ source/isosurf.cpp	2011-11-27 15:00:07.000000000 +0000
@@ -234,7 +234,7 @@
 		}
 
 		/*  METHOD 2   by R. Suzuki */
-		tmax = Depth2 = min(Depth2, BOUND_HUGE);
+		tmax = Depth2 = min(Depth2, static_cast<double>(BOUND_HUGE));
 		tmin = Depth1 = min(Depth2, Depth1);
 		if((tmax - tmin) < Isosrf->accuracy)
 			return (false);
@@ -818,12 +818,12 @@
 			}
 			else
 			{
-				DBL diff = (mginfo->eval_max / max(mginfo->eval_max - mginfo->eval_var, EPSILON));
+				DBL diff = (mginfo->eval_max / max(mginfo->eval_max - mginfo->eval_var, static_cast<double>(EPSILON)));
 
 				if((Isosrf->eval_param[0] > mginfo->eval_max) ||
 				   (Isosrf->eval_param[1] > diff))
 				{
-					mginfo->eval_cnt = max(mginfo->eval_cnt, 1.0); // make sure it won't be zero
+					mginfo->eval_cnt = max(mginfo->eval_cnt, static_cast<double>(1.0)); // make sure it won't be zero
 
 					WarningAt(0, fn->filename, fn->filepos.lineno, fn->filepos.offset,
 					          "Evaluate found a maximum gradient of %0.3f and an average\n"
@@ -834,7 +834,7 @@
 
 					if(opts.Options & VERBOSE)
 					{
-						diff = max(diff, 1.0); // prevent contradicting output
+						diff = max(diff, static_cast<double>(1.0)); // prevent contradicting output
 
 						Debug_Info("It is recommended to adjust the parameters of 'evaluate' to:\n"
 					               "First parameter less than %0.3f\n"
