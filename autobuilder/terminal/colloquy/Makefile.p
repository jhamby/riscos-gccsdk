--- Makefile.old	2003-04-24 09:00:16.000000000 +0100
+++ Makefile	2003-04-24 08:00:34.000000000 +0100
@@ -4,7 +4,7 @@
 	rm -rf build/*
 	[ -d build ] || ( rm -rf build; mkdir build )
 	cd src; $(MAKE)
-	cp -r src/colloquy src/resolver.lua src/config.lua data build/
+	cp -r src/colloquy,ff8 src/resolver.lua src/config.lua data build/
 	rm -f build/data/bans build/data/*.lua
 	chmod -R +w build/*
 
