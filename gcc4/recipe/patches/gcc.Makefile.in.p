--- gcc/Makefile.in.orig	2007-01-12 16:20:38.000000000 +0000
+++ gcc/Makefile.in	2007-01-11 16:15:54.000000000 +0000
@@ -400,6 +400,9 @@
 # Test to see whether <limits.h> exists in the system header files.
 LIMITS_H_TEST = [ -f $(SYSTEM_HEADER_DIR)/limits.h ]
 
+# Control whether to install our own version of limits.h
+INSTALL_LIMITS_H = true
+
 # Directory for prefix to system directories, for
 # each of $(system_prefix)/usr/include, $(system_prefix)/usr/lib, etc.
 TARGET_SYSTEM_ROOT = @TARGET_SYSTEM_ROOT@
@@ -3133,11 +3136,13 @@
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
@@ -3153,6 +3158,7 @@
 	  sed -n -e 's/^#define \([^_][a-zA-Z0-9_]*\).*/\1/p' \
 		-e 's/^#define \(_[^_A-Z][a-zA-Z0-9_]*\).*/\1/p' | \
 	  sort -u > tmp-macro_list
+	rm -f tmp-macro.c
 	$(SHELL) $(srcdir)/../move-if-change tmp-macro_list macro_list
 	$(STAMP) s-macro_list
 
@@ -3770,10 +3776,13 @@
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
@@ -3863,6 +3872,12 @@
 	  echo "append LDFLAGS \" \$$newlib_ldflags\"" >> ./tmp0; \
 	else true; \
 	fi
+	@if [ -d $(objdir)/../$(target_subdir)/libunixlib ] \
+	    && [ "${host}" != "${target}" ]; then \
+	  echo "set CFLAGS_FOR_TARGET \"-I\$$srcdir/../libunixlib/include -L$(objdir)/../$(target_subdir)/libunixlib/ -static\"" >> ./tmp0; \
+	  echo "set LDFLAGS_FOR_TARGET \"-L$(objdir)/../$(target_subdir)/libunixlib/\"" >> ./tmp0; \
+	else true; \
+	fi
 	@if [ -d $(objdir)/../ld ] ; then \
 	  echo "append LDFLAGS \" -L$(objdir)/../ld\"" >> ./tmp0; \
 	else true; \
