#!/usr/bin/perl

# Script to patch a QMake created makefile where the native compiler
# is required, but the cross compiler has been used.

use strict;
use warnings;

if ($#ARGV + 1 != 1)
  {
    printf "Expected an argument in change-compiler.pl perl script";
  }

my $file = $ARGV[0];
my $backup_file = "$ARGV[0].bak";

rename $file, $backup_file;

open(my $in,  "<",  $backup_file)  or die "Can't open $backup_file: $!";
open(my $out, ">",  $file) or die "Can't open $file: $!";

while (<$in>)
  {
    my $line = $_;
    if ($line =~ /CC +=.+arm-unknown-riscos-gcc$/)
      {
	print $out "CC            = /usr/bin/cc\n";
      }
    elsif ($line =~ /CXX +=.+arm-unknown-riscos-g\+\+$/)
      {
	print $out "CXX           = /usr/bin/c++\n";
      }
    elsif ($line =~ /LINK += g\+\+/)
      {
        print $out "LINK          = /usr/bin/c++\n";
      }
    else
      {
	print $out $line;
      }
  }

close $in;
close $out;
