# Makefile for GNU Libstdc++ for the ARM/RISC OS
# Copyright (c) 1997-2000 Nick Burrett
# Written by Nick Burrett <nick@dsvr.net>
#
# This file is part of the GNU Compiler Collection
#

OPTS = -mthrowback -mpoke-function-name
#OPTS = -mthrowback -mno-apcs-frame -mno-apcs-stack-check -muse-arg-pointer

# Defs for GCC
CC = $(standard_exec_prefix)/gcc -c $(CFLAGS) $(INCLUDES)
CFLAGS = -O2 $(OPTS) -DIN_GCC -DHAVE_CONFIG_H $(TARGET_DEFS) $(CFL)
INCLUDES = -I. -I../libio -I./stl

stdcobjdir=$(objdir)/gcc-$(VERSION)/libstdc/$(TRG)

# String objects put in directory o
# Float versions put in directory o
# Double versions put in directory o
# Long double versions put in directory o
# STL objects put in directory stl.o
# Because there are too many object files for one directory.

MAIN_OBJS = $(stdcobjdir)/cmathi.o \
	$(stdcobjdir)/cstdlibi.o \
	$(stdcobjdir)/cstringi.o \
	$(stdcobjdir)/stlinst.o \
	$(stdcobjdir)/stdexcepti.o \
	$(stdcobjdir)/valarray.o

STR_OBJS = $(stdcobjdir)/crep.o \
	$(stdcobjdir)/cmain.o \
	$(stdcobjdir)/ctraits.o \
	$(stdcobjdir)/caddss.o \
	$(stdcobjdir)/caddps.o \
	$(stdcobjdir)/caddcs.o \
	$(stdcobjdir)/caddsp.o \
	$(stdcobjdir)/caddsc.o \
	$(stdcobjdir)/ceqss.o \
	$(stdcobjdir)/ceqps.o \
	$(stdcobjdir)/ceqsp.o \
	$(stdcobjdir)/cness.o \
	$(stdcobjdir)/cneps.o \
	$(stdcobjdir)/cnesp.o \
	$(stdcobjdir)/cextract.o \
	$(stdcobjdir)/cinsert.o \
	$(stdcobjdir)/cgetline.o

COM_FOBJS = $(stdcobjdir)/fmain.o \
	$(stdcobjdir)/faddcc.o \
	$(stdcobjdir)/faddcf.o \
	$(stdcobjdir)/faddfc.o \
	$(stdcobjdir)/fsubcc.o \
	$(stdcobjdir)/fsubcf.o \
	$(stdcobjdir)/fsubfc.o \
	$(stdcobjdir)/fmulcc.o \
	$(stdcobjdir)/fmulcf.o \
	$(stdcobjdir)/fmulfc.o \
	$(stdcobjdir)/fdivcc.o \
	$(stdcobjdir)/fdivcf.o \
	$(stdcobjdir)/fdivfc.o \
	$(stdcobjdir)/fplus.o \
	$(stdcobjdir)/fminus.o \
	$(stdcobjdir)/feqcc.o \
	$(stdcobjdir)/feqcf.o \
	$(stdcobjdir)/feqfc.o \
	$(stdcobjdir)/fnecc.o \
	$(stdcobjdir)/fnecf.o \
	$(stdcobjdir)/fnefc.o \
	$(stdcobjdir)/fabs.o \
	$(stdcobjdir)/farg.o \
	$(stdcobjdir)/fpolar.o \
	$(stdcobjdir)/fconj.o \
	$(stdcobjdir)/fnorm.o \
	$(stdcobjdir)/fcos.o \
	$(stdcobjdir)/fcosh.o \
	$(stdcobjdir)/fexp.o \
	$(stdcobjdir)/flog.o \
	$(stdcobjdir)/fpowcc.o \
	$(stdcobjdir)/fpowcf.o \
	$(stdcobjdir)/fpowci.o \
	$(stdcobjdir)/fpowfc.o \
	$(stdcobjdir)/fsin.o \
	$(stdcobjdir)/fsinh.o \
	$(stdcobjdir)/fsqrt.o \
	$(stdcobjdir)/fextract.o \
	$(stdcobjdir)/finsert.o

