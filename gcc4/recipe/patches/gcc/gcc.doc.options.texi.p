Index: gcc/doc/options.texi
===================================================================
--- gcc/doc/options.texi	(revision 167269)
+++ gcc/doc/options.texi	(working copy)
@@ -432,6 +432,11 @@
 controls whether the option is accepted and whether it is printed in
 the @option{--help} output.
 
+@item Explicit
+If the option is a @code{MASK} option, create an additional variable
+@var{var}_explicit which is available to hold whether the option was
+set explicitly or implicitly via another option.
+
 @item Save
 Build the @code{cl_target_option} structure to hold a copy of the
 option, add the functions @code{cl_target_option_save} and
