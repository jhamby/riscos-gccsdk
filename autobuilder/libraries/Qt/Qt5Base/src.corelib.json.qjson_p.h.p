--- src/corelib/json/qjson_p.h.orig	2014-05-15 18:12:16.000000000 +0100
+++ src/corelib/json/qjson_p.h	2014-05-27 19:55:16.354522842 +0100
@@ -149,7 +149,7 @@
         val = qToLittleEndian(qFromLittleEndian(val) + i);
         return *this;
     }
-};
+} __attribute__((packed));
 
 typedef q_littleendian<short> qle_short;
 typedef q_littleendian<unsigned short> qle_ushort;
