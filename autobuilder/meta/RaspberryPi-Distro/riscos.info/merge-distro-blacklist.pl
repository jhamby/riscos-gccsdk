#!/bin/perl

#use strict;
#use warnings;
#use WWW::Mechanize;
use LWP::Simple;

my $riscpkg = 'http://www.riscpkg.org/pkg/Unstable';
#my $riscpkg = 'http://www.cl.cam.ac.uk/~atm26/ephemeral/Simple';
my $riscosinfo = 'http://www.riscos.info/packages/pkg/autobuilt';

my @blacklist = ( 'firefox-riscpc', 'GCC', 'CPP-2.95', 'GCC-2.95', 'G++',
  'Ld-GCCSDK', 'CPP', 'GCC-Common', 
  'G++-2.95', 'NettleSSH', 'GCC-2.95-Doc', 'DrWimp-4.50', 'DrWimp-4.60',
  'StrongED', 'StrongED-Cfg', 'StrongED-Modes-Assembly', 'StrongED-Modes-Document',
  'StrongED-Modes-Internet', 'StrongED-Modes-Misc', 'StrongED-Modes-Program',
  'StrongED-Modes-RISCOS', 'StrongED-Modes-Scripting',
  'RiscTerm',
  'MagicMush', 'FTPc', # not armv7
  'bison', 'flex', 'm4', # package name clash
  'RiscPkg',  # won't handle firmware updates correctly
  'Manuals', # Misc v Miscellaneous sections - tidies the index (ROOL version in Miscellaenous)
  'BumpRace',  # sprite file issue
  'Alienblaster', 'Anagramarama', 'Flobopuyo', 'Defendguin', # crashes
  'Arcem', # missing UnixHome dependency
  'Bc', # missing description
  'Fillets-Ng', 'Fillets-Ng-Data', # huge, install breaks
  'GCC4','GCC4-C++', # thrashes SD until it crashes
  'UnZip' # new package is 'Unzip'
  );

# fetch the packages file
my $riscpkg_content = get $riscpkg;
my $riscosinfo_content = get $riscosinfo;

# turn content relative URLs into absolute paths
$riscpkg_content =~ s/URL: \.\.\//URL: http:\/\/www\.riscpkg\.org\//g;
$riscosinfo_content =~ s/URL: \.\.\//URL: http:\/\/www\.riscos\.info\/packages\//g;

# fix up description for Nettle to mention SSH 2
$riscosinfo_content =~ s|and telnet client|and telnet/SSH2 client|g;

# split into individual package entries

%riscpkg_entries = parse_packages($riscpkg_content);
%riscosinfo_entries = parse_packages($riscosinfo_content);

#print %riscpkg_entries;
#print %riscosinfo_entries;

# look for clashes
foreach (keys %riscosinfo_entries) {
  $intersection{$_} = $riscosinfo_entries{$_} if exists $riscpkg_entries{$_};
}

if ( %intersection ) {
#  print "Clash of packages ";
  foreach (keys %intersection) {
#  print "$_\n";
  }
}

# merge the two databases, riscos.info entries taking preference in case of
# duplicates

%total = ();
while ( ($k,$v) = each (%riscpkg_entries) ) {
  $total{$k} = $v;
}
while ( ($k,$v) = each (%riscosinfo_entries) ) {
  $total{$k} = $v;
}

# remove everything on the blacklist
foreach my $removal (@blacklist) {
  delete $total{$removal};
}

# dump the resulting package list
while ( ($k,$v) = each (%total) ) {
  print "$v\n\n";
}

# THE END


sub parse_packages
{
  # input is a text variable containing the packages file
  $content = $_[0];
  
  my %entries = ();

  my @package_entries = split('\n\n', $content);
  foreach my $pkg (@package_entries) {
#  $pkg=$package_entries[0];
    my @package_lines = split('\n',$pkg);
    foreach my $pkg_line (@package_lines) {
      if ( $pkg_line =~ /Package/ )
      {
#  }
        $pkg_line =~ /Package: (.*)/;
        $package_name = $1;
#        print "$package_name\n";
        $entries { $package_name } = $pkg;
      
#    print $pkg_line;
#  print "\n\n";

#  my %package_hash = split("Package: ", $pkg_line);
  
#  foreach my $k (keys %package_hash) {
    
#    print "$k=$package_hash{$k}\n";
#  }
        }

#print $content;
      }
    }
  return %entries;
}
#print %riscpkg_entries;
#while ( my ($k,$v) = each %$riscpkg_entries ) {
#  print "BEGIN key: $k, value: $v END\n";
#}
