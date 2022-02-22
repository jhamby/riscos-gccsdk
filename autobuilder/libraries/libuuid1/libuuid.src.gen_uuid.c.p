--- libuuid/src/gen_uuid.c.orig	2021-10-02 10:28:49.559489142 +0100
+++ libuuid/src/gen_uuid.c	2021-10-02 10:37:03.290264720 +0100
@@ -250,6 +250,12 @@
 	}
 	if (state_fd >= 0) {
 		rewind(state_f);
+#ifdef __riscos__
+			fclose(state_f);
+			close(state_fd);
+			state_fd = -1;
+			ret = -1;
+#else
 		while (flock(state_fd, LOCK_EX) < 0) {
 			if ((errno == EAGAIN) || (errno == EINTR))
 				continue;
@@ -259,6 +265,7 @@
 			ret = -1;
 			break;
 		}
+#endif
 	}
 	if (state_fd >= 0) {
 		unsigned int cl;
@@ -329,7 +329,9 @@
                        fflush(state_f);
                }
                rewind(state_f);
+#ifndef __riscos__
                flock(state_fd, LOCK_UN);
+#endif
        }
 
        *clock_high = clock_reg >> 32;
