--- disk.c.orig	2005-09-22 02:16:31.000000000 +0200
+++ disk.c	2006-05-15 14:30:41.550000000 +0200
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
@@ -697,6 +707,9 @@
 NTSTATUS
 disk_set_information(NTHANDLE handle, uint32 info_class, STREAM in, STREAM out)
 {
+#ifdef __riscos__
+	return STATUS_INVALID_PARAMETER;
+#else
 	uint32 length, file_attributes, ft_high, ft_low, delete_on_close;
 	char newname[PATH_MAX], fullpath[PATH_MAX];
 	struct fileinfo *pfinfo;
@@ -866,6 +879,7 @@
 			return STATUS_INVALID_PARAMETER;
 	}
 	return STATUS_SUCCESS;
+#endif
 }
 
 NTSTATUS
@@ -1046,6 +1060,9 @@
 NTSTATUS
 disk_query_volume_information(NTHANDLE handle, uint32 info_class, STREAM out)
 {
+#ifdef __riscos__
+	return STATUS_INVALID_PARAMETER;
+#else
 	struct STATFS_T stat_fs;
 	struct fileinfo *pfinfo;
 	FsInfoType *fsinfo;
@@ -1106,6 +1123,7 @@
 			return STATUS_INVALID_PARAMETER;
 	}
 	return STATUS_SUCCESS;
+#endif
 }
 
 NTSTATUS
