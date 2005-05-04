/*
 * decode an AOF file
 *
 * Andy Duplain, BT Customer Systems, Brighton, UK.  duplain@btcs.bt.co.uk
 */

#include <stdio.h>
#ifdef BSD42
#include <strings.h>
#else
#include <string.h>
#endif
#include "decaof.h"
#include "cproto.h"
#include "io.h"
#include "main.h"
#include "misc.h"
#include "error.h"

static struct chunkhdr *hdr;
static struct chunkent *ents, *ent;
static struct aofhdr *aofhdr;
static struct areahdr *areahdrs;
static struct symbol *symboltab;
static char *stringtab;
static long area_offset;
static Word symboltab_size;

static char *cptr;

static void print_area P__((FILE *ifp, struct areahdr *areahdr, Word offset, Word reloff));
static char *string P__((Word offset));
static char *symname P__((Word offset));
static char *areaname P__((Word offset));

int
decode()
{
	register i;
	Word offset, reloff;
	FILE *ifp;
	char *filename;

	while (nfiles--) {
		filename = *files++;
		ifp = fopen(filename, R_OPENMODE);
		if (!ifp) {
			error("unable to open file \"%s\"", filename);
			continue;
		};

		hdr = NULL;
		ents = NULL;
		aofhdr = NULL;
		areahdrs = NULL;
		stringtab = NULL;
		symboltab = NULL;

		/* read chunk header */
		hdr = read_chunkhdr(ifp);
		if (!hdr) {
			error("reading file \"%s\"", filename);
			goto next_file;
		}
		if (hdr->chunkfileid != 0xc3cbc6c5) {
			error("file \"%s\" is not a chunk file", filename);
			goto next_file;
		}

		/* read chunk entries in */
		ents = read_chunkents(ifp, hdr);
		if (!ents) {
			error("reading chunk entries for file \"%s\"", filename);
			goto next_file;
		}

		printf("** File:\n%s\n", filename);

		/* read string table */
		ent = find_ent(hdr, ents, "OBJ_STRT");
		if (ent) {
			stringtab = read_stringtab(ifp, ent);
			if (!stringtab) {
				error("reading string table for file \"%s\"", filename);
				goto next_file;
			}
		} else
			puts("\n** no string table");

		/* read and print identification string */
		ent = find_ent(hdr, ents, "OBJ_IDFN");
		if (ent) {
			char *idstr;
			idstr = read_ident(ifp, ent);
			if (!idstr) {
				error("reading identification for file \"%s\"", filename);
				goto next_file;
			}
			printf("\n** Identification:\n%s\n", idstr);
			free_chunk_memory(idstr);	/* not needed */
		} else
			puts("\n** no identification chunk");

		/* find file offset of OBJ_AREA (for later use) */
		ent = find_ent(hdr, ents, "OBJ_AREA");
		area_offset = ent->offset;

		/* read-in AOF header */
		ent = find_ent(hdr, ents, "OBJ_HEAD");
		if (ent) {
			aofhdr = read_aofhdr(ifp, ent);
			if (!aofhdr) {
				error("reading AOF header for file \"%s\"", filename);
				goto next_file;
			}
			puts("\n** AOF Header:");
			if (aofhdr->filetype == 0xc5e2d080)
				cptr = "Relocatable object format";
			else if (aofhdr->filetype == 0xc5e2d081)
				cptr = "AOF image type 1";
			else if (aofhdr->filetype == 0xc5e2d083)
				cptr = "AOF image type 2";
			else if (aofhdr->filetype == 0xc5e2d087)
				cptr = "AOF image type 3";
			else
				cptr = "unknown image type";
			puts(cptr);

			printf("version %ld\n", aofhdr->version);
			i = aofhdr->numareas;
			printf("%ld area%s\n", i, i == 1 ? "" : "s");
			i = aofhdr->numsyms;
			printf("%ld symbol%s\n", i, i == 1 ? "" : "s");

			/* read in the symbol table, if any */
			if (aofhdr->numsyms) {
				ent = find_ent(hdr, ents, "OBJ_SYMT");
				if (ent) {
					symboltab = read_symboltab(ifp, ent, aofhdr->numsyms);
					if (!symboltab) {
						error("reading symbol table for file \"%s\"", filename);
						goto next_file;
					}					
					symboltab_size = ent->size;
				}
			}

			/* decode each of the areas */
			areahdrs = (struct areahdr *)(aofhdr + sizeof(struct aofhdr));
			offset = reloff = 0;
			for (i = 0; i < aofhdr->numareas; i++) {
				Byte flags = (areahdrs[i].flags >> 8) & 0xff;

				if (!(flags & AREA_UDATA))
					reloff = offset + areahdrs[i].size;
				print_area(ifp, &areahdrs[i], offset, reloff);
				if (!(flags & AREA_UDATA))
					offset = reloff + (areahdrs[i].numrelocs * sizeof(struct reloc));
			}
		} else
			puts("\n** no AOF header");

		if (symtab && aofhdr->numsyms) {
			puts("\n** Symbol table:");
			for (i = 0; i < aofhdr->numsyms; i++) {
				Byte flags = symboltab[i].flags & 0xff;
				printf("%-16s", string(symboltab[i].name));

				switch (flags & 0x3) {
					case 0x01:
						fputs("local ", stdout);
						break;
					case 0x02:
						fputs("extern ",
						    stdout);
						break;
					case 0x03:
						fputs("global ", stdout);
						break;
					default:
						fputs("unknown-type ", stdout);
						break;
				}				
				if ((flags & (1<<2)) && flags & (1<<0))
					fputs("constant ", stdout);
				if ((flags & (1<<3)) && !(flags & (1<<0)))
					fputs("case-insensitive ", stdout);
				if ((flags & (1<<4)) && ((flags & 0x03) == 0x02))
					fputs("weak ", stdout);
				if ((flags & (1<<5)) && ((flags & 0x03) == 0x03))
					fputs("strong ", stdout);
				if ((flags & (1<<6)) && ((flags & 0x03) == 0x02))
					fputs("common ", stdout);
				if ((flags & (1<<0)) || (flags & (1<<6))) {
					if (flags & (1<<2))
						printf("= 0x%08lx", symboltab[i].value);
					else
						printf("at \"%s\" + 0x%06lx", string(symboltab[i].areaname), symboltab[i].value);
				}
				putchar('\n');
			}
		}

		if (strtab && *(Word *)stringtab) {
			puts("\n** String table:");
			offset = 4;
			while (cptr = string(offset)) {
				printf("%06lx: %s\n", offset, cptr);
				offset += strlen(cptr) + 1;
			}
		}

next_file:
		free_chunk_memory((char *)aofhdr);
		free_chunk_memory((char *)symboltab);
		free_chunk_memory((char *)stringtab);
		free_chunk_memory((char *)ents);
		fclose(ifp);
	}

	return (0);
}

