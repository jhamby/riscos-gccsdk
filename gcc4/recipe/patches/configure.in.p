--- configure.in.orig	2007-01-07 19:12:25.000000000 +0100
+++ configure.in	2007-01-07 18:02:31.000000000 +0100
@@ -142,7 +142,9 @@
 # these libraries are built for the target environment, and are built after
 # the host libraries and the host tools (which may be a cross compiler)
 #
-target_libraries="target-libiberty \
+target_libraries="target-libunixlib \
+		target-libscl \
+		target-libiberty \
 		target-libgloss \
 		target-newlib \
 		target-libstdc++-v3 \
@@ -817,6 +819,14 @@
     ;;
 esac
 
+case "${target}" in
+  arm-riscos*-* | arm-*-riscos)
+    ;;
+  *)
+    noconfigdirs="$noconfigdirs target-libunixlib target-libscl"
+    ;;
+esac
+
 # If we aren't building newlib, then don't build libgloss, since libgloss
 # depends upon some newlib header files.
 case "${noconfigdirs}" in
@@ -2028,6 +2038,10 @@
    ;;
   esac
   ;;
+
+ *" libunixlib "*)
+  FLAGS_FOR_TARGET=$FLAGS_FOR_TARGET' -B$$r/$(TARGET_SUBDIR)/libunixlib/ -B$$r/$(TARGET_SUBDIR)/libunixlib/.libs/ -isystem $$s/libunixlib/include'
+  ;;
 esac
 
 # Allow the user to override the flags for
@@ -2184,6 +2198,7 @@
 NCN_STRICT_CHECK_TARGET_TOOLS(LD_FOR_TARGET, ld)
 NCN_STRICT_CHECK_TARGET_TOOLS(LIPO_FOR_TARGET, lipo)
 NCN_STRICT_CHECK_TARGET_TOOLS(NM_FOR_TARGET, nm)
+NCN_STRICT_CHECK_TARGET_TOOLS(OBJCOPY_FOR_TARGET, objcopy)
 NCN_STRICT_CHECK_TARGET_TOOLS(OBJDUMP_FOR_TARGET, objdump)
 NCN_STRICT_CHECK_TARGET_TOOLS(RANLIB_FOR_TARGET, ranlib, :)
 NCN_STRICT_CHECK_TARGET_TOOLS(STRIP_FOR_TARGET, strip)
@@ -2209,6 +2224,7 @@
 GCC_TARGET_TOOL(ld, LD_FOR_TARGET, LD, [ld/ld-new])
 GCC_TARGET_TOOL(lipo, LIPO_FOR_TARGET, LIPO)
 GCC_TARGET_TOOL(nm, NM_FOR_TARGET, NM, [binutils/nm-new])
+GCC_TARGET_TOOL(objcopy, OBJCOPY_FOR_TARGET, OBJCOPY, [binutils/objcopy])
 GCC_TARGET_TOOL(objdump, OBJDUMP_FOR_TARGET, OBJDUMP, [binutils/objdump])
 GCC_TARGET_TOOL(ranlib, RANLIB_FOR_TARGET, RANLIB, [binutils/ranlib])
 GCC_TARGET_TOOL(strip, STRIP_FOR_TARGET, STRIP, [binutils/strip])
