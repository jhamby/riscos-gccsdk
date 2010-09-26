@ __builtin_frame_address
@ This source is used by SCL and UnixLib libraries.
@ Copyright (c) 2002-2010 UnixLib Developers

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
	.global	__builtin_frame_address
	NAME	__builtin_frame_address
#if __TARGET_UNIXLIB__
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
	SUB	a2, sl, #512 + CHUNK_OVERHEAD

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
__builtin_frame_address_sharedclibrary:
	LDR	a2, [a2, #CHUNK_PREV]
	TEQ	a2, #0		@ No previous stack chunk
	MOVEQ	a1, #0
	LDMEQEA	fp, {v1, v2, fp, sp, pc}
	
	@ The SharedCLibrary creates a dummy frame to store the return address
	@ for use when deallocating stack chunks, UnixLib stores the return
	@ address in the stack chunk header.
	MOV	a1, ip

__builtin_frame_address_loop_end:
	ADD	a3, a3, #1
	CMP	a3, a4
	BLE	__builtin_frame_address_loop

	LDMEA	fp, {v1, v2, fp, sp, pc}	@ a1 => fp

#elif __TARGET_SCL__

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

#if __TARGET_MODULE__
	@ Check processor mode to determine type of stack. Only USR mode
	@ uses a chunked stack.
	TEQ	a1, a1
	TEQ	pc, pc
	MRSEQ	v2, cpsr
	MOVNE	v2, pc
	TST	v2, #3
	BNE	__builtin_frame_address_flat_stack
#endif

	@ We must cope with a variety of UnixLib and SharedCLibrary stacks
	@ occuring within a program.
	LDR	v2, __stackchunk_magic_number
	@ Make 'a2' point to the current stack chunk.
	SUB	a2, sl, #512 + CHUNK_OVERHEAD

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
	
	@ The SharedCLibrary creates a dummy frame to store the return address
	@ for use when deallocating stack chunks, UnixLib stores the return
	@ address in the stack chunk header.
	LDR	a1, [ip, #-12]

__builtin_frame_address_loop_end:
	ADD	a3, a3, #1
	CMP	a3, a4
	BLE	__builtin_frame_address_loop

	LDR	v1, [a2, #CHUNK_MAGIC]
	TEQ	v1, v2
	LDMNEEA	fp, {v1, v2, fp, sp, pc}	@ a1 => fp

	LDR	v1, [a2, #CHUNK_SIZE]
	ADD	v1, v1, a2
	CMP	a1, v1
	LDRGE	a1, [a1, #-12]
	LDMGEEA	fp, {v1, v2, fp, sp, pc}	@ a1 => fp

	CMP	a1, a2
	LDRLT	a1, [a1, #-12]
	LDMEA	fp, {v1, v2, fp, sp, pc}	@ a1 => fp
	
#if __TARGET_MODULE__
__builtin_frame_address_flat_stack:
	MOV	a3, #0
0:
	@ a1 = current fp
	LDR	a1, [a1, #-12]	@ a1 = next fp
	ADD	a3, a3, #1
	CMP	a3, a4
	BLE	0b

	LDMEA	fp, {v1, v2, fp, sp, pc}	@ a1 => fp
#endif

#else
#  error "Unsupported runtime"
#endif
	DECLARE_FUNCTION __builtin_frame_address

	.end
