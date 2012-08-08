# $Id: os.NetBSD.mk 2237 2011-11-28 03:49:43Z jkoshy $
#
# Build recipes for NetBSD.

LDSTATIC?=	-static		# link programs statically

MKDOC?=		yes		# Build documentation.
MKLINT?=	no		# lint dies with a sigbus
MKTESTS?=	yes		# Enable the test suites.
