--- man/Makefile.in.orig	2010-04-23 16:58:39.000000000 +0100
+++ man/Makefile.in	2010-06-08 17:43:35.609606789 +0100
@@ -1538,21 +1538,21 @@
 	      'or inadequate' 1>&2				\
 	    ;;							\
 	  *)							\
-	    rm -f $@ $@-t					\
-	    && {						\
-		 rm -rf $t;					\
-		 mkdir $t;					\
-		 (cd $t && $(LN_S) ../../src/$(mapped_name) $*); \
-		    $(PERL) -- $(srcdir)/help2man		\
-		     --source='$(PACKAGE_STRING)'		\
-		     --include=$(srcdir)/$*.x			\
-		     --output=$t/$@ $t/$*;			\
-	       }						\
-	    && sed 's|$*\.td/||g' $t/$@ > $@-t			\
-	    && rm -rf $t					\
-	    && chmod -w $@-t					\
-	    && mv $@-t $@					\
-	    ;;							\
+#	    rm -f $@ $@-t					\
+#	    && {						\
+#		 rm -rf $t;					\
+#		 mkdir $t;					\
+#		 (cd $t && $(LN_S) ../../src/$(mapped_name) $*); \
+#		    $(PERL) -- $(srcdir)/help2man		\
+#		     --source='$(PACKAGE_STRING)'		\
+#		     --include=$(srcdir)/$*.x			\
+#		     --output=$t/$@ $t/$*;			\
+#	       }						\
+#	    && sed 's|$*\.td/||g' $t/$@ > $@-t			\
+#	    && rm -rf $t					\
+#	    && chmod -w $@-t					\
+#	    && mv $@-t $@					\
+#	    ;;							\
 	esac
 
 # Option descriptions should not start with a capital letter
