--- lib/Support/Triple.cpp.orig	2021-08-09 20:36:36.132346100 -0700
+++ lib/Support/Triple.cpp	2021-08-09 20:37:05.328596222 -0700
@@ -213,6 +213,7 @@
   case NetBSD: return "netbsd";
   case OpenBSD: return "openbsd";
   case PS4: return "ps4";
+  case RISCOS: return "riscos";
   case RTEMS: return "rtems";
   case Solaris: return "solaris";
   case TvOS: return "tvos";
@@ -531,6 +532,7 @@
     .StartsWith("hurd", Triple::Hurd)
     .StartsWith("wasi", Triple::WASI)
     .StartsWith("emscripten", Triple::Emscripten)
+    .StartsWith("riscos", Triple::RISCOS)
     .Default(Triple::UnknownOS);
 }
 
