#!/usr/bin/perl
# Crude script for compiling GCC tests ready for execution
# on RISC OS.
# Written by Nick Burrett <nick@dsvr.net>
use Getopt::Long;

GetOptions ("test=s" => \$testtype);

if ($testtype eq "c" or $testtype eq "c++") {
    $dir = "gcc-tests/$testtype";
    $dst = "testdir/$testtype";
    $comp = "./3.3/riscos-cross/bin";

    open OUT, ">$dst/runtest,feb";
    opendir (DIR, "$dir");
    while ($entry = readdir (DIR)) {
	next if (-d "$dir/$entry");
	next if ($entry eq ".");
	next if ($entry eq "..");
	next if ($entry =~ /\.x$/);
	$exec = $entry;
	$exec =~ s/\.(c|C)//;
	printf "%s\n", $entry;
	if ($testtype eq "c") {
	    $status = system ("./3.3/riscos-cross/bin/gcc", "$dir/$entry",
			      "-O2", "-o", "$dst/$exec", "-mpoke-function-name");
	} elsif ($testtype eq "c++") {
	    $status = system ("./3.3/riscos-cross/bin/g++", "$dir/$entry",
			      "-O2", "-o", "$dst/$exec", "-mpoke-function-name");
	}
	if ($status == 0) {
	    printf OUT "echo %s\n", $entry;
	    printf OUT "run %s\n", $exec;
	}
    }
    closedir (DIR);
    close OUT;
} elsif ($testtype eq "libstdc") {
    # The libstdc++ testsuite consists of directories within directories
    $testdir = "gcc-3.3/libstdc++-v3/testsuite";
    $dst = "testdir/libstdc";

    open OUT, ">$dst/runtest,feb";

    @files = `find $testdir -name "*.cc"`;
    foreach (@files) {
	chomp;
	$srcfile = $_;
	$objfile = `basename $srcfile .cc`;
	chomp $objfile;
	printf ("srcfile = '%s'\n", $objfile);
	$status = system ("/rogcc/3.3/riscos-cross/bin/g++", $srcfile,
			  "-O1", "-o", "$dst/$objfile",
			  "-mpoke-function-name",
			  "-I", "$testdir",
			  "$testdir/testsuite_allocator.cc",
			  "$testdir/testsuite_hooks.cc", "-mlibscl");
	if ($status == 0) {
	    printf OUT "echo %s\n", $entry;
	    printf OUT "run %s\n", $exec;
	}
    }
    close OUT;
}



