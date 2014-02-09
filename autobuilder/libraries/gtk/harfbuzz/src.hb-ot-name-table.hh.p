--- src/hb-ot-name-table.hh.orig	2013-11-20 19:18:31.722614906 +0000
+++ src/hb-ot-name-table.hh	2014-02-08 14:18:32.786170891 +0000
@@ -70,7 +70,7 @@
   USHORT	offset;		/* String offset from start of storage area (in bytes). */
   public:
   DEFINE_SIZE_STATIC (12);
-};
+} PACKED;
 
 struct name
 {
@@ -125,7 +125,7 @@
   NameRecord	nameRecord[VAR];	/* The name records where count is the number of records. */
   public:
   DEFINE_SIZE_ARRAY (6, nameRecord);
-};
+} PACKED;
 
 
 } /* namespace OT */
