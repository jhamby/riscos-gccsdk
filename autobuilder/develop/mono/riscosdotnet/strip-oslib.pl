#!/usr/bin/perl
# Script to strip from oslib generated assembler source, those parts
# that are not required.

use warnings;

if ($#ARGV + 1 != 3)
  {
    die "Syntax: oslib-strip <file-to-strip> <result-file> <prefix-to-search-for>\n";
  }

my $in_file = $ARGV[0];
my $out_file = $ARGV[1];
my $prefix = $ARGV[2];

open(my $in,  "<",  $in_file)  or die "Can't open $in_file: $!";
open(my $out, ">",  $out_file) or die "Can't open $out_file: $!";

while (<$in>)
  {
    my $line = $_;

    # Strip declarations first
    if ($line =~ /.global $prefix/ || ($line =~ /.global/ && $line =~ /service/)) {
      $line = <$in>;
    } elsif ($line =~ /^$prefix\S+:/ || $line =~ /service/) {
      while (1)
	{
	  $line = <$in>;
	  # When we find a .size directive, we've found the end of the function.
	  if ($line =~ /.size/) {
	    # Remove any comment after the .size directive - doesn't matter
	    # if it's just a blank line.
	    $line = <$in>;
	    last;
	  }
	}
    } else {
      print $out $line;
    }
  }
