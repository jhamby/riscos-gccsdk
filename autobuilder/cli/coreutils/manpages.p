--- man/Makefile.in~	2004-03-11 08:58:00.000000000 +0000
+++ man/Makefile.in	2005-03-06 22:08:29.000000000 +0000
@@ -602,14 +602,14 @@
 .x.1:
 	@rm -f $@
 	@echo "Updating man page $@";		\
-	mkdir $t;				\
-	(cd $t && $(LN_S) ../../src/$(mapped_name)$(EXEEXT) $*$(EXEEXT)); \
-	$(PERL) -- $(srcdir)/help2man		\
-	    --info-page='$(PACKAGE) $*'		\
-	    --include=$(srcdir)/$*.x		\
-	    --output=$@ $t/$*$(EXEEXT)
-	@chmod a-w $@
-	@rm -rf $t
+	mkdir -p $t;				\
+	#(cd $t && $(LN_S) ../../src/$(mapped_name)$(EXEEXT) $*$(EXEEXT)); \
+	#$(PERL) -- $(srcdir)/help2man		\
+	#    --info-page='$(PACKAGE) $*'		\
+	#    --include=$(srcdir)/$*.x		\
+	#    --output=$@ $t/$*$(EXEEXT)
+	#@chmod a-w $@
+	#@rm -rf $t
 
 check-local: check-x-vs-1 check-programs-vs-x
 
