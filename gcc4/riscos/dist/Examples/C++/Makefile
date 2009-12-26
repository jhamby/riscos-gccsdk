# Simple C++ programs which are compiled 2 times:
#  1) using static UnixLib as runtime library
#  2) using shared UnixLib as runtime library

CXX = g++
CXXFLAGS = -O3
# Difference between using the static or shared UnixLib runtime library
# is only made when linking:
LDFLAGS_ULST = -static -Wl,-s
LDFLAGS_ULSO = -Wl,-s

all: template_static_ul template_shared_ul \
	exception_static_ul exception_shared_ul \
	helloworld_static_ul helloworld_shared_ul

TEMP_OBJ = template.o
template_static_ul: $(TEMP_OBJ)
	$(CXX) $(LDFLAGS_ULST) -o $@ $(TEMP_OBJ)

template_shared_ul: $(TEMP_OBJ)
	$(CXX) $(LDFLAGS_ULSO) -o $@ $(TEMP_OBJ)

EXC_OBJ = exception.o
exception_static_ul: $(EXC_OBJ)
	$(CXX) $(LDFLAGS_ULST) -o $@ $(EXC_OBJ)

exception_shared_ul: $(EXC_OBJ)
	$(CXX) $(LDFLAGS_ULSO) -o $@ $(EXC_OBJ)

HLLW_OBJ = helloworld.o
helloworld_static_ul: $(HLLW_OBJ)
	$(CXX) $(LDFLAGS_ULST) -o $@ $(HLLW_OBJ)

helloworld_shared_ul: $(HLLW_OBJ)
	$(CXX) $(LDFLAGS_ULSO) -o $@ $(HLLW_OBJ)

.cc.o:
	$(CXX) $(CXXFLAGS) -o $@ -c $<
