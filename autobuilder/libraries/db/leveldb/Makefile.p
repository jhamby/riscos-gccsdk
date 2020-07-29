--- Makefile.orig	2015-04-09 13:03:58.000000000 +0100
+++ Makefile	2015-04-09 13:04:18.424005344 +0100
@@ -118,6 +118,15 @@
 	rm -f $@
 	$(AR) -rs $@ $(LIBOBJECTS)
 
+ifneq ($(PLATFORM_SHARED_EXT),)
+ifeq ($(PLATFORM),OS_RISCOS)
+# Force libmemenv.a to contain PIC code so that it can be linked into a shared library.
+# It will not work in a static build though.
+helpers/memenv/memenv.o: helpers/memenv/memenv.cc
+	$(CXX) $(CXXFLAGS) -fPIC -c $< -o $@
+endif
+endif
+
 db_bench: db/db_bench.o $(LIBOBJECTS) $(TESTUTIL)
 	$(CXX) $(LDFLAGS) db/db_bench.o $(LIBOBJECTS) $(TESTUTIL) -o $@ $(LIBS)
 
