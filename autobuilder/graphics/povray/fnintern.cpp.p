--- source/fnintern.cpp.orig	2004-08-03 00:11:35.000000000 +0100
+++ source/fnintern.cpp	2011-11-27 15:00:07.000000000 +0000
@@ -313,7 +313,7 @@
   r = PARAM(1)*PARAM(1); 
   r2=(x2 + z2 + 2*PARAM(1)*PARAM_Y - r);
   r = ( y2*(r - (x2 + z2)) - r2*r2);
-  return( min(10., max(PARAM(0)*r,-10.)) );
+  return( min(static_cast<double>(10.), max(PARAM(0)*r,static_cast<double>(-10.))) );
 }
 
 DBL f_bifolia(DBL *ptr, unsigned int) // 5
@@ -321,7 +321,7 @@
   DBL r, x2=PARAM_X*PARAM_X, z2=PARAM_Z*PARAM_Z;
   r=x2+PARAM_Y*PARAM_Y+z2;
   r= -(r*r - PARAM(1)*(x2 + z2)*PARAM_Y);
-  return( min(10., max(PARAM(0)*r,-10.)) );
+  return( min(static_cast<double>(10.), max(PARAM(0)*r,static_cast<double>(-10.))) );
 }
 
 DBL f_blob(DBL *ptr, unsigned int) // 6
@@ -356,7 +356,7 @@
   12*r2*PARAM_Z*(27*ph*ph-24*z2*ph+ 36*sqrt(2.0)*PARAM_Y*PARAM_Z*(y2-3*x2)+4*z2*z2)+
   (9*x2+9*y2-2*z2)*(-81*ph*ph-72*z2*ph+ 
   108*sqrt(2.0)*PARAM_X*PARAM_Z*(x2-3*y2)+4*z2*z2) );
-  return( min(10., max(PARAM(0)*r,-10.)) );
+  return( min(static_cast<double>(10.), max(PARAM(0)*r,static_cast<double>(-10.))) );
 }
 
 DBL f_comma(DBL *ptr, unsigned int) // 9
@@ -386,14 +386,14 @@
 {
   DBL r;
   r=( PARAM_X*PARAM_X * PARAM_Z*PARAM_Z - PARAM_Y  );
-  return( min(10., max(PARAM(0)*r,-10.)) );
+  return( min(static_cast<double>(10.), max(PARAM(0)*r,static_cast<double>(-10.))) );
 }
 
 DBL f_cubic_saddle(DBL *ptr, unsigned int) // 12
 {
   DBL r;
   r=-(PARAM_X*PARAM_X*PARAM_X - PARAM_Y*PARAM_Y*PARAM_Y - PARAM_Z);
-  return( min(10., max(PARAM(0)*r,-10.)) );
+  return( min(static_cast<double>(10.), max(PARAM(0)*r,static_cast<double>(-10.))) );
 }
 
 DBL f_cushion(DBL *ptr, unsigned int) // 13
@@ -401,14 +401,14 @@
   DBL r, x2=PARAM_X*PARAM_X, y2=PARAM_Y*PARAM_Y, z2=PARAM_Z*PARAM_Z;
   r= (z2*x2 - z2*z2 - 2*PARAM_Z*x2 + 2*z2*PARAM_Z + x2 - z2
   -(x2 - PARAM_Z)*(x2 - PARAM_Z) - y2*y2 - 2*x2*y2 - y2*z2 + 2*y2*PARAM_Z + y2);       
-  return( min(10., max(PARAM(0)*r,-10.)) );
+  return( min(static_cast<double>(10.), max(PARAM(0)*r,static_cast<double>(-10.))) );
 }
 
 DBL f_devils_curve(DBL *ptr, unsigned int) // 14
 {
   DBL r, x2=PARAM_X*PARAM_X, y2=PARAM_Y*PARAM_Y, z2=PARAM_Z*PARAM_Z;
   r=-(x2*x2 + 2*x2*z2 - 0.36*x2 - y2*y2 + 0.25*y2 + z2*z2);
-  return( min(10., max(PARAM(0)*r,-10.)) );
+  return( min(static_cast<double>(10.), max(PARAM(0)*r,static_cast<double>(-10.))) );
 }
 
 DBL f_devils_curve_2d(DBL *ptr, unsigned int) // 15
