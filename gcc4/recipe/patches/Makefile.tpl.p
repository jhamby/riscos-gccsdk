--- Makefile.tpl.orig	2007-07-09 03:04:22.000000000 +0200
+++ Makefile.tpl	2007-07-09 02:26:26.000000000 +0200
@@ -161,6 +161,7 @@
 	GCC_FOR_TARGET="$(GCC_FOR_TARGET)"; export GCC_FOR_TARGET; \
 	LD_FOR_TARGET="$(LD_FOR_TARGET)"; export LD_FOR_TARGET; \
 	NM_FOR_TARGET="$(NM_FOR_TARGET)"; export NM_FOR_TARGET; \
+	OBJCOPY_FOR_TARGET="$(OBJCOPY_FOR_TARGET)"; export OBJCOPY_FOR_TARGET; \
 	OBJDUMP_FOR_TARGET="$(OBJDUMP_FOR_TARGET)"; export OBJDUMP_FOR_TARGET; \
 	RANLIB_FOR_TARGET="$(RANLIB_FOR_TARGET)"; export RANLIB_FOR_TARGET; \
 	TOPLEVEL_CONFIGURE_ARGUMENTS="$(TOPLEVEL_CONFIGURE_ARGUMENTS)"; export TOPLEVEL_CONFIGURE_ARGUMENTS; \
@@ -207,6 +208,7 @@
 	LDFLAGS="$(LDFLAGS_FOR_TARGET)"; export LDFLAGS; \
 	LIPO="$(LIPO_FOR_TARGET)"; export LIPO; \
 	NM="$(COMPILER_NM_FOR_TARGET)"; export NM; \
+	OBJCOPY="$(OBJCOPY_FOR_TARGET)"; export OBJCOPY; \
 	OBJDUMP="$(OBJDUMP_FOR_TARGET)"; export OBJDUMP; \
 	RANLIB="$(RANLIB_FOR_TARGET)"; export RANLIB; \
 	STRIP="$(STRIP_FOR_TARGET)"; export STRIP; \
@@ -319,6 +321,7 @@
 
 LIPO_FOR_TARGET=@LIPO_FOR_TARGET@
 NM_FOR_TARGET=@NM_FOR_TARGET@
+OBJCOPY_FOR_TARGET=@OBJCOPY_FOR_TARGET@
 OBJDUMP_FOR_TARGET=@OBJDUMP_FOR_TARGET@
 RANLIB_FOR_TARGET=@RANLIB_FOR_TARGET@
 STRIP_FOR_TARGET=@STRIP_FOR_TARGET@
@@ -441,6 +444,7 @@
 	'LIBCFLAGS=$$(LIBCFLAGS_FOR_TARGET)' \
 	'LIBCXXFLAGS=$$(LIBCXXFLAGS_FOR_TARGET)' \
 	'NM=$(COMPILER_NM_FOR_TARGET)' \
+	'OBJCOPY=$$(OBJCOPY_FOR_TARGET)' \
 	'OBJDUMP=$$(OBJDUMP_FOR_TARGET)' \
 	'RANLIB=$$(RANLIB_FOR_TARGET)' \
 	'WINDRES=$$(WINDRES_FOR_TARGET)'
@@ -1407,7 +1411,7 @@
 [+ ENDFOR target_modules +]
 
 [+ FOR lang_env_dependencies +]
-configure-target-[+module+]: maybe-all-target-newlib maybe-all-target-libgloss
+configure-target-[+module+]: maybe-all-target-libunixlib maybe-all-target-libscl maybe-all-target-newlib maybe-all-target-libgloss
 [+ IF cxx +]configure-target-[+module+]: maybe-all-target-libstdc++-v3
 [+ ENDIF cxx +][+ ENDFOR lang_env_dependencies +]
 
