OSLIB_DIR = $(top_srcdir)/build/oslib
OSLIB_SRC = $(OSLIB_DIR)/!OSLib/Source

DEFMOD = $(OSLIB_DIR)/\!OSLib/Bin/defmod
DEFMOD_FLAGS = -asmtype gccelf -fpu soft -s -32bit

STRIP_OSLIB_SCRIPT = $(top_srcdir)/strip-oslib.pl

BUILT_SOURCES = \
	wimp.s \
	font.s \
	colourtrans.s \
	os.s \
	reporter.s \
	toolbox.s \
	tb-window.s \
	osword.s \
	osspriteop.s \
	osheap.s

AM_CCASFLAGS = -xassembler-with-cpp -DASM

lib_LTLIBRARIES = libriscosdotnet.la
libriscosdotnet_la_LDFLAGS = -version-info 1:0:0

libriscosdotnet_la_SOURCES = \
	$(BUILT_SOURCES) \
	wimp-support.s \
	font-support.s \
	toolbox-support.s \
	copyright.s

$(OSLIB_DIR):
	svn co https://ro-oslib.svn.sourceforge.net/svnroot/ro-oslib/trunk $(OSLIB_DIR)

$(DEFMOD): $(OSLIB_DIR)
	CC=/usr/bin/cc make -C $(OSLIB_DIR)/\!OSLib/Tools/DefMod2 install

$(top_srcdir)/src/wimp.s: $(DEFMOD) $(OSLIB_SRC)/User/oslib/Wimp.swi
	$(DEFMOD) $(DEFMOD_FLAGS) < $(OSLIB_SRC)/User/oslib/Wimp.swi > temp.s
	$(STRIP_OSLIB_SCRIPT) temp.s $@ wimp
	rm -f temp.s

$(top_srcdir)/src/font.s: $(DEFMOD) $(OSLIB_SRC)/User/oslib/Font.swi
	$(DEFMOD) $(DEFMOD_FLAGS) < $(OSLIB_SRC)/User/oslib/Font.swi > temp.s
	$(STRIP_OSLIB_SCRIPT) temp.s $@ font
	rm -f temp.s

$(top_srcdir)/src/colourtrans.s: $(DEFMOD) $(OSLIB_SRC)/Core/oslib/ColourTrans.swi
	$(DEFMOD) $(DEFMOD_FLAGS) < $(OSLIB_SRC)/Core/oslib/ColourTrans.swi > temp.s
	$(STRIP_OSLIB_SCRIPT) temp.s $@ colourtrans
	rm -f temp.s

$(top_srcdir)/src/os.s: $(DEFMOD) $(OSLIB_SRC)/Core/oslib/OS.swi
	$(DEFMOD) $(DEFMOD_FLAGS) < $(OSLIB_SRC)/Core/oslib/OS.swi > temp.s
	$(STRIP_OSLIB_SCRIPT) temp.s $@ os
	rm -f temp.s

$(top_srcdir)/src/reporter.s: $(DEFMOD) $(OSLIB_SRC)/User/oslib/Report.swi
	$(DEFMOD) $(DEFMOD_FLAGS) < $(OSLIB_SRC)/User/oslib/Report.swi > temp.s
	$(STRIP_OSLIB_SCRIPT) temp.s $@ report
	rm -f temp.s

$(top_srcdir)/src/toolbox.s: $(DEFMOD) $(OSLIB_SRC)/Toolbox/oslib/Toolbox.swi
	$(DEFMOD) $(DEFMOD_FLAGS) < $(OSLIB_SRC)/Toolbox/oslib/Toolbox.swi > temp.s
	$(STRIP_OSLIB_SCRIPT) temp.s $@ toolbox
	rm -f temp.s

$(top_srcdir)/src/tb-window.s: $(DEFMOD) $(OSLIB_SRC)/Toolbox/oslib/Window.swi
	$(DEFMOD) $(DEFMOD_FLAGS) < $(OSLIB_SRC)/Toolbox/oslib/Window.swi > temp.s
	$(STRIP_OSLIB_SCRIPT) temp.s $@ window
	rm -f temp.s

$(top_srcdir)/src/osword.s: $(DEFMOD) $(OSLIB_SRC)/Core/oslib/OSWord.swi
	$(DEFMOD) $(DEFMOD_FLAGS) < $(OSLIB_SRC)/Core/oslib/OSWord.swi > temp.s
	$(STRIP_OSLIB_SCRIPT) temp.s $@ osword
	rm -f temp.s

$(top_srcdir)/src/osspriteop.s: $(DEFMOD) $(OSLIB_SRC)/Core/oslib/OSSpriteOp.swi
	$(DEFMOD) $(DEFMOD_FLAGS) < $(OSLIB_SRC)/Core/oslib/OSSpriteOp.swi > temp.s
	$(STRIP_OSLIB_SCRIPT) temp.s $@ osspriteop
	rm -f temp.s

$(top_srcdir)/src/osheap.s: $(DEFMOD) $(OSLIB_SRC)/Core/oslib/OSHeap.swi
	$(DEFMOD) $(DEFMOD_FLAGS) < $(OSLIB_SRC)/Core/oslib/OSHeap.swi > temp.s
	$(STRIP_OSLIB_SCRIPT) temp.s $@ osheap
	rm -f temp.s