COM_DOBJS = $(stdcobjdir)/dmain.o \
	$(stdcobjdir)/daddcc.o \
	$(stdcobjdir)/daddcf.o \
	$(stdcobjdir)/daddfc.o \
	$(stdcobjdir)/dsubcc.o \
	$(stdcobjdir)/dsubcf.o \
	$(stdcobjdir)/dsubfc.o \
	$(stdcobjdir)/dmulcc.o \
	$(stdcobjdir)/dmulcf.o \
	$(stdcobjdir)/dmulfc.o \
	$(stdcobjdir)/ddivcc.o \
	$(stdcobjdir)/ddivcf.o \
	$(stdcobjdir)/ddivfc.o \
	$(stdcobjdir)/dplus.o \
	$(stdcobjdir)/dminus.o \
	$(stdcobjdir)/deqcc.o \
	$(stdcobjdir)/deqcf.o \
	$(stdcobjdir)/deqfc.o \
	$(stdcobjdir)/dnecc.o \
	$(stdcobjdir)/dnecf.o \
	$(stdcobjdir)/dnefc.o \
	$(stdcobjdir)/dabs.o \
	$(stdcobjdir)/darg.o \
	$(stdcobjdir)/dpolar.o \
	$(stdcobjdir)/dconj.o \
	$(stdcobjdir)/dnorm.o \
	$(stdcobjdir)/dcos.o \
	$(stdcobjdir)/dcosh.o \
	$(stdcobjdir)/dexp.o \
	$(stdcobjdir)/dlog.o \
	$(stdcobjdir)/dpowcc.o \
	$(stdcobjdir)/dpowcf.o \
	$(stdcobjdir)/dpowci.o \
	$(stdcobjdir)/dpowfc.o \
	$(stdcobjdir)/dsin.o \
	$(stdcobjdir)/dsinh.o \
	$(stdcobjdir)/dsqrt.o \
	$(stdcobjdir)/dextract.o \
	$(stdcobjdir)/dinsert.o

COM_LDOBJS = $(stdcobjdir)/ldmain.o \
	$(stdcobjdir)/ldaddcc.o \
	$(stdcobjdir)/ldaddcf.o \
	$(stdcobjdir)/ldaddfc.o \
	$(stdcobjdir)/ldsubcc.o \
	$(stdcobjdir)/ldsubcf.o \
	$(stdcobjdir)/ldsubfc.o \
	$(stdcobjdir)/ldmulcc.o \
	$(stdcobjdir)/ldmulcf.o \
	$(stdcobjdir)/ldmulfc.o \
	$(stdcobjdir)/lddivcc.o \
	$(stdcobjdir)/lddivcf.o \
	$(stdcobjdir)/lddivfc.o \
	$(stdcobjdir)/ldplus.o \
	$(stdcobjdir)/ldminus.o \
	$(stdcobjdir)/ldeqcc.o \
	$(stdcobjdir)/ldeqcf.o \
	$(stdcobjdir)/ldeqfc.o \
	$(stdcobjdir)/ldnecc.o \
	$(stdcobjdir)/ldnecf.o \
	$(stdcobjdir)/ldnefc.o \
	$(stdcobjdir)/ldabs.o \
	$(stdcobjdir)/ldarg.o \
	$(stdcobjdir)/ldpolar.o \
	$(stdcobjdir)/ldconj.o \
	$(stdcobjdir)/ldnorm.o \
	$(stdcobjdir)/ldcos.o \
	$(stdcobjdir)/ldcosh.o \
	$(stdcobjdir)/ldexp.o \
	$(stdcobjdir)/ldlog.o \
	$(stdcobjdir)/ldpowcc.o \
	$(stdcobjdir)/ldpowcf.o \
	$(stdcobjdir)/ldpowci.o \
	$(stdcobjdir)/ldpowfc.o \
	$(stdcobjdir)/ldsin.o \
	$(stdcobjdir)/ldsinh.o \
	$(stdcobjdir)/ldsqrt.o \
	$(stdcobjdir)/ldextract.o \
	$(stdcobjdir)/ldinsert.o

all:	$(TBIN)/$(TRG)/libstdc++.o

$(TBIN)/$(TRG)/libstdc++.o: $(STR_OBJS) $(COM_FOBJS) $(COM_DOBJS) $(COM_LDOBJS) $(MAIN_OBJS)
	$(BIN)/libfile -c $@ $(STR_OBJS) $(COM_FOBJS) $(COM_DOBJS) $(COM_LDOBJS) $(MAIN_OBJS)

