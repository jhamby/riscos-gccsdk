# Makefile for GNU Libstdc++ for the ARM/RISC OS
# Copyright (c) 1997, 1998, 1999, 2000 Nick Burrett
# Written by Nick Burrett <nick@dsvr.net>
#
# This file is part of the GNU Compiler Collection
#

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

all:	$(ux_gcc_bin_dir)/$(TRG)/libstdc++.o \
	$(ro_gcc_bin_dir)/$(TRG)/o/libstdc++

$(ux_gcc_bin_dir)/$(TRG)/libstdc++.o: $(STR_OBJS) $(COM_FOBJS) \
	$(COM_DOBJS) $(COM_LDOBJS) $(MAIN_OBJS)
	$(CROSS_AR) $(CROSS_AR_FLAGS) $@ $(STR_OBJS) $(COM_FOBJS) \
	$(COM_DOBJS) $(COM_LDOBJS) $(MAIN_OBJS)

$(ro_gcc_bin_dir)/$(TRG)/o/libstdc++: $(STR_OBJS) $(COM_FOBJS) \
	$(COM_DOBJS) $(COM_LDOBJS) $(MAIN_OBJS)
	$(CROSS_AR) $(CROSS_AR_FLAGS) $@ $(STR_OBJS) $(COM_FOBJS) \
	$(COM_DOBJS) $(COM_LDOBJS) $(MAIN_OBJS)


install_headers:
	$(insthdr) stl "*.h" stl $(ux_gccpkg)/$(gn_gplusplus_include_dir) $(ro_gccpkg)/$(gn_gplusplus_include_dir)
	for hdr in algorithm bitset deque functional hash_map hash_set \
		iterator list map memory numeric pthread_alloc queue \
		rope set slist stack utility vector; do \
		$(insthdr) stl "$$hdr" stl $(ux_gccpkg)/$(gn_gplusplus_include_dir) $(ro_gccpkg)/$(gn_gplusplus_include_dir); \
	done
	$(insthdr) std "*.h" std $(ux_gccpkg)/$(gn_gplusplus_include_dir) $(ro_gccpkg)/$(gn_gplusplus_include_dir)

	for hdr in cassert cctype cerrno cfloat ciso646 climits clocale \
		cmath complex complex.h csetjmp csignal cstdarg cstddef \
		cstdio cstdlib cstring ctime cwchar cwctype fstream iomanip \
		iosfwd iostream stdexcept string strstream valarray; do \
		$(insthdr) . "$$hdr" . $(ux_gccpkg)/$(gn_gplusplus_include_dir) $(ro_gccpkg)/$(gn_gplusplus_include_dir); \
	done

# String objects

$(stdcobjdir)/crep.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DC -DREP -c sinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/cmain.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DC -DMAIN -c sinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/ctraits.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DC -DTRAITS -c sinst.cc -o $@ $(INCLUDES)


$(stdcobjdir)/caddss.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DC -DADDSS -c sinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/caddps.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DC -DADDPS -c sinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/caddcs.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DC -DADDCS -c sinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/caddsp.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DC -DADDSP -c sinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/caddsc.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DC -DADDSC -c sinst.cc -o $@ $(INCLUDES)


$(stdcobjdir)/ceqss.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DC -DEQSS -c sinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/ceqps.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DC -DEQPS -c sinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/ceqsp.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DC -DEQSP -c sinst.cc -o $@ $(INCLUDES)


$(stdcobjdir)/cness.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DC -DNESS -c sinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/cneps.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DC -DNEPS -c sinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/cnesp.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DC -DNESP -c sinst.cc -o $@ $(INCLUDES)


$(stdcobjdir)/cextract.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DC -DEXTRACT -c sinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/cinsert.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DC -DINSERT -c sinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/cgetline.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DC -DGETLINE -c sinst.cc -o $@ $(INCLUDES)


$(stdcobjdir)/cltss.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DC -DLTSS -c sinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/cltps.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DC -DLTPS -c sinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/cltsp.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DC -DLTSP -c sinst.cc -o $@ $(INCLUDES)


$(stdcobjdir)/cgtss.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DC -DGTSS -c sinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/cgtps.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DC -DGTPS -c sinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/cgtsp.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DC -DGTSP -c sinst.cc -o $@ $(INCLUDES)


$(stdcobjdir)/cless.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DC -DLESS -c sinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/cleps.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DC -DLEPS -c sinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/clesp.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DC -DLESP -c sinst.cc -o $@ $(INCLUDES)


$(stdcobjdir)/cgess.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DC -DGESS -c sinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/cgeps.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DC -DGEPS -c sinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/cgesp.o: sinst.cc std/bastring.h std/bastring.cc std/straits.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DC -DGESP -c sinst.cc -o $@ $(INCLUDES)


#################################

