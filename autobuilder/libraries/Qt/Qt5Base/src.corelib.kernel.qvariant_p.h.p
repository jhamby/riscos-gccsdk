--- src/corelib/kernel/qvariant_p.h.orig	2013-08-25 19:03:38.000000000 +0100
+++ src/corelib/kernel/qvariant_p.h	2013-11-13 19:18:27.582317996 +0000
@@ -215,8 +215,8 @@
 #if defined(Q_COMPILER_DECLTYPE) // C++11 version
     template<typename T>
     class HasIsNullMethod {
-        struct Yes { char unused[1]; };
-        struct No { char unused[2]; };
+        struct Yes { char unused[1]; } Q_PACKED;
+        struct No { char unused[2]; } Q_PACKED;
         Q_STATIC_ASSERT(sizeof(Yes) != sizeof(No));
 
         template<class C> static decltype(static_cast<const C*>(0)->isNull(), Yes()) test(int);
