--- source/atmosph.cpp.orig	2004-08-03 00:11:35.000000000 +0100
+++ source/atmosph.cpp	2011-11-27 15:00:07.000000000 +0000
@@ -690,7 +690,7 @@
 
     k = exp(-Width / Fog->Distance);
 
-    Width *= 1.0 - k * min(1.0, Turbulence(P, Fog->Turb, NULL)*Fog->Turb_Depth);
+    Width *= 1.0 - k * std::min(static_cast<double>(1.0), Turbulence(P, Fog->Turb, NULL)*Fog->Turb_Depth);
   }
 
   Assign_Colour(Colour, Fog->Colour);
@@ -822,7 +822,7 @@
 
     k = exp(-Width / Fog->Distance);
 
-    Width *= 1.0 - k * min(1.0, Turbulence(P, Fog->Turb, NULL)*Fog->Turb_Depth);
+    Width *= 1.0 - k * std::min(static_cast<double>(1.0), Turbulence(P, Fog->Turb, NULL)*Fog->Turb_Depth);
   }
 
   Assign_Colour(Colour, Fog->Colour);
