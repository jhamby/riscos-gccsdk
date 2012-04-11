--- makefile.orig	2012-04-09 17:01:41.000000000 +0100
+++ makefile	2012-04-09 17:02:56.000000000 +0100
@@ -24,7 +24,7 @@
 #-------------------------------------------------
 
 ifndef TARGET
-TARGET = mame
+TARGET = tiny
 endif
 
 ifndef SUBTARGET
@@ -60,6 +60,8 @@
 # win32, unix, macosx, os2
 #-------------------------------------------------
 
+TARGETOS = unix
+
 ifndef TARGETOS
 
 ifeq ($(OS),Windows_NT)
@@ -234,6 +236,8 @@
 # (default is OPTIMIZE = 3 normally, or OPTIMIZE = 0 with symbols)
 # OPTIMIZE = 3
 
+NOASM = 1
+NOWERROR = 1
 
 ###########################################################################
 ##################   END USER-CONFIGURABLE OPTIONS   ######################
@@ -467,7 +471,7 @@
 ifndef NOWERROR
 CCOMFLAGS += -Werror -fno-strict-aliasing $(ARCHOPTS)
 else
-CCOMFLAGS += -fno-strict-aliasing $(ARCHOPTS)
+CCOMFLAGS += -fpermissive -fno-strict-aliasing $(ARCHOPTS)
 endif
 else
 CCOMFLAGS += -fno-strict-aliasing $(ARCHOPTS)
@@ -507,6 +511,7 @@
 INCPATH += \
 	-I$(SRC)/$(TARGET) \
 	-I$(OBJ)/$(TARGET)/layout \
+	-I$(OBJ)/mame/layout \
 	-I$(SRC)/emu \
 	-I$(OBJ)/emu \
 	-I$(OBJ)/emu/layout \
@@ -770,12 +770,12 @@
 
 $(OBJ)/%.lh: $(SRC)/%.lay $(FILE2STR_TARGET)
 	@echo Converting $<...
-	@$(FILE2STR) $< $@ layout_$(basename $(notdir $<))
+	@./bin/mame-file2str $< $@ layout_$(basename $(notdir $<))
 
 $(OBJ)/%.fh: $(SRC)/%.png $(PNG2BDC_TARGET) $(FILE2STR_TARGET)
 	@echo Converting $<...
-	@$(PNG2BDC) $< $(OBJ)/temp.bdc
-	@$(FILE2STR) $(OBJ)/temp.bdc $@ font_$(basename $(notdir $<)) UINT8
+	@./bin/mame-png2bdc $< $(OBJ)/temp.bdc
+	@./bin/mame-file2str $(OBJ)/temp.bdc $@ font_$(basename $(notdir $<)) UINT8
 
 $(DRIVLISTOBJ): $(DRIVLISTSRC)
 	@echo Compiling $<...
@@ -787,11 +787,11 @@
 
 $(DRIVLISTSRC): $(SRC)/$(TARGET)/$(SUBTARGET).lst $(MAKELIST_TARGET)
 	@echo Building driver list $<...
-	@$(MAKELIST) $< >$@
+	@./bin/mame-makelist $< >$@
 
 $(DEVLISTSRC): $(SRC)/$(TARGET)/$(SUBTARGET)_dev.lst $(MAKEDEV_TARGET)
 	@echo Building device list $<...
-	@$(MAKEDEV) $< >$@
+	@./bin/mame-makedev $< >$@
 
 $(OBJ)/%.a:
 	@echo Archiving $@...
