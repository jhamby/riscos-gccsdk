#!/usr/bin/perl

# return a Debian packages file entry
# parameters: <package_name> <file_to_look_in>
# output: package entry

use Dpkg::Index;

#print "$#ARGV";
if ($#ARGV != 1) {
  print "usage: dpkg_get_packageentry.pl <package name> <index file>\n";
  exit;
}

my $package_name = $ARGV[0];
my $index_name = $ARGV[1];
#print "$package_name";

# create a new Dpkg index to hold our packages index
my $index = Dpkg::Index->new();#type=CTRL_INDEX_PKG);

# create search criteria that we're going to apply to the index
my %criteria = ();
# Make a criterion that:
# Looks at the 'Directory' field
# then applies the regular expression .*\/{package_name}$ to it,
# where {package_name} is the name of the package we're looking up
# for example it might be .*/joe$ matches a Directory field that ends in
# '/joe'
$criteria{ 'Directory' }  = qr/.*\/\Q$package_name\E$/m;

# Load and parse the full index file
$index->load("$index_name");

# Search it for what we want
my @search_results = $index->get(%criteria);

# Did we get lucky?
if (@search_results)
{
  print STDERR "Autobuilder: found Directory: $package_name in $index_name\n";
  print @search_results;
  exit;
}

@search_results = ();
# if we didn't, carry on

#print $index->get_by_key("$package_name");

# We didn't find a directory, so now let's look in the binary field
# reset the search criteria
%criteria = ();
# now search for Binary entries containing our package name
# surrounded by whitespaces or commas
#$criteria{ 'Binary' }  = qr/[\s,^]\Q$package_name\E[\s,$]/m;
# the above turns out to be a bit awkward with the ^ and $
# so use a function to search instead
$criteria{'Binary'} = \&searchbinary;
# search again
@search_results = $index->get(%criteria);

if (@search_results)
{
  print STDERR "Autobuilder: found Binary: $package_name in $index_name\n";
  print @search_results;
  exit;
}

#print @search_results;
#print searchbinary;

# split up each field on whitespace and commas, and search each
# one for what we want
sub searchbinary {
  ($field) = @_;
#  print "$field";
  my @members = split(/[\$\s,\^]/, $field);
#  print "$package_name = @members ";
#  my @search = grep(/\Q$package_name\E/, @members);
  return grep(/^$package_name$/, @members);
#  print "Found in @search!\n" if @search;
#  return true if @search;
#  return false;
}
