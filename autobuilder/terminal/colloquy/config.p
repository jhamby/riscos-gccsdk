--- src/config.lua.old	2003-04-05 22:10:19.000000000 +0100
+++ src/config.lua	2003-04-05 21:10:54.000000000 +0100
@@ -28,13 +28,13 @@
 
 -- filename of the file stuffed to users when they
 -- connect, before logging on.
-colloquy.welcome = "data/misc/welcome";
+colloquy.welcome = "<Colloquy$Dir>/data/misc/welcome";
 
 -- filename of the users database.
-colloquy.users = "data/users.lua";
+colloquy.users = "<Colloquy$Dir>/data/users.lua";
 
 -- directory where .help data is stored.
-colloquy.help = "data/help/";
+colloquy.help = "<Colloquy$Dir>/data/help/";
 
 -- where the resolver process should bind.
 colloquy.resolverIP = "127.0.0.1";
@@ -65,7 +65,7 @@
 };
 
 -- datafile for the lists
-colloquy.listsFile = "data/lists.lua";
+colloquy.listsFile = "<Colloquy$Dir>/data/lists.lua";
 
 -- how many non-permanent lists should users be allowed?
 colloquy.listQuota = 5;
@@ -74,15 +74,15 @@
 colloquy.listExpirey = 14;
 
 -- datafile for the bans
-colloquy.banFile = "data/bans";
+colloquy.banFile = "<Colloquy$Dir>/data/bans";
 
--- the talker's name.  "the talker" is default for gramatical correctness
+-- the talker's name.  "the talker" is default for grammatical correctness
 -- in some messages.
 colloquy.talkerName = "the talker";
 
 -- where to put the logfile
-logName = "logfile.txt";
+colloquy.logName = "<Colloquy$Dir>/logfile.txt";
 
 -- command to execute to retate log files.  Set to nil if you don't want
 -- daily rotations.  It is first passed through a strftime-like function.
-logRotate = "gzip -c9 logfile.txt > logfile.%Y%m%d.gz";
+colloquy.logRotate = "gzip -c9 logfile.txt > logfile.%Y%m%d.gz";
