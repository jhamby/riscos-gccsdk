--- src/lib/tools/sqldatabase.cpp.orig	2016-02-21 12:38:18.386840362 +0000
+++ src/lib/tools/sqldatabase.cpp	2016-02-21 12:40:12.914840479 +0000
@@ -53,8 +53,9 @@
         m_databases[thread].open();
     }
 
+#ifndef __riscos-_
     Q_ASSERT(m_databases[thread].isOpen());
-
+#endif
     return m_databases[thread];
 }
 
