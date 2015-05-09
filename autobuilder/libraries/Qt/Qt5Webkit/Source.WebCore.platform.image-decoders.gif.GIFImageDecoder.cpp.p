--- Source/WebCore/platform/image-decoders/gif/GIFImageDecoder.cpp.orig	2015-02-17 04:57:06.000000000 +0000
+++ Source/WebCore/platform/image-decoders/gif/GIFImageDecoder.cpp	2015-05-08 15:56:12.928231087 +0100
@@ -236,6 +236,11 @@
         const unsigned char sourceValue = rowBuffer[(m_scaled ? m_scaledColumns[x] : x) - frameContext->xOffset];
         if ((!frameContext->isTransparent || (sourceValue != frameContext->tpixel)) && (sourceValue < colorMapSize)) {
             const size_t colorIndex = static_cast<size_t>(sourceValue) * 3;
+#if OS(RISCOS)
+	    if (QSysInfo::requireRedBlueSwap())
+	      buffer.setRGBA(currentAddress, colorMap[colorIndex + 2], colorMap[colorIndex + 1], colorMap[colorIndex], 255);
+	    else
+#endif
             buffer.setRGBA(currentAddress, colorMap[colorIndex], colorMap[colorIndex + 1], colorMap[colorIndex + 2], 255);
         } else {
             m_currentBufferSawAlpha = true;
