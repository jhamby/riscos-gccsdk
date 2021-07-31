--- libbase/Thread.cpp	29 Oct 2006 18:34:11 -0000	1.9
+++ libbase/Thread.cpp	18 Nov 2006 17:34:02 -0000
@@ -54,9 +54,11 @@
 {
     pthread_attr_init(&_tattr);
 
+#ifndef __riscos__
     if (pthread_attr_setscope(&_tattr, PTHREAD_SCOPE_SYSTEM)) {
         log_msg("Couldn't set thread scope, %s\n", strerror(errno));
     }
+#endif
 
 #if 0
     // on non-Solaris OS, default stack may be too small. This seems
@@ -130,6 +132,7 @@
         log_msg("NONE SPECIFIED\n");
         break;
     }                                                                                
+#ifndef __riscos__
     log_msg("The thread schedule policy is \t");
     pthread_attr_getschedpolicy(&_tattr, &val);
     switch (val) {
@@ -177,6 +180,7 @@
         log_msg("NONE SPECIFIE\n");
         break;
     }    
+#endif
 }
 
 /// \brief Setup callback for thread exit
