--- Source/WebCore/platform/image-decoders/bmp/BMPImageReader.h.orig	2015-02-17 04:57:06.000000000 +0000
+++ Source/WebCore/platform/image-decoders/bmp/BMPImageReader.h	2015-04-25 13:39:32.908310854 +0100
@@ -248,7 +248,11 @@
                             unsigned blue,
                             unsigned alpha)
         {
+#if OS(RISCOS)
+            m_buffer->setRGBA(m_coord.x(), m_coord.y(), blue, green, red, alpha);
+#else
             m_buffer->setRGBA(m_coord.x(), m_coord.y(), red, green, blue, alpha);
+#endif
             m_coord.move(1, 0);
         }
 
