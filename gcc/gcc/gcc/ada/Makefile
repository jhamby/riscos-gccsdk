# Makefile for the GNU C++ compiler for ARM/RISC OS
# Copyright (c) 1997, 1998, 1999, 2000, 2002 Nick Burrett
# Written by Nick Burrett <nick@dsvr.net>

# Defs for GCC
LINK = $(CC) -o $@
INCLUDES = -I./ -I../ -I../../../include -I../config -I../ginclude
LIBS = -L$(objdir)/libiberty/$(STAGE) -liberty

MKDIR = mkdir
CP = cp

ADA_CFLAGS =
ADAFLAGS = -W -Wall -gnatpg -gnata
SOME_ADAFLAGS =-gnata
FORCE_DEBUG_ADAFLAGS = -g
GNATLIBFLAGS = -gnatpg
GNATLIBCFLAGS= -g -O2
ALL_ADAFLAGS = $(ADA_CFLAGS) $(X_ADAFLAGS) $(T_ADAFLAGS) $(ADAFLAGS)
MOST_ADAFLAGS = $(ADA_CFLAGS) $(X_ADAFLAGS) $(T_ADAFLAGS) $(SOME_ADAFLAGS)

adaobj = $(gccobjdir)/ada

$(adaobj)/%.o: %.c
	$(GNATCC) $(CFLAGS) $(ADA_CFLAGS) -o $@ -c $< $(INCLUDES)

$(adaobj)/%.o: %.adb
	$(GNATCC) $(CFLAGS) $(ALL_ADAFLAGS) -o $@ -c $< $(INCLUDES)

$(adaobj)/%.o: %.ads
	$(GNATCC) $(CFLAGS) $(ALL_ADAFLAGS) -o $@ -c $< $(INCLUDES)

all: tools

.PHONY: tools

tools: 	$(bin_dir)/gnatbind$(gexesfx) $(gcc_bin_dir)/gnat1$(gexesfx)

# Languages-specific object files for Ada.
# Object files for gnat1 from C sources.
GNAT1_C_OBJS = $(adaobj)/b_gnat1.o $(adaobj)/adadecode.o $(adaobj)/adaint.o $(adaobj)/cstreams.o \
 $(adaobj)/cio.o $(adaobj)/targtyps.o $(adaobj)/decl.o $(adaobj)/misc.o $(adaobj)/utils.o $(adaobj)/utils2.o \
 $(adaobj)/trans.o $(adaobj)/cuintp.o $(adaobj)/argv.o $(adaobj)/raise.o $(adaobj)/init.o $(adaobj)/tracebak.o

# Object files from Ada sources that are used by gnat1

