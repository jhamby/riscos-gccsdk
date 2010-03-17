--- !RTK/rtk/util/cumulative_sum.h.orig	2010-03-15 21:03:45.000000000 +0000
+++ !RTK/rtk/util/cumulative_sum.h	2010-03-15 21:10:51.000000000 +0000
@@ -182,8 +182,8 @@
 }
 
 template<class value_type>
-class cumulative_sum<value_type>::index_type
-cumulative_sum<value_type>::find(value_type value) const
+/* class cumulative_sum<value_type>::index_type */
+unsigned int cumulative_sum<value_type>::find(value_type value) const
 {
 	index_type index=0;
 
