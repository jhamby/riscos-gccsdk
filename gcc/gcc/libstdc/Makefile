# Makefile for GNU Libstdc++ V3 for the ARM/RISC OS GCC port
# Copyright (c) 2002, 2003, 2004 Nick Burrett
# Copyright (c) 2004, 2005 GCCSDK Developers
#

INCLUDES = -I ./include -I ./include/bits -I ./include/arm-riscos-aof -I ./include/backward -I ./libsupc
SUPCINCLUDES = -I ./libsupc -I ../gcc -I ./include -I ./include/arm-riscos-aof

libobjdir=$(objdir)/gcc-$(VERSION)/libstdc/$(STAGE)/$(TRG)
supcobjdir=$(objdir)/gcc-$(VERSION)/libstdc/$(STAGE)/$(TRG)

CROSS_CXXFLAGS += -D_GNU_SOURCE -fno-implicit-templates -Wall -Wno-format -W -Wwrite-strings -Winline -fdiagnostics-show-location=once -D_GLIBCXX_SJLJ_EXCEPTIONS -DDONT_USE_BUILTIN_SETJMP

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
INCLUDES += -I$(lib_incl_dir)
endif

OS_INC_SRCDIR = config/os/generic
ATOMICITY_INC_SRCDIR = config/cpu/generic
target_alias = config/io

stdcobjdir=$(objdir)/gcc-$(VERSION)/libstdc++/$(STAGE)

glibcpp_srcdir = /home/nick/riscos/masters/gcc-3.4/libstdc++-v3
glibcpp_builddir = @glibcpp_builddir@

bits_srcdir = ${glibcpp_srcdir}/include/bits
bits_builddir = ./bits
bits_headers = \
	${bits_srcdir}/allocator.h \
	${bits_srcdir}/atomicity.h \
	${bits_srcdir}/basic_ios.h \
	${bits_srcdir}/basic_ios.tcc \
	${bits_srcdir}/basic_string.h \
	${bits_srcdir}/basic_string.tcc \
	${bits_srcdir}/boost_concept_check.h \
	${bits_srcdir}/char_traits.h \
	${bits_srcdir}/codecvt.h \
	${bits_srcdir}/concept_check.h \
	${bits_srcdir}/concurrence.h \
	${bits_srcdir}/cpp_type_traits.h \
	${bits_srcdir}/deque.tcc \
	${bits_srcdir}/fstream.tcc \
	${bits_srcdir}/functexcept.h \
	${bits_srcdir}/gslice.h \
	${bits_srcdir}/gslice_array.h \
	${bits_srcdir}/indirect_array.h \
	${bits_srcdir}/ios_base.h \
	${bits_srcdir}/istream.tcc \
	${bits_srcdir}/list.tcc \
	${bits_srcdir}/locale_classes.h \
	${bits_srcdir}/locale_facets.h \
	${bits_srcdir}/locale_facets.tcc \
	${bits_srcdir}/localefwd.h \
	${bits_srcdir}/mask_array.h \
	${bits_srcdir}/ostream.tcc \
	${bits_srcdir}/postypes.h \
	${bits_srcdir}/stream_iterator.h \
	${bits_srcdir}/streambuf_iterator.h \
	${bits_srcdir}/slice_array.h \
	${bits_srcdir}/sstream.tcc \
	${bits_srcdir}/stl_algo.h \
	${bits_srcdir}/stl_algobase.h \
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
	${bits_srcdir}/valarray_before.h \
	${bits_srcdir}/valarray_after.h \
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
	${ext_srcdir}/bitmap_allocator.h \
	${ext_srcdir}/debug_allocator.h \
	${ext_srcdir}/enc_filebuf.h \
	${ext_srcdir}/stdio_filebuf.h \
	${ext_srcdir}/stdio_sync_filebuf.h \
	${ext_srcdir}/functional \
	${ext_srcdir}/hash_map \
	${ext_srcdir}/hash_set \
	${ext_srcdir}/iterator \
	${ext_srcdir}/malloc_allocator.h \
	${ext_srcdir}/memory \
	${ext_srcdir}/mt_allocator.h \
	${ext_srcdir}/new_allocator.h \
	${ext_srcdir}/numeric \
	${ext_srcdir}/pod_char_traits.h \
	${ext_srcdir}/pool_allocator.h \
	${ext_srcdir}/rb_tree \
	${ext_srcdir}/rope \
	${ext_srcdir}/ropeimpl.h \
	${ext_srcdir}/slist \
	${ext_srcdir}/hash_fun.h \
	${ext_srcdir}/hashtable.h


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

