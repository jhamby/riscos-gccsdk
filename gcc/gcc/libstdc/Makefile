# Makefile for GNU Libstdc++ V3 for the ARM/RISC OS GCC port
# Copyright (c) 2002 Nick Burrett
# Written by Nick Burrett <nick@dsvr.net>
#

INCLUDES = -I./include -I./include/bits -I./include/arm-riscos-aof -I./include/backward -I../libsupc++
libobjdir=$(objdir)/gcc-$(VERSION)/libstdc/$(STAGE)/$(TRG)

CROSS_CXXFLAGS += -D_GNU_SOURCE -fno-implicit-templates -Wall -Wno-format -W -Wwrite-strings -Winline -fdiagnostics-show-location=once

$(libobjdir)/%.o: %.cc
	$(CROSS_CXX) -nostdinc++ $(CROSS_CXXFLAGS) -o $@ -c $< $(INCLUDES) $(CFL)

ifeq (${STAGE},stage1)
bin_dir = $(ux_bin_dir)
gcc_bin_dir = $(ux_gcc_bin_dir)
else
bin_dir = $(ro_bin_dir)
gcc_bin_dir = $(ro_gcc_bin_dir)
CC = $(CROSS_CC)
CFLAGS = $(CROSS_CFLAGS)
INCLUDES += -I$(unixlib_incl_dir)
endif

OS_INC_SRCDIR = config/os/generic
ATOMICITY_INC_SRCDIR = config/cpu/generic
CPU_LIMITS_INC_SRCDIR = config/cpu/generic
target_alias = config/io

stdcobjdir=$(objdir)/gcc-$(VERSION)/libstdc++/$(STAGE)

glibcpp_srcdir = /home/nick/gcc-3.1/libstdc++-v3
glibcpp_builddir = @glibcpp_builddir@

bits_srcdir = ${glibcpp_srcdir}/include/bits
bits_builddir = ./bits
bits_headers = \
	${bits_srcdir}/basic_ios.h \
	${bits_srcdir}/basic_ios.tcc \
	${bits_srcdir}/basic_string.h \
	${bits_srcdir}/basic_string.tcc \
	${bits_srcdir}/boost_concept_check.h \
	${bits_srcdir}/char_traits.h \
	${bits_srcdir}/codecvt.h \
	${bits_srcdir}/concept_check.h \
	${bits_srcdir}/cpp_type_traits.h \
	${bits_srcdir}/deque.tcc \
	${bits_srcdir}/fpos.h \
	${bits_srcdir}/fstream.tcc \
	${bits_srcdir}/functexcept.h \
	${bits_srcdir}/generic_shadow.h \
	${bits_srcdir}/gslice.h \
	${bits_srcdir}/gslice_array.h \
	${bits_srcdir}/indirect_array.h \
	${bits_srcdir}/ios_base.h \
	${bits_srcdir}/istream.tcc \
	${bits_srcdir}/list.tcc \
	${bits_srcdir}/locale_facets.h \
	${bits_srcdir}/locale_facets.tcc \
	${bits_srcdir}/localefwd.h \
	${bits_srcdir}/mask_array.h \
	${bits_srcdir}/ostream.tcc \
	${bits_srcdir}/pthread_allocimpl.h \
	${bits_srcdir}/stream_iterator.h \
	${bits_srcdir}/streambuf_iterator.h \
	${bits_srcdir}/slice_array.h \
	${bits_srcdir}/sstream.tcc \
	${bits_srcdir}/stl_algo.h \
	${bits_srcdir}/stl_algobase.h \
	${bits_srcdir}/stl_alloc.h \
	${bits_srcdir}/stl_bvector.h \
	${bits_srcdir}/stl_construct.h \
	${bits_srcdir}/stl_deque.h \
	${bits_srcdir}/stl_function.h \
	${bits_srcdir}/stl_heap.h \
	${bits_srcdir}/stl_iterator.h \
	${bits_srcdir}/stl_iterator_base_funcs.h \
	${bits_srcdir}/stl_iterator_base_types.h \
	${bits_srcdir}/stl_list.h \
	${bits_srcdir}/stl_map.h \
	${bits_srcdir}/stl_multimap.h \
	${bits_srcdir}/stl_multiset.h \
	${bits_srcdir}/stl_numeric.h \
	${bits_srcdir}/stl_pair.h \
	${bits_srcdir}/stl_pthread_alloc.h \
	${bits_srcdir}/stl_queue.h \
	${bits_srcdir}/stl_raw_storage_iter.h \
	${bits_srcdir}/stl_relops.h \
	${bits_srcdir}/stl_set.h \
	${bits_srcdir}/stl_stack.h \
	${bits_srcdir}/stl_tempbuf.h \
	${bits_srcdir}/stl_threads.h \
	${bits_srcdir}/stl_tree.h \
	${bits_srcdir}/stl_uninitialized.h \
	${bits_srcdir}/stl_vector.h \
	${bits_srcdir}/streambuf.tcc \
	${bits_srcdir}/stringfwd.h \
	${bits_srcdir}/type_traits.h \
	${bits_srcdir}/valarray_array.h \
	${bits_srcdir}/valarray_array.tcc \
	${bits_srcdir}/valarray_meta.h \
	${bits_srcdir}/vector.tcc


