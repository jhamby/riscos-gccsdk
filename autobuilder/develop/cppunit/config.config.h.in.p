*** config/config.h.in.old	2006-06-29 22:15:48.000000000 +0100
--- config/config.h.in	2007-03-26 12:04:05.000000000 +0100
***************
*** 92,94 ****
--- 92,98 ----
  
  /* Version number of package */
  #undef VERSION
+ 
+ /* Special for Risc OS build */
+ #define CPPUNIT_NO_TESTPLUGIN 1
+ 