# Debug mode headers
debug_srcdir = ${glibcpp_srcdir}/include/debug
debug_builddir = ./debug
debug_headers = \
	${debug_srcdir}/bitset \
	${debug_srcdir}/debug.h \
	${debug_srcdir}/deque \
	${debug_srcdir}/formatter.h \
	${debug_srcdir}/hash_map \
	${debug_srcdir}/hash_map.h \
	${debug_srcdir}/hash_multimap.h \
        ${debug_srcdir}/hash_multiset.h \
        ${debug_srcdir}/hash_set \
        ${debug_srcdir}/hash_set.h \
        ${debug_srcdir}/list \
        ${debug_srcdir}/map \
        ${debug_srcdir}/map.h \
        ${debug_srcdir}/multimap.h \
        ${debug_srcdir}/multiset.h \
        ${debug_srcdir}/safe_base.h \
        ${debug_srcdir}/safe_iterator.h \
        ${debug_srcdir}/safe_iterator.tcc \
        ${debug_srcdir}/safe_sequence.h \
        ${debug_srcdir}/set \
        ${debug_srcdir}/set.h \
        ${debug_srcdir}/string \
        ${debug_srcdir}/vector


c_base_headers_extra = ${c_base_srcdir}/cmath.tcc 

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
	${glibcpp_srcdir}/$(ATOMICITY_INC_SRCDIR)/atomicity.h

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



# Source files linked in via configuration/make substitution for a
# particular host.
host_sources = \
	atomicity.cc \
	codecvt_members.cc \
	collate_members.cc \
	ctype_members.cc \
	messages_members.cc \
	monetary_members.cc \
	numeric_members.cc \
	time_members.cc 

# Source files linked in via configuration/make substitution for a
# particular host, but with ad hoc naming rules.
host_sources_extra = \
	basic_file.cc \
	c++locale.cc

host_objs = \
	$(libobjdir)/atomicity.o \
	$(libobjdir)/codecvt_members.o \
	$(libobjdir)/collate.o \
	$(libobjdir)/ctype_members.o \
	$(libobjdir)/messages.o \
	$(libobjdir)/monetary.o \
	$(libobjdir)/numeric.o \
	$(libobjdir)/time.o 

host_objs_extra = \
	$(libobjdir)/basic_file.o \
	$(libobjdir)/c++locale.o

sources = \
	allocator.cc \
	codecvt.cc \
	complex_io.cc \
	ctype.cc \
	debug.cc \
	debug_list.cc \
	functexcept.cc \
	globals_locale.cc \
	globals_io.cc \
	ios.cc \
	ios_failure.cc \
	ios_init.cc \
	ios_locale.cc \
	limits.cc \
	list.cc \
	locale.cc \
	locale_init.cc \
	locale_facets.cc \
	localename.cc \
	stdexcept.cc \
	strstream.cc \
	tree.cc \
	allocator-inst.cc \
	concept-inst.cc \
	fstream-inst.cc \
	ext-inst.cc \
	io-inst.cc \
	istream-inst.cc \
	locale-inst.cc \
	locale-misc-inst.cc \
	misc-inst.cc \
	ostream-inst.cc \
	sstream-inst.cc \
	streambuf-inst.cc \
	string-inst.cc \
	valarray-inst.cc \
	wlocale-inst.cc \
	wstring-inst.cc \
	${host_sources} \
	${host_sources_extra}

