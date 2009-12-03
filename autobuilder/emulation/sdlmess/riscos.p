--- ./src/emu/memory.h.orig	2009-12-02 15:00:34.000000000 -0800
+++ ./src/emu/memory.h	2009-12-02 15:02:06.000000000 -0800
@@ -1080,21 +1080,21 @@
 INLINE UINT16 memory_decrypted_read_word(const address_space *space, offs_t byteaddress)
 {
 	if (!memory_address_outside_direct_region(space, byteaddress) || memory_set_direct_region(space, &byteaddress))
-		return *(UINT16 *)&space->direct.decrypted[byteaddress & space->direct.bytemask];
+		return *(UINT16 *)(int)&space->direct.decrypted[byteaddress & space->direct.bytemask];
 	return memory_read_word(space, byteaddress);
 }
 
 INLINE UINT32 memory_decrypted_read_dword(const address_space *space, offs_t byteaddress)
 {
 	if (!memory_address_outside_direct_region(space, byteaddress) || memory_set_direct_region(space, &byteaddress))
-		return *(UINT32 *)&space->direct.decrypted[byteaddress & space->direct.bytemask];
+		return *(UINT32 *)(int)&space->direct.decrypted[byteaddress & space->direct.bytemask];
 	return memory_read_dword(space, byteaddress);
 }
 
 INLINE UINT64 memory_decrypted_read_qword(const address_space *space, offs_t byteaddress)
 {
 	if (!memory_address_outside_direct_region(space, byteaddress) || memory_set_direct_region(space, &byteaddress))
-		return *(UINT64 *)&space->direct.decrypted[byteaddress & space->direct.bytemask];
+		return *(UINT64 *)(int)&space->direct.decrypted[byteaddress & space->direct.bytemask];
 	return memory_read_qword(space, byteaddress);
 }
 
@@ -1123,21 +1123,21 @@
 INLINE UINT16 memory_raw_read_word(const address_space *space, offs_t byteaddress)
 {
 	if (!memory_address_outside_direct_region(space, byteaddress) || memory_set_direct_region(space, &byteaddress))
-		return *(UINT16 *)&space->direct.raw[byteaddress & space->direct.bytemask];
+		return *(UINT16 *)(int)&space->direct.raw[byteaddress & space->direct.bytemask];
 	return memory_read_word(space, byteaddress);
 }
 
 INLINE UINT32 memory_raw_read_dword(const address_space *space, offs_t byteaddress)
 {
 	if (!memory_address_outside_direct_region(space, byteaddress) || memory_set_direct_region(space, &byteaddress))
-		return *(UINT32 *)&space->direct.raw[byteaddress & space->direct.bytemask];
+		return *(UINT32 *)(int)&space->direct.raw[byteaddress & space->direct.bytemask];
 	return memory_read_dword(space, byteaddress);
 }
 
 INLINE UINT64 memory_raw_read_qword(const address_space *space, offs_t byteaddress)
 {
 	if (!memory_address_outside_direct_region(space, byteaddress) || memory_set_direct_region(space, &byteaddress))
-		return *(UINT64 *)&space->direct.raw[byteaddress & space->direct.bytemask];
+		return *(UINT64 *)(int)&space->direct.raw[byteaddress & space->direct.bytemask];
 	return memory_read_qword(space, byteaddress);
 }
 
--- ./src/emu/cpuexec.h.orig	2009-12-02 18:15:30.000000000 -0800
+++ ./src/emu/cpuexec.h	2009-12-02 18:15:34.000000000 -0800
@@ -321,7 +321,7 @@
 INLINE cpu_class_header *cpu_get_class_header(const device_config *device)
 {
 	if (device->token != NULL)
-		return (cpu_class_header *)((UINT8 *)device->token + device->tokenbytes) - 1;
+		return (cpu_class_header *)(int)((UINT8 *)device->token + device->tokenbytes) - 1;
 	return NULL;
 }
 
--- ./src/emu/mamecore.h.orig	2009-12-02 18:29:29.000000000 -0800
+++ ./src/emu/mamecore.h	2009-12-02 18:29:39.000000000 -0800
@@ -191,6 +191,7 @@
 #undef assert
 #undef assert_always
 
+#if 0
 #ifdef MAME_DEBUG
 #define assert(x)	do { if (!(x)) fatalerror("assert: %s:%d: %s", __FILE__, __LINE__, #x); } while (0)
 #define assert_always(x, msg) do { if (!(x)) fatalerror("Fatal error: %s\nCaused by assert: %s:%d: %s", msg, __FILE__, __LINE__, #x); } while (0)