backward_srcdir = ${glibcpp_srcdir}/include/backward
backward_builddir = ./backward
backward_headers = \
	${backward_srcdir}/complex.h \
	${backward_srcdir}/iomanip.h \
	${backward_srcdir}/istream.h \
	${backward_srcdir}/ostream.h \
	${backward_srcdir}/stream.h \
	${backward_srcdir}/streambuf.h \
	${backward_srcdir}/algo.h \
	${backward_srcdir}/algobase.h \
	${backward_srcdir}/alloc.h \
	${backward_srcdir}/bvector.h \
	${backward_srcdir}/defalloc.h \
	${backward_srcdir}/deque.h \
	${backward_srcdir}/function.h \
	${backward_srcdir}/hash_map.h \
	${backward_srcdir}/hash_set.h \
	${backward_srcdir}/hashtable.h \
	${backward_srcdir}/heap.h \
	${backward_srcdir}/iostream.h \
	${backward_srcdir}/iterator.h \
	${backward_srcdir}/list.h \
	${backward_srcdir}/map.h \
	${backward_srcdir}/multimap.h \
	${backward_srcdir}/new.h \
	${backward_srcdir}/multiset.h \
	${backward_srcdir}/pair.h \
	${backward_srcdir}/queue.h \
	${backward_srcdir}/rope.h \
	${backward_srcdir}/set.h \
	${backward_srcdir}/slist.h \
	${backward_srcdir}/stack.h \
	${backward_srcdir}/tempbuf.h \
	${backward_srcdir}/tree.h \
	${backward_srcdir}/vector.h \
	${backward_srcdir}/fstream.h \
	${backward_srcdir}/strstream \
	${backward_srcdir}/backward_warning.h


ext_srcdir = ${glibcpp_srcdir}/include/ext
ext_builddir = ./ext
ext_headers = \
	${ext_srcdir}/algorithm \
	${ext_srcdir}/enc_filebuf.h \
	${ext_srcdir}/stdio_filebuf.h \
	${ext_srcdir}/functional \
	${ext_srcdir}/hash_map \
	${ext_srcdir}/hash_set \
	${ext_srcdir}/iterator \
	${ext_srcdir}/memory \
	${ext_srcdir}/numeric \
	${ext_srcdir}/rb_tree \
	${ext_srcdir}/rope \
	${ext_srcdir}/ropeimpl.h \
	${ext_srcdir}/slist \
	${ext_srcdir}/stl_hash_fun.h \
	${ext_srcdir}/stl_hashtable.h \
	${ext_srcdir}/stl_rope.h