install_headers:
	mkdir -p $(gplusplus_include_dir)/std
	cp stl/*.h $(gplusplus_include_dir)
	cp stl/algorithm $(gplusplus_include_dir)
	cp stl/bitset $(gplusplus_include_dir)
	cp stl/deque $(gplusplus_include_dir)
	cp stl/functional $(gplusplus_include_dir)
	cp stl/hash_map $(gplusplus_include_dir)
	cp stl/hash_set $(gplusplus_include_dir)
	cp stl/iterator $(gplusplus_include_dir)
	cp stl/list $(gplusplus_include_dir)
	cp stl/map $(gplusplus_include_dir)
	cp stl/memory $(gplusplus_include_dir)
	cp stl/numeric $(gplusplus_include_dir)
	cp stl/pthread_alloc $(gplusplus_include_dir)
	cp stl/queue $(gplusplus_include_dir)
	cp stl/rope $(gplusplus_include_dir)
	cp stl/set $(gplusplus_include_dir)
	cp stl/slist $(gplusplus_include_dir)
	cp stl/stack $(gplusplus_include_dir)
	cp stl/utility $(gplusplus_include_dir)
	cp stl/vector $(gplusplus_include_dir)
	cp std/*.h $(gplusplus_include_dir)/std
	cp cassert $(gplusplus_include_dir)
	cp cctype $(gplusplus_include_dir)
	cp cerrno $(gplusplus_include_dir)
	cp cfloat $(gplusplus_include_dir)
	cp ciso646 $(gplusplus_include_dir)
	cp climits $(gplusplus_include_dir)
	cp clocale $(gplusplus_include_dir)
	cp cmath $(gplusplus_include_dir)
	cp complex $(gplusplus_include_dir)
	cp complex.h $(gplusplus_include_dir)
	cp csetjmp $(gplusplus_include_dir)
	cp csignal $(gplusplus_include_dir)
	cp cstdarg $(gplusplus_include_dir)
	cp cstddef $(gplusplus_include_dir)
	cp cstdio $(gplusplus_include_dir)
	cp cstdlib $(gplusplus_include_dir)
	cp cstring $(gplusplus_include_dir)
	cp ctime $(gplusplus_include_dir)
	cp cwchar $(gplusplus_include_dir)
	cp cwctype $(gplusplus_include_dir)
	cp fstream $(gplusplus_include_dir)
	cp iomanip $(gplusplus_include_dir)
	cp iosfwd $(gplusplus_include_dir)
	cp iostream $(gplusplus_include_dir)
	cp stdexcept $(gplusplus_include_dir)
	cp string $(gplusplus_include_dir)
	cp strstream $(gplusplus_include_dir)
	cp valarray $(gplusplus_include_dir)

# String objects

$(stdcobjdir)/crep.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CC) -DC -DREP sinst.cc -o $@

$(stdcobjdir)/cmain.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CC) -DC -DMAIN sinst.cc -o $@

$(stdcobjdir)/ctraits.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CC) -DC -DTRAITS sinst.cc -o $@


$(stdcobjdir)/caddss.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CC) -DC -DADDSS sinst.cc -o $@

$(stdcobjdir)/caddps.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CC) -DC -DADDPS sinst.cc -o $@

$(stdcobjdir)/caddcs.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CC) -DC -DADDCS sinst.cc -o $@

$(stdcobjdir)/caddsp.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CC) -DC -DADDSP sinst.cc -o $@

$(stdcobjdir)/caddsc.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CC) -DC -DADDSC sinst.cc -o $@


$(stdcobjdir)/ceqss.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CC) -DC -DEQSS sinst.cc -o $@

$(stdcobjdir)/ceqps.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CC) -DC -DEQPS sinst.cc -o $@

$(stdcobjdir)/ceqsp.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CC) -DC -DEQSP sinst.cc -o $@


$(stdcobjdir)/cness.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CC) -DC -DNESS sinst.cc -o $@

$(stdcobjdir)/cneps.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CC) -DC -DNEPS sinst.cc -o $@

$(stdcobjdir)/cnesp.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CC) -DC -DNESP sinst.cc -o $@


$(stdcobjdir)/cextract.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CC) -DC -DEXTRACT sinst.cc -o $@

$(stdcobjdir)/cinsert.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CC) -DC -DINSERT sinst.cc -o $@

$(stdcobjdir)/cgetline.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CC) -DC -DGETLINE sinst.cc -o $@


$(stdcobjdir)/cltss.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CC) -DC -DLTSS sinst.cc -o $@

$(stdcobjdir)/cltps.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CC) -DC -DLTPS sinst.cc -o $@

$(stdcobjdir)/cltsp.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CC) -DC -DLTSP sinst.cc -o $@


$(stdcobjdir)/cgtss.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CC) -DC -DGTSS sinst.cc -o $@

$(stdcobjdir)/cgtps.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CC) -DC -DGTPS sinst.cc -o $@

$(stdcobjdir)/cgtsp.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CC) -DC -DGTSP sinst.cc -o $@


$(stdcobjdir)/cless.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CC) -DC -DLESS sinst.cc -o $@

$(stdcobjdir)/cleps.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CC) -DC -DLEPS sinst.cc -o $@

$(stdcobjdir)/clesp.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CC) -DC -DLESP sinst.cc -o $@


$(stdcobjdir)/cgess.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CC) -DC -DGESS sinst.cc -o $@

$(stdcobjdir)/cgeps.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CC) -DC -DGEPS sinst.cc -o $@

$(stdcobjdir)/cgesp.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CC) -DC -DGESP sinst.cc -o $@


#################################

# Complex objects

$(stdcobjdir)/fmain.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DF -DMAIN cinst.cc -o $@
$(stdcobjdir)/dmain.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DD -DMAIN cinst.cc -o $@
$(stdcobjdir)/ldmain.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DLD -DMAIN cinst.cc -o $@

$(stdcobjdir)/faddcc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DF -DADDCC cinst.cc -o $@
$(stdcobjdir)/daddcc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DD -DADDCC cinst.cc -o $@
$(stdcobjdir)/ldaddcc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DLD -DADDCC cinst.cc -o $@

$(stdcobjdir)/faddcf.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DF -DADDCF cinst.cc -o $@
$(stdcobjdir)/daddcf.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DD -DADDCF cinst.cc -o $@
$(stdcobjdir)/ldaddcf.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DLD -DADDCF cinst.cc -o $@

$(stdcobjdir)/faddfc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DF -DADDFC cinst.cc -o $@
$(stdcobjdir)/daddfc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DD -DADDFC cinst.cc -o $@
$(stdcobjdir)/ldaddfc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DLD -DADDFC cinst.cc -o $@

$(stdcobjdir)/fsubcc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DF -DSUBCC cinst.cc -o $@
$(stdcobjdir)/dsubcc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DD -DSUBCC cinst.cc -o $@
$(stdcobjdir)/ldsubcc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DLD -DSUBCC cinst.cc -o $@

$(stdcobjdir)/fsubcf.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DF -DSUBCF cinst.cc -o $@
$(stdcobjdir)/dsubcf.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DD -DSUBCF cinst.cc -o $@
$(stdcobjdir)/ldsubcf.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DLD -DSUBCF cinst.cc -o $@

$(stdcobjdir)/fsubfc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DF -DSUBFC cinst.cc -o $@
$(stdcobjdir)/dsubfc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DD -DSUBFC cinst.cc -o $@
$(stdcobjdir)/ldsubfc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DLD -DSUBFC cinst.cc -o $@

$(stdcobjdir)/fmulcc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DF -DMULCC cinst.cc -o $@
$(stdcobjdir)/dmulcc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DD -DMULCC cinst.cc -o $@
$(stdcobjdir)/ldmulcc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DLD -DMULCC cinst.cc -o $@

$(stdcobjdir)/fmulcf.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DF -DMULCF cinst.cc -o $@
$(stdcobjdir)/dmulcf.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DD -DMULCF cinst.cc -o $@
$(stdcobjdir)/ldmulcf.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DLD -DMULCF cinst.cc -o $@

$(stdcobjdir)/fmulfc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DF -DMULFC cinst.cc -o $@
$(stdcobjdir)/dmulfc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DD -DMULFC cinst.cc -o $@
$(stdcobjdir)/ldmulfc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DLD -DMULFC cinst.cc -o $@

$(stdcobjdir)/fdivcc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DF -DDIVCC cinst.cc -o $@
$(stdcobjdir)/ddivcc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DD -DDIVCC cinst.cc -o $@
$(stdcobjdir)/lddivcc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DLD -DDIVCC cinst.cc -o $@

$(stdcobjdir)/fdivcf.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DF -DDIVCF cinst.cc -o $@
$(stdcobjdir)/ddivcf.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DD -DDIVCF cinst.cc -o $@
$(stdcobjdir)/lddivcf.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DLD -DDIVCF cinst.cc -o $@

$(stdcobjdir)/fdivfc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DF -DDIVFC cinst.cc -o $@
$(stdcobjdir)/ddivfc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DD -DDIVFC cinst.cc -o $@
$(stdcobjdir)/lddivfc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DLD -DDIVFC cinst.cc -o $@

$(stdcobjdir)/fplus.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DF -DPLUS cinst.cc -o $@
$(stdcobjdir)/dplus.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DD -DPLUS cinst.cc -o $@
$(stdcobjdir)/ldplus.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DLD -DPLUS cinst.cc -o $@

$(stdcobjdir)/fminus.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DF -DMINUS cinst.cc -o $@
$(stdcobjdir)/dminus.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DD -DMINUS cinst.cc -o $@
$(stdcobjdir)/ldminus.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DLD -DMINUS cinst.cc -o $@

$(stdcobjdir)/feqcc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DF -DEQCC cinst.cc -o $@
$(stdcobjdir)/deqcc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DD -DEQCC cinst.cc -o $@
$(stdcobjdir)/ldeqcc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DLD -DEQCC cinst.cc -o $@

$(stdcobjdir)/feqcf.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DF -DEQCF cinst.cc -o $@
$(stdcobjdir)/deqcf.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DD -DEQCF cinst.cc -o $@
$(stdcobjdir)/ldeqcf.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DLD -DEQCF cinst.cc -o $@

$(stdcobjdir)/feqfc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DF -DEQFC cinst.cc -o $@
$(stdcobjdir)/deqfc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DD -DEQFC cinst.cc -o $@
$(stdcobjdir)/ldeqfc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DLD -DEQFC cinst.cc -o $@

$(stdcobjdir)/fnecc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DF -DNECC cinst.cc -o $@
$(stdcobjdir)/dnecc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DD -DNECC cinst.cc -o $@
$(stdcobjdir)/ldnecc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DLD -DNECC cinst.cc -o $@

$(stdcobjdir)/fnecf.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DF -DNECF cinst.cc -o $@
$(stdcobjdir)/dnecf.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DD -DNECF cinst.cc -o $@
$(stdcobjdir)/ldnecf.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DLD -DNECF cinst.cc -o $@

$(stdcobjdir)/fnefc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DF -DNEFC cinst.cc -o $@
$(stdcobjdir)/dnefc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DD -DNEFC cinst.cc -o $@
$(stdcobjdir)/ldnefc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DLD -DNEFC cinst.cc -o $@

$(stdcobjdir)/fpowcc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DF -DPOWCC cinst.cc -o $@
$(stdcobjdir)/dpowcc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DD -DPOWCC cinst.cc -o $@
$(stdcobjdir)/ldpowcc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DLD -DPOWCC cinst.cc -o $@

$(stdcobjdir)/fpowcf.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DF -DPOWCF cinst.cc -o $@
$(stdcobjdir)/dpowcf.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DD -DPOWCF cinst.cc -o $@
$(stdcobjdir)/ldpowcf.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DLD -DPOWCF cinst.cc -o $@

$(stdcobjdir)/fpowci.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DF -DPOWCI cinst.cc -o $@
$(stdcobjdir)/dpowci.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DD -DPOWCI cinst.cc -o $@
$(stdcobjdir)/ldpowci.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DLD -DPOWCI cinst.cc -o $@

$(stdcobjdir)/fpowfc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DF -DPOWFC cinst.cc -o $@
$(stdcobjdir)/dpowfc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DD -DPOWFC cinst.cc -o $@
$(stdcobjdir)/ldpowfc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DLD -DPOWFC cinst.cc -o $@


$(stdcobjdir)/finsert.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DF -DINSERT cinst.cc -o $@
$(stdcobjdir)/dinsert.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DD -DINSERT cinst.cc -o $@
$(stdcobjdir)/ldinsert.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DLD -DINSERT cinst.cc -o $@

$(stdcobjdir)/fextract.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DF -DEXTRACT cinst.cc -o $@
$(stdcobjdir)/dextract.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DD -DEXTRACT cinst.cc -o $@
$(stdcobjdir)/ldextract.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DLD -DEXTRACT cinst.cc -o $@

$(stdcobjdir)/fsqrt.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DF -DSQRT cinst.cc -o $@
$(stdcobjdir)/dsqrt.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DD -DSQRT cinst.cc -o $@
$(stdcobjdir)/ldsqrt.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DLD -DSQRT cinst.cc -o $@

$(stdcobjdir)/fsinh.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DF -DSINH cinst.cc -o $@
$(stdcobjdir)/dsinh.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DD -DSINH cinst.cc -o $@
$(stdcobjdir)/ldsinh.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DLD -DSINH cinst.cc -o $@

$(stdcobjdir)/fsin.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DF -DSIN cinst.cc -o $@
$(stdcobjdir)/dsin.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DD -DSIN cinst.cc -o $@
$(stdcobjdir)/ldsin.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DLD -DSIN cinst.cc -o $@

$(stdcobjdir)/flog.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DF -DLOG cinst.cc -o $@
$(stdcobjdir)/dlog.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DD -DLOG cinst.cc -o $@
$(stdcobjdir)/ldlog.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DLD -DLOG cinst.cc -o $@

$(stdcobjdir)/fexp.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DF -DEXP cinst.cc -o $@
$(stdcobjdir)/dexp.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DD -DEXP cinst.cc -o $@
$(stdcobjdir)/ldexp.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DLD -DEXP cinst.cc -o $@

$(stdcobjdir)/fcosh.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DF -DCOSH cinst.cc -o $@
$(stdcobjdir)/dcosh.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DD -DCOSH cinst.cc -o $@
$(stdcobjdir)/ldcosh.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DLD -DCOSH cinst.cc -o $@

$(stdcobjdir)/fcos.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DF -DCOS cinst.cc -o $@
$(stdcobjdir)/dcos.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DD -DCOS cinst.cc -o $@
$(stdcobjdir)/ldcos.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DLD -DCOS cinst.cc -o $@

$(stdcobjdir)/fnorm.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DF -DNORM cinst.cc -o $@
$(stdcobjdir)/dnorm.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DD -DNORM cinst.cc -o $@
$(stdcobjdir)/ldnorm.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DLD -DNORM cinst.cc -o $@

$(stdcobjdir)/fconj.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DF -DCONJ cinst.cc -o $@
$(stdcobjdir)/dconj.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DD -DCONJ cinst.cc -o $@
$(stdcobjdir)/ldconj.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DLD -DCONJ cinst.cc -o $@

$(stdcobjdir)/fpolar.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DF -DPOLAR cinst.cc -o $@
$(stdcobjdir)/dpolar.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DD -DPOLAR cinst.cc -o $@
$(stdcobjdir)/ldpolar.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DLD -DPOLAR cinst.cc -o $@

$(stdcobjdir)/farg.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DF -DARG cinst.cc -o $@
$(stdcobjdir)/darg.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DD -DARG cinst.cc -o $@
$(stdcobjdir)/ldarg.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DLD -DARG cinst.cc -o $@

$(stdcobjdir)/fabs.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DF -DABS cinst.cc -o $@
$(stdcobjdir)/dabs.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DD -DABS cinst.cc -o $@
$(stdcobjdir)/ldabs.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CC) -DLD -DABS cinst.cc -o $@


$(stdcobjdir)/cmathi.o: cmathi.cc
	$(CC) -o $@ cmathi.cc

$(stdcobjdir)/cstdlibi.o: cstdlibi.cc
	$(CC) -o $@ cstdlibi.cc

$(stdcobjdir)/cstringi.o: cstringi.cc
	$(CC) -o $@ cstringi.cc

$(stdcobjdir)/stlinst.o: stlinst.cc
	$(CC) -o $@ stlinst.cc

$(stdcobjdir)/stdexcepti.o: stdexcepti.cc
	$(CC) -o $@ stdexcepti.cc -I../gcc/cp/inc

$(stdcobjdir)/valarray.o: valarray.cc
	$(CC) -o $@ valarray.cc -I../gcc/cp/inc

