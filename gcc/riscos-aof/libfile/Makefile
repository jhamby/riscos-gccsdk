INCLUDES = -I. -I../../
CXXFLAGS += -DUNIX $(TARGET_DEFS)
CFLAGS += -DUNIX $(TARGET_DEFS)

$(objdir)/riscos-aof/libfile/%.o: %.cc
	$(CXX) $(CXXFLAGS) -o $@ -c $< $(INCLUDES)

$(objdir)/riscos-aof/libfile/%.o: %.s
	$(CC) $(CFLAGS) -o $@ -c $<

OBJS = $(objdir)/riscos-aof/libfile/TimeStamp.o \
	$(objdir)/riscos-aof/libfile/OflSymt.o \
	$(objdir)/riscos-aof/libfile/LibDir.o \
	$(objdir)/riscos-aof/libfile/LibVersion.o \
	$(objdir)/riscos-aof/libfile/LibTimeBas.o \
	$(objdir)/riscos-aof/libfile/LibFile.o \
	$(objdir)/riscos-aof/libfile/LibData.o \
	$(objdir)/riscos-aof/libfile/Library.o \
	$(objdir)/riscos-aof/libfile/ArgParser.o \
	$(objdir)/riscos-aof/libfile/ObjFile.o \
	$(objdir)/riscos-aof/libfile/BException.o \
	$(objdir)/riscos-aof/libfile/BError.o \
	$(objdir)/riscos-aof/libfile/Librarian.o \
	$(objdir)/riscos-aof/libfile/BString.o \
	$(objdir)/riscos-aof/libfile/Buffer.o \
	$(objdir)/riscos-aof/libfile/Chunk.o \
	$(objdir)/riscos-aof/libfile/ChunkFile.o \
	$(objdir)/riscos-aof/libfile/Path.o

ifeq (${crosscompile},no)
OBJS +=	$(objdir)/riscos-aof/libfile/OS_File.o \
	$(objdir)/riscos-aof/libfile/OS_GBPB.o \
	$(objdir)/riscos-aof/libfile/OS_Word.o \
	$(objdir)/riscos-aof/libfile/OS_CnvtDT.o
endif

all:	$(BIN)/libfile

$(BIN)/libfile:	$(OBJS)
	$(CC) -o $@ $(OBJS) -lstdc++

$(objdir)/riscos-aof/libfile/TimeStamp.o: TimeStamp.cc
$(objdir)/riscos-aof/libfile/OflSymt.o: OflSymt.cc
$(objdir)/riscos-aof/libfile/LibDir.o: LibDir.cc
$(objdir)/riscos-aof/libfile/LibVersion.o: LibVersion.cc
$(objdir)/riscos-aof/libfile/LibTimeBas.o: LibTimeBas.cc
$(objdir)/riscos-aof/libfile/LibFile.o: LibFile.cc
$(objdir)/riscos-aof/libfile/LibData.o: LibData.cc
$(objdir)/riscos-aof/libfile/Library.o: Library.cc
$(objdir)/riscos-aof/libfile/ArgParser.o: ArgParser.cc
$(objdir)/riscos-aof/libfile/ObjFile.o: ObjFile.cc
$(objdir)/riscos-aof/libfile/BException.o: BException.cc
$(objdir)/riscos-aof/libfile/BError.o: BError.cc
$(objdir)/riscos-aof/libfile/Librarian.o: Librarian.cc
$(objdir)/riscos-aof/libfile/BString.o: BString.cc
$(objdir)/riscos-aof/libfile/Buffer.o: Buffer.cc
$(objdir)/riscos-aof/libfile/Chunk.o: Chunk.cc
$(objdir)/riscos-aof/libfile/ChunkFile.o: ChunkFile.cc
$(objdir)/riscos-aof/libfile/Path.o: Path.cc
$(objdir)/riscos-aof/libfile/OS_File.o: OS_File.s
$(objdir)/riscos-aof/libfile/OS_GBPB.o: OS_GBPB.s
$(objdir)/riscos-aof/libfile/OS_Word.o: OS_Word.s
$(objdir)/riscos-aof/libfile/OS_CnvtDT.o: OS_CnvtDT.s
