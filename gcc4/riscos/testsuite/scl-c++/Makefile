CXX = arm-unknown-riscos-g++
CXXFLAGS = -O2
LINKFLAGS = 
CMHG = cmunge
CMHGFLAGS = -tgcc -32bit

TARGETS = test_abs,ff8 test_mod,ffa

.phony: all
all: $(TARGETS)

test_abs,ff8:
	$(CXX) $(LINKFLAGS) -mlibscl -o $@ test.cc

test_mod,ffa: MdlHdr.h MdlHdr.o
	$(CXX) $(CXXFLAGS) -mmodule -o mod.o -c mod.cc
	$(CXX) $(CXXFLAGS) -mmodule -o test.o -c test.cc
	$(CXX) $(LINKFLAGS) -mmodule -o $@ mod.o test.o MdlHdr.o

.SUFFIXES:
.SUFFIXES: .o .h .cmhg

.cmhg.h:;	$(CMHG) $(CMHGFLAGS) -d $@ $<
.cmhg.o:;	$(CMHG) $(CMHGFLAGS) -o $@ $<

clean:
	-rm *.o MdlHdr.h $(TARGETS)

