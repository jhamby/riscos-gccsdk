# Makefile for GNU F77 compiler runtime for ARM/RISC OS
# Copyright (c) 1997-2000 Nick Burrett
# Written by Nick Burrett <nick@dsvr.net>
#
# This file is part of the GNU Compiler Collection
#

OPTS = -mthrowback -mpoke-function-name
#OPTS = -mthrowback -mno-apcs-frame -mno-apcs-stack-check -muse-arg-pointer

# Defs for GCC
CC = $(standard_exec_prefix)/gcc $(CFLAGS)
CFLAGS = -O2 $(OPTS) -DIN_GCC -DHAVE_CONFIG_H $(TARGET_DEFS)
INCLUDES = -I. -I./libi77 -I./libf77
Predefs = -DHAVE_CONFIG_H -DSkip_f2c_Undefs -DSTDC_HEADERS=1 -D_POSIX_SOURCE=1 -DHAVE_TEMPNAM=1 -DNON_ANSI_RW_MODES=1 -DNO_EOF_CHAR_CHECK=1 -DRETSIGTYPE=void -DIEEE_drem=1 -DNON_UNIX_STDIO=1 -DAllow_TYQUAD

f2cobjdir=$(objdir)/gcc-$(VERSION)/libf2c/$(TRG)
f77objdir=$(objdir)/gcc-$(VERSION)/libf2c/libf77/$(TRG)
i77objdir=$(objdir)/gcc-$(VERSION)/libf2c/libi77/$(TRG)
u77objdir=$(objdir)/gcc-$(VERSION)/libf2c/libu77/$(TRG)

$(f77objdir)/%.o: libf77/%.c
	$(CC) -o $@ -c $< $(INCLUDES) $(Predefs) $(CFL)
$(i77objdir)/%.o: libi77/%.c
	$(CC) -o $@ -c $< $(INCLUDES) $(Predefs) $(CFL)
$(u77objdir)/%.o: libu77/%.c
	$(CC) -o $@ -c $< $(INCLUDES) $(Predefs) $(CFL)

MISC =	$(f77objdir)/F77_aloc.o \
	$(f77objdir)/Version.o \
	$(f77objdir)/main.o \
	$(f77objdir)/s_rnge.o \
	$(f77objdir)/abort_.o \
	$(f77objdir)/getarg_.o \
	$(f77objdir)/iargc_.o \
	$(f77objdir)/getenv_.o \
	$(f77objdir)/signal_.o \
	$(f77objdir)/s_stop.o \
	$(f77objdir)/s_paus.o \
	$(f77objdir)/system_.o \
	$(f77objdir)/cabs.o \
	$(f77objdir)/derf_.o \
	$(f77objdir)/derfc_.o \
	$(f77objdir)/erf_.o \
	$(f77objdir)/erfc_.o \
	$(f77objdir)/sig_die.o \
	$(f77objdir)/exit_.o \
	$(f77objdir)/setarg.o \
	$(f77objdir)/setsig.o \
	$(f77objdir)/dtime_.o \
	$(f77objdir)/etime_.o

POW =	$(f77objdir)/pow_ci.o \
	$(f77objdir)/pow_dd.o \
	$(f77objdir)/pow_di.o \
	$(f77objdir)/pow_hh.o \
	$(f77objdir)/pow_ii.o \
	$(f77objdir)/pow_ri.o \
	$(f77objdir)/pow_zi.o \
	$(f77objdir)/pow_zz.o \
	$(f77objdir)/pow_qq.o

CX =	$(f77objdir)/c_abs.o \
	$(f77objdir)/c_cos.o \
	$(f77objdir)/c_div.o \
	$(f77objdir)/c_exp.o \
	$(f77objdir)/c_log.o \
	$(f77objdir)/c_sin.o \
	$(f77objdir)/c_sqrt.o

DCX =	$(f77objdir)/z_abs.o \
	$(f77objdir)/z_cos.o \
	$(f77objdir)/z_div.o \
	$(f77objdir)/z_exp.o \
	$(f77objdir)/z_log.o \
	$(f77objdir)/z_sin.o \
	$(f77objdir)/z_sqrt.o

