#!/usr/bin/perl
# Install header files into Unix and RISC OS distributions
# Copyright (c) 2000 Nick Burrett
#
# Syntax:
#   install-headers.pl <base dir> <header> <dest directory> <unix dir> <riscos dir>
$basedir = $ARGV[0];
$hdrarg = $ARGV[1];
$destdir = $ARGV[2];
$unixpath = $ARGV[3];
$ropath = $ARGV[4];

# Create installation directories, if they don't already exist
mkdir ("$unixpath/$destdir", 0755) if (! -d "$unixpath/$destdir");
mkdir ("$ropath/$destdir", 0755) if (! -d "$ropath/$destdir");

@header_list = glob ("$basedir/$hdrarg");
foreach $hdr (@header_list) {
  $hdr =~ s!$basedir/!!;

  # Get suffix
  $sfx = $hdr;
  $sfx =~ s/.+\.//;

  $sfx = "" if ($sfx eq $hdr);

  # Get filename without suffix
  $basename = $hdr;
  $basename =~ s/\..$//;

  mkdir ("$ropath/$destdir/$sfx", 0755) if (! -d "$ropath/$destdir/$sfx");

  #printf ("cp '%s' '%s'\n", "$basedir/$hdr", "$unixpath/$destdir/$hdr");
  system ("cp", "$basedir/$hdr", "$unixpath/$destdir/$hdr");
  #printf ("cp '%s' '%s'\n", "$basedir/$hdr", "$ropath/$destdir/$sfx/$basename");
  system ("cp", "$basedir/$hdr", "$ropath/$destdir/$sfx/$basename");
}
exit 0;
