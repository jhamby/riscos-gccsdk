--- include/X11/Xlibint.h.orig	2012-09-23 11:40:32.000000000 +0100
+++ include/X11/Xlibint.h	2012-09-23 11:46:54.000000000 +0100
@@ -414,6 +414,30 @@
 #define WORD64ALIGN
 #endif /* WORD64 */

+/**
+ * Return a len-sized request buffer for the request type. This function may
+ * flush the output queue.
+ *
+ * @param dpy The display connection
+ * @param type The request type
+ * @param len Length of the request in bytes
+ *
+ * @returns A pointer to the request buffer with a few default values
+ * initialized.
+ */
+extern void *_XGetRequest(Display *dpy, CARD8 type, size_t len);
+
+/* GetReqSized is the same as GetReq but allows the caller to specify the
+ * size in bytes. 'sz' must be a multiple of 4! */
+
+#if !defined(UNIXCPP) || defined(ANSICPP)
+#define GetReqSized(name, sz, req) \
+	req = (x##name##Req *) _XGetRequest(dpy, X_##name, sz)
+#else
+#define GetReqSized(name, sz, req) \
+	req = (x/**/name/**/Req *) _XGetRequest(dpy, X_/**/name, sz)
+#endif
+

 /*
  * GetReq - Get the next available X request packet in the buffer and
@@ -426,25 +450,10 @@

 #if !defined(UNIXCPP) || defined(ANSICPP)
 #define GetReq(name, req) \
-        WORD64ALIGN\
-	if ((dpy->bufptr + SIZEOF(x##name##Req)) > dpy->bufmax)\
-		_XFlush(dpy);\
-	req = (x##name##Req *)(dpy->last_req = dpy->bufptr);\
-	req->reqType = X_##name;\
-	req->length = (SIZEOF(x##name##Req))>>2;\
-	dpy->bufptr += SIZEOF(x##name##Req);\
-	dpy->request++
-
+	GetReqSized(name, SIZEOF(x##name##Req), req)
 #else  /* non-ANSI C uses empty comment instead of "##" for token concatenation */
 #define GetReq(name, req) \
-        WORD64ALIGN\
-	if ((dpy->bufptr + SIZEOF(x/**/name/**/Req)) > dpy->bufmax)\
-		_XFlush(dpy);\
-	req = (x/**/name/**/Req *)(dpy->last_req = dpy->bufptr);\
-	req->reqType = X_/**/name;\
-	req->length = (SIZEOF(x/**/name/**/Req))>>2;\
-	dpy->bufptr += SIZEOF(x/**/name/**/Req);\
-	dpy->request++
+	GetReqSized(name, SIZEOF(x/**/name/**/Req), req)
 #endif

 /* GetReqExtra is the same as GetReq, but allocates "n" additional
@@ -452,24 +461,10 @@

 #if !defined(UNIXCPP) || defined(ANSICPP)
 #define GetReqExtra(name, n, req) \
-        WORD64ALIGN\
-	if ((dpy->bufptr + SIZEOF(x##name##Req) + n) > dpy->bufmax)\
-		_XFlush(dpy);\
-	req = (x##name##Req *)(dpy->last_req = dpy->bufptr);\
-	req->reqType = X_##name;\
-	req->length = (SIZEOF(x##name##Req) + n)>>2;\
-	dpy->bufptr += SIZEOF(x##name##Req) + n;\
-	dpy->request++
+        GetReqSized(name, SIZEOF(x##name##Req) + n, req)
 #else
 #define GetReqExtra(name, n, req) \
-        WORD64ALIGN\
-	if ((dpy->bufptr + SIZEOF(x/**/name/**/Req) + n) > dpy->bufmax)\
-		_XFlush(dpy);\
-	req = (x/**/name/**/Req *)(dpy->last_req = dpy->bufptr);\
-	req->reqType = X_/**/name;\
-	req->length = (SIZEOF(x/**/name/**/Req) + n)>>2;\
-	dpy->bufptr += SIZEOF(x/**/name/**/Req) + n;\
-	dpy->request++
+        GetReqSized(name, SIZEOF(x/**/name/**/Req) + n, req)
 #endif


@@ -481,26 +476,12 @@

 #if !defined(UNIXCPP) || defined(ANSICPP)
 #define GetResReq(name, rid, req) \
-        WORD64ALIGN\
-	if ((dpy->bufptr + SIZEOF(xResourceReq)) > dpy->bufmax)\
-	    _XFlush(dpy);\
-	req = (xResourceReq *) (dpy->last_req = dpy->bufptr);\
-	req->reqType = X_##name;\
-	req->length = 2;\
-	req->id = (rid);\
-	dpy->bufptr += SIZEOF(xResourceReq);\
-	dpy->request++
+	req = (xResourceReq *) _XGetRequest(dpy, X_##name, SIZEOF(xResourceReq)); \
+	req->id = (rid)
 #else
 #define GetResReq(name, rid, req) \
-        WORD64ALIGN\
-	if ((dpy->bufptr + SIZEOF(xResourceReq)) > dpy->bufmax)\
-	    _XFlush(dpy);\
-	req = (xResourceReq *) (dpy->last_req = dpy->bufptr);\
-	req->reqType = X_/**/name;\
-	req->length = 2;\
-	req->id = (rid);\
-	dpy->bufptr += SIZEOF(xResourceReq);\
-	dpy->request++
+	req = (xResourceReq *) _XGetRequest(dpy, X_/**/name, SIZEOF(xResourceReq)); \
+	req->id = (rid)
 #endif

 /*
@@ -509,24 +490,10 @@
  */
 #if !defined(UNIXCPP) || defined(ANSICPP)
 #define GetEmptyReq(name, req) \
-        WORD64ALIGN\
-	if ((dpy->bufptr + SIZEOF(xReq)) > dpy->bufmax)\
-	    _XFlush(dpy);\
-	req = (xReq *) (dpy->last_req = dpy->bufptr);\
-	req->reqType = X_##name;\
-	req->length = 1;\
-	dpy->bufptr += SIZEOF(xReq);\
-	dpy->request++
+	req = (xReq *) _XGetRequest(dpy, X_##name, SIZEOF(xReq))
 #else
 #define GetEmptyReq(name, req) \
-        WORD64ALIGN\
-	if ((dpy->bufptr + SIZEOF(xReq)) > dpy->bufmax)\
-	    _XFlush(dpy);\
-	req = (xReq *) (dpy->last_req = dpy->bufptr);\
-	req->reqType = X_/**/name;\
-	req->length = 1;\
-	dpy->bufptr += SIZEOF(xReq);\
-	dpy->request++
+	req = (xReq *) _XGetRequest(dpy, X_/**/name, SIZEOF(xReq))
 #endif

 #ifdef WORD64
