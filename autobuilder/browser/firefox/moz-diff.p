Index: configure.in
===================================================================
RCS file: /cvsroot/mozilla/configure.in,v
retrieving revision 1.1476
diff -u -r1.1476 configure.in
--- configure.in	23 Jun 2005 16:55:49 -0000	1.1476
+++ configure.in	24 Jun 2005 14:51:24 -0000
@@ -1091,6 +1091,23 @@
     HOST_OPTIMIZE_FLAGS="${HOST_OPTIMIZE_FLAGS=-O2}"
     ;;
 
+*riscos*)
+    AR=/home/riscos/env/ro-ar
+    RANLIB=echo
+    CC=/home/riscos/cross/bin/gcc
+    CXX=/home/riscos/cross/bin/gcc
+    HOST_CFLAGS="$HOST_CFLAGS -DXP_UNIX"
+    HOST_OPTIMIZE_FLAGS="${HOST_OPTIMIZE_FLAGS=-O2}"
+    DSO_PIC_CFLAGS=
+    DSO_LDOPTS=
+    DLL_SUFFIX=.a
+    BIN_SUFFIX=,ff8
+    MKSHLIB='/home/riscos/env/ro-ar rc $@'
+    MKCSHLIB='/home/riscos/env/ro-ar rc $@'
+    MKSHLIB_FORCE_ALL=''
+    MKSHLIB_UNFORCE_ALL=''
+    MOZ_OPTIMIZE_FLAGS=-O2
+    ;;
 *)
     HOST_CFLAGS="$HOST_CFLAGS -DXP_UNIX"
     HOST_OPTIMIZE_FLAGS="${HOST_OPTIMIZE_FLAGS=-O2}"
Index: browser/app/Makefile.in
===================================================================
RCS file: /cvsroot/mozilla/browser/app/Makefile.in,v
retrieving revision 1.83
diff -u -r1.83 Makefile.in
--- browser/app/Makefile.in	18 Jun 2005 12:32:47 -0000	1.83
+++ browser/app/Makefile.in	24 Jun 2005 14:51:29 -0000
@@ -70,7 +70,7 @@
 LOCAL_INCLUDES += -I$(topsrcdir)/toolkit/xre
 
 ifdef BUILD_STATIC_LIBS
-STATIC_COMPONENTS_LINKER_PATH = -L$(DIST)/lib/components
+STATIC_COMPONENTS_LINKER_PATH = -L$(DIST)/lib/components -lSM -lICE -ldb-4 -lfreebl 
 endif
 
 ifneq (,$(filter mac cocoa,$(MOZ_WIDGET_TOOLKIT)))
Index: config/rules.mk
===================================================================
RCS file: /cvsroot/mozilla/config/rules.mk,v
retrieving revision 3.478
diff -u -r3.478 rules.mk
--- config/rules.mk	22 Jun 2005 15:55:31 -0000	3.478
+++ config/rules.mk	24 Jun 2005 14:51:32 -0000
@@ -40,6 +40,8 @@
 topsrcdir		= $(DEPTH)
 endif
 
+DISABLE_DIST_GRE=1
+
 ifndef MOZILLA_DIR
 MOZILLA_DIR = $(topsrcdir)
 endif
Index: dbm/include/mcom_db.h
===================================================================
RCS file: /cvsroot/mozilla/dbm/include/mcom_db.h,v
retrieving revision 3.41
diff -u -r3.41 mcom_db.h
--- dbm/include/mcom_db.h	29 Apr 2005 13:33:34 -0000	3.41
+++ dbm/include/mcom_db.h	24 Jun 2005 14:51:33 -0000
@@ -329,6 +329,8 @@
 #define	DB_TXN		    0x8000	/* Do transactions. */
 #endif
 
