--- makerun.c.orig	2012-11-02 10:45:21.000000000 +0000
+++ makerun.c	2012-11-04 10:05:58.000000000 +0000
@@ -28,8 +28,8 @@
 	unsigned int file_size;
 	char buf[256];

-	if (argc != 4) {
-		fprintf(stderr, "Usage: %s <runimage> <input> <output>\n",
+	if (argc != 4 && argc != 2) {
+		fprintf(stderr, "Usage: %s <runimage> <input> <output>\n",
 				argv[0]);
 		return 1;
 	}
@@ -56,7 +56,7 @@
 			return 1;
 		}

-		if (header.rosize + header.rwsize + header.dbsize !=
+		if (header.rosize + header.rwsize + header.dbsize !=
 				file_size) {
 			if ((header.decompress >> 24) != 0xEB) {
 				fprintf(stderr, "Mismatched field sizes\n");
@@ -64,11 +64,16 @@
 			}
 		}

-		file_size = header.rosize + header.rwsize +
-				header.dbsize + header.zisize +
+		file_size = header.rosize + header.rwsize +
+				header.dbsize + header.zisize +
 				0x8000 /* 32k of scratch space */;
 	}

+	if (argc == 2) {
+	   fprintf(stdout, "%d", file_size / 1024);
+          return 0;
+	}
+
 	f = fopen(argv[2], "r");
 	if (f == NULL) {
 		fprintf(stderr, "Failed opening %s\n", argv[2]);
@@ -95,8 +98,8 @@
 	fclose(f);

 #ifdef __riscos
-	if (__riscosify(argv[3], 0, __RISCOSIFY_STRICT_UNIX_SPECS |
-			__RISCOSIFY_NO_SUFFIX | __RISCOSIFY_FILETYPE_NOT_SET,
+	if (__riscosify(argv[3], 0, __RISCOSIFY_STRICT_UNIX_SPECS |
+			__RISCOSIFY_NO_SUFFIX | __RISCOSIFY_FILETYPE_NOT_SET,
 			buf, sizeof(buf), NULL) == NULL) {
 		fprintf(stderr, "Riscosify failed\n");
 		return 1;
