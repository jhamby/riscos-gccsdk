--- src/corelib/kernel/qvariant_p.h.orig	2015-02-17 04:56:50.000000000 +0000
+++ src/corelib/kernel/qvariant_p.h	2015-04-12 13:43:51.640657032 +0100
@@ -207,8 +207,8 @@
 #if defined(Q_COMPILER_DECLTYPE) // C++11 version
     template<typename T>
     class HasIsNullMethod {
-        struct Yes { char unused[1]; };
-        struct No { char unused[2]; };
+        struct Yes { char unused[1]; } Q_PACKED;
+        struct No { char unused[2]; } Q_PACKED;
         Q_STATIC_ASSERT(sizeof(Yes) != sizeof(No));
 
         template<class C> static decltype(static_cast<const C*>(0)->isNull(), Yes()) test(int);
