testdir = $(pkglibdir)
test_SCRIPTS = test-app.exe
CLEANFILES = test-app.exe
test_app_sources = test-app.cs

test_app_build_sources = $(addprefix $(srcdir)/, $(test_app_sources))

test-app.exe: $(test_app_build_sources) $(GCCSDK_INSTALL_ENV)/lib/libriscosdotnet/riscos.dll
	$(MCS) -out:$@ $< -R:$(GCCSDK_INSTALL_ENV)/lib/libriscosdotnet/riscos