@@ -416,7 +416,7 @@
   DBL r, x2=PARAM_X*PARAM_X, y2=PARAM_Y*PARAM_Y, th;
   ROT2D(PARAM(3),PARAM(4),PARAM(5))
   r=-( x2 * (x2 - PARAM(1)*PARAM(1)) -  y2 * (y2 - PARAM(2)*PARAM(2)) );
-  return( min(10., max(PARAM(0)*r,-10.)) );
+  return( min(static_cast<double>(10.), max(PARAM(0)*r,static_cast<double>(-10.))) );
 }
 
 DBL f_dupin_cyclid(DBL *ptr, unsigned int) // 16
@@ -431,7 +431,7 @@
   2*r2*((-th-p3 + ph+p4)* (2*PARAM_X*PARAM(3)+2*PARAM_Y*PARAM(4)-r2)-4*PARAM(4)*ph*PARAM_Y)*
   (x2+y2+z2)+ 4*r2*r2*(PARAM(3)*PARAM_X+PARAM(4)*PARAM_Y)
   *(-r2+PARAM(4)*PARAM_Y+PARAM(3)*PARAM_X)+ 4*r2*r2*p4*y2+ r2*r2*r2*r2);
-  return( min(10., max(PARAM(0)*r,-10.)) );
+  return( min(static_cast<double>(10.), max(PARAM(0)*r,static_cast<double>(-10.))) );
 }
 
 DBL f_ellipsoid(DBL *ptr, unsigned int) // 17
@@ -448,7 +448,7 @@
   r =((y2-x2)/(2*PARAM_Z)+2*z2/9+2/3);
   r2=((y2-x2)/(4*PARAM_Z)-(1/4)*(x2+y2+(8/9)*z2)+2/9);
   r=-( r*r*r -6*r2*r2) ;
-  return( min(10., max(PARAM(0)*r,-10.)) );
+  return( min(static_cast<double>(10.), max(PARAM(0)*r,static_cast<double>(-10.))) );
 }
 
 DBL f_flange_cover(DBL *ptr, unsigned int) // 19
@@ -465,7 +465,7 @@
 {
   DBL r, x2=PARAM_X*PARAM_X, y2=PARAM_Y*PARAM_Y, z2=PARAM_Z*PARAM_Z;
   r=((y2 + z2) * (1+(PARAM(2) - 4*PARAM(1))*PARAM_X)+x2*(1 + PARAM(2)));
-  return( min(10., max(PARAM(0)*r,-10.)) );
+  return( min(static_cast<double>(10.), max(PARAM(0)*r,static_cast<double>(-10.))) );
 }
 
 DBL f_folium_surface_2d(DBL *ptr, unsigned int) // 21
@@ -473,14 +473,14 @@
   DBL r, x2=PARAM_X*PARAM_X, y2=PARAM_Y*PARAM_Y, z2=PARAM_Z*PARAM_Z, th;
   ROT2D(PARAM(3),PARAM(4),PARAM(5))
   r = -( y2  * (1 + (PARAM(2) - 4*PARAM(1))*PARAM_X) + x2*(1 + PARAM(2)));
-  return( min(10., max(PARAM(0)*r, -10.)) );
+  return( min(static_cast<double>(10.), max(PARAM(0)*r, static_cast<double>(-10.))) );
 }
 
 DBL f_glob(DBL *ptr, unsigned int) // 22
 {
   DBL r, x2=PARAM_X*PARAM_X;
   r= ( 0.5*x2*x2*PARAM_X + 0.5*x2*x2 - (PARAM_Y*PARAM_Y + PARAM_Z*PARAM_Z)  );
-  return( min(10., max(PARAM(0)*r,-10.)) );
+  return( min(static_cast<double>(10.), max(PARAM(0)*r,static_cast<double>(-10.))) );
 }
 
 DBL f_heart(DBL *ptr, unsigned int) // 23
@@ -488,7 +488,7 @@
   DBL r, x2=PARAM_X*PARAM_X, y2=PARAM_Y*PARAM_Y, z2=PARAM_Z*PARAM_Z;
   r=-( (2*x2+y2+z2-1)*(2*x2+y2+z2-1)*(2*x2+y2+z2-1)-
   0.1*x2*z2*PARAM_Z-y2*z2*PARAM_Z  );
-  return( min(10., max(PARAM(0)*r,-10.)) );
+  return( min(static_cast<double>(10.), max(PARAM(0)*r,static_cast<double>(-10.))) );
 }
 
 DBL f_helical_torus(DBL *ptr, unsigned int) // 24