# Complex objects
$(stdcobjdir)/fmain.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DF -DMAIN -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/dmain.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DD -DMAIN -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/ldmain.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DLD -DMAIN -c cinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/faddcc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DF -DADDCC -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/daddcc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DD -DADDCC -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/ldaddcc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DLD -DADDCC -c cinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/faddcf.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DF -DADDCF -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/daddcf.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DD -DADDCF -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/ldaddcf.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DLD -DADDCF -c cinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/faddfc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DF -DADDFC -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/daddfc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DD -DADDFC -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/ldaddfc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DLD -DADDFC -c cinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/fsubcc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DF -DSUBCC -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/dsubcc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DD -DSUBCC -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/ldsubcc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DLD -DSUBCC -c cinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/fsubcf.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DF -DSUBCF -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/dsubcf.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DD -DSUBCF -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/ldsubcf.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DLD -DSUBCF -c cinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/fsubfc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DF -DSUBFC -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/dsubfc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DD -DSUBFC -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/ldsubfc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DLD -DSUBFC -c cinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/fmulcc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DF -DMULCC -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/dmulcc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DD -DMULCC -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/ldmulcc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DLD -DMULCC -c cinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/fmulcf.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DF -DMULCF -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/dmulcf.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DD -DMULCF -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/ldmulcf.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DLD -DMULCF -c cinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/fmulfc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DF -DMULFC -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/dmulfc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DD -DMULFC -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/ldmulfc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DLD -DMULFC -c cinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/fdivcc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DF -DDIVCC -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/ddivcc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DD -DDIVCC -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/lddivcc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DLD -DDIVCC -c cinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/fdivcf.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DF -DDIVCF -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/ddivcf.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DD -DDIVCF -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/lddivcf.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DLD -DDIVCF -c cinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/fdivfc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DF -DDIVFC -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/ddivfc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DD -DDIVFC -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/lddivfc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DLD -DDIVFC -c cinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/fplus.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DF -DPLUS -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/dplus.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DD -DPLUS -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/ldplus.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DLD -DPLUS -c cinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/fminus.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DF -DMINUS -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/dminus.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DD -DMINUS -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/ldminus.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DLD -DMINUS -c cinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/feqcc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DF -DEQCC -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/deqcc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DD -DEQCC -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/ldeqcc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DLD -DEQCC -c cinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/feqcf.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DF -DEQCF -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/deqcf.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DD -DEQCF -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/ldeqcf.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DLD -DEQCF -c cinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/feqfc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DF -DEQFC -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/deqfc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DD -DEQFC -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/ldeqfc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DLD -DEQFC -c cinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/fnecc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DF -DNECC -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/dnecc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DD -DNECC -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/ldnecc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DLD -DNECC -c cinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/fnecf.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DF -DNECF -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/dnecf.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DD -DNECF -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/ldnecf.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DLD -DNECF -c cinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/fnefc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DF -DNEFC -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/dnefc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DD -DNEFC -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/ldnefc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DLD -DNEFC -c cinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/fpowcc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DF -DPOWCC -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/dpowcc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DD -DPOWCC -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/ldpowcc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DLD -DPOWCC -c cinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/fpowcf.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DF -DPOWCF -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/dpowcf.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DD -DPOWCF -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/ldpowcf.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DLD -DPOWCF -c cinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/fpowci.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DF -DPOWCI -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/dpowci.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DD -DPOWCI -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/ldpowci.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DLD -DPOWCI -c cinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/fpowfc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DF -DPOWFC -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/dpowfc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DD -DPOWFC -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/ldpowfc.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DLD -DPOWFC -c cinst.cc -o $@ $(INCLUDES)


$(stdcobjdir)/finsert.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DF -DINSERT -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/dinsert.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DD -DINSERT -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/ldinsert.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DLD -DINSERT -c cinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/fextract.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DF -DEXTRACT -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/dextract.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DD -DEXTRACT -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/ldextract.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DLD -DEXTRACT -c cinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/fsqrt.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DF -DSQRT -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/dsqrt.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DD -DSQRT -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/ldsqrt.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DLD -DSQRT -c cinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/fsinh.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DF -DSINH -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/dsinh.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DD -DSINH -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/ldsinh.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DLD -DSINH -c cinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/fsin.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DF -DSIN -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/dsin.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DD -DSIN -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/ldsin.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DLD -DSIN -c cinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/flog.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DF -DLOG -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/dlog.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DD -DLOG -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/ldlog.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DLD -DLOG -c cinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/fexp.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DF -DEXP -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/dexp.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DD -DEXP -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/ldexp.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DLD -DEXP -c cinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/fcosh.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DF -DCOSH -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/dcosh.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DD -DCOSH -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/ldcosh.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DLD -DCOSH -c cinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/fcos.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DF -DCOS -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/dcos.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DD -DCOS -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/ldcos.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DLD -DCOS -c cinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/fnorm.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DF -DNORM -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/dnorm.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DD -DNORM -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/ldnorm.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DLD -DNORM -c cinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/fconj.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DF -DCONJ -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/dconj.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DD -DCONJ -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/ldconj.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DLD -DCONJ -c cinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/fpolar.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DF -DPOLAR -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/dpolar.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DD -DPOLAR -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/ldpolar.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DLD -DPOLAR -c cinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/farg.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DF -DARG -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/darg.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DD -DARG -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/ldarg.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DLD -DARG -c cinst.cc -o $@ $(INCLUDES)

$(stdcobjdir)/fabs.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DF -DABS -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/dabs.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DD -DABS -c cinst.cc -o $@ $(INCLUDES)
$(stdcobjdir)/ldabs.o: cinst.cc std/complext.cc std/complext.h std/dcomplex.h std/ldcomplex.h
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -DLD -DABS -c cinst.cc -o $@ $(INCLUDES)


$(stdcobjdir)/cmathi.o: cmathi.cc
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -o $@ -c cmathi.cc $(INCLUDES)

$(stdcobjdir)/cstdlibi.o: cstdlibi.cc
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -o $@ -c cstdlibi.cc $(INCLUDES)

$(stdcobjdir)/cstringi.o: cstringi.cc
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -o $@ -c cstringi.cc $(INCLUDES)

$(stdcobjdir)/stlinst.o: stlinst.cc
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -o $@ -c stlinst.cc $(INCLUDES)

$(stdcobjdir)/stdexcepti.o: stdexcepti.cc
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -o $@ -c stdexcepti.cc $(INCLUDES) -I../gcc/cp/inc

$(stdcobjdir)/valarray.o: valarray.cc
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -o $@ -c valarray.cc $(INCLUDES) -I../gcc/cp/inc
