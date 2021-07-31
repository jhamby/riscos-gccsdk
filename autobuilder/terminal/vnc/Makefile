
SRCS = \
  argsresources.c \
  caps.c \
  colour.c \
  cursor.c \
  desktop.c \
  dialogs.c \
  fullscreen.c \
  listen.c \
  misc.c \
  popup.c \
  rfbproto.c \
  selection.c \
  shm.c \
  sockets.c \
  tunnel.c \
  vncviewer.c \
  ../libvncauth/vncauth.c \
  ../libvncauth/d3des.o

OBJS = $(SRCS:.c=.o)

CFLAGS=-O2 -I. -I../include -I$(GCCSDK_INSTALL_ENV)/include -I../libvncauth/
LDFLAGS=-static -L/$(GCCSDK_INSTALL_ENV)/lib -lXmu -lXaw3d -lXext -lXt -lSM -lICE -lCX11 -lDesk -ljpeg -lz

vncviewer: $(OBJS)

clean:
	rm -f vncviewer *.o