GNAT_ADA_OBJS = \
 $(adaobj)/ada.o $(adaobj)/a-charac.o $(adaobj)/a-chlat1.o $(adaobj)/a-except.o $(adaobj)/s-memory.o \
 $(adaobj)/s-traceb.o $(adaobj)/s-mastop.o $(adaobj)/s-except.o $(adaobj)/ali.o $(adaobj)/alloc.o \
 $(adaobj)/atree.o $(adaobj)/butil.o $(adaobj)/casing.o $(adaobj)/checks.o $(adaobj)/comperr.o $(adaobj)/csets.o \
 $(adaobj)/cstand.o $(adaobj)/debug.o $(adaobj)/debug_a.o $(adaobj)/einfo.o $(adaobj)/elists.o $(adaobj)/errout.o \
 $(adaobj)/eval_fat.o $(adaobj)/exp_attr.o $(adaobj)/exp_ch11.o $(adaobj)/exp_ch12.o $(adaobj)/exp_ch13.o \
 $(adaobj)/exp_ch2.o $(adaobj)/exp_ch3.o $(adaobj)/exp_ch4.o $(adaobj)/exp_ch5.o $(adaobj)/exp_ch6.o \
 $(adaobj)/exp_ch7.o $(adaobj)/exp_ch8.o $(adaobj)/exp_ch9.o $(adaobj)/exp_code.o $(adaobj)/exp_dbug.o \
 $(adaobj)/exp_disp.o $(adaobj)/exp_dist.o $(adaobj)/exp_fixd.o $(adaobj)/exp_aggr.o $(adaobj)/exp_imgv.o \
 $(adaobj)/exp_intr.o $(adaobj)/exp_pakd.o $(adaobj)/exp_prag.o $(adaobj)/exp_smem.o $(adaobj)/exp_strm.o \
 $(adaobj)/exp_tss.o $(adaobj)/exp_util.o $(adaobj)/exp_vfpt.o $(adaobj)/expander.o $(adaobj)/fname.o \
 $(adaobj)/fname-uf.o $(adaobj)/fmap.o $(adaobj)/freeze.o $(adaobj)/frontend.o $(adaobj)/gnat.o \
 $(adaobj)/g-hesora.o $(adaobj)/g-htable.o $(adaobj)/g-os_lib.o $(adaobj)/g-speche.o $(adaobj)/s-crc32.o \
 $(adaobj)/get_targ.o $(adaobj)/gnatvsn.o $(adaobj)/hlo.o $(adaobj)/hostparm.o $(adaobj)/impunit.o \
 $(adaobj)/interfac.o $(adaobj)/itypes.o $(adaobj)/inline.o $(adaobj)/krunch.o $(adaobj)/lib.o $(adaobj)/layout.o \
 $(adaobj)/lib-load.o $(adaobj)/lib-util.o $(adaobj)/lib-xref.o $(adaobj)/lib-writ.o $(adaobj)/live.o \
 $(adaobj)/namet.o $(adaobj)/nlists.o $(adaobj)/nmake.o $(adaobj)/opt.o $(adaobj)/osint.o $(adaobj)/osint-c.o \
 $(adaobj)/output.o $(adaobj)/par.o $(adaobj)/repinfo.o $(adaobj)/restrict.o $(adaobj)/rident.o \
 $(adaobj)/rtsfind.o $(adaobj)/s-assert.o $(adaobj)/s-parame.o $(adaobj)/s-stache.o $(adaobj)/s-stalib.o \
 $(adaobj)/s-imgenu.o $(adaobj)/s-stoele.o $(adaobj)/s-soflin.o $(adaobj)/s-exctab.o $(adaobj)/s-secsta.o \
 $(adaobj)/s-wchcnv.o $(adaobj)/s-wchcon.o $(adaobj)/s-wchjis.o $(adaobj)/s-unstyp.o $(adaobj)/scans.o \
 $(adaobj)/scn.o $(adaobj)/sdefault.o $(adaobj)/sem.o $(adaobj)/sem_aggr.o $(adaobj)/sem_attr.o \
 $(adaobj)/sem_cat.o $(adaobj)/sem_ch10.o $(adaobj)/sem_ch11.o $(adaobj)/sem_ch12.o $(adaobj)/sem_ch13.o \
 $(adaobj)/sem_ch2.o $(adaobj)/sem_ch3.o $(adaobj)/sem_ch4.o $(adaobj)/sem_ch5.o $(adaobj)/sem_ch6.o \
 $(adaobj)/sem_ch7.o $(adaobj)/sem_ch8.o $(adaobj)/sem_ch9.o $(adaobj)/sem_case.o $(adaobj)/sem_disp.o \
 $(adaobj)/sem_dist.o $(adaobj)/sem_elab.o $(adaobj)/sem_elim.o $(adaobj)/sem_eval.o $(adaobj)/sem_intr.o \
 $(adaobj)/sem_maps.o $(adaobj)/sem_mech.o $(adaobj)/sem_prag.o $(adaobj)/sem_res.o $(adaobj)/sem_smem.o \
 $(adaobj)/sem_type.o $(adaobj)/sem_util.o $(adaobj)/sem_vfpt.o $(adaobj)/sem_warn.o $(adaobj)/sinfo-cn.o \
 $(adaobj)/sinfo.o $(adaobj)/sinput.o $(adaobj)/sinput-d.o $(adaobj)/sinput-l.o $(adaobj)/snames.o \
 $(adaobj)/sprint.o $(adaobj)/stand.o $(adaobj)/stringt.o $(adaobj)/style.o $(adaobj)/switch.o \
 $(adaobj)/switch-c.o $(adaobj)/stylesw.o $(adaobj)/validsw.o $(adaobj)/system.o $(adaobj)/table.o \
 $(adaobj)/targparm.o $(adaobj)/tbuild.o $(adaobj)/tree_gen.o $(adaobj)/tree_io.o $(adaobj)/treepr.o \
 $(adaobj)/treeprs.o $(adaobj)/ttypef.o $(adaobj)/ttypes.o $(adaobj)/types.o $(adaobj)/uintp.o $(adaobj)/uname.o \
 $(adaobj)/urealp.o $(adaobj)/usage.o $(adaobj)/widechar.o



# Object files for gnat executables
GNAT1_ADA_OBJS = $(GNAT_ADA_OBJS) $(adaobj)/back_end.o $(adaobj)/gnat1drv.o
GNAT1_OBJS = $(GNAT1_C_OBJS) $(GNAT1_ADA_OBJS) $(EXTRA_GNAT1_OBJS)


