--- src/display.cpp.orig	2008-08-05 16:16:38.117000000 +0100
+++ src/display.cpp	2008-08-05 16:19:09.300000000 +0100
@@ -1735,10 +1735,7 @@
 	}
 
 	res |= screen_.faked();
-	if (res)
-		return turbo_speed_;
-	else
-		return 1.0;
+	return (res && (turbo_speed_ >= 1.0)) ? turbo_speed_ : 1.0;
 }
 
 void display::set_idle_anim_rate(int rate)
