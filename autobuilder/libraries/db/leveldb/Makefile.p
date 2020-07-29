--- Makefile.orig	2018-04-12 08:01:31.484752290 +0100
+++ Makefile	2018-04-12 08:06:09.007530754 +0100
@@ -314,6 +314,15 @@
 	rm -f $@
 	$(AR) -rs $@ $(SHARED_MEMENVOBJECTS)
 
+ifneq ($(PLATFORM_SHARED_EXT),)
+ifeq ($(PLATFORM),OS_RISCOS)
+# Force libmemenv.a to contain PIC code so that it can be linked into a shared library.
+# It will not work in a static build though.
+helpers/memenv/memenv.o: helpers/memenv/memenv.cc
+	$(CXX) $(CXXFLAGS) -fPIC -c $< -o $@
+endif
+endif
+
 $(STATIC_OUTDIR)/db_bench:db/db_bench.cc $(STATIC_LIBOBJECTS) $(TESTUTIL)
 	$(CXX) $(LDFLAGS) $(CXXFLAGS) db/db_bench.cc $(STATIC_LIBOBJECTS) $(TESTUTIL) -o $@ $(LIBS)
 