/*
 * print each AOF area...
 */
static void
print_area(ifp, areahdr, offset, reloff)
	FILE *ifp;
	struct areahdr *areahdr;
	Word offset, reloff;
{
	Word flags = areahdr->flags >> 8;

	printf("\n** Area (0x%06x) \"%s\" ", areahdr->flags, string(areahdr->name));
	if (flags & AREA_DEBUG)
		fputs("[debug] ", stdout);
	else {
		if (flags & AREA_CODE)
			fputs("[code] ", stdout);
		else
			fputs("[data] ", stdout);
	}
	if (flags & AREA_COMMONDEF)
		fputs("[commdef] ", stdout);
	if (flags & AREA_READONLY)
		fputs("[readonly] ", stdout);
	if (flags & AREA_UDATA)
		fputs("[noinit] ", stdout);
	if (flags & AREA_32BITAPCS)
		fputs("[32bit] ", stdout);
	if (flags & AREA_SOFTFLOAT)
		fputs("[soft-float] ", stdout);
	if (flags & AREA_LINKONCE)
		fputs("[linkonce] ", stdout);

	printf("\nsize %ld byte%s, %ld relocation%s\n", areahdr->size, areahdr->size == 1 ? "" : "s", areahdr->numrelocs, areahdr->numrelocs == 1 ? "" : "s");

	if (area_contents) {
		register cols = 0, size = areahdr->size;
		register Word area_off;

		fseek(ifp, area_offset + offset, 0);
		fputs("contents:\n000000: ", stdout);
		area_off = 0;
		while (area_off < size) {
			printf("%l08x", read_word(ifp));
			area_off += 4;
			if (++cols == 8) {
				printf("\n%06lx: ", area_off);
				cols = 0;
			} else
				putchar(' ');
		}
		if (cols)
			putchar('\n');
	}

	if (reloc_dir && areahdr->numrelocs) {
		register struct reloc *reloc;
		register Word numrelocs;

		fseek(ifp, area_offset + reloff, 0);
		puts("relocations:");
		for (numrelocs = areahdr->numrelocs; numrelocs; numrelocs--) {
			enum {unknown, type1, type2} rtype;
			int shift;

			reloc = read_reloc(ifp);
			if (!reloc) {
				error("reading relocation directive");
				return;
			}

			if ((reloc->flags & 0xfff00000) == 0x00000000)
				rtype = type1;
			else if ((reloc->flags & 0xf0000000) == 0x80000000)
				rtype = type2;
			else {
				puts("** unknown relocation type");
				continue;
			}

			switch ((reloc->flags >> (rtype == type1 ? 16 : 24)) & 0x03) {
			case 0x00:
				cptr = "byte at";
				break;
			case 0x01:
				cptr = "halfword at";
				break;
			case 0x02:
				cptr = "word at";
				break;
			case 0x03:
				cptr = "unknown-field-type at";
				break;
			}
			printf("%s 0x%06lx ", cptr, reloc->offset);

			if (rtype == type1) {
				fputs("type-1 ", stdout);
				switch ((reloc->flags >> 18) & 0x03) {
				case 0x01:
				case 0x03:
					/* pc-relative (always symbol) */
					printf("pc-relative to symbol \"%s\"", symname(reloc->flags & 0xffff));
					break;
				case 0x02:
					/* additive symbol */
					printf("displaced by symbol \"%s\"", symname(reloc->flags & 0xffff));
					break;
				case 0x00:
					/* additive internal */
					fputs("displaced by this area", stdout);
					break;
				}
			} else if (rtype == type2) {
				fputs("type-2 ", stdout);
				switch ((reloc->flags >> 26) & 0x03) {
				case 0x00:
					/* additive internal */
					printf("displaced by area \"%s\"", areaname(reloc->flags & 0xffffff));
					break;
				case 0x01:
					/* pc-relative internal */
					printf("pc-relative to area \"%s\"", areaname(reloc->flags & 0xffffff));
					break;

				case 0x02:
					/* additive symbol */
					printf("displaced by symbol \"%s\"", symname(reloc->flags & 0xffffff));
					break;

				case 0x03:
					/* pc-relative symbol */
					printf("pc-relative to symbol \"%s\"", symname(reloc->flags & 0xffffff));
					break;
				}
			} else
				fputs("type-unknown", stdout);
			putchar('\n');
		}
	}
}

/*
 * return a pointer to a string in the string table
 */
static char *
string(offset)
	Word offset;
{
	if (!stringtab || offset < 4 || offset >= *(Word *)stringtab)
		return (NULL);
	return ((char *)(stringtab + offset));
}

/*
 * return a pointer to a symbol name in the symbol table
 */
static char *
symname(offset)
	Word offset;
{
	register struct symbol *sym;

	if (!symboltab || offset > symboltab_size)
		return (NULL);
	sym = (struct symbol *)(symboltab + offset);
	return (string(sym->name));
}

/*
 * return a pointer to an area name, given the area's offset
 * into the OBJ_AREA.
 */
static char *
areaname(offset)
	Word offset;
{
	register Word aoff;
	register i;

	if (!aofhdr || !areahdrs)
		return (NULL);
	for (i = 0, aoff = 0; i < aofhdr->numareas; i++)
		if (aoff == offset)
			return (string(areahdrs[i].name));
		else if (!(areahdrs[i].flags & AREA_UDATA))
			aoff += areahdrs[i].size + (sizeof(struct reloc) * areahdrs[i].numrelocs);
	return (NULL);
}
