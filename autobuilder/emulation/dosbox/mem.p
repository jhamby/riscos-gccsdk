--- include/mem.h.orig	2004-08-23 14:19:18.000000000 +0100
+++ include/mem.h	2005-01-06 15:45:51.250000000 +0000
@@ -87,18 +87,43 @@
 	return *(Bit8u *)off;
 };
 INLINE Bit16u host_readw(HostPt off) {
+#ifdef __riscos__
+	return off[0] | (off[1] << 8);
+#else
 	return *(Bit16u *)off;
+#endif
 };
 INLINE Bit32u host_readd(HostPt off) {
+#ifdef __riscos__
+     if (((int)off) & 3) return off[0] | (off[1] << 8) | (off[2] << 16) | (off[3] << 24);
+#endif
 	return *(Bit32u *)off;
 };
 INLINE void host_writeb(HostPt off,Bit8u val) {
 	*(Bit8u *)(off)=val;
 };
 INLINE void host_writew(HostPt off,Bit16u val) {
+#ifdef __riscos__
+   if ((int)off & 1)
+   {
+     off[0]=(Bit8u)((val & 0x00ff));
+     off[1]=(Bit8u)((val & 0xff00) >> 8);
+     return;
+   }
+#endif
 	*(Bit16u *)(off)=val;
 };
 INLINE void host_writed(HostPt off,Bit32u val) {
+#ifdef __riscos
+   if ((int)off & 3)
+   {
+	  off[0]=(Bit8u)((val & 0x000000ff));
+	  off[1]=(Bit8u)((val & 0x0000ff00) >> 8);
+	  off[2]=(Bit8u)((val & 0x00ff0000) >> 16);
+	  off[3]=(Bit8u)((val & 0xff000000) >> 24);
+      return;
+   }
+#endif
 	*(Bit32u *)(off)=val;
 };
 
