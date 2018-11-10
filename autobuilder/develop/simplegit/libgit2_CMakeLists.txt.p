--- libgit2/CMakeLists.txt
+++ libgit2/CMakeLists.txt
@@ -590,6 +590,9 @@ IF (WIN32 AND NOT CYGWIN)
 ELSEIF (AMIGA)
        ADD_DEFINITIONS(-DNO_ADDRINFO -DNO_READDIR_R -DNO_MMAP)
        FILE(GLOB SRC_OS src/unix/realpath.c src/unix/realpath.h)
+ELSEIF (RISCOS)
+       ADD_DEFINITIONS(-DNO_MMAP)
+       FILE(GLOB SRC_OS src/unix/*.c src/unix/*.h)
 ELSE()
        IF (VALGRIND)
                ADD_DEFINITIONS(-DNO_MMAP)

