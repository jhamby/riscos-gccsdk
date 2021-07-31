/*
    This utility has been adapted from the LogarithmicTable block in DigiRendV,
    the BASIC code that once was run to construct older versions of the module.

    NB: When run, this utility builds the table in memory then dumps it to stdout!
        So be sure to redirect stdout to a file where you want the table saved.
    --
    Christopher Martin (10 March 2012)
*/


#include <stdio.h>


/*
    Logarithmic table (8bit ulaw -> 16bit signed linear) for maximum volume (128).
    Real maximum volume is 127, therefore the maximum value in the table is a little
    higher (32961), but after volume-scaling it's fine (32961*127/128 = 32703)
*/

#define NUMSAMPLES (128)

int main(void) {
    double sampstep = 4.17;
    double samppos = sampstep / 2.;

    unsigned table[NUMSAMPLES];
    for (int i = 0; i < NUMSAMPLES; ++i) {
        table[i] = (unsigned)samppos;
        if (!(i & 15)) sampstep *= 2.0;
        samppos += sampstep;
    }

    if (fwrite(table, sizeof(unsigned), NUMSAMPLES, stdout) != NUMSAMPLES) {
        perror(NULL);
        return 1;
    }
    return 0;
}
