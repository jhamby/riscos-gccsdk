--- server/shm.cpp	29 Oct 2006 18:34:11 -0000	1.25
+++ server/shm.cpp	18 Nov 2006 17:34:09 -0000
@@ -29,11 +29,11 @@
 #include <unistd.h>
 #include <dirent.h>
 #include <fcntl.h>
-#ifndef HAVE_WINSOCK_H
+#if !defined(HAVE_WINSOCK_H) && !defined(__riscos__)
 #include <sys/mman.h>
 #include <sys/shm.h>
 #include <sys/ipc.h>
-#else
+#elif !defined(__riscos__)
 #include <windows.h>
 #include <process.h>
 #include <io.h>
@@ -133,10 +133,14 @@
     _shmfd = shmget(_shmkey, _size, shmflg);
     if (_shmfd < 0 && errno == EEXIST)
 # else
+#  ifdef __riscos__
+    if (0)
+#  else
 	_shmhandle = CreateFileMapping ((HANDLE) 0xFFFFFFFF, NULL,
 					PAGE_READWRITE, 0,
 					_size, filespec);
     if (_shmhandle <= 0)
+#  endif
 # endif	// end of HAVE_SHMGET
 #endif // end of HAVE_SHM_OPEN
 	{
@@ -152,11 +156,15 @@
 	// Get the shared memory id for this segment
 	_shmfd = shmget(_shmkey, _size, 0);
 # else
+#  ifdef __riscos__
+        // do nothing, we never get here.
+#  else
 	_shmhandle = CreateFileMapping ((HANDLE) 0xFFFFFFFF, NULL,
 					PAGE_READWRITE, 0,
 					_size, filespec);
 # endif
 #endif
+#endif
     }
     
     // MacOSX returns this when you use O_EXCL for shm_open() instead
@@ -164,8 +172,12 @@
 #if defined(HAVE_SHMGET) ||  defined(HAVE_SHM_OPEN)
     if (_shmfd < 0 && errno == EINVAL)
 #else
+# ifdef __riscos__
+    if (0)
+# else
     if (_shmhandle <= 0 && errno == EINVAL)
 #endif
+#endif
 	{
 	exists = true;
 	log_msg(
@@ -187,8 +199,12 @@
 #if defined(HAVE_SHMGET) ||  defined(HAVE_SHM_OPEN)
     if (_shmfd >= 0)
 #else
+# ifdef __riscos__
+    if (1)
+# else
     if (_shmhandle >= 0)
 #endif
+#endif
     {
 #ifdef HAVE_SHM_OPEN
 	if (!exists) {
@@ -211,10 +227,18 @@
 	    return false;
 	}
 # else
+#  ifdef __riscos__
+        _addr = (char *)malloc(_size);
+        if (_addr == 0) {
+            log_msg("WARNING: malloc() failed\n");
+            return false;
+        }
+#  else
 	_addr = (char *)MapViewOfFile (_shmhandle, FILE_MAP_ALL_ACCESS,
 				       0, 0, _size);
 # endif
 #endif
+#endif
 //	log_msg("The address to the shared memory segment is: %p", _addr);
         if (exists && !nuke) {
 	    // If there is an existing memory segment that we don't
@@ -260,9 +284,13 @@
 	shmdt(_addr);
 	_addr = (char *)shmat(_shmfd, (void *)addr, 0);
 # else
+#  ifdef __riscos__
+        _addr = _addr;
+#  else
 	CloseHandle(_shmhandle);	
 	_addr = (char *)MapViewOfFile (_shmhandle, FILE_MAP_ALL_ACCESS,
 			       0, 0, _size);
+#  endif
 # endif // end of HAVE_SHMAT
 	}
 #endif // end of HAVE_SHM_OPEN
@@ -408,9 +436,13 @@
 #ifdef HAVE_SHMGET
      shmctl(_shmfd, IPC_RMID, 0);
 #else
+# ifdef __riscos__
+     free(_addr);
+# else
      CloseHandle(_shmhandle);
 #endif
 #endif
+#endif
     
     _addr = 0;
     _alloced = 0;
