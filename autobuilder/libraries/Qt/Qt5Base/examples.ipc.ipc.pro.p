--- examples/ipc/ipc.pro.orig	2013-08-25 19:04:10.000000000 +0100
+++ examples/ipc/ipc.pro	2013-11-14 13:31:58.566266242 +0000
@@ -2,5 +2,5 @@
 
 TEMPLATE      = subdirs
 # no QSharedMemory
-!vxworks:!qnx:SUBDIRS = sharedmemory
+!vxworks:!qnx:!*riscos*:SUBDIRS = sharedmemory
 !wince*:qtHaveModule(network): SUBDIRS += localfortuneserver localfortuneclient