# This is the common subset of files that all three "C" header models use.
c_base_srcdir = $(glibcpp_srcdir)/include/c_std
c_base_builddir = .
c_base_headers = \
	${c_base_srcdir}/std_cassert.h \
	${c_base_srcdir}/std_cctype.h \
	${c_base_srcdir}/std_cerrno.h \
	${c_base_srcdir}/std_cfloat.h \
	${c_base_srcdir}/std_ciso646.h \
	${c_base_srcdir}/std_climits.h \
	${c_base_srcdir}/std_clocale.h \
	${c_base_srcdir}/std_cmath.h \
	${c_base_srcdir}/std_csetjmp.h \
	${c_base_srcdir}/std_csignal.h \
	${c_base_srcdir}/std_cstdarg.h \
	${c_base_srcdir}/std_cstddef.h \
	${c_base_srcdir}/std_cstdio.h \
	${c_base_srcdir}/std_cstdlib.h \
	${c_base_srcdir}/std_cstring.h \
	${c_base_srcdir}/std_ctime.h \
	${c_base_srcdir}/std_cwchar.h \
	${c_base_srcdir}/std_cwctype.h 

c_base_headers_rename = \
	cassert \
	cctype \
	cerrno \
	cfloat \
	ciso646 \
	climits \
	clocale \
	cmath \
	csetjmp \
	csignal \
	cstdarg \
	cstddef \
	cstdio \
	cstdlib \
	cstring \
	ctime \
	cwchar \
	cwctype 


# "C" compatibility headers.
c_compatibility_srcdir = ${glibcpp_srcdir}/include/c_compatibility
c_compatibility_builddir = .
c_compatibility_headers = \
	${c_compatibility_srcdir}/assert.h \
	${c_compatibility_srcdir}/ctype.h \
	${c_compatibility_srcdir}/errno.h \
	${c_compatibility_srcdir}/float.h \
	${c_compatibility_srcdir}/iso646.h \
	${c_compatibility_srcdir}/limits.h \
	${c_compatibility_srcdir}/locale.h \
	${c_compatibility_srcdir}/math.h \
	${c_compatibility_srcdir}/setjmp.h \
	${c_compatibility_srcdir}/signal.h \
	${c_compatibility_srcdir}/stdarg.h \
	${c_compatibility_srcdir}/stddef.h \
	${c_compatibility_srcdir}/stdio.h \
	${c_compatibility_srcdir}/stdlib.h \
	${c_compatibility_srcdir}/string.h \
	${c_compatibility_srcdir}/time.h \
	${c_compatibility_srcdir}/wchar.h \
	${c_compatibility_srcdir}/wctype.h 

c_base_headers_extra = ${c_base_srcdir}/cmath.tcc 
@GLIBCPP_C_HEADERS_COMPATIBILITY_TRUE@c_compatibility_headers_extra = @GLIBCPP_C_HEADERS_COMPATIBILITY_TRUE@${c_compatibility_headers}
@GLIBCPP_C_HEADERS_COMPATIBILITY_FALSE@c_compatibility_headers_extra = 

