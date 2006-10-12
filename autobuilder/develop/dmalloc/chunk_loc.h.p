--- ./chunk_loc.h.org	2004-10-19 15:51:21.000000000 +0100
+++ ./chunk_loc.h	2006-10-11 22:18:07.730000000 +0100
@@ -77,6 +77,8 @@
 #define ALLOC_FLAG_FENCE	BIT_FLAG(5)	/* slot is fence posted */
 #define ALLOC_FLAG_VALLOC	BIT_FLAG(6)	/* slot is block aligned */
 
+#define SA_CALLERS 4
+
 /*
  * Below defines an allocation structure either on the free or used
  * list.  It tracks allocations that fit in partial, one, or many
@@ -96,6 +98,8 @@
   
   void			*sa_mem;	/* pointer to the memory in question */
   const char		*sa_file;	/* .c filename where allocated */
+  const char		*sa_callers[SA_CALLERS];
+  					/* caller backtrace when allocated */
   unsigned long		sa_use_iter;	/* when last ``used'' */
   
 #if LOG_PNT_SEEN_COUNT
