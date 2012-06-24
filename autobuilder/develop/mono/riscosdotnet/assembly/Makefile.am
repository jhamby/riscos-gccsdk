rdndir = $(pkglibdir)
rdn_SCRIPTS = riscos.dll
CLEANFILES = riscos.dll
riscos_dll_sources = \
	NativeMethods/wimp.cs \
	NativeMethods/toolbox.cs \
	NativeMethods/os.cs \
	NativeMethods/misc.cs \
	NativeMethods/colourtrans.cs \
	NativeMethods/font.cs \
	wimp.cs \
	toolbox.cs \
	icon.cs \
	task.cs \
	os.cs \
	window.cs \
	colourtrans.cs \
	reporter.cs \
	font.cs \
	menu.cs

riscos_dll_build_sources = $(addprefix $(srcdir)/, $(riscos_dll_sources))

riscos.dll: $(riscos_dll_build_sources)
	$(MCS) -debug -target:library -out:$@ $(riscos_dll_build_sources)
