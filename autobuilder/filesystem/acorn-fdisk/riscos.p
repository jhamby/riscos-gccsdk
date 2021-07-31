diff -ur ./ChangeLog /usr/src/utils/acorn-fdisk-3.0.6/ChangeLog
--- ./ChangeLog	1999-05-26 02:16:19.000000000 -0700
+++ /usr/src/utils/acorn-fdisk-3.0.6/ChangeLog	2004-11-11 03:44:45.000000000 -0800
@@ -13,7 +13,7 @@
 3.0.3
 -----
 
-* RiscOS code sync'd:
+* RISC OS code sync'd:
   - linux scheme now contains writing code, but is currently disabled.
   - eesox scheme added.
   - powertec detection moved later.
diff -ur ./fdisk.c /usr/src/utils/acorn-fdisk-3.0.6/fdisk.c
--- ./fdisk.c	2006-06-26 22:10:18.000000000 -0700
+++ /usr/src/utils/acorn-fdisk-3.0.6/fdisk.c	2004-11-11 03:09:32.000000000 -0800
@@ -637,7 +637,7 @@
 	printf("OK\nRe-reading the partition table...");
 	fflush(stdout);
 
-	i = ioctl(part->blkio->fd, BLKRRPART);
+	i = ioctl(part->blkio->fd, BLKRRPART, 0);
 	if (i != 0)
 		error = errno;
 	else {
@@ -645,7 +645,7 @@
 		fflush(stdout);
 		sync();
 		sleep(2);
-		i = ioctl(part->blkio->fd, BLKRRPART);
+		i = ioctl(part->blkio->fd, BLKRRPART, 0);
 		if (i != 0)
 			error = errno;
 	}
diff -ur ./lib/blkio/Makefile /usr/src/utils/acorn-fdisk-3.0.6/lib/blkio/Makefile
--- ./lib/blkio/Makefile	2006-06-26 22:10:18.000000000 -0700
+++ /usr/src/utils/acorn-fdisk-3.0.6/lib/blkio/Makefile	2004-11-11 03:20:45.000000000 -0800
@@ -1,7 +1,7 @@
 AR		= ar
 CC		= cc
 ARFLAGS		= rc
-CFLAGS		= -DUNIX -O2 -Wall -I.. -g -D_FILE_OFFSET_BITS=64
+CFLAGS		= -DRISCOS -O2 -Wall -I.. -g -D_FILE_OFFSET_BITS=64
 
 OBJS		= close.o getgeo.o open.o read.o setblocksz.o setgeo.o write.o
 LIB		= ../blkio.a
diff -ur ./lib/blkio/blkio.h /usr/src/utils/acorn-fdisk-3.0.6/lib/blkio/blkio.h
--- ./lib/blkio/blkio.h	2006-06-26 22:10:18.000000000 -0700
+++ /usr/src/utils/acorn-fdisk-3.0.6/lib/blkio/blkio.h	2004-11-11 03:39:47.000000000 -0800
@@ -27,8 +27,8 @@
   /* Unix specific I/O data */
   int fd;				/* File descriptor */
 #endif
-#ifdef RISCOS
-  /* RiscOS specific I/O data */
+#ifdef __riscos__
+  /* RISC OS specific I/O data */
   u_int		swi_describedisc;	/* SWI number for xxx_DescribeDisc	*/
   u_int		swi_discop;		/* SWI number for xxx_DiscOp		*/
   u_int		swi_sectorop;		/* SWI number for xxx_SectorOp		*/
