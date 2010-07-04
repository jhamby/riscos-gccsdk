--- gcc/Makefile.in.orig	2006-11-01 15:40:44.000000000 +0100
+++ gcc/Makefile.in	2010-07-04 19:34:09.996901449 +0200
@@ -400,6 +400,9 @@ STMP_FIXINC = @STMP_FIXINC@
 # Test to see whether <limits.h> exists in the system header files.
 LIMITS_H_TEST = [ -f $(SYSTEM_HEADER_DIR)/limits.h ]
 
+# Control whether to install our own version of limits.h
+INSTALL_LIMITS_H = true
+
 # Directory for prefix to system directories, for
 # each of $(system_prefix)/usr/include, $(system_prefix)/usr/lib, etc.
 TARGET_SYSTEM_ROOT = @TARGET_SYSTEM_ROOT@
@@ -3134,11 +3137,13 @@ stmp-int-hdrs: $(STMP_FIXINC) $(USER_H) 
 	    chmod a+r include/$$realfile; \
 	  fi; \
 	done
-	rm -f include/limits.h
-	cp xlimits.h include/limits.h
 	cp $(UNWIND_H) include/unwind.h
-	chmod a+r include/limits.h
-# Install the README
+	rm -f include/limits.h
+	if $(INSTALL_LIMITS_H) ; then \
+	  cp xlimits.h include/limits.h; \
+	  chmod a+r include/limits.h; \
+	fi
+	# Install the README
 	rm -f include/README
 	cp $(srcdir)/../fixincludes/README-fixinc include/README
 	chmod a+r include/README
@@ -3154,6 +3159,7 @@ s-macro_list : $(GCC_PASSES) 
 	  sed -n -e 's/^#define \([^_][a-zA-Z0-9_]*\).*/\1/p' \
 		-e 's/^#define \(_[^_A-Z][a-zA-Z0-9_]*\).*/\1/p' | \
 	  sort -u > tmp-macro_list
+	rm -f tmp-macro.c
 	$(SHELL) $(srcdir)/../move-if-change tmp-macro_list macro_list
 	$(STAMP) s-macro_list
 
@@ -3771,10 +3777,13 @@ install-mkheaders: stmp-int-hdrs $(STMP_
 	  $(INSTALL_DATA) $$file \
 	    $(DESTDIR)$(itoolsdatadir)/include/$$realfile ; \
 	done
-	$(INSTALL_DATA) xlimits.h $(DESTDIR)$(itoolsdatadir)/include/limits.h
 	$(INSTALL_DATA) $(UNWIND_H) $(DESTDIR)$(itoolsdatadir)/include/unwind.h
-	$(INSTALL_DATA) $(srcdir)/gsyslimits.h \
-	  $(DESTDIR)$(itoolsdatadir)/gsyslimits.h
+	if $(INSTALL_LIMITS_H) ; then \
+	  $(INSTALL_DATA) xlimits.h \
+	    $(DESTDIR)$(itoolsdatadir)/include/limits.h ; \
+	  $(INSTALL_DATA) $(srcdir)/gsyslimits.h \
+	    $(DESTDIR)$(itoolsdatadir)/gsyslimits.h ; \
+	fi
 	$(INSTALL_DATA) macro_list $(DESTDIR)$(itoolsdatadir)/macro_list
 	if [ x$(STMP_FIXPROTO) != x ] ; then \
 	  $(INSTALL_SCRIPT) $(mkinstalldirs) \
