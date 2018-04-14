--- unix/tfconfig-old	2018-04-14 18:05:37.000000000 +0100
+++ unix/tfconfig	2018-04-14 18:06:04.000000000 +0100
@@ -35,7 +35,7 @@
 
 ### Initialize variables, just in case they're already defined.
 
-AOUT=tfconfig.out
+AOUT=tfconfig.out,ff8
 BINDIR=''
 # CC=''		;# Use the value from the environment if there is one.
 CCFLAGS=${TFDEV_CCFLAGS-'-O'}
@@ -61,7 +61,7 @@
 TERMINAL=''
 TFVER="$*"
 TTYDRIVER=''
-X=''
+X=',ff8'
 
 if [ -z "$USER" ]; then USER=$LOGNAME; fi
 export USER
@@ -225,6 +225,7 @@
 echo 'To change these locations type "n" now and edit the unix/Config file.'
 while [ -z "$ans" ]; do
     echo 'Continue? (y/n)'
+    break
     read ans;
     case "$ans" in
     y|Y)  break ;;
@@ -632,13 +633,13 @@
     return !(toupper('a') == 'A' && toupper('A') == 'A' && toupper('@') == '@');
 }
 EOF
-if ${CC} ${CCFLAGS} test.c -o ${AOUT}>/dev/null 2>&1 && ./${AOUT}; then
+#if ${CC} ${CCFLAGS} test.c -o ${AOUT}>/dev/null 2>&1 && ./${AOUT}; then
     echo "Case conversion works."
     echo '#define CASE_OK' >&4
-else
-    echo "Case conversion is broken, I'll have to use my own."
-    echo '#undef  CASE_OK' >&4
-fi
+#else
+#    echo "Case conversion is broken, I'll have to use my own."
+#    echo '#undef  CASE_OK' >&4
+#fi
 
 
 ### Figure out return type of signal handlers.