diff -ur ./lib/blkio/close.c /usr/src/utils/acorn-fdisk-3.0.6/lib/blkio/close.c
--- ./lib/blkio/close.c	1998-03-15 14:49:21.000000000 -0800
+++ /usr/src/utils/acorn-fdisk-3.0.6/lib/blkio/close.c	2004-11-11 03:40:06.000000000 -0800
@@ -23,7 +23,7 @@
 {
   dbg_printf("blkio_close()");
   if (blkio) {
-#if defined(RISCOS)
+#if defined(__riscos__)
     free(blkio->disc_record);
 #elif defined(UNIX)
     close(blkio->fd);
diff -ur ./lib/blkio/getgeo.c /usr/src/utils/acorn-fdisk-3.0.6/lib/blkio/getgeo.c
--- ./lib/blkio/getgeo.c	1998-02-21 00:16:50.000000000 -0800
+++ /usr/src/utils/acorn-fdisk-3.0.6/lib/blkio/getgeo.c	2004-11-11 03:39:30.000000000 -0800
@@ -5,7 +5,7 @@
  *
  * The block I/O interface allows an OS-independent method to access devices
  *
- * Close a device on RiscOS architecture
+ * Close a device on RISC OS architecture
  */
 #include <assert.h>
 #include <stdlib.h>
diff -ur ./lib/blkio/open.c /usr/src/utils/acorn-fdisk-3.0.6/lib/blkio/open.c
--- ./lib/blkio/open.c	2006-06-26 22:10:18.000000000 -0700
+++ /usr/src/utils/acorn-fdisk-3.0.6/lib/blkio/open.c	2004-11-11 05:33:28.000000000 -0800
@@ -5,7 +5,7 @@
  *
  * The block I/O interface allows an OS-independent method to access devices
  *
- * Open a device on RiscOS architecture
+ * Open a device on RISC OS architecture
  */
 #include <assert.h>
 #include <string.h>
@@ -22,9 +22,8 @@
 #define MAX_FS_NAME	10
 #define MAX_DRIVE_NAME	20
 
-#ifdef RISCOS
-#include "!Swi.h"
-#include "util/swi_num.h"
+#ifdef __riscos__
+#include <swis.h>
 
 struct ide_identify_request {
   unsigned long unused;
@@ -144,7 +143,7 @@
 
 static void set_disc_geometry (blkio_t *blkio, geometry_t *geo)
 {
-  u_int64 sectors;
+  u_int64_t sectors;
   u_int log2secsize, update_disc_record = 0;
 
   log2secsize = blkio->disc_record->d.log2secsize;
@@ -164,9 +163,9 @@
     int slave = 0, type;
 
     if (blkio->swi_controllertype) {
-      if (!swix (blkio->swi_controllertype, rin(r0)|rout(r0), blkio->drive, &type) &&
+      if (!_swix (blkio->swi_controllertype, _IN(0)|_OUT(0), blkio->drive, &type) &&
           type == TYPE_IDE &&
-          !swix(blkio->swi_controllertype, rin(r0)|rout(r0), blkio->drive - 1, &type) &&
+          !_swix(blkio->swi_controllertype, _IN(0)|_OUT(0), blkio->drive - 1, &type) &&
           type == TYPE_IDE)
         slave = 1;
     }
@@ -176,14 +175,30 @@
     memset (&ide_id_req, 0, sizeof (ide_id_req));
     ide_id_req.length = 160 | (slave ? 1 << 4 : 0);
     ide_id_req.cmd = 236;
-    if (!swix (blkio->swi_ideuserop, rin(r0|r2|r3|r4),
-               1 << 24, &ide_id_req, &ide_id_res, 512)) {
+    if (!_swix (blkio->swi_ideuserop, _INR(0,4),
+               1 << 24, 0, &ide_id_req, &ide_id_res, sizeof(ide_id_res))) {
       geo->log2secsize = 9;
       geo->sector_size = 1 << 9;
       geo->sectors   = ide_id_res.sectors;
       geo->heads     = ide_id_res.heads;
       geo->cylinders = ide_id_res.cyls;
       update_disc_record = 1;
+
+      /* Calculate sizes as per RISC OS 5 HForm */
+      if (ide_id_res.capability & 0x2) {
+         geo->heads = 16;
+         geo->sectors = 255;
+
+         geo->cylinders = ide_id_res.lba_capacity / geo->heads;
+
+         while ((geo->cylinders % geo->sectors) && geo->sectors > 63)
+           geo->sectors -= 1;
+
+         geo->cylinders /= geo->sectors;
+
+         dbg_printf ("-IDE supports LBA: [totsec=%d, sec/tk=%d, hds=%d, cyls=%d]",
+           ide_id_res.lba_capacity, geo->sectors, geo->heads, geo->cylinders);
+      }
     }
   } else if (geo->sectors == 1) {
     geo->log2secsize = 9;
@@ -193,6 +208,7 @@
     geo->cylinders = 0;
     update_disc_record = 1;
   }
+
   if (update_disc_record
 #if 0
        && (geo->sector_size == 1 << log2secsize) &&
@@ -214,6 +230,22 @@
   	     geo->sector_size, geo->sectors, geo->heads, geo->cylinders);
 }
 
+int swi_number(const char *base, const char *name)
+{
+  char buffer[256];
+  int number;
+
+  strncpy(buffer, base, sizeof(buffer));
+  strncat(buffer, "_", sizeof(buffer));
+  strncat(buffer, name, sizeof(buffer));
+
+  if (_swix(OS_SWINumberFromString, _IN(1)|_OUT(0), buffer, &number))
+    return 0;
+
+  return number;
+}
+
+
 /* Function: blkio_t *blkio_open (const char *dev_name)
  * Purpose : Open a device, specified in dev_name and return a structure
  * Params  : dev_name - device name (eg, ADFS::4, /dev/hda etc)
@@ -270,7 +302,7 @@
       break;
     }
 
-    if (swix (blkio->swi_describedisc, rin(r0|r1), drive_name, blkio->disc_record)) {
+    if (_swix (blkio->swi_describedisc, _IN(0)|_IN(1), drive_name, blkio->disc_record)) {
       set_error ("Invalid filesystem drive name `%s'", drive_name);
       free (blkio->disc_record);
       free (blkio);
diff -ur ./lib/blkio/read.c /usr/src/utils/acorn-fdisk-3.0.6/lib/blkio/read.c
--- ./lib/blkio/read.c	2006-06-26 22:10:18.000000000 -0700
+++ /usr/src/utils/acorn-fdisk-3.0.6/lib/blkio/read.c	2004-11-11 05:36:28.000000000 -0800
@@ -5,7 +5,7 @@
  *
  * The block I/O interface allows an OS-independent method to access devices
  *
- * Read data from device on RiscOS architecture
+ * Read data from device on RISC OS architecture
  */
 #include <errno.h>
 
@@ -14,8 +14,8 @@
 #include "blkio.h"
 #include "filecore.h"
 
-#ifdef RISCOS
-#include "!Swi.h"
+#ifdef __riscos__
+#include <swis.h>
 
 static u_int read_block(blkio_t *blkio, void *data, blk_t block, u_int nr_blocks)
 {
@@ -48,7 +48,7 @@
     		(((unsigned int)blkio->disc_record) << 6)|1,
     		(blkio->drive << 29) | (sector & 0x1fffffff), data, nr);
 
-    ret = swix(swino, rin(r1|r2|r3|r4), (((unsigned int)blkio->disc_record) << 6)|1,
+    ret = _swix(swino, _INR(1,4), (((unsigned int)blkio->disc_record) << 6)|1,
 		(blkio->drive << 29) | (sector & 0x1fffffff), data, nr);
     if (ret) {
       set_error("%s", ret->errmess);
diff -ur ./lib/blkio/setblocksz.c /usr/src/utils/acorn-fdisk-3.0.6/lib/blkio/setblocksz.c
--- ./lib/blkio/setblocksz.c	1998-03-15 14:54:35.000000000 -0800
+++ /usr/src/utils/acorn-fdisk-3.0.6/lib/blkio/setblocksz.c	2004-11-11 03:44:09.000000000 -0800
@@ -5,7 +5,7 @@
  *
  * The block I/O interface allows an OS-independent method to access devices
  *
- * Close a device on RiscOS architecture
+ * Close a device on RISC OS architecture
  */
 #include <assert.h>
 #include <stdlib.h>
@@ -28,7 +28,7 @@
 
   dbg_printf ("blkio_setblocksize(%d)", blocksize);
 
-#ifdef RISCOS
+#ifdef __riscos__
   if (blocksize < (1 << blkio->disc_record->d.log2secsize))
     blocksize = (1 << blkio->disc_record->d.log2secsize);
 #endif
diff -ur ./lib/blkio/setgeo.c /usr/src/utils/acorn-fdisk-3.0.6/lib/blkio/setgeo.c
--- ./lib/blkio/setgeo.c	1998-03-15 14:50:51.000000000 -0800
+++ /usr/src/utils/acorn-fdisk-3.0.6/lib/blkio/setgeo.c	2004-11-11 03:42:50.000000000 -0800
@@ -5,7 +5,7 @@
  *
  * The block I/O interface allows an OS-independent method to access devices
  *
- * Close a device on RiscOS architecture
+ * Close a device on RISC OS architecture
  */
 #include <assert.h>
 #include <stdlib.h>
diff -ur ./lib/blkio/write.c /usr/src/utils/acorn-fdisk-3.0.6/lib/blkio/write.c
--- ./lib/blkio/write.c	2006-06-26 22:10:18.000000000 -0700
+++ /usr/src/utils/acorn-fdisk-3.0.6/lib/blkio/write.c	2004-11-11 03:32:25.000000000 -0800
@@ -5,7 +5,7 @@
  *
  * The block I/O interface allows an OS-independent method to access devices
  *
- * Write data to device on RiscOS architecture
+ * Write data to device on RISC OS architecture
  */
 #include <errno.h>
 
@@ -14,8 +14,8 @@
 #include "blkio.h"
 #include "filecore.h"
 
-#ifdef RISCOS
-#include "!Swi.h"
+#ifdef __riscos__
+#include <swis.h>
 
 static u_int write_block(blkio_t *blkio, const void *data, blk_t block, u_int nr_blocks)
 {
@@ -48,7 +48,7 @@
     		(((unsigned int)blkio->disc_record) << 6)|2,
     		(blkio->drive << 29) | (sector & 0x1fffffff), data, nr);
 
-    ret = swix (swino, rin(r1|r2|r3|r4), (((unsigned int)blkio->disc_record) << 6)|2,
+    ret = _swix (swino, _INR(1,4), (((unsigned int)blkio->disc_record) << 6)|2,
 		(blkio->drive << 29) | (sector & 0x1fffffff), data, nr);
     if (ret) {
       set_error ("%s", ret->errmess);
diff -ur ./lib/part/part.h /usr/src/utils/acorn-fdisk-3.0.6/lib/part/part.h
--- ./lib/part/part.h	1999-03-27 02:17:31.000000000 -0800
+++ /usr/src/utils/acorn-fdisk-3.0.6/lib/part/part.h	2004-11-11 03:43:18.000000000 -0800
@@ -58,8 +58,8 @@
   ptyp_dosscc		= 0xf2,	 /* DOS SCC				*/
   ptyp_bbt		= 0xff,	 /* Bad block table			*/
   /* Other partition types */
-  ptyp_filecore		= 0x100, /* RiscOS filecore partition		*/
-  ptyp_linux_table	= 0x101, /* RiscOS filecore Linux partition tbl	*/
+  ptyp_filecore		= 0x100, /* RISC OS filecore partition		*/
+  ptyp_linux_table	= 0x101, /* RISC OS filecore Linux partition tbl*/
   ptyp_pc_table		= 0x102, /* PC partition table			*/
   ptyp_pt_oldmap	= 0x103, /* for PowerTec OLDMAP partitions	*/
   ptyp_pt_backup	= 0x104, /* for PowerTec BACKUP partitions	*/
diff -ur ./lib/util/types.h /usr/src/utils/acorn-fdisk-3.0.6/lib/util/types.h
--- ./lib/util/types.h	1998-03-15 14:57:44.000000000 -0800
+++ /usr/src/utils/acorn-fdisk-3.0.6/lib/util/types.h	2004-11-11 03:21:45.000000000 -0800
@@ -6,18 +6,8 @@
 #ifndef UTIL_TYPES_H
 #define UTIL_TYPES_H
 
-#ifndef UNIX
-
-typedef unsigned int   u_int;
-typedef unsigned int   u_long;
-typedef unsigned char  u_char;
-
-#else
-
 #include <sys/types.h>
 
-#endif
-
 typedef signed int     s_int;
 typedef signed int     s_long;
 typedef signed char    s_char;
