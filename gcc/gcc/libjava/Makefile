# Makefile for GNU Libgcj for the ARM/RISC OS GCC port
# Copyright (c) 2003 GCCSDK Developers
# Written by Nick Burrett <nick@dsvr.net>
#

#include rules.debug

INCLUDES = -I . -I ./include -I../libsupc++ -I ../libstdc++ -I ../libffi/include -I ../gcc -I ../zlib
libobjdir=$(objdir)/gcc-$(VERSION)/libjava
JFLAGS=--encoding=UTF-8 -C -classpath '' -bootclasspath . -d $(libobjdir)
LIBGCJ_CFLAGS = -D__NO_MATH_INLINES -ffloat-store
LIBGCJ_CXXFLAGS = -D__NO_MATH_INLINES -ffloat-store
LIBGCJ_JAVAFLAGS = -ffloat-store
ZIP = $(ux_bin_dir)/fastjar
GCJH_T = $(CROSS_GCJH) -classpath '' -bootclasspath $(libobjdir)
GCJ_T = $(CROSS_GCJ) $(JFLAGS)

.SUFFIXES:

$(libobjdir)/%.class: %.java
	$(CROSS_GCJ) -c $< $(JFLAGS)

$(libobjdir)/%.o: %.cc
	$(CROSS_CXX) -o $@ -c $< $(CROSS_CXXFLAGS) $(INCLUDES) $(CFL)

$(libobjdir)/%.o: %.c
	$(CROSS_CC) -o $@ -c $< $(CROSS_CFLAGS) $(INCLUDES) $(CFL)

ifeq (${STAGE},stage1)
bin_dir = $(ux_bin_dir)
gcc_bin_dir = $(ux_gcc_bin_dir)
else
bin_dir = $(ro_bin_dir)
gcc_bin_dir = $(ro_gcc_bin_dir)
endif

all:	$(ux_gccpkg)/$(gn_share_dir)/libgcj.jar \
	$(ro_gccpkg)/$(gn_share_dir)/jar/libgcj \
	genheaders \
	$(ux_gcc_bin_dir)/$(TRG)/libgcj.o \
	$(ro_gcc_bin_dir)/$(TRG)/o/libgcj


# We purposely don't include the gtk java stuff because it currently does
# not compile.
header_deps:
	rm -f .rules.headers
	for dirs in gcj gnu java javax org; do \
	  find $$dirs -name "*.java" | sed -e "/gtk/d" -e "s/\.\///" -e "s/\.java$$//" | awk '{print $$1 ".h: $$(libobjdir)/" $$1 ".class\n\t$$(GCJH_T) " $$1 }' >>.rules.headers ; \
	done
	echo ".PHONY: genheaders" >>.rules.headers
	echo "genheaders: \\" >>.rules.headers
	for dirs in gcj gnu java javax org; do \
	  find $$dirs -name "*.java" | sed -e "/gtk/d" -e "s/\.\///" -e "s/\.java$$//" | awk '{printf "%s.h ", $$1 }' >>.rules.headers ; \
	done


class_deps:
	find . -name "*.java" | sed -e "s/\.\///" -e "s/\.java$$//" | awk '{print "$$(libobjdir)/" $$1 ".class: " $$1 ".java\n\t$$(GCJ_T) -c $$<" }' >.rules.class

ifneq ($(GEN),gen)
include .rules.class
include .rules.headers
endif

# Install Posix specific classes.  Remove any surplus java files because these
# won't build properly and will screw up our auto-generated 
other:
	cp java/lang/natPosixProcess.cc java/lang/natConcreteProcess.cc
	cp java/io/natFileDescriptorPosix.cc java/io/natFileDescriptor.cc
	cp java/io/natFilePosix.cc java/io/natFile.cc
	rm -f java/lang/EcosProcess.java

lib_org_w3c_dom_la_SOURCES = $(libobjdir)/org/w3c/dom/Attr.class \
$(libobjdir)/org/w3c/dom/CDATASection.class \
$(libobjdir)/org/w3c/dom/CharacterData.class \
$(libobjdir)/org/w3c/dom/Comment.class \
$(libobjdir)/org/w3c/dom/DOMException.class \
$(libobjdir)/org/w3c/dom/DOMImplementation.class \
$(libobjdir)/org/w3c/dom/Document.class \
$(libobjdir)/org/w3c/dom/DocumentFragment.class \
$(libobjdir)/org/w3c/dom/DocumentType.class \
$(libobjdir)/org/w3c/dom/Element.class \
$(libobjdir)/org/w3c/dom/Entity.class \
$(libobjdir)/org/w3c/dom/EntityReference.class \
$(libobjdir)/org/w3c/dom/NamedNodeMap.class \
$(libobjdir)/org/w3c/dom/Node.class \
$(libobjdir)/org/w3c/dom/NodeList.class \
$(libobjdir)/org/w3c/dom/Notation.class \
$(libobjdir)/org/w3c/dom/ProcessingInstruction.class \
$(libobjdir)/org/w3c/dom/Text.class \
$(libobjdir)/org/w3c/dom/ranges/DocumentRange.class \
$(libobjdir)/org/w3c/dom/ranges/Range.class \
$(libobjdir)/org/w3c/dom/ranges/RangeException.class \
$(libobjdir)/org/w3c/dom/traversal/DocumentTraversal.class \
$(libobjdir)/org/w3c/dom/traversal/NodeFilter.class \
$(libobjdir)/org/w3c/dom/traversal/NodeIterator.class \
$(libobjdir)/org/w3c/dom/traversal/TreeWalker.class

lib_org_xml_sax_la_SOURCES = $(libobjdir)/org/xml/sax/ext/DeclHandler.class \
$(libobjdir)/org/xml/sax/ext/LexicalHandler.class \
$(libobjdir)/org/xml/sax/helpers/AttributeListImpl.class \
$(libobjdir)/org/xml/sax/helpers/AttributesImpl.class \
$(libobjdir)/org/xml/sax/helpers/DefaultHandler.class \
$(libobjdir)/org/xml/sax/helpers/LocatorImpl.class \
$(libobjdir)/org/xml/sax/helpers/NamespaceSupport.class \
$(libobjdir)/org/xml/sax/helpers/NewInstance.class \
$(libobjdir)/org/xml/sax/helpers/ParserAdapter.class \
$(libobjdir)/org/xml/sax/helpers/ParserFactory.class \
$(libobjdir)/org/xml/sax/helpers/XMLFilterImpl.class \
$(libobjdir)/org/xml/sax/helpers/XMLReaderAdapter.class \
$(libobjdir)/org/xml/sax/helpers/XMLReaderFactory.class \
$(libobjdir)/org/xml/sax/AttributeList.class \
$(libobjdir)/org/xml/sax/Attributes.class \
$(libobjdir)/org/xml/sax/ContentHandler.class \
$(libobjdir)/org/xml/sax/DTDHandler.class \
$(libobjdir)/org/xml/sax/DocumentHandler.class \
$(libobjdir)/org/xml/sax/EntityResolver.class \
$(libobjdir)/org/xml/sax/ErrorHandler.class \
$(libobjdir)/org/xml/sax/HandlerBase.class \
$(libobjdir)/org/xml/sax/InputSource.class \
$(libobjdir)/org/xml/sax/Locator.class \
$(libobjdir)/org/xml/sax/Parser.class \
$(libobjdir)/org/xml/sax/SAXException.class \
$(libobjdir)/org/xml/sax/SAXNotRecognizedException.class \
$(libobjdir)/org/xml/sax/SAXNotSupportedException.class \
$(libobjdir)/org/xml/sax/SAXParseException.class \
$(libobjdir)/org/xml/sax/XMLFilter.class \
$(libobjdir)/org/xml/sax/XMLReader.class

## Header files used when compiling some of the nat* files.
ordinary_nat_headers = $(ordinary_java_source_files:.class=.h) \
	$(built_java_source_files:.class=.h)

inner_nat_headers = java/io/ObjectOutputStream$$PutField.h \
	java/io/ObjectInputStream$$GetField.h \
	java/lang/reflect/Proxy$$ProxyData.h \
	java/lang/reflect/Proxy$$ProxyType.h

nat_headers = $(ordinary_nat_headers) $(inner_nat_headers)

x_nat_headers = $(x_java_source_files:.class=.h)

$(ordinary_nat_headers) $(x_nat_headers)

java/lang/ClassLoader.h: $(libobjdir)/java/lang/ClassLoader.class
	$(CROSS_GCJH) -classpath '' -bootclasspath $(libobjdir) \
		-friend 'jclass _Jv_FindClass (_Jv_Utf8Const *name, java::lang::ClassLoader *loader);' \
		-friend 'void _Jv_RunMain (jclass klass, const char *name, int argc, const char **argv, bool is_jar);' \
		java/lang/ClassLoader

java/lang/Thread.h: $(libobjdir)/java/lang/Thread.class
	$(CROSS_GCJH) -classpath '' -bootclasspath $(libobjdir) \
		-prepend 'class _Jv_JNIEnv;' \
		-prepend '#define _JV_NOT_OWNER 1' \
		-prepend '#define _JV_INTERRUPTED 2' \
		-friend '_Jv_JNIEnv * _Jv_GetCurrentJNIEnv ();' \
		-friend 'void _Jv_SetCurrentJNIEnv (_Jv_JNIEnv *env);' \
		-friend 'void _Jv_ThreadRun (java::lang::Thread* thread);' \
		-friend 'jint _Jv_AttachCurrentThread(java::lang::Thread* thread);' \
		-friend 'java::lang::Thread* _Jv_AttachCurrentThread(jstring name, java::lang::ThreadGroup* group);' \
		-friend 'java::lang::Thread* _Jv_AttachCurrentThreadAsDaemon(jstring name, java::lang::ThreadGroup* group);' \
		-friend 'jint _Jv_DetachCurrentThread ();' \
		java/lang/Thread

java/lang/String.h: $(libobjdir)/java/lang/String.class
	$(CROSS_GCJH) -classpath '' -bootclasspath $(libobjdir) \
	    -friend 'jchar* _Jv_GetStringChars (jstring str);' \
	    -friend 'jstring* _Jv_StringFindSlot (jchar*, jint, jint);' \
	    -friend 'jstring* _Jv_StringGetSlot (jstring);' \
	    -friend 'jstring _Jv_NewStringUtf8Const (_Jv_Utf8Const* str);' \
	    -friend 'jstring _Jv_NewStringLatin1 (const char*, jsize);' \
	    -friend 'jstring _Jv_AllocString (jsize);' \
	    java/lang/String

java/lang/reflect/Constructor.h: $(libobjdir)/java/lang/reflect/Constructor.class
	$(CROSS_GCJH) -classpath '' -bootclasspath $(libobjdir) \
	    -friend 'jmethodID _Jv_FromReflectedConstructor (java::lang::reflect::Constructor *);' \
	    -friend 'class java::lang::Class;' \
	    -friend 'jobject _Jv_JNI_ToReflectedMethod (_Jv_JNIEnv *, jclass, jmethodID, jboolean);' \
	    java/lang/reflect/Constructor

java/lang/reflect/Field.h: $(libobjdir)/java/lang/reflect/Field.class
	$(CROSS_GCJH) -classpath '' -bootclasspath $(libobjdir) \
	    -friend 'jfieldID _Jv_FromReflectedField (java::lang::reflect::Field *);' \
	    -friend 'jobject _Jv_JNI_ToReflectedField (_Jv_JNIEnv*, jclass, jfieldID, jboolean);' \
	    -friend 'class java::lang::Class;' \
	    java/lang/reflect/Field

java/lang/reflect/Method.h: $(libobjdir)/java/lang/reflect/Method.class
	$(CROSS_GCJH) -classpath '' -bootclasspath $(libobjdir) \
	    -friend 'jmethodID _Jv_FromReflectedMethod (java::lang::reflect::Method *);' \
	    -friend 'class java::lang::Class;' \
	    -friend 'class java::io::ObjectInputStream;' \
	    -friend 'jobject _Jv_JNI_ToReflectedMethod (_Jv_JNIEnv *, jclass, jmethodID, jboolean);' \
	    java/lang/reflect/Method

java/lang/reflect/Proxy.h: $(libobjdir)/java/lang/reflect/Proxy.class
	$(CROSS_GCJH) -classpath '' -bootclasspath $(libobjdir) \
	    java/lang/reflect/Proxy

java/lang/reflect/Proxy$$ProxyData.h: $(libobjdir)/java/lang/reflect/Proxy.class
	$(CROSS_GCJH) -classpath '' -bootclasspath $(libobjdir) \
	'java/lang/reflect/Proxy$$ProxyData'

java/lang/reflect/Proxy$$ProxyType.h: $(libobjdir)/java/lang/reflect/Proxy.class
	$(CROSS_GCJH) -classpath '' -bootclasspath $(libobjdir) \
	'java/lang/reflect/Proxy$$ProxyType'

gnu/gcj/runtime/VMClassLoader.h: $(libobjdir)/gnu/gcj/runtime/VMClassLoader.class
	$(CROSS_GCJH) -classpath '' -bootclasspath $(libobjdir) \
	    -friend 'class ::java::lang::ClassLoader;' \
	    gnu/gcj/runtime/VMClassLoader

java/io/ObjectInputStream$$GetField.h: $(libobjdir)/java/io/ObjectInputStream.class
	$(CROSS_GCJH) -classpath '' -bootclasspath $(libobjdir) \
	'java/io/ObjectInputStream$$GetField'

java/io/ObjectOutputStream$$PutField.h: $(libobjdir)/java/io/ObjectOutputStream.class
	$(CROSS_GCJH) -classpath '' -bootclasspath $(libobjdir) \
	'java/io/ObjectOutputStream$$PutField'

## Headers we maintain by hand and which we want to install.
extra_headers = java/lang/Object.h java/lang/Class.h

$(extra_headers) $(srcdir)/java/lang/Object.h $(srcdir)/java/lang/Class.h:
	@:

##
## This section lists all the source files we care about.
##

convert_source_files = \
$(libobjdir)/gnu/gcj/convert/BytesToUnicode.class \
$(libobjdir)/gnu/gcj/convert/Convert.class \
$(libobjdir)/gnu/gcj/convert/Input_8859_1.class \
$(libobjdir)/gnu/gcj/convert/Input_ASCII.class \
$(libobjdir)/gnu/gcj/convert/Input_EUCJIS.class \
$(libobjdir)/gnu/gcj/convert/Input_JavaSrc.class \
$(libobjdir)/gnu/gcj/convert/Input_SJIS.class \
$(libobjdir)/gnu/gcj/convert/Input_UTF8.class	\
$(libobjdir)/gnu/gcj/convert/Input_iconv.class \
$(libobjdir)/gnu/gcj/convert/IOConverter.class \
$(libobjdir)/gnu/gcj/convert/Output_8859_1.class \
$(libobjdir)/gnu/gcj/convert/Output_ASCII.class \
$(libobjdir)/gnu/gcj/convert/Output_EUCJIS.class \
$(libobjdir)/gnu/gcj/convert/Output_JavaSrc.class \
$(libobjdir)/gnu/gcj/convert/Output_SJIS.class \
$(libobjdir)/gnu/gcj/convert/Output_UTF8.class \
$(libobjdir)/gnu/gcj/convert/Output_iconv.class \
$(libobjdir)/gnu/gcj/convert/UnicodeToBytes.class

## List of all .class files for which the .h file is maintained by
## hand.
special_java_source_files = $(libobjdir)/java/lang/Class.class $(libobjdir)/java/lang/Object.class

