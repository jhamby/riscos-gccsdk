--- common.mak.orig	2011-02-06 12:40:20.000000000 +0000
+++ common.mak	2011-02-06 12:40:54.000000000 +0000
@@ -85,7 +85,7 @@
 TESTPROGS += $(TESTPROGS-yes)
 
 FFEXTRALIBS := $(addprefix -l,$(addsuffix $(BUILDSUF),$(FFLIBS))) $(EXTRALIBS)
-FFLDFLAGS   := $(addprefix -L$(BUILD_ROOT)/lib,$(ALLFFLIBS)) $(LDFLAGS)
+FFLDFLAGS   := $(addprefix -L$(BUILD_ROOT)/lib,$(ALLFFLIBS)) -static $(LDFLAGS)
 
 EXAMPLES  := $(addprefix $(SUBDIR),$(addsuffix -example$(EXESUF),$(EXAMPLES)))
 OBJS      := $(addprefix $(SUBDIR),$(sort $(OBJS)))
