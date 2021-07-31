--- libusal/CMakeLists.txt.orig	2020-12-09 18:04:08.140366534 +0000
+++ libusal/CMakeLists.txt	2020-12-09 18:04:38.651765386 +0000
@@ -1,10 +1,10 @@
 PROJECT (LIBSCG C)
 INCLUDE_DIRECTORIES(../include ${CMAKE_BINARY_DIR} ${CMAKE_BINARY_DIR}/include)
 include(../include/AddScgBits.cmake)
-ADD_DEFINITIONS(-DUSE_RCMD_RSH)
+#ADD_DEFINITIONS(-DUSE_RCMD_RSH)
 
 #SET(LIBSCG_SRCS rdummy.c usalsettarget.c usaltimes.c scsi-linux-ata.c scsi-linux-pg.c scsi-linux-sg.c scsierrs.c scsihack.c scsihelp.c scsiopen.c scsitransp.c)
-SET(LIBSCG_SRCS usalsettarget.c usaltimes.c scsierrs.c scsihack.c scsihelp.c scsiopen.c scsitransp.c scsi-remote.c)
+SET(LIBSCG_SRCS rdummy.c usalsettarget.c usaltimes.c scsierrs.c scsihack.c scsihelp.c scsiopen.c scsitransp.c scsi-remote.c)
 LINK_DIRECTORIES(../librols)
 ADD_LIBRARY (usal STATIC ${LIBSCG_SRCS})
 TARGET_LINK_LIBRARIES(usal ${SCG_SELF_LIBS})
