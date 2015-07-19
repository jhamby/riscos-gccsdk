--- src/corelib/kernel/qvariant_p.h.orig	2015-06-29 21:04:57.000000000 +0100
+++ src/corelib/kernel/qvariant_p.h	2015-07-15 18:24:20.398735902 +0100
@@ -205,8 +205,8 @@
 #if defined(Q_COMPILER_DECLTYPE) // C++11 version
     template<typename T>
     class HasIsNullMethod {
-        struct Yes { char unused[1]; };
-        struct No { char unused[2]; };
+        struct Yes { char unused[1]; } Q_PACKED;
+        struct No { char unused[2]; } Q_PACKED;
         Q_STATIC_ASSERT(sizeof(Yes) != sizeof(No));
 
         template<class C> static decltype(static_cast<const C*>(0)->isNull(), Yes()) test(int);
