--- src/emu/cpu/cpu.mak.orig	2012-04-09 18:37:56.000000000 +0100
+++ src/emu/cpu/cpu.mak	2012-04-09 18:39:32.000000000 +0100
@@ -1124,7 +1124,7 @@
 # rule to generate the C files
 $(CPUOBJ)/m68000/m68kops.c: $(M68KMAKE) $(CPUSRC)/m68000/m68k_in.c
 	@echo Generating M68K source files...
-	$(M68KMAKE) $(CPUOBJ)/m68000 $(CPUSRC)/m68000/m68k_in.c
+	bin/mame-m68kmake $(CPUOBJ)/m68000 $(CPUSRC)/m68000/m68k_in.c
 
 # rule to build the generator
 ifneq ($(CROSS_BUILD),1)
@@ -1724,7 +1724,7 @@
 # rule to generate the C file
 $(CPUOBJ)/tms57002/tms57002.inc: $(TMSMAKE) $(CPUSRC)/tms57002/tmsinstr.lst
 	@echo Generating TMS57002 source file...
-	$(TMSMAKE) $(CPUSRC)/tms57002/tmsinstr.lst $@
+	bin/mame-tmsmake $(CPUSRC)/tms57002/tmsinstr.lst $@
 
 # rule to build the generator
 ifneq ($(CROSS_BUILD),1)
