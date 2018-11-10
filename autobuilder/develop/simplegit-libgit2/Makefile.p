--- Makefile.orig	2018-11-10 12:09:20.463747693 +0000
+++ Makefile	2018-11-10 12:10:59.582716814 +0000
@@ -24,12 +24,12 @@
 	git submodule init
 	git submodule update;
 
-${CMAKE_MAKEFILE}:${LIBGIT2_INCLUDE_DIRECTORY} ${CMAKE_FILE_LISTS}
+${CMAKE_MAKEFILE}: ${CMAKE_FILE_LISTS}
 	mkdir -p "${BUILD_DIRECTORY}"
 	echo "#define SIMPLEGIT_REVISION_STRING \"$(COMMITS)\"" >${BUILD_DIRECTORY}/sgit-version.h
 	echo "#define SIMPLEGIT_DATE_STRING \"$(shell date +%d.%m.%Y)\"" >>${BUILD_DIRECTORY}/sgit-version.h
 	cd "${BUILD_DIRECTORY}" && cmake "${CMAKE_DIRECTORY}" -DCMAKE_C_FLAGS="\
-		-imacros ${BUILD_DIRECTORY}/sgit-version.h"
+		-imacros ${BUILD_DIRECTORY}/sgit-version.h" -DCMAKE_TOOLCHAIN_FILE=../toolchain-riscos.cmake -DRISCOS=1
 
 .PHONY:${CMAKE_MAKEFILE}
 