std_srcdir = ${glibcpp_srcdir}/include/std
std_builddir = .
std_headers = \
	${std_srcdir}/std_algorithm.h \
	${std_srcdir}/std_bitset.h \
	${std_srcdir}/std_complex.h \
	${std_srcdir}/std_deque.h \
	${std_srcdir}/std_fstream.h \
	${std_srcdir}/std_functional.h \
	${std_srcdir}/std_iomanip.h \
	${std_srcdir}/std_ios.h \
	${std_srcdir}/std_iosfwd.h \
	${std_srcdir}/std_iostream.h \
	${std_srcdir}/std_istream.h \
	${std_srcdir}/std_iterator.h \
	${std_srcdir}/std_limits.h \
	${std_srcdir}/std_list.h \
	${std_srcdir}/std_locale.h \
	${std_srcdir}/std_map.h \
	${std_srcdir}/std_memory.h \
	${std_srcdir}/std_numeric.h \
	${std_srcdir}/std_ostream.h \
	${std_srcdir}/std_queue.h \
	${std_srcdir}/std_set.h \
	${std_srcdir}/std_sstream.h \
	${std_srcdir}/std_stack.h \
	${std_srcdir}/std_stdexcept.h \
	${std_srcdir}/std_streambuf.h \
	${std_srcdir}/std_string.h \
	${std_srcdir}/std_utility.h \
	${std_srcdir}/std_valarray.h \
	${std_srcdir}/std_vector.h

# Renamed at build time. 
std_headers_rename = \
	algorithm \
	bitset \
	complex \
	deque \
	fstream \
	functional \
	iomanip \
	ios \
	iosfwd \
	iostream \
	istream \
	iterator \
	limits \
	list \
	locale \
	map \
	memory \
	numeric \
	ostream \
	queue \
	set \
	sstream \
	stack \
	stdexcept \
	streambuf \
	string \
	utility \
	valarray \
	vector


target_srcdir = ${glibcpp_srcdir}/$(OS_INC_SRCDIR)
target_builddir = ./include/arm-riscos-aof/bits
target_headers = \
	${target_srcdir}/ctype_base.h \
	${target_srcdir}/ctype_inline.h \
	${target_srcdir}/ctype_noninline.h \
	${target_srcdir}/os_defines.h \
	${glibcpp_srcdir}/$(ATOMICITY_INC_SRCDIR)/atomicity.h \
	${glibcpp_srcdir}/$(CPU_LIMITS_INC_SRCDIR)/cpu_limits.h 

# These target_headers_extra files are all built with ad hoc naming rules.
target_headers_extra = \
	${target_builddir}/basic_file.h \
	${target_builddir}/c++config.h \
	${target_builddir}/c++io.h \
	${target_builddir}/c++locale.h \
	${target_builddir}/messages_members.h \
	${target_builddir}/codecvt_specializations.h


thread_target_headers = \
	${target_builddir}/gthr.h \
	${target_builddir}/gthr-single.h \
	${target_builddir}/gthr-posix.h \
	${target_builddir}/gthr-default.h

sources = \
        basic_file.cc \
        bitset.cc \
        c++locale.cc \
        codecvt.cc \
        collate.cc \
        complex_io.cc \
        concept-inst.cc \
        ctype.cc \
        ext-inst.cc \
        fstream.cc \
        fstream-inst.cc \
        functexcept.cc \
        globals.cc \
        io-inst.cc \
        ios.cc \
        istream-inst.cc \
        limits.cc \
        locale-inst.cc \
        locale.cc \
        localename.cc \
        messages.cc \
        misc-inst.cc \
        monetary.cc \
        numeric.cc \
        ostream-inst.cc \
        sstream-inst.cc \
        stdexcept.cc \
        stl-inst.cc \
        streambuf-inst.cc \
        string-inst.cc \
        strstream.cc \
        time.cc \
        valarray-inst.cc \
        vterminate.cc \
        wstring-inst.cc

