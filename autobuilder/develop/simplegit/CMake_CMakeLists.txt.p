--- CMake/CMakeLists.txt.orig
+++ CMake/CMakeLists.txt
@@ -64,6 +64,11 @@ IF (AMIGA)
 	link_directories(${ROOT_DIRECTORY}/interim-openssl/openssl/repo/)
 	set(LIBS ${LIBS} ssl.a crypto.a)
 	set(LIBS ${LIBS} unix.a auto.a)
+ELSEIF(RISCOS)
+	include_directories(${OPENSSL_INCLUDE_DIR})
+	find_library(HTTPPARSER_LIBRARY libhttp_parser.a)
+	find_library(ZLIB_LIBRARY libz.a)
+	set(LIBS ${LIBS} ${OPENSSL_LIBRARIES} ${HTTPPARSER_LIBRARY} ${ZLIB_LIBRARY})
 ELSE()
 	set(LIBS ${LIBS} ssl crypto)
 ENDIF()
@@ -86,7 +91,7 @@ set_target_properties(sgit-bin PROPERTIES LINK_FLAGS -Wl,--cref,-M,-Map=sgit.map
 add_dependencies(sgit-bin build_libgit2)
 target_link_libraries(sgit-bin sgit-lib ${LIBS})
 
-IF (NOT AMIGA)
+IF (NOT AMIGA AND NOT RISCOS)
 	enable_testing()
 
 	add_executable(sgit-test ${UNIT_TESTS_DIRECTORY}/sgit-test.c)
