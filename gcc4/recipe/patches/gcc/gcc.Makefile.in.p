--- gcc/Makefile.in.orig	2007-01-16 23:44:03.000000000 +0100
+++ gcc/Makefile.in	2007-01-16 23:45:06.000000000 +0100
@@ -397,6 +397,9 @@
 # Test to see whether <limits.h> exists in the system header files.
 LIMITS_H_TEST = [ -f $(SYSTEM_HEADER_DIR)/limits.h ]
 
+# Control whether to install our own version of limits.h
+INSTALL_LIMITS_H = true
+
 # Directory for prefix to system directories, for
 # each of $(system_prefix)/usr/include, $(system_prefix)/usr/lib, etc.
 TARGET_SYSTEM_ROOT = @TARGET_SYSTEM_ROOT@
@@ -3129,11 +3132,13 @@
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
@@ -3149,6 +3154,7 @@
 	  sed -n 's/^#define \([^_][a-zA-Z0-9_]*\).*/\1/p ; \
 		s/^#define \(_[^_A-Z][a-zA-Z0-9_]*\).*/\1/p' | \
 	  sort -u > tmp-macro_list
+	rm -f tmp-macro.c
 	$(SHELL) $(srcdir)/../move-if-change tmp-macro_list macro_list
 	$(STAMP) s-macro_list
 
@@ -3763,10 +3769,13 @@
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

