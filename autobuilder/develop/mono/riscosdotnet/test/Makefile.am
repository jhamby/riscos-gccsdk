testdir = $(pkglibdir)
test_SCRIPTS = test-gadget.exe test-sprite.exe test-toolbox.exe test-app.exe test-font.exe tb-template.exe
CLEANFILES = test-gadget.exe test-sprite.exe test-toolbox.exe test-app.exe test-font.exe tb-template.exe
test_app_sources = test-app.cs
test_font_sources = test-font.cs
test_toolbox_sources = test-toolbox.cs
test_sprite_sources = test-sprite.cs
test_gadget_sources = test-gadget.cs
tb_template_sources = tb-template.cs

test_app_build_sources = $(addprefix $(srcdir)/, $(test_app_sources))
test_font_build_sources = $(addprefix $(srcdir)/, $(test_font_sources))
test_toolbox_build_sources = $(addprefix $(srcdir)/, $(test_toolbox_sources))
test_sprite_build_sources = $(addprefix $(srcdir)/, $(test_sprite_sources))
test_gadget_build_sources = $(addprefix $(srcdir)/, $(test_gadget_sources))
tb_template_build_sources = $(addprefix $(srcdir)/, $(tb_template_sources))

MCS_FLAGS = -debug -optimize

test-toolbox.exe: $(test_toolbox_build_sources) ../assembly/riscos.dll
	$(MCS) $(MCS_FLAGS) -out:$@ $< -R:../assembly/riscos

test-app.exe: $(test_app_build_sources) ../assembly/riscos.dll
	$(MCS) $(MCS_FLAGS) -out:$@ $< -R:../assembly/riscos

test-font.exe: $(test_font_build_sources) ../assembly/riscos.dll
	$(MCS) $(MCS_FLAGS) -out:$@ $< -R:../assembly/riscos

test-sprite.exe: $(test_sprite_build_sources) ../assembly/riscos.dll
	$(MCS) $(MCS_FLAGS) -out:$@ $< -R:../assembly/riscos

test-gadget.exe: $(test_gadget_build_sources) ../assembly/riscos.dll
	$(MCS) $(MCS_FLAGS) -out:$@ $< -R:../assembly/riscos

tb-template.exe: $(tb_template_build_sources) ../assembly/riscos.dll
	$(MCS) $(MCS_FLAGS) -out:$@ $< -R:../assembly/riscos
