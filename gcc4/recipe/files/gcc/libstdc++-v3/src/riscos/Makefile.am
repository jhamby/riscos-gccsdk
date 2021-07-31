## Makefile to regenerate the static archive using non-PIC objects.

CPP98_OBJECTS=$(wildcard ../c++98/*.o)
CPP11_OBJECTS=$(wildcard ../c++11/*.o)
SRC_OBJECTS=$(wildcard ../*.o)
SUPC_OBJECTS=$(wildcard ../../libsupc++/*.o)
all: libstdc++-static.a

libstdc++-static.a: ../.libs/libstdc++.a
	arm-unknown-riscos-ar cru $@ $(CPP98_OBJECTS) $(CPP11_OBJECTS) $(SRC_OBJECTS) $(SUPC_OBJECTS)
	arm-unknown-riscos-ranlib $@
	mv -T ../.libs/libstdc++.a ../.libs/libstdc++.a.bak
	cp -T $@ ../.libs/libstdc++.a