awt_java_source_files =	\
$(libobjdir)/gnu/awt/LightweightRedirector.class \
$(libobjdir)/gnu/awt/j2d/AbstractGraphicsState.class \
$(libobjdir)/gnu/awt/j2d/DirectRasterGraphics.class \
$(libobjdir)/gnu/awt/j2d/Graphics2DImpl.class \
$(libobjdir)/gnu/awt/j2d/IntegerGraphicsState.class \
$(libobjdir)/gnu/awt/j2d/MappedRaster.class \
$(libobjdir)/gnu/java/awt/BitMaskExtent.class \
$(libobjdir)/gnu/java/awt/Buffers.class \
$(libobjdir)/gnu/java/awt/ComponentDataBlitOp.class \
$(libobjdir)/gnu/java/awt/GLightweightPeer.class \
$(libobjdir)/gnu/java/awt/EventModifier.class \
$(libobjdir)/gnu/java/awt/image/ImageDecoder.class \
$(libobjdir)/gnu/java/awt/image/XBMDecoder.class \
$(libobjdir)/gnu/java/beans/editors/ColorEditor.class	\
$(libobjdir)/gnu/java/beans/editors/FontEditor.class \
$(libobjdir)/gnu/java/beans/editors/NativeBooleanEditor.class	\
$(libobjdir)/gnu/java/beans/editors/NativeByteEditor.class \
$(libobjdir)/gnu/java/beans/editors/NativeDoubleEditor.class \
$(libobjdir)/gnu/java/beans/editors/NativeFloatEditor.class \
$(libobjdir)/gnu/java/beans/editors/NativeIntEditor.class \
$(libobjdir)/gnu/java/beans/editors/NativeLongEditor.class \
$(libobjdir)/gnu/java/beans/editors/NativeShortEditor.class \
$(libobjdir)/gnu/java/beans/editors/StringEditor.class \
$(libobjdir)/gnu/java/beans/info/ComponentBeanInfo.class \
$(libobjdir)/gnu/java/beans/BeanInfoEmbryo.class \
$(libobjdir)/gnu/java/beans/EmptyBeanInfo.class \
$(libobjdir)/gnu/java/beans/ExplicitBeanInfo.class \
$(libobjdir)/gnu/java/beans/IntrospectionIncubator.class \
$(libobjdir)/java/applet/Applet.class \
$(libobjdir)/java/applet/AppletStub.class \
$(libobjdir)/java/applet/AppletContext.class \
$(libobjdir)/java/applet/AudioClip.class \
$(libobjdir)/java/awt/AWTError.class \
$(libobjdir)/java/awt/AWTEvent.class \
$(libobjdir)/java/awt/AWTEventMulticaster.class \
$(libobjdir)/java/awt/AWTException.class \
$(libobjdir)/java/awt/AWTPermission.class \
$(libobjdir)/java/awt/ActiveEvent.class \
$(libobjdir)/java/awt/Adjustable.class \
$(libobjdir)/java/awt/BorderLayout.class \
$(libobjdir)/java/awt/Button.class \
$(libobjdir)/java/awt/Canvas.class \
$(libobjdir)/java/awt/CardLayout.class \
$(libobjdir)/java/awt/Checkbox.class \
$(libobjdir)/java/awt/CheckboxGroup.class \
$(libobjdir)/java/awt/CheckboxMenuItem.class \
$(libobjdir)/java/awt/Choice.class \
$(libobjdir)/java/awt/Color.class \
$(libobjdir)/java/awt/Component.class \
$(libobjdir)/java/awt/ComponentOrientation.class \
$(libobjdir)/java/awt/Container.class \
$(libobjdir)/java/awt/Cursor.class \
$(libobjdir)/java/awt/Dialog.class \
$(libobjdir)/java/awt/Dimension.class \
$(libobjdir)/java/awt/Event.class \
$(libobjdir)/java/awt/EventDispatchThread.class \
$(libobjdir)/java/awt/EventQueue.class \
$(libobjdir)/java/awt/FileDialog.class \
$(libobjdir)/java/awt/FlowLayout.class \
$(libobjdir)/java/awt/Font.class \
$(libobjdir)/java/awt/FontMetrics.class \
$(libobjdir)/java/awt/Frame.class \
$(libobjdir)/java/awt/Graphics.class \
$(libobjdir)/java/awt/Graphics2D.class \
$(libobjdir)/java/awt/GraphicsConfiguration.class \
$(libobjdir)/java/awt/GridBagConstraints.class \
$(libobjdir)/java/awt/GridBagLayout.class \
$(libobjdir)/java/awt/GridLayout.class \
$(libobjdir)/java/awt/IllegalComponentStateException.class \
$(libobjdir)/java/awt/Image.class \
$(libobjdir)/java/awt/Insets.class \
$(libobjdir)/java/awt/ItemSelectable.class \
$(libobjdir)/java/awt/Label.class \
$(libobjdir)/java/awt/LayoutManager.class \
$(libobjdir)/java/awt/LayoutManager2.class \
$(libobjdir)/java/awt/List.class \
$(libobjdir)/java/awt/Menu.class \
$(libobjdir)/java/awt/MenuBar.class \
$(libobjdir)/java/awt/MenuComponent.class \
$(libobjdir)/java/awt/MenuContainer.class \
$(libobjdir)/java/awt/MenuItem.class \
$(libobjdir)/java/awt/MenuShortcut.class \
$(libobjdir)/java/awt/Paint.class \
$(libobjdir)/java/awt/PaintContext.class \
$(libobjdir)/java/awt/Panel.class \
$(libobjdir)/java/awt/Point.class \
$(libobjdir)/java/awt/PopupMenu.class \
$(libobjdir)/java/awt/Rectangle.class \
$(libobjdir)/java/awt/RenderingHints.class \
$(libobjdir)/java/awt/ScrollPane.class \
$(libobjdir)/java/awt/Scrollbar.class \
$(libobjdir)/java/awt/Shape.class \
$(libobjdir)/java/awt/TextArea.class \
$(libobjdir)/java/awt/TextComponent.class \
$(libobjdir)/java/awt/TextField.class \
$(libobjdir)/java/awt/Toolkit.class \
$(libobjdir)/java/awt/Transparency.class \
$(libobjdir)/java/awt/Window.class \
$(libobjdir)/java/awt/color/ColorSpace.class \
$(libobjdir)/java/awt/color/ICC_ColorSpace.class \
$(libobjdir)/java/awt/color/ICC_Profile.class \
$(libobjdir)/java/awt/color/ICC_ProfileGray.class \
$(libobjdir)/java/awt/color/ICC_ProfileRGB.class \
$(libobjdir)/java/awt/color/CMMException.class \
$(libobjdir)/java/awt/color/ProfileDataException.class \
$(libobjdir)/java/awt/datatransfer/Clipboard.class \
$(libobjdir)/java/awt/datatransfer/ClipboardOwner.class \
$(libobjdir)/java/awt/datatransfer/DataFlavor.class \
$(libobjdir)/java/awt/datatransfer/FlavorMap.class \
$(libobjdir)/java/awt/datatransfer/MimeTypeParseException.class \
$(libobjdir)/java/awt/datatransfer/StringSelection.class \
$(libobjdir)/java/awt/datatransfer/SystemFlavorMap.class \
$(libobjdir)/java/awt/datatransfer/Transferable.class \
$(libobjdir)/java/awt/datatransfer/UnsupportedFlavorException.class \
$(libobjdir)/java/awt/datatransfer/FlavorTable.class \
$(libobjdir)/java/awt/event/AWTEventListener.class \
$(libobjdir)/java/awt/event/ActionEvent.class \
$(libobjdir)/java/awt/event/ActionListener.class \
$(libobjdir)/java/awt/event/AdjustmentEvent.class \
$(libobjdir)/java/awt/event/AdjustmentListener.class \
$(libobjdir)/java/awt/event/ComponentAdapter.class \
$(libobjdir)/java/awt/event/ComponentEvent.class \
$(libobjdir)/java/awt/event/ComponentListener.class \
$(libobjdir)/java/awt/event/ContainerAdapter.class \
$(libobjdir)/java/awt/event/ContainerEvent.class \
$(libobjdir)/java/awt/event/ContainerListener.class \
$(libobjdir)/java/awt/event/FocusAdapter.class \
$(libobjdir)/java/awt/event/FocusEvent.class \
$(libobjdir)/java/awt/event/FocusListener.class \
$(libobjdir)/java/awt/event/HierarchyBoundsAdapter.class \
$(libobjdir)/java/awt/event/HierarchyBoundsListener.class \
$(libobjdir)/java/awt/event/HierarchyEvent.class \
$(libobjdir)/java/awt/event/HierarchyListener.class \
$(libobjdir)/java/awt/event/InputEvent.class \
$(libobjdir)/java/awt/event/InputMethodEvent.class \
$(libobjdir)/java/awt/event/InputMethodListener.class \
$(libobjdir)/java/awt/event/InvocationEvent.class \
$(libobjdir)/java/awt/event/ItemEvent.class \
$(libobjdir)/java/awt/event/ItemListener.class \
$(libobjdir)/java/awt/event/KeyAdapter.class \
$(libobjdir)/java/awt/event/KeyEvent.class \
$(libobjdir)/java/awt/event/KeyListener.class \
$(libobjdir)/java/awt/event/MouseAdapter.class \
$(libobjdir)/java/awt/event/MouseEvent.class \
$(libobjdir)/java/awt/event/MouseListener.class \
$(libobjdir)/java/awt/event/MouseMotionAdapter.class \
$(libobjdir)/java/awt/event/MouseMotionListener.class \
$(libobjdir)/java/awt/event/PaintEvent.class \
$(libobjdir)/java/awt/event/TextEvent.class \
$(libobjdir)/java/awt/event/TextListener.class \
$(libobjdir)/java/awt/event/WindowAdapter.class \
$(libobjdir)/java/awt/event/WindowEvent.class \
$(libobjdir)/java/awt/event/WindowListener.class \
$(libobjdir)/java/awt/event/AWTEventListenerProxy.class \
$(libobjdir)/java/awt/event/MouseWheelEvent.class \
$(libobjdir)/java/awt/event/MouseWheelListener.class \
$(libobjdir)/java/awt/event/WindowFocusListener.class \
$(libobjdir)/java/awt/event/WindowStateListener.class \
$(libobjdir)/java/awt/geom/AffineTransform.class \
$(libobjdir)/java/awt/geom/Dimension2D.class \
$(libobjdir)/java/awt/geom/Ellipse2D.class \
$(libobjdir)/java/awt/geom/IllegalPathStateException.class \
$(libobjdir)/java/awt/geom/NoninvertibleTransformException.class \
$(libobjdir)/java/awt/geom/PathIterator.class \
$(libobjdir)/java/awt/geom/Point2D.class \
$(libobjdir)/java/awt/geom/Rectangle2D.class \
$(libobjdir)/java/awt/geom/RectangularShape.class \
$(libobjdir)/java/awt/geom/RoundRectangle2D.class \
$(libobjdir)/java/awt/geom/Line2D.class \
$(libobjdir)/java/awt/geom/Arc2D.class \
$(libobjdir)/java/awt/geom/Area.class \
$(libobjdir)/java/awt/geom/CubicCurve2D.class \
$(libobjdir)/java/awt/geom/FlatteningPathIterator.class \
$(libobjdir)/java/awt/geom/GeneralPath.class \
$(libobjdir)/java/awt/geom/QuadCurve2D.class \
$(libobjdir)/java/awt/image/BufferedImage.class \
$(libobjdir)/java/awt/image/ColorModel.class \
$(libobjdir)/java/awt/image/ComponentColorModel.class \
$(libobjdir)/java/awt/image/ComponentSampleModel.class \
$(libobjdir)/java/awt/image/DataBuffer.class \
$(libobjdir)/java/awt/image/DataBufferByte.class \
$(libobjdir)/java/awt/image/DataBufferInt.class \
$(libobjdir)/java/awt/image/DataBufferUShort.class \
$(libobjdir)/java/awt/image/DirectColorModel.class \
$(libobjdir)/java/awt/image/ImageConsumer.class \
$(libobjdir)/java/awt/image/ImageObserver.class \
$(libobjdir)/java/awt/image/ImageProducer.class \
$(libobjdir)/java/awt/image/IndexColorModel.class \
$(libobjdir)/java/awt/image/PackedColorModel.class \
$(libobjdir)/java/awt/image/Raster.class \
$(libobjdir)/java/awt/image/RasterOp.class \
$(libobjdir)/java/awt/image/SampleModel.class \
$(libobjdir)/java/awt/image/SinglePixelPackedSampleModel.class \
$(libobjdir)/java/awt/image/WritableRaster.class \
$(libobjdir)/java/awt/image/AreaAveragingScaleFilter.class \
$(libobjdir)/java/awt/image/CropImageFilter.class \
$(libobjdir)/java/awt/image/FilteredImageSource.class \
$(libobjdir)/java/awt/image/ImageFilter.class \
$(libobjdir)/java/awt/image/MemoryImageSource.class \
$(libobjdir)/java/awt/image/PixelGrabber.class \
$(libobjdir)/java/awt/image/RGBImageFilter.class \
$(libobjdir)/java/awt/image/ReplicateScaleFilter.class \
$(libobjdir)/java/awt/image/BufferStrategy.class \
$(libobjdir)/java/awt/image/BufferedImageOp.class \
$(libobjdir)/java/awt/image/ImagingOpException.class \
$(libobjdir)/java/awt/image/RasterFormatException.class \
$(libobjdir)/java/awt/image/RenderedImage.class \
$(libobjdir)/java/awt/image/TileObserver.class \
$(libobjdir)/java/awt/image/VolatileImage.class \
$(libobjdir)/java/awt/image/WritableRenderedImage.class \
$(libobjdir)/java/awt/image/renderable/ContextualRenderedImageFactory.class \
$(libobjdir)/java/awt/image/renderable/ParameterBlock.class \
$(libobjdir)/java/awt/image/renderable/RenderContext.class \
$(libobjdir)/java/awt/image/renderable/RenderableImage.class \
$(libobjdir)/java/awt/image/renderable/RenderableImageOp.class \
$(libobjdir)/java/awt/image/renderable/RenderableImageProducer.class \
$(libobjdir)/java/awt/image/renderable/RenderedImageFactory.class \
$(libobjdir)/java/awt/peer/ButtonPeer.class \
$(libobjdir)/java/awt/peer/CanvasPeer.class \
$(libobjdir)/java/awt/peer/CheckboxMenuItemPeer.class \
$(libobjdir)/java/awt/peer/CheckboxPeer.class \
$(libobjdir)/java/awt/peer/ChoicePeer.class \
$(libobjdir)/java/awt/peer/ComponentPeer.class \
$(libobjdir)/java/awt/peer/ContainerPeer.class \
$(libobjdir)/java/awt/peer/DialogPeer.class \
$(libobjdir)/java/awt/peer/FileDialogPeer.class \
$(libobjdir)/java/awt/peer/FontPeer.class \
$(libobjdir)/java/awt/peer/FramePeer.class \
$(libobjdir)/java/awt/peer/LabelPeer.class \
$(libobjdir)/java/awt/peer/LightweightPeer.class \
$(libobjdir)/java/awt/peer/ListPeer.class \
$(libobjdir)/java/awt/peer/MenuBarPeer.class \
$(libobjdir)/java/awt/peer/MenuComponentPeer.class \
$(libobjdir)/java/awt/peer/MenuItemPeer.class \
$(libobjdir)/java/awt/peer/MenuPeer.class \
$(libobjdir)/java/awt/peer/PanelPeer.class \
$(libobjdir)/java/awt/peer/PopupMenuPeer.class \
$(libobjdir)/java/awt/peer/ScrollPanePeer.class \
$(libobjdir)/java/awt/peer/ScrollbarPeer.class \
$(libobjdir)/java/awt/peer/TextAreaPeer.class \
$(libobjdir)/java/awt/peer/TextComponentPeer.class \
$(libobjdir)/java/awt/peer/TextFieldPeer.class \
$(libobjdir)/java/awt/peer/WindowPeer.class \
$(libobjdir)/java/awt/SystemColor.class \
$(libobjdir)/java/awt/Polygon.class \
$(libobjdir)/java/awt/MediaTracker.class \
$(libobjdir)/java/awt/PrintGraphics.class \
$(libobjdir)/java/awt/PrintJob.class \
$(libobjdir)/java/awt/AWTKeyStroke.class \
$(libobjdir)/java/awt/AlphaComposite.class \
$(libobjdir)/java/awt/AttributeValue.class \
$(libobjdir)/java/awt/BasicStroke.class \
$(libobjdir)/java/awt/BufferCapabilities.class \
$(libobjdir)/java/awt/ColorPaintContext.class \
$(libobjdir)/java/awt/Composite.class \
$(libobjdir)/java/awt/CompositeContext.class \
$(libobjdir)/java/awt/ContainerOrderFocusTraversalPolicy.class \
$(libobjdir)/java/awt/DefaultFocusTraversalPolicy.class \
$(libobjdir)/java/awt/DefaultKeyboardFocusManager.class \
$(libobjdir)/java/awt/DisplayMode.class \
$(libobjdir)/java/awt/FocusTraversalPolicy.class \
$(libobjdir)/java/awt/FontFormatException.class \
$(libobjdir)/java/awt/GradientPaint.class \
$(libobjdir)/java/awt/GraphicsConfigTemplate.class \
$(libobjdir)/java/awt/GraphicsDevice.class \
$(libobjdir)/java/awt/GraphicsEnvironment.class \
$(libobjdir)/java/awt/HeadlessException.class \
$(libobjdir)/java/awt/ImageCapabilities.class \
$(libobjdir)/java/awt/JobAttributes.class \
$(libobjdir)/java/awt/KeyEventDispatcher.class \
$(libobjdir)/java/awt/KeyEventPostProcessor.class \
$(libobjdir)/java/awt/KeyboardFocusManager.class \
$(libobjdir)/java/awt/PageAttributes.class \
$(libobjdir)/java/awt/Robot.class \
$(libobjdir)/java/awt/ScrollPaneAdjustable.class \
$(libobjdir)/java/awt/Stroke.class \
$(libobjdir)/java/awt/TexturePaint.class \
$(libobjdir)/java/awt/dnd/peer/DragSourceContextPeer.class \
$(libobjdir)/java/awt/dnd/peer/DropTargetContextPeer.class \
$(libobjdir)/java/awt/dnd/DnDConstants.class \
$(libobjdir)/java/awt/dnd/DragGestureEvent.class \
$(libobjdir)/java/awt/dnd/DragGestureListener.class \
$(libobjdir)/java/awt/dnd/DragGestureRecognizer.class	\
$(libobjdir)/java/awt/dnd/DragSource.class \
$(libobjdir)/java/awt/dnd/DragSourceAdapter.class \
$(libobjdir)/java/awt/dnd/DragSourceContext.class \
$(libobjdir)/java/awt/dnd/DragSourceDragEvent.class \
$(libobjdir)/java/awt/dnd/DragSourceEvent.class \
$(libobjdir)/java/awt/dnd/DragSourceListener.class \
$(libobjdir)/java/awt/dnd/DragSourceMotionListener.class \
$(libobjdir)/java/awt/dnd/DropTarget.class \
$(libobjdir)/java/awt/dnd/InvalidDnDOperationException.class \
$(libobjdir)/java/awt/dnd/Autoscroll.class \
$(libobjdir)/java/awt/dnd/DragSourceDropEvent.class \
$(libobjdir)/java/awt/dnd/DropTargetAdapter.class \
$(libobjdir)/java/awt/dnd/DropTargetContext.class \
$(libobjdir)/java/awt/dnd/DropTargetDragEvent.class \
$(libobjdir)/java/awt/dnd/DropTargetDropEvent.class \
$(libobjdir)/java/awt/dnd/DropTargetEvent.class \
$(libobjdir)/java/awt/dnd/DropTargetListener.class \
$(libobjdir)/java/awt/dnd/MouseDragGestureRecognizer.class \
$(libobjdir)/java/awt/font/TextHitInfo.class \
$(libobjdir)/java/awt/im/spi/InputMethod.class \
$(libobjdir)/java/awt/im/spi/InputMethodContext.class \
$(libobjdir)/java/awt/im/spi/InputMethodDescriptor.class \
$(libobjdir)/java/awt/im/InputContext.class \
$(libobjdir)/java/awt/im/InputMethodHighlight.class \
$(libobjdir)/java/awt/im/InputMethodRequests.class \
$(libobjdir)/java/awt/im/InputSubset.class \
$(libobjdir)/java/awt/print/Book.class \
$(libobjdir)/java/awt/print/PageFormat.class \
$(libobjdir)/java/awt/print/Pageable.class \
$(libobjdir)/java/awt/print/Paper.class \
$(libobjdir)/java/awt/print/Printable.class \
$(libobjdir)/java/awt/print/PrinterAbortException.class \
$(libobjdir)/java/awt/print/PrinterException.class \
$(libobjdir)/java/awt/print/PrinterGraphics.class \
$(libobjdir)/java/awt/print/PrinterIOException.class \
$(libobjdir)/java/awt/print/PrinterJob.class \
$(libobjdir)/java/beans/beancontext/BeanContext.class \
$(libobjdir)/java/beans/beancontext/BeanContextChild.class \
$(libobjdir)/java/beans/beancontext/BeanContextChildComponentProxy.class \
$(libobjdir)/java/beans/beancontext/BeanContextChildSupport.class \
$(libobjdir)/java/beans/beancontext/BeanContextContainerProxy.class \
$(libobjdir)/java/beans/beancontext/BeanContextEvent.class \
$(libobjdir)/java/beans/beancontext/BeanContextMembershipEvent.class \
$(libobjdir)/java/beans/beancontext/BeanContextMembershipListener.class \
$(libobjdir)/java/beans/beancontext/BeanContextProxy.class \
$(libobjdir)/java/beans/beancontext/BeanContextServiceAvailableEvent.class \
$(libobjdir)/java/beans/beancontext/BeanContextServiceProvider.class \
$(libobjdir)/java/beans/beancontext/BeanContextServiceProviderBeanInfo.class \
$(libobjdir)/java/beans/beancontext/BeanContextServiceRevokedEvent.class \
$(libobjdir)/java/beans/beancontext/BeanContextServiceRevokedListener.class \
$(libobjdir)/java/beans/beancontext/BeanContextServices.class \
$(libobjdir)/java/beans/beancontext/BeanContextServicesListener.class \
$(libobjdir)/java/beans/BeanDescriptor.class \
$(libobjdir)/java/beans/BeanInfo.class \
$(libobjdir)/java/beans/Beans.class \
$(libobjdir)/java/beans/Customizer.class \
$(libobjdir)/java/beans/DesignMode.class \
$(libobjdir)/java/beans/EventSetDescriptor.class \
$(libobjdir)/java/beans/ExceptionListener.class \
$(libobjdir)/java/beans/FeatureDescriptor.class \
$(libobjdir)/java/beans/IndexedPropertyDescriptor.class \
$(libobjdir)/java/beans/IntrospectionException.class \
$(libobjdir)/java/beans/Introspector.class \
$(libobjdir)/java/beans/MethodDescriptor.class \
$(libobjdir)/java/beans/ParameterDescriptor.class \
$(libobjdir)/java/beans/PropertyChangeEvent.class \
$(libobjdir)/java/beans/PropertyChangeListener.class \
$(libobjdir)/java/beans/PropertyChangeListenerProxy.class \
$(libobjdir)/java/beans/PropertyChangeSupport.class \
$(libobjdir)/java/beans/PropertyDescriptor.class \
$(libobjdir)/java/beans/PropertyEditor.class \
$(libobjdir)/java/beans/PropertyEditorManager.class \
$(libobjdir)/java/beans/PropertyEditorSupport.class \
$(libobjdir)/java/beans/PropertyVetoException.class \
$(libobjdir)/java/beans/SimpleBeanInfo.class \
$(libobjdir)/java/beans/VetoableChangeListener.class \
$(libobjdir)/java/beans/VetoableChangeListenerProxy.class \
$(libobjdir)/java/beans/VetoableChangeSupport.class \
$(libobjdir)/java/beans/Visibility.class \
$(libobjdir)/java/beans/AppletInitializer.class \
$(libobjdir)/javax/swing/border/AbstractBorder.class \
$(libobjdir)/javax/swing/border/Border.class \
$(libobjdir)/javax/swing/border/CompoundBorder.class \
$(libobjdir)/javax/swing/border/EmptyBorder.class \
$(libobjdir)/javax/swing/border/MatteBorder.class \
$(libobjdir)/javax/swing/border/TitledBorder.class \
$(libobjdir)/javax/swing/border/BevelBorder.class \
$(libobjdir)/javax/swing/border/EtchedBorder.class \
$(libobjdir)/javax/swing/border/LineBorder.class \
$(libobjdir)/javax/swing/GrayFilter.class \
$(libobjdir)/javax/swing/AbstractAction.class \
$(libobjdir)/javax/swing/AbstractButton.class \
$(libobjdir)/javax/swing/plaf/basic/BasicButtonUI.class \
$(libobjdir)/javax/swing/plaf/basic/BasicCheckBoxUI.class \
$(libobjdir)/javax/swing/plaf/basic/BasicDefaults.class \
$(libobjdir)/javax/swing/plaf/basic/BasicGraphicsUtils.class \
$(libobjdir)/javax/swing/plaf/basic/BasicLabelUI.class \
$(libobjdir)/javax/swing/plaf/basic/BasicListUI.class \
$(libobjdir)/javax/swing/plaf/basic/BasicOptionPaneUI.class \
$(libobjdir)/javax/swing/plaf/basic/BasicPanelUI.class \
$(libobjdir)/javax/swing/plaf/basic/BasicRadioButtonUI.class \
$(libobjdir)/javax/swing/plaf/basic/BasicScrollPaneUI.class \
$(libobjdir)/javax/swing/plaf/basic/BasicTabbedPaneUI.class \
$(libobjdir)/javax/swing/plaf/basic/BasicTextUI.class \
$(libobjdir)/javax/swing/plaf/basic/BasicToggleButtonUI.class \
$(libobjdir)/javax/swing/plaf/basic/BasicTreeUI.class \
$(libobjdir)/javax/swing/plaf/basic/BasicViewportUI.class \
$(libobjdir)/javax/swing/plaf/basic/BasicLookAndFeel.class \
$(libobjdir)/javax/swing/plaf/basic/BasicBorders.class \
$(libobjdir)/javax/swing/plaf/basic/BasicIconFactory.class \
$(libobjdir)/javax/swing/plaf/BorderUIResource.class \
$(libobjdir)/javax/swing/plaf/UIResource.class \
$(libobjdir)/javax/swing/plaf/ButtonUI.class \
$(libobjdir)/javax/swing/plaf/ComponentUI.class \
$(libobjdir)/javax/swing/plaf/LabelUI.class \
$(libobjdir)/javax/swing/plaf/ListUI.class \
$(libobjdir)/javax/swing/plaf/OptionPaneUI.class \
$(libobjdir)/javax/swing/plaf/PanelUI.class \
$(libobjdir)/javax/swing/plaf/ScrollPaneUI.class \
$(libobjdir)/javax/swing/plaf/TabbedPaneUI.class \
$(libobjdir)/javax/swing/plaf/TextUI.class \
$(libobjdir)/javax/swing/plaf/TreeUI.class \
$(libobjdir)/javax/swing/plaf/ViewportUI.class \
$(libobjdir)/javax/swing/plaf/metal/MetalLookAndFeel.class \
$(libobjdir)/javax/swing/plaf/ColorUIResource.class \
$(libobjdir)/javax/swing/plaf/DimensionUIResource.class \
$(libobjdir)/javax/swing/plaf/FontUIResource.class \
$(libobjdir)/javax/swing/plaf/IconUIResource.class \
$(libobjdir)/javax/swing/plaf/InsetsUIResource.class \
$(libobjdir)/javax/swing/plaf/ActionMapUIResource.class \
$(libobjdir)/javax/swing/plaf/ColorChooserUI.class \
$(libobjdir)/javax/swing/plaf/ComboBoxUI.class \
$(libobjdir)/javax/swing/plaf/ComponentInputMapUIResource.class \
$(libobjdir)/javax/swing/plaf/DesktopIconUI.class \
$(libobjdir)/javax/swing/plaf/DesktopPaneUI.class \
$(libobjdir)/javax/swing/plaf/FileChooserUI.class \
$(libobjdir)/javax/swing/plaf/InputMapUIResource.class \
$(libobjdir)/javax/swing/plaf/InternalFrameUI.class \
$(libobjdir)/javax/swing/plaf/MenuBarUI.class \
$(libobjdir)/javax/swing/plaf/MenuItemUI.class \
$(libobjdir)/javax/swing/plaf/PopupMenuUI.class \
$(libobjdir)/javax/swing/plaf/ProgressBarUI.class \
$(libobjdir)/javax/swing/plaf/RootPaneUI.class \
$(libobjdir)/javax/swing/plaf/ScrollBarUI.class \
$(libobjdir)/javax/swing/plaf/SeparatorUI.class \
$(libobjdir)/javax/swing/plaf/SliderUI.class \
$(libobjdir)/javax/swing/plaf/SplitPaneUI.class \
$(libobjdir)/javax/swing/plaf/TableHeaderUI.class \
$(libobjdir)/javax/swing/plaf/TableUI.class \
$(libobjdir)/javax/swing/plaf/ToolBarUI.class \
$(libobjdir)/javax/swing/plaf/ToolTipUI.class \
$(libobjdir)/javax/swing/AbstractListModel.class \
$(libobjdir)/javax/swing/AbstractSet.class \
$(libobjdir)/javax/swing/colorchooser/AbstractColorChooserPanel.class \
$(libobjdir)/javax/swing/colorchooser/ColorChooserComponentFactory.class \
$(libobjdir)/javax/swing/colorchooser/ColorSelectionModel.class \
$(libobjdir)/javax/swing/colorchooser/DefaultColorSelectionModel.class \
$(libobjdir)/javax/swing/filechooser/FileFilter.class \
$(libobjdir)/javax/swing/filechooser/FileSystemView.class \
$(libobjdir)/javax/swing/filechooser/FileView.class \
$(libobjdir)/javax/swing/table/AbstractTableModel.class \
$(libobjdir)/javax/swing/table/DefaultTableColumnModel.class \
$(libobjdir)/javax/swing/table/DefaultTableModel.class \
$(libobjdir)/javax/swing/table/TableCellEditor.class \
$(libobjdir)/javax/swing/table/TableCellRenderer.class \
$(libobjdir)/javax/swing/table/TableColumn.class \
$(libobjdir)/javax/swing/table/TableColumnModel.class \
$(libobjdir)/javax/swing/table/TableModel.class \
$(libobjdir)/javax/swing/table/DefaultTableCellRenderer.class \
$(libobjdir)/javax/swing/Action.class \
$(libobjdir)/javax/swing/BorderFactory.class \
$(libobjdir)/javax/swing/Box.class \
$(libobjdir)/javax/swing/BoxLayout.class \
$(libobjdir)/javax/swing/ButtonGroup.class \
$(libobjdir)/javax/swing/ButtonModel.class \
$(libobjdir)/javax/swing/ComponentInputMap.class \
$(libobjdir)/javax/swing/DefaultButtonModel.class \
$(libobjdir)/javax/swing/DefaultCellRenderer.class \
$(libobjdir)/javax/swing/DefaultListModel.class \
$(libobjdir)/javax/swing/DefaultListSelectionModel.class \
$(libobjdir)/javax/swing/Icon.class \
$(libobjdir)/javax/swing/ImageIcon.class \
$(libobjdir)/javax/swing/InputMap.class \
$(libobjdir)/javax/swing/JApplet.class \
$(libobjdir)/javax/swing/JButton.class \
$(libobjdir)/javax/swing/JCheckBox.class \
$(libobjdir)/javax/swing/JComponent.class \
$(libobjdir)/javax/swing/JDialog.class \
$(libobjdir)/javax/swing/JEditorPane.class \
$(libobjdir)/javax/swing/JFrame.class \
$(libobjdir)/javax/swing/JLabel.class \
$(libobjdir)/javax/swing/JLayeredPane.class \
$(libobjdir)/javax/swing/JList.class \
$(libobjdir)/javax/swing/JMenuBar.class \
$(libobjdir)/javax/swing/JOptionPane.class \
$(libobjdir)/javax/swing/JPanel.class \
$(libobjdir)/javax/swing/JRadioButton.class \
$(libobjdir)/javax/swing/JRootPane.class \
$(libobjdir)/javax/swing/JScrollBar.class \
$(libobjdir)/javax/swing/JScrollPane.class \
$(libobjdir)/javax/swing/JTabbedPane.class \
$(libobjdir)/javax/swing/JTextField.class \
$(libobjdir)/javax/swing/JToggleButton.class \
$(libobjdir)/javax/swing/JToolTip.class \
$(libobjdir)/javax/swing/JTree.class \
$(libobjdir)/javax/swing/JViewport.class \
$(libobjdir)/javax/swing/JWindow.class \
$(libobjdir)/javax/swing/KeyStroke.class \
$(libobjdir)/javax/swing/ListCellRenderer.class \
$(libobjdir)/javax/swing/ListModel.class \
$(libobjdir)/javax/swing/ListSelectionModel.class \
$(libobjdir)/javax/swing/LookAndFeel.class \
$(libobjdir)/javax/swing/Scrollable.class \
$(libobjdir)/javax/swing/SwingConstants.class \
$(libobjdir)/javax/swing/SwingUtilities.class \
$(libobjdir)/javax/swing/Timer.class \
$(libobjdir)/javax/swing/ToggleButtonModel.class \
$(libobjdir)/javax/swing/UIDefaults.class \
$(libobjdir)/javax/swing/UIManager.class \
$(libobjdir)/javax/swing/UnsupportedLookAndFeelException.class \
$(libobjdir)/javax/swing/event/AncestorEvent.class \
$(libobjdir)/javax/swing/event/AncestorListener.class \
$(libobjdir)/javax/swing/event/ChangeEvent.class \
$(libobjdir)/javax/swing/event/ChangeListener.class \
$(libobjdir)/javax/swing/event/DocumentEvent.class \
$(libobjdir)/javax/swing/event/DocumentListener.class \
$(libobjdir)/javax/swing/event/HyperlinkEvent.class \
$(libobjdir)/javax/swing/event/HyperlinkListener.class \
$(libobjdir)/javax/swing/event/ListDataEvent.class \
$(libobjdir)/javax/swing/event/ListDataListener.class \
$(libobjdir)/javax/swing/event/ListSelectionEvent.class \
$(libobjdir)/javax/swing/event/ListSelectionListener.class \
$(libobjdir)/javax/swing/event/MenuEvent.class \
$(libobjdir)/javax/swing/event/UndoableEditEvent.class \
$(libobjdir)/javax/swing/event/UndoableEditListener.class \
$(libobjdir)/javax/swing/event/CaretEvent.class \
$(libobjdir)/javax/swing/event/CaretListener.class \
$(libobjdir)/javax/swing/event/CellEditorListener.class \
$(libobjdir)/javax/swing/event/EventListenerList.class \
$(libobjdir)/javax/swing/event/InternalFrameAdapter.class \
$(libobjdir)/javax/swing/event/InternalFrameEvent.class \
$(libobjdir)/javax/swing/event/InternalFrameListener.class \
$(libobjdir)/javax/swing/event/MenuDragMouseEvent.class \
$(libobjdir)/javax/swing/event/MenuDragMouseListener.class \
$(libobjdir)/javax/swing/event/MenuKeyEvent.class \
$(libobjdir)/javax/swing/event/MenuKeyListener.class \
$(libobjdir)/javax/swing/event/MenuListener.class \
$(libobjdir)/javax/swing/event/MouseInputAdapter.class \
$(libobjdir)/javax/swing/event/MouseInputListener.class \
$(libobjdir)/javax/swing/event/PopupMenuEvent.class \
$(libobjdir)/javax/swing/event/PopupMenuListener.class \
$(libobjdir)/javax/swing/event/SwingPropertyChangeSupport.class \
$(libobjdir)/javax/swing/event/TableColumnModelEvent.class \
$(libobjdir)/javax/swing/event/TableColumnModelListener.class \
$(libobjdir)/javax/swing/event/TableModelEvent.class \
$(libobjdir)/javax/swing/event/TableModelListener.class \
$(libobjdir)/javax/swing/event/TreeExpansionEvent.class \
$(libobjdir)/javax/swing/event/TreeExpansionListener.class \
$(libobjdir)/javax/swing/event/TreeModelEvent.class \
$(libobjdir)/javax/swing/event/TreeModelListener.class \
$(libobjdir)/javax/swing/event/TreeSelectionEvent.class \
$(libobjdir)/javax/swing/event/TreeSelectionListener.class \
$(libobjdir)/javax/swing/event/TreeWillExpandListener.class \
$(libobjdir)/javax/swing/ActionMap.class \
$(libobjdir)/javax/swing/text/AbstractDocument.class \
$(libobjdir)/javax/swing/text/AttributeSet.class \
$(libobjdir)/javax/swing/text/BadLocationException.class \
$(libobjdir)/javax/swing/text/Caret.class \
$(libobjdir)/javax/swing/text/CharacterIterator.class \
$(libobjdir)/javax/swing/text/ComponentView.class \
$(libobjdir)/javax/swing/text/DefaultCaret.class \
$(libobjdir)/javax/swing/text/DefaultEditorKit.class \
$(libobjdir)/javax/swing/text/Document.class \
$(libobjdir)/javax/swing/text/EditorKit.class \
$(libobjdir)/javax/swing/text/Element.class \
$(libobjdir)/javax/swing/text/GapContent.class \
$(libobjdir)/javax/swing/text/JTextComponent.class \
$(libobjdir)/javax/swing/text/Keymap.class \
$(libobjdir)/javax/swing/text/PlainDocument.class \
$(libobjdir)/javax/swing/text/PlainEditorKit.class \
$(libobjdir)/javax/swing/text/Position.class \
$(libobjdir)/javax/swing/text/Segment.class \
$(libobjdir)/javax/swing/text/Style.class \
$(libobjdir)/javax/swing/text/View.class \
$(libobjdir)/javax/swing/text/ViewFactory.class \
$(libobjdir)/javax/swing/text/MutableAttributeSet.class \
$(libobjdir)/javax/swing/text/StyledDocument.class \
$(libobjdir)/javax/swing/text/StyledEditorKit.class \
$(libobjdir)/javax/swing/text/TextAction.class \
$(libobjdir)/javax/swing/text/html/HTML.class \
$(libobjdir)/javax/swing/text/html/parser/ParserDelegator.class \
$(libobjdir)/javax/swing/tree/TreeNode.class \
$(libobjdir)/javax/swing/tree/AbstractLayoutCache.class \
$(libobjdir)/javax/swing/tree/DefaultMutableTreeNode.class \
$(libobjdir)/javax/swing/tree/DefaultTreeCellEditor.class \
$(libobjdir)/javax/swing/tree/DefaultTreeCellRenderer.class \
$(libobjdir)/javax/swing/tree/DefaultTreeModel.class \
$(libobjdir)/javax/swing/tree/DefaultTreeSelectionModel.class \
$(libobjdir)/javax/swing/tree/ExpandVetoException.class \
$(libobjdir)/javax/swing/tree/FixedHeightLayoutCache.class \
$(libobjdir)/javax/swing/tree/MutableTreeNode.class \
$(libobjdir)/javax/swing/tree/RowMapper.class \
$(libobjdir)/javax/swing/tree/TreeCellEditor.class \
$(libobjdir)/javax/swing/tree/TreeCellRenderer.class \
$(libobjdir)/javax/swing/tree/TreeModel.class \
$(libobjdir)/javax/swing/tree/TreePath.class \
$(libobjdir)/javax/swing/tree/TreeSelectionModel.class \
$(libobjdir)/javax/swing/tree/VariableHeightLayoutCache.class \
$(libobjdir)/javax/swing/undo/UndoableEdit.class \
$(libobjdir)/javax/swing/undo/AbstractUndoableEdit.class \
$(libobjdir)/javax/swing/undo/CannotRedoException.class \
$(libobjdir)/javax/swing/undo/CannotUndoException.class \
$(libobjdir)/javax/swing/undo/CompoundEdit.class \
$(libobjdir)/javax/swing/undo/StateEdit.class \
$(libobjdir)/javax/swing/undo/StateEditable.class \
$(libobjdir)/javax/swing/undo/UndoManager.class \
$(libobjdir)/javax/swing/undo/UndoableEditSupport.class \
$(libobjdir)/javax/swing/BoundedRangeModel.class \
$(libobjdir)/javax/swing/CellEditor.class \
$(libobjdir)/javax/swing/ComboBoxEditor.class \
$(libobjdir)/javax/swing/ComboBoxModel.class \
$(libobjdir)/javax/swing/DefaultBoundedRangeModel.class \
$(libobjdir)/javax/swing/DesktopManager.class \
$(libobjdir)/javax/swing/InputVerifier.class \
$(libobjdir)/javax/swing/JInternalFrame.class \
$(libobjdir)/javax/swing/JTable.class \
$(libobjdir)/javax/swing/MenuElement.class \
$(libobjdir)/javax/swing/MenuSelectionManager.class \
$(libobjdir)/javax/swing/MutableComboBoxModel.class \
$(libobjdir)/javax/swing/Renderer.class \
$(libobjdir)/javax/swing/RootPaneContainer.class \
$(libobjdir)/javax/swing/ScrollPaneConstants.class \
$(libobjdir)/javax/swing/SingleSelectionModel.class \
$(libobjdir)/javax/swing/SizeRequirements.class \
$(libobjdir)/javax/swing/SizeSequence.class \
$(libobjdir)/javax/swing/WindowConstants.class \
$(libobjdir)/javax/swing/AbstractCellEditor.class \
$(libobjdir)/javax/swing/CellRendererPane.class \
$(libobjdir)/javax/swing/DebugGraphics.class \
$(libobjdir)/javax/swing/DefaultCellEditor.class \
$(libobjdir)/javax/swing/DefaultComboBoxModel.class \
$(libobjdir)/javax/swing/DefaultDesktopManager.class \
$(libobjdir)/javax/swing/DefaultFocusManager.class \
$(libobjdir)/javax/swing/DefaultListCellRenderer.class \
$(libobjdir)/javax/swing/DefaultSingleSelectionModel.class \
$(libobjdir)/javax/swing/FocusManager.class \
$(libobjdir)/javax/swing/JCheckBoxMenuItem.class \
$(libobjdir)/javax/swing/JColorChooser.class \
$(libobjdir)/javax/swing/JComboBox.class \
$(libobjdir)/javax/swing/JDesktopPane.class \
$(libobjdir)/javax/swing/JFileChooser.class \
$(libobjdir)/javax/swing/JMenu.class \
$(libobjdir)/javax/swing/JMenuItem.class \
$(libobjdir)/javax/swing/JPasswordField.class \
$(libobjdir)/javax/swing/JPopupMenu.class \
$(libobjdir)/javax/swing/JProgressBar.class \
$(libobjdir)/javax/swing/JRadioButtonMenuItem.class \
$(libobjdir)/javax/swing/JSeparator.class \
$(libobjdir)/javax/swing/JSlider.class \
$(libobjdir)/javax/swing/JSplitPane.class \
$(libobjdir)/javax/swing/JTextPane.class \
$(libobjdir)/javax/swing/JToolBar.class \
$(libobjdir)/javax/swing/OverlayLayout.class \
$(libobjdir)/javax/swing/ProgressMonitor.class \
$(libobjdir)/javax/swing/ProgressMonitorInputStream.class \
$(libobjdir)/javax/swing/RepaintManager.class \
$(libobjdir)/javax/swing/ScrollPaneLayout.class \
$(libobjdir)/javax/swing/ToolTipManager.class \
$(libobjdir)/javax/swing/ViewportLayout.class

