--- bitmap.c.orig	2002-01-31 22:18:21.000000000 +0100
+++ bitmap.c	2012-04-19 10:23:22.000000000 +0200
@@ -37,7 +37,7 @@
 
 /*
  * AUTHORS
- * 
+ *
  *   Original Software:
  *     Jyrki Yli-Nokari <jty@intrin.UUCP>
  *     Ronald J. Hartranft <rjh2@ns.cc.lehigh.edu>
@@ -796,18 +796,27 @@
    **
    ** Plotting is done via b_move(x, y) and b_vector(x, y) functions,
    ** where the point (x,y) is the target to go from the current point
-   ** To set the color use b_setvalue(value) where value is the value 
+   ** To set the color use b_setvalue(value) where value is the value
    ** (0 or 1 or a color number) to be stored in every pixel.
    ** To get dotted line styles, use b_setlinetype(linetype).
    **
    ** Internally all plotting goes through b_setpixel(x, y, value).
  */
 
+/* BB change 20010510:
+   Make b_setpixel a pointer so that an other underlying bitmap
+   can be used. */
 
 /*
  * set pixel (x, y, value) to value value (this can be 1/0 or a color number).
  */
-void b_setpixel(x, y, value)
+static void default_b_setpixel __PROTO((unsigned int, unsigned int,
+                                        unsigned int));
+void (*b_setpixel) __PROTO((unsigned int, unsigned int,
+                            unsigned int)) =
+  default_b_setpixel;
+
+static void default_b_setpixel(x, y, value)
 unsigned int x, y, value;
 {
     register unsigned int row;
@@ -899,6 +908,11 @@
     register unsigned j;
     unsigned rows;
 
+    /* BB addition 20010510:
+       When using this function, be sure to set b_setpixel to
+       the corresponding function! */
+    b_setpixel = default_b_setpixel;
+
     x = 8 * (unsigned int) (x / 8.0 + 0.9);	/* round up to multiple of 8 */
     y = 8 * (unsigned int) (y / 8.0 + 0.9);	/* round up to multiple of 8 */
     b_psize = y / 8;		/* size of each plane */
@@ -971,13 +985,83 @@
 
     runcount = 0;
     dx = abs((int) (x1) - (int) (x2));
+    dy = abs((int) (y1) - (int) (y2));
+
+    /* BB 20010610
+       Due to rounding errors, epsecially for very small bitmaps,
+       coordinates may become negative.  But since coordinates are
+       unsigned this means very large values.  So in stead of drawing
+       a line from -1 to, say, 10, GnuPlot tries to draw a line from
+       10 to 4294967294.  And this takes forever, even on a 800 MHz
+       x86, let alone on my 40MHz ARM 710.
+       An extra problem occurs when drawing arrows.  The lines of the
+       arrows are calculated with incorrect data so coordinates end up
+       at values <= 2147483647, i.e. (int) coord > 0...
+       Assume incorrect data as well when coord > 0 but also
+       > a certain threshold.  E.g. 1.1 x term->...max or
+       100 pixels more.  The latter seems a good value, obtained
+       from debugging.)
+     */
+
+    if ((int) x1 < 0)
+    {
+      if (dx != 0)
+        y1 += dy * (-(int)x1) / dx;
+      x1 = 0;
+    }
+    if (x1 > term->xmax + 100)
+    {
+      if (dx != 0)
+        y1 += dy * (INT_MAX - (int)x1) / dx;
+      x1 = 0;
+    }
+    if ((int) x2 < 0)
+    {
+      if (dx != 0)
+        y2 += dy * (-(int)x2) / dx;
+      x2 = 0;
+    }
+    if (x2 > term->xmax + 100)
+    {
+      if (dx != 0)
+        y2 += dy * (INT_MAX - (int)x2) / dx;
+      x2 = 0;
+    }
+    if ((int) y1 < 0)
+    {
+      if (dy != 0)
+        y1 += dx * (-(int)y1) / dy;
+      y1 = 0;
+    }
+    if (y1 > term->ymax + 100)
+    {
+      if (dy != 0)
+        x1 += dx * (INT_MAX - (int)y1) / dy;
+      y1 = 0;
+    }
+    if ((int) y2 < 0)
+    {
+      if (dy != 0)
+        x2 += dy * (-(int)y2) / dy;
+      y2 = 0;
+    }
+    if (y2 > term->ymax + 100)
+    {
+      if (dy != 0)
+        x2 += dx * (INT_MAX - (int)y2) / dy;
+      y2 = 0;
+    }
+
     if (x2 > x1)
 	xinc = 1;
     else if (x2 == x1)
 	xinc = 0;
     else
 	xinc = -1;
-    dy = abs((int) (y1) - (int) (y2));
+
+    /* BB 20010610: moved */
+    /* dy = abs((int) (y1) - (int) (y2)); */
+
     if (y2 > y1)
 	yinc = 1;
     else if (y2 == y1)
