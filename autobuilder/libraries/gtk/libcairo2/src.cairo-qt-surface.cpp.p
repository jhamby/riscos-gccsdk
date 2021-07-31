--- src/cairo-qt-surface.cpp.orig	2013-08-26 16:07:21.000000000 +0100
+++ src/cairo-qt-surface.cpp	2014-03-24 20:51:28.779123915 +0000
@@ -785,7 +785,7 @@
 	    cairo_region_get_rectangle (clip_region, i, &rect);
 
 	    QRect r(rect.x, rect.y, rect.width, rect.height);
-	    qr = qr.unite(r);
+	    qr = qr.united(r);
 	}
 
 	qs->p->setClipRegion (qr, Qt::IntersectClip);
