#if 0
To: nickb@digibank.demon.co.uk
Subject: libio bug (integer input)
Date: Mon, 10 Feb 1997 15:27:54 +0000
Message-ID: <1274.855588474@cs.ucl.ac.uk>
From: Francis DEVEREUX <F.Devereux@cs.ucl.ac.uk>


Dear Mr Burrett.
        I think that there is a bug in !libio for Acorn RISCOS.  When I compile
and run the following program:
#endif

// bug.cc

#include <iostream.h>

int main()
{
    int x;
    cin >> x;
    cout << x << endl;
    return 0;
}

#if 0
using the commands
        g++ -c -Ilibio: cc.bug
        drlink -o bug o.bug libio:o.libiostrea gcc:o.libgcc unix:o.UnixLib
        bug
then whatever number I input the program always prints 0.
        I think that the problem is that the input to x always causes 0 to be
assigned to x because of what happens in other programs.
        A similar thing happens when I try to read ints from file streams,
however the bug doesn't seem to affect floats, doubles etc.  If you want I can
also send you the preprocessor output and the absolute file that are produced.

        I use gcc 2.7.2.1 v1.0.7, drlink 0.28, unixlib 3.7b and a version of
!libio with !libio.o.libiostrea date stamped 12 November 1996 and I use an
A5000 with RISC OS 3.10.

Francis Devereux
f.devereux@cs.ucl.ac.uk
#endif
