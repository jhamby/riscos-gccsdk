# Makefile for the ARM Library Format compiler - LIBFILE
# Written by Nick Burrett <nick@dsvr.net>

ifeq (${STAGE},stage1)
CXXFLAGS += -DCROSS_COMPILE
linklibs = -lstdc++
else
CC = $(CROSS_CC)
CXX = $(CROSS_CXX)
CXXFLAGS = $(CROSS_CXXFLAGS) -fpermissive
linklibs = -liostream -lstdc++
endif

INCLUDES = -I. -I../../
CXXFLAGS += $(TARGET_DEFS)
CFLAGS += $(TARGET_DEFS)

libfileobjdir = $(objdir)/riscos-aof/libfile/$(STAGE)

$(libfileobjdir)/%.o: %.cc
	$(CXX) $(CXXFLAGS) -o $@ -c $< $(INCLUDES)

$(libfileobjdir)/%.o: %.s
	$(CC) $(CFLAGS) -o $@ -c $<

OBJS = $(libfileobjdir)/TimeStamp.o \
	$(libfileobjdir)/OflSymt.o \
	$(libfileobjdir)/LibDir.o \
	$(libfileobjdir)/LibVersion.o \
	$(libfileobjdir)/LibTimeBas.o \
	$(libfileobjdir)/LibFile.o \
	$(libfileobjdir)/LibData.o \
	$(libfileobjdir)/Library.o \
	$(libfileobjdir)/ArgParser.o \
	$(libfileobjdir)/ObjFile.o \
	$(libfileobjdir)/BException.o \
	$(libfileobjdir)/BError.o \
	$(libfileobjdir)/Librarian.o \
	$(libfileobjdir)/BString.o \
	$(libfileobjdir)/Buffer.o \
	$(libfileobjdir)/Chunk.o \
	$(libfileobjdir)/ChunkFile.o \
	$(libfileobjdir)/Path.o

ifeq (${STAGE},stage2)
OBJS +=	$(libfileobjdir)/OS_File.o \
	$(libfileobjdir)/OS_GBPB.o \
	$(libfileobjdir)/OS_Word.o \
	$(libfileobjdir)/OS_CnvtDT.o
endif

all:	$(bin_dir)/libfile

$(bin_dir)/libfile:	$(OBJS)
	$(CXX) -o $@ $(OBJS) $(linklibs)

$(libfileobjdir)/TimeStamp.o: TimeStamp.cc
$(libfileobjdir)/OflSymt.o: OflSymt.cc
$(libfileobjdir)/LibDir.o: LibDir.cc
$(libfileobjdir)/LibVersion.o: LibVersion.cc
$(libfileobjdir)/LibTimeBas.o: LibTimeBas.cc
$(libfileobjdir)/LibFile.o: LibFile.cc
$(libfileobjdir)/LibData.o: LibData.cc
$(libfileobjdir)/Library.o: Library.cc
$(libfileobjdir)/ArgParser.o: ArgParser.cc
$(libfileobjdir)/ObjFile.o: ObjFile.cc
$(libfileobjdir)/BException.o: BException.cc
$(libfileobjdir)/BError.o: BError.cc
$(libfileobjdir)/Librarian.o: Librarian.cc
$(libfileobjdir)/BString.o: BString.cc
$(libfileobjdir)/Buffer.o: Buffer.cc
$(libfileobjdir)/Chunk.o: Chunk.cc
$(libfileobjdir)/ChunkFile.o: ChunkFile.cc
$(libfileobjdir)/Path.o: Path.cc
$(libfileobjdir)/OS_File.o: OS_File.s
$(libfileobjdir)/OS_GBPB.o: OS_GBPB.s
$(libfileobjdir)/OS_Word.o: OS_Word.s
$(libfileobjdir)/OS_CnvtDT.o: OS_CnvtDT.s
