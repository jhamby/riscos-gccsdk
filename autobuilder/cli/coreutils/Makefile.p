--- man/Makefile.in.orig	2008-01-30 22:02:12.000000000 -0800
+++ man/Makefile.in	2008-01-30 22:02:38.000000000 -0800
@@ -1008,7 +1008,7 @@
 	    && { echo "Updating man page $@";			\
 	         rm -rf $t;					\
 	         mkdir $t;					\
-	         (cd $t && $(LN_S) ../../src/$(mapped_name) $*); \
+	         (cd $t && $(LN_S) $$(which $(mapped_name)) $*); \
 	            $(PERL) -- $(srcdir)/help2man		\
 	             --source='$(PACKAGE_STRING)'		\
 	             --include=$(srcdir)/$*.x			\
