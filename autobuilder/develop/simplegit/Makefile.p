--- Makefile.orig
+++ Makefile
@@ -29,7 +29,7 @@ ${CMAKE_MAKEFILE}:${LIBGIT2_INCLUDE_DIRECTORY} ${CMAKE_FILE_LISTS}
 	echo "#define SIMPLEGIT_REVISION_STRING \"$(COMMITS)\"" >${BUILD_DIRECTORY}/sgit-version.h
 	echo "#define SIMPLEGIT_DATE_STRING \"$(shell date +%d.%m.%Y)\"" >>${BUILD_DIRECTORY}/sgit-version.h
 	cd "${BUILD_DIRECTORY}" && cmake "${CMAKE_DIRECTORY}" -DCMAKE_C_FLAGS="\
-		-imacros ${BUILD_DIRECTORY}/sgit-version.h"
+		-imacros ${BUILD_DIRECTORY}/sgit-version.h" -DCMAKE_TOOLCHAIN_FILE=../toolchain-riscos.cmake -DRISCOS=1
 
 .PHONY:${CMAKE_MAKEFILE}
 
@@ -45,4 +45,4 @@ clean:
 
 .PHONY: libgit2
 libgit2:
-	cd libgit2 && mkdir -p build && cd build && cmake .. -DBUILD_CLAR=0 && $(MAKE)
+	cd libgit2 && mkdir -p build && cd build && cmake .. -DCMAKE_TOOLCHAIN_FILE=../../toolchain-riscos.cmake -DRISCOS=1 -DBUILD_SHARED_LIBS=OFF -DTHREADSAFE=OFF -DBUILD_CLAR=0 && $(MAKE)

