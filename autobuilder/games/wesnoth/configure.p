--- configure.orig	2009-08-24 15:37:30.000000000 -0700
+++ configure	2009-08-24 15:37:51.000000000 -0700
@@ -14100,7 +14100,7 @@
       && boost_subminor_chk="|| (B_V_MAJ == $boost_version_major \
 && B_V_MIN == $boost_version_minor \
 && B_V_SUB < $boost_version_subminor)"
-    for boost_inc in "$with_boost/include" '' \
+    for boost_inc in "$with_boost/" '' \
              /opt/local/include /usr/local/include /opt/include /usr/include \
              "$with_boost" C:/Boost/include
     do
