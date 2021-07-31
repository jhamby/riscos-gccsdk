--- automake.in	2009-12-08 18:15:40.000000000 +0000
+++ automake.in	2018-04-08 12:43:49.334977950 +0100
@@ -4110,7 +4110,7 @@
 sub substitute_ac_subst_variables ($)
 {
   my ($text) = @_;
-  $text =~ s/\${([^ \t=:+{}]+)}/&substitute_ac_subst_variables_worker ($1)/ge;
+  $text =~ s/\$\{([^ \t=:+{}]+)\}/&substitute_ac_subst_variables_worker ($1)/ge;
   return $text;
 }
 
