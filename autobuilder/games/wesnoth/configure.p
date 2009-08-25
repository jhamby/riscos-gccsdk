--- configure.orig	2009-08-24 20:30:03.000000000 -0700
+++ configure	2009-08-24 20:42:15.000000000 -0700
@@ -14097,7 +14097,7 @@
       && boost_subminor_chk="|| (B_V_MAJ == $boost_version_major \
 && B_V_MIN == $boost_version_minor \
 && B_V_SUB < $boost_version_subminor)"
-    for boost_inc in "$with_boost/include" '' \
+    for boost_inc in "$with_boost" '' \
              /opt/local/include /usr/local/include /opt/include /usr/include \
              "$with_boost" C:/Boost/include
     do
@@ -14866,7 +14866,7 @@
     # right so instead we'll try to a location based on where the headers are.
     boost_tmp_lib=$with_boost
     test x"$with_boost" = x && boost_tmp_lib=${boost_cv_inc_path%/include}
-    for boost_ldpath in "$boost_tmp_lib/lib" '' \
+    for boost_ldpath in "$GCCSDK_INSTALL_ENV/lib" '' \
              /opt/local/lib /usr/local/lib /opt/lib /usr/lib \
              "$with_boost" C:/Boost/lib /lib /usr/lib64 /lib64
     do
@@ -15206,7 +15206,7 @@
     # right so instead we'll try to a location based on where the headers are.
     boost_tmp_lib=$with_boost
     test x"$with_boost" = x && boost_tmp_lib=${boost_cv_inc_path%/include}
-    for boost_ldpath in "$boost_tmp_lib/lib" '' \
+    for boost_ldpath in "$GCCSDK_INSTALL_ENV/lib" '' \
              /opt/local/lib /usr/local/lib /opt/lib /usr/lib \
              "$with_boost" C:/Boost/lib /lib /usr/lib64 /lib64
     do
