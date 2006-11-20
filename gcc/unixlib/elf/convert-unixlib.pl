#!/usr/bin/perl
# Convert UnixLib AOF source tree into an ELF source tree.

# convert-unixlib.pl <source aof tree> <destination elf dir for tree>
$aofsourcetree = $ARGV[0];
$elfsourcetree = $ARGV[1];

if ($#ARGV != 1) {
    die "syntax: convert-unixlib.pl <source aof tree> <destination elf dir for tree>";
}

if (! -d "$aofsourcetree") {
  die "$aofsourcetree does not exist\n";
}

if (! -e "$aofsourcetree/source/sys/_syslib.s") {
  die "$aofsourcetree looks invalid\n";
}


# Convert an assembler file from AOF syntax to ELF syntax
# convert_as (srcfile, dstfile)
sub convert_s {
    my $srcfile = $_[0];
    my $dstfile = $_[1];

    #printf ("converting asm '%s' to '%s'\n", $srcfile, $dstfile);

    if ($dstfile =~ /elf-macros.s$/
	|| $dstfile =~ /aof-macros.s$/) {
	open IN, "$srcfile" or die "$!";
	open OUT, "> $dstfile" or die "$!";
	while (<IN>) {
	    print OUT $_;
	}
	close IN;
	close OUT;
	return;
    }

    open IN, "$srcfile" or die "$!";
    open OUT, "> $dstfile" or die "$!";
    while (<IN>) {
	#if (/^;/) {
	#    next if (/; \$Source:/);
	#    next if (/; \$Date:/);
	#    next if (/; \$Revision:/);
	#    next if (/; \$State:/);
	#    next if (/; \$Author:/);
	#    next if (/;---+/);
	#}

	s/;/@/; # Change comments from ; to @

	if (/\w+\s+(RN|FN)\s+\w+/) {
	    s/(\w+)\s+(RN|FN)\s+(\w+)/$1\t.req\tr$3/;

	    # Don't copy in register defs from asm_dec.s.
	    next if ($dstfile =~ /asm_dec.s$/);

	    print OUT $_;
	    # Must shortcut loop to prevent invalid conversion into a label
	    next;
	}

	s/\s+GET\s*(.*)/\#include \"$1\"/;
	s!clib/unixlib/asm_dec\.s!unixlib/asm_dec.s!;
	s!clib/unixlib/buildoptions.s!unixlib/buildoptions.h!;
	s!incl-local/internal/aof-macros.s!internal/elf-macros.s!;

	# Special case to ease translations (from sys/_signal.s)
	if (/^\|__ul_errbuf__size/) {
	    print OUT ".set __ul_errbuf__size, . - __ul_errbuf_errblock\n";
	    next;
	}

	next if (/^\s+ENTRY/);

	# Fixup IMPORT |symname|, WEAK to ELF .weak references
	s/^\s+IMPORT\s+\|(.*)\|,\s*WEAK/\t.weak\t$1/;

	# Skip all other IMPORTs as they are not required for ELF.
	next if /IMPORT/;

	# Convert EXPORTs, and remove the vertical bars around symbols.
	s/^\s+EXPORT\s+\|(.*)\|/\t.global\t$1/;
	s/^\s+EXPORT\s+(.*)/\t.global\t$1/;

	if (/AREA/) {
	    if (/CODE/) { $_ = "\t.text"; }
	    elsif (/READONLY/) { $_ = "\t.rodata"; }
	    elsif (/NOINIT/) { $_ = "\t.section \".bss\""; }
	    elsif (/DATA/) { $_ = "\t.data"; }
	    print OUT $_."\n";
	    next;
	}

	s/DCD\s+(.+)/.word\t$1/i;
	s/DCB\s+([0-9a-fA-FxX]+)/.byte\t$1/i;
	s/DCF\s+([0-9a-fA-FxX]+)/.float\t$1/i;

	s/DCB\s+\"(.*)\"$/.ascii\t\"$1\"/i;
	s/DCB\s+\"(.*)\",\s+0$/.asciz\t\"$1\"/i;

	# Fix string/_memset.s.  Under gas the alignment is in powers
	# of two.
	if (/^\s+ALIGN\s+32/) {
	    s/ALIGN\s+32/.align\t5/i;
	    print OUT $_;
	    next;
	}

	if (/^\s+ALIGN\s+([0-9]+)/) {
	    s/ALIGN\s+([0-9]+)/.align\t$1/i;
	    print OUT $_;
	    next;
	}

	s/^\s+ALIGN$/\t.align/i;

	# Convert EQU statements with a base 2 number
	s/^(\w+)\s+EQU\s+2_(\w+)/.set\t$1, 0b$2/;

	# Convert EQU statements with a base 16 number
	s/^(\w+)\s+EQU\s+&(\w+)/.set\t$1, 0x$2/;

	# Convert EQU statements with a base 10 number
	s/^(\w+)\s+EQU\s+(\w+)/.set\t$1, $2/;

	# Convert '*' (EQU) statements with a base 2 number
	s/^(\w+)\s+\*\s+2_(\w+)/.set\t$1, 0b$2/;

	# Convert '*' (EQU) statements with a base 16 number
	s/^(\w+)\s+\*\s+&(\w+)/.set\t$1, 0x$2/;

	# Convert '*' (EQU) statements with a base 10 number
	s/^(\w+)\s+\*\s+(\w+)/.set\t$1, $2/;

	# Convert RISC OS-style hex notation &1f to 0x1f.
	s/&(\w+)/0x$1/;

	# Label handling
	if (/^[\.a-zA-Z_\|]/) {
	    if (/^\|/) { s/^\|([\.\w\$]+)\|/$1:/; }
	    else { s/([\.\w\$]+)/$1:/; }
	}

	# Turn statements like cmp a4,#"f" into cmp a4,#'f'
	s/\#\"(.)\"/\#\'$1\'/;

	s/\|(.*)\|/$1/; # Remove | from surrounding labels

	# Ensure that a '.end' statement isn't placed at the end
	# of asm_dec.s or _syslib.s
	if (/\s*END$/) {
	    next if ($dstfile =~ /asm_dec.s$/);
	    next if ($dstfile =~ /_syslib.s$/);
	    print OUT "\t.end\n";
	    next;
	}

	# Convert assembler if, else and endif statements into
	# C preprocessor directives.

	if (/^\t\[/ or /^\s+\[/) {
	    # If statement
	    my $i = $_;
	    chomp $i;
	    $i =~ s/^\t\[\s+//; # Remove .if
	    my ($var, $comp, $val) = $i =~ /(\w+)\s+(<|>|=)\s+(\d+)/;
	    #printf ("if: i='%s', var='%s', comp='%s', val='%s'\n", $i, $var, $comp, $val);
	    if ($i =~ /SOFTFLOAT/) {
		printf OUT "#ifdef __SOFTFP__\n" if ($i =~ /TRUE/);
		printf OUT "#ifndef __SOFTFP__\n" if ($i =~ /FALSE/);
		next;
	    }
	    if ($var =~ /_ELF/) {
		printf OUT "#ifdef __ELF__\n" if ($comp ne '=');	
		printf OUT "#ifndef __ELF__\n" if ($comp eq '=');
		next;
	    }

	    # Special case cause I'm too lazy today.
	    if ($i =~ /:LOR:/) {
		printf OUT "#if __UNIXLIB_EXTREMELY_PARANOID || __UNIXLIB_STACK_CHECK_MAGIC\n";
		next;
	    }

	    printf OUT "#if %s\n", $var if ($comp eq '>');
	    if ($comp eq '=') {
		printf OUT "#if !%s\n", $var if ($val eq '0');
		printf OUT "#if %s\n", $var, $val if ($val eq '1');
	    }
	    next;
	}
	s/^\t\|/\#else/;
	s/^\t\]/\#endif/;

	s/^\t\%\s+(\d+)/\t.space\t$1/;
	s/^\t\%\s+(\w+)/\t.space\t$1/;
	s/:SHL:/<</;
	s/:SHR:/>>/;

	# Convert AOF to ELF linker section names
	#s/Image\$\$RO\$\$Base/__executable_start/;
	#s/Image\$\$RW\$\$Base/__data_start/;
	#s/Image\$\$RW\$\$Limit/__end__/;

	print OUT $_;
    }

    close OUT;
    close IN;
}

# Convert C source files to ELF format
sub convert_c {
    my $srcfile = $_[0];
    my $dstfile = $_[1];

    #printf ("converting source '%s' to '%s'\n", $srcfile, $dstfile);

    open IN, "$srcfile" or die "$!";
    open OUT, "> $dstfile" or die "$!";
    while (<IN>) {
	print OUT $_;
    }
    close OUT;
    close IN;
}

# Convert C header files to ELF format
sub convert_h {
    my $srcfile = $_[0];
    my $dstfile = $_[1];

    #printf ("converting header '%s' to '%s'\n", $srcfile, $dstfile);
    
    open IN, "$srcfile" or die "$!";
    open OUT, "> $dstfile" or die "$!";
    while (<IN>) {
	print OUT $_;
    }
    close OUT;
    close IN;
}

# Return an array of files with a certain suffix type.
sub filelist {
    my $dir = $_[0];
    my $suffix = $_[1];
    my @a = ();
    opendir (DIR, "$dir");
    while ($e = readdir (DIR)) {
	next if ($e eq '.' or $e eq '..' or $e eq '.svn');
	if ($e =~ /\.$suffix$/) {
	    push (@a, $e);
	}
    }
    closedir (DIR);
    return @a;
}

sub convert_headers {
    my $from = $_[0];
    my $to = $_[1];
    my @header_list = ();

    mkdir "$to" if (! -d "$to");

    opendir (ROOT, "$from");
    my @subdirs = ();
    while ($entry = readdir (ROOT)) {
	next if ($entry eq '.' or $entry eq '..' or $entry eq '.svn');
	if (-d "$from/$entry") {
	    # Deal with sub-directories later
	    push (@subdirs, $entry);
	    next;
	}

	my $src = "$from/$entry";
	my $dst = "$to/$entry";
	if ($entry =~ /\.h$/) {
	    convert_h ($src, $dst);
	    push (@header_list, "$entry");
	} elsif ($entry =~ /\.s$/) {
	    convert_s ($src, $dst);
	    push (@header_list, "$entry");
	}
    }
    closedir (ROOT);

    # Deal with header file subdirectories
    foreach $dir (@subdirs) {
	opendir (SUBDIR, "$from/$dir");
	while ($entry = readdir (SUBDIR)) {
	    next if ($entry eq '.' or $entry eq '..' or $entry eq '.svn');
	    mkdir ("$to/$dir") if (! -d "$to/$dir");
	    my $src = "$from/$dir/$entry";
	    my $dst = "$to/$dir/$entry";
	    if ($entry =~ /\.h$/) {
		convert_h ($src, $dst);
		push (@header_list, "$dir/$entry");
	    } elsif ($entry =~ /\.s$/) {
		convert_s ($src, $dst);
		push (@header_list, "$dir/$entry");
	    }
	}
	closedir (SUBDIR);
    }

    return @header_list;
}


# output_make_var (makefile_name, array of files, optional directory)
sub output_make_var {
    my $name = $_[0];
    my $a = $_[1];
    my $dir = $_[2];
    my @array = @$a;

    printf MAKE "%s = ", $name;
    for (my $x = 0; $x <= $#array; $x++) {
	printf MAKE "\t%s/%s", $dir, $array[$x] if ($dir ne "");
	printf MAKE "\t%s", $array[$x] if ($dir eq "");

	print MAKE " \\" if ($x != $#array);
	printf MAKE "\n";
    }
}

# copy a file from the AOF source tree to the ELF source tree
sub copy_file {
    my $src = $_[0];
    my $dst = $_[1];

    system ("cp", "-a", "$aofsourcetree/$src", "$elfsourcetree/$dst");
}

if (! -d "$elfsourcetree") {
    mkdir $elfsourcetree or die "cannot create $elfsourcetree: $!";
}

# Construct the automake Makefile as we go along
open (MAKE, "> $elfsourcetree/Makefile.am");
print MAKE<<EOF;
# Automake style makefile for UnixLib
# Copyright (c) 2005, 2006 UnixLib developers
# Written by Nick Burrett <nick\@sqrt.co.uk>

# Multilib support
MAKEOVERRIDES=

# These flags exclusively apply to a C compiler
WARNINGSC = -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations \\
	-Wnested-externs

WARNINGS = -pedantic -Wall -Wundef -Wpointer-arith \\
	-Wcast-align -Wwrite-strings -Winline -Wno-unused \\
	-Winline -Wno-unused -W -Wcast-qual -Wshadow \$(WARNINGSC)

# The -isystem command tells GCC to prefer the header files in UnixLib,
# rather than trying to compile the library with the system headers.
# Header files that provide additional defintions only required for the
# building of UnixLib should be held in the source directories.
AM_CPPFLAGS = -isystem \$(top_srcdir)/include -I \$(top_srcdir)/incl-local

# C99 mode is required to build UnixLib because the library now contains
# a lot of C99 features.
# The __UNIXLIB_INTERNALS definition will disappear when the system headers
# have moved all private data into private header files.
AM_CFLAGS = -D__UNIXLIB_INTERNALS -D_GNU_SOURCE=1 -std=c99 \$(LIBM_FLAGS)

# Assembler files rely heavily on the C preprocessor to keep structures
# referred to by both languages in sync.
AM_CCASFLAGS = -xassembler-with-cpp -isystem \$(top_srcdir)/include -I \$(top_srcdir)/incl-local

SUBDIRS = test

includedir = \$(prefix)
EOF

# Contain a list of the make variables that hold a per-directory list
# of source code files.
@makevars = ();

# The top level source directory only contains sub-directories.
opendir (ROOT, "$aofsourcetree/source");
while ($dir = readdir (ROOT)) {
    # Do not look at .svn directories or the main header file
    # directories 'clib' and 'incl-local'.
    next if ($dir eq '.' or $dir eq '..' or $dir eq ".svn"
	     or $dir eq "test" or $dir eq "test.1"
	     or $dir eq "clib" or $dir eq "incl-local");
    next if (! -d "$aofsourcetree/source/$dir");

    # Ensure source sub-directories exist in new ELF tree
    mkdir "$elfsourcetree/$dir" if (! -d "$elfsourcetree/$dir");

    my @c_files = filelist ("$aofsourcetree/source/$dir", "c");
    my @h_files = filelist ("$aofsourcetree/source/$dir", "h");
    my @s_files = filelist ("$aofsourcetree/source/$dir", "s");
    my @marray = ();
    foreach $sf (@c_files) {
	my $src = "$aofsourcetree/source/$dir/$sf";
	my $dst = "$elfsourcetree/$dir/$sf";
	convert_c ($src, $dst);

	# libm-support/libm.c is specially handled
	next if ($dir eq "libm-support" and $sf eq "libm.c");

	push (@marray, $sf);
    }

    foreach $sf (@s_files) {
	my $src = "$aofsourcetree/source/$dir/$sf";
	my $dst = "$elfsourcetree/$dir/$sf";
	convert_s ($src, $dst);
	push (@marray, $sf);
    }

    foreach $sf (@h_files) {
	my $src = "$aofsourcetree/source/$dir/$sf";
	my $dst = "$elfsourcetree/$dir/$sf";
	convert_h ($src, $dst);
    }

    my $mvar = $dir."_src";
    $mvar =~ s/-/_/g; # Convert hyphens to underscores

    push (@makevars, $mvar);
    output_make_var ($mvar, \@marray, $dir);

    print MAKE "\n";
}
closedir (ROOT);

# Deal with the header files
@inst_headers = convert_headers ("$aofsourcetree/source/clib", "$elfsourcetree/include");
@noinst_headers = convert_headers ("$aofsourcetree/source/incl-local", "$elfsourcetree/incl-local");

output_make_var ("libc_headers", \@inst_headers, "include");
print MAKE "libc_headers += include/unixlib/stubs.h\n";

print MAKE "\n";
output_make_var ("nobase_noinst_HEADERS", \@noinst_headers, "incl-local");
print MAKE "\n";

print MAKE "libm_la_SOURCES = libm-support/libm.c\n\n";
print MAKE "toolexeclib_LTLIBRARIES = libunixlib.la libm.la\n";

print MAKE "libunixlib_la_SOURCES = ";
my $x = 0;
foreach $m (@makevars) {
    # Don't include the SUL module in the library make list.
    next if ($m eq 'module_src');
    printf MAKE "\$(%s) ", $m;
    $x ++;
    if ($x == 4) {
	printf MAKE "\\\n\t";
	$x = 0;
    }
}

print MAKE "\n\n";

print MAKE "libunixlib_la_LDFLAGS = -Wc,-nostdlib\n\n";

print MAKE "libm_la_LDFLAGS = -Wc,-nostdlib\n\n";

print MAKE "# Build the SharedUnixLibrary module\n";
print MAKE "bin_PROGRAMS = sul\n\n";
print MAKE "sul.o: module/sul.s\n";
print MAKE "\t\$(CCASCOMPILE) -o sul.o -c \`test -f \'module/sul.s\' || echo \'\$(srcdir)/\'\`module/sul.s\n\n";

print MAKE "# The ARM linker cannot change output formats during the link\n";
print MAKE "# stage.  We must use objcopy to convert the ELF binary into\n";
print MAKE "# a raw binary\n";
print MAKE "sul\$(EXEEXT): sul.o\n";
print MAKE "\t\$(OBJCOPY) -O binary sul.o sul\$(EXEEXT)\n";
print MAKE "\n";
print MAKE "# These rules are copied direct from a generated Makefile.\n";
print MAKE "# The only difference is that stack checking is turned off\n";
print MAKE "# for these targets.\n";
print MAKE "# Automake doesn't seem to provide a per-object file flag\n";
print MAKE "# setting function.\n";

@nostackfiles = ("sys/brk", "sys/stackalloc", "debug/dvsprintf");
foreach $f (@nostackfiles) {
    my $leaf = $f;
    $leaf =~ s/[a-z]*\///;
    printf MAKE "%s.lo: %s.c\n", $leaf, $f;
    printf MAKE "\tif \$(LTCOMPILE) -Wc,-mno-apcs-stack-check \\\n";
    printf MAKE "\t-MT %s.lo -MD -MP -MF \"\$(DEPDIR)/%s.Tpo\" -c -o %s.lo \\\n", $leaf, $leaf, $leaf;
    printf MAKE "\t\`test -f \'%s.c\' || echo \'\$(srcdir)/\'\`%s.c; \\\n", $f, $f;
    printf MAKE "\tthen mv -f \"\$(DEPDIR)/%s.Tpo\" \"\$(DEPDIR)/%s.Plo\"; \\\n", $leaf, $leaf;
    printf MAKE "\telse rm -f \"\$(DEPDIR)/%s.Tpo\"; exit 1; fi\n\n", $leaf;
}

print MAKE "install-data-local: install-headers\n\n";
print MAKE "install-headers:\n";
print MAKE "\t\$(mkinstalldirs) \$(DESTDIR)\${tooldir}/include\n";
print MAKE "\tfor dir in arpa bits net netinet resolv rpc string sys unixlib; do \\\n";
print MAKE "\t  \$(mkinstalldirs) \$(DESTDIR)\${tooldir}/include/\$\${dir} ; done\n";
print MAKE "\tfor file in \${libc_headers}; do \\\n";
print MAKE "\t  \$(INSTALL_DATA) \$(srcdir)/\$\${file} \$(DESTDIR)\${tooldir}/\$\${file} ; done\n";

print MAKE "\n";
close MAKE;

# Makefile has been completed.  Now move in other useful files
copy_file ("Docs/Changelog", "ChangeLog");
copy_file ("Docs/Copyright", "COPYING");
copy_file ("Docs/ReadMe", "README");
mkdir "$elfsourcetree/contrib" if (! -d "$elfsourcetree/contrib");
copy_file ("Boot,feb", "contrib/Boot,feb");
copy_file ("!Help", "contrib/Help");
copy_file ("Run,feb", "contrib/Run,feb");
copy_file ("Sprites,ff9", "contrib/Sprites,ff9");
copy_file ("Sprites22,ff9", "contrib/Sprites22,ff9");
copy_file ("elf/configure.ac", "configure.ac");
copy_file ("elf/libtool.m4", "libtool.m4");

# Copy in testsuite and convert to automake style
mkdir "$elfsourcetree/test" if (! -d "$elfsourcetree/test");
copy_file ("source/test/error.h", "test/error.h");
copy_file ("source/test/hp-timing.h", "test/hp-timing.h");
copy_file ("source/test/test-skeleton.c", "test/test-skeleton.c");

# We purposely ignore any tests that are not in sub-directories as
# they are probably not setup to run correctly with the automake
# testsuite.

open (MAKE, "> $elfsourcetree/test/Makefile.am");
print MAKE<<EOF;
# Testsuite automake-style infrastructure

AUTOMAKE_OPTIONS = no-dependencies
SUBDIRS = stdio stdlib string strings
EOF
close MAKE;


foreach $dir ("math", "stdio", "stdlib", "string", "strings") {
    mkdir "$elfsourcetree/test/$dir" if (! -d "$elfsourcetree/test/$dir");
    open (MAKE, "> $elfsourcetree/test/$dir/Makefile.am");
    print MAKE "# Testsuite automake-style infrastructure\n\n";
    print MAKE "AUTOMAKE_OPTIONS = no-dependencies\n";
    print MAKE "AM_CPPFLAGS = -isystem \$(top_srcdir)/include -I \$(top_srcdir)/test -D_GNU_SOURCE=1 -std=c99\n";
    print MAKE "LDADD = -L \$(top_builddir)\n\n";

    opendir (TESTS, "$aofsourcetree/source/test/$dir");
    my @tests = ();
    while ($entry = readdir (TESTS)) {
	next if ($entry eq '.' or $entry eq '..' or $entry eq '.svn');
	if ($entry =~ /\.c$/) {
	    push (@tests, $entry);
	}
	copy_file ("source/test/$dir/$entry", "test/$dir/$entry");
    }
    close TESTS;

    print MAKE "check_PROGRAMS = ";
    my $x = 0;
    for (my $y = 0; $y < $#tests; $y++) {
	my $test = $tests[$y];
	$x ++;
	# Strip suffix as automake wants the program name.
	$test =~ s/\.c//;
	printf MAKE "%s ", $test;
	if ($x == 5 and $y != $#tests) {
	    printf MAKE "\\\n\t";
	    $x = 0;
	}
    }
    print MAKE "\n\nTESTS = \$(check_PROGRAMS)\n";
    close MAKE;
}

# Generate the stubs header file

my @dirs;
my @srcs;
my @objs;

# Find all source files
sub find_files {
  my $dir = $_[0];
  my @entries = glob("$dir/*");
  my $entry;
  foreach $entry (@entries) {
    # Make sure names are in unix format
    $entry =~ s/\/([cosh])\/([^\/\.]*)$/\/$2.$1/;
    $entry =~ s/^source\///;
    # Ignore special directories
    next if $entry =~ m/(CVS)|(.svn)/;
    next if $entry =~ m/^(test)|(clib)|(module)|(conform)|(incl-local)/;
    if (-d "$entry") {
      push @dirs, $entry if !($entry =~ m/^(.*\/)?[cosh]$/);
      find_files("$entry");
    } else {
      if ($entry =~ m/\.[cs]$/) {
        my $obj = $entry;
        $obj =~ s/[cs]$/o/;
        push @objs, $obj;
        push @srcs, $entry;
      }
    }
  }
}

find_files($elfsourcetree);

# Generate include/unixlib/stubs.h
open(STUBS, ">$elfsourcetree/include/unixlib/stubs.h") or die $^E;

print STUBS "/* This file is automatically generated by convert-unixlib.pl.\n"
           ."It defines a symbol `__stub_FUNCTION' for each function\n"
           ."in the C library which is a stub, meaning it will fail\n"
           ."every time called, usually setting errno to ENOSYS.  */\n\n";

foreach $src (@srcs) {
  # Only check c files
  next if !($src =~ m/\.c$/);
  open(SRC, "<".$src) or die $^E;
  my $funcname = "none";
  my $return = 1;
  while (<SRC>) {
    # Get function name
    if (/^(\w*) \([^\)]*\)\W*$/) {
      $funcname = $1;
      $return = 0;
    }
    # Find the return statement
    if (/^\W*return.*ENOSYS/) {
      $return = -1 if ($return == 0);
    } elsif (/^\W*return/) {
      # Only a stub iff there is one return that always returns ENOSYS
      $return = 1;
    }
    # Find function end
    if (/^\}\W*$/) {
      print STUBS "#define __stub_$funcname\n" if ($return == -1);
    }
  }
  close SRC;
}
close STUBS;

# Finally change to the ELF directory and run the configuration stuff
printf ("-- building automake stuff\n");
chdir $elfsourcetree;
printf ("-- running aclocal\n");
system "aclocal-1.9 -I ./";
printf ("-- running autoheader\n");
system "autoheader";
printf ("-- running automake\n");
system "automake-1.9", "-a";
printf ("-- running autoconf\n");
system "autoconf";
printf ("-- complete\n");
exit 0;
