--- src/events.cpp.orig	2010-01-01 13:16:49.000000000 +0000
+++ src/events.cpp	2011-03-31 09:41:56.138752000 +0100
@@ -275,6 +275,7 @@
 	static int last_mouse_down = -1;
 	static int last_click_x = -1, last_click_y = -1;
 
+#ifndef __riscos__
 	SDL_Event temp_event;
 	int poll_count = 0;
 	int begin_ignoring = 0;
@@ -301,6 +302,10 @@
 	std::vector<SDL_Event>::iterator ev_end = events.end();
 	for(ev_it = events.begin(); ev_it != ev_end; ++ev_it){
 		SDL_Event &event = *ev_it;
+#else
+		SDL_Event event;
+		while (SDL_PollEvent(&event)) {
+#endif
 		switch(event.type) {
 
 			case SDL_ACTIVEEVENT: {
@@ -460,7 +465,9 @@
 	int discard_count = 0;
 	SDL_Event temp_event;
 	std::vector< SDL_Event > keepers;
+#ifndef __riscos__
 	SDL_Delay(10);
+#endif
 	while(SDL_PollEvent(&temp_event) > 0) {
 		if((SDL_EVENTMASK(temp_event.type) & event_mask) == 0) {
 			keepers.push_back( temp_event );
