#
# Rules for handling include files.
#
# $Id: elftoolchain.inc.mk 2285 2011-12-04 06:40:45Z jkoshy $

.if !defined(TOP)
.error	Make variable \"TOP\" has not been defined.
.endif

.include "${TOP}/mk/elftoolchain.os.mk"

.include <bsd.own.mk>

.if ${OS_HOST} == "DragonFly" || ${OS_HOST} == "FreeBSD" || \
	${OS_HOST} == "OpenBSD"
# Simulate <bsd.inc.mk>.
.PHONY:		incinstall
includes:	${INCS}	incinstall
.for inc in ${INCS}
incinstall::	${DESTDIR}${INCSDIR}/${inc}
.PRECIOUS:	${DESTDIR}${INCSDIR}/${inc}
${DESTDIR}${INCSDIR}/${inc}: ${inc}
	cmp -s $> $@ > /dev/null 2>&1 || \
		${INSTALL} -c -o ${BINOWN} -g ${BINGRP} -m ${NOBINMODE} $> $@
.endfor
.else
# Use the standard <bsd.inc.mk>.
.include <bsd.inc.mk>
.endif
