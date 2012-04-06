#!/usr/bin/perl
# Script to patch libtool so that it knows about the RISC OS dynamic linker
# patch-libtool.p <filename-out> <filename-in>

use strict;
use warnings;

if ($#ARGV + 1 != 2)
  {
    die "Expected two arguments in patch-libtool perl script\n";
  }

my $out_file = $ARGV[0];
my $in_file = $ARGV[1];

open(my $in,  "<",  $in_file)  or die "Can't open $in_file: $!";
open(my $out, ">",  $out_file) or die "Can't open $out_file: $!";

while (<$in>)
  {
    my $line = $_;
    # Look for a line ending with *) and having only white space before it.
    if ($line =~ /^\s*\*\)$/)
      {
	my $next_line = <$in>;
	if ($next_line =~ /\s*dynamic_linker\=no/)
	  {
	    print $out "\*riscos\)\n";
	    print $out "  version_type\=linux\n";
	    print $out "  library_names_spec\=\'\$\{libname\}\$versuffix.so \$\{libname\}.so.\$major \$libname.so\'\n";
	    print $out "  soname_spec\=\'\$\{libname\}.so.\$major\'\n";
	    print $out "  dynamic_linker\=\'RISC OS ld-riscos.so\'\n";
	    print $out "  shlibpath_var\=LD_LIBRARY_PATH\n";
	    print $out "  ;;\n";
	    print $out "\n";
	  }
	print $out $line;
	print $out $next_line;
      }
    else
      {
	print $out $line;
      }
  }