rmi_java_source_files =	\
$(libobjdir)/java/rmi/activation/Activatable.class \
$(libobjdir)/java/rmi/activation/ActivateFailedException.class \
$(libobjdir)/java/rmi/activation/ActivationDesc.class	\
$(libobjdir)/java/rmi/activation/ActivationException.class \
$(libobjdir)/java/rmi/activation/ActivationGroup.class \
$(libobjdir)/java/rmi/activation/ActivationGroupDesc.class \
$(libobjdir)/java/rmi/activation/ActivationGroupID.class \
$(libobjdir)/java/rmi/activation/ActivationID.class \
$(libobjdir)/java/rmi/activation/ActivationInstantiator.class	\
$(libobjdir)/java/rmi/activation/ActivationMonitor.class \
$(libobjdir)/java/rmi/activation/ActivationSystem.class \
$(libobjdir)/java/rmi/activation/Activator.class \
$(libobjdir)/java/rmi/activation/UnknownGroupException.class \
$(libobjdir)/java/rmi/activation/UnknownObjectException.class	\
$(libobjdir)/java/rmi/AccessException.class \
$(libobjdir)/java/rmi/AlreadyBoundException.class \
$(libobjdir)/java/rmi/ConnectException.class \
$(libobjdir)/java/rmi/ConnectIOException.class \
$(libobjdir)/java/rmi/MarshalException.class \
$(libobjdir)/java/rmi/MarshalledObject.class \
$(libobjdir)/java/rmi/Naming.class \
$(libobjdir)/java/rmi/NoSuchObjectException.class \
$(libobjdir)/java/rmi/NotBoundException.class	\
$(libobjdir)/java/rmi/RMISecurityException.class \
$(libobjdir)/java/rmi/RMISecurityManager.class \
$(libobjdir)/java/rmi/Remote.class \
$(libobjdir)/java/rmi/RemoteException.class \
$(libobjdir)/java/rmi/ServerError.class \
$(libobjdir)/java/rmi/ServerException.class \
$(libobjdir)/java/rmi/ServerRuntimeException.class \
$(libobjdir)/java/rmi/StubNotFoundException.class \
$(libobjdir)/java/rmi/UnexpectedException.class \
$(libobjdir)/java/rmi/UnknownHostException.class \
$(libobjdir)/java/rmi/UnmarshalException.class \
$(libobjdir)/java/rmi/dgc/DGC.class \
$(libobjdir)/java/rmi/dgc/Lease.class	\
$(libobjdir)/java/rmi/dgc/VMID.class \
$(libobjdir)/java/rmi/registry/LocateRegistry.class \
$(libobjdir)/java/rmi/registry/Registry.class	\
$(libobjdir)/java/rmi/registry/RegistryHandler.class \
$(libobjdir)/java/rmi/server/ExportException.class \
$(libobjdir)/java/rmi/server/LoaderHandler.class \
$(libobjdir)/java/rmi/server/LogStream.class \
$(libobjdir)/java/rmi/server/ObjID.class \
$(libobjdir)/java/rmi/server/Operation.class \
$(libobjdir)/java/rmi/server/RMIClassLoader.class \
$(libobjdir)/java/rmi/server/RMIClassLoaderSpi.class \
$(libobjdir)/java/rmi/server/RMIClientSocketFactory.class \
$(libobjdir)/java/rmi/server/RMIFailureHandler.class \
$(libobjdir)/java/rmi/server/RMIServerSocketFactory.class \
$(libobjdir)/java/rmi/server/RMISocketFactory.class \
$(libobjdir)/java/rmi/server/RemoteCall.class	\
$(libobjdir)/java/rmi/server/RemoteObject.class \
$(libobjdir)/java/rmi/server/RemoteRef.class \
$(libobjdir)/java/rmi/server/RemoteServer.class \
$(libobjdir)/java/rmi/server/RemoteStub.class	\
$(libobjdir)/java/rmi/server/ServerCloneException.class \
$(libobjdir)/java/rmi/server/ServerNotActiveException.class \
$(libobjdir)/java/rmi/server/ServerRef.class \
$(libobjdir)/java/rmi/server/Skeleton.class \
$(libobjdir)/java/rmi/server/SkeletonMismatchException.class \
$(libobjdir)/java/rmi/server/SkeletonNotFoundException.class \
$(libobjdir)/java/rmi/server/SocketSecurityException.class \
$(libobjdir)/java/rmi/server/UID.class \
$(libobjdir)/java/rmi/server/UnicastRemoteObject.class \
$(libobjdir)/java/rmi/server/Unreferenced.class \
$(libobjdir)/gnu/java/rmi/RMIMarshalledObjectInputStream.class \
$(libobjdir)/gnu/java/rmi/RMIMarshalledObjectOutputStream.class \
$(libobjdir)/gnu/java/rmi/dgc/DGCImpl.class \
$(libobjdir)/gnu/java/rmi/dgc/DGCImpl_Skel.class \
$(libobjdir)/gnu/java/rmi/dgc/DGCImpl_Stub.class \
$(libobjdir)/gnu/java/rmi/registry/RegistryImpl.class	\
$(libobjdir)/gnu/java/rmi/registry/RegistryImpl_Skel.class \
$(libobjdir)/gnu/java/rmi/registry/RegistryImpl_Stub.class \
$(libobjdir)/gnu/java/rmi/rmic/Compile_gcj.class \
$(libobjdir)/gnu/java/rmi/rmic/Compiler.class	\
$(libobjdir)/gnu/java/rmi/rmic/CompilerProcess.class \
$(libobjdir)/gnu/java/rmi/rmic/RMIC.class \
$(libobjdir)/gnu/java/rmi/rmic/TabbedWriter.class \
$(libobjdir)/gnu/java/rmi/server/ConnectionRunnerPool.class \
$(libobjdir)/gnu/java/rmi/server/ProtocolConstants.class \
$(libobjdir)/gnu/java/rmi/server/RMIDefaultSocketFactory.class \
$(libobjdir)/gnu/java/rmi/server/RMIHashes.class \
$(libobjdir)/gnu/java/rmi/server/RMIObjectInputStream.class \
$(libobjdir)/gnu/java/rmi/server/RMIObjectOutputStream.class \
$(libobjdir)/gnu/java/rmi/server/UnicastConnection.class \
$(libobjdir)/gnu/java/rmi/server/UnicastConnectionManager.class \
$(libobjdir)/gnu/java/rmi/server/UnicastRef.class \
$(libobjdir)/gnu/java/rmi/server/UnicastRemoteCall.class \
$(libobjdir)/gnu/java/rmi/server/UnicastRemoteStub.class \
$(libobjdir)/gnu/java/rmi/server/UnicastServer.class \
$(libobjdir)/gnu/java/rmi/server/UnicastServerRef.class

