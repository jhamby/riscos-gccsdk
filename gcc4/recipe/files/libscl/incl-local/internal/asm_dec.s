@ Global definitions used by all assembler files.
@ Copyright (c) 2007 UnixLib Developers.

	@ Import the assembler helper macros.
#include "elf-macros.s"

	@ Entries in the struct stack_chunk.  Must be kept in sync with
	@ kernel.h definition _kernel_stack_chunk.
.set	CHUNK_MAGIC, 0		@ Magic number to help detect if someone overwrites the stack
.set	CHUNK_NEXT, 4		@ Ptr to next chunk
.set	CHUNK_PREV, 8		@ Ptr to previous chunk
.set	CHUNK_SIZE, 12		@ Size of chunk, including header
.set	CHUNK_DEALLOC, 16	@ Function to call to free the chunk

.set	CHUNK_OVERHEAD, 20	@ Size of chunk header
