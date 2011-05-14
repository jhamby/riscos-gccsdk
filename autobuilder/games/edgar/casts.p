--- ./src/system/properties.h.orig	2009-11-25 13:54:55.000000000 -0800
+++ ./src/system/properties.h	2009-11-25 13:55:04.000000000 -0800
@@ -17,10 +17,10 @@
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

-void loadProperties(char *, Entity *);
-void setProperty(Entity *, char *, char *);
-char *getEntityTypeByID(int);
-int getEntityTypeByName(char *);
-void freeProperties(void);
-void setFlags(Entity *, char *);
-void unsetFlags(Entity *, char *);
+void loadProperties(const char *, Entity *);
+void setProperty(Entity *, char *, char *);
+char *getEntityTypeByID(int);
+int getEntityTypeByName(char *);
+void freeProperties(void);
+void setFlags(Entity *, char *);
+void unsetFlags(Entity *, char *);
--- ./src/system/properties.c.orig	2009-11-25 13:55:09.000000000 -0800
+++ ./src/system/properties.c	2009-11-25 13:55:17.000000000 -0800
@@ -81,7 +81,7 @@
 	memset(properties, 0, sizeof(Properties) * MAX_PROPS_FILES);
 }

-void loadProperties(char *name, Entity *e)
+void loadProperties(const char *name, Entity *e)
 {
 	int i, j, index, animationIndex, graphicsIndex, sprites[256];
 	char path[MAX_PATH_LENGTH], *line, *token, *savePtr1, *savePtr2;
--- ./src/boss/blob_boss.c.orig	2009-11-25 13:59:52.000000000 -0800
+++ ./src/boss/blob_boss.c	2009-11-25 14:00:21.000000000 -0800
@@ -357,7 +357,7 @@

 	self->startX = t->x;

-	t = getTargetByName(player.x < self->x ? "BLOB_TARGET_LEFT" : "BLOB_TARGET_RIGHT");
+	t = getTargetByName((char *)(player.x < self->x ? "BLOB_TARGET_LEFT" : "BLOB_TARGET_RIGHT"));

 	if (t == NULL)
 	{
--- ./src/menu/control_menu.c.orig	2009-11-25 13:58:11.000000000 -0800
+++ ./src/menu/control_menu.c	2009-11-25 13:58:58.000000000 -0800
@@ -555,7 +555,7 @@

 	else
 	{
-		text = SDL_GetKeyName(key);
+		text = SDL_GetKeyName((SDLKey)key);
 	}

 	return text;
--- ./src/event/objective.c.orig	2009-11-25 13:53:41.000000000 -0800
+++ ./src/event/objective.c	2009-11-25 13:54:07.000000000 -0800
@@ -69,7 +69,7 @@
 		showErrorAndExit("Objective name is missing");
 	}

-	addObjective(value[objectiveName], completionTrigger == -1 ? "" : value[completionTrigger]);
+	addObjective(value[objectiveName], (char *)(completionTrigger == -1 ? "" : value[completionTrigger]));
 }

 void addObjective(char *objectiveName, char *completionTrigger)
--- ./src/main.c.orig	2009-11-25 13:48:10.000000000 -0800
+++ ./src/main.c	2009-11-25 13:49:27.000000000 -0800
@@ -133,7 +133,7 @@

 	if (loadSlot == -1)
 	{
-		firstMap = game.firstRun == TRUE ? "tutorial" : "map01";
+		firstMap = (char *)(game.firstRun == TRUE ? "tutorial" : "map01");

 		if (recordingID != -1)
 		{
