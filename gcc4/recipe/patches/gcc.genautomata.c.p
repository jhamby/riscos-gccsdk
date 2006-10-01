--- gcc/genautomata.c.orig	2006-09-24 23:11:01.000000000 +0200
+++ gcc/genautomata.c	2006-09-24 23:11:11.000000000 +0200
@@ -10040,11 +10040,11 @@
       output_automaton_descriptions ();
       if (progress_flag)
 	fprintf (stderr, "done\n");
-      output_statistics (output_description_file);
+      /*output_statistics (output_description_file);*/
     }
-  output_statistics (stderr);
+  /*output_statistics (stderr); */
   ticker_off (&output_time);
-  output_time_statistics (stderr);
+  /*output_time_statistics (stderr);*/
   finish_states ();
   finish_arcs ();
   finish_automata_lists ();
