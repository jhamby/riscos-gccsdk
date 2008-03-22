--- man/Makefile.in.org	2008-03-22 03:13:17.000000000 +0100
+++ man/Makefile.in	2008-03-22 03:14:06.000000000 +0100
@@ -1004,19 +1004,19 @@
 	      'or inadequate' 1>&2 \
 	    ;;							\
 	  *)							\
-	    rm -f $@						\
-	    && { echo "Updating man page $@";			\
-	         rm -rf $t;					\
-	         mkdir $t;					\
-	         (cd $t && $(LN_S) ../../src/$(mapped_name) $*); \
-	            $(PERL) -- $(srcdir)/help2man		\
-	             --source='$(PACKAGE_STRING)'		\
-	             --include=$(srcdir)/$*.x			\
-	             --output=$t/$@ $t/$*;			\
-	       }						\
-	    && sed 's|$*\.td/||g' $t/$@ > $@			\
-	    && chmod a-w $@					\
-	    && rm -rf $t ;;					\
+	    #rm -f $@						\
+	    #&& { echo "Updating man page $@";			\
+	    #     rm -rf $t;					\
+	    #     mkdir $t;					\
+	    #     (cd $t && $(LN_S) ../../src/$(mapped_name) $*); \
+	    #        $(PERL) -- $(srcdir)/help2man		\
+	    #         --source='$(PACKAGE_STRING)'		\
+	    #         --include=$(srcdir)/$*.x			\
+	    #         --output=$t/$@ $t/$*;			\
+	    #   }						\
+	    #&& sed 's|$*\.td/||g' $t/$@ > $@			\
+	    #&& chmod a-w $@					\
+	    #&& rm -rf $t ;;					\
 	esac
 
 distcheck-hook: check-x-vs-1 check-programs-vs-x
