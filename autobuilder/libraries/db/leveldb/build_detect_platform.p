--- build_detect_platform.orig	2014-06-15 19:39:45.000000000 +0100
+++ build_detect_platform	2014-06-16 18:57:26.984189702 +0100
@@ -153,6 +153,12 @@
         PLATFORM_LIBS="-lpthread"
         PORT_FILE=port/port_posix.cc
         ;;
+    RISC_OS)
+	PLATFORM=OS_RISCOS
+	COMMON_FLAGS="$MEMCMP_FLAG -D_REENTRANT -DOS_LINUX -DOS_RISCOS -DLEVELDB_PLATFORM_POSIX"
+        PORT_FILE=port/port_posix.cc
+        CROSS_COMPILE=true
+        ;;
     *)
         echo "Unknown platform!" >&2
         exit 1