REAL =	$(f77objdir)/r_abs.o \
	$(f77objdir)/r_acos.o \
	$(f77objdir)/r_asin.o \
	$(f77objdir)/r_atan.o \
	$(f77objdir)/r_atn2.o \
	$(f77objdir)/r_cnjg.o \
	$(f77objdir)/r_cos.o \
	$(f77objdir)/r_cosh.o \
	$(f77objdir)/r_dim.o \
	$(f77objdir)/r_exp.o \
	$(f77objdir)/r_imag.o \
	$(f77objdir)/r_int.o \
	$(f77objdir)/r_lg10.o \
	$(f77objdir)/r_log.o \
	$(f77objdir)/r_mod.o \
	$(f77objdir)/r_nint.o \
	$(f77objdir)/r_sign.o \
	$(f77objdir)/r_sin.o \
	$(f77objdir)/r_sinh.o \
	$(f77objdir)/r_sqrt.o \
	$(f77objdir)/r_tan.o \
	$(f77objdir)/r_tanh.o

DBL =	$(f77objdir)/d_abs.o \
	$(f77objdir)/d_acos.o \
	$(f77objdir)/d_asin.o \
	$(f77objdir)/d_atan.o \
	$(f77objdir)/d_atn2.o \
	$(f77objdir)/d_cnjg.o \
	$(f77objdir)/d_cos.o \
	$(f77objdir)/d_cosh.o \
	$(f77objdir)/d_dim.o \
	$(f77objdir)/d_exp.o \
	$(f77objdir)/d_imag.o \
	$(f77objdir)/d_int.o \
	$(f77objdir)/d_lg10.o \
	$(f77objdir)/d_log.o \
	$(f77objdir)/d_mod.o \
	$(f77objdir)/d_nint.o \
	$(f77objdir)/d_prod.o \
	$(f77objdir)/d_sign.o \
	$(f77objdir)/d_sin.o \
	$(f77objdir)/d_sinh.o \
	$(f77objdir)/d_sqrt.o \
	$(f77objdir)/d_tan.o \
	$(f77objdir)/d_tanh.o

INT =	$(f77objdir)/i_abs.o \
	$(f77objdir)/i_dim.o \
	$(f77objdir)/i_dnnt.o \
	$(f77objdir)/i_indx.o \
	$(f77objdir)/i_len.o \
	$(f77objdir)/i_mod.o \
	$(f77objdir)/i_nint.o \
	$(f77objdir)/i_sign.o

HALF =	$(f77objdir)/h_abs.o \
	$(f77objdir)/h_dim.o \
	$(f77objdir)/h_dnnt.o \
	$(f77objdir)/h_indx.o \
	$(f77objdir)/h_len.o \
	$(f77objdir)/h_mod.o \
	$(f77objdir)/h_nint.o \
	$(f77objdir)/h_sign.o

CMP =	$(f77objdir)/l_ge.o \
	$(f77objdir)/l_gt.o \
	$(f77objdir)/l_le.o \
	$(f77objdir)/l_lt.o \
	$(f77objdir)/hl_ge.o \
	$(f77objdir)/hl_gt.o \
	$(f77objdir)/hl_le.o \
	$(f77objdir)/hl_lt.o

EFL =	$(f77objdir)/ef1asc_.o \
	$(f77objdir)/ef1cmc_.o

CHAR =	$(f77objdir)/s_cat.o \
	$(f77objdir)/s_cmp.o \
	$(f77objdir)/s_copy.o

F90BIT = $(f77objdir)/lbitbits.o \
	$(f77objdir)/lbitshft.o \
	$(f77objdir)/qbitbits.o \
	$(f77objdir)/qbitshft.o

FOBJ = $(MISC) $(POW) $(CX) $(DCX) $(REAL) $(DBL) $(INT) $(HALF) $(CMP) \
 $(EFL) $(CHAR) $(F90BIT)

