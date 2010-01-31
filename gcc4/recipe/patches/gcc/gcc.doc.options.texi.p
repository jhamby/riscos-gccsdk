--- gcc/doc/options.texi.orig	2006-05-17 20:38:58.000000000 +0200
+++ gcc/doc/options.texi	2008-06-09 22:36:35.000000000 +0200
@@ -215,4 +215,9 @@ The option should only be accepted if pr
 option will be present even if @var{cond} is false; @var{cond} simply
 controls whether the option is accepted and whether it is printed in
 the @option{--help} output.
+
+@item Explicit
+If the option is a @code{MASK} option, create an additional variable
+@var{var}_explicit which is available to hold whether the option was
+set explicitly or implicitly via another option.
 @end table
