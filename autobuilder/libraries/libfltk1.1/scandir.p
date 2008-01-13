--- src/filename_list.cxx.orig	2008-01-12 20:02:50.000000000 -0800
+++ src/filename_list.cxx	2008-01-12 20:03:00.000000000 -0800
@@ -54,7 +54,7 @@
                      Fl_File_Sort_F *sort) {
 #ifndef HAVE_SCANDIR
   int n = scandir(d, list, 0, sort);
-#elif defined(__hpux) || defined(__CYGWIN__)
+#elif defined(__hpux) || defined(__CYGWIN__) || defined(__riscos__)
   // HP-UX, Cygwin define the comparison function like this:
   int n = scandir(d, list, 0, (int(*)(const dirent **, const dirent **))sort);
 #elif defined(__osf__)
