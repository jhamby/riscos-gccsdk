OSLIB_DIR = $(top_srcdir)/build/oslib
OSLIB_SRC = $(OSLIB_DIR)/!OSLib/Source

DEFMOD = $(OSLIB_DIR)/\!OSLib/Bin/defmod
DEFMOD_FLAGS = -asmtype gccelf -float-abi soft -s -32bit

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
	osheap.s \
	osfind.s \
	osargs.s \
	osgbpb.s \
	drawfile.s

AM_CCASFLAGS = -xassembler-with-cpp -DASM

lib_LTLIBRARIES = libriscosdotnet.la
libriscosdotnet_la_LDFLAGS = -version-info 1:0:0

libriscosdotnet_la_SOURCES = \
	$(BUILT_SOURCES) \
	wimp-support.c \
	font-support.s \
	toolbox-support.c \
	toolbox-support-asm.s \
	os-support.c \
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

$(top_srcdir)/src/osfind.s: $(DEFMOD) $(OSLIB_SRC)/Computer/oslib/OSFind.swi
	$(DEFMOD) $(DEFMOD_FLAGS) < $(OSLIB_SRC)/Computer/oslib/OSFind.swi > temp.s
	$(STRIP_OSLIB_SCRIPT) temp.s $@ osfind
	rm -f temp.s

$(top_srcdir)/src/osargs.s: $(DEFMOD) $(OSLIB_SRC)/Computer/oslib/OSArgs.swi
	$(DEFMOD) $(DEFMOD_FLAGS) < $(OSLIB_SRC)/Computer/oslib/OSArgs.swi > temp.s
	$(STRIP_OSLIB_SCRIPT) temp.s $@ osargs
	rm -f temp.s

$(top_srcdir)/src/osgbpb.s: $(DEFMOD) $(OSLIB_SRC)/Computer/oslib/OSGBPB.swi
	$(DEFMOD) $(DEFMOD_FLAGS) < $(OSLIB_SRC)/Computer/oslib/OSGBPB.swi > temp.s
	$(STRIP_OSLIB_SCRIPT) temp.s $@ osgbpb
	rm -f temp.s

$(top_srcdir)/src/drawfile.s: $(DEFMOD) $(OSLIB_SRC)/User/oslib/DrawFile.swi
	$(DEFMOD) $(DEFMOD_FLAGS) < $(OSLIB_SRC)/User/oslib/DrawFile.swi > temp.s
	$(STRIP_OSLIB_SCRIPT) temp.s $@ drawfile
	rm -f temp.s
