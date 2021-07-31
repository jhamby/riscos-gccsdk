--- ./log.h.orig	2015-09-23 16:04:35.365552335 +0200
+++ ./log.h	2015-09-23 16:04:35.381552335 +0200
@@ -43,6 +43,8 @@
 	SYSLOG_LEVEL_DEBUG1,
 	SYSLOG_LEVEL_DEBUG2,
 	SYSLOG_LEVEL_DEBUG3,
+	SYSLOG_LEVEL_DEBUG4,
+	SYSLOG_LEVEL_DEBUG5,
 	SYSLOG_LEVEL_NOT_SET = -1
 }       LogLevel;
 
@@ -68,6 +70,8 @@
 void     debug(const char *, ...) __attribute__((format(printf, 1, 2)));
 void     debug2(const char *, ...) __attribute__((format(printf, 1, 2)));
 void     debug3(const char *, ...) __attribute__((format(printf, 1, 2)));
+void     debug4(const char *, ...) __attribute__((format(printf, 1, 2)));
+void     debug5(const char *, ...) __attribute__((format(printf, 1, 2)));
 
 
 void	 set_log_handler(log_handler_fn *, void *);
