/* mkresfs - generate C code to add/remove files to/from ResourceFS
 * Copyright (c) 2006-2007 Rob Kendrick <rjek@rjek.com>
 * Copyright (c) 2007-2008 GCCSDK Developers
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
 *	SharedCLibrary:
 *		_kernel_oserror *mkresfs_register (void);
 *		_kernel_oserror *mkresfs_deregister (void);
 *		_kernel_oserror *mkresfs_reregister (int callback, int workspace);
 *	OSLib:
 *		os_error *mkresfs_register (void);
 *		os_error *mkresfs_deregister (void);
 *		os_error *mkresfs_reregister (int callback, int workspace);
 *
 * Usage:
 * 	mkresfs [-o <outfile]> [-p <header>] [-v] [-z] [-r <func>] [-d <func>] [-s <func>] <dir>
 *
 *	-o	Specify output file.  stdout used if absent.
 *	-p	Filename to create a C header file with function prototypes in.
 *		Not created if absent.
 *	-v	Verbose mode.  Outputs status information as it works.
 *      -z      Use OSLib prototypes instead of SharedCLibrary ones.
 *	-r	Register function name <func> with prototype:
 *		  _kernel_oserror *<func> (void);
 *              or
 *                os_error *<func> (void);
 *		Function name 'mkresfs_register' is used if absent.
 *	-d	Deregister function name <func> with prototype:
 *		  _kernel_oserror *<func> (void);
 *              or
 *                os_error *<func> (void);
 *		Function name 'mkresfs_reregister' is used if absent.
 *	-s	Reregister function name <func> (when ResourceFS restarts)
 *              with prototype:
 *		  _kernel_oserror *<func> (int callback, int workspace);
 *              or
 *		  os_error *<func> (int callback, int workspace);
 *		Function name 'mkresfs_reregister' is used if absent.
 *	<dir>	Directory to create ResourceFS data from.  If running under
 *		RISC OS, OS_File,5 is used to read file system meta data,
 *		otherwise stat() is used, and file type data is manufactured
 *		by using the usual ,xxx filename suffix hack.
 *		Files or directories starting with '/' (RISC OS) or '.'
 *		(other OS) will be ignored.
 *
 * Limitations:
 *	1. Any file it can't calculate type information for gets set as 0xfff
 *	2. Local modified date information is not encoded in load/exec
 *	3. No facilities for non-file-typed load/exec pairs
 *	4. It can produce very large files that take a while and a lot of RAM
 *	   to compile on RISC OS.  Perhaps change to emitting words rather
 *	   than bytes to quarter parse complexity?
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <getopt.h>

#ifdef __riscos
#  include <swis.h>
#  include <unixlib/local.h>
#  ifndef OS_File
#    define OS_File 0x8
#  endif
#endif

#if HAVE_CONFIG_H
#  include "config.h"
#endif

#ifdef PACKAGE_VERSION
#  define MKRESFS_VERSION PACKAGE_VERSION " (" __DATE__ ")"
#else
#  define MKRESFS_VERSION "Development version (" __DATE__ ")"
#endif

typedef struct _resfs_entry {
	u_int32_t		loadaddr;
	u_int32_t		execaddr;
	u_int32_t		filesize;
	char			filename[256];
	char			localfile[256];

	struct _resfs_entry	*next;
} resfs_entry;

static resfs_entry *list_head = NULL;
static resfs_entry *list_tail = NULL;

#define VERB(...) if (verbose) fprintf(stderr, __VA_ARGS__)

#define WWORD(x) wword(fh, &wcount, (x))

static inline void
wword(FILE *fh, int *wcount, u_int32_t x)
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

static int
dump_data(FILE *fh, bool verbose)
{
	resfs_entry *n;
	int wcount;
	FILE *inf;

	for (n = list_head; n != NULL; n = n->next) {
		u_int32_t t;
		size_t filename_len = strlen(n->filename);

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
		t += filename_len + 1;		/* fname + NUL */
		t = (t + 3) & ~3;		/* round to nearest word */
		t += (n->filesize + 3) & ~3; 	/* file size and pad */
		t += 4;				/* size of file + 1 */

		WWORD(t);			/* write the word out */

		WWORD(n->loadaddr);
		WWORD(n->execaddr);
		WWORD(n->filesize);
		WWORD(19);			/* write attributes */

		for (t = 0; t < filename_len + 1; t += 4)
			WWORD(*(u_int32_t *)(n->filename + t));

		WWORD(n->filesize + 4);		/* who knows why? */

		inf = fopen(n->localfile, "rb");
		if (inf == NULL) {
			fprintf(stderr, "Unable to open '%s' for reading.\n",
				n->localfile);
			return EXIT_FAILURE;
		}

		while (!feof(inf)) {
			t = 0;
			if (fread(&t, 1, sizeof(t), inf) > 0)
				WWORD(t);
		}

		fclose(inf);

		fprintf(fh, "\n");
	}

	fprintf(fh, "  /* terminator */\n  0x00, 0x00, 0x00, 0x00\n");
	return EXIT_SUCCESS;
}

