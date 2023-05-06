From 1febb0ba29beebc12f89255bfa1b5e4b238f59b4 Mon Sep 17 00:00:00 2001
From: Cameron Cawley <ccawley2011@gmail.com>
Date: Sat, 6 May 2023 17:30:34 +0100
Subject: [PATCH] Ensure the tribyte structure is properly packed

---
 src/common.h | 2 +-
 src/pcm.c    | 6 ++++++
 2 files changed, 7 insertions(+), 1 deletion(-)

diff --git a/src/common.h b/src/common.h
index c7747c5c..fb2aced3 100644
--- src/common.h
+++ src/common.h
@@ -92,7 +92,7 @@
 #define		SF_MIN(a, b)	((a) < (b) ? (a) : (b))
 
 
-#define		COMPILE_TIME_ASSERT(e)	(sizeof (struct { int : - !! (e) ; }))
+#define		COMPILE_TIME_ASSERT(e)	(sizeof (struct { int : - ! (e) ; }))
 
 
 #define		SF_MAX_CHANNELS		1024
diff --git a/src/pcm.c b/src/pcm.c
index d7dc82c4..9a3cfad5 100644
--- src/pcm.c
+++ src/pcm.c
@@ -28,12 +28,18 @@
 ** type and use SIZEOF_TRIBYTE instead of (tribyte).
 */
 
+#pragma pack(1)
+
 typedef	struct tribyte
 {	uint8_t bytes [3] ;
 	} tribyte ;
 
+#pragma pack()
+
 #define	SIZEOF_TRIBYTE	3
 
+static const int tribyte_assert = COMPILE_TIME_ASSERT(sizeof(tribyte) == SIZEOF_TRIBYTE);
+
 static sf_count_t	pcm_read_sc2s	(SF_PRIVATE *psf, short *ptr, sf_count_t len) ;
 static sf_count_t	pcm_read_uc2s	(SF_PRIVATE *psf, short *ptr, sf_count_t len) ;
 static sf_count_t	pcm_read_bes2s	(SF_PRIVATE *psf, short *ptr, sf_count_t len) ;
-- 
2.30.2

