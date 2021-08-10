--- lib/Basic/Targets.cpp.orig	2021-06-28 09:23:38.000000000 -0700
+++ lib/Basic/Targets.cpp	2021-08-09 17:13:31.990952482 -0700
@@ -190,6 +190,8 @@
       return new NetBSDTargetInfo<ARMleTargetInfo>(Triple, Opts);
     case llvm::Triple::OpenBSD:
       return new OpenBSDTargetInfo<ARMleTargetInfo>(Triple, Opts);
+    case llvm::Triple::RISCOS:
+      return new RISCOSTargetInfo<ARMleTargetInfo>(Triple, Opts);
     case llvm::Triple::RTEMS:
       return new RTEMSTargetInfo<ARMleTargetInfo>(Triple, Opts);
     case llvm::Triple::NaCl:
