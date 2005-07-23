# Makefile for the ARM Library Format compiler - LIBFILE
# Written by Nick Burrett <nick@dsvr.net>

ifeq (${STAGE},stage1)
CFLAGS += -DCROSS_COMPILE -D_GNU_SOURCE
CXXFLAGS += -DCROSS_COMPILE -D_GNU_SOURCE
linklibs = -lstdc++
exesfx = 
else
CC = $(CROSS_CC)
CFLAGS = $(CROSS_CFLAGS) -D_GNU_SOURCE
CXX = $(CROSS_CXX)
CXXFLAGS = $(CROSS_CXXFLAGS) -D_GNU_SOURCE
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
	$(libfileobjdir)/LibData.o \
	$(libfileobjdir)/Library.o \
	$(libfileobjdir)/ArgParser.o \
	$(libfileobjdir)/ObjFile.o \
	$(libfileobjdir)/BError.o \
	$(libfileobjdir)/Librarian.o \
	$(libfileobjdir)/Ar.o \
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
DEPS += $(gcc_bin_dir)/$(TRG)/o/unixlib
endif

all:	$(bin_dir)/libfile$(exesfx) $(bin_dir)/ar$(exesfx)

$(bin_dir)/libfile$(exesfx):	$(libfileobjdir)/LibFile.o $(OBJS) $(DEPS)
	$(CXX) -o $@ $(libfileobjdir)/LibFile.o $(OBJS)

$(bin_dir)/ar$(exesfx):	$(libfileobjdir)/ArFile.o $(OBJS) $(DEPS)
	$(CXX) -o $@ $(libfileobjdir)/ArFile.o $(OBJS)

$(libfileobjdir)/TimeStamp.o: TimeStamp.cc
$(libfileobjdir)/OflSymt.o: OflSymt.cc
$(libfileobjdir)/LibDir.o: LibDir.cc
$(libfileobjdir)/LibVersion.o: LibVersion.cc
$(libfileobjdir)/LibTimeBas.o: LibTimeBas.cc
$(libfileobjdir)/LibFile.o: LibFile.cc
$(libfileobjdir)/LibData.o: LibData.cc
$(libfileobjdir)/Library.o: Library.cc
$(libfileobjdir)/ArgParser.o: ArgParser.cc
$(libfileobjdir)/Ar.o: Ar.cc Ar.h
$(libfileobjdir)/Ar.o: ArFile.cc Ar.h
$(libfileobjdir)/ObjFile.o: ObjFile.cc
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

$(libfileobjdir)/ArgParser.o:  \
 ArgParser.cc ArgParser.h BString.h BList.h BError.h
$(libfileobjdir)/BError.o:  BError.cc BError.h BString.h
$(libfileobjdir)/BString.o: BString.cc BString.h
$(libfileobjdir)/Buffer.o:  Buffer.cc Buffer.h BString.h BError.h
$(libfileobjdir)/Chunk.o:  \
  Chunk.cc Chunk.h BString.h Buffer.h Library.h BList.h LibData.h \
  TimeStamp.h ChunkFile.h BError.h
$(libfileobjdir)/ChunkFile.o:  \
 ChunkFile.cc ChunkFile.h BString.h Chunk.h Buffer.h BError.h
$(libfileobjdir)/LibData.o:  \
 LibData.cc LibData.h Chunk.h BString.h Buffer.h TimeStamp.h BError.h
$(libfileobjdir)/LibDir.o:  \
 LibDir.cc LibDir.h Chunk.h BString.h Buffer.h BList.h TimeStamp.h \
  BError.h Path.h
$(libfileobjdir)/LibFile.o:  \
 LibFile.cc Librarian.h BString.h BList.h BError.h
$(libfileobjdir)/LibTimeBas.o:  \
 LibTimeBas.cc LibTimeBas.h Chunk.h BString.h Buffer.h TimeStamp.h \
  BError.h
$(libfileobjdir)/LibVersion.o:  \
 LibVersion.cc LibVersion.h Chunk.h BString.h Buffer.h
$(libfileobjdir)/Librarian.o:  \
 Librarian.cc Chunk.h BString.h Buffer.h Librarian.h BList.h ArgParser.h \
  ObjFile.h ChunkFile.h BError.h LibTime.h LibTimeBas.h TimeStamp.h \
  LibVersion.h LibDir.h LibData.h Library.h
$(libfileobjdir)/Library.o:  \
 Library.cc Library.h BList.h LibData.h Chunk.h BString.h Buffer.h \
  TimeStamp.h ChunkFile.h LibTime.h LibTimeBas.h LibVersion.h LibDir.h \
  OflTime.h OflSymt.h BError.h Path.h
$(libfileobjdir)/ObjFile.o:  \
 ObjFile.cc ObjFile.h Buffer.h BString.h ChunkFile.h Chunk.h BList.h \
  BError.h
$(libfileobjdir)/OflSymt.o:  \
 OflSymt.cc OflSymt.h Chunk.h BString.h Buffer.h BList.h TimeStamp.h \
  BError.h ObjFile.h ChunkFile.h LibDir.h LibData.h
$(libfileobjdir)/Path.o:  Path.cc Path.h BString.h BList.h
$(libfileobjdir)/TimeStamp.o:  TimeStamp.cc TimeStamp.h BString.h Buffer.h