@@ -198,6 +199,7 @@
 #define assert(x)	do { } while (0)
 #define assert_always(x, msg) do { if (!(x)) fatalerror("Fatal error: %s (%s:%d)", msg, __FILE__, __LINE__); } while (0)
 #endif
+#endif
 
 
 /* map mame_* helpers to core_* helpers */
--- ./src/osd/sdl/sdl.mak.orig	2009-12-02 14:58:38.000000000 -0800
+++ ./src/osd/sdl/sdl.mak	2009-12-02 18:10:07.000000000 -0800
@@ -31,7 +31,7 @@
 
 # uncomment next line to build without OpenGL support
 
-# NO_OPENGL = 1
+NO_OPENGL = 1
 
 # uncomment next line to build without X11 support
 
@@ -45,7 +45,7 @@
 # GL-dispatching. 
 # This option takes precedence over MESA_INSTALL_ROOT
 
-USE_DISPATCH_GL = 1
+#USE_DISPATCH_GL = 1
 
 # uncomment and change the next line to compile and link to specific
 # SDL library. This is currently only supported for unix!
@@ -59,6 +59,8 @@
 #SDL_INSTALL_ROOT = /usr/local/test
 endif
 
+SDL_INSTALL_ROOT = $(GCCSDK_INSTALL_ENV)
+
 ###########################################################################
 ##################   END USER-CONFIGURABLE OPTIONS   ######################
 ###########################################################################
@@ -154,6 +156,14 @@
 SYNC_IMPLEMENTATION = tc
 endif
 
+ifeq ($(TARGETOS),riscos)
+#DEFS += -DSDLMAME_UNIX -DSDLMAME_X11 -DNO_DEBUGGER 
+DEFS += -DNO_AFFINITY_NP
+TARGETOS = unix
+SYNC_IMPLEMENTATION = sdl
+CFLAGS += -I$(GCCSDK_INSTALL_ENV)/include
+endif
+
 ifeq ($(TARGETOS),unix)
 DEFS += -DSDLMAME_UNIX 
 ifndef NO_X11
@@ -288,10 +298,10 @@
 # the new debugger relies on GTK+ in addition to the base SDLMAME needs
 # Non-X11 builds can not use the debugger
 ifndef NO_X11
-OSDCOREOBJS += $(SDLOBJ)/debugwin.o $(SDLOBJ)/dview.o $(SDLOBJ)/debug-sup.o $(SDLOBJ)/debug-intf.o
-CFLAGS += `pkg-config --cflags gtk+-2.0` `pkg-config --cflags gconf-2.0` 
-LIBS += `pkg-config --libs gtk+-2.0` `pkg-config --libs gconf-2.0`
-CFLAGS += -DGTK_DISABLE_DEPRECATED
+#OSDCOREOBJS += $(SDLOBJ)/debugwin.o $(SDLOBJ)/dview.o $(SDLOBJ)/debug-sup.o $(SDLOBJ)/debug-intf.o
+#CFLAGS += `pkg-config --cflags gtk+-2.0` `pkg-config --cflags gconf-2.0` 
+#LIBS += `pkg-config --libs gtk+-2.0` `pkg-config --libs gconf-2.0`
+#CFLAGS += -DGTK_DISABLE_DEPRECATED
 else
 OSDCOREOBJS += $(SDLOBJ)/debugwin.o
 endif # NO_X11
