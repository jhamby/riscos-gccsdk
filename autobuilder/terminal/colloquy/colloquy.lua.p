--- src/colloquy.lua	2003-04-05 20:09:28.000000000 +0100
+++ src/colloquy.lua.new	2003-04-05 21:09:36.000000000 +0100
@@ -3,12 +3,12 @@
 
 print("+++ Colloquy - Copyright (c) 2002-2003 The Pepperfish Consortium");
 
-if (not stringStack) then dofile("stringstack.lua") end;
-if (not clientSocket) then dofile("sockets.lua") end;
-if (not chattr) then dofile("colouring.lua") end;
-if (not connections) then dofile("connections.lua") end;
-if (not loadUsers) then dofile("users.lua") end;
-if (not swears) then dofile("swearing.lua") end;
+if (not stringStack) then dofile("<Colloquy$Dir>/stringstack.lua") end;
+if (not clientSocket) then dofile("<Colloquy$Dir>/sockets.lua") end;
+if (not chattr) then dofile("<Colloquy$Dir>/colouring.lua") end;
+if (not connections) then dofile("<Colloquy$Dir>/connections.lua") end;
+if (not loadUsers) then dofile("<Colloquy$Dir>/users.lua") end;
+if (not swears) then dofile("<Colloquy$Dir>/swearing.lua") end;
 
 colloquy = {
    version = "1.11",
@@ -93,9 +93,9 @@
 dataSent = 0;
 dataRead = 0;
 
-if (not lists) then dofile("lists.lua") end;
-if (not commands) then dofile("commands.lua") end;
-if (not selectery) then dofile("main.lua") end;
+if (not lists) then dofile("<Colloquy$Dir>/lists.lua") end;
+if (not commands) then dofile("<Colloquy$Dir>/commands.lua") end;
+if (not selectery) then dofile("<Colloquy$Dir>/main.lua") end;
 
 print("+++ Version " .. colloquy.version .. " [" .. colloquy.date .. "] (" .. colloquy.os .. ")");
 
--- src/colloquy.lua.old	2003-04-24 09:04:22.000000000 +0100
+++ src/colloquy.lua	2003-04-24 08:04:22.000000000 +0100
@@ -52,7 +52,7 @@
 if( arg[1] ) then
    dofile(arg[1])
 else
-   dofile("config.lua");
+   dofile("<Colloquy$Dir>/config.lua");
 end
 
 colloquy.os = (UNAME_SYSTEM or "Unknown") .. " " .. (UNAME_MACHINE or "");