javax_source_files = \
$(libobjdir)/javax/accessibility/Accessible.class \
$(libobjdir)/javax/accessibility/AccessibleAction.class \
$(libobjdir)/javax/accessibility/AccessibleBundle.class \
$(libobjdir)/javax/accessibility/AccessibleComponent.class \
$(libobjdir)/javax/accessibility/AccessibleContext.class \
$(libobjdir)/javax/accessibility/AccessibleEditableText.class \
$(libobjdir)/javax/accessibility/AccessibleExtendedComponent.class \
$(libobjdir)/javax/accessibility/AccessibleExtendedTable.class \
$(libobjdir)/javax/accessibility/AccessibleHyperlink.class \
$(libobjdir)/javax/accessibility/AccessibleHypertext.class \
$(libobjdir)/javax/accessibility/AccessibleIcon.class \
$(libobjdir)/javax/accessibility/AccessibleKeyBinding.class \
$(libobjdir)/javax/accessibility/AccessibleRelation.class \
$(libobjdir)/javax/accessibility/AccessibleRelationSet.class \
$(libobjdir)/javax/accessibility/AccessibleResourceBundle.class \
$(libobjdir)/javax/accessibility/AccessibleRole.class \
$(libobjdir)/javax/accessibility/AccessibleSelection.class \
$(libobjdir)/javax/accessibility/AccessibleState.class \
$(libobjdir)/javax/accessibility/AccessibleStateSet.class \
$(libobjdir)/javax/accessibility/AccessibleTable.class \
$(libobjdir)/javax/accessibility/AccessibleTableModelChange.class \
$(libobjdir)/javax/accessibility/AccessibleText.class \
$(libobjdir)/javax/accessibility/AccessibleValue.class \
$(libobjdir)/javax/naming/directory/Attribute.class \
$(libobjdir)/javax/naming/directory/AttributeInUseException.class \
$(libobjdir)/javax/naming/directory/AttributeModificationException.class \
$(libobjdir)/javax/naming/directory/Attributes.class \
$(libobjdir)/javax/naming/directory/DirContext.class \
$(libobjdir)/javax/naming/directory/InvalidAttributeIdentifierException.class	\
$(libobjdir)/javax/naming/directory/InvalidAttributeValueException.class \
$(libobjdir)/javax/naming/directory/InvalidAttributesException.class \
$(libobjdir)/javax/naming/directory/InvalidSearchControlsException.class \
$(libobjdir)/javax/naming/directory/InvalidSearchFilterException.class \
$(libobjdir)/javax/naming/directory/NoSuchAttributeException.class \
$(libobjdir)/javax/naming/directory/SchemaViolationException.class \
$(libobjdir)/javax/naming/directory/BasicAttribute.class \
$(libobjdir)/javax/naming/directory/BasicAttributes.class \
$(libobjdir)/javax/naming/directory/ModificationItem.class \
$(libobjdir)/javax/naming/directory/SearchControls.class \
$(libobjdir)/javax/naming/directory/SearchResult.class \
$(libobjdir)/javax/naming/directory/InitialDirContext.class \
$(libobjdir)/javax/naming/AuthenticationException.class \
$(libobjdir)/javax/naming/AuthenticationNotSupportedException.class \
$(libobjdir)/javax/naming/CannotProceedException.class \
$(libobjdir)/javax/naming/CommunicationException.class \
$(libobjdir)/javax/naming/ConfigurationException.class \
$(libobjdir)/javax/naming/Context.class \
$(libobjdir)/javax/naming/ContextNotEmptyException.class \
$(libobjdir)/javax/naming/InsufficientResourcesException.class \
$(libobjdir)/javax/naming/InterruptedNamingException.class \
$(libobjdir)/javax/naming/BinaryRefAddr.class	\
$(libobjdir)/javax/naming/LimitExceededException.class \
$(libobjdir)/javax/naming/LinkException.class	\
$(libobjdir)/javax/naming/LinkLoopException.class \
$(libobjdir)/javax/naming/MalformedLinkException.class \
$(libobjdir)/javax/naming/Name.class \
$(libobjdir)/javax/naming/NameAlreadyBoundException.class \
$(libobjdir)/javax/naming/NameNotFoundException.class	\
$(libobjdir)/javax/naming/NameParser.class \
$(libobjdir)/javax/naming/NamingEnumeration.class \
$(libobjdir)/javax/naming/InitialContext.class \
$(libobjdir)/javax/naming/NamingSecurityException.class \
$(libobjdir)/javax/naming/NoInitialContextException.class \
$(libobjdir)/javax/naming/NoPermissionException.class	\
$(libobjdir)/javax/naming/NotContextException.class \
$(libobjdir)/javax/naming/NamingException.class \
$(libobjdir)/javax/naming/PartialResultException.class \
$(libobjdir)/javax/naming/event/EventContext.class \
$(libobjdir)/javax/naming/event/EventDirContext.class	\
$(libobjdir)/javax/naming/event/NamespaceChangeListener.class	\
$(libobjdir)/javax/naming/event/NamingEvent.class \
$(libobjdir)/javax/naming/event/NamingExceptionEvent.class \
$(libobjdir)/javax/naming/event/NamingListener.class \
$(libobjdir)/javax/naming/event/ObjectChangeListener.class \
$(libobjdir)/javax/naming/RefAddr.class \
$(libobjdir)/javax/naming/Reference.class \
$(libobjdir)/javax/naming/Referenceable.class	\
$(libobjdir)/javax/naming/ReferralException.class \
$(libobjdir)/javax/naming/ServiceUnavailableException.class \
$(libobjdir)/javax/naming/SizeLimitExceededException.class \
$(libobjdir)/javax/naming/StringRefAddr.class	\
$(libobjdir)/javax/naming/TimeLimitExceededException.class \
$(libobjdir)/javax/naming/spi/InitialContextFactory.class \
$(libobjdir)/javax/naming/spi/InitialContextFactoryBuilder.class \
$(libobjdir)/javax/naming/spi/NamingManager.class \
$(libobjdir)/javax/naming/spi/ObjectFactory.class \
$(libobjdir)/javax/naming/spi/Resolver.class \
$(libobjdir)/javax/naming/spi/DirObjectFactory.class \
$(libobjdir)/javax/naming/spi/DirStateFactory.class \
$(libobjdir)/javax/naming/spi/DirectoryManager.class \
$(libobjdir)/javax/naming/spi/ObjectFactoryBuilder.class \
$(libobjdir)/javax/naming/spi/ResolveResult.class \
$(libobjdir)/javax/naming/spi/StateFactory.class \
$(libobjdir)/javax/naming/InvalidNameException.class \
$(libobjdir)/javax/naming/Binding.class \
$(libobjdir)/javax/naming/CompositeName.class	\
$(libobjdir)/javax/naming/CompoundName.class \
$(libobjdir)/javax/naming/LinkRef.class \
$(libobjdir)/javax/naming/NameClassPair.class	\
$(libobjdir)/javax/naming/ldap/Control.class \
$(libobjdir)/javax/naming/ldap/ControlFactory.class \
$(libobjdir)/javax/naming/ldap/ExtendedRequest.class \
$(libobjdir)/javax/naming/ldap/ExtendedResponse.class	\
$(libobjdir)/javax/naming/ldap/HasControls.class \
$(libobjdir)/javax/naming/ldap/InitialLdapContext.class \
$(libobjdir)/javax/naming/ldap/LdapContext.class \
$(libobjdir)/javax/naming/ldap/LdapReferralException.class \
$(libobjdir)/javax/naming/ldap/UnsolicitedNotification.class \
$(libobjdir)/javax/naming/ldap/UnsolicitedNotificationEvent.class \
$(libobjdir)/javax/naming/ldap/UnsolicitedNotificationListener.class \
$(libobjdir)/javax/naming/OperationNotSupportedException.class \
$(libobjdir)/javax/sql/ConnectionEvent.class \
$(libobjdir)/javax/sql/ConnectionEventListener.class \
$(libobjdir)/javax/sql/ConnectionPoolDataSource.class \
$(libobjdir)/javax/sql/DataSource.class \
$(libobjdir)/javax/sql/PooledConnection.class \
$(libobjdir)/javax/sql/RowSetEvent.class \
$(libobjdir)/javax/sql/RowSetInternal.class \
$(libobjdir)/javax/sql/RowSet.class \
$(libobjdir)/javax/sql/RowSetListener.class \
$(libobjdir)/javax/sql/RowSetMetaData.class \
$(libobjdir)/javax/sql/RowSetReader.class \
$(libobjdir)/javax/sql/RowSetWriter.class \
$(libobjdir)/javax/sql/XAConnection.class \
$(libobjdir)/javax/sql/XADataSource.class \
$(libobjdir)/javax/transaction/xa/XAException.class \
$(libobjdir)/javax/transaction/xa/XAResource.class \
$(libobjdir)/javax/transaction/xa/Xid.class \
$(libobjdir)/javax/transaction/HeuristicCommitException.class	\
$(libobjdir)/javax/transaction/HeuristicMixedException.class \
$(libobjdir)/javax/transaction/HeuristicRollbackException.class \
$(libobjdir)/javax/transaction/InvalidTransactionException.class \
$(libobjdir)/javax/transaction/NotSupportedException.class \
$(libobjdir)/javax/transaction/RollbackException.class \
$(libobjdir)/javax/transaction/Status.class \
$(libobjdir)/javax/transaction/Synchronization.class \
$(libobjdir)/javax/transaction/SystemException.class \
$(libobjdir)/javax/transaction/Transaction.class \
$(libobjdir)/javax/transaction/TransactionManager.class \
$(libobjdir)/javax/transaction/TransactionRequiredException.class \
$(libobjdir)/javax/transaction/TransactionRolledbackException.class \
$(libobjdir)/javax/transaction/UserTransaction.class

