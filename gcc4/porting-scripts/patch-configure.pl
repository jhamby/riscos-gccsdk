#!/usr/bin/perl
# Script to patch configure so that it knows about the RISC OS dynamic linker.
#
# patch-configure.pl <filename-out> <filename-in>
#
# Look for specific lines in the configure script and insert relevant sections for
# RISC OS.

use strict;
use warnings;

if ($#ARGV + 1 != 2)
  {
    printf "Expected two arguments in patch-configure.pl perl script";
  }

my $out_file = $ARGV[0];
my $in_file = $ARGV[1];

open(my $in,  "<",  $in_file)  or die "Can't open $in_file: $!";
open(my $out, ">",  $out_file) or die "Can't open $out_file: $!";

while (<$in>)
  {
    my $line = $_;
    if ($line =~ /^rdos\*\)$/)
      {
	my $next_line = <$in>;
	if ($next_line =~ /\s*dynamic_linker\=no/)
	  {
	    print $out "riscos\*\)\n";
	    print $out "  version_type\=linux\n";
	    print $out "  library_names_spec\=\'\$\{libname\}\$\{release\}\$\{shared_ext\}\$versuffix \$\{libname\}\$\{release\}\$\{shared_ext\}\$\{major\} \$\{libname\}\$\{shared_ext\}\'\n";
	    print $out "  soname_spec\=\'\$\{libname\}\$\{release\}\$\{shared_ext\}\$major\'\n";
	    print $out "  shlibpath_var\=LD_LIBRARY_PATH\n";
	    print $out "  shlibpath_overrides_runpath\=no\n";
	    print $out "  hardcode_into_libs=yes\n";
	    print $out "  ;;\n";
	    print $out "\n";
	  }
	elsif ($next_line =~ /\s*lt_cv_deplibs_check_method\=pass_all/)
	  {
	    print $out "riscos\*\)\n";
	    print $out $next_line;
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

close $in;
close $out;
