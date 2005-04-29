/*
 * file input/output
 *
 * Andy Duplain, BT Customer Systems, Brighton, UK.  duplain@btcs.bt.co.uk
 */

#include <stdio.h>
#include "decaof.h"
#include "cproto.h"
#include "error.h"

#if defined(BSD42) || defined(SYSV2)
extern char *malloc P__((unsigned len));
#else
#include <malloc.h>
#endif

/*
 * check for EOF or write/read errors on stream.
 */
Ferror
check_stream(fp)
	FILE *fp;
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
read_byte(ifp)
	FILE *ifp;
{
	return ((Byte)getc(ifp));
}

/*
 * read a little-endian 2-byte halfword from the input stream.
 */
Halfword
read_halfword(ifp)
	FILE *ifp;
{
	union {
		Halfword h;
		Byte b[sizeof(Halfword)];
	} ret;

#if defined(LITTLE_ENDIAN)
	fread((char *)&ret.h, 1, sizeof(Halfword), ifp);
#else
	ret.b[HALFWORD0] = read_byte(ifp);
	ret.b[HALFWORD1] = read_byte(ifp);
#endif
	return (ret.h);
}

/*
 * read a little-endian 4-byte word from the input stream.
 */
Word
read_word(ifp)
	FILE *ifp;
{
	union {
		Word w;
		Byte b[sizeof(Word)];
	} ret;

#if defined(LITTLE_ENDIAN)
	fread((char *)&ret.w, 1, sizeof(Word), ifp);
#else
	ret.b[WORD0] = read_byte(ifp);
	ret.b[WORD1] = read_byte(ifp);
	ret.b[WORD2] = read_byte(ifp);
	ret.b[WORD3] = read_byte(ifp);
#endif
	return (ret.w);
}

/*
 * read in the chunk header
 */
struct chunkhdr *
read_chunkhdr(ifp)
	FILE *ifp;
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
free_chunk_memory(ptr)
	char *ptr;
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
read_chunkents(ifp, hdr)
	FILE *ifp;
	struct chunkhdr *hdr;
{
	register i;

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
read_stringtab(ifp, strent)
	FILE *ifp;
	struct chunkent *strent;
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
read_symboltab(ifp, syment, numsyms)
	FILE *ifp;
	struct chunkent *syment;
	int numsyms;
{
	register i;

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
read_ident(ifp, ident)
	FILE *ifp;
	struct chunkent *ident;
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
read_aofhdr(ifp, hdrent)
	FILE *ifp;
	struct chunkent *hdrent;
{
	register i;
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
read_reloc(ifp)
	FILE *ifp;
{
	static struct reloc reloc;

	reloc.offset = read_word(ifp);
	reloc.flags = read_word(ifp);
	return (check_stream(ifp) != FRWERR ? &reloc : NULL);
}