@@ -682,7 +682,7 @@
   DBL r, x2=PARAM_X*PARAM_X, y2=PARAM_Y*PARAM_Y, z2=PARAM_Z*PARAM_Z;
   r=-( 4*(x2+y2+z2-13)*(x2+y2+z2-13)*(x2+y2+z2-13) + 
   27*(3*x2+y2-4*z2-12)*(3*x2+y2-4*z2-12)  );
-  return( min(10., max(PARAM(0)*r,-10.)) );
+  return( min(static_cast<double>(10.), max(PARAM(0)*r,static_cast<double>(-10.))) );
 }
 
 DBL f_hyperbolic_torus(DBL *ptr, unsigned int) // 31
@@ -691,7 +691,7 @@
   ph=PARAM(1)*PARAM(1); th=PARAM(2)*PARAM(2);
   r=-( x2*x2 + 2*x2*y2 - 2*x2*z2 - 2*(ph+th)*x2 + y2*y2 -
   2*y2*z2 + 2*(ph-th)*y2 + z2*z2 + 2*(ph+th)*z2 + (ph-th)*(ph-th));
-  return( min(10., max(PARAM(0)*r,-10.)) );
+  return( min(static_cast<double>(10.), max(PARAM(0)*r,static_cast<double>(-10.))) );
 }
 
 DBL f_isect_ellipsoids(DBL *ptr, unsigned int) // 32
@@ -708,7 +708,7 @@
 {
   DBL r, x2=PARAM_X*PARAM_X;
   r=-( (PARAM_Y*PARAM_Y + PARAM_Z*PARAM_Z) - PARAM(2)*PARAM(2) * x2*x2 +  PARAM(2)*PARAM(2) * PARAM(1)*PARAM(1) * x2  );
-  return( min(10., max(PARAM(0)*r,-10.)) );
+  return( min(static_cast<double>(10.), max(PARAM(0)*r,static_cast<double>(-10.))) );
 }
 
 DBL f_kampyle_of_eudoxus_2d(DBL *ptr, unsigned int) // 34
@@ -716,7 +716,7 @@
   DBL r, x2=PARAM_X*PARAM_X, y2=PARAM_Y*PARAM_Y, th;
   ROT2D(PARAM(3),PARAM(4),PARAM(5))
   r=-( y2  - PARAM(2)*PARAM(2) * x2*x2 +    PARAM(2)*PARAM(2) * PARAM(1)*PARAM(2) * x2  );
-  return( min(10., max(PARAM(0)*r,-10.)) );
+  return( min(static_cast<double>(10.), max(PARAM(0)*r,static_cast<double>(-10.))) );
 }
 
 DBL f_klein_bottle(DBL *ptr, unsigned int) // 35
@@ -724,28 +724,28 @@
   DBL r, x2=PARAM_X*PARAM_X, y2=PARAM_Y*PARAM_Y, z2=PARAM_Z*PARAM_Z;
   r=-( (x2+y2+z2+2*PARAM_Y-1)*((x2+y2+z2-2*PARAM_Y-1)*(x2+y2+z2-2*PARAM_Y-1)-8*z2)+
   16*PARAM_X*PARAM_Z*(x2+y2+z2-2*PARAM_Y-1) );
-  return( min(10., max(PARAM(0)*r,-10.)) );
+  return( min(static_cast<double>(10.), max(PARAM(0)*r,static_cast<double>(-10.))) );
 }
 
 DBL f_kummer_surface_v1(DBL *ptr, unsigned int) // 36
 {
   DBL r, x2=PARAM_X*PARAM_X, y2=PARAM_Y*PARAM_Y, z2=PARAM_Z*PARAM_Z;
   r=-( x2*x2+y2*y2+z2*z2-x2-y2-z2-x2*y2-x2*z2-y2*z2+1  );
-  return( min(10., max(PARAM(0)*r,-10.)) );
+  return( min(static_cast<double>(10.), max(PARAM(0)*r,static_cast<double>(-10.))) );
 }
 
 DBL f_kummer_surface_v2(DBL *ptr, unsigned int) // 37
 {
   DBL r, x2=PARAM_X*PARAM_X, y2=PARAM_Y*PARAM_Y, z2=PARAM_Z*PARAM_Z;
   r=-( x2*x2+y2*y2+z2*z2+PARAM(1)*(x2+y2+z2)+PARAM(2)*(x2*y2+x2*z2+y2*z2)+PARAM(3)*PARAM_X*PARAM_Y*PARAM_Z-1 );
-  return( min(10., max(PARAM(0)*r,-10.)) );
+  return( min(static_cast<double>(10.), max(PARAM(0)*r,static_cast<double>(-10.))) );
 }
 
 DBL f_lemniscate_of_gerono(DBL *ptr, unsigned int) // 38
 {
   DBL r, x2=PARAM_X*PARAM_X, y2=PARAM_Y*PARAM_Y, z2=PARAM_Z*PARAM_Z;
   r=-(  x2*x2 - x2 + y2 + z2  );
-  return( min(10., max(PARAM(0)*r,-10.)) );
+  return( min(static_cast<double>(10.), max(PARAM(0)*r,static_cast<double>(-10.))) );
 }
 
 DBL f_lemniscate_of_gerono_2d(DBL *ptr, unsigned int) // 39