GNATBIND_OBJS = \
 $(adaobj)/link.o $(adaobj)/ada.o $(adaobj)/adaint.o $(adaobj)/cstreams.o $(adaobj)/cio.o $(adaobj)/ali.o \
 $(adaobj)/ali-util.o $(adaobj)/alloc.o $(adaobj)/a-tags.o $(adaobj)/a-stream.o $(adaobj)/bcheck.o \
 $(adaobj)/binde.o $(adaobj)/binderr.o $(adaobj)/bindgen.o $(adaobj)/bindusg.o $(adaobj)/butil.o \
 $(adaobj)/casing.o $(adaobj)/csets.o $(adaobj)/debug.o $(adaobj)/fname.o $(adaobj)/gnat.o $(adaobj)/g-hesora.o \
 $(adaobj)/g-htable.o $(adaobj)/g-os_lib.o $(adaobj)/s-crc32.o $(adaobj)/fmap.o $(adaobj)/gnatbind.o \
 $(adaobj)/gnatvsn.o $(adaobj)/hostparm.o $(adaobj)/krunch.o $(adaobj)/namet.o $(adaobj)/opt.o $(adaobj)/osint.o \
 $(adaobj)/osint-b.o $(adaobj)/output.o $(adaobj)/rident.o $(adaobj)/s-assert.o $(adaobj)/s-parame.o \
 $(adaobj)/s-sopco3.o $(adaobj)/s-sopco4.o $(adaobj)/s-sopco5.o $(adaobj)/s-stache.o $(adaobj)/s-stalib.o \
 $(adaobj)/s-stoele.o $(adaobj)/s-imgenu.o $(adaobj)/s-strops.o $(adaobj)/s-soflin.o $(adaobj)/s-wchcon.o \
 $(adaobj)/s-wchjis.o $(adaobj)/sdefault.o $(adaobj)/switch.o $(adaobj)/switch-b.o $(adaobj)/stylesw.o \
 $(adaobj)/validsw.o $(adaobj)/system.o $(adaobj)/table.o $(adaobj)/tree_io.o $(adaobj)/types.o \
 $(adaobj)/widechar.o $(adaobj)/raise.o $(adaobj)/exit.o $(adaobj)/argv.o $(adaobj)/init.o $(adaobj)/final.o \
 $(adaobj)/s-wchcnv.o $(adaobj)/s-exctab.o $(adaobj)/a-except.o $(adaobj)/s-memory.o $(adaobj)/s-traceb.o \
 $(adaobj)/tracebak.o $(adaobj)/s-mastop.o $(adaobj)/s-except.o $(adaobj)/s-secsta.o $(adaobj)/atree.o \
 $(adaobj)/scans.o $(adaobj)/einfo.o $(adaobj)/sinfo.o $(adaobj)/scn.o $(adaobj)/sinput.o $(adaobj)/sinput-l.o \
 $(adaobj)/targparm.o $(adaobj)/errout.o $(adaobj)/style.o $(adaobj)/stand.o $(adaobj)/lib.o $(adaobj)/uintp.o \
 $(adaobj)/elists.o $(adaobj)/nlists.o $(adaobj)/stringt.o $(adaobj)/snames.o $(adaobj)/uname.o \
 $(adaobj)/urealp.o \
 $(EXTRA_GNATBIND_OBJS)

# List of extra object files linked in with various programs.
EXTRA_GNAT1_OBJS = 
EXTRA_GNATBIND_OBJS = $(gccobjdir)/version.o

# The runtime library for gnat comprises two directories.  One contains the
# Ada source files that the compiler (gnat1) needs -- these files are listed
# by ADA_INCLUDE_SRCS -- and the other contains the object files and their
# corresponding .ali files for the parts written in Ada, libgnat.a for
# the parts of the runtime written in C, and libgthreads.a for the pthreads
# emulation library.  LIBGNAT_OBJS lists the objects that go into libgnat.a,
# while GNATRTL_OBJS lists the object files compiled from Ada sources that
# go into the directory.  The pthreads emulation is built in the threads
# subdirectory and copied.
LIBGNAT_SRCS = ada.h adaint.c adaint.h argv.c cio.c cstreams.c \
  errno.c exit.c cal.c \
  raise.h raise.c sysdep.c types.h io-aux.c init.c \
  final.c tracebak.c expect.c $(EXTRA_LIBGNAT_SRCS)

