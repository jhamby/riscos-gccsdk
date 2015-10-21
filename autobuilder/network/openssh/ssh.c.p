--- ./ssh.c.orig	2015-09-23 16:04:35.305552336 +0200
+++ ./ssh.c	2015-09-23 16:04:35.321552335 +0200
@@ -462,7 +462,7 @@
 			fatal("Can't open user config file %.100s: "
 			    "%.100s", config, strerror(errno));
 	} else {
-		r = snprintf(buf, sizeof buf, "%s/%s", pw->pw_dir,
+		r = snprintf(buf, sizeof buf, "%s.%s", pw->pw_dir,
 		    _PATH_SSH_USER_CONFFILE);
 		if (r > 0 && (size_t)r < sizeof(buf))
 			(void)read_config_file(buf, pw, host, host_arg,
@@ -509,7 +509,7 @@
 	struct stat st;
 	struct passwd *pw;
 	int timeout_ms;
-	extern int optind, optreset;
+	extern int optind;
 	extern char *optarg;
 	struct Forward fwd;
 	struct addrinfo *addrs = NULL;
@@ -729,7 +729,7 @@
 				debug_flag = 1;
 				options.log_level = SYSLOG_LEVEL_DEBUG1;
 			} else {
-				if (options.log_level < SYSLOG_LEVEL_DEBUG3)
+				if (options.log_level < SYSLOG_LEVEL_DEBUG5)
 					options.log_level++;
 			}
 			break;
@@ -926,7 +926,7 @@
 		} else
 			host = xstrdup(*av);
 		if (ac > 1) {
-			optind = optreset = 1;
+			optind = 1;
 			goto again;
 		}
 		ac--, av++;
@@ -1296,8 +1296,10 @@
 	 * directory if it doesn't already exist.
 	 */
 	if (config == NULL) {
-		r = snprintf(buf, sizeof buf, "%s%s%s", pw->pw_dir,
-		    strcmp(pw->pw_dir, "/") ? "/" : "", _PATH_SSH_USER_DIR);
+                debug("_PATH_SSH_USER_DIR=%s",_PATH_SSH_USER_DIR);
+                r = snprintf(buf, sizeof buf, "%s%s%s", pw->pw_dir,
+                    strcmp(pw->pw_dir, ".") ? "." : "", _PATH_SSH_USER_DIR);
+                debug("buf=%s",buf);
 		if (r > 0 && (size_t)r < sizeof(buf) && stat(buf, &st) < 0) {
 #ifdef WITH_SELINUX
 			ssh_selinux_setfscreatecon(buf);