static int
add_entry(u_int32_t loadaddr, u_int32_t execaddr,
	  u_int32_t filesize, const char *filename,
	  const char *localfile)
{
	resfs_entry *e = calloc(sizeof(resfs_entry), 1);
	if (e == NULL)
		return EXIT_FAILURE;

	if (list_head == NULL)
		list_head = list_tail = e;
	else {
		list_tail->next = e;
		list_tail = e;
	}

	e->loadaddr = loadaddr;
	e->execaddr = execaddr;
	e->filesize = filesize;
	strncpy(e->filename, filename, sizeof(e->filename)-1);
	// Not needed because we used calloc():
	// e->filename[sizeof(e->filename)-1] = '\0';
	strncpy(e->localfile, localfile, sizeof(e->localfile)-1);
	// Not needed because we used calloc():
	// e->localfile[sizeof(e->localfile)-1] = '\0';
	e->next = NULL;

	return EXIT_SUCCESS;
}


/* Given the path to one of the ResourceFS file, return its load- and
   exec address.  */
static int
get_loadexecaddress(char *unixpath, int *loadaddr, int *execaddr)
{
#ifdef __riscos
	char fbuf[256];
	u_int32_t objtype, filesize, attrib;
	int ftype;
	_kernel_oserror *err;

	__riscosify(unixpath, 0, 0, fbuf, sizeof(fbuf), &ftype);

	if ((err = _swix(OS_File, _INR(0, 1) | _OUT(0) | _OUTR(2, 5), 5, fbuf,
			 &objtype, loadaddr, execaddr, &filesize, &attrib)) != NULL) {
		fprintf(stderr, "Error when retrieving file information (%s).\n", err->errmess);
		return EXIT_FAILURE;
	}

	if ((*loadaddr & 0xfff00000) != 0xfff00000) {
		/* we don't support "old" load/exec */
		*loadaddr = 0xfffffd00;
		*execaddr = 0;
	}
#else
	size_t len = strlen(unixpath);
	unsigned int type;
	struct stat buf;
	uint64_t rotime;

	/* Determine RISC OS time:  */
	if (stat (unixpath, &buf)) {
		fprintf(stderr, "Error when retrieving file information.\n");
		return EXIT_FAILURE;
	}
	/* 0x336e996a00 is number of centiseconds between Unix and RISC OS
	   start.  */
	rotime = 100ULL * buf.st_mtime + 0x336e996a00ULL;

	if (len > 4 && unixpath[len - 4] == ','
	    && isxdigit(unixpath[len - 1])
	    && isxdigit(unixpath[len - 2])
	    && isxdigit(unixpath[len - 3])) {
		sscanf(unixpath + (len - 3), "%x", &type);
		unixpath[len - 4] = '\0';
	}
	else
		type = 0xfff; /* default to plain text */

	*loadaddr = 0xfff00000 | (type << 8) | ((rotime >> 32) & 0xFF);
	*execaddr = rotime & 0xFFFFFFFF;
#endif
	return EXIT_SUCCESS;
}

