
CFLAGS = -mlibscl -mmodule

gccmodule: gccmodule.o module.o
	$(CC) $(CFLAGS) $< -o $@

# cmunge invocation rule
module.h module.o: cmhg/module
	cmunge -tgcc -32bit module.cmhg -o module.o -d module.h

gccmodule.c: module.h
