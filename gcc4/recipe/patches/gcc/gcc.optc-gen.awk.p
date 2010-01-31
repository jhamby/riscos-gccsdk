--- gcc/optc-gen.awk.orig	2006-05-17 20:38:58.000000000 +0200
+++ gcc/optc-gen.awk	2008-06-09 22:39:19.000000000 +0200
@@ -80,6 +80,8 @@ for (i = 0; i < n_opts; i++) {
 	print "/* Set by -" opts[i] "."
 	print "   " help[i] "  */"
 	print var_type(flags[i]) name init ";"
+	if (flag_set_p("Explicit", flags[i]))
+		print var_type(flags[i]) name "_explicit;"
 	print ""
 
 	var_seen[name] = 1;
