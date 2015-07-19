--- examples/corelib/ipc/ipc.pro.orig	2015-07-15 18:24:20.210735900 +0100
+++ examples/corelib/ipc/ipc.pro	2015-07-15 19:56:27.390807524 +0100
@@ -2,5 +2,5 @@
 
 TEMPLATE      = subdirs
 # no QSharedMemory
-!vxworks:SUBDIRS = sharedmemory
+!vxworks:!*riscos*:SUBDIRS = sharedmemory
 !wince*:qtHaveModule(network): SUBDIRS += localfortuneserver localfortuneclient