--- ./src/osd/sdl/sdldir.c.orig	2009-12-02 15:02:41.000000000 -0800
+++ ./src/osd/sdl/sdldir.c	2009-12-02 15:05:54.000000000 -0800
@@ -43,7 +43,7 @@
 struct _osd_directory
 {
 	osd_directory_entry ent;
-#if defined(SDLMAME_DARWIN) || defined(SDLMAME_WIN32) || defined(SDLMAME_NO64BITIO) || defined(SDLMAME_BSD) || defined(SDLMAME_OS2)
+#if defined(SDLMAME_DARWIN) || defined(SDLMAME_WIN32) || defined(SDLMAME_NO64BITIO) || defined(SDLMAME_BSD) || defined(SDLMAME_OS2) || defined(SDLMAME_RISCOS)
 	struct dirent *data;
 #else
 	struct dirent64 *data;
@@ -171,7 +171,7 @@
 
 const osd_directory_entry *osd_readdir(osd_directory *dir)
 {
-	#if defined(SDLMAME_DARWIN) || defined(SDLMAME_WIN32) || defined(SDLMAME_NO64BITIO) || defined(SDLMAME_BSD) || defined(SDLMAME_OS2)
+	#if defined(SDLMAME_DARWIN) || defined(SDLMAME_WIN32) || defined(SDLMAME_NO64BITIO) || defined(SDLMAME_BSD) || defined(SDLMAME_OS2) || defined(SDLMAME_RISCOS)
 	dir->data = readdir(dir->fd);
 	#else
 	dir->data = readdir64(dir->fd);
--- ./src/osd/sdl/sdlprefix.h.orig	2009-12-02 15:04:25.000000000 -0800
+++ ./src/osd/sdl/sdlprefix.h	2009-12-02 15:31:27.000000000 -0800
@@ -32,6 +32,11 @@
 
 #elif defined(__FreeBSD__) || defined(__DragonFly__) || defined(__OpenBSD__)
 #define SDLMAME_BSD 1
+
+#elif defined(__riscos__)
+#define SDLMAME_RISCOS
+#define SDLMAME_NO64BITIO
+
 #endif
 
 // fix for Ubuntu 8.10
--- ./src/osd/sdl/sdlfile.c.orig	2009-12-02 15:06:17.000000000 -0800
+++ ./src/osd/sdl/sdlfile.c	2009-12-02 15:38:51.000000000 -0800
@@ -96,7 +96,7 @@
 	UINT32 access;
 	const char *src;
 	char *dst;
-        #if defined(SDLMAME_DARWIN) || defined(SDLMAME_WIN32) || defined(SDLMAME_NO64BITIO) || defined(SDLMAME_BSD) || defined(SDLMAME_OS2)
+        #if defined(SDLMAME_DARWIN) || defined(SDLMAME_WIN32) || defined(SDLMAME_NO64BITIO) || defined(SDLMAME_BSD) || defined(SDLMAME_OS2) || defined(SDLMAME_RISCOS)
 	struct stat st;
 	#else
 	struct stat64 st;
@@ -180,7 +180,7 @@
 	#endif
 
 	// attempt to open the file
-        #if defined(SDLMAME_DARWIN) || defined(SDLMAME_WIN32) || defined(SDLMAME_NO64BITIO) || defined(SDLMAME_BSD) || defined(SDLMAME_OS2)
+        #if defined(SDLMAME_DARWIN) || defined(SDLMAME_WIN32) || defined(SDLMAME_NO64BITIO) || defined(SDLMAME_BSD) || defined(SDLMAME_OS2) || defined(SDLMAME_RISCOS)
 	(*file)->handle = open(tmpstr, access, 0666);
 	#else
 	(*file)->handle = open64(tmpstr, access, 0666);
@@ -203,7 +203,7 @@
 				// attempt to reopen the file
 				if (error == NO_ERROR)
 				{
-		                        #if defined(SDLMAME_DARWIN) || defined(SDLMAME_WIN32) || defined(SDLMAME_NO64BITIO) || defined(SDLMAME_BSD) || defined(SDLMAME_OS2)
+		                        #if defined(SDLMAME_DARWIN) || defined(SDLMAME_WIN32) || defined(SDLMAME_NO64BITIO) || defined(SDLMAME_BSD) || defined(SDLMAME_OS2) || defined(SDLMAME_RISCOS)
 					(*file)->handle = open(tmpstr, access, 0666);
 					#else
 					(*file)->handle = open64(tmpstr, access, 0666);
@@ -223,7 +223,7 @@
 	}
 
 	// get the file size
-        #if defined(SDLMAME_DARWIN) || defined(SDLMAME_WIN32) || defined(SDLMAME_NO64BITIO) || defined(SDLMAME_BSD) || defined(SDLMAME_OS2)
+        #if defined(SDLMAME_DARWIN) || defined(SDLMAME_WIN32) || defined(SDLMAME_NO64BITIO) || defined(SDLMAME_BSD) || defined(SDLMAME_OS2) || defined(SDLMAME_RISCOS)
 	fstat((*file)->handle, &st);
 	#else
 	fstat64((*file)->handle, &st);
@@ -449,9 +449,10 @@
 file_error osd_copyfile(const char *destfile, const char *srcfile)
 {
 	char command[1024];
+	int unused;
 
 	sprintf(command, "cp %s %s\n", srcfile, destfile);
-	system(command);
+	unused = system(command); unused = unused;
 
 	return FILERR_NONE;
 }
--- ./src/osd/sdl/osinline.h.orig	2009-12-02 18:14:37.000000000 -0800
+++ ./src/osd/sdl/osinline.h	2009-12-02 18:14:45.000000000 -0800
@@ -86,4 +86,9 @@
 
 #endif
 
+#ifdef __arm__
+void osd_yield_processor(void) { sched_yield(); }
+#endif
+
+
 #endif /* __OSINLINE__ */
