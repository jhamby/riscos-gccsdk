/*
 * H.ChunkFile - Definitions for Arm Chunk files
 * Borrowed from Gustav (pmoore@cix.compulink.co.uk) C utilities
 */

#ifndef __chunkfile_h
#define __chunkfile_h

/* Format of a chunk file header - first few bytes of a chunk file */

typedef struct
{
  uint32_t ChunkIDPrefix;
  uint32_t ChunkIDType;
  uint32_t FileOffset;
  uint32_t Size;
}
ChunkFileHeaderEntry;

typedef struct
{
  uint32_t ChunkField;
  uint32_t maxChunks;
  uint32_t noChunks;
}
ChunkFileHeader;

#define ChunkFileID (0xC3CBC6C5)
#define ChunkID_LIB (0x5F42494C)	/* (unsigned)('LIB_') */
#define ChunkID_OFL (0x5F4C464F)	/* (unsigned)('OFL_') */
#define ChunkID_OBJ (0x5F4A424F)	/* (unsigned)('OBJ_') */
#define ChunkID_LIB_DIRY (0x59524944)	/* (unsigned)('DIRY') */
#define ChunkID_LIB_TIME (0x454D4954)	/* (unsigned)('TIME') */
#define ChunkID_LIB_VRSN (0x4E535256)	/* (unsigned)('VRSN') */
#define ChunkID_LIB_DATA (0x41544144)	/* (unsigned)('DATA') */
#define ChunkID_OFL_SYMT (0x544D5953)	/* (unsigned)('SYMT') */
#define ChunkID_OFL_TIME (0x454D4954)	/* (unsigned)('TIME') */
#define ChunkID_OBJ_HEAD (0x44414548)	/* (unsigned)('HEAD') */
#define ChunkID_OBJ_AREA (0x41455241)	/* (unsigned)('AREA') */
#define ChunkID_OBJ_IDFN (0x4E464449)	/* (unsigned)('IDFN') */
#define ChunkID_OBJ_SYMT (0x544D5953)	/* (unsigned)('SYMT') */
#define ChunkID_OBJ_STRT (0x54525453)	/* (unsigned)('STRT') */

#define chunkFileHeaderSize(max) \
        (sizeof (ChunkFileHeader) + \
        (max - 1) * sizeof (ChunkFileHeaderEntry))

typedef struct
{
  uint32_t ChunkIndex;
  uint32_t EntryLength;
  uint32_t DataLength;
  char LibData[1];
}
LibDirectoryEntry;

#define Lib_DataName(ptr) (char *)((ptr)->LibData)
#define Lib_DataTime(ptr) ((struct lib_TimeStamp *)((ptr)->LibData + \
                          ( (strlen((ptr)->LibData)+4) & ~0x03 ) ) )

typedef struct
{
  unsigned char t[8];
}
LibTimeStamp;

#endif
