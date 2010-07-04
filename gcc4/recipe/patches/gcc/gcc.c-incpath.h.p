--- gcc/c-incpath.h.orig	2010-07-04 15:44:41.486901572 +0200
+++ gcc/c-incpath.h	2010-07-04 15:43:36.874400089 +0200
@@ -18,7 +18,8 @@ Foundation, 51 Franklin Street, Fifth Fl
 extern void split_quote_chain (void);
 extern void add_path (char *, int, int, bool);
 extern void register_include_chains (cpp_reader *, const char *,
-				     const char *, int, int, int);
+				     const char *, const char *,
+				     int, int, int);
 extern void add_cpp_dir_path (struct cpp_dir *, int);
 
 struct target_c_incpath_s {