static int
read_directory(bool verbose, const char *unixdir, const char *prefix)
{
	DIR *d = opendir(unixdir);
	struct dirent *e;
	struct stat s;

	if (d == NULL) {
		fprintf(stderr, "unable to open directory '%s'.\n", unixdir);
		return EXIT_FAILURE;
	}

	while ((e = readdir(d)) != NULL) {
		char unixpath[1024];	/* eugh, TODO: use pathinfo */

		/* Skip all objects starting with a '.' */
		if (e->d_name[0] == '.')
			continue;

		/* Skip current and parent directory. This code is no longer
		   relevant as we're already skipping all objects starting
		   with a '.'. */
#if 0
		if (strcmp(e->d_name, ".") == 0 ||
			strcmp(e->d_name, "..") == 0)
			continue;
#endif

		snprintf(unixpath, sizeof(unixpath), "%s/%s", unixdir, e->d_name);
		VERB("| Processing %s...\n", unixpath);
		if (stat(unixpath, &s)) {
			fprintf(stderr, "Failed to get info on <%s>.\n", unixpath);
			return EXIT_FAILURE;
		}

		if (S_ISDIR(s.st_mode)) {
			if (read_directory(verbose, unixpath, prefix) != EXIT_SUCCESS)
				return EXIT_FAILURE;
		} else {
			u_int32_t loadaddr, execaddr;
			u_int32_t filesize = s.st_size;
			char filelocal[1024], fileresfs[1024]; /* eugh again */
			char *fl = filelocal + strlen(prefix), *fr = fileresfs;

			strcpy(filelocal, unixpath);
			if (get_loadexecaddress(filelocal, &loadaddr, &execaddr) != EXIT_SUCCESS)
				return EXIT_FAILURE;

			/* create the resfs filename for this file by working
			 * through filelocal skipping what's in prefix, and
			 * swapping / and .  - get_loadexecaddress() has already
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

			VERB("| ... adding %s...\n", fileresfs);
			if (add_entry(loadaddr, execaddr, filesize,
					fileresfs, unixpath) != EXIT_SUCCESS)
				return EXIT_FAILURE;
		}
	}

	closedir(d);
	return EXIT_SUCCESS;
}

static int
generate(FILE *fh, bool verbose,
	 const char *regfunc, const char *deregfunc, const char *reregfunc,
	 const char *prototype, const char *dir, bool oslib)
{
	const char * const roerrP = (oslib) ? "os_error" : "_kernel_oserror";

	/* output header */

	fputs("/* Generated by mkresfs " MKRESFS_VERSION ", do not edit by hand!\n"
	      " */\n\n"
	      "#include <stdlib.h>\n"
	      "#include <swis.h>\n", fh);
	fputs((oslib) ? "#include \"oslib/os.h\"\n" : "#include <kernel.h>\n", fh);
	fputs("#ifndef ResourceFS_RegisterFiles\n"
	      "#  define ResourceFS_RegisterFiles 0x41b40\n"
	      "#  define ResourceFS_DeregisterFiles 0x41b41\n"
	      "#endif\n\n", fh);

	/* recurse directory structure building up entries in our list */

	if (read_directory(verbose, dir, dir) != EXIT_SUCCESS)
		return EXIT_FAILURE;

	/* output list */

	fputs("static const unsigned char payload[] =\n{\n", fh);
	if (dump_data(fh, verbose) != EXIT_SUCCESS)
		return EXIT_FAILURE;
	fputs("};\n\n", fh);

	/* output functions */

	fprintf(fh, "%s *\n"
		    "%s (void)\n"
		    "{\n"
		    "  %s *err;\n"
		    "\n"  
		    "  __asm__ volatile (\"MOV	r0, %%[payload];\\n\\t\"\n"
		    "		    \"SWI	%%[resourcefs_registerfiles];\\n\\t\"\n"
		    "		    \"MOVVS	%%[err], r0;\\n\\t\"\n"
		    "		    \"MOVVC	%%[err], #0;\\n\\t\"\n"
		    "		    : [err] \"=r\" (err)\n"
		    "		    : [payload] \"r\" (payload),\n"
		    "		      [resourcefs_registerfiles] \"i\" (XOS_Bit | ResourceFS_RegisterFiles)\n"
		    "		    : \"r0\", \"r14\", \"cc\");\n"
		    "  return err;\n"
		    "}\n\n", roerrP, regfunc, roerrP);

	fprintf(fh, "%s *\n"
		    "%s (void)\n"
		    "{\n"
		    "  %s *err;\n"
		    "\n"  
		    "  __asm__ volatile (\"MOV	r0, %%[payload];\\n\\t\"\n"
		    "		    \"SWI	%%[resourcefs_deregisterfiles];\\n\\t\"\n"
		    "		    \"MOVVS	%%[err], r0;\\n\\t\"\n"
		    "		    \"MOVVC	%%[err], #0;\\n\\t\"\n"
		    "		    : [err] \"=r\" (err)\n"
		    "		    : [payload] \"r\" (payload),\n"
		    "		      [resourcefs_deregisterfiles] \"i\" (XOS_Bit | ResourceFS_DeregisterFiles)\n"
		    "		    : \"r0\", \"r14\", \"cc\");\n"
		    "  return err;\n"
		    "}\n\n", roerrP, deregfunc, roerrP);

	 /* It doesn't look like the given re-registering routine can return
	    an error but better leaven the mkresfs generated API open for
	    this possibility.  */
	fprintf(fh, "%s *\n"
		    "%s (int callback, int workspace)\n"
		    "{\n"
		    "  __asm__ volatile (\"MOV	r0, %%[payload];\\n\\t\"\n"
		    "		    \"MOV	r3, %%[workspace];\\n\\t\"\n"
		    "		    \"MOV	lr, pc;\\n\\t\"\n"
		    "		    \"MOV	pc, %%[callback];\\n\\t\"\n"
		    "		    :	/* no output */\n"
		    "		    : [payload] \"r\" (payload),\n"
		    "		      [workspace] \"r\" (workspace),\n"
		    "		      [callback] \"r\" (callback)\n"
		    "		    : \"r0\", \"r3\", \"lr\", \"cc\");\n"
		    "  return NULL;\n"
		    "}\n", roerrP, reregfunc);

	if (prototype != NULL) {
		FILE *hfh;
		size_t prot_len = strlen(prototype);
		char guardname[prot_len];
		int i;

		VERB("| Creating C header file...\n");
		if ((hfh = fopen(prototype, "w")) == NULL) {
			fprintf(stderr, "Unable to open %s for writing.\n",
				prototype);
			return EXIT_FAILURE;
		}

		for (i = 0; i < prot_len; i++) {
			char o = prototype[i];
			if (o != '\0' && !isalnum(o))
				o = '_';
			guardname[i] = o;
		}
		guardname[i] = '\0';

		fputs("/* Generated by mkresfs " MKRESFS_VERSION ", do not edit by hand!\n"
		      " */\n\n", hfh);
		fprintf(hfh, "#ifndef MKRESFS_%s_HEADER_INCLUDED\n", guardname);
		fprintf(hfh, "#define MKRESFS_%s_HEADER_INCLUDED\n", guardname);
		fputs((oslib) ? "\n#include \"oslib/os.h\"\n" : "\n#include <kernel.h>\n", hfh);

		fprintf(hfh, "\n/* Register files with ResourceFS when ResourceFS is already active.\n"
			" * Returns pointer to error, or NULL on success.\n"
			" */\n"
			"%s *%s (void);\n",
			roerrP, regfunc);

		fprintf(hfh, "\n/* Deregister files from ResourceFS.\n"
			" * Returns pointer to error, or NULL on success.\n"
			" */\n"
			"%s *%s (void);\n",
			roerrP, deregfunc);

		fprintf(hfh, "\n/* Register files with ResourceFS when ResourceFS is starting up\n"
			" * (i.e. at Service_ResourceFSStarting time).\n"
			" */\n"
			"%s *%s (int callback, int workspace);\n",
			roerrP, reregfunc);

		fprintf(hfh, "\n#endif\n");

		fclose(hfh);
	}

	VERB("| Done.\n");

	/* TODO: don't be lazy and let the OS garbage collect: free the list */
	return EXIT_SUCCESS;
}

/* -- front end ----------------------------------------------------------- */

static void
usage(const char *progname)
{
	puts("mkresfs " MKRESFS_VERSION " - Rob Kendrick <rjek@rjek.com>, GCCSDK Developers\n"
	     PACKAGE_BUGREPORT);
	printf("usage: %s [-o <outfile]> [-p <header>] [-v] [-z] [-r <regfunc>]"
		" [-d <deregfunc>] [-s <reregfunc>] <dir>\n", progname);
	puts(" -o    Specify output file. stdout used if absent.\n"
	     " -p    Create a C header called this with function prototypes.\n"
	     " -v    Verbose mode. Outputs status information as it works.\n"
	     " -z    Use OSLib instead of SharedCLibrary for prototypes.\n"
	     " -r    Register function name. mkresfs_register default.\n"
	     " -d    Deregister function name. mkresfs_deregister default.\n"
	     " -s    Reregister function name. mkresfs_reregister default.\n"
	     " <dir> Directory to create ResourceFS data from.");
}

int
main(int argc, char *argv[])
{
	const char *s_opts = "hvzr:d:s:o:p:";
	int o;
	FILE *fh = stdout;
	int rtrn;

	/* parsed options */
	bool opt_verbose = false;
	bool opt_oslib = false;
	const char *opt_outfile = NULL;
	const char *opt_regfunc = "mkresfs_register";
	const char *opt_deregfunc = "mkresfs_deregister";
	const char *opt_reregfunc = "mkresfs_reregister";
	const char *opt_indir = NULL;
	const char *opt_prototypes = NULL;

	while ((o = getopt(argc, argv, s_opts)) != -1) {
		switch (o) {
		case 'h':
			usage(argv[0]);
			exit(EXIT_SUCCESS);
			break;
		case 'v':
			opt_verbose = true;
			break;
		case 'z':
			opt_oslib = true;
			break;
		case 'r':
			opt_regfunc = optarg;
			break;
		case 'd':
			opt_deregfunc = optarg;
			break;
		case 's':
			opt_reregfunc = optarg;
			break;
		case 'o':
			opt_outfile = optarg;
			break;
		case 'p':
			opt_prototypes = optarg;
			break;
		default:
			exit(EXIT_FAILURE);
			break;
		}
	}

	if (optind == argc) {
		fprintf(stderr, "%s: no directory specified.\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	opt_indir = argv[optind];

	if (opt_outfile != NULL) {
		fh = fopen(opt_outfile, "w");
		if (fh == NULL) {
			fprintf(stderr, "%s: unable to open %s for writing.",
				argv[0], opt_outfile);
			exit(EXIT_FAILURE);
		}
	}

	rtrn = generate(fh, opt_verbose,
			opt_regfunc, opt_deregfunc, opt_reregfunc,
			opt_prototypes, opt_indir, opt_oslib);

	if (opt_outfile != NULL)
		fclose(fh);

	return rtrn;
}
