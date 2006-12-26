; __builtin_frame_address
; Copyright (c) 2002, 2003, 2004, 2005, 2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	[ __UNIXLIB_ELF > 0
	@ BEWARE: this looks like GAS syntax but isn't, the labels don't
	@ have a colon.

	.text

__stackchunk_magic_number
	WORD	0xF60690FF

	@ unsigned int __builtin_frame_address (unsigned int level)
	@
	@ Return the address of the frame 'level' functions ago in a1.
	@
	@ SharedCLibrary:
	@   typedef struct __exception_stack_chunk
	@   {
	@     unsigned long sc_mark;   == 0xf60690ff
	@     struct __exception_stack_chunk *sc_next, *sc_prev;
	@     unsigned long sc_size;
	@     int (*sc_deallocate)(void *);
	@   } _exception_stack_chunk;
	@
	@ UnixLib:
	@   struct __stack_chunk
	@   {
	@     unsigned int magic;            /* CHUNK_MAGIC */
	@     struct __stack_chunk *next;    /* CHUNK_NEXT */
	@     struct __stack_chunk *prev;    /* CHUNK_PREV */
	@     unsigned int size;             /* CHUNK_SIZE */
	@     void (*dealloc) (void *);      /* CHUNK_DEALLOC */
	@     void (*returnaddr) (void);     /* CHUNK_RETURN */
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
__builtin_frame_address
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
	SUB	a2, sl, #536	@ Check for UnixLib chunked stack
	LDR	v1, [a2, #CHUNK_MAGIC]
	TEQ	v1, v2
	@ If not a UnixLib chunked stack, then it must be a
	@ SharedCLibrary chunked stack or it is not chunked.
	SUBNE	a2, sl, #560

	MOV	a3, #0
__builtin_frame_address_loop
	@ a1 = current fp
	@ a2 => current stack chunk
	@ v2 = stack magic word

	LDR	ip, [a1, #-12]	@ ip = next fp
	LDR	v1, [a2, #CHUNK_MAGIC]
	TEQ	v2, v1
	MOVNE	a1, ip
	BNE	__builtin_frame_address_loop_end

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
__builtin_frame_address_sharedclibrary
	LDR	a2, [a2, #CHUNK_PREV]
	TEQ	a2, #0		@ No previous stack chunk
	MOVEQ	a1, #0
	LDMEQEA	fp, {v1, v2, fp, sp, pc}
	LDR	a1, [ip, #-12]

__builtin_frame_address_loop_end
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
	DECLARE_FUNCTION __builtin_frame_address

	]

	END
