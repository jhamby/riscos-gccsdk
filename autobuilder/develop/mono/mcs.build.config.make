RUNTIME = mono

TEST_RUNTIME = $(RUNTIME) --debug

ifeq ($(thisdir),jay)
# Compile the C# parser with the native compiler
CCOMPILE = /usr/bin/gcc $(USE_CFLAGS)
endif

prefix = $(GCCSDK_INSTALL_ENV)

INSTALL = $(topdir)/../install-sh