@@ -753,7 +753,7 @@
   DBL r, x2=PARAM_X*PARAM_X, y2=PARAM_Y*PARAM_Y, th;
   ROT2D(PARAM(3),PARAM(4),PARAM(5))
   r=-( y2 - PARAM(2)*PARAM(2) * PARAM(1)*PARAM(1) * x2 +   PARAM(2)*PARAM(2)*x2*x2  );
-  return( min(10., max(PARAM(0)*r,-10.)) );
+  return( min(static_cast<double>(10.), max(PARAM(0)*r,static_cast<double>(-10.))) );
 }
 
 DBL f_mesh1(DBL *ptr, unsigned int) // 40
@@ -795,14 +795,14 @@
 {
   DBL r, x2=PARAM_X*PARAM_X, y2=PARAM_Y*PARAM_Y, z2=PARAM_Z*PARAM_Z;
   r=-( 4*x2*(x2 + y2 + z2) - y2*(1 - y2 - z2)  );
-  return( min(10., max(PARAM(0)*r,-10.)) );
+  return( min(static_cast<double>(10.), max(PARAM(0)*r,static_cast<double>(-10.))) );
 }
 
 DBL f_nodal_cubic(DBL *ptr, unsigned int) // 42
 {
   DBL r;
   r=-( PARAM_Y*PARAM_Y*PARAM_Y + PARAM_Z*PARAM_Z*PARAM_Z - 6*PARAM_Y*PARAM_Z );
-  return( min(10., max(PARAM(0)*r,-10.)) );
+  return( min(static_cast<double>(10.), max(PARAM(0)*r,static_cast<double>(-10.))) );
 }
 
 DBL f_odd(DBL *ptr, unsigned int) // 43
@@ -810,7 +810,7 @@
   DBL r, x2=PARAM_X*PARAM_X, y2=PARAM_Y*PARAM_Y, z2=PARAM_Z*PARAM_Z;
   r=( z2*x2 - z2*z2 - 2*PARAM_Z*x2 + 2*z2*PARAM_Z + x2 - z2 - 
   (x2 - PARAM_Z)*(x2 - PARAM_Z) - y2*y2 - 2*y2*x2 - y2*z2 + 2*y2*PARAM_Z + y2  );
-  return( min(10., max(PARAM(0)*r,-10.)) );
+  return( min(static_cast<double>(10.), max(PARAM(0)*r,static_cast<double>(-10.))) );
 }
 
 DBL f_ovals_of_cassini(DBL *ptr, unsigned int) // 44
@@ -818,14 +818,14 @@
   DBL r, r2,x2=PARAM_X*PARAM_X, y2=PARAM_Y*PARAM_Y, z2=PARAM_Z*PARAM_Z;
   r2 = (x2 + y2 + z2 + PARAM(1)*PARAM(1));
   r  = -(r2*r2 - PARAM(3)*PARAM(1)*PARAM(1)*(x2 + z2) - PARAM(2)*PARAM(2) );
-  return( min(10., max(PARAM(0)*r,-10.)) );
+  return( min(static_cast<double>(10.), max(PARAM(0)*r,static_cast<double>(-10.))) );
 }
 
 DBL f_paraboloid(DBL *ptr, unsigned int) // 45
 {
   DBL r;
   r=-( PARAM_X*PARAM_X - PARAM_Y + PARAM_Z*PARAM_Z);
-  return( min(10., max(PARAM(0)*r,-10.)) );
+  return( min(static_cast<double>(10.), max(PARAM(0)*r,static_cast<double>(-10.))) );
 }
 
 DBL f_parabolic_torus(DBL *ptr, unsigned int) // 46
