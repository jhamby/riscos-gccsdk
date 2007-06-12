/* mkresfs - generate C code to add/remove files to/from ResourceFS
 * Copyright (c) 2006-2007 Rob Kendrick <rjek@rjek.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit
 * persons to whom the Software is furnished to do so, subject to the
 * following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT
 * OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
 * THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Purpose:
 *	Generates C source code to be compiled targeting RISC OS which
 *	creates and removes files to ResourceFS.  The link object created
 *	will export two symbols, which have the following prototype:
 *		void mkresfs_register(void);
 *		void mkresfs_deregister(void);
 *
 * Usage:
 * 	mkresfs [-o <outfile]> [-v] [-r <func>] [-d <func>] <dir>
 *
 *	-o	Specify output file.  stdout used if absent.
 *	-v	Verbose mode.  Outputs status information as it works
 *	-r	Register function name.  'mkresfs_register' used if absent
 *	-d	Deregister function name.  'mkresfs_reregister' used if absent
 *	<dir>	Directory to create ResourceFS data from.  If running under
 *		RISC OS, OS_File,5 is used to read file system meta data,
 *		otherwise stat() is used, and file type data is manufactured
 *		by using the usual ,xxx filename suffix hack.
 *
 * Limitations:
 *	1. Any file it can't calculate type information for gets set as 0xfff
 *	2. Local modified date information is not encoded in load/exec
 *	3. No facilities for non-file-typed load/exec pairs
 *	4. The register/deregister functions should return error status
 *	5. It can produce very large files that take a while and a lot of RAM
 *	   to compile on RISC OS.  Perhaps change to emitting words rather
 *	   than bytes to quarter parse complexity?
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <getopt.h>

#ifdef __riscos
#include <swis.h>
#ifndef OS_File
#define OS_File 0x8
#endif
#endif

#define MKRESFS_VERSION "1.01 (09 Jun 2007)"

typedef struct _resfs_entry {
	u_int32_t		loadaddr;
	u_int32_t		execaddr;
	u_int32_t		filesize;
	u_int8_t		filename[256];
	char			localfile[256];

	struct _resfs_entry	*next;
} resfs_entry;

static resfs_entry *list_head = NULL;
static resfs_entry *list_tail = NULL;

#define VERB(...) if (verbose) fprintf(stderr, __VA_ARGS__)

#define WWORD(x) wword(fh, &wcount, (x))

static inline void wword(FILE *fh, int *wcount, u_int32_t x)
{
	fprintf(fh, "0x%02x, 0x%02x, 0x%02x, 0x%02x, ",
		(x) & 0xff, ((x) >> 8) & 0xff, ((x) >> 16) & 0xff,
		((x) >> 24) & 0xff);

	*wcount += 1;

	if (*wcount > 2) {
		fprintf(fh, "\n  ");
		*wcount = 0;
	}

}

static void dump_data(FILE *fh, bool verbose)
{
	resfs_entry *n = list_head;
	int wcount;
	FILE *inf;

	while (n != NULL) {
		u_int32_t t;

		VERB("| Generating %s (%d bytes)...\n", n->filename,
			n->filesize);

		fprintf(fh, "  /* %s %08x/%08x %d bytes */\n  ",
			n->filename, n->loadaddr, n->execaddr, n->filesize);
		wcount = 0;

		/* offset to next file or 0 for last file.
		 * this is a bit sticky, as it has to skip over 20 bytes of
		 * fixed header, then the length of the filename + 1 + pad
		 * bytes to word-align, the size of the file, and more bytes
		 * to word-align.
		 */

		t = 20;				/* constant header size */
		t += strlen((char *)n->filename) + 1; /* fname + NUL */
		t = (t + 3) & ~3;		/* round to nearest word */
		t += (n->filesize + 3) & ~3; 	/* file size and pad */
		t += 4;				/* size of file + 1 */

		WWORD(t);			/* write the word out */

		WWORD(n->loadaddr);
		WWORD(n->execaddr);
		WWORD(n->filesize);
		WWORD(19);

		for (t = 0; t < strlen((char *)n->filename) + 1; t += 4)
			WWORD(*(u_int32_t *)(n->filename + t));

		WWORD(n->filesize + 4);		/* who knows why? */

		inf = fopen(n->localfile, "rb");
		if (inf == NULL) {
			fprintf(stderr, "Unable to open '%s' for reading.\n",
				n->localfile);
			exit(1);
		}

		while (!feof(inf)) {
			t = 0;
			if (fread(&t, 4, 1, inf) > 0)
				WWORD(t);
		}

		fclose(inf);

		fprintf(fh, "\n");
		n = n->next;
	}

	fprintf(fh, "  /* terminator */\n  0x00, 0x00, 0x00, 0x00\n");
}

