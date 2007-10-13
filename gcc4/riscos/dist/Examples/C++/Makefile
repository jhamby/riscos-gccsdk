# Simple C++ programs.

CXXFLAGS = -static -static-libgcc -O3 -v 
LDFLAGS = -static -static-libgcc -v -Wl,-v

all: template exception helloworld

TEMP_OBJ = template.o
template: $(TEMP_OBJ)
	$(LINK) $(LDFLAGS) -o $@ $(TEMP_OBJ)

EXC_OBJ = exception.o
exception: $(EXC_OBJ)
	$(LINK) $(LDFLAGS) -o $@ $(EXC_OBJ)

HLLW_OBJ = helloworld.o
helloworld: $(HLLW_OBJ)
	$(LINK) $(LDFLAGS) -o $@ $(HLLW_OBJ)

.cpp.o:
	$(CXX) $(CXXFLAGS) -o $@ -c $<
