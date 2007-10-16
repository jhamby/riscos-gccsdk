# Simple C++ programs.

CXX = g++
CXXFLAGS = -static -static-libgcc -O3
LDFLAGS = -static -static-libgcc

all: template exception helloworld

TEMP_OBJ = template.o
template: $(TEMP_OBJ)
	$(CXX) $(LDFLAGS) -o $@ $(TEMP_OBJ)

EXC_OBJ = exception.o
exception: $(EXC_OBJ)
	$(CXX) $(LDFLAGS) -o $@ $(EXC_OBJ)

HLLW_OBJ = helloworld.o
helloworld: $(HLLW_OBJ)
	$(CXX) $(LDFLAGS) -o $@ $(HLLW_OBJ)

.cc.o:
	$(CXX) $(CXXFLAGS) -o $@ -c $<