OBJS =	 \
	$(libobjdir)/allocator.o \
	$(libobjdir)/codecvt.o \
	$(libobjdir)/complex_io.o \
	$(libobjdir)/ctype.o \
	$(libobjdir)/debug.o \
	$(libobjdir)/debug_list.o \
	$(libobjdir)/functexcept.o \
	$(libobjdir)/globals_locale.o \
	$(libobjdir)/globals_io.o \
	$(libobjdir)/ios.o \
	$(libobjdir)/ios_failure.o \
	$(libobjdir)/ios_init.o \
	$(libobjdir)/ios_locale.o \
	$(libobjdir)/limits.o \
	$(libobjdir)/list.o \
	$(libobjdir)/locale.o \
	$(libobjdir)/locale_init.o \
	$(libobjdir)/locale_facets.o \
	$(libobjdir)/localename.o \
	$(libobjdir)/stdexcept.o \
	$(libobjdir)/strstream.o \
	$(libobjdir)/tree.o \
	$(libobjdir)/allocator-inst.o \
	$(libobjdir)/concept-inst.o \
	$(libobjdir)/fstream-inst.o \
	$(libobjdir)/ext-inst.o \
	$(libobjdir)/io-inst.o \
	$(libobjdir)/istream-inst.o \
	$(libobjdir)/locale-inst.o \
	$(libobjdir)/locale-misc-inst.o \
	$(libobjdir)/misc-inst.o \
	$(libobjdir)/ostream-inst.o \
	$(libobjdir)/sstream-inst.o \
	$(libobjdir)/streambuf-inst.o \
	$(libobjdir)/string-inst.o \
	$(libobjdir)/valarray-inst.o \
	$(libobjdir)/wlocale-inst.o \
	$(libobjdir)/wstring-inst.o \
	${host_objs} \
	${host_objs_extra}

SUPCOBJS =	$(supcobjdir)/del_op.o \
	$(supcobjdir)/del_opnt.o \
	$(supcobjdir)/del_opv.o \
	$(supcobjdir)/del_opvnt.o \
	$(supcobjdir)/eh_alloc.o \
	$(supcobjdir)/eh_aux_runtime.o \
	$(supcobjdir)/eh_catch.o \
	$(supcobjdir)/eh_exception.o \
	$(supcobjdir)/eh_globals.o \
	$(supcobjdir)/eh_personality.o \
	$(supcobjdir)/eh_term_handler.o \
	$(supcobjdir)/eh_terminate.o \
	$(supcobjdir)/eh_throw.o \
	$(supcobjdir)/eh_type.o \
	$(supcobjdir)/eh_unex_handler.o \
	$(supcobjdir)/guard.o \
	$(supcobjdir)/new_handler.o \
	$(supcobjdir)/new_op.o \
	$(supcobjdir)/new_opnt.o \
	$(supcobjdir)/new_opv.o \
	$(supcobjdir)/new_opvnt.o \
	$(supcobjdir)/pure.o \
	$(supcobjdir)/tinfo2.o \
	$(supcobjdir)/tinfo.o \
	$(supcobjdir)/vec.o \
	$(supcobjdir)/vterminate.o \
	$(supcobjdir)/cp-demangle.o


all:    $(ux_gcc_bin_dir)/$(TRG)/libstdc++.o \
        $(ro_gcc_bin_dir)/$(TRG)/o/libstdc++

