--- src/ag.c.orig	2010-08-20 20:08:52.000000000 +0100
+++ src/ag.c	2010-08-20 20:07:38.000000000 +0100
@@ -74,7 +74,7 @@
 //module level variables for game control
 char shuffle[]  = "£££££££";
 char answer[]   = "£££££££";
-char language[10];
+char language[64];
 char txt[50];
 char* rootWord;
 int updateAnswers = 0;
@@ -189,15 +189,15 @@
 ***********************************************************/
 void bufferSounds(struct sound** soundCache){
 
-	pushSound(&(*soundCache),"click-answer", "audio/click-answer.wav");
-	pushSound(&(*soundCache),"click-shuffle", "audio/click-shuffle.wav");
-	pushSound(&(*soundCache),"foundbig", "audio/foundbig.wav");
-	pushSound(&(*soundCache),"found", "audio/found.wav");
-	pushSound(&(*soundCache),"clear", "audio/clearword.wav");
-	pushSound(&(*soundCache),"duplicate", "audio/duplicate.wav");
-	pushSound(&(*soundCache),"badword", "audio/badword.wav");
-	pushSound(&(*soundCache),"shuffle", "audio/shuffle.wav");
-	pushSound(&(*soundCache),"clock-tick", "audio/clock-tick.wav");
+	pushSound(&(*soundCache),"click-answer", "/<Anagramarama$Dir>/audio/click-answer.wav");
+	pushSound(&(*soundCache),"click-shuffle", "/<Anagramarama$Dir>/audio/click-shuffle.wav");
+	pushSound(&(*soundCache),"foundbig", "/<Anagramarama$Dir>/audio/foundbig.wav");
+	pushSound(&(*soundCache),"found", "/<Anagramarama$Dir>/audio/found.wav");
+	pushSound(&(*soundCache),"clear", "/<Anagramarama$Dir>/audio/clearword.wav");
+	pushSound(&(*soundCache),"duplicate", "/<Anagramarama$Dir>/audio/duplicate.wav");
+	pushSound(&(*soundCache),"badword", "/<Anagramarama$Dir>/audio/badword.wav");
+	pushSound(&(*soundCache),"shuffle", "/<Anagramarama$Dir>/audio/shuffle.wav");
+	pushSound(&(*soundCache),"clock-tick", "/<Anagramarama$Dir>/audio/clock-tick.wav");
 
 }
 
@@ -1732,7 +1732,7 @@
 SDL_Surface *screen;
 struct sprite* letters = NULL;
 //pthread_t audio;	
-	strcpy(language,"i18n/");
+	strcpy(language,"/<Anagramarama$Dir>/i18n/");
 
 	if(argc == 2) {
 		strcat(language,argv[1]);
@@ -1741,7 +1741,7 @@
 		char * language2 = getenv("LANG");
 	
 		if(language2 == 0){
-			strcpy(language,"i18n/en_GB/");
+			strcpy(language,"/<Anagramarama$Dir>/i18n/en_GB/");
         	}
 		else {
 			char local[10];
