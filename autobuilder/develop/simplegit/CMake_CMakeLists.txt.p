--- CMake/CMakeLists.txt.orig
+++ CMake/CMakeLists.txt
@@ -64,6 +64,9 @@ IF (AMIGA)
 	link_directories(${ROOT_DIRECTORY}/interim-openssl/openssl/repo/)
 	set(LIBS ${LIBS} ssl.a crypto.a)
 	set(LIBS ${LIBS} unix.a auto.a)
+ELSEIF(RISCOS)
+	include_directories(${OPENSSL_INCLUDE_DIR})
+	set(LIBS ${LIBS} ${OPENSSL_LIBRARIES})
 ELSE()
 	set(LIBS ${LIBS} ssl crypto)
 ENDIF()
@@ -86,7 +89,7 @@ set_target_properties(sgit-bin PROPERTIES LINK_FLAGS -Wl,--cref,-M,-Map=sgit.map
 add_dependencies(sgit-bin build_libgit2)
 target_link_libraries(sgit-bin sgit-lib ${LIBS})
 
-IF (NOT AMIGA)
+IF (NOT AMIGA AND NOT RISCOS)
 	enable_testing()
 
 	add_executable(sgit-test ${UNIT_TESTS_DIRECTORY}/sgit-test.c)
