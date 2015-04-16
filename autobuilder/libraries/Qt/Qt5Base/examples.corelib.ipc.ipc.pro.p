--- examples/corelib/ipc/ipc.pro.orig	2015-02-17 04:56:28.000000000 +0000
+++ examples/corelib/ipc/ipc.pro	2015-04-12 18:59:14.737199712 +0100
@@ -2,5 +2,5 @@
 
 TEMPLATE      = subdirs
 # no QSharedMemory
-!vxworks:!qnx:SUBDIRS = sharedmemory
+!vxworks:!qnx:!*riscos*:SUBDIRS = sharedmemory
 !wince*:qtHaveModule(network): SUBDIRS += localfortuneserver localfortuneclient