static void add_entry(u_int32_t loadaddr, u_int32_t execaddr,
			u_int32_t filesize, const char *filename,
			const char *localfile)
{
	resfs_entry *e = calloc(sizeof(resfs_entry), 1);

	if (list_head == NULL)
		list_head = list_tail = e;
	else {
		list_tail->next = e;
		list_tail = e;
	}

	e->loadaddr = loadaddr;
	e->execaddr = execaddr;
	e->filesize = filesize;
	strncpy((char *)e->filename, filename, 255);
	strncpy(e->localfile, localfile, 255);
	e->next = NULL;
}


static int get_filetype(char *unixpath)
{
	/* returns an int for the file's type, and strips it from the filename
	 * if that is where it is encoded by inserting a NUL.
	 */

#ifdef __riscos
	char fbuf[256];
	u_int32_t objtype, loadaddr, execaddr, filesize, attrib;
	int ftype;

	__riscosify(unixpath, 0, 0, fbuf, 256, &ftype);

	_swi(OS_File, _INR(0, 1) | _OUT(0) | _OUTR(2, 5), 5, fbuf,
		&objtype, &loadaddr, &execaddr, &filesize, &attrib);

	if ((loadaddr & 0xfff00000) == 0xfff00000) {
	        return (loadaddr >> 8) & 0xfff;
	} else {
		return 0xffd;	/* we don't support "old" load/exec */
	}
#else
	size_t len = strlen(unixpath);
	unsigned int type = 0xfff;	/* default to plain text */

	if (len > 4 && unixpath[len - 4] == ',') {
		if (isxdigit(unixpath[len - 1]) &&
			isxdigit(unixpath[len - 2]) &&
			isxdigit(unixpath[len - 3])) {
			sscanf(unixpath + (len - 3), "%x", &type);
			unixpath[len - 4] = '\0';
		}
	}

	return type;
#endif
}

static void read_directory(bool verbose, const char *unixdir,
				const char *prefix)
{
	DIR *d = opendir(unixdir);
	struct dirent *e;
	struct stat s;

	if (d == NULL) {
		fprintf(stderr, "unable to open directory '%s'.\n", unixdir);
		return;
	}

	while ((e = readdir(d)) != NULL) {
		char unixpath[1024];	/* eugh, TODO: use pathinfo */
		if (strcmp(e->d_name, ".") == 0 ||
			strcmp(e->d_name, "..") == 0)
			continue;

		snprintf(unixpath, 1024, "%s/%s", unixdir, e->d_name);
		VERB("| Processing %s...\n", unixpath);
		stat(unixpath, &s);

		if (S_ISDIR(s.st_mode)) {
			read_directory(verbose, unixpath, prefix);
		} else {
			u_int32_t loadaddr = 0, execaddr = 0;
			u_int32_t filesize = s.st_size;
			char filelocal[1024], fileresfs[1024]; /* eugh again */
			char *fl = filelocal + strlen(prefix), *fr = fileresfs;

			strcpy(filelocal, unixpath);
			loadaddr = 0xfff00000 | (get_filetype(filelocal) << 8);

			/* create the resfs filename for this file by working
			 * through filelocal skipping what's in prefix, and
			 * swapping / and .  - get_filetype() has already
			 * dealt with any ,xxx for us.
			 */

			while (*fl == '/')
				fl++;	/* skip any leading slashes */

			do {
				switch (*fl) {
				case '/':
					*fr = '.';
					break;
				case '.':
					*fr = '/';
					break;
				default:
					*fr = *fl;
					break;
				}
				fl++;
				fr++;
			} while (*fl != '\0');

			*fr = '\0';

			add_entry(loadaddr, execaddr, filesize, fileresfs,
					unixpath);
		}
	}

	closedir(d);
}

