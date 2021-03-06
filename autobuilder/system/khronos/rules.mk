#SUFFIXES=.cmhg

C_OBJS = $(patsubst %.c,%.o,$(CSRCS))
C_SHOBJS = $(patsubst %.c,%.os,$(CSRCS))
ASM_OBJS = $(patsubst %.s,%.o,$(ASMSRCS))
ASM_SHOBJS = $(patsubst %.s,%.os,$(ASMSRCS))

OBJS = $(MODULE_OBJECT) $(C_OBJS) $(ASM_OBJS) $(CPP_OBJS)
SHOBJS = $(MODULE_OBJECT) $(C_SHOBJS) $(ASM_SHOBJS) $(CPP_OBJS)

ifneq ($(SONAME),)
SONAME_OPT=-Wl,-soname=$(SONAME)
else
SONAME_OPT=
endif

all::
	$(MAKE) build


build:: $(DIRS) $(TARGET) $(LIBRARY) $(BIN_TARGET)

install: $(DIRS) $(TARGET) $(LIBRARY) $(BIN_TARGET)

clean:: $(DIRS)
ifneq ($(OBJS) $(SHOBJS),)
	rm -f $(OBJS) $(SHOBJS)
endif


$(C_OBJS): %.o: %.c
ifeq ($(MODULE_CMHG),)
	$(CC) $(LIB_CFLAGS) -o $@ $<
else
	$(MOD_CC) $(MOD_CFLAGS) -o $@ $<
endif

$(C_SHOBJS): %.os: %.c
ifeq ($(MODULE_CMHG),)
	$(CC) $(SHLIB_CFLAGS) -o $@ $<
else
	$(MOD_CC) $(MOD_CFLAGS) -o $@ $<
endif

$(ASM_OBJS): %.o: %.s
ifeq ($(MODULE_CMHG),)
	$(ASM) $(LIB_ASMFLAGS) -o $@ $<
else
	$(MOD_ASM) $(MOD_ASMFLAGS) -o $@ $<
endif

$(ASM_SHOBJS): %.os: %.s
ifeq ($(MODULE_CMHG),)
	$(ASM) $(SHLIB_ASMFLAGS) -o $@ $<
else
	$(MOD_ASM) $(MOD_ASMFLAGS) -o $@ $<
endif

$(MODULE_INCLUDE): $(MODULE_CMHG)
	cmunge $(CMUNGEFLAGS) -d $@ $<
$(MODULE_OBJECT): $(MODULE_CMHG)
	cmunge $(CMUNGEFLAGS) -o $@ $<


ifeq ($(MODULE_CMHG),)
$(TARGET): $(TARGET).so $(TARGET).a $(DIRS) $(EXTRA_TARGET_DEPS) $(SHOBJS) $(OBJS) $(LIBS)

$(TARGET).so: $(DIRS) $(EXTRA_TARGET_DEPS) $(SHOBJS) $(OBJS) $(LIBS)
	@echo Generating shared library $(TARGET).so
	$(LINK) -o $(TARGET).so $(SHOBJS) $(LIBS) $(SYSTEM_LIBS) $(SONAME_OPT) -shared -Wl,--no-undefined

$(TARGET).a: $(DIRS) $(EXTRA_TARGET_DEPS) $(SHOBJS) $(OBJS) $(LIBS)
	@echo Generating archive library $(TARGET).a
	$(AR) cru $(TARGET).a $(OBJS)
else
$(TARGET): $(DIRS) $(EXTRA_TARGET_DEPS) $(OBJS) $(LIBS)
	@echo Generating module $(TARGET)
	$(MOD_LINK) -mmodule -o $(TARGET) $(OBJS) $(LIBS) $(SYSTEM_LIBS)
endif

$(BIN_TARGET): $(OBJS) $(LIBS)
	@echo Generating binary $(BIN_TARGET)
	$(LINK) -o $(BIN_TARGET) $(OBJS) $(LIBS) $(SYSTEM_BIN_LIBS)

$(DIRS)::
	@echo Executing command $(MAKECMDGOALS) for directory $@
	@$(MAKE) -C $@ $(MAKECMDGOALS)
