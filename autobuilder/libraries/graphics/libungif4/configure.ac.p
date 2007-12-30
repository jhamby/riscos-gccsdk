--- configure.ac.orig	2007-12-30 09:31:19.000000000 -0800
+++ configure.ac	2007-12-30 09:31:35.000000000 -0800
@@ -18,11 +18,11 @@
 dnl Checks for libraries.
 
     AC_CHECK_LIB(m, pow, [MATH_LIB="${MATH_LIB} -lm"
-                 COMPILABLE_EXTRAS="${COMPILABLE_EXTRAS} gifclrmp gifrotat"] )
+                 COMPILABLE_EXTRAS="${COMPILABLE_EXTRAS} gifclrmp${ac_exeext} gifrotat${ac_exeext}"] )
 
 if test ${MATH_LIB}; then
     AC_CHECK_LIB(rle, rle_hdr_init, [RLE_LIB="${RLE_LIBS} -lrle -lm"
-                 COMPILABLE_EXTRAS="${COMPILABLE_EXTRAS} gif2rle rle2gif"],
+                 COMPILABLE_EXTRAS="${COMPILABLE_EXTRAS} gif2rle${ac_exeext} rle2gif${ac_exeext}"],
                  ,"-lm" )
 fi
 
@@ -30,11 +30,11 @@
 dnl check that it works?
     AC_CHECK_LIB(gl_s, main, [GL_S_LIB="${GL_S_LIB} -lgl_s $X_LIBS"
                              AC_DEFINE(HAVE_LIBGL_S, [1], [Define if the gl_s library is installed (SGI GL library)])
-                 COMPILABLE_EXTRAS="${COMPILABLE_EXTRAS} gif2iris"],, $X_LIBS)
+                 COMPILABLE_EXTRAS="${COMPILABLE_EXTRAS} gif2iris${ac_exeext}"],, $X_LIBS)
 
     AC_CHECK_LIB(X11, main, [X11_LIB="${X11_LIB} ${X_PRE_LIBS} $X_LIBS ${X_EXTRA_LIBS} -lX11"
                              AC_DEFINE(HAVE_LIBX11, [1], [Define if the X11 library is installed])
-                 COMPILABLE_EXTRAS="${COMPILABLE_EXTRAS} gif2x11"],, $X_LIBS)
+                 COMPILABLE_EXTRAS="${COMPILABLE_EXTRAS} gif2x11${ac_exeext}"],, $X_LIBS)
     DEVS="${GL_S_LIB} ${X11_LIB}" 
     AC_SUBST(DEVS)
     AC_SUBST(COMPILABLE_EXTRAS)