IOBJ =	$(i77objdir)/Version.o \
	$(i77objdir)/backspace.o \
	$(i77objdir)/close.o \
	$(i77objdir)/dfe.o \
	$(i77objdir)/dolio.o \
	$(i77objdir)/due.o \
	$(i77objdir)/endfile.o \
	$(i77objdir)/err.o \
	$(i77objdir)/fmt.o \
	$(i77objdir)/fmtlib.o \
	$(i77objdir)/iio.o \
	$(i77objdir)/ilnw.o \
	$(i77objdir)/inquire.o \
	$(i77objdir)/lread.o \
	$(i77objdir)/lwrite.o \
	$(i77objdir)/open.o \
	$(i77objdir)/rdfmt.o \
	$(i77objdir)/rewind.o \
	$(i77objdir)/rsfe.o \
	$(i77objdir)/rsli.o \
	$(i77objdir)/rsne.o \
	$(i77objdir)/sfe.o \
	$(i77objdir)/sue.o \
	$(i77objdir)/typesize.o \
	$(i77objdir)/uio.o \
	$(i77objdir)/util.o \
	$(i77objdir)/wref.o \
	$(i77objdir)/wrtfmt.o \
	$(i77objdir)/wsfe.o \
	$(i77objdir)/wsle.o \
	$(i77objdir)/wsne.o \
	$(i77objdir)/xwsne.o \
	$(i77objdir)/ftell_.o

UOBJ =  $(u77objdir)/Version.o \
	$(u77objdir)/gerror_.o \
	$(u77objdir)/perror_.o \
	$(u77objdir)/ierrno_.o \
	$(u77objdir)/itime_.o \
	$(u77objdir)/time_.o \
	$(u77objdir)/unlink_.o \
	$(u77objdir)/fnum_.o \
	$(u77objdir)/getpid_.o \
	$(u77objdir)/getuid_.o \
	$(u77objdir)/getgid_.o \
	$(u77objdir)/kill_.o \
	$(u77objdir)/rand_.o \
	$(u77objdir)/srand_.o \
	$(u77objdir)/irand_.o \
	$(u77objdir)/sleep_.o \
	$(u77objdir)/idate_.o \
	$(u77objdir)/ctime_.o \
	$(u77objdir)/etime_.o \
	$(u77objdir)/dtime_.o \
	$(u77objdir)/isatty_.o \
	$(u77objdir)/ltime_.o \
	$(u77objdir)/fstat_.o \
	$(u77objdir)/stat_.o \
	$(u77objdir)/lstat_.o \
	$(u77objdir)/access_.o \
	$(u77objdir)/link_.o \
	$(u77objdir)/getlog_.o \
	$(u77objdir)/ttynam_.o \
	$(u77objdir)/getcwd_.o \
	$(u77objdir)/vxttime_.o \
	$(u77objdir)/vxtidate_.o \
	$(u77objdir)/gmtime_.o \
	$(u77objdir)/fdate_.o \
	$(u77objdir)/secnds_.o \
	$(u77objdir)/bes.o \
	$(u77objdir)/dbes.o \
	$(u77objdir)/chdir_.o \
	$(u77objdir)/chmod_.o \
	$(u77objdir)/lnblnk_.o \
	$(u77objdir)/hostnm_.o \
	$(u77objdir)/rename_.o \
	$(u77objdir)/fgetc_.o \
	$(u77objdir)/fputc_.o \
	$(u77objdir)/umask_.o \
	$(u77objdir)/sys_clock_.o \
	$(u77objdir)/date_.o \
	$(u77objdir)/second_.o \
	$(u77objdir)/flush1_.o \
	$(u77objdir)/alarm_.o \
	$(u77objdir)/mclock_.o \
	$(u77objdir)/symlnk_.o \
	$(u77objdir)/datetime_.o

F2CEXT = $(f2cobjdir)/abort.o \
	$(f2cobjdir)/derf.o \
	$(f2cobjdir)/derfc.o \
	$(f2cobjdir)/ef1asc.o \
	$(f2cobjdir)/ef1cmc.o \
	$(f2cobjdir)/erf.o \
	$(f2cobjdir)/erfc.o \
	$(f2cobjdir)/exit.o \
	$(f2cobjdir)/getarg.o \
	$(f2cobjdir)/getenv.o \
	$(f2cobjdir)/iargc.o \
	$(f2cobjdir)/signal.o \
	$(f2cobjdir)/flush.o \
	$(f2cobjdir)/ftell.o \
	$(f2cobjdir)/fseek.o \
	$(f2cobjdir)/access.o \
	$(f2cobjdir)/besj0.o \
	$(f2cobjdir)/besj1.o \
	$(f2cobjdir)/besjn.o \
	$(f2cobjdir)/besy0.o \
	$(f2cobjdir)/besy1.o \
	$(f2cobjdir)/besyn.o \
	$(f2cobjdir)/chdir.o \
	$(f2cobjdir)/chmod.o \
	$(f2cobjdir)/ctime.o \
	$(f2cobjdir)/date.o \
	$(f2cobjdir)/dbesj0.o \
	$(f2cobjdir)/dbesj1.o \
	$(f2cobjdir)/dbesjn.o \
	$(f2cobjdir)/dbesy0.o \
	$(f2cobjdir)/dbesy1.o \
	$(f2cobjdir)/dbesyn.o \
	$(f2cobjdir)/dtime.o \
	$(f2cobjdir)/etime.o \
	$(f2cobjdir)/fdate.o \
	$(f2cobjdir)/fgetc.o \
	$(f2cobjdir)/fget.o \
	$(f2cobjdir)/flush1.o \
	$(f2cobjdir)/fnum.o \
	$(f2cobjdir)/fputc.o \
	$(f2cobjdir)/fput.o \
	$(f2cobjdir)/fstat.o \
	$(f2cobjdir)/gerror.o