LIBGNAT_OBJS = $(adaobj)/adaint.o $(adaobj)/argv.o $(adaobj)/cio.o $(adaobj)/cstreams.o $(adaobj)/errno.o $(adaobj)/exit.o \
  $(adaobj)/raise.o $(adaobj)/sysdep.o $(adaobj)/io-aux.o $(adaobj)/init.o $(adaobj)/cal.o $(adaobj)/final.o \
  $(adaobj)/tracebak.o $(adaobj)/expect.o ../../$(adaobj)/prefix.o $(EXTRA_LIBGNAT_OBJS)

# NOTE ??? - when the -I option for compiling Ada code is made to work,
#  the library installation will change and there will be a
#  GNAT_RTL_SRCS.  Right now we count on being able to build GNATRTL_OBJS
#  from ADA_INCLUDE_SRCS.

# Objects needed only for tasking
GNATRTL_TASKING_OBJS= \
  $(adaobj)/a-dynpri.o \
  $(adaobj)/a-interr.o \
  $(adaobj)/a-intsig.o \
  $(adaobj)/a-intnam.o \
  $(adaobj)/a-reatim.o \
  $(adaobj)/a-retide.o \
  $(adaobj)/a-sytaco.o \
  $(adaobj)/a-taside.o \
  $(adaobj)/g-thread.o \
  $(adaobj)/s-asthan.o \
  $(adaobj)/s-inmaop.o \
  $(adaobj)/s-interr.o \
  $(adaobj)/s-intman.o \
  $(adaobj)/s-osinte.o \
  $(adaobj)/s-proinf.o \
  $(adaobj)/s-taenca.o \
  $(adaobj)/s-taprob.o \
  $(adaobj)/s-taprop.o \
  $(adaobj)/s-tarest.o \
  $(adaobj)/s-tasdeb.o \
  $(adaobj)/s-tasinf.o \
  $(adaobj)/s-tasini.o \
  $(adaobj)/s-taskin.o \
  $(adaobj)/s-taspri.o \
  $(adaobj)/s-tasque.o \
  $(adaobj)/s-tasres.o \
  $(adaobj)/s-tasren.o \
  $(adaobj)/s-tassta.o \
  $(adaobj)/s-tasuti.o \
  $(adaobj)/s-taasde.o \
  $(adaobj)/s-tadeca.o \
  $(adaobj)/s-tadert.o \
  $(adaobj)/s-tataat.o \
  $(adaobj)/s-tpinop.o \
  $(adaobj)/s-tpoben.o \
  $(adaobj)/s-tpobop.o \
  $(adaobj)/s-tposen.o $(EXTRA_GNATRTL_TASKING_OBJS)

