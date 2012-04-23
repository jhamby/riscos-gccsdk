--- configure.orig	2012-03-19 07:58:04.000000000 +0000
+++ configure	2012-04-23 14:18:07.000000000 +0100
@@ -3051,7 +3051,7 @@
 	break;;
   esac
 done
-test "$ac_cv_exeext" = no && ac_cv_exeext=
+test "$ac_cv_exeext" = no && ac_cv_exeext=,e1f;
 
 else
   ac_file=''
@@ -10719,7 +10719,7 @@
   #include <string.h>
 fi
 
-for ac_func in memcpy lchown
+for ac_func in memcpy
 do :
   as_ac_var=`$as_echo "ac_cv_func_$ac_func" | $as_tr_sh`
 ac_fn_c_check_func "$LINENO" "$ac_func" "$as_ac_var"
@@ -10791,7 +10791,7 @@
 WFLAGS="-Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers \
 	 -Wmissing-declarations -Wmissing-format-attribute \
 	 -Wformat-security -Wpointer-arith \
-	 -Wvla -Winit-self -Wwrite-strings -Wcast-align -Wshadow"
+	 -Winit-self -Wwrite-strings -Wcast-align -Wshadow"
 WCFLAGS="-Wdeclaration-after-statement -Wnested-externs -Wbad-function-cast \
 	 -Wstrict-prototypes -Wmissing-prototypes -Wold-style-definition"
 # Temporarily here until #542031 gets fixed in ncurses