+#define uint unsigned int
+
 /* Access method description structure. */
 typedef struct __db {
 	DBTYPE type;			/* Underlying db type. */
Index: dbm/src/mktemp.c
===================================================================
RCS file: /cvsroot/mozilla/dbm/src/mktemp.c,v
retrieving revision 3.5
diff -u -r3.5 mktemp.c
--- dbm/src/mktemp.c	20 Jan 2003 23:13:37 -0000	3.5
+++ dbm/src/mktemp.c	24 Jun 2005 14:51:33 -0000
@@ -99,7 +99,7 @@
 _gettemp(char *path, register int *doopen, int extraFlags)
 {    
 #if !defined(_WINDOWS) || defined(_WIN32)
-	extern int errno;                    
+	//extern int errno;                    
 #endif
 	register char *start, *trv;
 	struct stat sbuf;
Index: directory/c-sdk/ldap/include/portable.h
===================================================================
RCS file: /cvsroot/mozilla/directory/c-sdk/ldap/include/portable.h,v
retrieving revision 5.14
diff -u -r5.14 portable.h
--- directory/c-sdk/ldap/include/portable.h	15 Mar 2004 06:50:59 -0000	5.14
+++ directory/c-sdk/ldap/include/portable.h	24 Jun 2005 14:51:35 -0000
@@ -274,7 +274,7 @@
     defined(NCR) || defined(OSF1) || defined(NEC) || defined(VMS) || \
     ( defined(HPUX10) && !defined(_REENTRANT)) || defined(HPUX11) || \
     defined(UnixWare) || defined(NETBSD) || \
-    defined(FREEBSD) || defined(OPENBSD) || \
+    defined(FREEBSD) || defined(OPENBSD) || defined(__riscos__) || \
     (defined(LINUX) && __GLIBC__ < 2) || \
     (defined(AIX) && !defined(USE_REENTRANT_LIBC))
 #define GETHOSTBYNAME( n, r, b, l, e )  gethostbyname( n )
Index: directory/c-sdk/ldap/libraries/libprldap/ldappr-error.c
===================================================================
RCS file: /cvsroot/mozilla/directory/c-sdk/ldap/libraries/libprldap/ldappr-error.c,v
retrieving revision 5.7
diff -u -r5.7 ldappr-error.c
--- directory/c-sdk/ldap/libraries/libprldap/ldappr-error.c	15 Mar 2004 06:25:32 -0000	5.7
+++ directory/c-sdk/ldap/libraries/libprldap/ldappr-error.c	24 Jun 2005 14:51:35 -0000
@@ -223,17 +223,17 @@
 #define ETXTBSY         -1
 #endif
 
-#if defined(BSDI) || defined(OPENBSD) || defined (NETBSD)
+#if defined(BSDI) || defined(OPENBSD) || defined (NETBSD) || defined (__riscos__)
 #define ENOTSUP		-1
 #endif
 
-#if defined(OSF1) || defined(BSDI) || defined(VMS) || defined(OPENBSD)
+#if defined(OSF1) || defined(BSDI) || defined(VMS) || defined(OPENBSD) || defined (__riscos__)
 #define EOVERFLOW       -1
 #endif
 
 #if defined(__hpux) || defined(_AIX) || defined(OSF1) || defined(DARWIN) || \
   defined(BEOS) || defined(FREEBSD) || defined(BSDI) || defined(VMS) || \
-  defined(OPENBSD) || defined(NETBSD)
+  defined(OPENBSD) || defined(NETBSD) || defined (__riscos__)
 #define EDEADLOCK       -1
 #endif
 
Index: gfx/src/gtk/nsFontMetricsGTK.cpp
===================================================================
RCS file: /cvsroot/mozilla/gfx/src/gtk/nsFontMetricsGTK.cpp,v
retrieving revision 1.280
diff -u -r1.280 nsFontMetricsGTK.cpp
--- gfx/src/gtk/nsFontMetricsGTK.cpp	4 May 2005 01:37:05 -0000	1.280
+++ gfx/src/gtk/nsFontMetricsGTK.cpp	24 Jun 2005 14:52:01 -0000
@@ -270,7 +270,7 @@
     TT_OS2_CPR1_HEBREW, TT_OS2_CPR2_HEBREW };
 // change from  
 // { "ISO-8859-8", SingleByteConvertReverse, 0, 0, 0 };
-// untill we fix the layout and ensure we only call this with pure RTL text
+// until we fix the layout and ensure we only call this with pure RTL text
 static nsFontCharSetInfo ISO88599 =
   { "ISO-8859-9", SingleByteConvert, 0,
     TT_OS2_CPR1_TURKISH, TT_OS2_CPR2_TURKISH };
@@ -870,7 +870,7 @@
 
 //
 // smart quotes (and other special chars) in Asian (double byte)
-// fonts are too large to use is western fonts.
+// fonts are too large to use as Western fonts.
 // Here we define those characters.
 // XXX: This array can (and need, for performance) be made |const| when 
 // GTK port of gfx gets sync'd with Xlib port for multiple device contexts.
Index: gfx/src/x11shared/nsFontDebug.h
===================================================================
RCS file: /cvsroot/mozilla/gfx/src/x11shared/nsFontDebug.h,v
retrieving revision 1.3
diff -u -r1.3 nsFontDebug.h
--- gfx/src/x11shared/nsFontDebug.h	20 Aug 2004 09:11:25 -0000	1.3
+++ gfx/src/x11shared/nsFontDebug.h	24 Jun 2005 14:52:01 -0000
@@ -52,7 +52,7 @@
 #define NS_FONT_DEBUG_FREETYPE_GRAPHICS 0x800
 
 #undef NS_FONT_DEBUG
-#define NS_FONT_DEBUG 1
+//#define NS_FONT_DEBUG 1
 #ifdef NS_FONT_DEBUG
 
 # define DEBUG_PRINTF(x) \
Index: gfx/src/x11shared/nsX11AlphaBlend.cpp
===================================================================
RCS file: /cvsroot/mozilla/gfx/src/x11shared/nsX11AlphaBlend.cpp,v
retrieving revision 1.4
diff -u -r1.4 nsX11AlphaBlend.cpp
--- gfx/src/x11shared/nsX11AlphaBlend.cpp	17 Apr 2004 21:52:34 -0000	1.4
+++ gfx/src/x11shared/nsX11AlphaBlend.cpp	24 Jun 2005 14:52:02 -0000
@@ -48,7 +48,7 @@
 #include "nsAntiAliasedGlyph.h"
 #include "nsX11AlphaBlend.h"
 
-#define ENABLE_X11ALPHA_BLEND_PRINTF 1
+//#define ENABLE_X11ALPHA_BLEND_PRINTF 1
 
 #if ENABLE_X11ALPHA_BLEND_PRINTF
 static PRUint32 gX11AlphaBlendDebug;
Index: gfx/src/xlib/nsImageXlib.cpp
===================================================================
RCS file: /cvsroot/mozilla/gfx/src/xlib/nsImageXlib.cpp,v
retrieving revision 1.48
diff -u -r1.48 nsImageXlib.cpp
--- gfx/src/xlib/nsImageXlib.cpp	8 Mar 2005 03:44:27 -0000	1.48
+++ gfx/src/xlib/nsImageXlib.cpp	24 Jun 2005 14:52:02 -0000
@@ -110,7 +110,7 @@
 
     if (mAlphaPixmap != nsnull) 
     {
-      // The display cant be null.  It gets fetched from the drawing 
+      // The display can't be null.  It gets fetched from the drawing 
       // surface used to create the pixmap.  It gets assigned once
       // in Draw()
       NS_ASSERTION(nsnull != mDisplay,"display is null.");
@@ -447,7 +447,7 @@
   }
 
 #ifdef HAVE_XIE
-  /* XIE seriosly loses scaling images with alpha */
+  /* XIE seriously loses scaling images with alpha */
   if (!mAlphaDepth) {
     /* Draw with XIE */
     PRBool succeeded = PR_FALSE;
Index: js/src/Makefile.in
===================================================================
RCS file: /cvsroot/mozilla/js/src/Makefile.in,v
retrieving revision 3.95
diff -u -r3.95 Makefile.in
--- js/src/Makefile.in	1 Jun 2005 14:26:26 -0000	3.95
+++ js/src/Makefile.in	24 Jun 2005 14:52:35 -0000
@@ -307,7 +307,7 @@
 
 # On OS/2 & win32 we are already linking against fdlibm, so don't bother
 # creating jsmathtemp
-ifeq (,$(filter OS2 WINNT OpenVMS,$(OS_ARCH)))
+ifeq (,$(filter OS2 WINNT OpenVMS riscos,$(OS_ARCH)))
 # special rule for jsmath.o since we want to incrementally link
 # against fdlibm to pull in only what is needed
 # Do this in a single step to avoid dependency problems
@@ -322,7 +322,8 @@
 	$(LD) $(DASH_R) -o $@ $(JSMATH_PRELINK) $(FDLIBM_LIBRARY)
 endif
 else
-	$(LD) $(DASH_R) -o $@ $(JSMATH_PRELINK) $(FDLIBM_LIBRARY)
+	$(AR) rc $(FDLIBM_LIBRARY) $@ $(JSMATH_PRELINK)
+#	$(LD) $(DASH_R) -o $@ $(JSMATH_PRELINK) $(FDLIBM_LIBRARY)
 endif
 	@$(RM) -f $(JSMATH_PRELINK)
 else
Index: js/src/jsfile.c
===================================================================
RCS file: /cvsroot/mozilla/js/src/jsfile.c,v
retrieving revision 3.35
diff -u -r3.35 jsfile.c
--- js/src/jsfile.c	9 Dec 2004 01:32:19 -0000	3.35
+++ js/src/jsfile.c	24 Jun 2005 14:52:39 -0000
@@ -1385,6 +1385,7 @@
     jsval		count, size;
     JSBool      fileInitiallyOpen=JS_FALSE;
 
+puts("file_copyto 1");
     SECURITY_CHECK(cx, NULL, "copyTo", file);   /* may need a second argument!*/
     JSFILE_CHECK_ONE_ARG("copyTo");
     JSFILE_CHECK_NATIVE("copyTo");
@@ -1392,6 +1393,7 @@
     fileInitiallyOpen = file->isOpen;
     JSFILE_CHECK_READ;
 
+puts("file_copyto 2");
     dest = JS_GetStringBytes(JS_ValueToString(cx, argv[0]));
 
     /* make sure we are not reading a file open for writing */
@@ -1401,6 +1403,7 @@
         goto out;
     }
 
+puts("file_copyto 3");
     if (file->handle==NULL){
         JS_ReportErrorNumber(cx, JSFile_GetErrorMessage, NULL,
             JSFILEMSG_OP_FAILED, "open", file->path);
@@ -1409,12 +1412,14 @@
 
     handle = PR_Open(dest, PR_WRONLY|PR_CREATE_FILE|PR_TRUNCATE, 0644);
 
+puts("file_copyto 4");
     if(!handle){
         JS_ReportErrorNumber(cx, JSFile_GetErrorMessage, NULL,
             JSFILEMSG_OP_FAILED, "open", dest);
         goto out;
     }
 
+puts("file_copyto 5");
     if ((size=js_size(cx, file))==JSVAL_VOID) {
         goto out;
     }
@@ -1430,6 +1435,7 @@
               JSFILEMSG_COPY_READ_ERROR, file->path);
         goto out;
     }
+puts("file_copyto 6");
 
     count = INT_TO_JSVAL(PR_Write(handle, buffer, JSVAL_TO_INT(size)));
 
@@ -1447,6 +1453,7 @@
 		if(!file_close(cx, obj, 0, NULL, rval)) goto out;
 	}
 
