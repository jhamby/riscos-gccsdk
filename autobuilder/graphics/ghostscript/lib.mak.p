--- base/lib.mak.orig	2012-09-25 13:18:30.000000000 +0100
+++ base/lib.mak	2012-09-25 10:45:19.000000000 +0100
@@ -2684,7 +2684,7 @@
 	$(SETMOD) $(GLD)romfs0

 $(GLGEN)gsromfs1.c : $(MKROMFS_XE) $(PS_ROMFS_DEPS)
-	$(EXP)$(MKROMFS_XE) -o $(GLGEN)gsromfs1.c \
+	$(EXP)obj/host-mkromfs -o $(GLGEN)gsromfs1.c \
 	-X .svn -X CVS $(UFST_ROMFS_ARGS) \
 	$(PCLXL_ROMFS_ARGS) $(PJL_ROMFS_ARGS) $(XPS_ROMFS_ARGS) \
 	$(PS_ROMFS_ARGS)
