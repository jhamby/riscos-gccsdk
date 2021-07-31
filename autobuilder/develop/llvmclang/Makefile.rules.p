--- Makefile.rules.orig	2014-06-20 20:00:41.000000000 +0100
+++ Makefile.rules	2015-01-24 19:29:41.588318009 +0000
@@ -282,7 +282,7 @@
 ifeq ($(ENABLE_OPTIMIZED),1)
   BuildMode := Release
   # Don't use -fomit-frame-pointer on Darwin or FreeBSD.
-  ifneq ($(HOST_OS), $(filter $(HOST_OS), Cygwin Darwin DragonFly FreeBSD GNU/kFreeBSD))
+  ifneq ($(HOST_OS), $(filter $(HOST_OS), Cygwin Darwin DragonFly FreeBSD GNU/kFreeBSD RISCOS))
     OmitFramePointer := -fomit-frame-pointer
   endif
 
@@ -418,9 +418,18 @@
       CXX.Flags += -fno-common
       C.Flags   += -fno-common
     else
-      # Linux and others; pass -fPIC
-      CXX.Flags += -fPIC
-      C.Flags   += -fPIC
+      ifeq ($(HOST_OS),RISCOS)
+        ifdef LIBRARYNAME
+          ifeq (,$(findstring clang,$(LIBRARYNAME)))
+            CXX.Flags += -fPIC
+            C.Flags   += -fPIC
+          endif
+        endif
+      else
+        # Linux and others; pass -fPIC
+        CXX.Flags += -fPIC
+        C.Flags   += -fPIC
+      endif
     endif
   endif
 else
@@ -1356,7 +1365,7 @@
 endif
 endif
 
-ifeq ($(HOST_OS), $(filter $(HOST_OS), DragonFly Linux NetBSD FreeBSD GNU/kFreeBSD GNU))
+ifeq ($(HOST_OS), $(filter $(HOST_OS), DragonFly Linux NetBSD FreeBSD GNU/kFreeBSD GNU RISCOS))
 ifneq ($(ARCH), Mips)
   LD.Flags += -Wl,--version-script=$(LLVM_SRC_ROOT)/autoconf/ExportMap.map
 endif
@@ -1434,6 +1443,13 @@
 	$(LLVMLibsOptions) $(ExtraLibs) $(TOOLLINKOPTSB) $(LIBS)
 	$(Echo) ======= Finished Linking $(BuildMode) Executable $(TOOLNAME) \
           $(StripWarnMsg)
+ifeq ($(TOOLNAME),clang)
+ifeq ($(LLVM_CROSS_COMPILING),0)
+	cp -T $@ $(GCCSDK_INSTALL_CROSSBIN)/arm-unknown-riscos-clang
+	ln -T -s -f $(GCCSDK_INSTALL_CROSSBIN)/arm-unknown-riscos-clang \
+		    $(GCCSDK_INSTALL_CROSSBIN)/arm-unknown-riscos-clang++
+endif
+endif
 endif
 
 ifneq ($(strip $(ToolAliasBuildPath)),)
