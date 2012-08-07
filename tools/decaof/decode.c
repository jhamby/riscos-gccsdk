/*
 * decode an AOF file
 *
 * Copyright (c) 1992 Andy Duplain, andy.duplain@dsl.pipex.com
 * Copyright (c) 2005-2006 GCCSDK Developers
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#include "config.h"

#include <stdio.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

#include "decaof.h"
#include "decode.h"
#include "error.h"
#include "io.h"
#include "main.h"
#include "misc.h"

static struct chunkhdr *hdr;
static struct chunkent *ents, *ent;
static struct aofhdr *aofhdr;
static struct areahdr *areahdrs;
static struct symbol *symboltab;
static char *stringtab;
static long area_offset;
static Word symboltab_size;

static void print_area (FILE *ifp, struct areahdr *areahdr, Word offset, Word reloff);
static const char *string (Word offset);
static const char *symname (Word offset);
static const char *areaname (Word offset);

void
decode (void)
{
	int i;
	Word offset, reloff;
	FILE *ifp;
	const char *filename;
	const char *cptr;

	while (nfiles--) {
		filename = *files++;
		ifp = fopen(filename, "r");
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
			puts("\n** No String table");

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
			puts("\n** No Identification chunk");

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
			switch (aofhdr->filetype) {
				case 0xc5e2d080:
					cptr = "Relocatable object format";
					break;
				case 0xc5e2d081:
					cptr = "AOF image type 1";
					break;
				case 0xc5e2d083:
					cptr = "AOF image type 2";
					break;
				case 0xc5e2d087:
					cptr = "AOF image type 3";
					break;
				default:
					cptr = "unknown image type";
					break;
			}
			puts(cptr);

			printf("version %d\n", aofhdr->version);
			i = aofhdr->numareas;
			printf("%d area%s\n", i, i == 1 ? "" : "s");
			i = aofhdr->numsyms;
			printf("%d symbol%s\n", i, i == 1 ? "" : "s");

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
			areahdrs = (struct areahdr *)&aofhdr[1];
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
			puts("\n** No AOF header");

		if (symtab) {
			if (aofhdr) {
				if (aofhdr->numsyms)
					puts("\n** Symbol table:");
				for (i = 0; i < aofhdr->numsyms; i++) {
					int flags = symboltab[i].flags;
					printf("%-16s (%02x) ", string(symboltab[i].name), flags);

					switch (flags & 0x3) {
						case 0x01:
							fputs("local", stdout);
							break;
						case 0x02:
							fputs("extern", stdout);
							break;
						case 0x03:
							fputs("global", stdout);
							break;
						default:
							fputs("unknown-type", stdout);
							break;
					}
					if ((flags & (1<<2)) && flags & (1<<0))
						fputs(", constant", stdout);
					if ((flags & (1<<3)) && !(flags & (1<<0)))
						fputs(", case-insensitive", stdout);
					if ((flags & (1<<4)) && ((flags & 0x03) == 0x02))
						fputs(", weak", stdout);
					if ((flags & (1<<5)) && ((flags & 0x03) == 0x03))
						fputs(", strong", stdout);
					if ((flags & (1<<6)) && ((flags & 0x03) == 0x02))
						fputs(", common", stdout);
					if (flags & (1<<8))
						fputs(", cadatum", stdout);
					if (flags & (1<<9))
						fputs(", fpargs", stdout);
					if (flags & (1<<11))
						fputs(", leaf", stdout);
					if (flags & (1<<12))
						fputs(", thumb", stdout);
					if (flags & ((1<<0) | (1<<6))) {
						if (flags & ((1<<2) | (1<<6)))
							printf(" = 0x%08x", symboltab[i].value);
						else
							printf(" at \"%s\" + 0x%06x", string(symboltab[i].areaname), symboltab[i].value);
					}
					putchar('\n');
				}
			} else
				puts("\n** No Symbol table");
		}

		if (strtab && *(Word *)stringtab) {
			puts("\n** String table:");
			offset = 4;
			while ((cptr = string(offset)) != NULL) {
				printf("%06x: %s\n", offset, cptr);
				offset += strlen(cptr) + 1;
			}
		}

next_file:
		free_chunk_memory(aofhdr);
		free_chunk_memory(symboltab);
		free_chunk_memory(stringtab);
		free_chunk_memory(ents);
		fclose(ifp);
	}
}

/*
 * print each AOF area...
 */
