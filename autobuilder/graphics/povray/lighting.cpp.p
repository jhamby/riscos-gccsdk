--- source/lighting.cpp.orig	2004-08-03 00:11:36.000000000 +0100
+++ source/lighting.cpp	2011-11-27 15:00:07.000000000 +0000
@@ -2133,7 +2133,7 @@
 
       F = 0.014567225 / Sqr(x - 1.12) - 0.011612903;
 
-      F=min(1.0,max(0.0,F));
+      F=min(static_cast<double>(1.0),max(static_cast<double>(0.0),F));
       Cs[pRED]   = Light_Colour[pRED]   * (1.0 + Finish->Metallic * (1.0 - F) * (Layer_Pigment_Colour[pRED]   - 1.0));
       Cs[pGREEN] = Light_Colour[pGREEN] * (1.0 + Finish->Metallic * (1.0 - F) * (Layer_Pigment_Colour[pGREEN] - 1.0));
       Cs[pBLUE]  = Light_Colour[pBLUE]  * (1.0 + Finish->Metallic * (1.0 - F) * (Layer_Pigment_Colour[pBLUE]  - 1.0));
@@ -2228,7 +2228,7 @@
 
         F = 0.014567225 / Sqr(x - 1.12) - 0.011612903;
 
-        F=min(1.0,max(0.0,F));
+        F=min(static_cast<double>(1.0),max(static_cast<double>(0.0),F));
         Cs[pRED]   = Light_Colour[pRED]   * (1.0 + Finish->Metallic * (1.0 - F) * (Layer_Pigment_Colour[pRED]   - 1.0));
         Cs[pGREEN] = Light_Colour[pGREEN] * (1.0 + Finish->Metallic * (1.0 - F) * (Layer_Pigment_Colour[pGREEN] - 1.0));
         Cs[pBLUE]  = Light_Colour[pBLUE]  * (1.0 + Finish->Metallic * (1.0 - F) * (Layer_Pigment_Colour[pBLUE]  - 1.0));
@@ -3887,7 +3887,7 @@
 
         DBL x = fabs(acos(Cos_Angle_Incidence)) / M_PI_2;
         DBL F = 0.014567225 / Sqr(x - 1.12) - 0.011612903;
-        F=min(1.0,max(0.0,F));
+        F=min(static_cast<double>(1.0),max(static_cast<double>(0.0),F));
 
         ListReflec[layer_number][0]*=
           (1.0 + R_M * (1.0 - F) * (LayCol[0] - 1.0));
@@ -4025,7 +4025,7 @@
     /* Get new remaining translucency. */
 
     /* NK layers - changed this */
-    Trans = min(1.0, fabs(FilCol[pFILTER]*GREY_SCALE(FilCol)) + fabs(FilCol[pTRANSM]));
+    Trans = min(static_cast<double>(1.0), fabs(FilCol[pFILTER]*GREY_SCALE(FilCol)) + fabs(FilCol[pTRANSM]));
     /* NK ---- */
   }
 
@@ -4771,7 +4771,7 @@
     New_Weight = Weight * Trans;
     colour_found = Compute_Pigment (LayCol, Layer->Pigment, IPoint, Intersection);
     
-    Att = Trans * (1.0 - min(1.0, (DBL)(LayCol[pFILTER] + LayCol[pTRANSM])));
+    Att = Trans * (1.0 - min(static_cast<double>(1.0), (DBL)(LayCol[pFILTER] + LayCol[pTRANSM])));
 
     LayCol[0]*=FilCol[0];
     LayCol[1]*=FilCol[1];
@@ -4823,7 +4823,7 @@
 
       DBL x = fabs(acos(Cos_Angle_Incidence)) / M_PI_2;
       DBL F = 0.014567225 / Sqr(x - 1.12) - 0.011612903;
-      F=min(1.0,max(0.0,F));
+      F=min(static_cast<double>(1.0),max(static_cast<double>(0.0),F));
 
       ListReflec[layer_number][0]*=
         (1.0 + R_M * (1.0 - F) * (LayCol[0] - 1.0));
@@ -4867,7 +4867,7 @@
     }
 
     /* Get new remaining translucency. */
-    Trans = min(1.0, fabs(FilCol[pFILTER]*GREY_SCALE(FilCol)));
+    Trans = min(static_cast<double>(1.0), fabs(FilCol[pFILTER]*GREY_SCALE(FilCol)));
   }
 
   /*******************
@@ -4888,14 +4888,14 @@
   {
     /* if photon is for global map, then decide which we want to do
     */
-    diffuseWeight = max(0.0, fabs(GREY_SCALE(ResCol)));
+    diffuseWeight = max(static_cast<double>(0.0), fabs(GREY_SCALE(ResCol)));
     /* use top-layer finish only */
     if(Texture->Finish)
       diffuseWeight*=Texture->Finish->Diffuse;
     refractionWeight = Trans;
     /* reflection only for top layer!!!!!! */
-    reflectionWeight = max(0.0, fabs(GREY_SCALE(ListReflec[0])));
-    dieWeight = max(0.0,(1.0-diffuseWeight));
+    reflectionWeight = max(static_cast<double>(0.0), fabs(GREY_SCALE(ListReflec[0])));
+    dieWeight = max(static_cast<double>(0.0),(1.0-diffuseWeight));
 
     /* normalize weights: make sum be 1.0 */
     totalWeight = reflectionWeight + refractionWeight + diffuseWeight + dieWeight;
@@ -5734,7 +5734,7 @@
             F = 0.5 * (Sqr(g - cos_angle) / Sqr(g + cos_angle));
             F = F * (1.0 + Sqr(cos_angle * (g + cos_angle) - 1.0) / Sqr(cos_angle * (g - cos_angle) + 1.0));
 
-            F=min(1.0,max(0.0,F));
+            F=min(static_cast<double>(1.0),max(static_cast<double>(0.0),F));
             CRGBLinComb2(reflectivity,F,Reflection_Max,(1.0-F),Reflection_Min);
           }
           else