F2CEXT2 = $(f2cobjdir)/getcwd.o \
	$(f2cobjdir)/getgid.o \
	$(f2cobjdir)/getlog.o \
	$(f2cobjdir)/getpid.o \
	$(f2cobjdir)/getuid.o \
	$(f2cobjdir)/gmtime.o \
	$(f2cobjdir)/hostnm.o \
	$(f2cobjdir)/idate.o \
	$(f2cobjdir)/ierrno.o \
	$(f2cobjdir)/irand.o \
	$(f2cobjdir)/isatty.o \
	$(f2cobjdir)/itime.o \
	$(f2cobjdir)/kill.o \
	$(f2cobjdir)/link.o \
	$(f2cobjdir)/lnblnk.o \
	$(f2cobjdir)/lstat.o \
	$(f2cobjdir)/ltime.o \
	$(f2cobjdir)/mclock.o \
	$(f2cobjdir)/perror.o \
	$(f2cobjdir)/rand.o \
	$(f2cobjdir)/rename.o \
	$(f2cobjdir)/secnds.o \
	$(f2cobjdir)/second.o \
	$(f2cobjdir)/sleep.o \
	$(f2cobjdir)/srand.o \
	$(f2cobjdir)/stat.o \
	$(f2cobjdir)/symlnk.o \
	$(f2cobjdir)/sclock.o \
	$(f2cobjdir)/system.o \
	$(f2cobjdir)/time.o \
	$(f2cobjdir)/ttynam.o \
	$(f2cobjdir)/umask.o \
	$(f2cobjdir)/unlink.o \
	$(f2cobjdir)/vxtidt.o \
	$(f2cobjdir)/vxttim.o \
	$(f2cobjdir)/alarm.o

all:	mkdirs $(TBIN)/$(TRG)/libg2c.o

.PHONY: mkdirs

mkdirs:
	mkdir -p $(f2cobjdir) $(u77objdir) $(i77objdir) $(f77objdir)

$(TBIN)/$(TRG)/libg2c.o:	$(FOBJ) $(IOBJ) $(UOBJ) $(F2CEXT) $(F2CEXT2)
	$(BIN)/libfile -c $@ $(FOBJ) $(IOBJ) $(UOBJ) $(F2CEXT) $(F2CEXT2)

# The f2cext.c thang

$(f2cobjdir)/abort.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLabort

$(f2cobjdir)/derf.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLderf

$(f2cobjdir)/derfc.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLderfc

$(f2cobjdir)/ef1asc.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLef1asc

$(f2cobjdir)/ef1cmc.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLef1cmc

$(f2cobjdir)/erf.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLerf

$(f2cobjdir)/erfc.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLerfc

$(f2cobjdir)/exit.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLexit

$(f2cobjdir)/getarg.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLgetarg

$(f2cobjdir)/getenv.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLgetenv

$(f2cobjdir)/iargc.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLiargc

$(f2cobjdir)/signal.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLsignal

$(f2cobjdir)/flush.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLflush

$(f2cobjdir)/ftell.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLftell

$(f2cobjdir)/fseek.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLfseek

$(f2cobjdir)/access.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLaccess

$(f2cobjdir)/besj0.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLbesj0

$(f2cobjdir)/besj1.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLbesj1

$(f2cobjdir)/besjn.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLbesjn

$(f2cobjdir)/besy0.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLbesy0

$(f2cobjdir)/besy1.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLbesy1

$(f2cobjdir)/besyn.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLbesyn

