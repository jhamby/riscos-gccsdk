bin_PROGRAMS=decaof

SVN_ROOT := $(shell svn info ${srcdir} | grep "^Repository Root: " | sed -e "s/^Repository Root: //")
SVN_BRANCH := $(shell svn info ${srcdir} | grep "^URL: " | sed -e "s/^URL: //")
SVN_REV := r$(shell svnversion -n ${srcdir}) at $(subst $(SVN_ROOT),,$(SVN_BRANCH))

###### _GNU_SOURCE to make strndup available
###### Add -DDEBUG if you want to enable the debug routines.
decaof_CFLAGS = -std=c99 \
	-W -Wall -pedantic -Wbad-function-cast -Wcast-align \
	-Wchar-subscripts -Winline \
	-Wmissing-prototypes -Wnested-externs -Wpointer-arith \
	-Wredundant-decls -Wshadow -Wstrict-prototypes -Wwrite-strings \
	-DGCCSDK_REVISION="\"$(SVN_REV)\""
decaof_SOURCES=decode.c error.c io.c main.c misc.c
