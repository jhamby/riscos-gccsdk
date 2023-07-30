CC = gcc
TARGET = !RunImage,e1f
STRIP = strip

OBJS = muview.o timer.o

CFLAGS = -Wall -O3

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -static -o $@ -lOSLib32 -lOSLibSupport32 \
	 -lmupdf-third -lmupdf -lmupdf-third -lOSLib32 -lharfbuzz \
	 -lglib-2.0 -lintl
	$(STRIP) $@

clean:
	rm -f $(OBJS) $(TARGET)
