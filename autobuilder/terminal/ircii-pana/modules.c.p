--- source/modules.c.old	2002-12-17 20:57:43.000000000 +0000
+++ source/modules.c	2002-12-17 20:57:28.000000000 +0000
@@ -84,7 +84,7 @@
 extern int (*dcc_input_func)  (int, int, char *, int, int);
 extern int (*dcc_close_func) (int, unsigned long, int);
 
-int (*serv_open_func) (int, unsigned long, int);
+extern int (*serv_open_func) (int, unsigned long, int);
 extern int (*serv_output_func) (int, int, char *, int);
 extern int (*serv_input_func)  (int, char *, int, int, int);
 extern int (*serv_close_func) (int, unsigned long, int);