$(f2cobjdir)/chdir.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLchdir

$(f2cobjdir)/chmod.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLchmod

$(f2cobjdir)/ctime.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLctime

$(f2cobjdir)/date.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLdate

$(f2cobjdir)/dbesj0.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLdbesj0

$(f2cobjdir)/dbesj1.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLdbesj1

$(f2cobjdir)/dbesjn.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLdbesjn

$(f2cobjdir)/dbesy0.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLdbesy0

$(f2cobjdir)/dbesy1.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLdbesy1

$(f2cobjdir)/dbesyn.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLdbesyn

$(f2cobjdir)/dtime.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLdtime

$(f2cobjdir)/etime.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLetime

$(f2cobjdir)/fdate.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLfdate

$(f2cobjdir)/fgetc.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLfgetc

$(f2cobjdir)/fget.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLfget

$(f2cobjdir)/flush1.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLflush1

$(f2cobjdir)/fnum.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLfnum

$(f2cobjdir)/fputc.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLfputc

$(f2cobjdir)/fput.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLfput

$(f2cobjdir)/fstat.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLfstat

$(f2cobjdir)/gerror.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLgerror

$(f2cobjdir)/getcwd.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLgetcwd

$(f2cobjdir)/getgid.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLgetgid

$(f2cobjdir)/getlog.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLgetlog

$(f2cobjdir)/getpid.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLgetpid

$(f2cobjdir)/getuid.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLgetuid

$(f2cobjdir)/gmtime.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLgmtime

$(f2cobjdir)/hostnm.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLhostnm

$(f2cobjdir)/idate.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLidate

$(f2cobjdir)/ierrno.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLierrno

$(f2cobjdir)/irand.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLirand

$(f2cobjdir)/isatty.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLisatty

$(f2cobjdir)/itime.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLitime

$(f2cobjdir)/kill.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLkill

$(f2cobjdir)/link.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLlink

$(f2cobjdir)/lnblnk.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLlnblnk

$(f2cobjdir)/lstat.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLlstat

$(f2cobjdir)/ltime.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLltime

$(f2cobjdir)/mclock.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLmclock

$(f2cobjdir)/perror.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLperror

$(f2cobjdir)/rand.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLrand

$(f2cobjdir)/rename.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLrename

$(f2cobjdir)/secnds.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLsecnds

$(f2cobjdir)/second.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLsecond

$(f2cobjdir)/sleep.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLsleep

$(f2cobjdir)/srand.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLsrand

$(f2cobjdir)/stat.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLstat

$(f2cobjdir)/symlnk.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLsymlnk

$(f2cobjdir)/system.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLsystem

$(f2cobjdir)/sclock.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLsclock

$(f2cobjdir)/time.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLtime

$(f2cobjdir)/ttynam.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLttynam

$(f2cobjdir)/umask.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLumask

$(f2cobjdir)/unlink.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLunlink

$(f2cobjdir)/vxtidt.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLvxtidt

$(f2cobjdir)/vxttim.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLvxttim

$(f2cobjdir)/alarm.o:	f2cext.c
	$(CC) -o $@ -c f2cext.c $(INCLUDES) $(CFL) $(Predefs) -DLalarm


