--- ./src/corelib/configure.json.orig	2021-05-17 23:43:52.000000000 -0700
+++ ./src/corelib/configure.json	2021-08-16 21:58:09.002602658 -0700
@@ -773,7 +773,7 @@
             "purpose": "Provides access to a shared memory segment.",
             "section": "Kernel",
             "condition": [
-                "config.android || config.win32 || (!config.vxworks && (tests.ipc_sysv || tests.ipc_posix))"
+                "config.android || config.win32 || (!config.vxworks && !config.riscos && (tests.ipc_sysv || tests.ipc_posix))"
             ],
             "output": [ "publicFeature", "feature" ]
         },
