--- choxlibint.c.orig	2012-09-23 11:38:17.000000000 +0100
+++ choxlibint.c	2012-09-23 11:55:37.000000000 +0100
@@ -190,6 +190,37 @@
     return NULL;
 }
 
+/*
+ * WARNING: This implementation's pre-conditions and post-conditions
+ * must remain compatible with the old macro-based implementations of
+ * GetReq, GetReqExtra, GetResReq, and GetEmptyReq. The portions of the
+ * Display structure affected by those macros are part of libX11's
+ * ABI.
+ */
+void *_XGetRequest(Display *dpy, CARD8 type, size_t len)
+{
+    xReq *req;
+
+    WORD64ALIGN
+
+    if (dpy->bufptr + len > dpy->bufmax)
+	_XFlush(dpy);
+
+    if (len % 4)
+	fprintf(stderr,
+		"Xlib: request %d length %zd not a multiple of 4.\n",
+		type, len);
+
+    dpy->last_req = dpy->bufptr;
+
+    req = (xReq*)dpy->bufptr;
+    req->reqType = type;
+    req->length = len / 4;
+    dpy->bufptr += len;
+    dpy->request++;
+    return req;
+}
+
 extern Bool _XAsyncErrorHandler(
     Display*	dpy,
     xReply*	rep,