## Java files which are created by configure and thus are in the build
## directory.
built_java_source_files = $(libobjdir)/java/lang/ConcreteProcess.class \
                          $(libobjdir)/gnu/classpath/Configuration.class

## Java files in the core packages java.lang, java.io, and java.util.
## These are built before the other source files, in order to reduce 
## build time.
core_java_source_files = \
$(libobjdir)/java/lang/AbstractMethodError.class \
$(libobjdir)/java/lang/ArithmeticException.class \
$(libobjdir)/java/lang/ArrayIndexOutOfBoundsException.class \
$(libobjdir)/java/lang/ArrayStoreException.class \
$(libobjdir)/java/lang/AssertionError.class \
$(libobjdir)/java/lang/Boolean.class \
$(libobjdir)/java/lang/Byte.class \
$(libobjdir)/java/lang/Character.class \
$(libobjdir)/java/lang/CharSequence.class \
$(libobjdir)/java/lang/ClassCastException.class \
$(libobjdir)/java/lang/ClassCircularityError.class \
$(libobjdir)/java/lang/ClassFormatError.class	\
$(libobjdir)/java/lang/ClassLoader.class \
$(libobjdir)/java/lang/ClassNotFoundException.class \
$(libobjdir)/java/lang/CloneNotSupportedException.class \
$(libobjdir)/java/lang/Cloneable.class \
$(libobjdir)/java/lang/Comparable.class \
$(libobjdir)/java/lang/Compiler.class	\
$(libobjdir)/java/lang/Double.class \
$(libobjdir)/java/lang/Error.class \
$(libobjdir)/java/lang/Exception.class \
$(libobjdir)/java/lang/ExceptionInInitializerError.class \
$(libobjdir)/java/lang/Float.class \
$(libobjdir)/java/lang/IllegalAccessError.class \
$(libobjdir)/java/lang/IllegalAccessException.class \
$(libobjdir)/java/lang/IllegalArgumentException.class	\
$(libobjdir)/java/lang/IllegalMonitorStateException.class \
$(libobjdir)/java/lang/IllegalStateException.class \
$(libobjdir)/java/lang/IllegalThreadStateException.class \
$(libobjdir)/java/lang/IncompatibleClassChangeError.class \
$(libobjdir)/java/lang/IndexOutOfBoundsException.class \
$(libobjdir)/java/lang/InheritableThreadLocal.class \
$(libobjdir)/java/lang/InstantiationError.class \
$(libobjdir)/java/lang/InstantiationException.class \
$(libobjdir)/java/lang/Integer.class \
$(libobjdir)/java/lang/InternalError.class \
$(libobjdir)/java/lang/InterruptedException.class \
$(libobjdir)/java/lang/LinkageError.class \
$(libobjdir)/java/lang/Long.class \
$(libobjdir)/java/lang/Math.class \
$(libobjdir)/java/lang/NegativeArraySizeException.class \
$(libobjdir)/java/lang/NoClassDefFoundError.class \
$(libobjdir)/java/lang/NoSuchFieldError.class	\
$(libobjdir)/java/lang/NoSuchFieldException.class \
$(libobjdir)/java/lang/NoSuchMethodError.class \
$(libobjdir)/java/lang/NoSuchMethodException.class \
$(libobjdir)/java/lang/NullPointerException.class \
$(libobjdir)/java/lang/Number.class \
$(libobjdir)/java/lang/NumberFormatException.class \
$(libobjdir)/java/lang/OutOfMemoryError.class	\
$(libobjdir)/java/lang/Package.class \
$(libobjdir)/java/lang/Process.class \
$(libobjdir)/java/lang/Runnable.class	\
$(libobjdir)/java/lang/Runtime.class \
$(libobjdir)/java/lang/RuntimeException.class	\
$(libobjdir)/java/lang/RuntimePermission.class \
$(libobjdir)/java/lang/SecurityException.class \
$(libobjdir)/java/lang/SecurityManager.class \
$(libobjdir)/java/lang/Short.class \
$(libobjdir)/java/lang/StackOverflowError.class \
$(libobjdir)/java/lang/StackTraceElement.class \
$(libobjdir)/java/lang/StrictMath.class \
$(libobjdir)/java/lang/String.class \
$(libobjdir)/java/lang/StringBuffer.class \
$(libobjdir)/java/lang/StringIndexOutOfBoundsException.class \
$(libobjdir)/java/lang/System.class \
$(libobjdir)/java/lang/Thread.class \
$(libobjdir)/java/lang/ThreadDeath.class \
$(libobjdir)/java/lang/ThreadGroup.class \
$(libobjdir)/java/lang/ThreadLocal.class \
$(libobjdir)/java/lang/Throwable.class \
$(libobjdir)/java/lang/UnknownError.class \
$(libobjdir)/java/lang/UnsatisfiedLinkError.class \
$(libobjdir)/java/lang/UnsupportedClassVersionError.class \
$(libobjdir)/java/lang/UnsupportedOperationException.class \
$(libobjdir)/java/lang/VerifyError.class \
$(libobjdir)/java/lang/VirtualMachineError.class \
$(libobjdir)/java/lang/VMClassLoader.class \
$(libobjdir)/java/lang/VMSecurityManager.class \
$(libobjdir)/java/lang/VMThrowable.class \
$(libobjdir)/java/lang/Void.class \
$(libobjdir)/java/io/BufferedInputStream.class \
$(libobjdir)/java/io/BufferedOutputStream.class \
$(libobjdir)/java/io/BufferedReader.class \
$(libobjdir)/java/io/BufferedWriter.class \
$(libobjdir)/java/io/ByteArrayInputStream.class \
$(libobjdir)/java/io/ByteArrayOutputStream.class \
$(libobjdir)/java/io/CharArrayReader.class \
$(libobjdir)/java/io/CharArrayWriter.class \
$(libobjdir)/java/io/CharConversionException.class \
$(libobjdir)/java/io/DataInput.class \
$(libobjdir)/java/io/DataInputStream.class \
$(libobjdir)/java/io/DataOutput.class	\
$(libobjdir)/java/io/DataOutputStream.class \
$(libobjdir)/java/io/EOFException.class \
$(libobjdir)/java/io/Externalizable.class \
$(libobjdir)/java/io/File.class \
$(libobjdir)/java/io/FileDescriptor.class \
$(libobjdir)/java/io/FileFilter.class \
$(libobjdir)/java/io/FileInputStream.class \
$(libobjdir)/java/io/FileNotFoundException.class \
$(libobjdir)/java/io/FileOutputStream.class \
$(libobjdir)/java/io/FilePermission.class \
$(libobjdir)/java/io/FileReader.class	\
$(libobjdir)/java/io/FileWriter.class	\
$(libobjdir)/java/io/FilenameFilter.class \
$(libobjdir)/java/io/FilterInputStream.class \
$(libobjdir)/java/io/FilterOutputStream.class	\
$(libobjdir)/java/io/FilterReader.class \
$(libobjdir)/java/io/FilterWriter.class \
$(libobjdir)/java/io/IOException.class \
$(libobjdir)/java/io/InputStream.class \
$(libobjdir)/java/io/InputStreamReader.class \
$(libobjdir)/java/io/InterruptedIOException.class \
$(libobjdir)/java/io/InvalidClassException.class \
$(libobjdir)/java/io/InvalidObjectException.class \
$(libobjdir)/java/io/LineNumberInputStream.class \
$(libobjdir)/java/io/LineNumberReader.class \
$(libobjdir)/java/io/NotActiveException.class	\
$(libobjdir)/java/io/NotSerializableException.class \
$(libobjdir)/java/io/ObjectInput.class \
$(libobjdir)/java/io/ObjectInputStream.class \
$(libobjdir)/java/io/ObjectInputValidation.class \
$(libobjdir)/java/io/ObjectOutput.class \
$(libobjdir)/java/io/ObjectOutputStream.class	\
$(libobjdir)/java/io/ObjectStreamClass.class \
$(libobjdir)/java/io/ObjectStreamConstants.class \
$(libobjdir)/java/io/ObjectStreamException.class \
$(libobjdir)/java/io/ObjectStreamField.class \
$(libobjdir)/java/io/OptionalDataException.class \
$(libobjdir)/java/io/OutputStream.class \
$(libobjdir)/java/io/OutputStreamWriter.class	\
$(libobjdir)/java/io/PipedInputStream.class \
$(libobjdir)/java/io/PipedOutputStream.class \
$(libobjdir)/java/io/PipedReader.class \
$(libobjdir)/java/io/PipedWriter.class \
$(libobjdir)/java/io/PrintStream.class \
$(libobjdir)/java/io/PrintWriter.class \
$(libobjdir)/java/io/PushbackInputStream.class \
$(libobjdir)/java/io/PushbackReader.class \
$(libobjdir)/java/io/RandomAccessFile.class \
$(libobjdir)/java/io/Reader.class \
$(libobjdir)/java/io/SequenceInputStream.class \
$(libobjdir)/java/io/Serializable.class \
$(libobjdir)/java/io/SerializablePermission.class \
$(libobjdir)/java/io/StreamCorruptedException.class \
$(libobjdir)/java/io/StreamTokenizer.class \
$(libobjdir)/java/io/StringBufferInputStream.class \
$(libobjdir)/java/io/StringReader.class \
$(libobjdir)/java/io/StringWriter.class \
$(libobjdir)/java/io/SyncFailedException.class \
$(libobjdir)/java/io/UTFDataFormatException.class \
$(libobjdir)/java/io/UnsupportedEncodingException.class \
$(libobjdir)/java/io/VMObjectStreamClass.class \
$(libobjdir)/java/io/WriteAbortedException.class \
$(libobjdir)/java/io/Writer.class \
$(libobjdir)/java/util/AbstractCollection.class \
$(libobjdir)/java/util/AbstractList.class \
$(libobjdir)/java/util/AbstractMap.class \
$(libobjdir)/java/util/AbstractSequentialList.class \
$(libobjdir)/java/util/AbstractSet.class \
$(libobjdir)/java/util/ArrayList.class \
$(libobjdir)/java/util/Arrays.class \
$(libobjdir)/java/util/BitSet.class \
$(libobjdir)/java/util/Calendar.class	\
$(libobjdir)/java/util/Collection.class \
$(libobjdir)/java/util/Collections.class \
$(libobjdir)/java/util/Comparator.class \
$(libobjdir)/java/util/ConcurrentModificationException.class \
$(libobjdir)/java/util/Date.class \
$(libobjdir)/java/util/Dictionary.class \
$(libobjdir)/java/util/EmptyStackException.class \
$(libobjdir)/java/util/Enumeration.class \
$(libobjdir)/java/util/EventListener.class \
$(libobjdir)/java/util/EventListenerProxy.class \
$(libobjdir)/java/util/EventObject.class \
$(libobjdir)/java/util/GregorianCalendar.class \
$(libobjdir)/java/util/HashMap.class \
$(libobjdir)/java/util/HashSet.class \
$(libobjdir)/java/util/Hashtable.class \
$(libobjdir)/java/util/IdentityHashMap.class \
$(libobjdir)/java/util/Iterator.class	\
$(libobjdir)/java/util/LinkedList.class \
$(libobjdir)/java/util/LinkedHashMap.class \
$(libobjdir)/java/util/LinkedHashSet.class \
$(libobjdir)/java/util/List.class \
$(libobjdir)/java/util/ListIterator.class \
$(libobjdir)/java/util/ListResourceBundle.class \
$(libobjdir)/java/util/Locale.class \
$(libobjdir)/java/util/Map.class \
$(libobjdir)/java/util/MissingResourceException.class	\
$(libobjdir)/java/util/NoSuchElementException.class \
$(libobjdir)/java/util/Observable.class \
$(libobjdir)/java/util/Observer.class	\
$(libobjdir)/java/util/Properties.class \
$(libobjdir)/java/util/PropertyPermission.class \
$(libobjdir)/java/util/PropertyPermissionCollection.class \
$(libobjdir)/java/util/PropertyResourceBundle.class \
$(libobjdir)/java/util/Random.class \
$(libobjdir)/java/util/RandomAccess.class \
$(libobjdir)/java/util/ResourceBundle.class \
$(libobjdir)/java/util/Set.class \
$(libobjdir)/java/util/SimpleTimeZone.class \
$(libobjdir)/java/util/SortedMap.class \
$(libobjdir)/java/util/SortedSet.class \
$(libobjdir)/java/util/Stack.class \
$(libobjdir)/java/util/StringTokenizer.class \
$(libobjdir)/java/util/TimeZone.class	\
$(libobjdir)/java/util/Timer.class \
$(libobjdir)/java/util/TimerTask.class \
$(libobjdir)/java/util/TreeMap.class \
$(libobjdir)/java/util/TreeSet.class \
$(libobjdir)/java/util/TooManyListenersException.class \
$(libobjdir)/java/util/Vector.class \
$(libobjdir)/java/util/WeakHashMap.class \
$(libobjdir)/java/util/regex/Matcher.class \
$(libobjdir)/java/util/regex/Pattern.class \
$(libobjdir)/java/util/regex/PatternSyntaxException.class

