# Makefile for libsupc++ partof GNU Libstdc++ v3
# Written by Nick Burrett <nick@dsvr.net>
#

INCLUDES = -I. -I../gcc -I../libstdc/include -I../libstdc/include/arm-riscos-aof
supcobjdir=$(objdir)/gcc-$(VERSION)/libsupc++/$(STAGE)/$(TRG)

CROSS_CXXFLAGS += -D_GNU_SOURCE

$(supcobjdir)/%.o: %.cc
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -o $@ -c $< $(INCLUDES) $(CFL)

OBJS =	$(supcobjdir)/del_op.o \
	$(supcobjdir)/del_opnt.o \
	$(supcobjdir)/del_opv.o \
	$(supcobjdir)/del_opvnt.o \
	$(supcobjdir)/eh_alloc.o \
	$(supcobjdir)/eh_aux_runtime.o \
	$(supcobjdir)/eh_catch.o \
	$(supcobjdir)/eh_exception.o \
	$(supcobjdir)/eh_globals.o \
	$(supcobjdir)/eh_personality.o \
	$(supcobjdir)/eh_terminate.o \
	$(supcobjdir)/eh_throw.o \
	$(supcobjdir)/eh_type.o \
	$(supcobjdir)/guard.o \
	$(supcobjdir)/new_handler.o \
	$(supcobjdir)/new_op.o \
	$(supcobjdir)/new_opnt.o \
	$(supcobjdir)/new_opv.o \
	$(supcobjdir)/new_opvnt.o \
	$(supcobjdir)/pure.o \
	$(supcobjdir)/tinfo2.o \
	$(supcobjdir)/tinfo.o \
	$(supcobjdir)/vec.o

all:	$(ux_gcc_bin_dir)/$(TRG)/libsupc++.o \
	$(ro_gcc_bin_dir)/$(TRG)/o/libsupc++

install_headers:
	$(insthdr) . "*.h" . $(ux_gccpkg)/$(gn_gplusplus_include_dir) $(ro_gccpkg)/$(gn_gplusplus_include_dir)
	$(insthdr) . exception . $(ux_gccpkg)/$(gn_gplusplus_include_dir) $(ro_gccpkg)/$(gn_gplusplus_include_dir)
	$(insthdr) . new . $(ux_gccpkg)/$(gn_gplusplus_include_dir) $(ro_gccpkg)/$(gn_gplusplus_include_dir)
	$(insthdr) . typeinfo . $(ux_gccpkg)/$(gn_gplusplus_include_dir) $(ro_gccpkg)/$(gn_gplusplus_include_dir)

$(ux_gcc_bin_dir)/$(TRG)/libsupc++.o: $(OBJS)
	$(CROSS_AR) $(CROSS_AR_FLAGS) $@ $(OBJS)

$(ro_gcc_bin_dir)/$(TRG)/o/libsupc++: $(OBJS)
	$(CROSS_AR) $(CROSS_AR_FLAGS) $@ $(OBJS)

$(supcobjdir)/del_op.o: del_op.cc
$(supcobjdir)/del_opnt.o: del_opnt.cc
$(supcobjdir)/del_opv.o: del_opv.cc
$(supcobjdir)/del_opvnt.o: del_opvnt.cc
$(supcobjdir)/eh_alloc.o: eh_alloc.cc
$(supcobjdir)/eh_aux_runtime.o: eh_aux_runtime.cc
$(supcobjdir)/eh_catch.o: eh_catch.cc
$(supcobjdir)/eh_exception.o: eh_exception.cc
$(supcobjdir)/eh_globals.o: eh_globals.cc
$(supcobjdir)/eh_personality.o: eh_personality.cc
$(supcobjdir)/eh_terminate.o: eh_terminate.cc
$(supcobjdir)/eh_throw.o: eh_throw.cc
$(supcobjdir)/eh_type.o: eh_type.cc
$(supcobjdir)/guard.o: guard.cc
$(supcobjdir)/new_handler.o: new_handler.cc
$(supcobjdir)/new_op.o: new_op.cc
$(supcobjdir)/new_opnt.o: new_opnt.cc
$(supcobjdir)/new_opv.o: new_opv.cc
$(supcobjdir)/new_opvnt.o: new_opvnt.cc
$(supcobjdir)/pure.o: pure.cc
$(supcobjdir)/tinfo2.o: tinfo2.cc
$(supcobjdir)/tinfo.o: tinfo.cc
$(supcobjdir)/vec.o: vec.cc
