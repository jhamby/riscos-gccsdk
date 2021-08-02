--- examples/corelib/ipc/ipc.pro.orig	2021-07-30 12:12:47.539265846 -0700
+++ examples/corelib/ipc/ipc.pro	2021-07-30 12:13:09.923389900 -0700
@@ -2,5 +2,5 @@
 
 TEMPLATE      = subdirs
 # no QSharedMemory
-!vxworks:SUBDIRS = sharedmemory
+!vxworks:!*riscos*:SUBDIRS = sharedmemory
 !wince:qtHaveModule(network): SUBDIRS += localfortuneserver localfortuneclient
