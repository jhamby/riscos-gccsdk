#!/usr/bin/perl
# Convert GAS format assembly into AOF format
# Used only to convert the ieee754 assembler files for fast
# floating point operations.

if (@ARGV != 2) { die "syntax: aof-convert.pl <input> <output>"; }
$srcfile = $ARGV[0];
$dstfile = $ARGV[1];
open IN, "$srcfile";
open OUT, "> $dstfile";
print OUT<<EOF;
a1 RN 0
a2 RN 1
a3 RN 2
a4 RN 3
v1 RN 4
v2 RN 5
v3 RN 6
v4 RN 7
v5 RN 8
v6 RN 9
sl RN 10
fp RN 11
ip RN 12
sp RN 13
lr RN 14
pc RN 15
f0 FN 0
f1 FN 1
f2 FN 2
f3 FN 3
f4 FN 4
f5 FN 5
f6 FN 6
f7 FN 7
r0 RN 0
r1 RN 1
r2 RN 2
r3 RN 3
r4 RN 4
r5 RN 5
r6 RN 6
r7 RN 7
r8 RN 8
r9 RN 9
r10 RN 10
r11 RN 11
r12 RN 12
r13 RN 13
r14 RN 14

	AREA |C\$\$code|, CODE, READONLY
EOF

while (<IN>) {
    s/FUNC_END.*//; # Destroy all FUNC END
    s/@/;/;  # Change comments from @ to ;
    s/LSYM\((.*)\)/$1/; # Remove LSYM(...)
    s/^(.*):/$1/; # Remove colon from labels
    s/RETc\(([a-z]*)\)/mov$1\tpc, lr/; # Convert RETc into a mov(c) pc, lr
    s/RETLDM.*\"(.*)\".*/ldmfd\tsp!, {$1, pc}/; # LDMFD return
    s/RETLDM/ldmfd\tsp!, {pc}/;
    s/RET/mov\tpc, lr/; # Convert RET to mov pc, lr

    # Convert local relative labels.
    s/([ \t])([0-9])b/$1%B0$2/;
    s/([ \t])([0-9])f/$1%F0$2/;
    s/^([0-9])/0$1/;

    # Change ARM_FUNC_START to an AOF function definition
    if (/ARM_FUNC_START/) {
	($arg) = ($_ =~ /ARM_FUNC_START (.*)/);
	print OUT "\tEXPORT\t|__$arg|\n";
	print OUT "|__$arg|\tROUT\n";
    } elsif (/ARM_FUNC_ALIAS/) {
	($arg1, $arg2) = ($_ =~ /ARM_FUNC_ALIAS (.*) (.*)/);
	print OUT "\tEXPORT\t|__$arg1|\n";
	print OUT "|__$arg1|\n";
    } else {
	print OUT $_;
    }
}
print OUT "\n\tEND\n\n";
close OUT;
close IN;