OBJS =	$(libobjdir)/basic_file.o \
	$(libobjdir)/bitset.o \
        $(libobjdir)/c++locale.o \
        $(libobjdir)/codecvt.o \
        $(libobjdir)/collate.o \
        $(libobjdir)/complex_io.o \
        $(libobjdir)/concept-inst.o \
        $(libobjdir)/ctype.o \
        $(libobjdir)/ext-inst.o \
        $(libobjdir)/fstream.o \
        $(libobjdir)/fstream-inst.o \
        $(libobjdir)/functexcept.o \
        $(libobjdir)/globals.o \
        $(libobjdir)/io-inst.o \
        $(libobjdir)/ios.o \
        $(libobjdir)/istream-inst.o \
        $(libobjdir)/limits.o \
        $(libobjdir)/locale.o \
        $(libobjdir)/locale-inst.o \
        $(libobjdir)/localename.o \
        $(libobjdir)/messages.o \
        $(libobjdir)/misc-inst.o \
        $(libobjdir)/monetary.o \
        $(libobjdir)/numeric.o \
        $(libobjdir)/ostream-inst.o \
        $(libobjdir)/sstream-inst.o \
        $(libobjdir)/stdexcept.o \
        $(libobjdir)/stl-inst.o \
        $(libobjdir)/streambuf-inst.o \
        $(libobjdir)/string-inst.o \
        $(libobjdir)/strstream.o \
        $(libobjdir)/time.o \
        $(libobjdir)/valarray-inst.o \
        $(libobjdir)/vterminate.o \
        $(libobjdir)/wstring-inst.o


all:    $(ux_gcc_bin_dir)/$(TRG)/libstdc++.o \
        $(ro_gcc_bin_dir)/$(TRG)/o/libstdc++

install_headers:
	$(insthdr) include "*" . $(ux_gccpkg)/$(gn_gplusplus_include_dir) $(ro_gccpkg)/$(gn_gplusplus_include_dir)
	#mkdir -p $(ux_gccpkg)/$(gn_gplusplus_include_dir)/arm-riscos-aof
	#mkdir -p $(ro_gccpkg)/$(gn_gplusplus_include_dir)/arm-riscos-aof
	$(insthdr) include/arm-riscos-aof/bits "*" bits $(ux_gccpkg)/$(gn_gplusplus_include_dir) $(ro_gccpkg)/$(gn_gplusplus_include_dir)
	$(insthdr) include/backward "*" backward $(ux_gccpkg)/$(gn_gplusplus_include_dir) $(ro_gccpkg)/$(gn_gplusplus_include_dir)
	$(insthdr) include/bits "*" bits $(ux_gccpkg)/$(gn_gplusplus_include_dir) $(ro_gccpkg)/$(gn_gplusplus_include_dir)
	$(insthdr) include/ext "*" ext $(ux_gccpkg)/$(gn_gplusplus_include_dir) $(ro_gccpkg)/$(gn_gplusplus_include_dir)


$(ux_gcc_bin_dir)/$(TRG)/libstdc++.o: $(OBJS)
	$(CROSS_AR) $(CROSS_AR_FLAGS) $@ $(OBJS)

$(ro_gcc_bin_dir)/$(TRG)/o/libstdc++: $(OBJS)
	$(CROSS_AR) $(CROSS_AR_FLAGS) $@ $(OBJS)


