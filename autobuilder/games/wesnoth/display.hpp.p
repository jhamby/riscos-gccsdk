--- src/display.hpp.orig	2008-08-05 16:16:29.200000000 +0100
+++ src/display.hpp	2008-08-05 16:20:06.776000000 +0100
@@ -254,7 +254,7 @@
 
 	double turbo_speed() const;
 
-	void set_turbo_speed(const double speed) { turbo_speed_ = speed; }
+	void set_turbo_speed(const double speed) { turbo_speed_ = (speed < 1.0) ? 1.0 : speed; }
 
 	// control unit idle animations and their frequency
 	void set_idle_anim(bool ison) { idle_anim_ = ison; }