# Objects needed for non-tasking.
GNATRTL_NONTASKING_OBJS= \
  $(adaobj)/a-caldel.o \
  $(adaobj)/a-calend.o \
  $(adaobj)/a-chahan.o \
  $(adaobj)/a-charac.o \
  $(adaobj)/a-chlat1.o \
  $(adaobj)/a-colien.o \
  $(adaobj)/a-colire.o \
  $(adaobj)/a-comlin.o \
  $(adaobj)/a-cwila1.o \
  $(adaobj)/a-decima.o \
  $(adaobj)/a-einuoc.o \
  $(adaobj)/a-except.o \
  $(adaobj)/a-exctra.o \
  $(adaobj)/a-filico.o \
  $(adaobj)/a-finali.o \
  $(adaobj)/a-flteio.o \
  $(adaobj)/a-fwteio.o \
  $(adaobj)/a-inteio.o \
  $(adaobj)/a-ioexce.o \
  $(adaobj)/a-iwteio.o \
  $(adaobj)/a-lfteio.o \
  $(adaobj)/a-lfwtio.o \
  $(adaobj)/a-liteio.o \
  $(adaobj)/a-liwtio.o \
  $(adaobj)/a-llftio.o \
  $(adaobj)/a-llfwti.o \
  $(adaobj)/a-llitio.o \
  $(adaobj)/a-lliwti.o \
  $(adaobj)/a-ncelfu.o \
  $(adaobj)/a-nlcefu.o \
  $(adaobj)/a-nlcoty.o \
  $(adaobj)/a-nlelfu.o \
  $(adaobj)/a-nllcef.o \
  $(adaobj)/a-nllcty.o \
  $(adaobj)/a-nllefu.o \
  $(adaobj)/a-nscefu.o \
  $(adaobj)/a-nscoty.o \
  $(adaobj)/a-nselfu.o \
  $(adaobj)/a-nucoty.o \
  $(adaobj)/a-nuelfu.o \
  $(adaobj)/a-nuflra.o \
  $(adaobj)/a-numaux.o \
  $(adaobj)/a-numeri.o \
  $(adaobj)/a-sfteio.o \
  $(adaobj)/a-sfwtio.o \
  $(adaobj)/a-siteio.o \
  $(adaobj)/a-siwtio.o \
  $(adaobj)/a-ssicst.o \
  $(adaobj)/a-ssitio.o \
  $(adaobj)/a-ssiwti.o \
  $(adaobj)/a-stmaco.o \
  $(adaobj)/a-strbou.o \
  $(adaobj)/a-stream.o \
  $(adaobj)/a-strfix.o \
  $(adaobj)/a-string.o \
  $(adaobj)/a-strmap.o \
  $(adaobj)/a-strsea.o \
  $(adaobj)/a-strunb.o \
  $(adaobj)/a-ststio.o \
  $(adaobj)/a-stunau.o \
  $(adaobj)/a-stwibo.o \
  $(adaobj)/a-stwifi.o \
  $(adaobj)/a-stwima.o \
  $(adaobj)/a-stwise.o \
  $(adaobj)/a-stwiun.o \
  $(adaobj)/a-suteio.o \
  $(adaobj)/a-swuwti.o \
  $(adaobj)/a-swmwco.o \
  $(adaobj)/a-tags.o \
  $(adaobj)/a-teioed.o \
  $(adaobj)/a-textio.o \
  $(adaobj)/a-ticoau.o \
  $(adaobj)/a-tideau.o \
  $(adaobj)/a-tienau.o \
  $(adaobj)/a-tiflau.o \
  $(adaobj)/a-tigeau.o \
  $(adaobj)/a-tiinau.o \
  $(adaobj)/a-timoau.o \
  $(adaobj)/a-tiocst.o \
  $(adaobj)/a-titest.o \
  $(adaobj)/a-witeio.o \
  $(adaobj)/a-wtcoau.o \
  $(adaobj)/a-wtcstr.o \
  $(adaobj)/a-wtdeau.o \
  $(adaobj)/a-wtedit.o \
  $(adaobj)/a-wtenau.o \
  $(adaobj)/a-wtflau.o \
  $(adaobj)/a-wtgeau.o \
  $(adaobj)/a-wtinau.o \
  $(adaobj)/a-wtmoau.o \
  $(adaobj)/a-wttest.o \
  $(adaobj)/ada.o \
  $(adaobj)/calendar.o \
  $(adaobj)/g-awk.o \
  $(adaobj)/g-busora.o \
  $(adaobj)/g-calend.o \
  $(adaobj)/g-casuti.o \
  $(adaobj)/g-catiio.o \
  $(adaobj)/g-cgi.o    \
  $(adaobj)/g-cgicoo.o \
  $(adaobj)/g-cgideb.o \
  $(adaobj)/g-comlin.o \
  $(adaobj)/g-crc32.o \
  $(adaobj)/g-curexc.o \
  $(adaobj)/g-debuti.o \
  $(adaobj)/g-debpoo.o \
  $(adaobj)/g-diopit.o \
  $(adaobj)/g-dirope.o \
  $(adaobj)/g-except.o \
  $(adaobj)/g-exctra.o \
  $(adaobj)/g-expect.o \
  $(adaobj)/g-flocon.o \
  $(adaobj)/g-hesora.o \
  $(adaobj)/g-htable.o \
  $(adaobj)/g-io.o \
  $(adaobj)/g-io_aux.o \
  $(adaobj)/g-locfil.o \
  $(adaobj)/g-moreex.o \
  $(adaobj)/g-os_lib.o \
  $(adaobj)/g-regexp.o \
  $(adaobj)/g-regpat.o \
  $(adaobj)/g-soccon.o \
  $(adaobj)/g-socket.o \
  $(adaobj)/g-socthi.o \
  $(adaobj)/g-soliop.o \
  $(adaobj)/g-souinf.o \
  $(adaobj)/g-speche.o \
  $(adaobj)/g-spipat.o \
  $(adaobj)/g-spitbo.o \
  $(adaobj)/g-sptabo.o \
  $(adaobj)/g-sptain.o \
  $(adaobj)/g-sptavs.o \
  $(adaobj)/g-tasloc.o \
  $(adaobj)/g-traceb.o \
  $(adaobj)/g-trasym.o \
  $(adaobj)/gnat.o \
  $(adaobj)/i-c.o \
  $(adaobj)/i-cexten.o \
  $(adaobj)/i-cobol.o \
  $(adaobj)/i-cpp.o \
  $(adaobj)/i-cstrea.o \
  $(adaobj)/i-cstrin.o \
  $(adaobj)/i-fortra.o \
  $(adaobj)/i-pacdec.o \
  $(adaobj)/interfac.o \
  $(adaobj)/ioexcept.o \
  $(adaobj)/machcode.o \
  $(adaobj)/s-addima.o \
  $(adaobj)/s-arit64.o \
  $(adaobj)/s-assert.o \
  $(adaobj)/s-auxdec.o \
  $(adaobj)/s-bitops.o \
  $(adaobj)/s-chepoo.o \
  $(adaobj)/s-crc32.o  \
  $(adaobj)/s-direio.o \
  $(adaobj)/s-errrep.o \
  $(adaobj)/s-except.o \
  $(adaobj)/s-exctab.o \
  $(adaobj)/s-exnflt.o \
  $(adaobj)/s-exngen.o \
  $(adaobj)/s-exnint.o \
  $(adaobj)/s-exnlfl.o \
  $(adaobj)/s-exnlin.o \
  $(adaobj)/s-exnllf.o \
  $(adaobj)/s-exnlli.o \
  $(adaobj)/s-exnsfl.o \
  $(adaobj)/s-exnsin.o \
  $(adaobj)/s-exnssi.o \
  $(adaobj)/s-expflt.o \
  $(adaobj)/s-expgen.o \
  $(adaobj)/s-expint.o \
  $(adaobj)/s-explfl.o \
  $(adaobj)/s-explin.o \
  $(adaobj)/s-expllf.o \
  $(adaobj)/s-explli.o \
  $(adaobj)/s-expllu.o \
  $(adaobj)/s-expmod.o \
  $(adaobj)/s-expsfl.o \
  $(adaobj)/s-expsin.o \
  $(adaobj)/s-expssi.o \
  $(adaobj)/s-expuns.o \
  $(adaobj)/s-fatflt.o \
  $(adaobj)/s-fatlfl.o \
  $(adaobj)/s-fatllf.o \
  $(adaobj)/s-fatsfl.o \
  $(adaobj)/s-ficobl.o \
  $(adaobj)/s-fileio.o \
  $(adaobj)/s-finimp.o \
  $(adaobj)/s-finroo.o \
  $(adaobj)/s-fore.o \
  $(adaobj)/s-imgbiu.o \
  $(adaobj)/s-imgboo.o \
  $(adaobj)/s-imgcha.o \
  $(adaobj)/s-imgdec.o \
  $(adaobj)/s-imgenu.o \
  $(adaobj)/s-imgint.o \
  $(adaobj)/s-imgllb.o \
  $(adaobj)/s-imglld.o \
  $(adaobj)/s-imglli.o \
  $(adaobj)/s-imgllu.o \
  $(adaobj)/s-imgllw.o \
  $(adaobj)/s-imgrea.o \
  $(adaobj)/s-imguns.o \
  $(adaobj)/s-imgwch.o \
  $(adaobj)/s-imgwiu.o \
  $(adaobj)/s-io.o \
  $(adaobj)/s-gloloc.o \
  $(adaobj)/s-maccod.o \
  $(adaobj)/s-mantis.o \
  $(adaobj)/s-mastop.o \
  $(adaobj)/s-osprim.o \
  $(adaobj)/s-pack03.o \
  $(adaobj)/s-pack05.o \
  $(adaobj)/s-pack06.o \
  $(adaobj)/s-pack07.o \
  $(adaobj)/s-pack09.o \
  $(adaobj)/s-pack10.o \
  $(adaobj)/s-pack11.o \
  $(adaobj)/s-pack12.o \
  $(adaobj)/s-pack13.o \
  $(adaobj)/s-pack14.o \
  $(adaobj)/s-pack15.o \
  $(adaobj)/s-pack17.o \
  $(adaobj)/s-pack18.o \
  $(adaobj)/s-pack19.o \
  $(adaobj)/s-pack20.o \
  $(adaobj)/s-pack21.o \
  $(adaobj)/s-pack22.o \
  $(adaobj)/s-pack23.o \
  $(adaobj)/s-pack24.o \
  $(adaobj)/s-pack25.o \
  $(adaobj)/s-pack26.o \
  $(adaobj)/s-pack27.o \
  $(adaobj)/s-pack28.o \
  $(adaobj)/s-pack29.o \
  $(adaobj)/s-pack30.o \
  $(adaobj)/s-pack31.o \
  $(adaobj)/s-pack33.o \
  $(adaobj)/s-pack34.o \
  $(adaobj)/s-pack35.o \
  $(adaobj)/s-pack36.o \
  $(adaobj)/s-pack37.o \
  $(adaobj)/s-pack38.o \
  $(adaobj)/s-pack39.o \
  $(adaobj)/s-pack40.o \
  $(adaobj)/s-pack41.o \
  $(adaobj)/s-pack42.o \
  $(adaobj)/s-pack43.o \
  $(adaobj)/s-pack44.o \
  $(adaobj)/s-pack45.o \
  $(adaobj)/s-pack46.o \
  $(adaobj)/s-pack47.o \
  $(adaobj)/s-pack48.o \
  $(adaobj)/s-pack49.o \
  $(adaobj)/s-pack50.o \
  $(adaobj)/s-pack51.o \
  $(adaobj)/s-pack52.o \
  $(adaobj)/s-pack53.o \
  $(adaobj)/s-pack54.o \
  $(adaobj)/s-pack55.o \
  $(adaobj)/s-pack56.o \
  $(adaobj)/s-pack57.o \
  $(adaobj)/s-pack58.o \
  $(adaobj)/s-pack59.o \
  $(adaobj)/s-pack60.o \
  $(adaobj)/s-pack61.o \
  $(adaobj)/s-pack62.o \
  $(adaobj)/s-pack63.o \
  $(adaobj)/s-parame.o \
  $(adaobj)/s-parint.o \
  $(adaobj)/s-pooglo.o \
  $(adaobj)/s-pooloc.o \
  $(adaobj)/s-poosiz.o \
  $(adaobj)/s-powtab.o \
  $(adaobj)/s-rpc.o \
  $(adaobj)/s-scaval.o \
  $(adaobj)/s-secsta.o \
  $(adaobj)/s-sequio.o \
  $(adaobj)/s-shasto.o \
  $(adaobj)/s-sopco3.o \
  $(adaobj)/s-sopco4.o \
  $(adaobj)/s-sopco5.o \
  $(adaobj)/s-stache.o \
  $(adaobj)/s-stalib.o \
  $(adaobj)/s-stoele.o \
  $(adaobj)/s-stopoo.o \
  $(adaobj)/s-stratt.o \
  $(adaobj)/s-strops.o \
  $(adaobj)/s-soflin.o \
  $(adaobj)/s-memory.o \
  $(adaobj)/s-traceb.o \
  $(adaobj)/s-unstyp.o \
  $(adaobj)/s-vaflop.o \
  $(adaobj)/s-valboo.o \
  $(adaobj)/s-valcha.o \
  $(adaobj)/s-valdec.o \
  $(adaobj)/s-valenu.o \
  $(adaobj)/s-valint.o \
  $(adaobj)/s-vallld.o \
  $(adaobj)/s-vallli.o \
  $(adaobj)/s-valllu.o \
  $(adaobj)/s-valrea.o \
  $(adaobj)/s-valuns.o \
  $(adaobj)/s-valuti.o \
  $(adaobj)/s-valwch.o \
  $(adaobj)/s-vercon.o \
  $(adaobj)/s-vmexta.o \
  $(adaobj)/s-wchcnv.o \
  $(adaobj)/s-wchcon.o \
  $(adaobj)/s-wchjis.o \
  $(adaobj)/s-wchstw.o \
  $(adaobj)/s-wchwts.o \
  $(adaobj)/s-widboo.o \
  $(adaobj)/s-widcha.o \
  $(adaobj)/s-widenu.o \
  $(adaobj)/s-widlli.o \
  $(adaobj)/s-widllu.o \
  $(adaobj)/s-widwch.o \
  $(adaobj)/s-wwdcha.o \
  $(adaobj)/s-wwdenu.o \
  $(adaobj)/s-wwdwch.o \
  $(adaobj)/system.o \
  $(adaobj)/text_io.o $(EXTRA_GNATRTL_NONTASKING_OBJS)