static void generate(FILE *fh, bool verbose, const char *regfunc,
			const char *deregfunc, const char *dir)
{
	/* output header */

	fprintf(fh, "/* Generated by mkresfs %s, do not edit by hand!\n",
		MKRESFS_VERSION);
	fprintf(fh, " */\n\n");
	fprintf(fh, "#include <swis.h>\n#include <stdio.h>\n");
	fprintf(fh, "#ifndef ResourceFS_RegisterFiles\n");
	fprintf(fh, "#define ResourceFS_RegisterFiles 0x41b40\n");
	fprintf(fh, "#define ResourceFS_DeregisterFiles 0x41b41\n");
	fprintf(fh, "#endif\n\n");

	/* recurse directory structure building up entries in our list */

	read_directory(verbose, dir, dir);

	/* output list */

	fprintf(fh, "static const unsigned char payload[] = {\n");
	dump_data(fh, verbose);
	fprintf(fh, "};\n\n");

	/* output functions */
	fprintf(fh, "void %s(void)\n", regfunc);
	fprintf(fh, "{\n");
	fprintf(fh, "  _swi(ResourceFS_RegisterFiles, _IN(0), payload);\n");
	fprintf(fh, "}\n\n");
	fprintf(fh, "void %s(void)\n", deregfunc);
	fprintf(fh, "{\n");
	fprintf(fh, "  _swi(ResourceFS_DeregisterFiles, _IN(0), payload);\n");
	fprintf(fh, "}\n\n");

	VERB("| Done.\n");

	/* TODO: don't be lazy and let the OS garbage collect: free the list */
}

/* -- front end ----------------------------------------------------------- */

static void usage(const char *progname)
{
	printf("mkresfs %s - Rob Kendrick <rjek@rjek.com>\n", MKRESFS_VERSION);
	printf("usage: %s [-o <outfile]> [-v] [-r <regfunc>] [-d <deregfunc>]"
		" <dir>\n", progname);
	puts(" -o    Specify output file. stdout used if absent.");
	puts(" -v    Verbose mode. Outputs status information as it works.");
	puts(" -r    Register function name. mkresfs_register default.");
	puts(" -d    Deregister function name. mkresfs_reregister default.");
	puts(" <dir> Directory to create ResourceFS data from.");
}

int main(int argc, char *argv[])
{
	const char *s_opts = "hvr:d:o:";
	int o;
	FILE *fh = stdout;

	/* parsed options */
	bool opt_verbose = false;
	char *opt_outfile = NULL;
	char *opt_regfunc = "mkresfs_register";
	char *opt_deregfunc = "mkresfs_deregister";
	char *opt_indir = NULL;

	while ((o = getopt(argc, argv, s_opts)) != -1) {
		switch (o) {
		case 'h':
			usage(argv[0]);
			exit(0);
			break;
		case 'v':
			opt_verbose = true;
			break;
		case 'r':
			opt_regfunc = optarg;
			break;
		case 'd':
			opt_deregfunc = optarg;
			break;
		case 'o':
			opt_outfile = optarg;
			break;
		default:
			exit(1);
			break;
		}
	}

	if (optind == argc) {
		fprintf(stderr, "%s: no directory specified.\n", argv[0]);
		exit(1);
	}

	opt_indir = argv[optind];

	if (opt_outfile != NULL) {
		fh = fopen(opt_outfile, "w");
		if (fh == NULL) {
			fprintf(stderr, "%s: unable to open %s for writing.",
				argv[0], opt_outfile);
			exit(1);
		}
	}

	generate(fh, opt_verbose, opt_regfunc, opt_deregfunc, opt_indir);

	if (opt_outfile != NULL)
		fclose(fh);

	return 0;
}
