--- src/corelib/json/qjson_p.h.orig	2015-02-17 04:56:49.000000000 +0000
+++ src/corelib/json/qjson_p.h	2015-04-12 13:43:51.636657032 +0100
@@ -141,7 +141,7 @@
         val = qToLittleEndian(qFromLittleEndian(val) + i);
         return *this;
     }
-};
+} __attribute__((packed));
 
 typedef q_littleendian<short> qle_short;
 typedef q_littleendian<unsigned short> qle_ushort;
