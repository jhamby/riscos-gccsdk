--- googletest/make/Makefile.orig	2017-07-20 12:37:38.119008000 +0100
+++ googletest/make/Makefile	2017-07-20 12:47:02.531008000 +0100
@@ -14,7 +14,7 @@
 
 # Points to the root of Google Test, relative to where this file is.
 # Remember to tweak this if you move this file.
-GTEST_DIR = $(dir $(lastword $(MAKEFILE_LIST)))..
+GTEST_DIR = $(GCCSDK_INSTALL_ENV)/src/gtest
 
 # Where to find user code.
 USER_DIR = $(dir $(lastword $(MAKEFILE_LIST)))/../samples
@@ -22,10 +22,10 @@
 # Flags passed to the preprocessor.
 # Set Google Test's header directory as a system directory, such that
 # the compiler doesn't generate warnings in Google Test headers.
-CPPFLAGS += -isystem $(GTEST_DIR)/include
+CPPFLAGS += -isystem -static -I$(GTEST_DIR)/include
 
 # Flags passed to the C++ compiler.
-CXXFLAGS += -g -Wall -Wextra -pthread
+CXXFLAGS += -g -static -Wall -Wextra
 
 # All tests produced by this Makefile.  Remember to add new tests you
 # created to the list.
@@ -33,8 +33,8 @@
 
 # All Google Test headers.  Usually you shouldn't change this
 # definition.
-GTEST_HEADERS = /usr/include/gtest/*.h \
-                /usr/include/gtest/internal/*.h
+GTEST_HEADERS = $(GCCSDK_INSTALL_ENV)/include/gtest/*.h \
+                $(GCCSDK_INSTALL_ENV)/include/gtest/internal/*.h
 
 # House-keeping build targets.
 
@@ -79,4 +79,4 @@
 	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/sample1_unittest.cc
 
 sample1_unittest : sample1.o sample1_unittest.o gtest_main.a
-	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@
+	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@