@@ -834,7 +834,7 @@
   ph=PARAM(1)*PARAM(1); th=PARAM(2)*PARAM(2);
   r=-(  x2*x2 + 2*x2*y2 - 2*x2*PARAM_Z - (ph+th)*x2 + y2*y2 - 2*y2*PARAM_Z +
   (ph-th)*y2 + z2 + (ph+th)*PARAM_Z +  (ph-th)* (ph-th)  );
-  return( min(10., max(PARAM(0)*r,-10.)) );
+  return( min(static_cast<double>(10.), max(PARAM(0)*r,static_cast<double>(-10.))) );
 }
 
 DBL f_ph(DBL *ptr, unsigned int) // 47
@@ -846,14 +846,14 @@
 {
   DBL r, x2=PARAM_X*PARAM_X, y2=PARAM_Y*PARAM_Y, z2=PARAM_Z*PARAM_Z;
   r=( x2*x2 + y2*y2 + z2*z2 - (x2 + y2 + z2)  );
-  return( min(10., max(PARAM(0)*r,-10.)) );
+  return( min(static_cast<double>(10.), max(PARAM(0)*r,static_cast<double>(-10.))) );
 }
 
 DBL f_piriform(DBL *ptr, unsigned int) // 49
 {
   DBL r, x2=PARAM_X*PARAM_X;
   r=-( (x2*x2 - x2*PARAM_X) + PARAM_Y*PARAM_Y + PARAM_Z*PARAM_Z  );
-  return( -min(10., max(PARAM(0)*r,-10.)) );
+  return( -min(static_cast<double>(10.), max(PARAM(0)*r,static_cast<double>(-10.))) );
 }
 
 DBL f_piriform_2d(DBL *ptr, unsigned int) // 50
@@ -861,7 +861,7 @@
   DBL r, x2=PARAM_X*PARAM_X, y2=PARAM_Y*PARAM_Y,th;
   ROT2D(PARAM(4),PARAM(5),PARAM(6))
   r=-( PARAM_Y*PARAM_Y - PARAM(1) * PARAM(3)* PARAM(3) * x2*PARAM_X -  PARAM(2) * PARAM(3)*PARAM(3) * x2*x2  );       
-  return( min(10., max(PARAM(0)*r,-10.)) );
+  return( min(static_cast<double>(10.), max(PARAM(0)*r,static_cast<double>(-10.))) );
 }
 
 DBL f_poly4(DBL *ptr, unsigned int) // 51
@@ -908,21 +908,21 @@
 {
   DBL r, x2=PARAM_X*PARAM_X, z2=PARAM_Z*PARAM_Z;
   r=-( x2*x2 + z2*z2 - PARAM_Y  );
-  return( min(10., max(PARAM(0)*r,-10.)) );
+  return( min(static_cast<double>(10.), max(PARAM(0)*r,static_cast<double>(-10.))) );
 }
 
 DBL f_quartic_saddle(DBL *ptr, unsigned int) // 55
 {
   DBL r, x2=PARAM_X*PARAM_X, z2=PARAM_Z*PARAM_Z;
   r=-( x2*x2 - z2*z2 - PARAM_Y  );
-  return( min(10., max(PARAM(0)*r,-10.)) );
+  return( min(static_cast<double>(10.), max(PARAM(0)*r,static_cast<double>(-10.))) );
 }
 
 DBL f_quartic_cylinder(DBL *ptr, unsigned int) // 56
 {
   DBL r, x2=PARAM_X*PARAM_X, z2=PARAM_Z*PARAM_Z;
   r=-( (x2 + z2) * PARAM_Y*PARAM_Y + PARAM(2)*PARAM(2) * (x2 + z2) - PARAM(2)*PARAM(2) * PARAM(1)*PARAM(1)  );
-  return( min(10., max(PARAM(0)*r,-10.)) );
+  return( min(static_cast<double>(10.), max(PARAM(0)*r,static_cast<double>(-10.))) );
 }
 
 DBL f_r(DBL *ptr, unsigned int) // 57
@@ -1108,7 +1108,7 @@
 {
   DBL r, x2=PARAM_X*PARAM_X, y2=PARAM_Y*PARAM_Y, z2=PARAM_Z*PARAM_Z;
   r=-( x2*y2 + x2*z2 + y2*z2 + PARAM_X*PARAM_Y*PARAM_Z  );
-  return( min(10., max(PARAM(0)*r,-10.)) );
+  return( min(static_cast<double>(10.), max(PARAM(0)*r,static_cast<double>(-10.))) );
 }
 
 DBL f_strophoid(DBL *ptr, unsigned int) // 66