install_headers:
	$(insthdr) include "*" . $(ux_gccpkg)/$(gn_gplusplus_include_dir) $(ro_gccpkg)/$(gn_gplusplus_include_dir)
	$(insthdr) include/arm-riscos-aof/bits "*" bits $(ux_gccpkg)/$(gn_gplusplus_include_dir) $(ro_gccpkg)/$(gn_gplusplus_include_dir)
	$(insthdr) include/backward "*" backward $(ux_gccpkg)/$(gn_gplusplus_include_dir) $(ro_gccpkg)/$(gn_gplusplus_include_dir)
	$(insthdr) include/bits "*" bits $(ux_gccpkg)/$(gn_gplusplus_include_dir) $(ro_gccpkg)/$(gn_gplusplus_include_dir)
	$(insthdr) include/debug "*" debug $(ux_gccpkg)/$(gn_gplusplus_include_dir) $(ro_gccpkg)/$(gn_gplusplus_include_dir)
	$(insthdr) include/ext "*" ext $(ux_gccpkg)/$(gn_gplusplus_include_dir) $(ro_gccpkg)/$(gn_gplusplus_include_dir)
	$(insthdr) include/std "*" std $(ux_gccpkg)/$(gn_gplusplus_include_dir) $(ro_gccpkg)/$(gn_gplusplus_include_dir)
	$(insthdr) libsupc "*.h" . $(ux_gccpkg)/$(gn_gplusplus_include_dir) $(ro_gccpkg)/$(gn_gplusplus_include_dir)
	$(insthdr) libsupc exception . $(ux_gccpkg)/$(gn_gplusplus_include_dir) $(ro_gccpkg)/$(gn_gplusplus_include_dir)
	$(insthdr) libsupc new . $(ux_gccpkg)/$(gn_gplusplus_include_dir) $(ro_gccpkg)/$(gn_gplusplus_include_dir)
	$(insthdr) libsupc typeinfo . $(ux_gccpkg)/$(gn_gplusplus_include_dir) $(ro_gccpkg)/$(gn_gplusplus_include_dir)

$(ux_gcc_bin_dir)/$(TRG)/libstdc++.o: $(OBJS) $(SUPCOBJS)
	$(CROSS_AR) $(CROSS_AR_FLAGS) $@ $(OBJS) $(SUPCOBJS)

$(ro_gcc_bin_dir)/$(TRG)/o/libstdc++: $(OBJS) $(SUPCOBJS)
	$(CROSS_AR) $(CROSS_AR_FLAGS) $@ $(OBJS) $(SUPCOBJS)


