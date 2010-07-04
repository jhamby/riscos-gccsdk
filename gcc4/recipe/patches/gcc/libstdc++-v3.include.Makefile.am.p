--- libstdc++-v3/include/Makefile.am.orig	2005-11-05 10:42:01.000000000 +0100
+++ libstdc++-v3/include/Makefile.am	2010-07-04 15:43:36.884402200 +0200
@@ -612,6 +612,7 @@ endif
 
 host_srcdir = ${glibcxx_srcdir}/$(OS_INC_SRCDIR)
 host_builddir = ./${host_alias}/bits
+host_installdir = ${gxx_include_dir}/${host_alias}$(MULTISUBDIR)/bits
 host_headers = \
 	${host_srcdir}/ctype_base.h \
 	${host_srcdir}/ctype_inline.h \
@@ -646,6 +647,7 @@ thread_host_headers = \
 
 pch_input = ${host_builddir}/stdc++.h
 pch_output_builddir = ${host_builddir}/stdc++.h.gch
+pch_output_installdir = ${host_installdir}/stdc++.h.gch
 pch_source = ${glibcxx_srcdir}/include/stdc++.h
 PCHFLAGS=-Winvalid-pch -Wno-deprecated -x c++-header $(CXXFLAGS)
 if GLIBCXX_BUILD_PCH
@@ -907,9 +909,9 @@ endif
 # are copied here.
 install-freestanding-headers:
 	$(mkinstalldirs) $(DESTDIR)${gxx_include_dir}
-	$(mkinstalldirs) $(DESTDIR)${gxx_include_dir}/${host_builddir}
+	$(mkinstalldirs) $(DESTDIR)${host_installdir}
 	for file in ${host_srcdir}/os_defines.h ${host_builddir}/c++config.h; do \
-	  $(INSTALL_DATA) $${file} $(DESTDIR)${gxx_include_dir}/${host_builddir}; done
+	  $(INSTALL_DATA) $${file} $(DESTDIR)${host_installdir}; done
 	$(mkinstalldirs) $(DESTDIR)${gxx_include_dir}/${std_builddir}
 	$(INSTALL_DATA) ${std_builddir}/limits $(DESTDIR)${gxx_include_dir}/${std_builddir}
 	$(mkinstalldirs) $(DESTDIR)${gxx_include_dir}/${c_base_builddir}
@@ -969,15 +971,15 @@ install-headers:
 	$(mkinstalldirs) $(DESTDIR)${gxx_include_dir}/${debug_builddir}
 	for file in ${debug_headers}; do \
 	  $(INSTALL_DATA) $${file} $(DESTDIR)${gxx_include_dir}/${debug_builddir}; done
-	$(mkinstalldirs) $(DESTDIR)${gxx_include_dir}/${host_builddir}
+	$(mkinstalldirs) $(DESTDIR)${host_installdir}
 	for file in ${host_headers} ${host_headers_extra} \
 	 ${thread_host_headers}; do \
-	  $(INSTALL_DATA) $${file} $(DESTDIR)${gxx_include_dir}/${host_builddir}; done
+	  $(INSTALL_DATA) $${file} $(DESTDIR)${host_installdir}; done
 
 install-pch:
-	$(mkinstalldirs) $(DESTDIR)${gxx_include_dir}/${pch_output_builddir}
+	$(mkinstalldirs) $(DESTDIR)${pch_output_installdir}
 	for file in ${pch_output_builddir}/*; do \
-	  $(INSTALL_DATA) $$file $(DESTDIR)${gxx_include_dir}/${pch_output_builddir}; done
+	  $(INSTALL_DATA) $$file $(DESTDIR)${pch_output_installdir}; done
 
 # By adding these files here, automake will remove them for 'make clean'
 CLEANFILES = ${pch_input} ${pch_output_builddir}/*
