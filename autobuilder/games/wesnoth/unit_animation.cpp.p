--- src/unit_animation.cpp.orig	2008-09-08 13:44:53.952642400 +0100
+++ src/unit_animation.cpp	2008-09-08 13:49:02.842642400 +0100
@@ -956,7 +956,8 @@
 	game_display*disp = game_display::get_singleton();
 	double speed = disp->turbo_speed();
 	int end_tick = animated_units_[0].my_unit->get_animation()->time_to_tick(animation_time);
-	while (SDL_GetTicks() < (unsigned int)end_tick - 20/speed) {
+        unsigned int tail = (unsigned int)(20 / speed); // No need to recalculate this on each loop
+	while (SDL_GetTicks() < (unsigned int)end_tick - tail) {
 		disp->draw();
                 end_tick = animated_units_[0].my_unit->get_animation()->time_to_tick(animation_time);
 		events::pump();
