--- gcc/mklibgcc.in.orig	2007-04-06 19:32:49.000000000 +0100
+++ gcc/mklibgcc.in	2007-04-07 11:37:28.000000000 +0100
@@ -170,6 +170,15 @@
   libunwind_a=
   libunwind_so=
 
+  # Avoid -mmodule/-fPIC combination. This doesn't stop the build, it just stops
+  # -fPIC being added to the command line. We end up with a DSO that wasn't
+  # compiled with -fPIC, but it's not a library that will ever be used anyway.
+  if echo $flags | grep -q -- "-mmodule" ; then
+    fpic=
+  else
+    fpic=-fPIC
+  fi
+
   if [ "$LIBUNWIND" ]; then
     libunwind_a=$dir/libunwind.a
   fi
@@ -220,7 +229,7 @@
       outV="libgcc/${dir}/${name}.vis"
 
       echo ${outS}: stmp-dirs '$(srcdir)/config/$(LIB1ASMSRC)'
-      echo "	$gcc_s_compile" $flags -DL$name -xassembler-with-cpp \
+      echo "	$gcc_s_compile" $flags $fpic -DL$name -xassembler-with-cpp \
 	  -c '$(srcdir)/config/$(LIB1ASMSRC)' -o $outS
 
       echo ${out}: stmp-dirs '$(srcdir)/config/$(LIB1ASMSRC)' ${outV}
@@ -253,7 +262,7 @@
       outS="libgcc/${dir}/${name}_s${objext}"
 
       echo $outS: $libgcc2_c_dep
-      echo "	$gcc_s_compile" $flags -DL$name -c '$(srcdir)/libgcc2.c' \
+      echo "	$gcc_s_compile" $flags $fpic -DL$name -c '$(srcdir)/libgcc2.c' \
 	-o $outS
 
       echo $out: $libgcc2_c_dep
@@ -288,7 +297,7 @@
       outS="libgcc/${dir}/${name}_s${objext}"
 
       echo $outS: $libgcc2_c_dep
-      echo "	$gcc_s_compile" $flags -DL$name \
+      echo "	$gcc_s_compile" $flags $fpic -DL$name \
         -fexceptions -fnon-call-exceptions -c '$(srcdir)/libgcc2.c' -o $outS
 
       echo $out: $libgcc2_c_dep
@@ -321,7 +330,7 @@
 	outS="libgcc/${dir}/${name}_s${objext}"
 
 	echo $outS: $FPBIT $fpbit_c_dep
-	echo "	$gcc_s_compile" -DFINE_GRAINED_LIBRARIES $flags -DL$name \
+	echo "	$gcc_s_compile" -DFINE_GRAINED_LIBRARIES $flags $fpic -DL$name \
 	  -c $FPBIT -o $outS
 
         echo $out: $FPBIT $fpbit_c_dep
@@ -352,7 +361,7 @@
 	outS="libgcc/${dir}/${name}_s${objext}"
 
 	echo $outS: $DPBIT $fpbit_c_dep
-	echo "	$gcc_s_compile" -DFINE_GRAINED_LIBRARIES $flags -DL$name \
+	echo "	$gcc_s_compile" -DFINE_GRAINED_LIBRARIES $flags $fpic -DL$name \
 	  -c $DPBIT -o $outS
 
         echo $out: $DPBIT $fpbit_c_dep
@@ -382,7 +391,7 @@
 	outS="libgcc/${dir}/${name}_s${objext}"
 
 	echo $outS: $TPBIT $fpbit_c_dep
-	echo "	$gcc_s_compile" -DFINE_GRAINED_LIBRARIES $flags -DL$name \
+	echo "	$gcc_s_compile" -DFINE_GRAINED_LIBRARIES $flags $fpic -DL$name \
 	  -c $TPBIT -o $outS
 
         echo $out: $TPBIT $fpbit_c_dep
@@ -416,7 +425,7 @@
       case $file in
 	*.c)
 	  echo $outS: stmp-dirs $file $libgcc_dep
-	  echo "	$gcc_s_compile" $flags -c $file -o $outS
+	  echo "	$gcc_s_compile" $flags $fpic -c $file -o $outS
 
 	  echo $out: stmp-dirs $file $libgcc_dep
 	  echo "	$gcc_compile" $flags '$(vis_hide)' -c $file -o $out
@@ -426,7 +435,7 @@
 	  outV="libgcc/${dir}/${oname}.vis"
 
 	  echo $outS: stmp-dirs $file $libgcc_dep
-	  echo "	$gcc_s_compile" $flags -xassembler-with-cpp \
+	  echo "	$gcc_s_compile" $flags $fpic -xassembler-with-cpp \
 	         -c $file -o $outS
 
 	  echo $out: stmp-dirs $file $libgcc_dep $outV
@@ -552,11 +561,11 @@
       case $file in
         *.c)
 	  echo $outS: stmp-dirs $file $LIB2ADDEHDEP $libgcc_dep
-	  echo "	$gcc_s_compile" $flags -fexceptions -c $file -o $outS
+	  echo "	$gcc_s_compile" $flags $fpic -fexceptions -c $file -o $outS
 	  ;;
 	*.asm | *.S)
 	  echo $outS: stmp-dirs $file $LIB2ADDEHDEP $libgcc_dep
-	  echo "	$gcc_s_compile" $flags -xassembler-with-cpp -c $file -o $outS
+	  echo "	$gcc_s_compile" $flags $fpic -xassembler-with-cpp -c $file -o $outS
 	  ;;
 	*)   echo "Unhandled extension: $file">&2; exit 1 ;;
       esac
@@ -622,7 +631,7 @@
 	echo "	$gcc_compile $flags -fexceptions \$(vis_hide) -c $file -o $out"
 
 	echo $outS: stmp-dirs $file $LIBUNWINDDEP
-	echo "	$gcc_s_compile $flags -fexceptions -c $file -o $outS"
+	echo "	$gcc_s_compile $flags $fpic -fexceptions -c $file -o $outS"
 
 	echo $libunwind_a: $out
 	echo $libunwind_so: $outS