## List of all .class files to be compiled.  Please keep this list
## alphabetical.  Please put files from java/lang, java/io, and java/util
## into core_java_source_files. Please put files from gnu/gcj/convert 
## into convert_source_files and files from java/awt into
## awt_java_source_files.  If the .class file has a hand-maintained
## header, please list it in special_java_source_files.
ordinary_java_source_files = $(core_java_source_files) \
$(libobjdir)/gnu/gcj/Core.class \
$(libobjdir)/gnu/gcj/RawData.class \
$(libobjdir)/gnu/gcj/io/DefaultMimeTypes.class \
$(libobjdir)/gnu/gcj/io/MimeTypes.class \
$(libobjdir)/gnu/gcj/io/SimpleSHSStream.class	\
$(libobjdir)/gnu/gcj/protocol/core/Connection.class \
$(libobjdir)/gnu/gcj/protocol/core/Handler.class \
$(libobjdir)/gnu/gcj/protocol/core/CoreInputStream.class \
$(libobjdir)/gnu/gcj/protocol/file/Connection.class \
$(libobjdir)/gnu/gcj/protocol/file/Handler.class \
$(libobjdir)/gnu/gcj/protocol/http/Connection.class \
$(libobjdir)/gnu/gcj/protocol/http/Handler.class \
$(libobjdir)/gnu/gcj/protocol/jar/Connection.class \
$(libobjdir)/gnu/gcj/protocol/jar/Handler.class \
$(libobjdir)/gnu/gcj/runtime/FileDeleter.class \
$(libobjdir)/gnu/gcj/runtime/FinalizerThread.class \
$(libobjdir)/gnu/gcj/runtime/FirstThread.class \
$(libobjdir)/gnu/gcj/runtime/JNIWeakRef.class \
$(libobjdir)/gnu/gcj/runtime/MethodRef.class \
$(libobjdir)/gnu/gcj/runtime/NameFinder.class \
$(libobjdir)/gnu/gcj/runtime/SharedLibLoader.class \
$(libobjdir)/gnu/gcj/runtime/StackTrace.class \
$(libobjdir)/gnu/gcj/runtime/StringBuffer.class \
$(libobjdir)/gnu/gcj/runtime/VMClassLoader.class \
$(libobjdir)/gnu/java/io/ClassLoaderObjectInputStream.class \
$(libobjdir)/gnu/java/io/NullOutputStream.class \
$(libobjdir)/gnu/java/io/ObjectIdentityWrapper.class \
$(libobjdir)/gnu/java/lang/ArrayHelper.class \
$(libobjdir)/gnu/java/lang/ClassHelper.class \
$(libobjdir)/gnu/java/lang/CharData.class \
$(libobjdir)/gnu/java/lang/reflect/TypeSignature.class \
$(libobjdir)/gnu/java/locale/Calendar.class \
$(libobjdir)/gnu/java/locale/Calendar_de.class \
$(libobjdir)/gnu/java/locale/Calendar_en.class \
$(libobjdir)/gnu/java/locale/Calendar_nl.class \
$(libobjdir)/gnu/java/locale/LocaleInformation.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_af_ZA.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_ar_AE.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_ar_BH.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_ar_DZ.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_ar_EG.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_ar_IN.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_ar_IQ.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_ar_JO.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_ar_KW.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_ar_LB.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_ar_LY.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_ar_MA.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_ar_OM.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_ar_QA.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_ar_SD.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_ar_SY.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_ar_TN.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_ar_YE.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_be_BY.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_bn_IN.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_br_FR.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_bs_BA.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_ca_ES.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_cs_CZ.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_cy_GB.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_da_DK.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_de.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_de_AT.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_de_BE.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_de_CH.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_de_DE.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_de_LU.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_el_GR.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_en.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_en_AU.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_en_BW.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_en_CA.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_en_DK.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_en_GB.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_en_HK.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_en_IE.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_en_IN.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_en_NZ.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_en_PH.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_en_SG.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_en_US.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_en_ZA.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_en_ZW.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_es_AR.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_es_BO.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_es_CL.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_es_CO.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_es_CR.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_es_DO.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_es_EC.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_es_ES.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_es_GT.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_es_HN.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_es_MX.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_es_NI.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_es_PA.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_es_PE.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_es_PR.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_es_PY.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_es_SV.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_es_US.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_es_UY.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_es_VE.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_et_EE.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_eu_ES.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_fa_IR.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_fi_FI.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_fo_FO.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_fr_BE.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_fr_CA.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_fr_CH.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_fr_FR.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_fr_LU.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_ga_IE.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_gd_GB.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_gl_ES.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_gv_GB.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_he_IL.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_hi_IN.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_hr_HR.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_hu_HU.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_id_ID.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_it_CH.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_it_IT.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_iw_IL.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_ja_JP.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_ka_GE.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_kl_GL.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_ko_KR.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_kw_GB.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_lt_LT.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_lv_LV.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_mi_NZ.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_mk_MK.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_mr_IN.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_mt_MT.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_nl.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_nl_BE.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_nl_NL.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_nn_NO.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_no_NO.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_oc_FR.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_pl_PL.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_pt_BR.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_pt_PT.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_ro_RO.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_ru_RU.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_ru_UA.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_se_NO.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_sk_SK.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_sl_SI.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_sq_AL.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_sr_YU.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_sv_FI.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_sv_SE.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_ta_IN.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_te_IN.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_tg_TJ.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_tl_PH.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_tr_TR.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_uk_UA.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_ur_PK.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_uz_UZ.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_vi_VN.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_yi_US.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_zh_CN.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_zh_HK.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_zh_SG.class \
$(libobjdir)/gnu/java/locale/LocaleInformation_zh_TW.class \
$(libobjdir)/gnu/java/math/MPN.class \
$(libobjdir)/gnu/java/nio/ByteBufferImpl.class \
$(libobjdir)/gnu/java/nio/CharBufferImpl.class \
$(libobjdir)/gnu/java/nio/DatagramChannelImpl.class \
$(libobjdir)/gnu/java/nio/DoubleBufferImpl.class \
$(libobjdir)/gnu/java/nio/FileChannelImpl.class \
$(libobjdir)/gnu/java/nio/FloatBufferImpl.class \
$(libobjdir)/gnu/java/nio/IntBufferImpl.class \
$(libobjdir)/gnu/java/nio/LongBufferImpl.class \
$(libobjdir)/gnu/java/nio/PipeImpl.class \
$(libobjdir)/gnu/java/nio/SelectionKeyImpl.class \
$(libobjdir)/gnu/java/nio/SelectorImpl.class \
$(libobjdir)/gnu/java/nio/SelectorProviderImpl.class \
$(libobjdir)/gnu/java/nio/ServerSocketChannelImpl.class \
$(libobjdir)/gnu/java/nio/ShortBufferImpl.class \
$(libobjdir)/gnu/java/nio/SocketChannelImpl.class \
$(libobjdir)/gnu/java/nio/charset/ISO_8859_1.class \
$(libobjdir)/gnu/java/nio/charset/Provider.class \
$(libobjdir)/gnu/java/nio/charset/US_ASCII.class \
$(libobjdir)/gnu/java/nio/charset/UTF_16.class \
$(libobjdir)/gnu/java/nio/charset/UTF_16BE.class \
$(libobjdir)/gnu/java/nio/charset/UTF_16Decoder.class \
$(libobjdir)/gnu/java/nio/charset/UTF_16Encoder.class \
$(libobjdir)/gnu/java/nio/charset/UTF_16LE.class \
$(libobjdir)/gnu/java/nio/charset/UTF_8.class \
$(libobjdir)/gnu/java/security/der/DEREncodingException.class \
$(libobjdir)/gnu/java/security/provider/DERReader.class \
$(libobjdir)/gnu/java/security/provider/DERWriter.class \
$(libobjdir)/gnu/java/security/provider/DefaultPolicy.class \
$(libobjdir)/gnu/java/security/provider/DSAKeyPairGenerator.class \
$(libobjdir)/gnu/java/security/provider/DSAParameterGenerator.class \
$(libobjdir)/gnu/java/security/provider/DSAParameters.class \
$(libobjdir)/gnu/java/security/provider/DSASignature.class \
$(libobjdir)/gnu/java/security/provider/Gnu.class \
$(libobjdir)/gnu/java/security/provider/GnuDSAPrivateKey.class \
$(libobjdir)/gnu/java/security/provider/GnuDSAPublicKey.class \
$(libobjdir)/gnu/java/security/provider/MD5.class \
$(libobjdir)/gnu/java/security/provider/SHA.class \
$(libobjdir)/gnu/java/security/provider/SHA1PRNG.class \
$(libobjdir)/gnu/java/security/util/Prime.class \
$(libobjdir)/gnu/java/text/BaseBreakIterator.class \
$(libobjdir)/gnu/java/text/CharacterBreakIterator.class \
$(libobjdir)/gnu/java/text/LineBreakIterator.class \
$(libobjdir)/gnu/java/text/SentenceBreakIterator.class \
$(libobjdir)/gnu/java/text/WordBreakIterator.class \
$(libobjdir)/gnu/java/util/DoubleEnumeration.class \
$(libobjdir)/gnu/java/util/EmptyEnumeration.class \
$(libobjdir)/java/lang/ref/PhantomReference.class \
$(libobjdir)/java/lang/ref/Reference.class \
$(libobjdir)/java/lang/ref/ReferenceQueue.class \
$(libobjdir)/java/lang/ref/SoftReference.class \
$(libobjdir)/java/lang/ref/WeakReference.class \
$(libobjdir)/java/lang/reflect/AccessibleObject.class	\
$(libobjdir)/java/lang/reflect/Array.class \
$(libobjdir)/java/lang/reflect/Constructor.class \
$(libobjdir)/java/lang/reflect/Field.class \
$(libobjdir)/java/lang/reflect/InvocationHandler.class \
$(libobjdir)/java/lang/reflect/InvocationTargetException.class \
$(libobjdir)/java/lang/reflect/Member.class \
$(libobjdir)/java/lang/reflect/Method.class \
$(libobjdir)/java/lang/reflect/Modifier.class	\
$(libobjdir)/java/lang/reflect/Proxy.class \
$(libobjdir)/java/lang/reflect/ReflectPermission.class \
$(libobjdir)/java/lang/reflect/UndeclaredThrowableException.class \
$(libobjdir)/java/math/BigDecimal.class \
$(libobjdir)/java/math/BigInteger.class \
$(libobjdir)/java/net/Authenticator.class \
$(libobjdir)/java/net/BindException.class \
$(libobjdir)/java/net/ConnectException.class \
$(libobjdir)/java/net/ContentHandler.class \
$(libobjdir)/java/net/ContentHandlerFactory.class \
$(libobjdir)/java/net/DatagramPacket.class \
$(libobjdir)/java/net/DatagramSocket.class \
$(libobjdir)/java/net/DatagramSocketImpl.class \
$(libobjdir)/java/net/DatagramSocketImplFactory.class \
$(libobjdir)/java/net/FileNameMap.class \
$(libobjdir)/java/net/HttpURLConnection.class	\
$(libobjdir)/java/net/InetAddress.class \
$(libobjdir)/java/net/Inet4Address.class \
$(libobjdir)/java/net/Inet6Address.class \
$(libobjdir)/java/net/InetSocketAddress.class \
$(libobjdir)/java/net/JarURLConnection.class \
$(libobjdir)/java/net/MalformedURLException.class \
$(libobjdir)/java/net/MulticastSocket.class \
$(libobjdir)/java/net/NetPermission.class \
$(libobjdir)/java/net/NetworkInterface.class \
$(libobjdir)/java/net/NoRouteToHostException.class \
$(libobjdir)/java/net/PasswordAuthentication.class \
$(libobjdir)/java/net/PlainDatagramSocketImpl.class \
$(libobjdir)/java/net/PlainSocketImpl.class \
$(libobjdir)/java/net/PortUnreachableException.class \
$(libobjdir)/java/net/ProtocolException.class	\
$(libobjdir)/java/net/ServerSocket.class \
$(libobjdir)/java/net/Socket.class \
$(libobjdir)/java/net/SocketAddress.class \
$(libobjdir)/java/net/SocketException.class \
$(libobjdir)/java/net/SocketImpl.class \
$(libobjdir)/java/net/SocketImplFactory.class	\
$(libobjdir)/java/net/SocketOptions.class \
$(libobjdir)/java/net/SocketPermission.class \
$(libobjdir)/java/net/SocketTimeoutException.class \
$(libobjdir)/java/net/URI.class \
$(libobjdir)/java/net/URISyntaxException.class \
$(libobjdir)/java/net/URL.class \
$(libobjdir)/java/net/URLClassLoader.class \
$(libobjdir)/java/net/URLConnection.class \
$(libobjdir)/java/net/URLDecoder.class \
$(libobjdir)/java/net/URLEncoder.class \
$(libobjdir)/java/net/URLStreamHandler.class \
$(libobjdir)/java/net/URLStreamHandlerFactory.class \
$(libobjdir)/java/net/UnknownHostException.class \
$(libobjdir)/java/net/UnknownServiceException.class \
$(libobjdir)/java/nio/Buffer.class \
$(libobjdir)/java/nio/BufferOverflowException.class \
$(libobjdir)/java/nio/BufferUnderflowException.class \
$(libobjdir)/java/nio/ByteBuffer.class \
$(libobjdir)/java/nio/ByteOrder.class \
$(libobjdir)/java/nio/CharBuffer.class \
$(libobjdir)/java/nio/DoubleBuffer.class \
$(libobjdir)/java/nio/FloatBuffer.class \
$(libobjdir)/java/nio/IntBuffer.class \
$(libobjdir)/java/nio/InvalidMarkException.class \
$(libobjdir)/java/nio/LongBuffer.class \
$(libobjdir)/java/nio/MappedByteBuffer.class \
$(libobjdir)/java/nio/ReadOnlyBufferException.class \
$(libobjdir)/java/nio/ShortBuffer.class \
$(libobjdir)/java/nio/channels/AlreadyConnectedException.class \
$(libobjdir)/java/nio/channels/AsynchronousCloseException.class \
$(libobjdir)/java/nio/channels/ByteChannel.class \
$(libobjdir)/java/nio/channels/CancelledKeyException.class \
$(libobjdir)/java/nio/channels/Channel.class \
$(libobjdir)/java/nio/channels/Channels.class \
$(libobjdir)/java/nio/channels/ClosedByInterruptException.class \
$(libobjdir)/java/nio/channels/ClosedChannelException.class \
$(libobjdir)/java/nio/channels/ClosedSelectorException.class \
$(libobjdir)/java/nio/channels/ConnectionPendingException.class \
$(libobjdir)/java/nio/channels/DatagramChannel.class \
$(libobjdir)/java/nio/channels/FileChannel.class \
$(libobjdir)/java/nio/channels/FileLock.class \
$(libobjdir)/java/nio/channels/FileLockInterruptionException.class \
$(libobjdir)/java/nio/channels/GatheringByteChannel.class \
$(libobjdir)/java/nio/channels/IllegalBlockingModeException.class \
$(libobjdir)/java/nio/channels/IllegalSelectorException.class \
$(libobjdir)/java/nio/channels/InterruptibleChannel.class \
$(libobjdir)/java/nio/channels/NoConnectionPendingException.class \
$(libobjdir)/java/nio/channels/NonReadableChannelException.class \
$(libobjdir)/java/nio/channels/NonWritableChannelException.class \
$(libobjdir)/java/nio/channels/NotYetBoundException.class \
$(libobjdir)/java/nio/channels/NotYetConnectedException.class \
$(libobjdir)/java/nio/channels/OverlappingFileLockException.class \
$(libobjdir)/java/nio/channels/Pipe.class \
$(libobjdir)/java/nio/channels/ReadableByteChannel.class \
$(libobjdir)/java/nio/channels/ScatteringByteChannel.class \
$(libobjdir)/java/nio/channels/SelectableChannel.class \
$(libobjdir)/java/nio/channels/SelectionKey.class \
$(libobjdir)/java/nio/channels/Selector.class \
$(libobjdir)/java/nio/channels/ServerSocketChannel.class \
$(libobjdir)/java/nio/channels/SocketChannel.class \
$(libobjdir)/java/nio/channels/UnresolvedAddressException.class \
$(libobjdir)/java/nio/channels/UnsupportedAddressTypeException.class \
$(libobjdir)/java/nio/channels/WritableByteChannel.class \
$(libobjdir)/java/nio/channels/spi/AbstractSelectableChannel.class \
$(libobjdir)/java/nio/channels/spi/AbstractInterruptibleChannel.class \
$(libobjdir)/java/nio/channels/spi/AbstractSelectionKey.class \
$(libobjdir)/java/nio/channels/spi/AbstractSelector.class \
$(libobjdir)/java/nio/channels/spi/SelectorProvider.class \
$(libobjdir)/java/nio/charset/Charset.class \
$(libobjdir)/java/nio/charset/CharacterCodingException.class \
$(libobjdir)/java/nio/charset/CharsetDecoder.class \
$(libobjdir)/java/nio/charset/CharsetEncoder.class \
$(libobjdir)/java/nio/charset/CoderMalfunctionError.class \
$(libobjdir)/java/nio/charset/CoderResult.class \
$(libobjdir)/java/nio/charset/CodingErrorAction.class \
$(libobjdir)/java/nio/charset/IllegalCharsetNameException.class \
$(libobjdir)/java/nio/charset/MalformedInputException.class \
$(libobjdir)/java/nio/charset/UnmappableCharacterException.class \
$(libobjdir)/java/nio/charset/UnsupportedCharsetException.class \
$(libobjdir)/java/nio/charset/spi/CharsetProvider.class \
$(libobjdir)/java/security/AccessControlContext.class \
$(libobjdir)/java/security/AccessControlException.class \
$(libobjdir)/java/security/AccessController.class \
$(libobjdir)/java/security/AlgorithmParameterGenerator.class \
$(libobjdir)/java/security/AlgorithmParameters.class \
$(libobjdir)/java/security/AlgorithmParameterGeneratorSpi.class \
$(libobjdir)/java/security/AlgorithmParametersSpi.class \
$(libobjdir)/java/security/AllPermission.class \
$(libobjdir)/java/security/BasicPermission.class \
$(libobjdir)/java/security/Certificate.class \
$(libobjdir)/java/security/CodeSource.class \
$(libobjdir)/java/security/DigestException.class \
$(libobjdir)/java/security/DigestOutputStream.class \
$(libobjdir)/java/security/DummyKeyPairGenerator.class \
$(libobjdir)/java/security/DummySignature.class \
$(libobjdir)/java/security/DigestInputStream.class \
$(libobjdir)/java/security/DomainCombiner.class \
$(libobjdir)/java/security/DummyMessageDigest.class \
$(libobjdir)/java/security/GeneralSecurityException.class \
$(libobjdir)/java/security/Guard.class \
$(libobjdir)/java/security/GuardedObject.class \
$(libobjdir)/java/security/Identity.class \
$(libobjdir)/java/security/IdentityScope.class \
$(libobjdir)/java/security/InvalidAlgorithmParameterException.class \
$(libobjdir)/java/security/InvalidKeyException.class \
$(libobjdir)/java/security/InvalidParameterException.class \
$(libobjdir)/java/security/Key.class \
$(libobjdir)/java/security/KeyFactorySpi.class \
$(libobjdir)/java/security/KeyPairGenerator.class \
$(libobjdir)/java/security/KeyStoreException.class \
$(libobjdir)/java/security/KeyException.class \
$(libobjdir)/java/security/KeyManagementException.class \
$(libobjdir)/java/security/KeyPairGeneratorSpi.class \
$(libobjdir)/java/security/KeyStoreSpi.class \
$(libobjdir)/java/security/KeyFactory.class \
$(libobjdir)/java/security/KeyPair.class \
$(libobjdir)/java/security/KeyStore.class \
$(libobjdir)/java/security/MessageDigest.class \
$(libobjdir)/java/security/MessageDigestSpi.class \
$(libobjdir)/java/security/NoSuchAlgorithmException.class \
$(libobjdir)/java/security/NoSuchProviderException.class \
$(libobjdir)/java/security/Permission.class \
$(libobjdir)/java/security/Principal.class \
$(libobjdir)/java/security/PrivilegedExceptionAction.class \
$(libobjdir)/java/security/PublicKey.class \
$(libobjdir)/java/security/PermissionCollection.class \
$(libobjdir)/java/security/PrivateKey.class \
$(libobjdir)/java/security/ProtectionDomain.class \
$(libobjdir)/java/security/Permissions.class \
$(libobjdir)/java/security/PrivilegedAction.class \
$(libobjdir)/java/security/Provider.class \
$(libobjdir)/java/security/Policy.class \
$(libobjdir)/java/security/PrivilegedActionException.class \
$(libobjdir)/java/security/ProviderException.class \
$(libobjdir)/java/security/SecureClassLoader.class \
$(libobjdir)/java/security/SecureRandomSpi.class \
$(libobjdir)/java/security/SecurityPermission.class \
$(libobjdir)/java/security/SignatureException.class \
$(libobjdir)/java/security/SignedObject.class \
$(libobjdir)/java/security/SecureRandom.class \
$(libobjdir)/java/security/Security.class \
$(libobjdir)/java/security/Signature.class \
$(libobjdir)/java/security/SignatureSpi.class \
$(libobjdir)/java/security/Signer.class \
$(libobjdir)/java/security/UnrecoverableKeyException.class \
$(libobjdir)/java/security/UnresolvedPermission.class \
$(libobjdir)/java/security/acl/Acl.class \
$(libobjdir)/java/security/acl/AclNotFoundException.class \
$(libobjdir)/java/security/acl/LastOwnerException.class \
$(libobjdir)/java/security/acl/Owner.class \
$(libobjdir)/java/security/acl/AclEntry.class \
$(libobjdir)/java/security/acl/Group.class \
$(libobjdir)/java/security/acl/NotOwnerException.class \
$(libobjdir)/java/security/acl/Permission.class \
$(libobjdir)/java/security/cert/CRL.class \
$(libobjdir)/java/security/cert/CRLException.class \
$(libobjdir)/java/security/cert/Certificate.class \
$(libobjdir)/java/security/cert/CertificateEncodingException.class \
$(libobjdir)/java/security/cert/CertificateException.class \
$(libobjdir)/java/security/cert/CertificateExpiredException.class \
$(libobjdir)/java/security/cert/CertificateFactory.class \
$(libobjdir)/java/security/cert/CertificateFactorySpi.class \
$(libobjdir)/java/security/cert/CertificateNotYetValidException.class \
$(libobjdir)/java/security/cert/CertificateParsingException.class \
$(libobjdir)/java/security/cert/CertPath.class \
$(libobjdir)/java/security/cert/CertPathBuilderException.class \
$(libobjdir)/java/security/cert/CertPathValidatorException.class \
$(libobjdir)/java/security/cert/CertStoreException.class \
$(libobjdir)/java/security/cert/X509CRL.class \
$(libobjdir)/java/security/cert/X509CRLEntry.class \
$(libobjdir)/java/security/cert/X509Certificate.class \
$(libobjdir)/java/security/cert/X509Extension.class \
$(libobjdir)/java/security/interfaces/DSAKey.class \
$(libobjdir)/java/security/interfaces/DSAKeyPairGenerator.class \
$(libobjdir)/java/security/interfaces/DSAParams.class \
$(libobjdir)/java/security/interfaces/DSAPrivateKey.class \
$(libobjdir)/java/security/interfaces/DSAPublicKey.class \
$(libobjdir)/java/security/interfaces/RSAKey.class \
$(libobjdir)/java/security/interfaces/RSAPrivateCrtKey.class \
$(libobjdir)/java/security/interfaces/RSAPrivateKey.class \
$(libobjdir)/java/security/interfaces/RSAPublicKey.class \
$(libobjdir)/java/security/spec/AlgorithmParameterSpec.class \
$(libobjdir)/java/security/spec/DSAParameterSpec.class \
$(libobjdir)/java/security/spec/DSAPrivateKeySpec.class \
$(libobjdir)/java/security/spec/DSAPublicKeySpec.class \
$(libobjdir)/java/security/spec/EncodedKeySpec.class \
$(libobjdir)/java/security/spec/InvalidKeySpecException.class \
$(libobjdir)/java/security/spec/InvalidParameterSpecException.class \
$(libobjdir)/java/security/spec/KeySpec.class \
$(libobjdir)/java/security/spec/PKCS8EncodedKeySpec.class \
$(libobjdir)/java/security/spec/RSAKeyGenParameterSpec.class \
$(libobjdir)/java/security/spec/RSAPrivateCrtKeySpec.class \
$(libobjdir)/java/security/spec/RSAPrivateKeySpec.class \
$(libobjdir)/java/security/spec/RSAPublicKeySpec.class \
$(libobjdir)/java/security/spec/X509EncodedKeySpec.class \
$(libobjdir)/java/sql/Array.class \
$(libobjdir)/java/sql/BatchUpdateException.class \
$(libobjdir)/java/sql/Blob.class \
$(libobjdir)/java/sql/CallableStatement.class	\
$(libobjdir)/java/sql/Clob.class \
$(libobjdir)/java/sql/Connection.class \
$(libobjdir)/java/sql/DataTruncation.class \
$(libobjdir)/java/sql/DatabaseMetaData.class \
$(libobjdir)/java/sql/Date.class \
$(libobjdir)/java/sql/Driver.class \
$(libobjdir)/java/sql/DriverManager.class \
$(libobjdir)/java/sql/DriverPropertyInfo.class \
$(libobjdir)/java/sql/ParameterMetaData.class \
$(libobjdir)/java/sql/PreparedStatement.class	\
$(libobjdir)/java/sql/Ref.class \
$(libobjdir)/java/sql/ResultSet.class	\
$(libobjdir)/java/sql/ResultSetMetaData.class	\
$(libobjdir)/java/sql/Savepoint.class \
$(libobjdir)/java/sql/SQLData.class \
$(libobjdir)/java/sql/SQLException.class \
$(libobjdir)/java/sql/SQLInput.class \
$(libobjdir)/java/sql/SQLOutput.class \
$(libobjdir)/java/sql/SQLPermission.class \
$(libobjdir)/java/sql/SQLWarning.class \
$(libobjdir)/java/sql/Statement.class	\
$(libobjdir)/java/sql/Struct.class	\
$(libobjdir)/java/sql/Time.class \
$(libobjdir)/java/sql/Timestamp.class	\
$(libobjdir)/java/sql/Types.class \
$(libobjdir)/java/text/Annotation.class \
$(libobjdir)/java/text/AttributedCharacterIterator.class \
$(libobjdir)/java/text/AttributedString.class	\
$(libobjdir)/java/text/AttributedStringIterator.class	\
$(libobjdir)/java/text/BreakIterator.class \
$(libobjdir)/java/text/CharacterIterator.class \
$(libobjdir)/java/text/ChoiceFormat.class \
$(libobjdir)/java/text/CollationElementIterator.class	\
$(libobjdir)/java/text/CollationKey.class \
$(libobjdir)/java/text/Collator.class	\
$(libobjdir)/java/text/DateFormat.class \
$(libobjdir)/java/text/DateFormatSymbols.class \
$(libobjdir)/java/text/DecimalFormat.class \
$(libobjdir)/java/text/DecimalFormatSymbols.class \
$(libobjdir)/java/text/FieldPosition.class \
$(libobjdir)/java/text/Format.class \
$(libobjdir)/java/text/MessageFormat.class \
$(libobjdir)/java/text/NumberFormat.class \
$(libobjdir)/java/text/ParseException.class \
$(libobjdir)/java/text/ParsePosition.class \
$(libobjdir)/java/text/RuleBasedCollator.class \
$(libobjdir)/java/text/SimpleDateFormat.class	\
$(libobjdir)/java/text/StringCharacterIterator.class \
$(libobjdir)/java/util/jar/Attributes.class \
$(libobjdir)/java/util/jar/JarEntry.class \
$(libobjdir)/java/util/jar/JarException.class \
$(libobjdir)/java/util/jar/JarFile.class \
$(libobjdir)/java/util/jar/JarInputStream.class \
$(libobjdir)/java/util/jar/JarOutputStream.class \
$(libobjdir)/java/util/jar/Manifest.class \
$(libobjdir)/java/util/zip/Adler32.class \
$(libobjdir)/java/util/zip/CRC32.class \
$(libobjdir)/java/util/zip/CheckedInputStream.class \
$(libobjdir)/java/util/zip/CheckedOutputStream.class \
$(libobjdir)/java/util/zip/Checksum.class \
$(libobjdir)/java/util/zip/DataFormatException.class \
$(libobjdir)/java/util/zip/Deflater.class \
$(libobjdir)/java/util/zip/DeflaterOutputStream.class	\
$(libobjdir)/java/util/zip/GZIPInputStream.class \
$(libobjdir)/java/util/zip/GZIPOutputStream.class \
$(libobjdir)/java/util/zip/Inflater.class \
$(libobjdir)/java/util/zip/InflaterInputStream.class \
$(libobjdir)/java/util/zip/ZipConstants.class	\
$(libobjdir)/java/util/zip/ZipEntry.class \
$(libobjdir)/java/util/zip/ZipException.class	\
$(libobjdir)/java/util/zip/ZipFile.class \
$(libobjdir)/java/util/zip/ZipInputStream.class \
$(libobjdir)/java/util/zip/ZipOutputStream.class \
$(rmi_java_source_files) \
$(awt_java_source_files) \
$(convert_source_files) \
$(javax_source_files)


