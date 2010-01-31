@ __builtin_frame_address
@ This source is used by SCL and UnixLib libraries.
@ Copyright (c) 2002-2009 UnixLib Developers

#include "internal/asm_dec.s"

	.text

__stackchunk_magic_number:
	.word	0xF60690FF

	@ unsigned int __builtin_frame_address (unsigned int level)
	@
	@ Return the address of the frame 'level' functions ago in a1.
	@
	@ SharedCLibrary:
	@   typedef struct __exception_stack_chunk
	@   {
	@     unsigned long sc_mark;		/* == 0xf60690ff */
	@     struct __exception_stack_chunk *sc_next, *sc_prev;
	@     unsigned long sc_size;
	@     int (*sc_deallocate)(void *);
	@   } _exception_stack_chunk;
	@
	@ UnixLib:
	@   struct __stack_chunk
	@   {
	@     unsigned int magic;		/* CHUNK_MAGIC */
	@     struct __stack_chunk *next;	/* CHUNK_NEXT */
	@     struct __stack_chunk *prev;	/* CHUNK_PREV */
	@     unsigned int size;		/* CHUNK_SIZE */
	@     void (*dealloc) (void *);		/* CHUNK_DEALLOC */
	@     void (*returnaddr) (void);	/* CHUNK_RETURN */
	@   };
	@
	@ A UnixLib chunked stack is the same as above, but with an additional
	@ word at the end holding the return address after the chunk is freed.
	@ Cfr the CHUNK_ definitions in asm_dec.s.
	@ It is 536 bytes below sl, whereas for the scl it is 560 bytes below sl.
	@ FIXME: at run-time we can only have one stack type so we could
	@ at assemble time check __TARGET_SCL__ / __TARGET_UNIXLIB__ and
	@ reduce the code a bit.
	.global	__builtin_frame_address
	NAME	__builtin_frame_address
__builtin_frame_address:
	MOV	ip, sp
	STMFD	sp!, {v1, v2, fp, ip, lr, pc}
	SUB	fp, ip, #4

	MOV	a4, a1

	@ Get previous fp value. If zero, we have no enclosing frame,
	@ so no return address.
	LDR	a1, [fp, #-12]
	TEQ	a1, #0
	@ No enclosing frame, so no return address.
	LDMEQEA	fp, {v1, v2, fp, sp, pc}

	MOV	a1, fp
	@ We must cope with a variety of UnixLib and SharedCLibrary stacks
	@ occuring within a program.
	LDR	v2, __stackchunk_magic_number
	@ Make 'a2' point to the current stack chunk.
#if __TARGET_UNIXLIB__
	SUB	a2, sl, #512 + CHUNK_OVERHEAD
#elif __TARGET_SCL__
	SUB	a2, sl, #560
#else
#  error "Unsupported runtime"
#endif

	MOV	a3, #0
__builtin_frame_address_loop:
	@ a1 = current fp
	@ a2 => current stack chunk
	@ v2 = stack magic word

	LDR	ip, [a1, #-12]	@ ip = next fp
	LDR	v1, [a2, #CHUNK_MAGIC]

	@ Check that this a valid stack chunk
	TEQ	v2, v1
	MOVNE	a1, ip
	BNE	__builtin_frame_address_loop_end

	@ Is the frame within the current stack chunk?
	LDR	v1, [a2, #CHUNK_SIZE]
	ADD	v1, v1, a2
	CMP	ip, v1
	BGE	__builtin_frame_address_sharedclibrary

	CMP	ip, a2
	MOVGE	a1, ip
	BGE	__builtin_frame_address_loop_end

	@ SharedCLibrary style: Current frame is outside current stack
	@ chunk, so current frame's stack is the first in this chunk
	@ and was created via stack extension.

	@ This means that x$stack_overflow has copied the current stack
	@ frame somewhere else and then created a new stack frame (in its
	@ old place) which points to the 'current' frame.
__builtin_frame_address_sharedclibrary:
	LDR	a2, [a2, #CHUNK_PREV]
	TEQ	a2, #0		@ No previous stack chunk
	MOVEQ	a1, #0
	LDMEQEA	fp, {v1, v2, fp, sp, pc}
	
#if __TARGET_SCL__
	@ This may well be correct for the SharedCLibrary, but it doesn't seem
	@ to work for UnixLib and causes failures in the C++ call frame
	@ exception system. From the comment above, this may be because
	@ UnxiLib doesn't copy the current stack frame during stack extension.
	LDR	a1, [ip, #-12]
#elif __TARGET_UNIXLIB__
	MOV	a1, ip
#endif

__builtin_frame_address_loop_end:
	ADD	a3, a3, #1
	CMP	a3, a4
	BLE	__builtin_frame_address_loop

#if __TARGET_SCL__
	LDR	v1, [a2, #CHUNK_MAGIC]
	TEQ	v1, v2
	LDMNEEA	fp, {v1, v2, fp, sp, pc}	@ a1 => fp

	@ Again, this code can cause C++ call frame exception handling to fail.
	LDR	v1, [a2, #CHUNK_SIZE]
	ADD	v1, v1, a2
	CMP	a1, v1
	LDRGE	a1, [a1, #-12]
	LDMGEEA	fp, {v1, v2, fp, sp, pc}	@ a1 => fp

	CMP	a1, a2
	LDRLT	a1, [a1, #-12]
#endif
	LDMEA	fp, {v1, v2, fp, sp, pc}	@ a1 => fp
	
	DECLARE_FUNCTION __builtin_frame_address

	.end
