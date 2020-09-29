--- ./src/dos/drive_local.cpp.orig	2020-09-26 10:52:55.314139804 +0100
+++ ./src/dos/drive_local.cpp	2020-09-25 11:09:27.572095642 +0100
@@ -1070,10 +1070,11 @@
 
 }
 
-#if !defined(WIN32)
+#if !defined(WIN32) && !defined(__riscos__)
 #include <sys/statvfs.h>
 #endif
 bool localDrive::AllocationInfo(Bit16u * _bytes_sector,Bit8u * _sectors_cluster,Bit16u * _total_clusters,Bit16u * _free_clusters) {
+#ifndef __riscos__
 	*_bytes_sector=allocation.bytes_sector;
 	*_sectors_cluster=allocation.sectors_cluster;
 	*_total_clusters=allocation.total_clusters;
@@ -1162,6 +1163,7 @@
 			}
 		}
 	}
+#endif /* __riscos__ */
 	return true;
 }
 
