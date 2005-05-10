/*
 * file input/output
 *
 * Andy Duplain, BT Customer Systems, Brighton, UK.  duplain@btcs.bt.co.uk
 * Copyright 2005 GCCSDK Developers
 */

#include "config.h"

#include <stdio.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#include "decaof.h"
#include "error.h"

#ifdef HAVE_MALLOC_H
#include <malloc.h>
#endif

/*
 * check for EOF or write/read errors on stream.
 */
Ferror
check_stream(FILE *fp)
{
	int ret = FNOERR;

	if (feof(fp))
		ret = FEND;
	else if (ferror(fp))
		ret = FRWERR;
	if (ret != FNOERR)
		clearerr(fp);
	return (ret);
}

/*
 * read a byte from the input stream.
 */
Byte
read_byte(FILE *ifp)
{
	return ((Byte)getc(ifp));
}

/*
 * read a little-endian 2-byte halfword from the input stream.
 */
Halfword
read_halfword(FILE *ifp)
{
	Byte lowByte = read_byte(ifp);
	Byte highByte = read_byte(ifp);

	return lowByte + (highByte << 8);
}

/*
 * read a little-endian 4-byte word from the input stream.
 */
Word
read_word(FILE *ifp)
{
	Halfword lowHalfword = read_halfword(ifp);
	Halfword highHalfword = read_halfword(ifp);

	return lowHalfword + (highHalfword << 16);
}

/*
 * read in the chunk header
 */
struct chunkhdr *
read_chunkhdr(FILE *ifp)
{
	static struct chunkhdr hdr;

	fseek(ifp, 0L, 0);
	hdr.chunkfileid = read_word(ifp);
	hdr.maxchunks = read_word(ifp);
	hdr.numchunks = read_word(ifp);
	return (check_stream(ifp) != FRWERR ? &hdr : NULL);
}

/*
 * memory pointers maintained by read_xxx functions
 */

static struct chunkent *ents = NULL;	/* chunk file entries */
static char *strptr = NULL;		/* string table */
static struct symbol *symptr = NULL;	/* symbol table */
static char *idptr = NULL;		/* identification string */
static struct aofhdr *aofhdr = NULL;	/* AOF header */

/*
 * free the memory used by a chunk
 */
int
free_chunk_memory(char *ptr)
{
	if (!ptr)
		return (0);

	if (ptr == (char *)ents) {
		free(ents);
		ents = NULL;
	} else if (ptr == strptr) {
		free(strptr);
		strptr = NULL;
	} else if (ptr == (char *)symptr) {
		free(symptr);
		symptr = NULL;
	} else if (ptr == idptr) {
		free(idptr);
		idptr = NULL;
	} else if (ptr == (char *)aofhdr) {
		free(aofhdr);
		aofhdr = NULL;
	} else
		return (-1);
	return (0);
}

/*
 * read in the chunk entries
 */
struct chunkent *
read_chunkents(FILE *ifp, struct chunkhdr *hdr)
{
	int i;

	if (ents)
		free(ents);
	ents = (struct chunkent *)malloc(
	    sizeof(struct chunkent) * hdr->maxchunks);
	if (!ents) {
		error("memory exhausted");
		abort();
	}

	fseek(ifp, sizeof(struct chunkhdr), 0);
	for (i = 0; i < hdr->numchunks; i++) {
		fread(ents[i].chunkid, 1, 8, ifp);
		ents[i].offset = read_word(ifp);
		ents[i].size = read_word(ifp);
	}

	return (check_stream(ifp) != FRWERR ? ents : NULL);
}

/*
 * read in the string table
 */
char *
read_stringtab(FILE *ifp, struct chunkent *strent)
{
	if (strptr)
		free(strptr);
	strptr = malloc(strent->size);
	if (!strptr) {
		error("memory exhausted");
		abort();
	}

	fseek(ifp, strent->offset, 0);
	*(Word *)strptr = read_word(ifp);	/* size in 1st word */
	fread(strptr + 4, 1, (int)strent->size - 4, ifp);

	return (check_stream(ifp) != FRWERR ? strptr : NULL);
}

/*
 * read in the symbol table
 */
struct symbol *
read_symboltab(FILE *ifp, struct chunkent *syment, int numsyms)
{
	int i;

	if (symptr)
		free(symptr);
	symptr = (struct symbol *)malloc(numsyms * sizeof(struct symbol));
	if (!symptr) {
		error("memory exhausted");
		abort();
	}

	fseek(ifp, syment->offset, 0);
	for (i = 0; i < numsyms; i++) {
		symptr[i].name = read_word(ifp);
		symptr[i].flags = read_word(ifp);
		symptr[i].value = read_word(ifp);
		symptr[i].areaname = read_word(ifp);
	}

	return (check_stream(ifp) != FRWERR ? symptr : NULL);
}

/*
 * read in the identification chunk
 */
char *
read_ident(FILE *ifp, struct chunkent *ident)
{
	if (idptr)
		free(idptr);
	idptr = malloc(ident->size);
	if (!idptr) {
		error("memory exhausted");
		abort();
	}

	fseek(ifp, (long)ident->offset, 0);
	fread(idptr, 1, (int)ident->size, ifp);

	return (check_stream(ifp) != FRWERR ? idptr : NULL);
}

/*
 * read in the AOF header
 */
struct aofhdr *
read_aofhdr(FILE *ifp, struct chunkent *hdrent)
{
	int i;
	struct areahdr *areahdr;

	if (aofhdr)
		free(aofhdr);
	aofhdr = (struct aofhdr *)malloc(hdrent->size);
	if (!aofhdr) {
		error("memory exhausted");
		abort();
	}

	/* read-in whole of AOF header */
	fseek(ifp, hdrent->offset, 0);
	aofhdr->filetype = read_word(ifp);
	aofhdr->version = read_word(ifp);
	aofhdr->numareas = read_word(ifp);
	aofhdr->numsyms = read_word(ifp);
	aofhdr->entryarea = read_word(ifp);
	aofhdr->entryoffset = read_word(ifp);
	areahdr = (struct areahdr *)(aofhdr + sizeof(struct aofhdr));
	for (i = 0; i < aofhdr->numareas; i++) {
		areahdr[i].name = read_word(ifp);
		areahdr[i].flags = read_word(ifp);
		areahdr[i].size = read_word(ifp);
		areahdr[i].numrelocs = read_word(ifp);
		areahdr[i].reserved = read_word(ifp);
	}
	return (check_stream(ifp) != FRWERR ? aofhdr : NULL);
}

/*
 * read in a relocation directive
 */
struct reloc *
read_reloc(FILE *ifp)
{
	static struct reloc reloc;

	reloc.offset = read_word(ifp);
	reloc.flags = read_word(ifp);
	return (check_stream(ifp) != FRWERR ? &reloc : NULL);
}
