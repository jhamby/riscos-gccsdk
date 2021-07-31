include ../../Version.mk
include ../../Config.mk
include ../../../../config.mk

OBJDIR = ../../../$(BUILDDIR)/dynamic

CFLAGS += -I.. -I. -DNO_UNDERSCORE -DVERBOSE_DLINKER
CFLAGS += -fPIC -D__PIC__ -mpic-register=v4 -ffixed-sl
# -g -mno-apcs-stack-check
#-DDEBUG # -funroll-loops

$(OBJDIR)/%.o: %.s
	$(CC) $(CFLAGS) -xassembler-with-cpp -c $< -o $@
$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	$(OBJCOPY) --redefine-sym __rt_stkovf_split_small=_dl_stkovf_split_small --redefine-sym __rt_stkovf_split_big=_dl_stkovf_split_big $@

SRC1S = resolve.s stkovf.s
SRC2S = elfinterp.c dl_open.c dl_stat.c

SRCS	= $(SRC1S) $(SRC2S)
OBJ1S	= $(SRC1S:.s=.o)
OBJ2S	= $(SRC2S:.c=.o)
OBJS	= $(OBJ1S) $(OBJ2S)

DLINK_OBJS:= $(addprefix $(OBJDIR)/, $(OBJS))

all: $(DLINK_OBJS)

$(OBJDIR)/resolve.o: resolve.s
$(OBJDIR)/dl_open.o: dl_open.c ../../config.h
$(OBJDIR)/dl_stat.o: dl_stat.c ../../config.h
$(OBJDIR)/stkovf.o: stkovf.s
