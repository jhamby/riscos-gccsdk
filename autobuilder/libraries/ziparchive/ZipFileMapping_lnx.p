--- ZipArchive/ZipFileMapping_lnx.h.orig	2009-04-24 16:47:02.000000000 +0100
+++ ZipArchive/ZipFileMapping_lnx.h	2009-04-09 11:35:20.658156700 +0100
@@ -41,7 +41,11 @@
                 
 			if (m_pFileMap)
 			{
+#ifdef __riscos__
+                                munmap((caddr_t)m_pFileMap, m_iSize);
+#else
 				munmap(m_pFileMap, m_iSize);
+#endif
 				m_pFileMap = NULL;
 			}
 		}
