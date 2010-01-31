--- gcc/opth-gen.awk.orig	2005-06-25 04:02:01.000000000 +0200
+++ gcc/opth-gen.awk	2008-06-09 22:40:15.000000000 +0200
@@ -69,6 +69,12 @@ for (i = 0; i < n_opts; i++) {
 		continue;
 
 	print "extern " var_type(flags[i]) name ";"
+
+	if (flag_set_p("Explicit", flags[i]) && !(name in var_explicit)) {
+	    var_explicit[name] = 1;
+	    print "extern " var_type(flags[i]) name "_explicit;";
+	}
+
 }
 print ""
 
