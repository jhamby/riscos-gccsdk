Index: src/video/riscos/SDL_riscostask.c
===================================================================
RCS file: /home/sdlweb/libsdl.org/cvs/SDL12/src/video/riscos/SDL_riscostask.c,v
retrieving revision 1.4
diff -u -r1.4 SDL_riscostask.c
--- src/video/riscos/SDL_riscostask.c	12 Feb 2005 18:01:30 -0000	1.4
+++ src/video/riscos/SDL_riscostask.c	10 May 2005 14:04:43 -0000
@@ -52,7 +52,6 @@
 /* RISC OS variables to help compatability with certain programs */
 int riscos_backbuffer = 0; /* Create a back buffer in system memory for full screen mode */
 int riscos_closeaction = 1; /* Close icon action */
-int riscos_audiobuffer = 0; /* Audio buffer size */
 
 static int stored_mode = -1; /* -1 when in desktop, mode number or pointer when full screen */
 
@@ -178,13 +177,6 @@
                     0 Don't show close icon
                     1 Show close icon
 
-               SDL$<name>$AudioBuffer - set to number of samples to buffer
-                    in advance. Will default to a minimum of 1024 or twice
-                    amount requested by program whichever is largest.
-                    If not specified default is amount for 10 csecs.
-                    Time that will be pre-buffered can be calculated as
-                    sample to buffer * 1000 / freq milliseconds.
-                    
 ***************************************************************************/
 
 int RISCOS_GetTaskName(char *task_name)
@@ -255,13 +247,6 @@
 			   env_val = getenv(env_var);
 			   if (env_val && strcmp(env_val,"0") == 0) riscos_closeaction = 0;
 
-			   strcpy(env_var, "SDL$");
-			   strcat(env_var, p);
-			   strcat(env_var, "$AudioBuffer");
-
-			   env_val = getenv(env_var);
-			   riscos_audiobuffer = atoi(env_val);
-
 			   free(env_var);
 		   }
 		   

