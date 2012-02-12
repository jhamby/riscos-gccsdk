MCS=mcs

SOURCES=NativeMethods.cs \
	wimp.cs \
	toolbox.cs \
	icon.cs \
	task.cs \
	os.cs \
	window.cs \
	colourtrans.cs \
	reporter.cs

all: riscos.dll

riscos.dll: $(SOURCES)
	$(MCS) $(SOURCES) -target:library -out:$@