java_source_files = $(special_java_source_files) $(ordinary_java_source_files)

## Math library: C, not C++
c_source_files = \
  java/lang/dtoa.c        java/lang/k_rem_pio2.c  java/lang/s_tan.c       \
  java/lang/e_acos.c      java/lang/k_sin.c       java/lang/strtod.c      \
  java/lang/e_asin.c      java/lang/k_tan.c       java/lang/w_acos.c      \
  java/lang/e_atan2.c     java/lang/mprec.c       java/lang/w_asin.c      \
  java/lang/e_exp.c       java/lang/s_atan.c      java/lang/w_atan2.c     \
  java/lang/e_fmod.c      java/lang/s_ceil.c      java/lang/w_exp.c       \
  java/lang/e_log.c       java/lang/s_copysign.c  java/lang/w_fmod.c      \
  java/lang/e_pow.c       java/lang/s_cos.c       java/lang/w_log.c       \
  java/lang/e_rem_pio2.c  java/lang/s_fabs.c      java/lang/w_pow.c       \
  java/lang/e_remainder.c java/lang/s_floor.c     java/lang/w_remainder.c \
  java/lang/e_scalb.c     java/lang/s_rint.c      java/lang/w_sqrt.c      \
  java/lang/e_sqrt.c      java/lang/s_scalbn.c    java/lang/sf_rint.c     \
  java/lang/k_cos.c       java/lang/s_sin.c       java/lang/sf_fabs.c

#java/awt/natToolkit.cc

## This lists all the C++ source files in subdirectories.
nat_source_files = \
$(libobjdir)/gnu/gcj/natCore.o \
$(libobjdir)/gnu/gcj/convert/JIS0208_to_Unicode.o \
$(libobjdir)/gnu/gcj/convert/JIS0212_to_Unicode.o \
$(libobjdir)/gnu/gcj/convert/Unicode_to_JIS.o \
$(libobjdir)/gnu/gcj/convert/natIconv.o \
$(libobjdir)/gnu/gcj/convert/natInput_EUCJIS.o \
$(libobjdir)/gnu/gcj/convert/natInput_SJIS.o \
$(libobjdir)/gnu/gcj/convert/natOutput_EUCJIS.o \
$(libobjdir)/gnu/gcj/convert/natOutput_SJIS.o \
$(libobjdir)/gnu/gcj/io/natSimpleSHSStream.o \
$(libobjdir)/gnu/gcj/io/shs.o \
$(libobjdir)/gnu/gcj/protocol/core/natCoreInputStream.o \
$(libobjdir)/gnu/gcj/runtime/natFinalizerThread.o \
$(libobjdir)/gnu/gcj/runtime/natFirstThread.o \
$(libobjdir)/gnu/gcj/runtime/natNameFinder.o \
$(libobjdir)/gnu/gcj/runtime/natSharedLibLoader.o \
$(libobjdir)/gnu/gcj/runtime/natStackTrace.o \
$(libobjdir)/gnu/gcj/runtime/natStringBuffer.o \
$(libobjdir)/gnu/gcj/runtime/natVMClassLoader.o \
$(libobjdir)/gnu/java/nio/natByteBufferImpl.o \
$(libobjdir)/gnu/java/nio/natCharBufferImpl.o \
$(libobjdir)/gnu/java/nio/natDoubleBufferImpl.o \
$(libobjdir)/gnu/java/nio/natFileChannelImpl.o \
$(libobjdir)/gnu/java/nio/natFloatBufferImpl.o \
$(libobjdir)/gnu/java/nio/natIntBufferImpl.o \
$(libobjdir)/gnu/java/nio/natLongBufferImpl.o \
$(libobjdir)/gnu/java/nio/natSelectorImpl.o \
$(libobjdir)/gnu/java/nio/natShortBufferImpl.o \
$(libobjdir)/gnu/java/nio/natSocketChannelImpl.o \
$(libobjdir)/java/io/natFile.o \
$(libobjdir)/java/io/natFileDescriptor.o \
$(libobjdir)/java/io/natObjectInputStream.o \
$(libobjdir)/java/io/natVMObjectStreamClass.o \
$(libobjdir)/java/lang/natCharacter.o \
$(libobjdir)/java/lang/natClass.o \
$(libobjdir)/java/lang/natClassLoader.o \
$(libobjdir)/java/lang/natConcreteProcess.o \
$(libobjdir)/java/lang/natDouble.o \
$(libobjdir)/java/lang/natFloat.o \
$(libobjdir)/java/lang/natMath.o \
$(libobjdir)/java/lang/natObject.o \
$(libobjdir)/java/lang/natRuntime.o	\
$(libobjdir)/java/lang/natString.o \
$(libobjdir)/java/lang/natStringBuffer.o \
$(libobjdir)/java/lang/natSystem.o \
$(libobjdir)/java/lang/natThread.o \
$(libobjdir)/java/lang/natVMSecurityManager.o \
$(libobjdir)/java/lang/ref/natReference.o \
$(libobjdir)/java/lang/reflect/natArray.o \
$(libobjdir)/java/lang/reflect/natConstructor.o \
$(libobjdir)/java/lang/reflect/natField.o \
$(libobjdir)/java/lang/reflect/natMethod.o \
$(libobjdir)/java/lang/reflect/natProxy.o \
$(libobjdir)/java/net/natNetworkInterface.o \
$(libobjdir)/java/net/natInetAddress.o \
$(libobjdir)/java/net/natPlainDatagramSocketImpl.o \
$(libobjdir)/java/net/natPlainSocketImpl.o \
$(libobjdir)/java/text/natCollator.o \
$(libobjdir)/java/util/natResourceBundle.o \
$(libobjdir)/java/util/natTimeZone.o \
$(libobjdir)/java/util/zip/natDeflater.o \
$(libobjdir)/java/util/zip/natInflater.o

x_java_source_files = \
$(libobjdir)/gnu/gcj/xlib/Clip.class \
$(libobjdir)/gnu/gcj/xlib/Colormap.class \
$(libobjdir)/gnu/gcj/xlib/Display.class \
$(libobjdir)/gnu/gcj/xlib/Drawable.class \
$(libobjdir)/gnu/gcj/xlib/Font.class \
$(libobjdir)/gnu/gcj/xlib/GC.class \
$(libobjdir)/gnu/gcj/xlib/Pixmap.class \
$(libobjdir)/gnu/gcj/xlib/Screen.class \
$(libobjdir)/gnu/gcj/xlib/Visual.class \
$(libobjdir)/gnu/gcj/xlib/WMSizeHints.class \
$(libobjdir)/gnu/gcj/xlib/Window.class \
$(libobjdir)/gnu/gcj/xlib/WindowAttributes.class \
$(libobjdir)/gnu/gcj/xlib/XAnyEvent.class \
$(libobjdir)/gnu/gcj/xlib/XButtonEvent.class \
$(libobjdir)/gnu/gcj/xlib/XColor.class \
$(libobjdir)/gnu/gcj/xlib/XConfigureEvent.class \
$(libobjdir)/gnu/gcj/xlib/XConnectException.class \
$(libobjdir)/gnu/gcj/xlib/XEvent.class \
$(libobjdir)/gnu/gcj/xlib/XException.class \
$(libobjdir)/gnu/gcj/xlib/XExposeEvent.class \
$(libobjdir)/gnu/gcj/xlib/XID.class \
$(libobjdir)/gnu/gcj/xlib/XImage.class \
$(libobjdir)/gnu/gcj/xlib/XUnmapEvent.class \
$(libobjdir)/gnu/awt/xlib/XCanvasPeer.class \
$(libobjdir)/gnu/awt/xlib/XEventLoop.class \
$(libobjdir)/gnu/awt/xlib/XEventQueue.class \
$(libobjdir)/gnu/awt/xlib/XFontMetrics.class \
$(libobjdir)/gnu/awt/xlib/XFramePeer.class \
$(libobjdir)/gnu/awt/xlib/XGraphics.class \
$(libobjdir)/gnu/awt/xlib/XGraphicsConfiguration.class \
$(libobjdir)/gnu/awt/xlib/XPanelPeer.class \
$(libobjdir)/gnu/awt/xlib/XToolkit.class

x_nat_source_files = \
$(libobjdir)/gnu/gcj/xlib/natClip.o \
$(libobjdir)/gnu/gcj/xlib/natColormap.o \
$(libobjdir)/gnu/gcj/xlib/natDisplay.o \
$(libobjdir)/gnu/gcj/xlib/natDrawable.o \
$(libobjdir)/gnu/gcj/xlib/natFont.o \
$(libobjdir)/gnu/gcj/xlib/natGC.o \
$(libobjdir)/gnu/gcj/xlib/natPixmap.o \
$(libobjdir)/gnu/gcj/xlib/natScreen.o \
$(libobjdir)/gnu/gcj/xlib/natVisual.o \
$(libobjdir)/gnu/gcj/xlib/natWMSizeHints.o \
$(libobjdir)/gnu/gcj/xlib/natWindow.o \
$(libobjdir)/gnu/gcj/xlib/natWindowAttributes.o \
$(libobjdir)/gnu/gcj/xlib/natXAnyEvent.o \
$(libobjdir)/gnu/gcj/xlib/natXButtonEvent.o \
$(libobjdir)/gnu/gcj/xlib/natXColor.o \
$(libobjdir)/gnu/gcj/xlib/natXConfigureEvent.o \
$(libobjdir)/gnu/gcj/xlib/natXException.o \
$(libobjdir)/gnu/gcj/xlib/natXExposeEvent.o \
$(libobjdir)/gnu/gcj/xlib/natXImage.o \
$(libobjdir)/gnu/gcj/xlib/natXUnmapEvent.o


libgcj_objs = \
$(libobjdir)/prims.o \
$(libobjdir)/jni.o \
$(libobjdir)/exception.o \
$(libobjdir)/resolve.o \
$(libobjdir)/defineclass.o \
$(libobjdir)/interpret.o \
$(libobjdir)/verify.o \
$(nat_source_files)


all_java_source_files = \
    $(java_source_files) \
    $(built_java_source_files) \
    $(lib_org_xml_sax_la_SOURCES) \
    $(lib_org_w3c_dom_la_SOURCES) \
    $(x_java_source_files)

all_java_class_files = $(all_java_source_files)

#all_java_class_files = $(all_java_source_files:.java=.class)

## ################################################################

$(ux_gccpkg)/$(gn_share_dir)/libgcj.jar: $(all_java_class_files)
	$(ZIP) -cf $@ -C $(libobjdir) gnu -C $(libobjdir) java -C $(libobjdir) javax -C $(libobjdir) org

$(ro_gccpkg)/$(gn_share_dir)/jar/libgcj: $(all_java_class_files)
	$(ZIP) -cf $@ -C $(libobjdir) gnu -C $(libobjdir) java -C $(libobjdir) javax -C $(libobjdir) org

$(ux_gcc_bin_dir)/$(TRG)/libgcj.o: $(libgcj_objs)
	$(CROSS_AR) $(CROSS_AR_FLAGS) $@ $(libgcj_objs)

$(ux_gcc_bin_dir)/libgcj.jar: $(all_java_class_files)
	$(ZIP) -c $@ $(all_java_class_files)

$(ro_gcc_bin_dir)/$(TRG)/o/libgcj: $(libgcj_objs)
	$(CROSS_AR) $(CROSS_AR_FLAGS) $@ $(libgcj_objs)


$(libobjdir)/java/lang/natRuntime.o: java/lang/natRuntime.cc
	$(CROSS_CXX) -o $@ -c $< $(CROSS_CXXFLAGS) $(INCLUDES) $(CFL) -DPREFIX=\"gccpkg:\"


## ################################################################


$(libobjdir)/boehm.o: boehm.cc
$(libobjdir)/defineclass.o: defineclass.cc
$(libobjdir)/exception.o: exception.cc
$(libobjdir)/gij.o: gij.cc gcj/cni.h java/lang/Object.h gcj/javaprims.h \
  gcj/libgcj-config.h java/lang/Class.h
$(libobjdir)/interpret.o: interpret.cc
$(libobjdir)/jni.o: jni.cc include/config.h gcj/cni.h java/lang/Object.h \
  gcj/javaprims.h gcj/libgcj-config.h java/lang/Class.h \
  java/lang/String.h gcj/array.h java/net/URL.h \
  java/lang/reflect/Modifier.h java/security/ProtectionDomain.h \
  java/lang/Package.h gnu/gcj/runtime/StackTrace.h include/jvm.h \
  include/java-assert.h include/java-threads.h include/java-gc.h \
  gcj/field.h java/lang/reflect/Field.h \
  java/lang/reflect/AccessibleObject.h gnu/gcj/RawData.h include/jvmpi.h \
  include/jni.h gnu/gcj/runtime/JNIWeakRef.h \
  java/lang/ref/WeakReference.h java/lang/ref/Reference.h \
  java/lang/ClassLoader.h java/lang/Throwable.h \
  java/lang/ArrayIndexOutOfBoundsException.h \
  java/lang/IndexOutOfBoundsException.h java/lang/RuntimeException.h \
  java/lang/Exception.h java/lang/StringIndexOutOfBoundsException.h \
  java/lang/UnsatisfiedLinkError.h java/lang/LinkageError.h \
  java/lang/Error.h java/lang/InstantiationException.h \
  java/lang/NoSuchFieldError.h java/lang/IncompatibleClassChangeError.h \
  java/lang/NoSuchMethodError.h java/lang/reflect/Constructor.h \
  java/lang/reflect/Method.h java/lang/OutOfMemoryError.h \
  java/lang/VirtualMachineError.h java/util/IdentityHashMap.h \
  java/util/AbstractMap.h java/lang/Integer.h java/lang/Number.h \
  java/lang/ThreadGroup.h java/lang/Thread.h gcj/method.h \
  include/java-interp.h include/java-cpool.h gnu/gcj/runtime/NameFinder.h \
  ../libffi/include/ffi.h ../libffi/include/fficonfig.h

