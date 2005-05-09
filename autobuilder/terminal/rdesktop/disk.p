Index: disk.c
===================================================================
RCS file: /cvsroot/rdesktop/rdesktop/disk.c,v
retrieving revision 1.50
diff -u -r1.50 disk.c
--- disk.c	8 May 2005 17:03:04 -0000	1.50
+++ disk.c	9 May 2005 08:50:19 -0000
@@ -75,6 +75,15 @@
 #endif
 
 #ifdef STAT_STATVFS
+#elif (defined(__riscos__))
+
+#define STATFS_FN(path, buf) (statfs(path,buf))
+#define STATFS_T statfs
+#define F_NAMELEN(buf) (FILENAME_MAX)
+
+#elif (defined(__SGI_IRIX__))
+#include <sys/types.h>
+#include <sys/statvfs.h>
 #define STATFS_FN(path, buf) (statvfs(path,buf))
 #define STATFS_T statvfs
 #define USE_STATVFS
@@ -426,7 +435,8 @@
 					return STATUS_NO_SUCH_FILE;
 			}
 		}
-		handle = DIRFD(dirp);
+		handle = 0;
+		//handle = DIRFD(dirp);
 	}
 	else
 	{
@@ -697,7 +707,9 @@
 NTSTATUS
 disk_set_information(NTHANDLE handle, uint32 info_class, STREAM in, STREAM out)
 {
-	uint32 length, file_attributes, ft_high, ft_low, delete_on_close;
+#ifdef __riscos__
+	return STATUS_INVALID_PARAMETER;
+#else
 	char newname[256], fullpath[256];
 	struct fileinfo *pfinfo;
 	int mode;
@@ -866,6 +878,7 @@
 			return STATUS_INVALID_PARAMETER;
 	}
 	return STATUS_SUCCESS;
+#endif
 }
 
 NTSTATUS
@@ -1046,6 +1059,9 @@
 NTSTATUS
 disk_query_volume_information(NTHANDLE handle, uint32 info_class, STREAM out)
 {
+#ifdef __riscos__
+	return STATUS_INVALID_PARAMETER;
+#else
 	struct STATFS_T stat_fs;
 	struct fileinfo *pfinfo;
 	FsInfoType *fsinfo;
@@ -1106,6 +1122,7 @@
 			return STATUS_INVALID_PARAMETER;
 	}
 	return STATUS_SUCCESS;
+#endif
 }
 
 NTSTATUS
