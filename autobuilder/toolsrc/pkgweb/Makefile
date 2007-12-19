
CPPFLAGS =
CXXFLAGS = -I. -I../libpkg -Wall -W -Wno-unused -Wno-deprecated -O3
LD=gcc
LDFLAGS = -L../libpkg -lpkg -lz -lstdc++

TARGET=../../pkgweb

OBJ = main.o summary.o writer.o

.PHONY: all

all: $(TARGET)

$(TARGET): $(OBJ)
	$(LD) $(OBJ) $(LDFLAGS) -o $(TARGET)

clean:
	rm -f $(OBJ)
	rm -f $(TARGET)
	rm -f $(TARGET).exe

main.o: main.cc summary.h detailspage.h catpage.h alphapage.h
summary.o: summary.cc summary.h
writer.o: writer.cc writer.h
