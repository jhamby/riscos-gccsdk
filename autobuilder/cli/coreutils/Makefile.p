--- man/Makefile.in.orig	2009-05-07 05:01:40.000000000 -0700
+++ man/Makefile.in	2009-07-25 08:59:51.000000000 -0700
@@ -1278,18 +1278,18 @@
 	      'or inadequate' 1>&2 \
 	    ;;							\
 	  *)							\
-	    rm -f $@						\
-	    && { echo "Updating man page $@";			\
-		 rm -rf $t;					\
-		 mkdir $t;					\
-		 (cd $t && $(LN_S) ../../src/$(mapped_name) $*); \
-		    $(PERL) -- $(srcdir)/help2man		\
-		     --source='$(PACKAGE_STRING)'		\
-		     --include=$(srcdir)/$*.x			\
-		     --output=$t/$@ $t/$*;			\
-	       }						\
-	    && sed 's|$*\.td/||g' $t/$@ > $@			\
-	    && rm -rf $t ;;					\
+	    #rm -f $@						\
+	    #&& { echo "Updating man page $@";			\
+	#	 rm -rf $t;					\
+	#	 mkdir $t;					\
+	#	 (cd $t && $(LN_S) ../../src/$(mapped_name) $*); \
+	#	    $(PERL) -- $(srcdir)/help2man		\
+	#	     --source='$(PACKAGE_STRING)'		\
+	#	     --include=$(srcdir)/$*.x			\
+	#	     --output=$t/$@ $t/$*;			\
+	#      }						\
+	#    && sed 's|$*\.td/||g' $t/$@ > $@			\
+	#    && rm -rf $t ;;					\
 	esac
 
 distcheck-hook: check-x-vs-1 check-programs-vs-x
