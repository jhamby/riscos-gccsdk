--- src/gui/painting/qrgb.h.orig	2015-02-17 04:56:47.000000000 +0000
+++ src/gui/painting/qrgb.h	2015-05-08 12:47:47.876608556 +0100
@@ -45,31 +45,71 @@
 // non-namespaced Qt global variable
 const Q_DECL_UNUSED QRgb  RGB_MASK    = 0x00ffffff;     // masks RGB values
 
+#ifdef Q_OS_RISCOS
+inline int qRed(QRgb rgb)                // get red part of RGB
+{
+    return (QSysInfo::requireRedBlueSwap()) ? (rgb & 0xff) : ((rgb >> 16) & 0xff);
+}
+#else
 inline Q_DECL_CONSTEXPR int qRed(QRgb rgb)                // get red part of RGB
 { return ((rgb >> 16) & 0xff); }
+#endif
 
 inline Q_DECL_CONSTEXPR int qGreen(QRgb rgb)                // get green part of RGB
 { return ((rgb >> 8) & 0xff); }
 
+#ifdef Q_OS_RISCOS
+inline int qBlue(QRgb rgb)                // get blue part of RGB
+{
+    return  (QSysInfo::requireRedBlueSwap()) ? ((rgb >> 16) & 0xff) : (rgb & 0xff);
+}
+#else
 inline Q_DECL_CONSTEXPR int qBlue(QRgb rgb)                // get blue part of RGB
 { return (rgb & 0xff); }
+#endif
 
 inline Q_DECL_CONSTEXPR int qAlpha(QRgb rgb)                // get alpha part of RGBA
 { return rgb >> 24; }
 
+#ifdef Q_OS_RISCOS
+inline QRgb qRgb(int r, int g, int b)// set RGB value
+{
+    return QSysInfo::requireRedBlueSwap() ?
+	(0xffu << 24) | ((b & 0xff) << 16) | ((g & 0xff) << 8) | (r & 0xff) :
+	(0xffu << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff);
+}
+#else
 inline Q_DECL_CONSTEXPR QRgb qRgb(int r, int g, int b)// set RGB value
 { return (0xffu << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff); }
+#endif
 
+#ifdef Q_OS_RISCOS
+inline QRgb qRgba(int r, int g, int b, int a)// set RGBA value
+{
+    return QSysInfo::requireRedBlueSwap() ?
+	((a & 0xff) << 24) | ((b & 0xff) << 16) | ((g & 0xff) << 8) | (r & 0xff) :
+	((a & 0xff) << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff);
+}
+#else
 inline Q_DECL_CONSTEXPR QRgb qRgba(int r, int g, int b, int a)// set RGBA value
 { return ((a & 0xff) << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff); }
+#endif
 
 inline Q_DECL_CONSTEXPR int qGray(int r, int g, int b)// convert R,G,B to gray 0..255
 { return (r*11+g*16+b*5)/32; }
 
+#ifdef Q_OS_RISCOS
+inline int qGray(QRgb rgb)                // convert RGB to gray 0..255
+#else
 inline Q_DECL_CONSTEXPR int qGray(QRgb rgb)                // convert RGB to gray 0..255
+#endif
 { return qGray(qRed(rgb), qGreen(rgb), qBlue(rgb)); }
 
+#ifdef Q_OS_RISCOS
+inline bool qIsGray(QRgb rgb)
+#else
 inline Q_DECL_CONSTEXPR bool qIsGray(QRgb rgb)
+#endif
 { return qRed(rgb) == qGreen(rgb) && qRed(rgb) == qBlue(rgb); }
 
 