$(f77objdir)/abort_.o: libf77/abort_.c
$(f77objdir)/cabs.o: libf77/cabs.c
$(f77objdir)/c_abs.o: libf77/c_abs.c
$(f77objdir)/c_cos.o: libf77/c_cos.c
$(f77objdir)/c_div.o: libf77/c_div.c
$(f77objdir)/c_exp.o: libf77/c_exp.c
$(f77objdir)/c_log.o: libf77/c_log.c
$(f77objdir)/c_sin.o: libf77/c_sin.c
$(f77objdir)/c_sqrt.o: libf77/c_sqrt.c
$(f77objdir)/derfc_.o: libf77/derfc_.c
$(f77objdir)/derf_.o: libf77/derf_.c
$(f77objdir)/dtime_.o: libf77/dtime_.c
$(f77objdir)/d_abs.o: libf77/d_abs.c
$(f77objdir)/d_acos.o: libf77/d_acos.c
$(f77objdir)/d_asin.o: libf77/d_asin.c
$(f77objdir)/d_atan.o: libf77/d_atan.c
$(f77objdir)/d_atn2.o: libf77/d_atn2.c
$(f77objdir)/d_cnjg.o: libf77/d_cnjg.c
$(f77objdir)/d_cos.o: libf77/d_cos.c
$(f77objdir)/d_cosh.o: libf77/d_cosh.c
$(f77objdir)/d_dim.o: libf77/d_dim.c
$(f77objdir)/d_exp.o: libf77/d_exp.c
$(f77objdir)/d_imag.o: libf77/d_imag.c
$(f77objdir)/d_int.o: libf77/d_int.c
$(f77objdir)/d_lg10.o: libf77/d_lg10.c
$(f77objdir)/d_log.o: libf77/d_log.c
$(f77objdir)/d_mod.o: libf77/d_mod.c
$(f77objdir)/d_nint.o: libf77/d_nint.c
$(f77objdir)/d_prod.o: libf77/d_prod.c
$(f77objdir)/d_sign.o: libf77/d_sign.c
$(f77objdir)/d_sin.o: libf77/d_sin.c
$(f77objdir)/d_sinh.o: libf77/d_sinh.c
$(f77objdir)/d_sqrt.o: libf77/d_sqrt.c
$(f77objdir)/d_tan.o: libf77/d_tan.c
$(f77objdir)/d_tanh.o: libf77/d_tanh.c
$(f77objdir)/ef1asc_.o: libf77/ef1asc_.c
$(f77objdir)/ef1cmc_.o: libf77/ef1cmc_.c
$(f77objdir)/erfc_.o: libf77/erfc_.c
$(f77objdir)/erf_.o: libf77/erf_.c
$(f77objdir)/etime_.o: libf77/etime_.c
$(f77objdir)/exit_.o: libf77/exit_.c
$(f77objdir)/F77_aloc.o: libf77/F77_aloc.c
$(f77objdir)/getarg_.o: libf77/getarg_.c
$(f77objdir)/getenv_.o: libf77/getenv_.c
$(f77objdir)/hl_ge.o: libf77/hl_ge.c
$(f77objdir)/hl_gt.o: libf77/hl_gt.c
$(f77objdir)/hl_le.o: libf77/hl_le.c
$(f77objdir)/hl_lt.o: libf77/hl_lt.c
$(f77objdir)/h_abs.o: libf77/h_abs.c
$(f77objdir)/h_dim.o: libf77/h_dim.c
$(f77objdir)/h_dnnt.o: libf77/h_dnnt.c
$(f77objdir)/h_indx.o: libf77/h_indx.c
$(f77objdir)/h_len.o: libf77/h_len.c
$(f77objdir)/h_mod.o: libf77/h_mod.c
$(f77objdir)/h_nint.o: libf77/h_nint.c
$(f77objdir)/h_sign.o: libf77/h_sign.c
$(f77objdir)/iargc_.o: libf77/iargc_.c
$(f77objdir)/i_abs.o: libf77/i_abs.c
$(f77objdir)/i_dim.o: libf77/i_dim.c
$(f77objdir)/i_dnnt.o: libf77/i_dnnt.c
$(f77objdir)/i_indx.o: libf77/i_indx.c
$(f77objdir)/i_len.o: libf77/i_len.c
$(f77objdir)/i_mod.o: libf77/i_mod.c
$(f77objdir)/i_nint.o: libf77/i_nint.c
$(f77objdir)/i_sign.o: libf77/i_sign.c
$(f77objdir)/lbitbits.o: libf77/lbitbits.c
$(f77objdir)/lbitshft.o: libf77/lbitshft.c
$(f77objdir)/l_ge.o: libf77/l_ge.c
$(f77objdir)/l_gt.o: libf77/l_gt.c
$(f77objdir)/l_le.o: libf77/l_le.c
$(f77objdir)/l_lt.o: libf77/l_lt.c
$(f77objdir)/main.o: libf77/main.c
$(f77objdir)/pow_ci.o: libf77/pow_ci.c
$(f77objdir)/pow_dd.o: libf77/pow_dd.c
$(f77objdir)/pow_di.o: libf77/pow_di.c
$(f77objdir)/pow_hh.o: libf77/pow_hh.c
$(f77objdir)/pow_ii.o: libf77/pow_ii.c
$(f77objdir)/pow_qq.o: libf77/pow_qq.c
$(f77objdir)/pow_ri.o: libf77/pow_ri.c
$(f77objdir)/pow_zi.o: libf77/pow_zi.c
$(f77objdir)/pow_zz.o: libf77/pow_zz.c
$(f77objdir)/qbitbits.o: libf77/qbitbits.c
$(f77objdir)/qbitshft.o: libf77/qbitshft.c
$(f77objdir)/r_abs.o: libf77/r_abs.c
$(f77objdir)/r_acos.o: libf77/r_acos.c
$(f77objdir)/r_asin.o: libf77/r_asin.c
$(f77objdir)/r_atan.o: libf77/r_atan.c
$(f77objdir)/r_atn2.o: libf77/r_atn2.c
$(f77objdir)/r_cnjg.o: libf77/r_cnjg.c
$(f77objdir)/r_cos.o: libf77/r_cos.c
$(f77objdir)/r_cosh.o: libf77/r_cosh.c
$(f77objdir)/r_dim.o: libf77/r_dim.c
$(f77objdir)/r_exp.o: libf77/r_exp.c
$(f77objdir)/r_imag.o: libf77/r_imag.c
$(f77objdir)/r_int.o: libf77/r_int.c
$(f77objdir)/r_lg10.o: libf77/r_lg10.c
$(f77objdir)/r_log.o: libf77/r_log.c
$(f77objdir)/r_mod.o: libf77/r_mod.c
$(f77objdir)/r_nint.o: libf77/r_nint.c
$(f77objdir)/r_sign.o: libf77/r_sign.c
$(f77objdir)/r_sin.o: libf77/r_sin.c
$(f77objdir)/r_sinh.o: libf77/r_sinh.c
$(f77objdir)/r_sqrt.o: libf77/r_sqrt.c
$(f77objdir)/r_tan.o: libf77/r_tan.c
$(f77objdir)/r_tanh.o: libf77/r_tanh.c
$(f77objdir)/setarg.o: libf77/setarg.c
$(f77objdir)/setsig.o: libf77/setsig.c
$(f77objdir)/signal_.o: libf77/signal_.c
$(f77objdir)/sig_die.o: libf77/sig_die.c
$(f77objdir)/system_.o: libf77/system_.c
$(f77objdir)/s_cat.o: libf77/s_cat.c
$(f77objdir)/s_cmp.o: libf77/s_cmp.c
$(f77objdir)/s_copy.o: libf77/s_copy.c
$(f77objdir)/s_paus.o: libf77/s_paus.c
$(f77objdir)/s_rnge.o: libf77/s_rnge.c
$(f77objdir)/s_stop.o: libf77/s_stop.c
$(f77objdir)/Version.o: libf77/Version.c
$(f77objdir)/z_abs.o: libf77/z_abs.c
$(f77objdir)/z_cos.o: libf77/z_cos.c
$(f77objdir)/z_div.o: libf77/z_div.c
$(f77objdir)/z_exp.o: libf77/z_exp.c
$(f77objdir)/z_log.o: libf77/z_log.c
$(f77objdir)/z_sin.o: libf77/z_sin.c
$(f77objdir)/z_sqrt.o: libf77/z_sqrt.c