import:
	# Merge header files
	mkdir -p ./include/bits
	mkdir -p ./include/backward
	mkdir -p ./include/ext
	mkdir -p ./include/arm-riscos-aof/bits
	for x in $(bits_headers); do cp -a $$x ./include/bits; done
	for x in $(backward_headers); do cp -a $$x ./include/backward; done
	for x in $(ext_headers); do cp -a $$x ./include/ext; done
	for x in $(c_base_headers); do cp -a $$x ./include; done
	for x in $(c_base_headers_extra); do cp -a $$x ./include/bits; done
	#for x in $(c_compatibility_headers); do cp -a $$x ./include; done
	for x in $(std_headers); do cp -a $$x ./include; done
	for x in $(target_headers); do cp -a $$x $(target_builddir); done
	cp -a $(glibcpp_srcdir)/config/io/basic_file_stdio.h $(target_builddir)/basic_file.h
	cp -a $(glibcpp_srcdir)/config/io/c_io_stdio.h $(target_builddir)/c++io.h
	cp -a $(glibcpp_srcdir)/config/locale/generic/c_locale.h $(target_builddir)/c++locale.h
	cp -a $(glibcpp_srcdir)/config/locale/ieee_1003.1-2001/codecvt_specializations.h $(target_builddir)/codecvt_specializations.h
	cp -a $(glibcpp_srcdir)/config/locale/generic/messages_members.h $(target_builddir)/messages_members.h
	cp -a ../gcc/gthr*.h $(target_builddir)
	for x in $(std_headers_rename); do mv ./include/std_$$x.h ./include/$$x; done
	for x in $(c_base_headers_rename); do mv ./include/std_$$x.h ./include/$$x; done
	# Merge src directory
	cp -a $(glibcpp_srcdir)/src/*.cc ./
	cp -a $(glibcpp_srcdir)/config/io/basic_file_stdio.cc ./basic_file.cc
	cp -a $(glibcpp_srcdir)/config/locale/generic/c_locale.cc ./c++locale.cc
	cp -a $(glibcpp_srcdir)/config/locale/generic/collate_members.cc ./collate.cc
	cp -a $(glibcpp_srcdir)/config/locale/generic/ctype_members.cc ./ctype.cc
	cp -a $(glibcpp_srcdir)/config/locale/generic/messages_members.cc ./messages.cc
	cp -a $(glibcpp_srcdir)/config/locale/generic/monetary_members.cc ./monetary.cc
	cp -a $(glibcpp_srcdir)/config/locale/generic/numeric_members.cc ./numeric.cc
	cp -a $(glibcpp_srcdir)/config/locale/generic/time_members.cc ./time.cc
	# Merge libio directory
	#cp -a $(glibcpp_srcdir)/libio/*.c ./
	#cp -a $(glibcpp_srcdir)/libio/*.h ./
	# Merge libsupc directory
	#cp -a $(glibcpp_srcdir)/libsupc++/*.cc ./
	#cp -a $(glibcpp_srcdir)/libsupc++/*.h ./

$(libobjdir)/bitset.o: bitset.cc
$(libobjdir)/c++locate.o: c++locate.cc
$(libobjdir)/codecvt.o: codecvt.cc
$(libobjdir)/collate.o: collate.cc
$(libobjdir)/complex_io.o: complex_io.cc
$(libobjdir)/concept-inst.o: concept-inst.cc
$(libobjdir)/ctype.o: ctype.cc
$(libobjdir)/ext-inst.o: ext-inst.cc
$(libobjdir)/fstream.o: fstream.cc
$(libobjdir)/fstream-inst.o: fstream-inst.cc
$(libobjdir)/functexcept.o: functexcept.cc
$(libobjdir)/globals.o: globals.cc
$(libobjdir)/io-inst.o: io-inst.cc
$(libobjdir)/ios.o: ios.cc
$(libobjdir)/istream-inst.o: istream-inst.cc
$(libobjdir)/limits.o: limits.cc
$(libobjdir)/locale-inst.o: locale-inst.cc
$(libobjdir)/locale.o: locale.cc
$(libobjdir)/localename.o: localename.cc
$(libobjdir)/messages.o: messages.cc
$(libobjdir)/misc-inst.o: misc-inst.cc
$(libobjdir)/monetary.o: monetary.cc
$(libobjdir)/numeric.o: numeric.cc
$(libobjdir)/ostream-inst.o: ostream-inst.cc
$(libobjdir)/sstream-inst.o: sstream-inst.cc
$(libobjdir)/stdexcept.o: stdexcept.cc
$(libobjdir)/stl-inst.o: stl-inst.cc
$(libobjdir)/streambuf-inst.o: streambuf-inst.cc
$(libobjdir)/string-inst.o: string-inst.cc
$(libobjdir)/strstream.o: strstream.cc
$(libobjdir)/time.o: time.cc
$(libobjdir)/valarray-inst.o: valarray-inst.cc
$(libobjdir)/vterminate.o: vterminate.cc
$(libobjdir)/wstring-inst.o: wstring-inst.cc

