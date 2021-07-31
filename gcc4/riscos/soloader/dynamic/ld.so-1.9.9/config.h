#ifdef DEBUG
#  define LDSO_IMAGE "../ld-so/ld.so"
#  define LDSO_CONF  "../util/ld.so.conf"
#  define LDSO_CACHE "../util/ld.so.cache"
#  define LDSO_PRELOAD "../util/ld.so.preload"
#  define LDDSTUB    "../util/lddstub"
#else
#  define LDSO_IMAGE "/lib/ld.so"
#  define LDSO_CONF  "SharedLibs:etc.ld/so/conf"
#  define LDSO_CACHE "SharedLibs:etc.ld/so/cache"
#  define LDSO_PRELOAD "SharedLibs:etc.ld/so/preload"
#  define LDDSTUB    "/usr/lib/lddstub"
#endif

#define LDD_ARGV0    "__LDD_ARGV0"
#define DIR_SEP      ":, \t\n"
#define MAX_DIRS     32

typedef void (*loadptr)(int func, ...);
typedef void (*callbackptr)(int ver, int nlibs, char **libs,
		int nmods, char **mods);

#define CALLBACK_VER 1

#define LIB_ANY	     -1
#define LIB_DLL       0
#define LIB_ELF       1
#define LIB_ELF_LIBC5 2
#define LIB_ELF_LIBC6 3

#define FUNC_VERS    0
#define FUNC_LDD     1
#define FUNC_LINK    2
#define FUNC_LINK_AND_CALLBACK 3

#define LDSO_CACHE_MAGIC "ld.so-"
#define LDSO_CACHE_MAGIC_LEN (sizeof LDSO_CACHE_MAGIC -1)
#define LDSO_CACHE_VER "1.7.0"
#define LDSO_CACHE_VER_LEN (sizeof LDSO_CACHE_VER -1)

#ifndef LD_USE_SYMLINKS
#define LD_USE_SYMLINKS 1
#endif

#define USE_MAPPED_LIBRARIES 0

typedef struct {
	char magic   [LDSO_CACHE_MAGIC_LEN];
	char version [LDSO_CACHE_VER_LEN];
	int nlibs;
} header_t;

typedef struct {
	int flags;
	int sooffset;
	int liboffset;
} libentry_t;