+puts("file_copyto 7");
     if(PR_Close(handle)!=PR_SUCCESS){
         JS_ReportErrorNumber(cx, JSFile_GetErrorMessage, NULL,
               JSFILEMSG_OP_FAILED, "close", dest);
@@ -1456,6 +1463,7 @@
     *rval = JSVAL_TRUE;
     return JS_TRUE;
 out:
+puts("file_copyto out");
     if(file->isOpen && !fileInitiallyOpen){
         if(PR_Close(file->handle)!=PR_SUCCESS){
             JS_ReportWarning(cx, "Can't close %s, proceeding", file->path);
Index: layout/xul/base/src/nsBoxFrame.cpp
===================================================================
RCS file: /cvsroot/mozilla/layout/xul/base/src/nsBoxFrame.cpp,v
retrieving revision 1.273
diff -u -r1.273 nsBoxFrame.cpp
--- layout/xul/base/src/nsBoxFrame.cpp	28 Apr 2005 23:48:00 -0000	1.273
+++ layout/xul/base/src/nsBoxFrame.cpp	24 Jun 2005 14:52:47 -0000
@@ -48,7 +48,7 @@
 // Boxes layout a bit differently than html. html does a bottom up layout. Where boxes do a top down.
 // 1) First thing a box does it goes out and askes each child for its min, max, and preferred sizes.
 // 2) It then adds them up to determine its size.
-// 3) If the box was asked to layout it self intrinically it will layout its children at their preferred size
+// 3) If the box was asked to layout itself intrinically it will layout its children at their preferred size
 //    otherwise it will layout the child at the size it was told to. It will squeeze or stretch its children if 
 //    Necessary.
 //
Index: modules/libpr0n/decoders/gif/imgContainerGIF.cpp
===================================================================
RCS file: /cvsroot/mozilla/modules/libpr0n/decoders/gif/imgContainerGIF.cpp,v
retrieving revision 1.20
diff -u -r1.20 imgContainerGIF.cpp
--- modules/libpr0n/decoders/gif/imgContainerGIF.cpp	20 Feb 2005 03:21:04 -0000	1.20
+++ modules/libpr0n/decoders/gif/imgContainerGIF.cpp	24 Jun 2005 14:52:49 -0000
@@ -605,7 +605,7 @@
   switch (prevFrameDisposalMethod) {
     case DISPOSE_CLEAR:
       if (needToBlankComposite) {
-        // If we just created the composite, it could have anything in it's
+        // If we just created the composite, it could have anything in its
         // buffers. Clear them
         BlackenFrame(mCompositingFrame);
         SetMaskVisibility(mCompositingFrame, PR_FALSE);
@@ -634,7 +634,7 @@
   }
 
   // Check if the frame we are composing wants the previous image restored afer
-  // it is done. Don't store it (again) if last frame wanted it's image restored
+  // it is done. Don't store it (again) if last frame wanted its image restored
   // too
   if ((nextFrameDisposalMethod == DISPOSE_RESTORE_PREVIOUS) &&
       (prevFrameDisposalMethod != DISPOSE_RESTORE_PREVIOUS)) {
@@ -655,7 +655,7 @@
     CopyFrameImage(mCompositingFrame, mCompositingPrevFrame);
   }
 
-  // blit next frame into it's correct spot
+  // blit next frame into its correct spot
   aNextFrame->DrawTo(mCompositingFrame, nextFrameRect.x, nextFrameRect.y,
                      nextFrameRect.width, nextFrameRect.height);
   // put the mask in
@@ -1085,7 +1085,7 @@
     aDstFrame->UnlockAlphaData();
   }
 
-  // Tell the image that it's data has been updated
+  // Tell the image that its data has been updated
   nsCOMPtr<nsIInterfaceRequestor> ireq(do_QueryInterface(aDstFrame));
   if (!ireq)
     return PR_FALSE;
Index: modules/plugin/base/src/nsPluginsDirUnix.cpp
===================================================================
RCS file: /cvsroot/mozilla/modules/plugin/base/src/nsPluginsDirUnix.cpp,v
retrieving revision 1.37
diff -u -r1.37 nsPluginsDirUnix.cpp
--- modules/plugin/base/src/nsPluginsDirUnix.cpp	17 Jun 2005 17:02:05 -0000	1.37
+++ modules/plugin/base/src/nsPluginsDirUnix.cpp	24 Jun 2005 14:52:50 -0000
@@ -168,7 +168,7 @@
         PRBool prefSonameListIsSet = PR_TRUE;
         res = prefs->GetCharPref(sonamesListFromPref, &sonameList);
         if (!sonameList) {
-            // pref is not set, lets use hardcoded list
+            // pref is not set, let's use hardcoded list
             prefSonameListIsSet = PR_FALSE;
             sonameList = PL_strdup(DEFAULT_EXTRA_LIBS_LIST);
         }
Index: netwerk/protocol/http/src/nsHttpHandler.cpp
===================================================================
RCS file: /cvsroot/mozilla/netwerk/protocol/http/src/nsHttpHandler.cpp,v
retrieving revision 1.112
diff -u -r1.112 nsHttpHandler.cpp
--- netwerk/protocol/http/src/nsHttpHandler.cpp	2 Jun 2005 23:17:50 -0000	1.112
+++ netwerk/protocol/http/src/nsHttpHandler.cpp	24 Jun 2005 14:52:52 -0000
@@ -84,6 +84,13 @@
 #include <Gestalt.h>
 #endif
 
+#if defined(XP_RISCOS)
+#include <kernel.h>
+extern "C" {
+  extern _kernel_oserror *__os_byte (int a, int x, int y, int *result);
+}
+#endif
+
 #ifdef DEBUG
 // defined by the socket transport service while active
 extern PRThread *gSocketThread;
@@ -672,6 +679,24 @@
         mOscpu.AssignLiteral("PPC Mac OS X");
     else
         mOscpu.AssignLiteral("PPC");
+#elif defined (XP_RISCOS)
+    {
+      _kernel_oserror *result = __os_byte(0, 0, 0, NULL);
+      char *bracket = strchr(result->errmess, '(');
+
+      mPlatform.AssignLiteral("ChoX11");
+
+      if (!bracket) {
+        mOscpu.AssignLiteral(result->errmess);
+      } else {
+        char *buf = (char *)malloc(bracket - result->errmess);
+
+        strncpy(buf, result->errmess, bracket - result->errmess - 1);
+        buf[bracket - result->errmess - 1] = '\0';
+
+        mOscpu.Assign(buf);
+      }
+    }
 #elif defined (XP_UNIX) || defined (XP_BEOS)
     struct utsname name;
     
Index: parser/htmlparser/src/COtherElements.h
===================================================================
RCS file: /cvsroot/mozilla/parser/htmlparser/src/COtherElements.h,v
retrieving revision 3.73
diff -u -r3.73 COtherElements.h
--- parser/htmlparser/src/COtherElements.h	1 Jun 2005 22:23:12 -0000	3.73
+++ parser/htmlparser/src/COtherElements.h	24 Jun 2005 14:53:02 -0000
@@ -419,13 +419,13 @@
 public:
 
   static CGroupMembers& GetGroup(void) {
-    static CGroupMembers theGroup={0};
+    static CGroupMembers theGroup; //={0};
     theGroup.mBits.mInlineEntity=1;
     return theGroup;
   }
 
   static CGroupMembers& GetContainedGroups(void) {
-    static CGroupMembers theGroup={0};
+    static CGroupMembers theGroup; //={0};
     static PRBool initialized=PR_FALSE;
     if(!initialized) {
       initialized=PR_TRUE;
@@ -682,7 +682,7 @@
 
 
 /**********************************************************
-  This defines the Table block itself, not it's children.
+  This defines the Table block itself, not its children.
  **********************************************************/
 
 class CTableElement: public CElement {
@@ -764,7 +764,7 @@
         AutoGenerateStructure(theTags,aContext,aSink);
       }
       
-      //pop the current state and restore it's predecessor, if any...
+      //pop the current state and restore its predecessor, if any...
       CTableState *theState=aContext->mTableStates;      
       aContext->mTableStates=theState->mPrevious; 
       delete theState;
@@ -773,7 +773,7 @@
   }
  
   /**********************************************************
-    Table handles the opening of it's own children
+    Table handles the opening of its own children
    **********************************************************/
   virtual nsresult HandleStartToken(nsCParserNode* aNode,eHTMLTags aTag,nsDTDContext* aContext,nsIHTMLContentSink* aSink) {
     nsresult result=NS_OK;
@@ -842,7 +842,7 @@
   }
 
   /**********************************************************
-    Table handles the closing of it's own children
+    Table handles the closing of its own children
    **********************************************************/
   virtual nsresult HandleEndToken(nsCParserNode* aNode,eHTMLTags aTag,nsDTDContext* aContext,nsIHTMLContentSink* aSink) {
     nsresult result=NS_OK;
@@ -912,7 +912,7 @@
 };
 
 /**********************************************************
-  This defines the Table block itself, not it's children.
+  This defines the Table block itself, not its children.
  **********************************************************/
 
 class CTableRowElement: public CElement {
@@ -1196,7 +1196,7 @@
   }
 
   /**********************************************************
-    Textcontainer handles the opening of it's own children
+    Textcontainer handles the opening of its own children
    **********************************************************/
   virtual nsresult HandleStartToken(nsIParserNode* aNode,eHTMLTags aTag,nsDTDContext* aContext,nsIHTMLContentSink* aSink) {
 
@@ -1276,7 +1276,7 @@
   }
 
   /**********************************************************
-    Title handles the opening of it's own children
+    Title handles the opening of its own children
    **********************************************************/
   virtual nsresult HandleStartToken(nsIParserNode* aNode,eHTMLTags aTag,nsDTDContext* aContext,nsIHTMLContentSink* aSink) {
     nsresult result=NS_OK;
@@ -1480,7 +1480,7 @@
   }
 
   /**********************************************************
-    Pre handles the opening of it's own children
+    Pre handles the opening of its own children
    **********************************************************/
   virtual nsresult HandleStartToken(nsCParserNode* aNode,eHTMLTags aTag,nsDTDContext* aContext,nsIHTMLContentSink* aSink) {
     nsresult result=CElement::HandleStartToken(aNode,aTag,aContext,aSink);
@@ -1489,7 +1489,7 @@
 
 
   /**********************************************************
-    Pre handles the closing of it's own children
+    Pre handles the closing of its own children
    **********************************************************/
   virtual nsresult HandleEndToken(nsCParserNode* aNode,eHTMLTags aTag,nsDTDContext* aContext,nsIHTMLContentSink* aSink) {
     nsresult result=CElement::HandleEndToken(aNode,aTag,aContext,aSink);
@@ -1528,7 +1528,7 @@
   }
 
   /**********************************************************
-    handles the opening of it's own children
+    handles the opening of its own children
    **********************************************************/
   virtual nsresult HandleStartToken(nsCParserNode* aNode,eHTMLTags aTag,nsDTDContext* aContext,nsIHTMLContentSink* aSink) {
     nsresult result=NS_OK;
@@ -1585,7 +1585,7 @@
   }
 
   /**********************************************************
-    fieldset  handles the opening of it's own children
+    fieldset  handles the opening of its own children
    **********************************************************/
   virtual nsresult HandleStartToken(nsCParserNode* aNode,eHTMLTags aTag,nsDTDContext* aContext,nsIHTMLContentSink* aSink) {
     nsresult result=NS_OK;
@@ -1640,7 +1640,7 @@
 
 
   /**********************************************************
-    Toplevel handles the opening of it's own children
+    Toplevel handles the opening of its own children
    **********************************************************/
   virtual nsresult HandleStartToken(  nsCParserNode* aNode,
                                       eHTMLTags aTag,
@@ -1659,7 +1659,7 @@
   }
 
   /**********************************************************
-    TopLevel handles the opening of it's own children
+    TopLevel handles the opening of its own children
    **********************************************************/
   virtual nsresult HandleEndToken(nsCParserNode* aNode,eHTMLTags aTag,nsDTDContext* aContext,nsIHTMLContentSink* aSink) {
 
@@ -1721,7 +1721,7 @@
   }
 
   /**********************************************************
-    HTML handles the opening of it's own children
+    HTML handles the opening of its own children
    **********************************************************/
   nsresult HandleDoctypeDecl( nsIParserNode* aNode,
                               eHTMLTags aTag,
@@ -1744,7 +1744,7 @@
   }
 
   /**********************************************************
-    HTML handles the opening of it's own children
+    HTML handles the opening of its own children
    **********************************************************/
   virtual nsresult HandleStartToken(  nsCParserNode* aNode,
                                       eHTMLTags aTag,
@@ -1832,7 +1832,7 @@
   }
 
   /**********************************************************
-    HTML handles the closing of it's own children
+    HTML handles the closing of its own children
    **********************************************************/
   virtual nsresult HandleEndToken(nsCParserNode* aNode,eHTMLTags aTag,nsDTDContext* aContext,nsIHTMLContentSink* aSink) {
     nsresult result=NS_OK;
@@ -1945,7 +1945,7 @@
   }
 
   /**********************************************************
-    Body handles the opening of it's own children
+    Body handles the opening of its own children
    **********************************************************/
   virtual nsresult HandleStartToken(  nsCParserNode* aNode,
                                       eHTMLTags aTag,
@@ -1972,7 +1972,7 @@
   }
 
   /**********************************************************
-    Body doesnt really need to handle it's own kids, but it's
+    Body doesnt really need to handle its own kids, but it's
     a really convenient break point for debugging purposes.
    **********************************************************/
   virtual nsresult HandleEndToken(nsCParserNode* aNode,eHTMLTags aTag,nsDTDContext* aContext,nsIHTMLContentSink* aSink) {
@@ -2540,7 +2540,7 @@
 
 /******************************************************************************
   Yes, I know it's inconvenient to find this methods here, but it's easier
-  for the compiler -- and making it's life easier is my top priority.
+  for the compiler -- and making its life easier is my top priority.
  ******************************************************************************/
 PRInt32 CElement::FindAutoCloseIndexForStartTag(CElement* anElement,PRInt32 aParentIndex,nsDTDContext* aContext) {
   PRInt32 result=kNotFound;
@@ -2576,7 +2576,7 @@
 
 /******************************************************************************
   Yes, I know it's inconvenient to find this methods here, but it's easier
-  for the compiler -- and making it's life easier is my top priority.
+  for the compiler -- and making its life easier is my top priority.
  ******************************************************************************/
 PRBool CElement::CanBeClosedByEndTag(CElement* anElement,nsDTDContext* aContext) {
   PRBool result=PR_FALSE;
@@ -2604,7 +2604,7 @@
 
 /******************************************************************************
   Yes, I know it's inconvenient to find this methods here, but it's easier
-  for the compiler -- and making it's life easier is my top priority.
+  for the compiler -- and making its life easier is my top priority.
  ******************************************************************************/
 PRBool CElement::CanContain(CElement* anElement,nsDTDContext* aContext) {
   PRBool result=PR_FALSE;
Index: rdf/base/src/nsRDFParserUtils.cpp
===================================================================
RCS file: /cvsroot/mozilla/rdf/base/src/nsRDFParserUtils.cpp,v
retrieving revision 1.17
diff -u -r1.17 nsRDFParserUtils.cpp
--- rdf/base/src/nsRDFParserUtils.cpp	17 Apr 2004 18:29:58 -0000	1.17
+++ rdf/base/src/nsRDFParserUtils.cpp	24 Jun 2005 14:53:04 -0000
@@ -149,6 +149,7 @@
                 if (cp - cbuf > 5) {
                     continue;
                 }
+#undef atoi
                 PRInt32 ch = PRInt32( ::atoi(cbuf) );
                 if (ch > 65535) {
                     continue;
Index: security/coreconf/config.mk
===================================================================
RCS file: /cvsroot/mozilla/security/coreconf/config.mk,v
retrieving revision 1.17
diff -u -r1.17 config.mk
--- security/coreconf/config.mk	25 Apr 2004 15:02:17 -0000	1.17
+++ security/coreconf/config.mk	24 Jun 2005 14:53:04 -0000
@@ -63,7 +63,7 @@
 #######################################################################
 
 TARGET_OSES = FreeBSD BSD_OS NetBSD OpenUNIX OS2 QNX Darwin BeOS OpenBSD \
-              OpenVMS AIX
+              OpenVMS AIX riscos
 
 ifeq (,$(filter-out $(TARGET_OSES),$(OS_TARGET)))
 include $(CORE_DEPTH)/coreconf/$(OS_TARGET).mk
Index: security/coreconf/nsinstall/nsinstall.c
===================================================================
RCS file: /cvsroot/mozilla/security/coreconf/nsinstall/nsinstall.c,v
retrieving revision 1.8
diff -u -r1.8 nsinstall.c
--- security/coreconf/nsinstall/nsinstall.c	25 Apr 2004 15:02:18 -0000	1.8
+++ security/coreconf/nsinstall/nsinstall.c	24 Jun 2005 14:53:04 -0000
@@ -58,7 +58,7 @@
 
 #define HAVE_LCHOWN
 
-#if defined(AIX) || defined(BSDI) || defined(HPUX) || defined(LINUX) || defined(SUNOS4) || defined(SCO) || defined(UNIXWARE) || defined(VMS) || defined(NTO) || defined(DARWIN) || defined(BEOS)
+#if defined(AIX) || defined(BSDI) || defined(HPUX) || defined(LINUX) || defined(SUNOS4) || defined(SCO) || defined(UNIXWARE) || defined(VMS) || defined(NTO) || defined(DARWIN) || defined(BEOS) || defined(__riscos__)
 #undef HAVE_LCHOWN
 #endif
 
@@ -68,9 +68,9 @@
 #undef HAVE_FCHMOD
 #endif
 
-#ifdef LINUX
+/*#if defined(LINUX)*/
 #include <getopt.h>
-#endif
+/*#endif*/
 
 #if defined(SCO) || defined(UNIXWARE) || defined(SNI) || defined(NCR) || defined(NEC)
 #if !defined(S_ISLNK) && defined(S_IFLNK)
Index: security/manager/ssl/src/nsKeygenHandler.cpp
===================================================================
RCS file: /cvsroot/mozilla/security/manager/ssl/src/nsKeygenHandler.cpp,v
retrieving revision 1.33
diff -u -r1.33 nsKeygenHandler.cpp
--- security/manager/ssl/src/nsKeygenHandler.cpp	17 Jun 2004 00:13:16 -0000	1.33
+++ security/manager/ssl/src/nsKeygenHandler.cpp	24 Jun 2005 14:53:05 -0000
@@ -95,6 +95,7 @@
     { 0, }
 };
 
+#if 0
 DERTemplate SECAlgorithmIDTemplate[] = {
     { DER_SEQUENCE,
 	  0, NULL, sizeof(SECAlgorithmID) },
@@ -112,6 +113,7 @@
     { SEC_ASN1_INTEGER, offsetof(PQGParams,base) },
     { 0, }
 };
+#endif
 
 
 static NS_DEFINE_IID(kIDOMHTMLSelectElementIID, NS_IDOMHTMLSELECTELEMENT_IID);
Index: security/nss/lib/freebl/unix_rand.c
===================================================================
RCS file: /cvsroot/mozilla/security/nss/lib/freebl/unix_rand.c,v
retrieving revision 1.17
diff -u -r1.17 unix_rand.c
--- security/nss/lib/freebl/unix_rand.c	10 May 2005 21:14:51 -0000	1.17
+++ security/nss/lib/freebl/unix_rand.c	24 Jun 2005 14:53:05 -0000
@@ -34,6 +34,8 @@
  *
  * ***** END LICENSE BLOCK ***** */
 
+#define __USE_BSD
+
 #include <stdio.h>
 #include <string.h>
 #include <signal.h>
@@ -81,7 +83,7 @@
 }
 
 #if defined(SCO) || defined(UNIXWARE) || defined(BSDI) || defined(FREEBSD) \
-    || defined(NETBSD) || defined(NTO) || defined(DARWIN) || defined(OPENBSD)
+    || defined(NETBSD) || defined(NTO) || defined(DARWIN) || defined(OPENBSD) || defined(__riscos__)
 #include <sys/times.h>
 
 #define getdtablesize() sysconf(_SC_OPEN_MAX)
Index: security/nss/lib/softoken/Makefile
===================================================================
RCS file: /cvsroot/mozilla/security/nss/lib/softoken/Makefile,v
retrieving revision 1.5
diff -u -r1.5 Makefile
--- security/nss/lib/softoken/Makefile	25 Apr 2004 15:03:16 -0000	1.5
+++ security/nss/lib/softoken/Makefile	24 Jun 2005 14:53:06 -0000
@@ -78,6 +78,8 @@
 
 export:: private_export
 
+CFLAGS += -I/usr/src/mozilla/dist/include/dbm
+
 # On AIX 4.3, IBM xlC_r compiler (version 3.6.6) cannot compile
 # pkcs11c.c in 64-bit mode for unknown reasons.  A workaround is
 # to compile it with optimizations turned on.  (Bugzilla bug #63815)
Index: security/nss/lib/softoken/pcertdb.c
===================================================================
RCS file: /cvsroot/mozilla/security/nss/lib/softoken/pcertdb.c,v
retrieving revision 1.49
diff -u -r1.49 pcertdb.c
--- security/nss/lib/softoken/pcertdb.c	29 Mar 2005 18:21:18 -0000	1.49
+++ security/nss/lib/softoken/pcertdb.c	24 Jun 2005 14:53:07 -0000
@@ -3985,6 +3985,11 @@
 #define NO_CREATE	(O_RDWR | O_CREAT | O_TRUNC)
 #endif
 
+DB *dbopen(const char *file, int flags, int mode, DBTYPE type, const void *openinfo)
+{
+  return __db185_open_4001(file, flags, mode, type, openinfo);
+}
+
 /*
  * open an old database that needs to be updated
  */
Index: toolkit/profile/src/nsToolkitProfileService.cpp
===================================================================
RCS file: /cvsroot/mozilla/toolkit/profile/src/nsToolkitProfileService.cpp,v
retrieving revision 1.10
diff -u -r1.10 nsToolkitProfileService.cpp
--- toolkit/profile/src/nsToolkitProfileService.cpp	15 Jun 2005 03:52:48 -0000	1.10
+++ toolkit/profile/src/nsToolkitProfileService.cpp	24 Jun 2005 14:53:17 -0000
@@ -305,11 +305,18 @@
 {
     nsresult rv;
 
+//    mDirectory = aDirectory;
+//    return 0;
     rv = mLock.Lock(aDirectory, aUnlocker);
 
     if (NS_SUCCEEDED(rv)) {
         mDirectory = aDirectory;
         mLocalDirectory = aLocalDirectory;
+    } else {
+      printf("profile lock failed\n");
+        mDirectory = aDirectory;
+        mLocalDirectory = aLocalDirectory;
+      return 0;  
     }
 
     return rv;
@@ -704,7 +711,7 @@
             rv = profileDefaultsDir->CopyToNative(profileDirParent,
                                                   profileDirName);
         if (NS_FAILED(rv)) {
-            // if copying failed, lets just ensure that the profile directory exists.
+            // if copying failed, let's just ensure that the profile directory exists.
             rv = rootDir->Create(nsIFile::DIRECTORY_TYPE, 0700);
             NS_ENSURE_SUCCESS(rv, rv);
         }
Index: toolkit/xre/nsAppRunner.cpp
===================================================================
RCS file: /cvsroot/mozilla/toolkit/xre/nsAppRunner.cpp,v
retrieving revision 1.98
diff -u -r1.98 nsAppRunner.cpp
--- toolkit/xre/nsAppRunner.cpp	20 Jun 2005 23:18:00 -0000	1.98
+++ toolkit/xre/nsAppRunner.cpp	24 Jun 2005 14:53:19 -0000
@@ -1177,9 +1177,20 @@
   if (_execv(exePath.get(), gRestartArgv) == -1)
     return NS_ERROR_FAILURE;
 #elif defined(XP_UNIX)
+  printf("LaunchChild: %s\n", exePath.get());
+
+  {
+    int arg = 0;
+
+    while (gRestartArgv[arg]) {
+      printf("arg: %s\n", gRestartArgv[arg++]);
+    }
+  }
+
   if (execv(exePath.get(), gRestartArgv) == -1)
     return NS_ERROR_FAILURE;
 #else
+
   PRProcess* process = PR_CreateProcess(exePath.get(), gRestartArgv,
                                         nsnull, nsnull);
   if (!process) return NS_ERROR_FAILURE;
@@ -1405,15 +1416,20 @@
   *aResult = nsnull;
   *aStartOffline = PR_FALSE;
 
+puts("profile1");
   arg = PR_GetEnv("XRE_START_OFFLINE");
   if ((arg && *arg) || CheckArg("offline"))
     *aStartOffline = PR_TRUE;
+puts("profile1a");
 
   arg = PR_GetEnv("XRE_PROFILE_PATH");
+puts("profile1b");
   if (arg && *arg) {
     nsCOMPtr<nsILocalFile> lf;
+puts("profile1bb");
     rv = NS_NewNativeLocalFile(nsDependentCString(arg), PR_TRUE,
                                getter_AddRefs(lf));
+puts("profile1bc");
     NS_ENSURE_SUCCESS(rv, rv);
 
     nsCOMPtr<nsILocalFile> localDir;
@@ -1428,6 +1444,7 @@
 
     return NS_LockProfilePath(lf, localDir, nsnull, aResult);
   }
+puts("profile2");
 
   if (CheckArg("migration"))
     gDoMigration = PR_TRUE;
@@ -1453,6 +1470,7 @@
     return ProfileLockedDialog(lf, lf, unlocker, aNative, aResult);
   }
 
+puts("profile3");
   nsCOMPtr<nsIToolkitProfileService> profileSvc;
   rv = NS_NewToolkitProfileService(getter_AddRefs(profileSvc));
   NS_ENSURE_SUCCESS(rv, rv);
@@ -1462,9 +1480,11 @@
     PR_fprintf(PR_STDERR, "Error: argument -createprofile requires a profile name\n");
     return NS_ERROR_FAILURE;
   }
+puts("profile4");
   if (ar) {
     nsCOMPtr<nsIToolkitProfile> profile;
 
+puts("profile4a");
     const char* delim = strchr(arg, ' ');
     if (delim) {
       nsCOMPtr<nsILocalFile> lf;
@@ -1483,6 +1503,7 @@
       rv = profileSvc->CreateProfile(nsnull, nsnull, nsDependentCString(arg),
                                      getter_AddRefs(profile));
     }
+puts("profile4b");
     if (NS_SUCCEEDED(rv)) {
       rv = NS_ERROR_ABORT;
       PR_fprintf(PR_STDERR, "Success: created profile '%s'\n", arg);
@@ -1491,6 +1512,7 @@
 
     // XXXben need to ensure prefs.js exists here so the tinderboxes will
     //        not go orange.
+puts("profile4c");
     nsCOMPtr<nsILocalFile> prefsJSFile;
     profile->GetRootDir(getter_AddRefs(prefsJSFile));
     prefsJSFile->AppendNative(NS_LITERAL_CSTRING("prefs.js"));
@@ -1502,22 +1524,28 @@
 
     return rv;
   }
+puts("profile5");
 
   PRUint32 count;
   rv = profileSvc->GetProfileCount(&count);
   NS_ENSURE_SUCCESS(rv, rv);
+puts("profile5a");
 
   if (gAppData->flags & NS_XRE_ENABLE_PROFILE_MIGRATOR) {
     arg = PR_GetEnv("XRE_IMPORT_PROFILES");
+puts("profile5b");
     if (!count && (!arg || !*arg)) {
+puts("profile5c");
       return ImportProfiles(profileSvc, aNative);
     }
   }
 
+puts("profile6");
   ar = CheckArg("p", &arg);
   if (ar == ARG_BAD) {
     return ShowProfileManager(profileSvc, aNative);
   }
+puts("profile7");
   if (ar) {
     nsCOMPtr<nsIToolkitProfile> profile;
     rv = profileSvc->GetProfileByName(nsDependentCString(arg),
@@ -1540,9 +1568,11 @@
                                  aNative, aResult);
     }
 
+puts("profile8");
     return ShowProfileManager(profileSvc, aNative);
   }
 
+puts("profile9");
   if (CheckArg("profilemanager")) {
     return ShowProfileManager(profileSvc, aNative);
   }
@@ -1550,6 +1580,7 @@
   if (!count) {
     gDoMigration = PR_TRUE;
 
+puts("profile10");
     // create a default profile
     nsCOMPtr<nsIToolkitProfile> profile;
     nsresult rv = profileSvc->CreateProfile(nsnull, // choose a default dir for us
@@ -1568,29 +1599,36 @@
   if (count > 1)
     profileSvc->GetStartWithLastProfile(&useDefault);
 
+puts("profile11");
   if (useDefault) {
     nsCOMPtr<nsIToolkitProfile> profile;
     // GetSelectedProfile will auto-select the only profile if there's just one
     profileSvc->GetSelectedProfile(getter_AddRefs(profile));
+puts("profile12");
     if (profile) {
       nsCOMPtr<nsIProfileUnlocker> unlocker;
       rv = profile->Lock(getter_AddRefs(unlocker), aResult);
+puts("profile13");
       if (NS_SUCCEEDED(rv))
         return NS_OK;
 
+puts("profile14");
       nsCOMPtr<nsILocalFile> profileDir;
       rv = profile->GetRootDir(getter_AddRefs(profileDir));
       NS_ENSURE_SUCCESS(rv, rv);
+puts("profile15");
 
       nsCOMPtr<nsILocalFile> profileLocalDir;
       rv = profile->GetRootDir(getter_AddRefs(profileLocalDir));
       NS_ENSURE_SUCCESS(rv, rv);
+puts("profile16");
 
       return ProfileLockedDialog(profileDir, profileLocalDir, unlocker,
                                  aNative, aResult);
     }
   }
 
+puts("profile7");
   return ShowProfileManager(profileSvc, aNative);
 }
 
@@ -1739,6 +1777,7 @@
 {
   nsresult rv;
   NS_TIMELINE_MARK("enter main");
+puts("main 1");
 
 #ifdef DEBUG
   if (PR_GetEnv("XRE_MAIN_BREAK"))
@@ -1763,9 +1802,11 @@
 #if defined(XP_UNIX) || defined(XP_BEOS)
   InstallUnixSignalHandlers(argv[0]);
 #endif
+puts("main 2");
 
   // Unbuffer stdout, needed for tinderbox tests.
   setbuf(stdout, 0);
+puts("main 3");
 
 #if defined(FREEBSD)
   // Disable all SIGFPE's on FreeBSD, as it has non-IEEE-conformant fp
@@ -1773,6 +1814,7 @@
   // the JS engine.  See bugzilla bug 9967 details.
   fpsetmask(0);
 #endif
+puts("main 4");
 
   gArgc = argc;
   gArgv = argv;
@@ -1792,6 +1834,7 @@
   gRestartArgv = (char**) malloc(sizeof(char*) * (argc + 1));
   if (!gRestartArgv) return 1;
 
+puts("main 5");
   int i;
   for (i = 0; i < argc; ++i) {
 #if defined(XP_WIN)
@@ -1836,6 +1879,7 @@
   gArgc = argc = NS_TraceMallocStartupArgs(argc, argv);
 #endif
 
+puts("main 6");
   nsXREDirProvider dirProvider;
   {
     rv = dirProvider.Initialize(gAppData->directory);
@@ -1843,6 +1887,7 @@
       return 1;
   }
 
+puts("main 7");
   // Check for -register, which registers chrome and then exits immediately.
   if (CheckArg("register")) {
     ScopedXPCOMStartup xpcom;
@@ -1903,6 +1948,7 @@
 // XXXtimeless fix me! How do we get a Display from here to nsAppShell.cpp ?
 // #endif
     
+puts("main 8");
   // Call the code to install our handler
 #ifdef MOZ_JPROF
   setupProfilingStuff();
@@ -1919,6 +1965,7 @@
   if (NS_FAILED(rv) || !canRun) {
     return 1;
   }
+puts("main 9");
 
   //----------------------------------------------------------------
   // We need to check if a previous installation occured and
@@ -1962,23 +2009,29 @@
       return 0;
   }
 #endif
+puts("main 10");
 
   nsCOMPtr<nsIProfileLock> profileLock;
   PRBool startOffline = PR_FALSE;
 
+puts("main 10a");
   rv = SelectProfile(getter_AddRefs(profileLock), nativeApp, &startOffline);
   if (rv == NS_ERROR_LAUNCHED_CHILD_PROCESS ||
       rv == NS_ERROR_ABORT) return 0;
+  printf("%d %d\n", rv == NS_ERROR_LAUNCHED_CHILD_PROCESS, rv);
   if (NS_FAILED(rv)) return 1;
 
+puts("main 10b");
   nsCOMPtr<nsILocalFile> profD;
   rv = profileLock->GetDirectory(getter_AddRefs(profD));
   NS_ENSURE_SUCCESS(rv, 1);
 
+puts("main 10c");
   nsCOMPtr<nsILocalFile> profLD;
   rv = profileLock->GetLocalDirectory(getter_AddRefs(profLD));
   NS_ENSURE_SUCCESS(rv, 1);
 
+puts("main 10d");
   rv = dirProvider.SetProfile(profD, profLD);
   NS_ENSURE_SUCCESS(rv, 1);
 
@@ -1995,6 +2048,7 @@
   // Build the version stamp for the running application.
   nsCAutoString version;
   BuildVersion(version);
+puts("main 11");
 
   // Every time a profile is loaded by a build with a different version,
   // it updates the compatibility.ini file saying what version last wrote
@@ -2032,6 +2086,7 @@
     WriteVersion(profD, version);
   }
 
+puts("main 12");
   PRBool needsRestart = PR_FALSE;
   PRBool appInitiatedRestart = PR_FALSE;
 
@@ -2041,13 +2096,19 @@
   {
     // Start the real application
     ScopedXPCOMStartup xpcom;
+puts("main 12.1");
     rv = xpcom.Initialize();
-    NS_ENSURE_SUCCESS(rv, 1); 
+    NS_ENSURE_SUCCESS(rv, 1);
+puts("main 12.2");
     rv = xpcom.DoAutoreg();
+printf("rv: %x\n", rv);
     rv |= xpcom.InitEventQueue();
+printf("rv: %x\n", rv);
     rv |= xpcom.SetWindowCreator(nativeApp);
+printf("rv: %x\n", rv);
     NS_ENSURE_SUCCESS(rv, 1);
 
+puts("main 12a");
     {
       if (startOffline) {
         nsCOMPtr<nsIIOService> io (do_GetService("@mozilla.org/network/io-service;1"));
@@ -2055,6 +2116,7 @@
         io->SetOffline(PR_TRUE);
       }
 
+puts("main 12b");
       {
         NS_TIMELINE_ENTER("startupNotifier");
         nsCOMPtr<nsIObserver> startupNotifier
@@ -2072,6 +2134,7 @@
       // So we can open and close windows during startup
       appStartup->EnterLastWindowClosingSurvivalArea();
 
+puts("main 12c");
       // Profile Migration
       if (gAppData->flags & NS_XRE_ENABLE_PROFILE_MIGRATOR && gDoMigration) {
         gDoMigration = PR_FALSE;
@@ -2082,6 +2145,7 @@
       }
       dirProvider.DoStartup();
 
+puts("main 12c");
       nsCOMPtr<nsICommandLineRunner> cmdLine
         (do_CreateInstance("@mozilla.org/toolkit/command-line;1"));
       NS_ENSURE_TRUE(cmdLine, 1);
@@ -2094,6 +2158,7 @@
                          workingDir, nsICommandLine::STATE_INITIAL_LAUNCH);
       NS_ENSURE_SUCCESS(rv, 1);
 
+puts("main 13");
       /* Special-case services that need early access to the command
          line. */
       nsCOMPtr<nsIObserver> chromeObserver
@@ -2102,22 +2167,29 @@
         chromeObserver->Observe(cmdLine, "command-line-startup", nsnull);
       }
 
+puts("main 14");
       NS_TIMELINE_ENTER("appStartup->CreateHiddenWindow");
       rv = appStartup->CreateHiddenWindow();
       NS_TIMELINE_LEAVE("appStartup->CreateHiddenWindow");
       NS_ENSURE_SUCCESS(rv, 1);
 
+puts("main 15");
       // Extension Compatibility Checking and Startup
       if (gAppData->flags & NS_XRE_ENABLE_EXTENSION_MANAGER) {
         nsCOMPtr<nsIExtensionManager> em(do_GetService("@mozilla.org/extensions/manager;1"));
+puts("main 15a");
         NS_ENSURE_TRUE(em, 1);
 
+puts("main 15b");
         if (CheckArg("install-global-extension") || CheckArg("install-global-theme")) {
           // Do the required processing and then shut down.
+puts("main 15c");
           em->HandleCommandLineArgs(cmdLine);
+puts("main 15d");
           return 0;
         }
 
+puts("main 16");
         if (upgraded) {
           rv = em->CheckForMismatches(&needsRestart);
           if (NS_FAILED(rv)) {
@@ -2126,6 +2198,7 @@
           }
         }
 
+printf("upgrade 1: %d restart: %d\n", upgraded, needsRestart);
         if (!upgraded || !needsRestart)
           em->Start(cmdLine, &needsRestart);
       }
@@ -2140,6 +2213,9 @@
         needsRestart = upgraded = PR_FALSE;
       }
 
+printf("upgrade 2: %d restart: %d\n", upgraded, needsRestart);
+      needsRestart = upgraded = PR_FALSE;
+
       if (!upgraded && !needsRestart) {
         SaveStateForAppInitiatedRestart();
 
@@ -2247,6 +2323,7 @@
 
     profileLock->Unlock();
   }
+puts("main 14");
 
   // Restart the app after XPCOM has been shut down cleanly. 
   if (needsRestart) {
@@ -2273,8 +2350,7 @@
     PR_SetEnv(kEnvVar1);
     PR_SetEnv(kEnvVar2);
 
-    rv = LaunchChild(nativeApp, appInitiatedRestart);
-    return rv == NS_ERROR_LAUNCHED_CHILD_PROCESS ? 0 : 1;
+    return LaunchChild(nativeApp) == NS_ERROR_LAUNCHED_CHILD_PROCESS ? 0 : 1;
   }
 
   return NS_FAILED(rv) ? 1 : 0;
Index: toolkit/xre/nsAppRunner.h
===================================================================
RCS file: /cvsroot/mozilla/toolkit/xre/nsAppRunner.h,v
retrieving revision 1.16
diff -u -r1.16 nsAppRunner.h
--- toolkit/xre/nsAppRunner.h	13 Jun 2005 16:45:21 -0000	1.16
+++ toolkit/xre/nsAppRunner.h	24 Jun 2005 14:53:19 -0000
@@ -44,7 +44,7 @@
 #elif defined(CCHMAXPATH)
 #define MAXPATHLEN CCHMAXPATH
 #else
-#define MAXPATHLEN 1024
+#define MAXPATHLEN 256
 #endif
 #endif
 
Index: widget/src/xlib/nsClipboard.cpp
===================================================================
RCS file: /cvsroot/mozilla/widget/src/xlib/nsClipboard.cpp,v
retrieving revision 1.28
diff -u -r1.28 nsClipboard.cpp
--- widget/src/xlib/nsClipboard.cpp	10 Jun 2004 21:10:11 -0000	1.28
+++ widget/src/xlib/nsClipboard.cpp	24 Jun 2005 14:53:22 -0000
@@ -118,7 +118,7 @@
 
 // This is the callback function for our nsWidget. It is given the
 // XEvent from nsAppShell.
-// FIXME: We _should_ assign mTransferable here depending on if its a 
+// FIXME: We _should_ assign mTransferable here depending on if it's a 
 // selectionrequest
 nsEventStatus PR_CALLBACK nsClipboard::Callback(nsGUIEvent *event) {
   XEvent *ev = (XEvent *)event->nativeMsg;
@@ -219,7 +219,7 @@
 
   // FIXME Need to make sure mTransferable has reference to selectionclipboard.
   // This solves the problem with copying to an external app.
-  // but cannot be sure if its fully correct until menu copy/paste is working.
+  // but cannot be sure if it's fully correct until menu copy/paste is working.
   if (aWhichClipboard == kSelectionClipboard) {
     NS_IF_RELEASE(mTransferable);
     mTransferable = transferable; 
Index: widget/src/xlib/nsWidget.cpp
===================================================================
RCS file: /cvsroot/mozilla/widget/src/xlib/nsWidget.cpp,v
retrieving revision 1.99
diff -u -r1.99 nsWidget.cpp
--- widget/src/xlib/nsWidget.cpp	28 Apr 2005 23:48:28 -0000	1.99
+++ widget/src/xlib/nsWidget.cpp	24 Jun 2005 14:53:24 -0000
@@ -412,25 +412,29 @@
     PRInt32 screenWidth = WidthOfScreen(mScreen);
     PRInt32 screenHeight = HeightOfScreen(mScreen);
 
+    printf("popup: %d %d\n", aX, aY);
+
     if (aX >= screenWidth)
       aX = screenWidth - mBounds.width;
     if (aY >= screenHeight)
       aY = screenHeight - mBounds.height;
 
-    aRect.x = aX;
-    aRect.y = aY;
-
-    if (mParentWidget) {
-      mParentWidget->WidgetToScreen(aRect, transRect);
-    } else if (mParentWindow) {
-      Window child;
-      XTranslateCoordinates(mDisplay, mParentWindow,
-                            XRootWindowOfScreen(mScreen),
-                            aX, aY, &transRect.x, &transRect.y,
-                            &child);
-    }
-    aX = transRect.x;
-    aY = transRect.y;
+//    aRect.x = aX;
+//    aRect.y = aY;
+//
+//    if (mParentWidget) {
+//      puts("parentWidget");
+//      mParentWidget->WidgetToScreen(aRect, transRect);
+//    } else if (mParentWindow) {
+//      Window child;
+//      puts("parentWindow");
+//      XTranslateCoordinates(mDisplay, mParentWindow,
+//                            XRootWindowOfScreen(mScreen),
+//                            aX, aY, &transRect.x, &transRect.y,
+//                            &child);
+//    }
+//    aX = transRect.x;
+//    aY = transRect.y;
   }
 
   mRequestedSize.x = aX;
Index: widget/src/xlib/nsWindow.cpp
===================================================================
RCS file: /cvsroot/mozilla/widget/src/xlib/nsWindow.cpp,v
retrieving revision 1.70
diff -u -r1.70 nsWindow.cpp
--- widget/src/xlib/nsWindow.cpp	28 Apr 2005 23:48:28 -0000	1.70
+++ widget/src/xlib/nsWindow.cpp	24 Jun 2005 14:53:24 -0000
@@ -678,7 +678,7 @@
     Invalidate(bounds, PR_TRUE);
   }
 
-  // If we are obscurred by another window we have to update those areas
+  // If we are obscured by another window we have to update those areas
   // which were not copied with the XCopyArea function.
 
   if (mVisibility == VisibilityPartiallyObscured)
Index: xpcom/build/Makefile.in
===================================================================
RCS file: /cvsroot/mozilla/xpcom/build/Makefile.in,v
retrieving revision 1.85
diff -u -r1.85 Makefile.in
--- xpcom/build/Makefile.in	1 Apr 2005 18:22:52 -0000	1.85
+++ xpcom/build/Makefile.in	24 Jun 2005 14:53:24 -0000
@@ -155,7 +155,7 @@
 		-DEXPORT_XPTC_API \
 		-DEXPORT_XPTI_API
 
-EXTRA_DSO_LDOPTS += $(NSPR_LIBS)
+#EXTRA_DSO_LDOPTS += $(NSPR_LIBS)
 
 ifdef GC_LEAK_DETECTOR
 DEFINES += -DGC_LEAK_DETECTOR
Index: xpcom/io/nsLocalFileUnix.h
===================================================================
RCS file: /cvsroot/mozilla/xpcom/io/nsLocalFileUnix.h,v
retrieving revision 1.24
diff -u -r1.24 nsLocalFileUnix.h
--- xpcom/io/nsLocalFileUnix.h	18 Apr 2004 14:18:14 -0000	1.24
+++ xpcom/io/nsLocalFileUnix.h	24 Jun 2005 14:53:24 -0000
@@ -52,6 +52,8 @@
 #include "nsString.h"
 #include "nsReadableUtils.h"
 
+#undef HAVE_SYS_STATVFS_H
+#undef HAVE_SYS_STATFS_H
 /** 
  *  we need these for statfs()
  */
Index: xpcom/obsolete/nsFileSpecUnix.cpp
===================================================================
RCS file: /cvsroot/mozilla/xpcom/obsolete/nsFileSpecUnix.cpp,v
retrieving revision 1.9
diff -u -r1.9 nsFileSpecUnix.cpp
--- xpcom/obsolete/nsFileSpecUnix.cpp	16 Mar 2005 20:07:34 -0000	1.9
+++ xpcom/obsolete/nsFileSpecUnix.cpp	24 Jun 2005 14:53:25 -0000
@@ -51,6 +51,9 @@
 #include "prio.h"   /* for PR_Rename */
 #include "nsAutoBuffer.h"
 
+#undef HAVE_SYS_STATVFS_H
+#undef HAVE_SYS_VFS_H
+
 #if defined(_SCO_DS)
 #define _SVID3  /* for statvfs.h */
 #endif
Index: xpcom/reflect/xptcall/src/md/unix/Makefile.in
===================================================================
RCS file: /cvsroot/mozilla/xpcom/reflect/xptcall/src/md/unix/Makefile.in,v
retrieving revision 1.80
diff -u -r1.80 Makefile.in
--- xpcom/reflect/xptcall/src/md/unix/Makefile.in	15 Jun 2005 08:32:21 -0000	1.80
+++ xpcom/reflect/xptcall/src/md/unix/Makefile.in	24 Jun 2005 14:53:25 -0000
@@ -161,6 +161,13 @@
 CPPSRCS		:= xptcinvoke_arm_netbsd.cpp xptcstubs_arm_netbsd.cpp
 endif
 endif
+#
+# RISC OS
+#
+ifeq ($(OS_ARCH),riscos)
+CPPSRCS		:= xptcinvoke_arm.cpp xptcstubs_arm_riscos.cpp
+CXXFLAGS += -O2
+endif
 
 ######################################################################
 # HPPA
Index: xpcom/reflect/xptcall/src/md/unix/xptcinvoke_arm.cpp
===================================================================
RCS file: /cvsroot/mozilla/xpcom/reflect/xptcall/src/md/unix/xptcinvoke_arm.cpp,v
retrieving revision 1.7
diff -u -r1.7 xptcinvoke_arm.cpp
--- xpcom/reflect/xptcall/src/md/unix/xptcinvoke_arm.cpp	18 Apr 2004 14:18:18 -0000	1.7
+++ xpcom/reflect/xptcall/src/md/unix/xptcinvoke_arm.cpp	24 Jun 2005 14:53:32 -0000
@@ -37,9 +37,10 @@
 
 /* Platform specific code to invoke XPCOM methods on native objects */
 
+#include <stdio.h>
 #include "xptcprivate.h"
 
-#if !defined(LINUX) || !defined(__arm__)
+#if (!defined(LINUX) && !defined(__riscos__)) || !defined(__arm__)
 #error "This code is for Linux ARM only. Check that it works on your system, too.\nBeware that this code is highly compiler dependent."
 #endif
 
@@ -144,6 +145,7 @@
 {
     PRUint32 result;
     struct my_params_struct my_params;
+
     my_params.that = that;
     my_params.Index = methodIndex;
     my_params.Count = paramCount;
@@ -177,43 +179,44 @@
  */
  
   __asm__ __volatile__(
-    "ldr	r1, [%1, #12]	\n\t"	/* prepare to call invoke_count_words	*/
-    "ldr	ip, [%1, #16]	\n\t"	/* r0=paramCount, r1=params		*/
-    "ldr	r0, [%1,  #8]	\n\t"
-    "mov	lr, pc		\n\t"	/* call it...				*/
+    "mov	r5, %1		\n\t"
+    "ldr	r1, [r5, #12]	\n\t"	/* prepare to call invoke_count_words	*/
+    "ldr	ip, [r5, #16]	\n\t"	/* r0=paramCount, r1=params		*/
+    "ldr	r4, [r5,  #8]	\n\t"
+    "mov	r0, r4		\n\t"
+    "mov	lr, pc		\n\t"	/* call it				*/
     "mov	pc, ip		\n\t"
-    "mov	r4, r0, lsl #2	\n\t"	/* This is the amount of bytes needed.	*/
-    "sub	sp, sp, r4	\n\t"	/* use stack space for the args...	*/
+    "sub	sp, sp, r4, lsl #2\n\t"	/* Use stack space for the args		*/
     "mov	r0, sp		\n\t"	/* prepare a pointer an the stack	*/
-    "ldr	r1, [%1,  #8]	\n\t"	/* =paramCount				*/
-    "ldr	r2, [%1, #12]	\n\t"	/* =params				*/
-    "ldr	ip, [%1, #20]	\n\t"	/* =invoke_copy_to_stack		*/
+    "ldr	r1, [r5,  #8]	\n\t"	/* =paramCount				*/
+    "ldr	r2, [r5, #12]	\n\t"	/* =params				*/
+    "ldr	ip, [r5, #20]	\n\t"	/* =invoke_copy_to_stack		*/
     "mov	lr, pc		\n\t"	/* copy args to the stack like the	*/
     "mov	pc, ip		\n\t"	/* compiler would.			*/
-    "ldr	r0, [%1]	\n\t"	/* =that				*/
+    "ldr	r0, [r5]	\n\t"	/* =that				*/
     "ldr	r1, [r0, #0]	\n\t"	/* get that->vtable offset		*/
-    "ldr	r2, [%1, #4]	\n\t"
-    "mov	r2, r2, lsl #2	\n\t"	/* a vtable_entry(x)=8 + (4 bytes * x)	*/
+    "ldr	r2, [r5, #4]	\n\t"
+ /*   "mov	r2, r2, lsl #2	\n\t"*/	/* a vtable_entry(x)=8 + (4 bytes * x)	*/
 #if defined(__GXX_ABI_VERSION) && __GXX_ABI_VERSION >= 100 /* G++ V3 ABI */
-    "ldr        ip, [r1, r2]    \n\t"   /* get method adress from vtable        */
+    "ldr        ip, [r1, r2, lsl #2]    \n\t"   /* get method adress from vtable        */
 #else /* non G++ V3 ABI */
-    "add	r2, r2, #8	\n\t"	/* with this compilers			*/
-    "ldr	ip, [r1, r2]	\n\t"	/* get method adress from vtable	*/
+    "add	r2, r2, #2	\n\t"	/* with these compilers			*/
+    "ldr	ip, [r1, r2, lsl #2]	\n\t"	/* get dmethod adress from vtable	*/
 #endif
-    "cmp	r4, #12		\n\t"	/* more than 3 arguments???		*/
+    "cmp	r4, #3		\n\t"	/* more than 3 arguments?		*/
     "ldmgtia	sp!, {r1, r2, r3}\n\t"	/* yes: load arguments for r1-r3	*/
-    "subgt	r4, r4, #12	\n\t"	/*      and correct the stack pointer	*/
+    "subgt	r4, r4, #3	\n\t"	/*      and correct the stack pointer	*/
     "ldmleia	sp, {r1, r2, r3}\n\t"	/* no:  load r1-r3 from stack		*/ 
-    "addle	sp, sp, r4	\n\t"	/*      and restore stack pointer	*/
+    "addle	sp, sp, r4, lsl #2\n\t"	/*      and restore stack pointer	*/
     "movle	r4, #0		\n\t"	/*	a mark for restoring sp		*/
-    "ldr	r0, [%1, #0]	\n\t"	/* get (self)				*/
-    "mov	lr, pc		\n\t"	/* call mathod				*/
+    "ldr	r0, [r5, #0]	\n\t"	/* get (self)				*/
+    "mov	lr, pc		\n\t"	/* call method				*/
     "mov	pc, ip		\n\t"
-    "add	sp, sp, r4	\n\t"	/* restore stack pointer		*/
-    "mov	%0, r0		\n\t"	/* the result...			*/
+    "add	sp, sp, r4, lsl #2\n\t"	/* restore stack pointer		*/
+    "mov	%0, r0		\n\t"	/* the result				*/
     : "=r" (result)
     : "r" (&my_params)
-    : "r0", "r1", "r2", "r3", "r4", "ip", "lr", "sp"
+    : "r0", "r1", "r2", "r3", "r4", "r5", "ip", "lr", "sp"
     );
     
   return result;