@@ -1116,7 +1116,7 @@
   DBL r, r2,x2=PARAM_X*PARAM_X;
   r2=PARAM(3)*PARAM(3);
   r=-((PARAM(2) - PARAM_X)*(PARAM_Y*PARAM_Y + PARAM_Z*PARAM_Z) - r2*PARAM(1)*x2 - r2*x2*PARAM_X );
-  return( min(10., max(PARAM(0)*r,-10.)) );
+  return( min(static_cast<double>(10.), max(PARAM(0)*r,static_cast<double>(-10.))) );
 }
 
 DBL f_strophoid_2d(DBL *ptr, unsigned int) // 67
@@ -1125,7 +1125,7 @@
   ROT2D(PARAM(4),PARAM(5),PARAM(6))
   r2=PARAM(3)*PARAM(3);
   r=-( (PARAM(2) - PARAM_X)*y2  - r2*PARAM(1)*x2 -  r2*x2*PARAM_X  );
-  return( min(10., max(PARAM(0)*r,-10.)) );
+  return( min(static_cast<double>(10.), max(PARAM(0)*r,static_cast<double>(-10.))) );
 }
 
 DBL f_superellipsoid(DBL *ptr, unsigned int) // 68
@@ -1152,28 +1152,28 @@
   DBL r, ph,th,x2=PARAM_X*PARAM_X, y2=PARAM_Y*PARAM_Y, z2=PARAM_Z*PARAM_Z;
   ph=PARAM(1)*PARAM(1); th=PARAM(2)*PARAM(2);
   r=-( x2*x2 + y2*y2 + z2*z2 + 2*x2*y2 + 2*x2*z2 + 2*y2*z2 -2* (ph + th)* x2 + 2* (ph - th)* y2 -2* (ph + th)* z2 + (ph - th)*(ph - th)  );
-  return( min(10., max(PARAM(0)*r,-10.)) );
+  return( min(static_cast<double>(10.), max(PARAM(0)*r,static_cast<double>(-10.))) );
 }
 
 DBL f_torus_gumdrop(DBL *ptr, unsigned int) // 72
 {
   DBL r, x2=PARAM_X*PARAM_X, y2=PARAM_Y*PARAM_Y, z2=PARAM_Z*PARAM_Z;
   r=-( 4*(x2*x2 + (y2 + z2)*(y2 + z2)) + 17 * x2 * (y2 + z2) - 20 * (x2 + y2 + z2) + 17);
-  return( min(10., max(PARAM(0)*r,-10.)) );
+  return( min(static_cast<double>(10.), max(PARAM(0)*r,static_cast<double>(-10.))) );
 }
 
 DBL f_umbrella(DBL *ptr, unsigned int) // 73
 {
   DBL r;
   r=-( PARAM_X*PARAM_X - PARAM_Y*PARAM_Z*PARAM_Z );
-  return( min(10., max(PARAM(0)*r,-10.)) );
+  return( min(static_cast<double>(10.), max(PARAM(0)*r,static_cast<double>(-10.))) );
 }
 
 DBL f_witch_of_agnesi(DBL *ptr, unsigned int) // 74
 {
   DBL r;
   r=-( PARAM(1) * (PARAM_Y - 1) + (PARAM_X*PARAM_X + PARAM_Z*PARAM_Z) * PARAM_Y   );
-  return( min(10., max(PARAM(0)*r,-10.)) );
+  return( min(static_cast<double>(10.), max(PARAM(0)*r,static_cast<double>(-10.))) );
 }
 
 DBL f_witch_of_agnesi_2d(DBL *ptr, unsigned int) // 75
@@ -1181,7 +1181,7 @@
   DBL r, x2=PARAM_X*PARAM_X, y2=PARAM_Y*PARAM_Y, th;
   ROT2D(PARAM(3),PARAM(4),PARAM(5))
   r=-( PARAM(1)*PARAM(1) * PARAM_Y + x2 * PARAM_Y - PARAM(2)  );
-  return( min(10.0, max(PARAM(0)*r,-10.0)) );
+  return( min(static_cast<double>(10.0), max(PARAM(0)*r,static_cast<double>(-10.0))) );
 }
 
 DBL f_noise3d(DBL *ptr, unsigned int) // 76
