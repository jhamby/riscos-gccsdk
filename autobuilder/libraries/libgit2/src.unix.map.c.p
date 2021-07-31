*** src/unix/map.c.orig	2018-11-11 14:26:43.295859061 +0000
--- src/unix/map.c	2018-11-11 14:28:06.066166774 +0000
***************
*** 9,15 ****
  
  #include "git2/common.h"
  
! #if !defined(GIT_WIN32) && !defined(NO_MMAP)
  
  #include "map.h"
  #include <sys/mman.h>
--- 9,15 ----
  
  #include "git2/common.h"
  
! #if !defined(GIT_WIN32) && !defined(NO_MMAP) || defined(GIT_RISCOS)
  
  #include "map.h"
  #include <sys/mman.h>
***************
*** 18,24 ****
--- 18,28 ----
  
  int git__page_size(size_t *page_size)
  {
+ #ifdef GIT_RISCOS
+ 	long sc_page_size = 8192;
+ #else
  	long sc_page_size = sysconf(_SC_PAGE_SIZE);
+ #endif
  	if (sc_page_size < 0) {
  		giterr_set(GITERR_OS, "can't determine system page size");
  		return -1;
