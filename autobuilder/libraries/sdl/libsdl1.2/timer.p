Index: src/timer/SDL_timer.c
===================================================================
RCS file: /home/sdlweb/libsdl.org/cvs/SDL12/src/timer/SDL_timer.c,v
retrieving revision 1.5
diff -u -b -r1.5 SDL_timer.c
--- src/timer/SDL_timer.c	13 Jan 2005 23:24:56 -0000	1.5
+++ src/timer/SDL_timer.c	7 Jul 2005 11:40:01 -0000
@@ -287,7 +287,7 @@
		}
	}
	if ( SDL_timer_threaded ) {
-		SDL_mutexP(SDL_timer_mutex);
+		SDL_mutexV(SDL_timer_mutex);
	}

	return retval;


