--- src/corelib/json/qjson_p.h.orig	2015-06-29 21:05:03.000000000 +0100
+++ src/corelib/json/qjson_p.h	2015-07-15 18:24:20.390735902 +0100
@@ -143,7 +143,7 @@
         val = qToLittleEndian(qFromLittleEndian(val) + i);
         return *this;
     }
-};
+} __attribute__((packed));
 
 typedef q_littleendian<short> qle_short;
 typedef q_littleendian<unsigned short> qle_ushort;
