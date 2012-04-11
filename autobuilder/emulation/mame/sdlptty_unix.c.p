--- src/osd/sdl/sdlptty_unix.c.orig	2012-04-09 14:57:32.000000000 +0100
+++ src/osd/sdl/sdlptty_unix.c	2012-04-09 14:58:00.000000000 +0100
@@ -36,6 +36,7 @@
 	int aslave;
 	char name[100];
 
+/*
 	if (openpty(&master, &aslave, name, NULL, NULL) >= 0)
 	{
 		printf("Slave of device %s is %s\n", path, name );
@@ -49,6 +50,9 @@
 	}
 
 	return FILERR_NONE;
+*/
+	return FILERR_ACCESS_DENIED;
+
 }
 
 file_error sdl_read_ptty(osd_file *file, void *buffer, UINT64 offset, UINT32 count, UINT32 *actual)
