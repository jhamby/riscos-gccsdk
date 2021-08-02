--- makerun.c.orig	2021-08-01 17:03:26.852746351 -0700
+++ makerun.c	2021-08-01 17:04:12.621162695 -0700
@@ -28,7 +28,7 @@
 	unsigned int file_size;
 	char buf[256];
 
-	if (argc != 4) {
+	if (argc != 4 && argc != 2) {
 		fprintf(stderr, "Usage: %s <runimage> <input> <output>\n",
 				argv[0]);
 		return 1;
@@ -49,7 +49,7 @@
 
 	if (header.decompress == 0x464c457f /* FLE\x7f */) {
 		/* ELF binary */
-		file_size += 0x8000; /* Add 32k of scratch space */
+		file_size += 0x80000; /* Add 512k of scratch space */
 	} else {
 		if ((header.entry >> 24) != 0xEB) {
 			fprintf(stderr, "%s not binary\n", argv[1]);
@@ -69,6 +69,11 @@
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
