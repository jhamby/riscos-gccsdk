bin_PROGRAMS=decaof

###### _GNU_SOURCE to make strndup available
###### Add -DDEBUG if you want to enable the debug routines.
decaof_CFLAGS = -std=c99 \
	-W -Wall -pedantic -Wbad-function-cast -Wcast-align \
	-Wchar-subscripts -Winline \
	-Wmissing-prototypes -Wnested-externs -Wpointer-arith \
	-Wredundant-decls -Wshadow -Wstrict-prototypes -Wwrite-strings \
	-DGCCSDK_REVISION=\"$(shell svnversion -n ${srcdir})\"
decaof_SOURCES=decode.c error.c io.c main.c misc.c

