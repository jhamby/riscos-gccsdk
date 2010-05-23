# Simple C++ programs which are compiled 3 times:
#  1) using SharedCLibrary as runtime library
#  2) using static UnixLib as runtime library (both in ELF and AIF output)
#  3) using shared UnixLib as runtime library

CXX = g++
# Compile + link flags for SharedCLibrary as runtime library:
CXXFLAGS_SCL = -mlibscl -O3
# Compile + link flags for static UnixLib as runtime library. Note that
# '-static' option is a linker option and should not be specified when
# only compiling.  The '-Wl,-s' option instructs the linker to strip
# the resulting ELF binary resulting in smaller binaries (not without
# runtime difference as the ELF loader will not load the symbol and
# debugging information).
CXXFLAGS_ULST = -static -Wl,-s -O3
# Compile + link flags for shared UnixLib as runtime library (default):
CXXFLAGS_ULSO = -Wl,-s -O3

ELF2AIF = elf2aif

all: template_scl template_static_ul template_static_ul_aif template_shared_ul \
	exception_scl exception_static_ul exception_static_ul_aif exception_shared_ul \
	helloworld_scl helloworld_static_ul helloworld_static_ul_aif helloworld_shared_ul

template_scl: template.cc
	$(CXX) $(CXXFLAGS_SCL) -o $@ template.cc

template_static_ul: template.cc
	$(CXX) $(CXXFLAGS_ULST) -o $@ template.cc

template_static_ul_aif: template_static_ul
	$(ELF2AIF) $< $@

template_shared_ul: template.cc
	$(CXX) $(CXXFLAGS_ULSO) -o $@ template.cc

exception_scl: exception.cc
	$(CXX) $(CXXFLAGS_SCL) -o $@ exception.cc

exception_static_ul: exception.cc
	$(CXX) $(CXXFLAGS_ULST) -o $@ exception.cc

exception_static_ul_aif: exception_static_ul
	$(ELF2AIF) $< $@

exception_shared_ul: exception.cc
	$(CXX) $(CXXFLAGS_ULSO) -o $@ exception.cc

helloworld_scl: helloworld.cc
	$(CXX) $(CXXFLAGS_SCL) -o $@ helloworld.cc

helloworld_static_ul: helloworld.cc
	$(CXX) $(CXXFLAGS_ULST) -o $@ helloworld.cc

helloworld_static_ul_aif: helloworld_static_ul
	$(ELF2AIF) $< $@

helloworld_shared_ul: helloworld.cc
	$(CXX) $(CXXFLAGS_ULSO) -o $@ helloworld.cc
