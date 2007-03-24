--- config-ml.in.orig	2007-03-24 16:53:23.000000000 +0100
+++ config-ml.in	2007-03-24 16:43:07.000000000 +0100
@@ -233,6 +233,36 @@
 	fi
 	;;
 arm-*-*)
+	# Not all libraries have runtime support for both UnixLib and SCL.
+	# So filter out those combinations which are expected not to work.
+	case "x`basename $ml_realsrcdir`" in
+	  xlibunixlib | xlibiberty | xlibstdc++-v3)
+	    # No SCL-runtime support for those libraries:
+	    old_multidirs=${multidirs}
+	    multidirs=""
+	    for x in ${old_multidirs}; do
+	      case "${x}" in
+	        *scl/fpu*) : ;;
+	        *) multidirs="${multidirs} ${x}" ;;
+	      esac
+	    done
+	    ;;
+	  xlibscl)
+	    # No UnixLib-runtime support for this:
+	    # Note that we can't filter out the default multilib configuration
+	    # which is soft-float & UnixLib.
+	    old_multidirs=${multidirs}
+	    multidirs=""
+	    for x in ${old_multidirs}; do
+	      case "${x}" in
+	        *scl/fpu*) multidirs="${multidirs} ${x}" ;;
+	        *) : ;;
+	      esac
+	    done
+	    ;;
+	  *) : ;;
+	esac
+
 	if [ x"$enable_fpu" = xno ]
 	then
 	  old_multidirs=${multidirs}
@@ -532,6 +562,10 @@
 
 PWD_COMMAND=$${PWDCMD-pwd}
 
+LIBSCL_FILTERED_FLAGS_TO_PASS=$(subst /libunixlib/,/libscl/,$(FLAGS_TO_PASS))
+LIBSCL_FILTERED_CC=$(subst /libunixlib/,/libscl/,$(CC))
+LIBSCL_FILTERED_CXX=$(subst /libunixlib/,/libscl/,$(CXX))
+
 # FIXME: There should be an @-sign in front of the `if'.
 # Leave out until this is tested a bit more.
 multi-do:
@@ -549,7 +583,10 @@
 	    else \
 	      if [ -d ../$${dir}/$${lib} ]; then \
 		flags=`echo $$i | sed -e 's/^[^;]*;//' -e 's/@/ -/g'`; \
-		if (cd ../$${dir}/$${lib}; $(MAKE) $(FLAGS_TO_PASS) \
+		if echo $${dir} | grep "scl/fpu" > /dev/null ; then \
+		  CC='$(LIBSCL_FILTERED_CC)'; \
+		  CXX='$(LIBSCL_FILTERED_CXX)'; \
+		  if (cd ../$${dir}/$${lib}; $(MAKE) $(LIBSCL_FILTERED_FLAGS_TO_PASS) \
 				CFLAGS="$(CFLAGS) $${flags}" \
 				FCFLAGS="$(FCFLAGS) $${flags}" \
 				FFLAGS="$(FFLAGS) $${flags}" \
@@ -568,9 +605,34 @@
 				INSTALL_PROGRAM="$(INSTALL_PROGRAM)" \
 				INSTALL_SCRIPT="$(INSTALL_SCRIPT)" \
 				$(DO)); then \
-		  true; \
+		    true; \
+		  else \
+		    exit 1; \
+		  fi; \
 		else \
-		  exit 1; \
+		  if (cd ../$${dir}/$${lib}; $(MAKE) $(FLAGS_TO_PASS) \
+				CFLAGS="$(CFLAGS) $${flags}" \
+				FCFLAGS="$(FCFLAGS) $${flags}" \
+				FFLAGS="$(FFLAGS) $${flags}" \
+				ADAFLAGS="$(ADAFLAGS) $${flags}" \
+				prefix="$(prefix)" \
+				exec_prefix="$(exec_prefix)" \
+				GCJFLAGS="$(GCJFLAGS) $${flags}" \
+				CXXFLAGS="$(CXXFLAGS) $${flags}" \
+				LIBCFLAGS="$(LIBCFLAGS) $${flags}" \
+				LIBCXXFLAGS="$(LIBCXXFLAGS) $${flags}" \
+				LDFLAGS="$(LDFLAGS) $${flags}" \
+				MULTIFLAGS="$${flags}" \
+				DESTDIR="$(DESTDIR)" \
+				INSTALL="$(INSTALL)" \
+				INSTALL_DATA="$(INSTALL_DATA)" \
+				INSTALL_PROGRAM="$(INSTALL_PROGRAM)" \
+				INSTALL_SCRIPT="$(INSTALL_SCRIPT)" \
+				$(DO)); then \
+		    true; \
+		  else \
+		    exit 1; \
+		  fi; \
 		fi; \
 	      else true; \
 	      fi; \
@@ -799,6 +861,9 @@
 	popdir_rx=`echo ${ML_POPDIR} | sed 's,.,.,g'`
 	CC_=
 	for arg in ${CC}; do
+	  if echo ${ml_dir} | grep "scl/fpu" > /dev/null ; then
+	    arg=`echo "${arg}" | sed -e s,/libunixlib/,/libscl/,g`
+	  fi
 	  case $arg in
 	  -[BIL]"${ML_POPDIR}"/*)
 	    CC_="${CC_}"`echo "X${arg}" | sed -n "s/X\\(-[BIL]${popdir_rx}\\).*/\\1/p"`/${ml_dir}`echo "X${arg}" | sed -n "s/X-[BIL]${popdir_rx}\\(.*\\)/\1/p"`' ' ;;
@@ -811,6 +876,9 @@
 
 	CXX_=
 	for arg in ${CXX}; do
+	  if echo ${ml_dir} | grep "scl/fpu" > /dev/null ; then
+	    arg=`echo "${arg}" | sed -e s,/libunixlib/,/libscl/,g`
+	  fi
 	  case $arg in
 	  -[BIL]"${ML_POPDIR}"/*)
 	    CXX_="${CXX_}"`echo "X${arg}" | sed -n "s/X\\(-[BIL]${popdir_rx}\\).*/\\1/p"`/${ml_dir}`echo "X${arg}" | sed -n "s/X-[BIL]${popdir_rx}\\(.*\\)/\\1/p"`' ' ;;
