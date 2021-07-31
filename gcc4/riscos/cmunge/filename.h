/*******************************************************************
 * File:     filename
 * Purpose:  Filename translations for supporting unix-style naming
 *           schemes
 * Author:   Justin Fletcher
 * Date:     20 Jul 2005
 * Note:     The indention of this file is to support the most general
 *           forms of names that might be passed to commands that expect
 *           (or are expected to expect) unix-style filenames. This
 *           means that simple extensions will be translated from the
 *           unix-style format to the RISC OS format, eg file.c becomes
 *           c.file.
 *           The extensions which will be processed by this operation
 *           are described through a parameter to the functions.
 ******************************************************************/

#ifndef FILENAME_H
#define FILENAME_H

/* These extension lists are defined for ease of use as being the common
   extensions supported by the various tools. */
#define EXTLIST_NORCROFT_CMHG     "c:c++:cmhg:h:o:p:s"
#define EXTLIST_NORCROFT_CC       "a:A:c:C:h:H:o:O:s:S:c++:C++"
#define EXTLIST_NORCROFT_PC       "p:P:h:H"
#define EXTLIST_NORCROFT_OBJASM   "a:A:c:C:h:H:o:O:s:S"
#define EXTLIST_NORCROFT_AMU      "aof:o:s:c:c++:f:p:l:h:x:bas:pas:cmhg"
#define EXTLIST_UNIXLIB           "a:c:cc:f:h:i:ii:l:o:p:s:y"
#define EXTLIST_GNU_GCC           "f:for:F:fpp:cc:cxx:cpp:c++:C:i:ii:rpo:c:m:h:hh:s:S:xrb:xrs:l:o:y:tcc"
/* The UNIXLIB definition is used where no variable has been set for suffix
   swapping.
   The GNU GCC definition list above is the default set by the GCC
   resources for the GCC tool.
   They are provided as advisories only.
 */

/* Note: Details taken from :
            CC      5.09 (binary)
            CMHG    5.20 (binary)
            ObjAsm  3.27 (binary)
            PC      4.10 (binary)
            AMU     5.24 (binary)
            GCC     (documentation sent by John Tytgat)
            UnixLib (documentation sent by John Tytgat)

         The following do not appear to have special code for processing
         extensions :
            AAsm
            CFront
            C++
            Link
            LibFile
 */


/*************************************************** Gerph *********
 Function:     filename_unixtoriscos
 Description:  Convert a filename from Unix-style to RISC OS
 Parameters:   src-> the source string to convert (unix style)
               extlist-> a colon separated list of extensions to
                         reverse
 Returns:      pointer to a static string
 ******************************************************************/
char *filename_unixtoriscos(const char *src, const char *extlist);

/*************************************************** Gerph *********
 Function:     filename_riscostounix
 Description:  Convert a filename from RISC OS-style to Unix-style
 Parameters:   src-> the source string to convert (RISC OS style)
               extlist-> a colon separated list of extensions to
                         reverse
 Returns:      pointer to a static string
 ******************************************************************/
char *filename_riscostounix(const char *src, const char *extlist);

#endif