import:
	# Merge header files
	mkdir -p ./include/bits
	mkdir -p ./include/backward
	mkdir -p ./include/ext
	mkdir -p ./include/arm-riscos-aof/bits
	for x in $(bits_headers); do cp -pR $$x ./include/bits; done
	for x in $(backward_headers); do cp -pR $$x ./include/backward; done
	for x in $(ext_headers); do cp -pR $$x ./include/ext; done
	for x in $(debug_headers); do cp -pR $$x ./include/debug; done
	for x in $(c_base_headers); do cp -pR $$x ./include; done
	for x in $(c_base_headers_extra); do cp -pR $$x ./include/bits; done
	cp -pR $(glibcpp_srcdir)/config/locale/generic/time_members.h ./include/bits
	#for x in $(c_compatibility_headers); do cp -pR $$x ./include; done
	for x in $(std_headers); do cp -pR $$x ./include; done
	for x in $(target_headers); do cp -pR $$x $(target_builddir); done
	cp -pR $(glibcpp_srcdir)/config/cpu/generic/atomic_word.h $(target_builddir)/atomic_word.h
	cp -pR $(glibcpp_srcdir)/config/io/basic_file_stdio.h $(target_builddir)/basic_file.h
	cp -pR $(glibcpp_srcdir)/config/allocator/new_allocator_base.h $(target_builddir)/c++allocator.h
	cp -pR $(glibcpp_srcdir)/config/io/c_io_stdio.h $(target_builddir)/c++io.h
	cp -pR $(glibcpp_srcdir)/config/locale/generic/c_locale.h $(target_builddir)/c++locale.h
	cp -pR $(glibcpp_srcdir)/config/locale/ieee_1003.1-2001/codecvt_specializations.h $(target_builddir)/codecvt_specializations.h
	cp -pR $(glibcpp_srcdir)/config/locale/generic/messages_members.h $(target_builddir)/messages_members.h
	cp -pR ../gcc/gthr*.h $(target_builddir)
	for x in $(std_headers_rename); do mv ./include/std_$$x.h ./include/$$x; done
	for x in $(c_base_headers_rename); do mv ./include/std_$$x.h ./include/$$x; done
	# Merge src directory
	cp -pR $(glibcpp_srcdir)/src/*.cc ./
	cp -pR $(glibcpp_srcdir)/config/cpu/generic/atomicity.h ./atomicity.cc
	cp -pR $(glibcpp_srcdir)/config/io/basic_file_stdio.cc ./basic_file.cc
	cp -pR $(glibcpp_srcdir)/config/locale/generic/c_locale.cc ./c++locale.cc
	cp -pR $(glibcpp_srcdir)/config/locale/generic/codecvt_members.cc ./codecvt_members.cc
	cp -pR $(glibcpp_srcdir)/config/locale/generic/collate_members.cc ./collate.cc
	cp -pR $(glibcpp_srcdir)/config/locale/generic/ctype_members.cc ./ctype_members.cc
	cp -pR $(glibcpp_srcdir)/config/locale/generic/messages_members.cc ./messages.cc
	cp -pR $(glibcpp_srcdir)/config/locale/generic/monetary_members.cc ./monetary.cc
	cp -pR $(glibcpp_srcdir)/config/locale/generic/numeric_members.cc ./numeric.cc
	cp -pR $(glibcpp_srcdir)/config/locale/generic/time_members.cc ./time.cc	
	# Merge libio directory
	#cp -pR $(glibcpp_srcdir)/libio/*.c ./
	#cp -pR $(glibcpp_srcdir)/libio/*.h ./
	# Merge libsupc directory
	cp -pR $(glibcpp_srcdir)/libsupc++/*.cc ./libsupc
	cp -pR $(glibcpp_srcdir)/libsupc++/*.h ./libsupc
	cp -pR ${glibcpp_srcdir}/libsupc++/new ./libsupc
	cp -pR ${glibcpp_srcdir}/libsupc++/typeinfo ./libsupc
	cp -pR ${glibcpp_srcdir}/libsupc++/exception ./libsupc

$(libobjdir)/cp-demangle.o: ../../libiberty/cp-demangle.c ../../include/demangle.h
	$(CROSS_CC) $(CROSS_CCFLAGS) -o $@ -c $< -I ../../include $(CFL) -DIN_GLIBCPP_V3

$(libobjdir)/dyn-string.o: ../../libiberty/dyn-string.c ../../include/dyn-string.h
	$(CROSS_CC) $(CROSS_CCFLAGS) -o $@ -c $< -I ../../include $(CFL) -DIN_GLIBCPP_V3


$(libobjdir)/atomicity.o: atomicity.cc
$(libobjdir)/bitset.o: bitset.cc
$(libobjdir)/c++locate.o: c++locate.cc
$(libobjdir)/codecvt.o: codecvt.cc
$(libobjdir)/codecvt_members.o: codecvt_members.cc
$(libobjdir)/collate.o: collate.cc
$(libobjdir)/complex_io.o: complex_io.cc
$(libobjdir)/concept-inst.o: concept-inst.cc
$(libobjdir)/ctype.o: ctype.cc
$(libobjdir)/ctype_members.o: ctype_members.cc
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

$(supcobjdir)/del_op.o: libsupc/del_op.cc
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -o $@ -c $< $(SUPCINCLUDES) $(CFL)
$(supcobjdir)/del_opnt.o: libsupc/del_opnt.cc
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -o $@ -c $< $(SUPCINCLUDES) $(CFL)
$(supcobjdir)/del_opv.o: libsupc/del_opv.cc
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -o $@ -c $< $(SUPCINCLUDES) $(CFL)
$(supcobjdir)/del_opvnt.o: libsupc/del_opvnt.cc
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -o $@ -c $< $(SUPCINCLUDES) $(CFL)
$(supcobjdir)/eh_alloc.o: libsupc/eh_alloc.cc
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -o $@ -c $< $(SUPCINCLUDES) $(CFL)
$(supcobjdir)/eh_aux_runtime.o: libsupc/eh_aux_runtime.cc
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -o $@ -c $< $(SUPCINCLUDES) $(CFL)
$(supcobjdir)/eh_catch.o: libsupc/eh_catch.cc
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -o $@ -c $< $(SUPCINCLUDES) $(CFL)
$(supcobjdir)/eh_exception.o: libsupc/eh_exception.cc
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -o $@ -c $< $(SUPCINCLUDES) $(CFL)
$(supcobjdir)/eh_globals.o: libsupc/eh_globals.cc
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -o $@ -c $< $(SUPCINCLUDES) $(CFL)
$(supcobjdir)/eh_personality.o: libsupc/eh_personality.cc
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -o $@ -c $< $(SUPCINCLUDES) $(CFL)
$(supcobjdir)/eh_term_handler.o: libsupc/eh_term_handler.cc
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -o $@ -c $< $(SUPCINCLUDES) $(CFL)
$(supcobjdir)/eh_terminate.o: libsupc/eh_terminate.cc
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -o $@ -c $< $(SUPCINCLUDES) $(CFL)
$(supcobjdir)/eh_throw.o: libsupc/eh_throw.cc
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -o $@ -c $< $(SUPCINCLUDES) $(CFL)
$(supcobjdir)/eh_type.o: libsupc/eh_type.cc
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -o $@ -c $< $(SUPCINCLUDES) $(CFL)
$(supcobjdir)/eh_unex_handler.o: libsupc/eh_unex_handler.cc
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -o $@ -c $< $(SUPCINCLUDES) $(CFL)
$(supcobjdir)/guard.o: libsupc/guard.cc
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -o $@ -c $< $(SUPCINCLUDES) $(CFL)
$(supcobjdir)/new_handler.o: libsupc/new_handler.cc
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -o $@ -c $< $(SUPCINCLUDES) $(CFL)
$(supcobjdir)/new_op.o: libsupc/new_op.cc
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -o $@ -c $< $(SUPCINCLUDES) $(CFL)
$(supcobjdir)/new_opnt.o: libsupc/new_opnt.cc
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -o $@ -c $< $(SUPCINCLUDES) $(CFL)
$(supcobjdir)/new_opv.o: libsupc/new_opv.cc
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -o $@ -c $< $(SUPCINCLUDES) $(CFL)
$(supcobjdir)/new_opvnt.o: libsupc/new_opvnt.cc
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -o $@ -c $< $(SUPCINCLUDES) $(CFL)
$(supcobjdir)/pure.o: libsupc/pure.cc
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -o $@ -c $< $(SUPCINCLUDES) $(CFL)
$(supcobjdir)/tinfo2.o: libsupc/tinfo2.cc
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -o $@ -c $< $(SUPCINCLUDES) $(CFL)
$(supcobjdir)/tinfo.o: libsupc/tinfo.cc
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -o $@ -c $< $(SUPCINCLUDES) $(CFL)
$(supcobjdir)/vec.o: libsupc/vec.cc
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -o $@ -c $< $(SUPCINCLUDES) $(CFL)
$(supcobjdir)/vterminate.o: libsupc/vterminate.cc
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -o $@ -c $< $(SUPCINCLUDES) $(CFL)

$(supcobjdir)/cp-demangle.o: ../../libiberty/cp-demangle.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(SUPCINCLUDES) -I ../../include $(CFL) -DIN_GLIBCPP_V3
