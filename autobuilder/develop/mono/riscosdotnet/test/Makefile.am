testdir = $(pkglibdir)
test_SCRIPTS = test-app.exe test-font.exe
CLEANFILES = test-app.exe
test_app_sources = test-app.cs
test_font_sources = test-font.cs

test_app_build_sources = $(addprefix $(srcdir)/, $(test_app_sources))
test_font_build_sources = $(addprefix $(srcdir)/, $(test_font_sources))

test-app.exe: $(test_app_build_sources) $(testdir)/riscos.dll
	$(MCS) -out:$@ $< -R:$(testdir)/riscos

test-font.exe: $(test_font_build_sources) $(testdir)/riscos.dll
	$(MCS) -out:$@ $< -R:$(testdir)/riscos
