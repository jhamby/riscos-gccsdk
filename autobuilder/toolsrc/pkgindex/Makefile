
CPPFLAGS =
CXXFLAGS = -I. -I../libpkg -Wall -W -Wno-unused -Wno-deprecated -O2
LD=gcc
LDFLAGS = -lstdc++ -L../libpkg -lpkg -lz

TARGET=../../pkgindex

OBJ = main.o

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(LD) $(OBJ) $(LDFLAGS) -o $(TARGET)

clean:
	rm -f $(OBJ)
	rm -f $(TARGET)
	rm -f $(TARGET).exe

