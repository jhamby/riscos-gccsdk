--- build_detect_platform.orig	2018-04-09 22:58:02.775136374 +0100
+++ build_detect_platform	2018-04-09 23:32:19.802746920 +0100
@@ -171,6 +171,13 @@
         PLATFORM_LIBS="-lpthread"
         PORT_FILE=port/port_posix.cc
         ;;
+    RISC_OS)
+	PLATFORM=OS_RISCOS
+	COMMON_FLAGS="$MEMCMP_FLAG -D_REENTRANT -DOS_LINUX -DOS_RISCOS -DLEVELDB_PLATFORM_POSIX"
+        PORT_FILE=port/port_posix.cc
+        PORT_SSE_FILE=port/port_posix_sse.cc
+        CROSS_COMPILE=true
+        ;;
     *)
         echo "Unknown platform!" >&2
         exit 1
