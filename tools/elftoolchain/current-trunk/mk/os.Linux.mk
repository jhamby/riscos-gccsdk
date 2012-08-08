# $Id: os.Linux.mk 2432 2012-02-20 03:05:23Z jkoshy $
#
# Build recipes for Debian GNU/Linux based operating systems.

MKDOC?=		yes	# Build documentation.
MKLINT?=	no
MKPIC?=		no
MKTESTS?=	yes	# Enable the test suites.
MKTEX?=		yes	# Build TeX-based documentation.

OBJECT_FORMAT=	ELF	# work around a bug in the pmake package

YFLAGS+=	-d		# force bison to write y.tab.h

EPSTOPDF?=	/usr/bin/epstopdf
MAKEINDEX?=	/usr/bin/makeindex
MPOST?=		/usr/bin/mpost
MPOSTTEX?=	/usr/bin/latex
PDFLATEX?=	/usr/bin/pdflatex