$(i77objdir)/backspace.o: libi77/backspace.c
$(i77objdir)/close.o: libi77/close.c
$(i77objdir)/dfe.o: libi77/dfe.c
$(i77objdir)/dolio.o: libi77/dolio.c
$(i77objdir)/due.o: libi77/due.c
$(i77objdir)/endfile.o: libi77/endfile.c
$(i77objdir)/err.o: libi77/err.c
$(i77objdir)/fmt.o: libi77/fmt.c
$(i77objdir)/fmtlib.o: libi77/fmtlib.c
$(i77objdir)/ftell_.o: libi77/ftell_.c
$(i77objdir)/iio.o: libi77/iio.c
$(i77objdir)/ilnw.o: libi77/ilnw.c
$(i77objdir)/inquire.o: libi77/inquire.c
$(i77objdir)/lread.o: libi77/lread.c
$(i77objdir)/lwrite.o: libi77/lwrite.c
$(i77objdir)/open.o: libi77/open.c
$(i77objdir)/rdfmt.o: libi77/rdfmt.c
$(i77objdir)/rewind.o: libi77/rewind.c
$(i77objdir)/rsfe.o: libi77/rsfe.c
$(i77objdir)/rsli.o: libi77/rsli.c
$(i77objdir)/rsne.o: libi77/rsne.c
$(i77objdir)/sfe.o: libi77/sfe.c
$(i77objdir)/sue.o: libi77/sue.c
$(i77objdir)/typesize.o: libi77/typesize.c
$(i77objdir)/uio.o: libi77/uio.c
$(i77objdir)/util.o: libi77/util.c
$(i77objdir)/Version.o: libi77/Version.c
$(i77objdir)/wref.o: libi77/wref.c
$(i77objdir)/wrtfmt.o: libi77/wrtfmt.c
$(i77objdir)/wsfe.o: libi77/wsfe.c
$(i77objdir)/wsle.o: libi77/wsle.c
$(i77objdir)/wsne.o: libi77/wsne.c
$(i77objdir)/xwsne.o: libi77/xwsne.c

