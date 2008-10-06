--- src/events.cpp.orig	2008-08-05 16:17:18.323000000 +0100
+++ src/events.cpp	2008-08-05 16:25:45.919000000 +0100
@@ -272,6 +272,7 @@
 	static int last_mouse_down = -1;
 	static int last_click_x = -1, last_click_y = -1;
 
+#ifndef __riscos__
 	SDL_Event temp_event;
 	int poll_count = 0;
 	int begin_ignoring = 0;
@@ -298,6 +299,10 @@
 	std::vector<SDL_Event>::iterator ev_end = events.end();
 	for(ev_it = events.begin(); ev_it != ev_end; ++ev_it){
 		SDL_Event &event = *ev_it;
+#else
+		SDL_Event event;
+		while (SDL_PollEvent(&event)) {
+#endif
 		switch(event.type) {
 
 			case SDL_ACTIVEEVENT: {
@@ -457,7 +462,9 @@
 	int discard_count = 0;
 	SDL_Event temp_event;
 	std::vector< SDL_Event > keepers;
+#ifndef __riscos__
 	SDL_Delay(10);
+#endif
 	while(SDL_PollEvent(&temp_event) > 0) {
 		if((SDL_EVENTMASK(temp_event.type) & event_mask) == 0) {
 			keepers.push_back( temp_event );
