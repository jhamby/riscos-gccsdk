--- source/numbers.c.old	2002-12-17 20:48:01.000000000 +0000
+++ source/numbers.c	2002-12-17 20:47:56.000000000 +0000
@@ -64,7 +64,7 @@
 int	stats_k_grep		(char **);
 void	who_handlekill		(char *, char *, char *);
 void	handle_tracekill	(int, char *, char *, char *);
-int	no_hook_notify;
+extern int	no_hook_notify;
 extern  AJoinList *ajoin_list;
 void	remove_from_server_list (int);
 
