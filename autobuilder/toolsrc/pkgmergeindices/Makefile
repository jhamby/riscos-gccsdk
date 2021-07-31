
CPPFLAGS =
CXXFLAGS = -I. -I../libpkg -Wall -W -Wno-unused -Wno-deprecated -O3
LD=gcc
LDFLAGS = -L../libpkg -lpkg -lz -lstdc++

TARGET=../../pkgmergeindices

OBJ = main.o

.PHONY: all

all: $(TARGET)

$(TARGET): $(OBJ)
	$(LD) $(OBJ) $(LDFLAGS) -o $(TARGET)

clean:
	rm -f $(OBJ)
	rm -f $(TARGET)
	rm -f $(TARGET).exe