$(libobjdir)/no-threads.o: no-threads.cc
$(libobjdir)/nogc.o: nogc.cc
$(libobjdir)/posix-threads.o: posix-threads.cc
$(libobjdir)/posix.o: posix.cc
$(libobjdir)/prims.o: prims.cc include/config.h include/platform.h gcj/cni.h \
  java/lang/Object.h gcj/javaprims.h gcj/libgcj-config.h \
  java/lang/Class.h java/lang/String.h gcj/array.h java/net/URL.h \
  java/lang/reflect/Modifier.h java/security/ProtectionDomain.h \
  java/lang/Package.h gnu/gcj/runtime/StackTrace.h java/util/Properties.h \
  java/net/InetAddress.h include/jvm.h include/java-assert.h \
  include/java-threads.h include/java-gc.h gcj/field.h \
  java/lang/reflect/Field.h gnu/gcj/RawData.h include/jvmpi.h \
  include/jni.h gnu/gcj/runtime/JNIWeakRef.h include/java-signal.h \
  java/lang/ThreadGroup.h include/java-props.h java/lang/ClassLoader.h \
  java/lang/Runtime.h java/lang/Thread.h \
  java/lang/ArrayIndexOutOfBoundsException.h \
  java/lang/ArithmeticException.h java/lang/ClassFormatError.h \
  java/lang/InternalError.h java/lang/VirtualMachineError.h \
  java/lang/NegativeArraySizeException.h java/lang/NullPointerException.h \
  java/lang/OutOfMemoryError.h java/lang/System.h java/lang/VMThrowable.h \
  java/io/PrintStream.h java/lang/UnsatisfiedLinkError.h \
  gnu/gcj/runtime/VMClassLoader.h gnu/gcj/runtime/FinalizerThread.h \
  gnu/gcj/runtime/FirstThread.h java/util/Hashtable.h \
  java/lang/reflect/AccessibleObject.h java/lang/ref/WeakReference.h \
  java/lang/IndexOutOfBoundsException.h java/lang/RuntimeException.h \
  java/lang/LinkageError.h java/lang/Error.h \
  java/io/FilterOutputStream.h java/net/URLClassLoader.h \
  java/util/Dictionary.h java/lang/ref/Reference.h \
  java/lang/Exception.h java/io/OutputStream.h \
  java/security/SecureClassLoader.h java/lang/Throwable.h

$(libobjdir)/resolve.o: resolve.cc include/config.h include/java-interp.h \
  include/jvm.h gcj/javaprims.h gcj/libgcj-config.h include/java-assert.h \
  include/java-threads.h include/java-gc.h java/lang/Class.h \
  java/lang/Object.h java/lang/String.h gcj/array.h java/net/URL.h \
  java/lang/reflect/Modifier.h java/security/ProtectionDomain.h \
  java/lang/Package.h gnu/gcj/runtime/StackTrace.h gcj/cni.h gcj/field.h \
  java/lang/reflect/Field.h java/lang/reflect/AccessibleObject.h \
  gnu/gcj/RawData.h include/jvmpi.h include/jni.h \
  gnu/gcj/runtime/JNIWeakRef.h java/lang/ref/WeakReference.h \
  java/lang/ref/Reference.h include/java-cpool.h \
  gnu/gcj/runtime/NameFinder.h java/lang/ClassLoader.h \
  ../libffi/include/ffi.h ../libffi/include/fficonfig.h \
  java/lang/Thread.h java/lang/InternalError.h \
  java/lang/VirtualMachineError.h java/lang/Error.h java/lang/Throwable.h \
  java/lang/NoSuchFieldError.h java/lang/IncompatibleClassChangeError.h \
  java/lang/LinkageError.h java/lang/NoSuchMethodError.h \
  java/lang/ClassFormatError.h java/lang/StringBuffer.h \
 java/lang/IllegalAccessError.h java/lang/AbstractMethodError.h \
 java/lang/NoClassDefFoundError.h

$(libobjdir)/verify.o: verify.cc
$(libobjdir)/win32-threads.o: win32-threads.cc
$(libobjdir)/win32.o: win32.cc
$(libobjdir)/gnu/awt/gtk/gtkcommon.o: gnu/awt/gtk/gtkcommon.cc
$(libobjdir)/gnu/awt/gtk/natGtkButtonPeer.o: gnu/awt/gtk/natGtkButtonPeer.cc
$(libobjdir)/gnu/awt/gtk/natGtkComponentPeer.o: gnu/awt/gtk/natGtkComponentPeer.cc
$(libobjdir)/gnu/awt/gtk/natGtkContainerPeer.o: gnu/awt/gtk/natGtkContainerPeer.cc
$(libobjdir)/gnu/awt/gtk/natGtkFramePeer.o: gnu/awt/gtk/natGtkFramePeer.cc
$(libobjdir)/gnu/awt/gtk/natGtkLabelPeer.o: gnu/awt/gtk/natGtkLabelPeer.cc
$(libobjdir)/gnu/awt/gtk/natGtkMainThread.o: gnu/awt/gtk/natGtkMainThread.cc
$(libobjdir)/gnu/awt/gtk/natGtkToolkit.o: gnu/awt/gtk/natGtkToolkit.cc
$(libobjdir)/gnu/awt/gtk/natGtkWindowPeer.o: gnu/awt/gtk/natGtkWindowPeer.cc
$(libobjdir)/gnu/gcj/convert/JIS0208_to_Unicode.o: gnu/gcj/convert/JIS0208_to_Unicode.cc
$(libobjdir)/gnu/gcj/convert/JIS0212_to_Unicode.o: gnu/gcj/convert/JIS0212_to_Unicode.cc
$(libobjdir)/gnu/gcj/convert/Unicode_to_JIS.o: gnu/gcj/convert/Unicode_to_JIS.cc
$(libobjdir)/gnu/gcj/convert/natIconv.o: gnu/gcj/convert/natIconv.cc
$(libobjdir)/gnu/gcj/convert/natInput_EUCJIS.o: gnu/gcj/convert/natInput_EUCJIS.cc
$(libobjdir)/gnu/gcj/convert/natInput_SJIS.o: gnu/gcj/convert/natInput_SJIS.cc
$(libobjdir)/gnu/gcj/convert/natOutput_EUCJIS.o: gnu/gcj/convert/natOutput_EUCJIS.cc
$(libobjdir)/gnu/gcj/convert/natOutput_SJIS.o: gnu/gcj/convert/natOutput_SJIS.cc
$(libobjdir)/gnu/gcj/natCore.o: gnu/gcj/natCore.cc
$(libobjdir)/gnu/gcj/io/natSimpleSHSStream.o: gnu/gcj/io/natSimpleSHSStream.cc
$(libobjdir)/gnu/gcj/io/shs.o: gnu/gcj/io/shs.cc
$(libobjdir)/gnu/gcj/protocol/core/natCoreInputStream.o: gnu/gcj/protocol/core/natCoreInputStream.cc
$(libobjdir)/gnu/gcj/runtime/natFinalizerThread.o: gnu/gcj/runtime/natFinalizerThread.cc
$(libobjdir)/gnu/gcj/runtime/natFirstThread.o: gnu/gcj/runtime/natFirstThread.cc
$(libobjdir)/gnu/gcj/runtime/natNameFinder.o: gnu/gcj/runtime/natNameFinder.cc
$(libobjdir)/gnu/gcj/runtime/natSharedLibLoader.o: gnu/gcj/runtime/natSharedLibLoader.cc
$(libobjdir)/gnu/gcj/runtime/natStackTrace.o: gnu/gcj/runtime/natStackTrace.cc
$(libobjdir)/gnu/gcj/runtime/natStringBuffer.o: gnu/gcj/runtime/natStringBuffer.cc
$(libobjdir)/gnu/gcj/runtime/natVMClassLoader.o: gnu/gcj/runtime/natVMClassLoader.cc
$(libobjdir)/gnu/gcj/xlib/natClip.o: gnu/gcj/xlib/natClip.cc
$(libobjdir)/gnu/gcj/xlib/natColormap.o: gnu/gcj/xlib/natColormap.cc
$(libobjdir)/gnu/gcj/xlib/natDisplay.o: gnu/gcj/xlib/natDisplay.cc
$(libobjdir)/gnu/gcj/xlib/natDrawable.o: gnu/gcj/xlib/natDrawable.cc
$(libobjdir)/gnu/gcj/xlib/natFont.o: gnu/gcj/xlib/natFont.cc
$(libobjdir)/gnu/gcj/xlib/natGC.o: gnu/gcj/xlib/natGC.cc
$(libobjdir)/gnu/gcj/xlib/natPixmap.o: gnu/gcj/xlib/natPixmap.cc
$(libobjdir)/gnu/gcj/xlib/natScreen.o: gnu/gcj/xlib/natScreen.cc
$(libobjdir)/gnu/gcj/xlib/natVisual.o: gnu/gcj/xlib/natVisual.cc
$(libobjdir)/gnu/gcj/xlib/natWMSizeHints.o: gnu/gcj/xlib/natWMSizeHints.cc
$(libobjdir)/gnu/gcj/xlib/natWindow.o: gnu/gcj/xlib/natWindow.cc
$(libobjdir)/gnu/gcj/xlib/natWindowAttributes.o: gnu/gcj/xlib/natWindowAttributes.cc
$(libobjdir)/gnu/gcj/xlib/natXAnyEvent.o: gnu/gcj/xlib/natXAnyEvent.cc
$(libobjdir)/gnu/gcj/xlib/natXButtonEvent.o: gnu/gcj/xlib/natXButtonEvent.cc
$(libobjdir)/gnu/gcj/xlib/natXColor.o: gnu/gcj/xlib/natXColor.cc
$(libobjdir)/gnu/gcj/xlib/natXConfigureEvent.o: gnu/gcj/xlib/natXConfigureEvent.cc
$(libobjdir)/gnu/gcj/xlib/natXException.o: gnu/gcj/xlib/natXException.cc
$(libobjdir)/gnu/gcj/xlib/natXExposeEvent.o: gnu/gcj/xlib/natXExposeEvent.cc
$(libobjdir)/gnu/gcj/xlib/natXImage.o: gnu/gcj/xlib/natXImage.cc
$(libobjdir)/gnu/gcj/xlib/natXUnmapEvent.o: gnu/gcj/xlib/natXUnmapEvent.cc
$(libobjdir)/gnu/java/nio/natByteBufferImpl.o: gnu/java/nio/natByteBufferImpl.cc
$(libobjdir)/gnu/java/nio/natCharBufferImpl.o: gnu/java/nio/natCharBufferImpl.cc
$(libobjdir)/gnu/java/nio/natDoubleBufferImpl.o: gnu/java/nio/natDoubleBufferImpl.cc
$(libobjdir)/gnu/java/nio/natFileChannelImpl.o: gnu/java/nio/natFileChannelImpl.cc
$(libobjdir)/gnu/java/nio/natFloatBufferImpl.o: gnu/java/nio/natFloatBufferImpl.cc
$(libobjdir)/gnu/java/nio/natIntBufferImpl.o: gnu/java/nio/natIntBufferImpl.cc
$(libobjdir)/gnu/java/nio/natLongBufferImpl.o: gnu/java/nio/natLongBufferImpl.cc
$(libobjdir)/gnu/java/nio/natSelectorImpl.o: gnu/java/nio/natSelectorImpl.cc
$(libobjdir)/gnu/java/nio/natShortBufferImpl.o: gnu/java/nio/natShortBufferImpl.cc
$(libobjdir)/gnu/java/nio/natSocketChannelImpl.o: gnu/java/nio/natSocketChannelImpl.cc
$(libobjdir)/java/io/natFileDescriptorEcos.o: java/io/natFileDescriptorEcos.cc
$(libobjdir)/java/io/natFileDescriptorPosix.o: java/io/natFileDescriptorPosix.cc
$(libobjdir)/java/io/natFileDescriptorWin32.o: java/io/natFileDescriptorWin32.cc
$(libobjdir)/java/io/natFilePosix.o: java/io/natFilePosix.cc
$(libobjdir)/java/io/natFileWin32.o: java/io/natFileWin32.cc
$(libobjdir)/java/io/natObjectInputStream.o: java/io/natObjectInputStream.cc java/io/ObjectInputStream$$GetField.h
$(libobjdir)/java/io/natVMObjectStreamClass.o: java/io/natVMObjectStreamClass.cc
$(libobjdir)/java/lang/ref/natReference.o: java/lang/ref/natReference.cc
$(libobjdir)/java/lang/natCharacter.o: java/lang/natCharacter.cc
$(libobjdir)/java/lang/natClass.o: java/lang/natClass.cc
$(libobjdir)/java/lang/natClassLoader.o: java/lang/natClassLoader.cc
$(libobjdir)/java/lang/natDouble.o: java/lang/natDouble.cc
$(libobjdir)/java/lang/natEcosProcess.o: java/lang/natEcosProcess.cc
$(libobjdir)/java/lang/natFloat.o: java/lang/natFloat.cc
$(libobjdir)/java/lang/natMath.o: java/lang/natMath.cc
$(libobjdir)/java/lang/natObject.o: java/lang/natObject.cc
$(libobjdir)/java/lang/natPosixProcess.o: java/lang/natPosixProcess.cc
$(libobjdir)/java/lang/natString.o: java/lang/natString.cc
$(libobjdir)/java/lang/natStringBuffer.o: java/lang/natStringBuffer.cc
$(libobjdir)/java/lang/natSystem.o: java/lang/natSystem.cc
$(libobjdir)/java/lang/natThread.o: java/lang/natThread.cc
$(libobjdir)/java/lang/natVMSecurityManager.o: java/lang/natVMSecurityManager.cc
$(libobjdir)/java/lang/natWin32Process.o: java/lang/natWin32Process.cc
$(libobjdir)/java/lang/reflect/natArray.o: java/lang/reflect/natArray.cc
$(libobjdir)/java/lang/reflect/natConstructor.o: java/lang/reflect/natConstructor.cc
$(libobjdir)/java/lang/reflect/natField.o: java/lang/reflect/natField.cc
$(libobjdir)/java/lang/reflect/natMethod.o: java/lang/reflect/natMethod.cc
$(libobjdir)/java/lang/reflect/natProxy.o: java/lang/reflect/natProxy.cc java/lang/reflect/Proxy$$ProxyData.h
$(libobjdir)/java/net/natInetAddress.o: java/net/natInetAddress.cc
$(libobjdir)/java/net/natNetworkInterface.o: java/net/natNetworkInterface.cc
$(libobjdir)/java/net/natPlainDatagramSocketImpl.o: java/net/natPlainDatagramSocketImpl.cc
$(libobjdir)/java/net/natPlainSocketImpl.o: java/net/natPlainSocketImpl.cc
$(libobjdir)/java/text/natCollator.o: java/text/natCollator.cc
$(libobjdir)/java/util/zip/natDeflater.o: java/util/zip/natDeflater.cc
$(libobjdir)/java/util/zip/natInflater.o: java/util/zip/natInflater.cc
$(libobjdir)/java/util/natResourceBundle.o: java/util/natResourceBundle.cc
$(libobjdir)/java/util/natTimeZone.o: java/util/natTimeZone.cc

$(libobjdir)/gnu/gcj/convert/gen-from-JIS.o: gnu/gcj/convert/gen-from-JIS.c
$(libobjdir)/gnu/gcj/convert/make-trie.o: gnu/gcj/convert/make-trie.c
$(libobjdir)/java/lang/dtoa.o: java/lang/dtoa.c
$(libobjdir)/java/lang/e_acos.o: java/lang/e_acos.c
$(libobjdir)/java/lang/e_asin.o: java/lang/e_asin.c
$(libobjdir)/java/lang/e_atan2.o: java/lang/e_atan2.c
$(libobjdir)/java/lang/e_exp.o: java/lang/e_exp.c
$(libobjdir)/java/lang/e_fmod.o: java/lang/e_fmod.c
$(libobjdir)/java/lang/e_log.o: java/lang/e_log.c
$(libobjdir)/java/lang/e_pow.o: java/lang/e_pow.c
$(libobjdir)/java/lang/e_rem_pio2.o: java/lang/e_rem_pio2.c
$(libobjdir)/java/lang/e_remainder.o: java/lang/e_remainder.c
$(libobjdir)/java/lang/e_scalb.o: java/lang/e_scalb.c
$(libobjdir)/java/lang/e_sqrt.o: java/lang/e_sqrt.c
$(libobjdir)/java/lang/k_cos.o: java/lang/k_cos.c
$(libobjdir)/java/lang/k_rem_pio2.o: java/lang/k_rem_pio2.c
$(libobjdir)/java/lang/k_sin.o: java/lang/k_sin.c
$(libobjdir)/java/lang/k_tan.o: java/lang/k_tan.c
$(libobjdir)/java/lang/mprec.o: java/lang/mprec.c
$(libobjdir)/java/lang/s_atan.o: java/lang/s_atan.c
$(libobjdir)/java/lang/s_ceil.o: java/lang/s_ceil.c
$(libobjdir)/java/lang/s_copysign.o: java/lang/s_copysign.c
$(libobjdir)/java/lang/s_cos.o: java/lang/s_cos.c
$(libobjdir)/java/lang/s_fabs.o: java/lang/s_fabs.c
$(libobjdir)/java/lang/s_floor.o: java/lang/s_floor.c
$(libobjdir)/java/lang/s_rint.o: java/lang/s_rint.c
$(libobjdir)/java/lang/s_scalbn.o: java/lang/s_scalbn.c
$(libobjdir)/java/lang/s_sin.o: java/lang/s_sin.c
$(libobjdir)/java/lang/s_tan.o: java/lang/s_tan.c
$(libobjdir)/java/lang/sf_fabs.o: java/lang/sf_fabs.c
$(libobjdir)/java/lang/sf_rint.o: java/lang/sf_rint.c
$(libobjdir)/java/lang/strtod.o: java/lang/strtod.c
$(libobjdir)/java/lang/w_acos.o: java/lang/w_acos.c
$(libobjdir)/java/lang/w_asin.o: java/lang/w_asin.c
$(libobjdir)/java/lang/w_atan2.o: java/lang/w_atan2.c
$(libobjdir)/java/lang/w_exp.o: java/lang/w_exp.c
$(libobjdir)/java/lang/w_fmod.o: java/lang/w_fmod.c
$(libobjdir)/java/lang/w_log.o: java/lang/w_log.c
$(libobjdir)/java/lang/w_pow.o: java/lang/w_pow.c
$(libobjdir)/java/lang/w_remainder.o: java/lang/w_remainder.c
$(libobjdir)/java/lang/w_sqrt.o: java/lang/w_sqrt.c
$(libobjdir)/libltdl/ltdl.o: libltdl/ltdl.c
$(libobjdir)/sysdep/ia64.o: sysdep/ia64.c
