#!/usr/bin/perl
# Install source files into the RISC OS distribution
# Copyright (c) 2000, 2002 Nick Burrett
#
# Syntax:
#   install-headers.pl <base dir> <header> <dest directory> <riscos dir>
$basedir = $ARGV[0];
$hdrarg = $ARGV[1];
$destdir = $ARGV[2];
$ropath = $ARGV[3];

# Create installation directories, if they don't already exist
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
  $basename =~ s/\..+$//;

  mkdir ("$ropath/$destdir/$sfx", 0755) if (! -d "$ropath/$destdir/$sfx");

  #printf ("cp '%s' '%s'\n", "$basedir/$hdr", "$ropath/$destdir/$sfx/$basename");
  system ("cp", "$basedir/$hdr", "$ropath/$destdir/$sfx/$basename");
}
exit 0;
