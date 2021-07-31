--- disk.c.orig	2008-02-15 16:13:25.000000000 -0800
+++ disk.c	2009-08-27 08:26:02.000000000 -0700
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
 					return RD_STATUS_NO_SUCH_FILE;
 			}
 		}
-		handle = DIRFD(dirp);
+		handle = 0;
+		//handle = DIRFD(dirp);
 	}
 	else
 	{
@@ -700,6 +710,9 @@
 RD_NTSTATUS
 disk_set_information(RD_NTHANDLE handle, uint32 info_class, STREAM in, STREAM out)
 {
+#ifdef __riscos__
+	return RD_STATUS_INVALID_PARAMETER;
+#else
 	uint32 length, file_attributes, ft_high, ft_low, delete_on_close;
 	char newname[PATH_MAX], fullpath[PATH_MAX];
 	struct fileinfo *pfinfo;
@@ -869,6 +882,7 @@
 			return RD_STATUS_INVALID_PARAMETER;
 	}
 	return RD_STATUS_SUCCESS;
+#endif
 }
 
 RD_NTSTATUS
@@ -1049,6 +1063,9 @@
 RD_NTSTATUS
 disk_query_volume_information(RD_NTHANDLE handle, uint32 info_class, STREAM out)
 {
+#ifdef __riscos__
+	return RD_STATUS_INVALID_PARAMETER;
+#else
 	struct STATFS_T stat_fs;
 	struct fileinfo *pfinfo;
 	FsInfoType *fsinfo;
@@ -1109,6 +1126,7 @@
 			return RD_STATUS_INVALID_PARAMETER;
 	}
 	return RD_STATUS_SUCCESS;
+#endif
 }
 
 RD_NTSTATUS
