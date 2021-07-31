--- macros/pkg.m4	4 Nov 2006 10:14:20 -0000	1.4
+++ macros/pkg.m4	18 Nov 2006 17:34:09 -0000
@@ -44,7 +44,7 @@
 	fi
   fi		
 else
-  PKG_CONFIG=""
+  PKG_CONFIG=$ac_cv_env_PKG_CONFIG_value
 fi[]dnl
 
 ])# PKG_PROG_PKG_CONFIG
