--- !DLSources/Makefile.unix.orig	2023-08-11 17:19:47.563754423 -0700
+++ !DLSources/Makefile.unix	2023-08-11 17:23:47.995259402 -0700
@@ -25,7 +25,7 @@
 
 DESKLIB_INCLUDE=$(realpath ../!DeskLib/include/)
 DESKLIB_OLD_INCLUDE=$(realpath ../!DeskLib/oldinclude/)
-DESKLIB_PATH=$(DESKLIB_INCLUDE)/,$(DESKLIB_OLD_INCLUDE)
+DESKLIB_INCLUDES = -I$(DESKLIB_INCLUDE) -I$(DESKLIB_OLD_INCLUDE)
 
 libraries := $(notdir $(filter-out Libraries/.svn Libraries/h \
                Libraries/Macros.h Libraries/RegDefs.h, $(wildcard Libraries/*)))
@@ -40,13 +40,13 @@
            -Winline -Wno-unused -Wno-long-long -W -Wcast-qual -Wshadow -Werror
 
 # We do ELF based builds only
-CC = $(GCCSDK_INSTALL_CROSSBIN)/arm-unknown-riscos-gcc
-CFLAGS = -O2 $(WARNINGS) -std=c99 $(SCLFLAGS)
+#CC = $(GCCSDK_INSTALL_CROSSBIN)/arm-unknown-riscos-gcc
+CFLAGS = -O2 $(WARNINGS) -std=gnu99 -D_GNU_SOURCE $(SCLFLAGS)
 
 AS = $(CC)
 ASFLAGS = -Wa,-ILibraries,-I$(@D) -c $(SCLFLAGS)
 
-AR = $(GCCSDK_INSTALL_CROSSBIN)/arm-unknown-riscos-ar
+#AR = $(GCCSDK_INSTALL_CROSSBIN)/arm-unknown-riscos-ar
 ARFLAGS = rcv
 
 # Make the library
@@ -66,13 +66,13 @@
 	-rm -f $(DESKLIB)
 
 %.o: %.c
-	DESKLIB_PATH=$(DESKLIB_PATH) $(CC) $(CFLAGS) -c $< -o $@
+	$(CC) $(CFLAGS) $(DESKLIB_INCLUDES) -c $< -o $@
 
 %.o: %.s
 	$(AS) $(ASFLAGS) $< -o $@
 
 %.os: %.c
-	DESKLIB_PATH=$(DESKLIB_PATH) $(CC) $(CFLAGS) -c $< -o $@ -fPIC
+	$(CC) $(CFLAGS) $(DESKLIB_INCLUDES) -c $< -o $@ -fPIC
 
 %.os: %.s
 	$(AS) $(ASFLAGS) $< -o $@ -fPIC