static void
print_area(FILE *ifp, struct areahdr *areahdr, Word offset, Word reloff)
{
	Word ubits; /* the unknown bits */
	int isCode, isData;
	Word flags = areahdr->flags;

	printf("\n** Area (0x%06x) \"%s\", aligned at %d byte%s, ",
	       areahdr->flags >> 8,
	       string(areahdr->name),
	       1 << (areahdr->flags & 0xFF),
	       (areahdr->flags & 0xFF) ? "s" : "");
	if (flags & AREA_DEBUG) {
		fputs("[debug] ", stdout);
		isCode = isData = 0;
		ubits = flags & AREA_CODE;
	} else {
		if (flags & AREA_CODE) {
			fputs("[code] ", stdout);
			isCode = 1; isData = 0;
		} else {
			fputs("[data] ", stdout);
			isData = 1; isCode = 0;
		}
		ubits = 0;
	}
	if (flags & AREA_ABS)
		fputs("[abs] ", stdout);
	if (flags & AREA_COMMONDEF)
		fputs("[commdef] ", stdout);
	if (flags & AREA_COMMONREF)
		fputs("[commref] ", stdout);
	if (flags & AREA_UDATA)
		fputs("[noinit] ", stdout);
	if (flags & AREA_READONLY)
		fputs("[readonly] ", stdout);
	if (flags & AREA_PIC)
		fputs("[pic] ", stdout);
	if (flags & AREA_32BITAPCS) {
		if (isCode)
			fputs("[32bit] ", stdout);
		else
			ubits |= AREA_32BITAPCS;
	}
	if (flags & AREA_REENTRANT) {
		if (isCode)
			fputs("[reentrant] ", stdout);
		else
			ubits |= AREA_REENTRANT;
	}
	if (flags & AREA_EXTFPSET) {
		if (isCode)
			fputs("[extfpset] ", stdout);
		else
			ubits |= AREA_EXTFPSET;
	}
	if (flags & AREA_NOSTACKCHECK) {
		if (isCode)
			fputs("[nostackcheck] ", stdout);
		else
			ubits |= AREA_NOSTACKCHECK;
	}
	if (flags & AREA_BASED) { /* is same test for AREA_THUMB */
		if (isCode) {
			fputs("[thumb] ", stdout);
			ubits |= flags & AREA_MASKBASEREGS;
		} else if (isData)
			printf("[based, reg %d] ", (flags & AREA_MASKBASEREGS)>>24);
		else
			ubits |= flags & (AREA_BASED | AREA_MASKBASEREGS);
	}
	if (flags & AREA_STUBDATA) { /* is same test for AREA_HALFWORD */
		if (isCode)
			fputs("[halfword] ", stdout);
		else if (isData)
			fputs("[stubdata] ", stdout);
		else
			ubits |= AREA_STUBDATA;
	}
	if (flags & AREA_INTERWORK) {
		if (isCode)
			fputs("[interwork] ", stdout);
		else
			ubits |= AREA_INTERWORK;
	}
	if (flags & AREA_SOFTFLOAT)
		fputs("[soft-float] ", stdout);
	if (flags & AREA_LINKONCE)
		fputs("[linkonce] ", stdout);
	ubits |= flags & AREA_UNKNOWNBITS;
	if (ubits)
		printf("[unknown bits %06x00] ", (flags & AREA_UNKNOWNBITS) >> 8);

	printf("\nsize %d byte%s, %d relocation%s\n",
	       areahdr->size,
	       (areahdr->size == 1) ? "" : "s",
	       areahdr->numrelocs,
	       (areahdr->numrelocs == 1) ? "" : "s");

	if (area_contents) {
		int cols = 0, size = areahdr->size;
		Word area_off;

		fseek(ifp, area_offset + offset, 0);
		fputs("contents:\n000000: ", stdout);
		area_off = 0;
		while (area_off < size) {
			printf("%08x", read_word(ifp));
			area_off += 4;
			if (++cols == 8) {
				printf("\n%06x: ", area_off);
				cols = 0;
			} else
				putchar(' ');
		}
		if (cols)
			putchar('\n');
	}

	if (reloc_dir && areahdr->numrelocs) {
		struct reloc *reloc;
		Word numrelocs;

		fseek(ifp, area_offset + reloff, 0);
		puts("relocations:");
		for (numrelocs = areahdr->numrelocs; numrelocs; numrelocs--) {
			enum {unknown, type1, type2} rtype;
			const char *cptr;


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
			default: /* to satisfy compiler */
			case 0x03:
				cptr = "unknown-field-type at";
				break;
			}
			printf("%s 0x%06x ", cptr, reloc->offset);

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
static const char *
string(Word offset)
{
	if (!stringtab || offset < 4 || offset >= *(Word *)stringtab)
		return NULL;
	return (char *)(stringtab + offset);
}

/*
 * return a pointer to a symbol name in the symbol table
 */
static const char *
symname(Word offset)
{
	struct symbol *sym;

	if (!symboltab || offset > symboltab_size)
		return NULL;
	sym = (struct symbol *)(symboltab + offset);
	return string(sym->name);
}

/*
 * return a pointer to an area name, given the area's offset
 * into the OBJ_AREA.
 */
static const char *
areaname(Word offset)
{
	Word aoff;
	int i;

	if (!aofhdr || !areahdrs)
		return NULL;
	for (i = 0, aoff = 0; i < aofhdr->numareas; i++)
		if (aoff == offset)
			return string(areahdrs[i].name);
		else if (!(areahdrs[i].flags & AREA_UDATA))
			aoff += areahdrs[i].size + (sizeof(struct reloc) * areahdrs[i].numrelocs);
	return NULL;
}
