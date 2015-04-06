--- src/gui/painting/qrgb.h.orig	2014-05-15 18:12:16.000000000 +0100
+++ src/gui/painting/qrgb.h	2014-05-27 19:55:16.242522840 +0100
@@ -53,23 +53,43 @@
 // non-namespaced Qt global variable
 const Q_DECL_UNUSED QRgb  RGB_MASK    = 0x00ffffff;     // masks RGB values
 
+#ifdef __riscos__
+inline Q_DECL_CONSTEXPR int qRed(QRgb rgb)                // get red part of RGB
+{ return (rgb & 0xff); }
+#else
 inline Q_DECL_CONSTEXPR int qRed(QRgb rgb)                // get red part of RGB
 { return ((rgb >> 16) & 0xff); }
+#endif
 
 inline Q_DECL_CONSTEXPR int qGreen(QRgb rgb)                // get green part of RGB
 { return ((rgb >> 8) & 0xff); }
 
+#ifdef __riscos__
+inline Q_DECL_CONSTEXPR int qBlue(QRgb rgb)                // get blue part of RGB
+{ return ((rgb >> 16) & 0xff); }
+#else
 inline Q_DECL_CONSTEXPR int qBlue(QRgb rgb)                // get blue part of RGB
 { return (rgb & 0xff); }
+#endif
 
 inline Q_DECL_CONSTEXPR int qAlpha(QRgb rgb)                // get alpha part of RGBA
 { return rgb >> 24; }
 
+#ifdef __riscos__
+inline Q_DECL_CONSTEXPR QRgb qRgb(int r, int g, int b)// set RGB value
+{ return (0xffu << 24) | ((b & 0xff) << 16) | ((g & 0xff) << 8) | (r & 0xff); }
+#else
 inline Q_DECL_CONSTEXPR QRgb qRgb(int r, int g, int b)// set RGB value
 { return (0xffu << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff); }
+#endif
 
+#ifdef __riscos__
+inline Q_DECL_CONSTEXPR QRgb qRgba(int r, int g, int b, int a)// set RGBA value
+{ return ((a & 0xff) << 24) | ((b & 0xff) << 16) | ((g & 0xff) << 8) | (r & 0xff); }
+#else
 inline Q_DECL_CONSTEXPR QRgb qRgba(int r, int g, int b, int a)// set RGBA value
 { return ((a & 0xff) << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff); }
+#endif
 
 inline Q_DECL_CONSTEXPR int qGray(int r, int g, int b)// convert R,G,B to gray 0..255
 { return (r*11+g*16+b*5)/32; }
