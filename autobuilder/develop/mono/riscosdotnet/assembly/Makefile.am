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
	NativeMethods/spriteop.cs \
	NativeMethods/draw.cs \
	toolbox/toolbox.cs \
	toolbox/window.cs \
	toolbox/iconbar.cs \
	toolbox/menu.cs \
	toolbox/gadget.cs \
	toolbox/action-button.cs \
	toolbox/adjuster-arrows.cs \
	toolbox/button.cs \
	toolbox/display-field.cs \
	toolbox/draggable.cs \
	toolbox/label.cs \
	toolbox/labelled-box.cs \
	toolbox/number-range.cs \
	toolbox/option-button.cs \
	toolbox/popup-menu.cs \
	toolbox/radio-button.cs \
	toolbox/slider.cs \
	toolbox/string-set.cs \
	toolbox/writable-field.cs \
	toolbox/saveas-dialogue.cs \
	toolbox/proginfo-dialogue.cs \
	toolbox/quit-dialogue.cs \
	toolbox/fileinfo-dialogue.cs \
	toolbox/colour-dialogue.cs \
	toolbox/colour-menu.cs \
	toolbox/font-dialogue.cs \
	toolbox/font-menu.cs \
	toolbox/scale-dialogue.cs \
	toolbox/print-dialogue.cs \
	toolbox/dcs-dialogue.cs \
	toolbox/toolbox-task.cs \
	toolbox/object-template.cs \
	toolbox/window-template.cs \
	wimp.cs \
	icon.cs \
	task.cs \
	os.cs \
	window.cs \
	colourtrans.cs \
	reporter.cs \
	font.cs \
	menu.cs \
	sprite.cs \
	window-common.cs \
	wimp-task.cs \
	osfind.cs \
	osargs.cs \
	osgbpb.cs \
	draw-file.cs

riscos_dll_build_sources = $(addprefix $(srcdir)/, $(riscos_dll_sources))

riscos.dll: $(riscos_dll_build_sources)
	$(MCS) -debug -optimize -target:library -out:$@ $(riscos_dll_build_sources)
