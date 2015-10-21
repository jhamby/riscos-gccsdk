--- ./log.c.orig	2015-09-23 16:04:35.005552338 +0200
+++ ./log.c	2015-09-23 16:04:35.017552338 +0200
@@ -104,6 +104,8 @@
 	{ "DEBUG1",	SYSLOG_LEVEL_DEBUG1 },
 	{ "DEBUG2",	SYSLOG_LEVEL_DEBUG2 },
 	{ "DEBUG3",	SYSLOG_LEVEL_DEBUG3 },
+	{ "DEBUG4",	SYSLOG_LEVEL_DEBUG4 },
+	{ "DEBUG5",	SYSLOG_LEVEL_DEBUG5 },
 	{ NULL,		SYSLOG_LEVEL_NOT_SET }
 };
 
@@ -235,6 +237,26 @@
 	va_end(args);
 }
 
+void
+debug4(const char *fmt,...)
+{
+	va_list args;
+
+	va_start(args, fmt);
+	do_log(SYSLOG_LEVEL_DEBUG4, fmt, args);
+	va_end(args);
+}
+
+void
+debug5(const char *fmt,...)
+{
+	va_list args;
+
+	va_start(args, fmt);
+	do_log(SYSLOG_LEVEL_DEBUG5, fmt, args);
+	va_end(args);
+}
+
 /*
  * Initialize the log.
  */
@@ -425,6 +447,14 @@
 		txt = "debug3";
 		pri = LOG_DEBUG;
 		break;
+	case SYSLOG_LEVEL_DEBUG4:
+		txt = "debug4";
+		pri = LOG_DEBUG;
+		break;
+	case SYSLOG_LEVEL_DEBUG5:
+		txt = "debug5";
+		pri = LOG_DEBUG;
+		break;
 	default:
 		txt = "internal error";
 		pri = LOG_ERR;