$(u77objdir)/access_.o: libu77/access_.c
$(u77objdir)/alarm_.o: libu77/alarm_.c
$(u77objdir)/bes.o: libu77/bes.c
$(u77objdir)/chdir_.o: libu77/chdir_.c
$(u77objdir)/chmod_.o: libu77/chmod_.c
$(u77objdir)/ctime_.o: libu77/ctime_.c
$(u77objdir)/datetime_.o: libu77/datetime_.c
$(u77objdir)/date_.o: libu77/date_.c
$(u77objdir)/dbes.o: libu77/dbes.c
$(u77objdir)/dtime_.o: libu77/dtime_.c
$(u77objdir)/etime_.o: libu77/etime_.c
$(u77objdir)/fdate_.o: libu77/fdate_.c
$(u77objdir)/fgetc_.o: libu77/fgetc_.c
$(u77objdir)/flush1_.o: libu77/flush1_.c
$(u77objdir)/fnum_.o: libu77/fnum_.c
$(u77objdir)/fputc_.o: libu77/fputc_.c
$(u77objdir)/fstat_.o: libu77/fstat_.c
$(u77objdir)/gerror_.o: libu77/gerror_.c
$(u77objdir)/getcwd_.o: libu77/getcwd_.c
$(u77objdir)/getgid_.o: libu77/getgid_.c
$(u77objdir)/getlog_.o: libu77/getlog_.c
$(u77objdir)/getpid_.o: libu77/getpid_.c
$(u77objdir)/getuid_.o: libu77/getuid_.c
$(u77objdir)/gmtime_.o: libu77/gmtime_.c
$(u77objdir)/hostnm_.o: libu77/hostnm_.c
$(u77objdir)/idate_.o: libu77/idate_.c
$(u77objdir)/ierrno_.o: libu77/ierrno_.c
$(u77objdir)/irand_.o: libu77/irand_.c
$(u77objdir)/isatty_.o: libu77/isatty_.c
$(u77objdir)/itime_.o: libu77/itime_.c
$(u77objdir)/kill_.o: libu77/kill_.c
$(u77objdir)/link_.o: libu77/link_.c
$(u77objdir)/lnblnk_.o: libu77/lnblnk_.c
$(u77objdir)/lstat_.o: libu77/lstat_.c
$(u77objdir)/ltime_.o: libu77/ltime_.c
$(u77objdir)/mclock_.o: libu77/mclock_.c
$(u77objdir)/perror_.o: libu77/perror_.c
$(u77objdir)/rand_.o: libu77/rand_.c
$(u77objdir)/rename_.o: libu77/rename_.c
$(u77objdir)/secnds_.o: libu77/secnds_.c
$(u77objdir)/second_.o: libu77/second_.c
$(u77objdir)/sleep_.o: libu77/sleep_.c
$(u77objdir)/srand_.o: libu77/srand_.c
$(u77objdir)/stat_.o: libu77/stat_.c
$(u77objdir)/symlnk_.o: libu77/symlnk_.c
$(u77objdir)/sys_clock_.o: libu77/sys_clock_.c
$(u77objdir)/time_.o: libu77/time_.c
$(u77objdir)/ttynam_.o: libu77/ttynam_.c
$(u77objdir)/umask_.o: libu77/umask_.c
$(u77objdir)/unlink_.o: libu77/unlink_.c
$(u77objdir)/Version.o: libu77/Version.c
$(u77objdir)/vxtidate_.o: libu77/vxtidate_.c
$(u77objdir)/vxttime_.o: libu77/vxttime_.c
