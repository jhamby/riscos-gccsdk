bin_PROGRAMS=cmunge

SVN_ROOT := $(shell svn info ${srcdir} | grep "^Repository Root: " | sed -e "s/^Repository Root: //")
SVN_BRANCH := $(shell svn info ${srcdir} | grep "^URL: " | sed -e "s/^URL: //")
SVN_REV := r$(shell svnversion -n ${srcdir}) at $(subst $(SVN_ROOT),,$(SVN_BRANCH))

cmunge_SOURCES=apcscli.c assemble.c blank.c comments.c datestamp.c error.c filename.c format.c gfile.c main.c mem.c options.c readfile.c str.c system.c throwback.c writeexport.c writefile.c writeheader.c apcscli.h assemble.h blank.h comments.h copyright.h datestamp.h error.h filename.h format.h gfile.h MemCheck.h mem.h options.h readfile.h str.h system.h throwback.h writeexport.h writefile.h writeheader.h

cmunge_CFLAGS=-Wall -std=c99 -DGCCSDK_REVISION="\"$(SVN_REV)\""
