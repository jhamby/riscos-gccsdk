testdir = $(pkglibdir)
test_SCRIPTS = test-sprite.exe test-toolbox.exe test-app.exe test-font.exe
CLEANFILES = test-app.exe
test_app_sources = test-app.cs
test_font_sources = test-font.cs
test_toolbox_sources = test-toolbox.cs
test_sprite_sources = test-sprite.cs

test_app_build_sources = $(addprefix $(srcdir)/, $(test_app_sources))
test_font_build_sources = $(addprefix $(srcdir)/, $(test_font_sources))
test_toolbox_build_sources = $(addprefix $(srcdir)/, $(test_toolbox_sources))
test_sprite_build_sources = $(addprefix $(srcdir)/, $(test_sprite_sources))

test-toolbox.exe: $(test_toolbox_build_sources) $(testdir)/riscos.dll
	$(MCS) -debug -out:$@ $< -R:$(testdir)/riscos

test-app.exe: $(test_app_build_sources) $(testdir)/riscos.dll
	$(MCS) -debug -out:$@ $< -R:$(testdir)/riscos

test-font.exe: $(test_font_build_sources) $(testdir)/riscos.dll
	$(MCS) -debug -out:$@ $< -R:$(testdir)/riscos

test-sprite.exe: $(test_sprite_build_sources) $(testdir)/riscos.dll
	$(MCS) -debug -out:$@ $< -R:$(testdir)/riscos

