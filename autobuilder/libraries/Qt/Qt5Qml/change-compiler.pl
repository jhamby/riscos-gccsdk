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
    if ($line =~ /CC +=.+arm-riscos-gnueabihf-gcc$/)
      {
	print $out "CC            = /usr/bin/cc\n";
      }
    elsif ($line =~ /CXX +=.+arm-riscos-gnueabihf-g\+\+$/)
      {
	print $out "CXX           = /usr/bin/c++\n";
      }
    elsif ($line =~ /LINK +=.+arm-riscos-gnueabihf-g\+\+/)
      {
        print $out "LINK          = /usr/bin/c++\n";
      }
    else
      {
	$line =~ s/\-lOSLib32/\-L\/usr\/local\/Qt-5.5.1\/lib/;
	print $out $line;
      }
  }

close $in;
close $out;
