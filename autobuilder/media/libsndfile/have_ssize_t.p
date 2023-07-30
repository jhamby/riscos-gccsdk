From c8afc0c633c6f5ce4fa94f55a775ebd25ebb0f27 Mon Sep 17 00:00:00 2001
From: Cameron Cawley <ccawley2011@gmail.com>
Date: Sat, 6 May 2023 17:02:27 +0100
Subject: [PATCH 1/2] Avoid redefining ssize_t when building with autotools

---
 src/config.h.cmake | 4 ++++
 src/sfconfig.h     | 8 --------
 2 files changed, 4 insertions(+), 8 deletions(-)

diff --git a/src/config.h.cmake b/src/config.h.cmake
index 71fdb4f2..e8d81453 100644
--- src/config.h.cmake
+++ src/config.h.cmake
@@ -148,6 +148,10 @@
 /* Define to 1 if the system has the type `ssize_t'. */
 #cmakedefine01 HAVE_SSIZE_T
 
+#if (HAVE_SSIZE_T == 0)
+#define ssize_t intptr_t
+#endif
+
 /* Define to 1 if you have the <stdint.h> header file. */
 #cmakedefine01 HAVE_STDINT_H
 
diff --git a/src/sfconfig.h b/src/sfconfig.h
index d70f405b..de2e56d6 100644
--- src/sfconfig.h
+++ src/sfconfig.h
@@ -128,12 +128,4 @@
 #define USE_SSE2
 #endif
 
-#ifndef HAVE_SSIZE_T
-#define HAVE_SSIZE_T 0
-#endif
-
-#if (HAVE_SSIZE_T == 0)
-#define ssize_t intptr_t
-#endif
-
 #endif
-- 
2.30.2