GNATRTL_OBJS = $(GNATRTL_NONTASKING_OBJS) $(GNATRTL_TASKING_OBJS)

ADA_BACKEND = $(BACKEND) $(gccobjdir)/attribs.o

# Needs to be built with CC=gcc
# Since the RTL should be built with the latest compiler, remove the
#  stamp target in the parent directory whenever gnat1 is rebuilt
$(gcc_bin_dir)/gnat1$(gexesfx): $(P) $(GNAT1_OBJS) $(ADA_BACKEND) $(LIBDEPS) $(TARGET_ADA_SRCS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(GNAT1_OBJS) $(ADA_BACKEND) $(LIBDEPS) $(LIBS)

$(bin_dir)/gnatbind$(gexesfx): $(P) $(adaobj)/b_gnatb.o $(GNATBIND_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(adaobj)/b_gnatb.o $(GNATBIND_OBJS) \
	      $(LIBIBERTY) $(LIBDEPS) $(LIBS)



# Ada language specific files.

ada_extra_files : treeprs.ads einfo.h sinfo.h nmake.adb nmake.ads

b_gnat1.c : $(GNAT1_ADA_OBJS)
	$(GNATBIND) $(ADA_INCLUDES) -o b_gnat1.c -n $(adaobj)/gnat1drv.ali
$(adaobj)/b_gnat1.o : b_gnat1.c

b_gnatb.c : $(GNATBIND_OBJS)
	$(GNATBIND) $(ADA_INCLUDES) -o b_gnatb.c $(adaobj)/gnatbind.ali
$(adaobj)/b_gnatb.o : b_gnatb.c


treeprs.ads : treeprs.adt sinfo.ads xtreeprs.adb
	-$(MKDIR) bldtools
	$(CP) $^ bldtools
	(cd bldtools; gnatmake -q xtreeprs ; ./xtreeprs ../treeprs.ads )

einfo.h : einfo.ads einfo.adb xeinfo.adb
	-$(MKDIR) bldtools
	$(CP) $^ bldtools
	(cd bldtools; gnatmake -q xeinfo ; ./xeinfo ../einfo.h )

sinfo.h : sinfo.ads xsinfo.adb
	-$(MKDIR) bldtools
	$(CP) $^ bldtools
	(cd bldtools; gnatmake -q xsinfo ; ./xsinfo ../sinfo.h )

nmake.adb : sinfo.ads nmake.adt xnmake.adb
	-$(MKDIR) bldtools
	$(CP) $^ bldtools
	(cd bldtools; gnatmake -q xnmake ; ./xnmake -b ../nmake.adb )

nmake.ads :  sinfo.ads nmake.adt xnmake.adb
	-$(MKDIR) bldtools
	$(CP) $^ bldtools
	(cd bldtools; gnatmake -q xnmake ; ./xnmake -s ../nmake.ads )

ADA_INCLUDE_DIR = $(libsubdir)/adainclude
ADA_RTL_OBJ_DIR = $(libsubdir)/adalib
ECHO=echo

# Note: the strings below do not make sense for Ada strings in the OS/2
#  case.  This is ignored for now since the OS/2 version doesn't use
#  these -- there are no default locations.
sdefault.adb: ../version.c
	$(ECHO) "package body Sdefault is" >sdefault.adb
	$(ECHO) "   S1 : aliased constant String := \"$(ADA_INCLUDE_DIR)/\";" >>sdefault.adb
	$(ECHO) "   S2 : aliased constant String := \"$(ADA_RTL_OBJ_DIR)/\";" >>sdefault.adb
	$(ECHO) "   S3 : aliased constant String := \"$(target)/\";" >>sdefault.adb
	$(ECHO) "   S4 : aliased constant String := \"$(libsubdir)/\";" >>sdefault.adb
	$(ECHO) "   function Include_Dir_Default_Name return String_Ptr is" >>sdefault.adb
	$(ECHO) "   begin" >>sdefault.adb
	$(ECHO) "      return new String'(S1);" >>sdefault.adb
	$(ECHO) "   end Include_Dir_Default_Name;" >>sdefault.adb
	$(ECHO) "   function Object_Dir_Default_Name return String_Ptr is" >>sdefault.adb
	$(ECHO) "   begin" >>sdefault.adb
	$(ECHO) "      return new String'(S2);" >>sdefault.adb
	$(ECHO) "   end Object_Dir_Default_Name;" >>sdefault.adb
	$(ECHO) "   function Target_Name return String_Ptr is" >>sdefault.adb
	$(ECHO) "   begin" >>sdefault.adb
	$(ECHO) "      return new String'(S3);" >>sdefault.adb
	$(ECHO) "   end Target_Name;" >>sdefault.adb
	$(ECHO) "   function Search_Dir_Prefix return String_Ptr is" >>sdefault.adb
	$(ECHO) "   begin" >>sdefault.adb
	$(ECHO) "      return new String'(S4);" >>sdefault.adb
	$(ECHO) "   end Search_Dir_Prefix;" >>sdefault.adb
	$(ECHO) "end Sdefault;" >> sdefault.adb


# special compiles for sdefault without -gnatg, to avoid long line error
sdefault.o: sdefault.ads sdefault.adb types.ads unchdeal.ads \
   system.ads s-exctab.ads s-stalib.ads unchconv.ads
	$(CC) -c -O2 $(MOST_ADAFLAGS) $(ADA_INCLUDES) sdefault.adb


$(adaobj)/adaint.o: adaint.c



