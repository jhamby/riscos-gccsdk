C+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+
C+                                                                     +
C+                        FILE sssu01.FOR                              +
C+                                                                     +
C+   CONTAINS DRIVER FOR LOWESTOFT VERSION OF THE CONTOURING PROGRAM   +
C+                                                                     +
C+       ROUTINES IN FILE ORDER             ALPHABETICAL ORDER         +
C+   HSU01  : LINES    13 TO  1176    HSU01  : LINES    13 TO  1176    +
C+                                                                     +
C+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+:+
C+*+* HSU01  START  +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
C
C
C
      PROGRAM HSU01
C
C     ******************************************************************
C     *                                                                *
C     *    PROGRAM HSU01                                               *
C     *                                                                *
C     *        DRIVER FOR CONTOURING PACKAGE                           *
C     *                                                                *
C     ******************************************************************
C
C                                                                      C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C                                                                      C
C             CONTOURING PACKAGE STRUCTURE                             C
C                                                                      C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C                                                                      C
C      FILE                         COMMENT                            C
C      ====                         =======                            C
C                                                                      C
C   SSSU00.FOR  Contains the driver for the Salford version of the     C
C               contouring program (SCONT)                             C
C               Used by SCONT                                          C
C                                                                      C
C   SSSU01.FOR  Contains the driver for the Lowestoft version of the   C
C               contouring program, ie without the extra gridding      C
C               facilities (LCONT)                                     C
C               Used by LCONT                                          C
C                                                                      C
C   SSSU02.FOR  Contains the driver for the control file generator     C
C               program (CFG)                                          C
C               Used by CFG                                            C
C                                                                      C
C   SSSU03.FOR  Contains the driver for the plotter program (PLOT)     C
C               Used by PLOT                                           C
C                                                                      C
C   SSSU04.FOR  Contains the driver for the profiler program (PROF)    C
C               Used by PROF                                           C
C                                                                      C
C                       cccccccccccc                                   C
C                                                                      C
C   SSSU05.FOR  Contains miscellaneous utility subprograms required    C
C               by all parts of the package                            C
C               Used by SCONT, LCONT, CFG, PLOT, PROF                  C
C                                                                      C
C   SSSU06.FOR  Contains miscellaneous utility subprograms required    C
C               only by the contouring programs                        C
C               Used by SCONT, LCONT                                   C
C                                                                      C
C                       cccccccccccc                                   C
C                                                                      C
C   SSSU10.FOR  Contains driver and auxiliary subprograms for the      C
C               initial user interface of the contouring program and   C
C               the control file generator, also sets up control       C
C               common from user dialogue or from control file         C
C               Used by SCONT, LCONT, CFG                              C
C                                                                      C
C   SSSU11.FOR  Contains auxiliary subprograms for the user interface  C
C               relating to questions about gridding                   C
C               Used by SCONT, LCONT, CFG                              C
C                                                                      C
C   SSSU12.FOR  Contains subprograms for creating control files from   C
C               control common for the contouring program              C
C               Used by SCONT, LCONT, CFG                              C
C                                                                      C
C   SSSU13.FOR  Contains driver and auxiliary subprograms for the      C
C               redraw user interface of the contouring program        C
C               Used by SCONT, LCONT                                   C
C                                                                      C
C   SSSU17.FOR  Contains driver and auxiliary subprograms for the      C
C               initial user interface of the profiler program, also   C
C               sets up control common from user dialogue or from      C
C               control file                                           C
C               Used by PROF                                           C
C                                                                      C
C   SSSU18.FOR  Contains subprograms for creating control files from   C
C               control common for the profiler program                C
C               Used by PROF                                           C
C                                                                      C
C   SSSU19.FOR  Contains driver and auxiliary subprograms for the      C
C               redraw user interface of the profiler program          C
C               Used by PROF                                           C
C                                                                      C
C                       cccccccccccc                                   C
C                                                                      C
C   SSSU20.FOR  Contains drivers for the various contourer gridding    C
C               methods                                                C
C               Used by SCONT, LCONT                                   C
C                                                                      C
C   SSSU21.FOR  Contains Lowestoft ordinary gridder, supergridder and  C
C               hypergridder subprograms                               C
C               Used by SCONT, LCONT                                   C
C                                                                      C
C   SSSU22.FOR  Contains various contourer gridding subprograms        C
C               Used by SCONT                                          C
C                                                                      C
C   SSSU23.FOR  Contains various contourer gridding subprograms        C
C               Used by SCONT                                          C
C                                                                      C
C   SSSU24.FOR  Contains various contourer gridding subprograms        C
C               Used by SCONT                                          C
C                                                                      C
C   SSSU25.FOR  Contains various contourer gridding subprograms        C
C               Used by SCONT                                          C
C                                                                      C
C   SSSU26.FOR  Contains program stubs to replace gridding subprograms C
C               not in use at Lowestoft                                C
C               Used by LCONT                                          C
C                                                                      C
C   SSSU27.FOR  Contains driver and subprograms interfacing to the     C
C               NAG spline routines                                    C
C               Used by SCONT, LCONT                                   C
C                                                                      C
C   SSSU29.FOR  Contains driver and subprograms for profiler program   C
C               data fitting on an irregular rectangular grid          C
C               Used by PROF                                           C
C                                                                      C
C                       cccccccccccc                                   C
C                                                                      C
C   SSSU30.FOR  Contains contouring driver and auxilary subprograms    C
C               Used by SCONT, LCONT                                   C
C                                                                      C
C   SSSU31.FOR  Contains contourer auxiliary subprograms               C
C               Used by SCONT, LCONT                                   C
C                                                                      C
C   SSSU32.FOR  Contains Harwell contourer subprograms                 C
C               Used by SCONT, LCONT                                   C
C                                                                      C
C   SSSU33.FOR  Contains line-printer contourer subprograms            C
C               Used by SCONT, LCONT                                   C
C                                                                      C
C   SSSU34.FOR  Contains PF file creation subprograms                  C
C               Used by SCONT, LCONT                                   C
C                                                                      C
C   SSSU35.FOR  Contains area and centre of mass subprograms           C
C               Used by SCONT, LCONT                                   C
C                                                                      C
C   SSSU39.FOR  Contains profiler contouring subprograms               C
C               Used by PROF                                           C
C                                                                      C
C                       cccccccccccc                                   C
C                                                                      C
C   SSSU40.FOR  Contains previewing and plotting driver and auxiliary  C
C               subprograms                                            C
C               Used by SCONT, LCONT, PLOT, PROF                       C
C                                                                      C
C   SSSU41.FOR  Contains interface subprograms to GKS                  C
C               Used by SCONT, LCONT, PLOT, PROF                       C
C                                                                      C
C                                                                      C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
 
C ******************************************************************** C
C *                                                                  * C
C *          KEY TO CONTROL COMMON                                   * C
C *          Last revised   25 MAR  1988                             * C
C *                                                                  * C
C ******************************************************************** C
C *                                                                  * C
C *     COMMON BLOCK /CHARS/   Character variables                   * C
C *     --------------------                                         * C
C *         TITLE -- FROM SOURCE DATA FILE                           * C
C *         XTITLE,YTITLE,ZTITLE - PLOT TITLES                       * C
C *         XUNITS,YUNITS,ZUNITS - USED IN ISOMETRIC KEY             * C
C *         NAMES  -- ARRAY, MAINLY USED FOR FILENAMES               * C
C *                                                                  * C
C *     COMMON BLOCK /CNTRL/  Logical, integer and real variables    * C
C *     --------------------                                         * C
C *         LGSV -- LOGICAL ARRAY FOR SAVE PURPOSES                  * C
C *         LU -- LOGICAL UNIT NUMBER OF INPUT DEVICE                * C
C *         LL -- LOGICAL UNIT NUMBER OF LISTING DEVICE              * C
C *         LR -- LOGICAL UNIT NUMBER OF REPORT FILE                 * C
C *         QNO-- CURRENT QUESTION NUMBER IN SSSU10 AND SSSU13       * C
C *         IN -- INTEGER ARRAY FOR UNIT NOS,FLAGS,ETC               * C
C *         IDD AND RDD -- ARRAYS FOR DATA DESCRIPTION               * C
C *         ICS AND RCS -- ARRAYS FOR CONTOURING DETAILS             * C
C *         IGG AND RGG -- ARRAYS FOR GRID GENERATION DETAILS        * C
C *         IKN AND RKN -- ARRAYS FOR SPLINE DETAILS                 * C
C *         IPS AND RPS -- ARRAYS FOR PLOTTING DETAILS               * C
C *      THE FILE CMNFIL IS COMPOSED OF ALL THE ABOVE                * C
C *      EXCEPT FOR  LU,LL,LR,QNO                                    * C
C *                                                                  * C
C ******************************************************************** C
C ******************************************************************** C
C *                                                                  * C
C *   NAMES ARRAY  - size 20 - names of files and other strings      * C
C *   -----------                                                    * C
C *   NAMES(1)  Source datafile                                      * C
C *   NAMES(2)  Working datafile (may be the same as NAMES(1))       * C
C *   NAMES(3)  Gridded datafile (may be the same as NAMES(1))       * C
C *   NAMES(4)  Sequence name for batch jobs, e.g A01C05             * C
C *   NAMES(5)  Source coastline file                                * C
C *   NAMES(6)  Coastline file actually used (may be the same as     * C
C *             NAMES(5))                                            * C
C *   NAMES(7)  File containing additional data points to be         * C
C *             superimposed on a contour plot if specified by the   * C
C *             REDRAW facility.                                     * C
C *   NAMES(8)  File used for output of line-printer contours (if    * C
C *             specified)                                           * C
C *   NAMES(9)  Control file if specified.                           * C
C *   NAMES(10) File containing Host-Dependant information -- CTHST  * C
C *   NAMES(11) Batch File                                           * C
C *   NAMES(12) Job/report file name when in HP system batch mode    * C
C *   NAMES(13) Name of HELP file                                    * C
C *   NAMES(14) Name of starting data file for hypergridder          * C
C *   NAMES(15) Holds date and time to be output on plot and in      * C
C *                   report file.                                   * C
C *   NAMES(16) 'Updates' filename                                   * C
C *   NAMES(17) Unused                                               * C
C *   NAMES(18) Unused                                               * C
C *   NAMES(19) Unused                                               * C
C *   NAMES(20) Unused                                               * C
C *                                                                  * C
C ******************************************************************** C
C *                                                                  * C
C *   LGSV ARRAY   - size 30 , used to SAVE various logical flags    * C
C *   ----------                                                     * C
C *   LGSV(1)  = OPEN      in ZREAD in SSSU05                        * C
C *   LGSV(2)  = FIRST     in ZREAD in SSSU05                        * C
C *   LGSV(3)  = FIRST     in BATCH in SSSU10                        * C
C *   LGSV(4)  = Unused                                              * C
C *   LGSV(5)  = FIRST     in IPLOT in SSSU30                        * C
C *   LGSV(6)  = FIRST     in CHSIZ in SSSU31                        * C
C *   LGSV(7)  = Unused                                              * C
C *   LGSV(8)  = REPEAT flag                                         * C
C *   LGSV(9)  = BEOF   batch file exhausted flag                    * C
C *   LGSV(10) = BATCH  flag                                         * C
C *   LGSV(11) = STEP flag                                           * C
C *   LGSV(12) = Unused                                              * C
C *   LGSV(13) = FIRST     in DVICE in SSSU05                        * C
C *   LGSV(14) = Unused                                              * C
C *   LGSV(15) = FRAME     throughout SSSU40                         * C
C *   LGSV(16) = NWFRAM    throughout SSSU40                         * C
C *   LGSV(17) = Unused                                              * C
C *   LGSV(18) = Unused                                              * C
C *   LGSV(19) = Unused                                              * C
C *   LGSV(20) = Unused                                              * C
C *   LGSV(21) = Unused                                              * C
C *   LGSV(22) = Unused                                              * C
C *   LGSV(23) = Unused                                              * C
C *   LGSV(24) = Unused                                              * C
C *   LGSV(25) = Unused                                              * C
C *   LGSV(26) = Unused                                              * C
C *   LGSV(27) = Unused                                              * C
C *   LGSV(28) = Unused                                              * C
C *   LGSV(29) = Unused                                              * C
C *   LGSV(30) = Unused                                              * C
C *                                                                  * C
C ******************************************************************** C
C *                                                                  * C
C *   IN ARRAY     - size 40 - unit numbers, flags, etc.             * C
C *   --------                                                       * C
C *   IN(1)     Flag set to 1/2/3                                    * C
C *                1  control common created interactively           * C
C *                2  control file read from disc                    * C
C *                3  control file read from paper tape              * C
C *   IN(2)     Terminal number (usually LU, LL)                     * C
C *   IN(3)     Flag set 0/1 for redrawing not specified/specified   * C
C *   IN(4)     Flag set 0/1/2 for                                   * C
C *             non-existence/existence/redundancy of file PFCTR     * C
C *   IN(5)     Flag set 0/1/2 for                                   * C
C *             non-existence/existence/redundancy of file PFAXS     * C
C *   IN(6)     Flag set 0/1/2 for                                   * C
C *             non-existence/existence/redundancy of file PFCST     * C
C *   IN(7)     Flag set 0/1/2 for                                   * C
C *             non-existence/existence/redundancy of file PFDTA     * C
C *   IN(8)     Flag set 0/1/2 for                                   * C
C *             non-existence/existence/redundancy of file PFADD     * C
C *   IN(9)     Flag set 0/1/2 for                                   * C
C *             non-existence/existence/redundancy of file PFKEY     * C
C *   IN(10)    Set to 1 when REPORT file opened                     * C
C *   IN(11)    Count of non-fatal error messages sent to REPORT     * C
C *             FILE                                                 * C
C *   IN(12)    Flag set to 0/1/2 for area/centre of mass            * C
C *                0    Not requested                                * C
C *                1    Requested but not yet performed              * C
C *                2    Calculation completed                        * C
C *   IN(13)    Flag set to 0/1/2 according to driver                * C
C *   IN(14)    Flag set to zero unless changes are to be made       * C
C *             interactively in the REDRAW facility                 * C
C *   IN(15)    Flag set to 0/1 to indicate whether currently in     * C
C *             INTERACTIVE/NON-INTERACTIVE mode. This will depend   * C
C *             on the type of control common creation and on        * C
C *             whether batch mode is being used.                    * C
C *   IN(16)    Pointer to last '**REPEAT  '  when REPEAT            * C
C *             modifications present                                * C
C *   IN(17)    Flag set to 0/1 to indicate whether, at SALFORD,     * C
C *             the file PICTURE contains hard-copy plots, used in   * C
C *             the plotting program                                 * C
C *   IN(18)    Holds last IOSTAT value from I/O calls               * C
C *   IN(19)    Default cartridge number                             * C
C *   IN(20)    'Tidy up' flag, set in HOST, used in TIDY            * C
C *   IN(21)    Line number for HP plotters at Lowestoft             * C
C *   IN(22)    Flag set to 0/1 for plotting file PFCTR being        * C
C *             not open/open                                        * C
C *   IN(23)    Holds 'home' cartridge no; currently 910             * C
C *   IN(24)    Unused                                               * C
C *   IN(25)    Control File counter when in Batch Mode              * C
C *   IN(26)    Used in C.F.G.  prog                                 * C
C *   IN(27)    |        used for non-local label                    * C
C *   IN(28)    |   EQUIV to REAL*8   at SALFORD                     * C
C *   IN(29)    |   EQUIV to INTEGER  at LOWESTOFT                   * C
C *   IN(30)    |                                                    * C
C *   IN(31)    'start' FORTRAN unit number e.g. 11                  * C
C *   IN(32)    number of FORTRAN units to be made available         * C
C *   IN(33)    Unused                                               * C
C *   IN(34)    Unused                                               * C
C *   IN(35)    Unused                                               * C
C *   IN(36)    Unused                                               * C
C *   IN(37)    Unused                                               * C
C *   IN(38)    Unused                                               * C
C *   IN(39)    Unused                                               * C
C *   IN(40)    Unused                                               * C
C *                                                                  * C
C ******************************************************************** C
C *                                                                  * C
C *   IDD ARRAY    - size 10 - data description : integer parameters * C
C *   ---------                                                      * C
C *   IDD(1)    Data-file-type : 1 = rectangular,                    * C
C *                              2 = scattered,                      * C
C *                              3 = tabular,                        * C
C *   IDD(2)    Column number for tabular data                       * C
C *   IDD(3)    Data-format : 1 = standardised (i.e. X-Y or dec deg) * C
C *                           2 = various LAT/LONG formats           * C
C *   IDD(4)    LAT/LONG numeric format : 1 = decimal degrees        * C
C *                                       2 = degree-decimal minutes * C
C *                                       3 = degree-minutes-sec     * C
C *   IDD(5)    LAT/LONG NSEW format : 1 = 0/1 for N/S and E/W       * C
C *                                    2 = +/- for N/S and E/W       * C
C *                                    3 = N,S,E,W                   * C
C *   IDD(6)    Unused                                               * C
C *   IDD(7)    Total number of function values in 'working' data    * C
C *             file, i.e. valid function values from a scattered    * C
C *             source data file or 'm*n' for a regular data file    * C
C *   IDD(8)    Flag set  0/1 for data in lat-long/abstract x-y      * C
C *             coordinates                                          * C
C *   IDD(9)    Unused                                               * C
C *   IDD(10)   Unused                                               * C
C *                                                                  * C
C ******************************************************************** C
C *                                                                  * C
C *   RDD ARRAY    - size 10 - data description : real parameters    * C
C *   ---------                                                      * C
C *   RDD(1)    Unused                                               * C
C *   RDD(2)    Unused                                               * C
C *   RDD(3)    Xmin                                                 * C
C *   RDD(4)    Xmax                                                 * C
C *   RDD(5)    Ymin                                                 * C
C *   RDD(6)    Ymax                                                 * C
C *   RDD(7)    VALUEmin                                             * C
C *   RDD(8)    VALUEmax                                             * C
C *   RDD(9)    Unused                                               * C
C *   RDD(10)   Unused                                               * C
C *                                                                  * C
C ******************************************************************** C
C *                                                                  * C
C *   ICS ARRAY    - size 10 - contouring : integer parameters       * C
C *   ---------                                                      * C
C *   ICS(1)    Contouring type : 1 = line-printer(bi-linear)        * C
C *                               2 = line-printer(bi-cubic)         * C
C *                               3 = NAG-regular                    * C
C *                               4 = NAG-scattered                  * C
C *                               5 = Harwell                        * C
C *                               6 = Isometric projection           * C
C *                               7 = plotting of sample points only * C
C *                                       -- no contouring           * C
C *   ICS(2)    To be used in coastline/polygon approx               * C
C *   ICS(3)    Region-type : 3  = triangle,                         * C
C *                           4  = rectangle,                        * C
C *                           <n> = n-sided polygon (max(n)=50)      * C
C *   ICS(4)    Number of contours (max allowed for is 10)           * C
C *   ICS(5)    Unused                                               * C
C *   ICS(6)    Unused                                               * C
C *   ICS(7)    Contouring transformation switch :                   * C
C *                          0 = no transformation                   * C
C *                          1 = change of origin                    * C
C *                          2 = standard linear transf              * C
C *                          3 = user-defined linear transf          * C
C *                          4 = standard log transf                 * C
C *                          5 = user-defined log transf             * C
C *   ICS(8)    Pointer to start of x-vertices of polygon            * C
C *                 -- set to 31 in SSSU10 ( in QPOLY )              * C
C *   ICS(9)    Pointer to start of y-vertices of polygon            * C
C *                 -- set to 81 in SSSU10 ( in QPOLY )              * C
C *   ICS(10)   Unused                                               * C
C *                                                                  * C
C ******************************************************************** C
C *                                                                  * C
C *   RCS ARRAY    - size 130 - contouring : real parameters         * C
C *   ---------                                                      * C
C *   RCS(1)    Xmin    |                                            * C
C *   RCS(2)    Xmax    |  PLOTTING                                  * C
C *   RCS(3)    Ymin    |  REGION                                    * C
C *   RCS(4)    Ymax    |                                            * C
C *   RCS(5)    Data precision parameter used when a log             * C
C *             transformation is required                           * C
C *   RCS(6)    Xmin    |                                            * C
C *   RCS(7)    Xmax    |  CONTOURING                                * C
C *   RCS(8)    Ymin    |  REGION                                    * C
C *   RCS(9)    Ymax    |                                            * C
C *   RCS(10)   Function height at the bottom left-hand corner of    * C
C *             contouring region -- needed by area/centre of mass   * C
C *   RCS(11)-RCS(20)    Contour heights.                            * C
C *   RCS(21)   a  |  used to hold constants |  v'=a+b*v             * C
C *   RCS(22)   b  |  in transformations     |  v'=log(a+b*v)        * C
C *   RCS(31)-RCS(80)    X-coordinates of vertices of plotting       * C
C *                      polygon                                     * C
C *   RCS(51)-RCS(130)   Y-coordinates of vertices of plotting       * C
C *                      polygon                                     * C
C *                         -- see ICS(8) & ICS(9) above             * C
C *                                                                  * C
C ******************************************************************** C
C *                                                                  * C
C *   IGG ARRAY    - size 30 - grid-generation : integer parameters  * C
C *   ---------                                                      * C
C *   IGG(1)    Gridding switch : 0 = no gridder is required,        * C
C *                               1 = gridder is required.           * C
C *   IGG(2)    Gridder-type : 1 = Lowestoft ordinary gridder        * C
C *   IGG(2)    Gridder-type : 1 = Lowestoft hyper    gridder        * C
C *                            2 = Lowestoft super    gridder        * C
C *                            3 = Lowestoft ordinary gridder        * C
C *                            4 = Franke - thin plate splines       * C
C *                            5 = Modified quadratic Shepard        * C
C *                            6 = Nielson-Frank quadratic Shepard   * C
C *                            7 = Duchon thin plate                 * C
C *                            8 = Duchon radial cubic               * C
C *                            9 = Hardy multiquadric                * C
C *                           10 = Hardy reciprocal multiquadric     * C
C *                           11 = Akima                             * C
C *                           12 = Schagen                           * C
C *                           13 = Foley                             * C
C *   IGG(3)    Weight function in Lowestoft gridders (range 1 to 6) * C
C *   IGG(4)    Max number of iterations in hyper gridder.           * C
C *   IGG(5)    Grid size : number of points 'across' -- n           * C
C *   IGG(6)    Grid size : number of points  'down'  -- m           * C
C *   IGG(7)    Gridding transformation switch :                     * C
C *                          0 = no transformation                   * C
C *                          1 = change of origin                    * C
C *                          2 = standard linear transf              * C
C *                          3 = user-defined linear transf          * C
C *                          4 = standard log transf                 * C
C *                          5 = user-defined log transf             * C
C *   IGG(8)    Paper-tape ?                                         * C
C *   IGG(9)    Starting-value-switch for hypergridder :             * C
C *                                0 = 'zeros'                       * C
C *                                1 = 'ordinay gridder'             * C
C *                                2 = 'data from file'              * C
C *   IGG(10)   Return from gridding flag : 0 = successful,          * C
C *                                         > 0 = otherwise.         * C
C *   IGG(11)   Average no of pts per panel in Franke's Grid Method  * C
C *               >0  --  signifies panels selected automatically    * C
C *                0  --  signifies user selected panels             * C
C *   IGG(12)   No of X knots in Franke's method                     * C
C *   IGG(13)   No of Y knots in Franke's method                     * C
C *   IGG(14)   Radius selection option in Mod Quad Shepard Method   * C
C *                1  --  specify no of pts                          * C
C *                2  --  specify radius                             * C
C *   IGG(15)   No of pts for nodal  functions in MQS/NFMQS          * C
C *   IGG(16)   No of pts for weight functions in MQS                * C
C *   IGG(17)   non-default param in Duchon methods                  * C
C *   IGG(18)   NPPR. parameter in RHMTH1                            * C
C *   IGG(19)                                                        * C
C *             IGG(20)-IGG(25) : Foley's Method                     * C
C *   IGG(20)   Surface fitting mode : 0/1 interpolattion/approx     * C
C *   IGG(21)   Degree of fit on boundary                            * C
C *   IGG(22)   Degree of fit on interior                            * C
C *   IGG(23)   Log10 transform : 0/1 no/yes                         * C
C *   IGG(24)   Choice of grid :  1/2 uniform/user-defined           * C
C *   IGG(25)   Partial derivative defn : 0/1 least sq/ 3 pt diff    * C
C *   IGG(26)   Unused                                               * C
C *   IGG(27)   Unused                                               * C
C *   IGG(28)   Unused                                               * C
C *   IGG(29)   Unused                                               * C
C *   IGG(30)   Unused                                               * C
C *                                                                  * C
C ******************************************************************** C
C *                                                                  * C
C *   RGG ARRAY    - size 50 - grid-generation : real parameters     * C
C *   ---------                                                      * C
C *   RGG(1)    Sample spacing : x direction.                        * C
C *   RGG(2)    Sample spacing : y direction.                        * C
C *   RGG(3)    Relaxation parameter.                                * C
C *   RGG(4)    Xmin     |                                           * C
C *   RGG(5)    Xmax     |   GRIDDED                                 * C
C *   RGG(6)    Ymin     |   REGION                                  * C
C *   RGG(7)    Ymax     |                                           * C
C *   RGG(8)    a  |  used to hold constants |  v'=a+b*v             * C
C *   RGG(9)    b  |  in transformations     |  v'=log(a+b*v)        * C
C *   RGG(10)   Accuracy criterion in iteration in Hyper Gridder     * C
C *   RGG(11)-RGG(30) Position of X knots in Franke & Foley methods  * C
C *   RGG(31)-RGG(50) Position of Y knots in Franke & Foley methods  * C
C *   RGG(11)   Grand Mean             in Schagen's Method           * C
C *   RGG(12)   Correlation distance   in Schagen's Method           * C
C *   RGG(38)   radius for nodal  functions  in MQS/NFMQS            * C
C *   RGG(39)   radius for weight functions  in MQS                  * C
C *                                                                  * C
C ******************************************************************** C
C *                                                                  * C
C *   IKN ARRAY    - size 10 - splines : integer parameters          * C
C *   ---------                                                      * C
C *   IKN(1)    Spline switch :                                      * C
C *                  0 = splines are not required                    * C
C *                  1 = program-defined number and position of      * C
C *                      knots                                       * C
C *                  2 = user-defined number of knots,               * C
C *                      program-defined position of knots           * C
C *                  3 = user-defined number and position of knots   * C
C *   IKN(2)    Total number of knots, X direction                   * C
C *   IKN(3)    Total number of knots, Y direction                   * C
C *   IKN(4)    Spline weight switch :                               * C
C *                  0 = weights for splines are all to be 10.0      * C
C *                  1 = weights for splines are to be taken from    * C
C *                        scattered source data-file                * C
C *   IKN(5)    Spline report switch                                 * C
C *                  0 = splines fit with source data                * C
C *                  1 = splines do not fit with source data         * C
C *   IKN(6)    Unused                                               * C
C *   IKN(7)    Unused                                               * C
C *   IKN(8)    Unused                                               * C
C *   IKN(9)    Unused                                               * C
C *   IKN(10)   Unused                                               * C
C *                                                                  * C
C ******************************************************************** C
C *                                                                  * C
C *   RKN ARRAY    - size 350 - splines : real parameters            * C
C *   ---------                                                      * C
C *   RKN(1)-RKN(20)   Position of X knots                           * C
C *   RKN(21)-RKN(40)  Position of Y knots                           * C
C *                    (Note that the first four and the last four   * C
C *                    knots are at the 'edges')                     * C
C *   RKN(41)          Value of EPS                                  * C
C *   RKN(42)-RKN(50)  Unused                                        * C
C *   RKN(51)-RKN(350) Spline coeffs (array C in NAG convention)     * C
C *                                                                  * C
C ******************************************************************** C
C *                                                                  * C
C *   IPS ARRAY    - size 40 - plotting : integer parameters         * C
C *   ---------                                                      * C
C *   IPS(1)    Plotting device : 1 = line-printer ( using SSSU33 )  * C
C *                               2 = any other device               * C
C *   IPS(2)    Mercator projection-switch : 0 = no projection,      * C
C *                                          1 = Mercator.           * C
C *   IPS(3)    Coastine-switch : 0 = off,                           * C
C *                               1 = on.                            * C
C *   IPS(4)    Coastline plotting colour : 1 = black,               * C
C *                                         2 = green,               * C
C *                                         3 = red.                 * C
C *   IPS(5)    Contour plotting/Isometric projection colour:        * C
C *                                   1 = black,                     * C
C *                                   2 = green,                     * C
C *                                   3 = red.                       * C
C *   IPS(6)    Contour labelling-switch  : 0 = off,                 * C
C *                                         1 = on.                  * C
C *   IPS(7)    Sample position-switch : 0 = not included,           * C
C *                                      1 = position only,          * C
C *                                      2 = position and value.     * C
C *   IPS(8)    Sample position symbol : see Calcomp set.            * C
C *   IPS(9)    Sample position colour : 1 = black,                  * C
C *                                      2 = green,                  * C
C *                                      3 = red.                    * C
C *   IPS(10)   Data Title-switch : 0 = not included,                * C
C *                                 1 = included.                    * C
C *   IPS(11)   Annotation-lat-long : 1 = no annotation,             * C
C *                                   2 = X-Y,                       * C
C *                                   3 = decimal degrees            * C
C *                                   4 = deg-dec mins,              * C
C *                                   5 = deg-min-secs.              * C
C *   IPS(12)   Tick mark switch : 0 = off,                          * C
C *                                1 = on.                           * C
C *   IPS(13)   Isometric projection rotation parameter : 0 = NE,    * C
C *                                                       1 = NW,    * C
C *                                                       2 = SW,    * C
C *                                                       3 = SE.    * C
C *   IPS(14)   Plotting polygon switch:                             * C
C *                 0 = plot contours within normal rectangular      * C
C *                                 plotting region                  * C
C *                 1 = plot contours within specified polygon only  * C
C *                 2 = as 1, but plot polygon boundary also.        * C
C *   IPS(15)   Unused                                               * C
C *   IPS(16)   Additional data points superposition switch          * C
C *                             (contour plot only)                  * C
C *                 0 = no additional data points                    * C
C *                 1 = positions only of additional data points     * C
C *                 2 = position and value of additional data points * C
C *   IPS(17)   Additional data points symbol : see   Calcomp set    * C
C *   IPS(18)   Additional data points colour : 1 = black,           * C
C *                                             2 = green,           * C
C *                                             3 = red.             * C
C *   IPS(19)   Number of tick marks for the x-axis                  * C
C *                            (0 = chosen by program)               * C
C *   IPS(20)   Number of tick marks for the y-axis                  * C
C *                            (0 = chosen by program)               * C
C *   IPS(21)   Flag = 0/1 for long/tall default plotting box        * C
C *                    -1 for non-default plotting box.              * C
C *   IPS(22)   Flag = 0/1 for scale factor in use in REPLT          * C
C *   IPS(23)   Flag set 0 if DEFAULT PLOT OPTIONS;   1 otherwise    * C
C *   IPS(24)   Flag set 0 if DEFAULT TICK MARKS etc; 1 otherwise    * C
C *   IPS(25)   Flag set 0 if DEFAULT PLOT SIZE;      1 otherwise    * C
C *   IPS(26)   Flag set 0 if a title is NOT to be drawn on the      * C
C *             vertical axis of an isometric plot OR a              * C
C *             non-geographic contour plot          1 otherwise     * C
C *   IPS(27)   Flag set to 0 if a title is NOT to be drawn on the   * C
C *             horizontal axis of a non-geographic contour plot     * C
C *                                                  1 otherwise     * C
C *   IPS(28)   Flag set to 0 if a title is NOT to be drawn on the   * C
C *             x-axis of an isometric plot          1 otherwise     * C
C *   IPS(29)   Flag set to 0 if a title is NOT to be drawn on the   * C
C *             y-axis of an isometric plot          1 otherwise     * C
C *   IPS(30)   GKS terminal type ( stored for multiple previews)    * C
C *   IPS(31)   Flag set to 0/1 for software/hardware characters     * C
C *             on Tektronix                                         * C
C *   IPS(32)   Number of GKS terminal devices supported             * C
C *   IPS(33)   Number of GKS plotters supported                     * C
C *   IPS(34)   Unused                                               * C
C *   IPS(35)   Unused                                               * C
C *   IPS(36)   Unused                                               * C
C *   IPS(37)   Unused                                               * C
C *   IPS(38)   Unused                                               * C
C *   IPS(39)   Unused                                               * C
C *   IPS(40)   Unused                                               * C
C *                                                                  * C
C ******************************************************************** C
C *                                                                  * C
C *   RPS ARRAY    - size 30 - plotting : real parameters            * C
C *   ---------                                                      * C
C *   RPS(1)    X-axis length (mm)                                   * C
C *   RPS(2)    Y-axis length (mm)                                   * C
C *   RPS(3)    Ratio of main plotting area length/height, set       * C
C *             in SSSU10                                            * C
C *   RPS(4)    Unused                                               * C
C *   RPS(5)    Unused                                               * C
C *   RPS(6)    UMIN |  MAIN                                         * C
C *   RPS(7)    UMAX |  PLOTTING                                     * C
C *   RPS(8)    VMIN |  VIEWPORT                                     * C
C *   RPS(9)    VMAX |  for contour plot or isometric plot           * C
C *   RPS(10)   Height factor |                                      * C
C *   RPS(11)   ZMIN          | for isometric projection routine     * C
C *   RPS(12)   ZMAX          |                                      * C
C *   RPS(13)   SCALE factor -- see IPS(22)                          * C
C *   RPS(14)   Used to SAVE real WIDTH in subr CHSIZ in SSSU31      * C
C *   RPS(15)   Used to SAVE real HIGHT in subr CHSIZ in SSSU31      * C
C *   RPS(16)   Used to SAVE real SPACE in subr CHSIZ in SSSU31      * C
C *   RPS(17)   Unused                                               * C
C *   RPS(18)   Unused                                               * C
C *   RPS(19)   Unused                                               * C
C *   RPS(20)   Unused                                               * C
C *   RPS(21)   Unused                                               * C
C *   RPS(22)   Unused                                               * C
C *   RPS(23)   Unused                                               * C
C *   RPS(24)   Unused                                               * C
C *   RPS(25)   Unused                                               * C
C *   RPS(26)   Unused                                               * C
C *   RPS(27)   UMIN |  OVERALL                                      * C
C *   RPS(28)   UMAX |  PLOTTING                                     * C
C *   RPS(29)   VMIN |  VIEWPORT                                     * C
C *   RPS(30)   VMAX |  initialised in SSSU30                        * C
C *                                                                  * C
C ******************************************************************** C
C
      IMPLICIT LOGICAL (A-Z)
C
C     COMMON BLOCKS
C     *************
C     CHARS   CNTRL   PF      CSU20E  SPCMN   GRIDD   LPHARE  INTER
C     ARR     RPCMN
C
C     *****************************************
C     *** HP restrictions for common blocks ***
C     *****************************************
C
C     See RTE-6/VM LOADER REFERENCE MANUAL pp 6-7 to 6-10
C        -- notes below as interpreted from manual and
C           current practice, ( DE : 31/01/85 )
C
C     Common blocks are classified as 'EMA', 'SAVE' or 'ordinary'.
C
C     Each ordinary common block used in the main program must be
C     declared in the main program by means of a NO ALLOCATE statement.
C     All ordinary common blocks should appear in a single BLOCK DATA
C     subprogram with each block again declared with a NO ALLOCATE
C     statement.
C
C     All EMA and SAVE common blocks must be appear in the main program
C     WITHOUT a NO ALLOCATE statement and must NOT appear in the BLOCK
C     DATA subprogram.
C
C     Each subprogram using an EMA common blocks must have an
C     appropriate $EMA statement.
C
C
C-----INCLUDE STATEMENT FOR CNTRL COMMON BLOCKS ------------------------
C      INCLUDE 'H:\CONTOUR\MASTER\COMMON'
CCCC  CNTRL COMMON BLOCKS
C
      CHARACTER*80 TITLE
      CHARACTER*30 XTITLE,YTITLE,ZTITLE
      CHARACTER*5  XUNITS,YUNITS,ZUNITS
      CHARACTER*30 NAMES(20)
      COMMON /CHARS/ TITLE,XTITLE,YTITLE,ZTITLE,
     *               XUNITS,YUNITS,ZUNITS,NAMES
C
      LOGICAL SALF,LOW,PC386,PRIME,HP9000,VAX
      LOGICAL LGSV(30)
      INTEGER LU,LL,LR,QNO,IN(40)
      INTEGER IDD(10),ICS(10), IGG(30),IKN(10) ,IPF(10),IPS(40)
      REAL*8  RDD(10),RCS(130),RGG(50),RKN(350),RPF(20),RPS(30)
      COMMON /CNTRL/ SALF,LOW,PC386,PRIME,HP9000,VAX,
     *               LL,LU,LR, QNO,LGSV,IN,IDD,ICS,IGG,IKN,IPF,IPS,
     *                           RDD,RCS,RGG,RKN,RPF,RPS
C--
C--
C--
C--
C--
C--
C--
C--
C--
C--
C--
C--
C--
C--
C-----------------------------------------------------------------------
C
CCCC  PF COMMON BLOCK
C
      CHARACTER*32 PFLIST(10)
      COMMON /PF/ PFLIST
C
CCCC  CSU20E COMMON BLOCK
C
      REAL*8 X(2001),Y(2001),V(2001),F(1601)
      COMMON /CSU20E/ X,Y,V,F
C
CCCC  SPCMN COMMON BLOCK
C
      INTEGER XNOT,YNOT,NC,NPTS,NADRES,NPOINT
      PARAMETER (XNOT=17,YNOT=17,NC=(XNOT-4)*(YNOT-4),NPTS=2002,
     *   NADRES=(XNOT-7)*(YNOT-7),NPOINT=NPTS+NADRES)
      INTEGER M1,PX,PY,QX,QY,NAD,MC,RANK,POINT(NPOINT)
      REAL*8 EPS,SIGMA,X1(NPTS),Y1(NPTS),F1(NPTS),W1(NPTS)
      COMMON /SPCMN/ M1,PX,PY,QX,QY,NAD,MC,RANK,POINT,EPS,SIGMA,X1,Y1,
     *   F1,W1
C
CCCC  GRIDD COMMON BLOCK
C
      INTEGER MDIM,MA,MB,NA,NB
      PARAMETER (MDIM=40)
      REAL*8 SURFCE(MDIM,MDIM)
      COMMON /GRIDD/ MA,MB,NA,NB,SURFCE
C
CCCC  LPHARE COMMON BLOCK
C
      REAL*8 F2(40,40),FIL(204),H(10)
      COMMON /LPHARE/F2,FIL,H
C
CCCC  INTER COMMON BLOCK
C
      INTEGER IR
      REAL*8 RECON(8,1000)
      COMMON /INTER/ RECON,IR
C
CCCC  ARR COMMON BLOCK
C
      INTEGER ICFT,IBTOT
      REAL*8 CFT(6,70),BTOTL(8,60)
      COMMON /ARR/CFT,ICFT,BTOTL,IBTOT
C
CCCC  RPCMN COMMON BLOCK
C
      INTEGER RPUNIT,RPBTCH
      COMMON /RPCMN/ RPUNIT,RPBTCH
C
C     PROGRAM HSU01  CALLS THE FOLLOWING SUBROUTINES
C     **********************************************
C     ABORT   APPCN   CHKFN   DATTIM  HOST    INITCM  INQIRE  LABEL@
C     RSERR   SSU10   SSU12   SSU13   SSU20   SSU27   SSU30   SSU35
C     SSU40   TIDY    UPDATE  WRCMN   ZREAD
C
C     PROGRAM HSU01  REFERENCES THE FOLLOWING FUNCTIONS
C     *************************************************
      INTEGER IUNIT
C
C     LOCAL VARIABLES
C     ***************
      CHARACTER*30 NAME
      CHARACTER ANS,LINE*80,IFORM*7,MARK*25
      CHARACTER*8 DATE,TIME
      LOGICAL LOG1,LOG2,OPEN,FIRST,EXISTS
      INTEGER FLAG,IBATCH
C
C     EQUIVALENCED VARIABLES
C     **********************
      LOGICAL BEOF,BATCH
      EQUIVALENCE (BEOF,LGSV(9)),(BATCH,LGSV(10))
      COMPLEX*16 LABEL                                                  C-PC386
C     REAL*8 LABEL                                                      C-PRIME
C     INTEGER LABEL                                                     C-HP9000
C     INTEGER LABEL                                                     C-VAX
      EQUIVALENCE (LABEL,IN(27))
C
C     DATA STATEMENTS
C     ***************
      DATA IFORM/'(BN,I6)'/
C
C-----------------------------------------------------------------------
C
C     Mark Revision History : Significant Changes
C.......................................................................
C
C     Mark 4.3  -- PLOT call added to DVICE because of HP bug
C                  RETURN added to NGINT to test whether necessary
C.......................................................................
C
C     Mark 5.1  -- version in sub-dir NEW -- never released at Lowestoft
C                  Major changes in the manner of handling
C                        C-LOW/C-SALF  ==>  C-HP/C-PRIME/C-CCINT/C-GIINT
C                  in order to accomodate version for Lowestoft Prime.
C                  Important changes to subprogram HOST
C     Mark 5.2  -- version in sub-dir MASTER
C                  Further major changes in graphics/plotting.
C                  Module SSSU30 now only produces PF/MF files, no plots
C                  New modules SSSU41 and SSSU42.
C                  Module SSSU41 drives the plotting.
C                  Module SSSU42 is GLIB software to drive Tektronix
C     Mark 5.3  -- Tidying up of Mark 5.2
C                  Plotting software for HP and T4015 terminals
C     Mark 5.4  -- Minor enhancements to output of contour and
C                  isometric plots
C                  Plotting software for various terminals
C.......................................................................
C
C     Mark 6.1  -- New interface to GKS
C                  Significant re-writing of SSSU41.f77
C                  Minor edits elsewhere
C                  New SSSU43.f77 (interface to GKS single precision
C                  and long integer.
C                  Preparation for VAX version
C     Mark 6.2  -- Tidying up ready for conversion to VAX form
C     Mark 6.3  -- Tidying up after conversion to VAX/NAG2 form but
C                  still uses NAG1 at Salford.
C                  Includes 'update' and 'comment out' enhancements.
C.......................................................................
C
C     Mark 7.1  -- PC386 version
C                  Renaming files in a logical manner
C
C-----------------------------------------------------------------------
C
CCC   initialise control common
C
      FIRST=.TRUE.
      CALL INITCM(FIRST)
C
CCC   set up non local label to enable return from ABORT
C
C      CALL LABEL@(LABEL,*8888)                                          C-PC386
C     CALL MKLB$F($8888,LABEL)                                          C-PRIME
C     ASSIGN 8888 TO LABEL                                              C-HP9000
C     ASSIGN 8888 TO LABEL                                              C-VAX
C
CCC   Specify host and location
C
      SALF=.TRUE.
      PC386=.TRUE.                                                      C-PC386
C     PRIME=.TRUE.                                                      C-PRIME
C     HP9000=.TRUE.                                                     C-HP9000
C     VAX=.TRUE.                                                        C-VAX
C
CCCC  Set flag to indicate which driver is being used
C
      IN(13)=0
C
CCC   DETERMINE HOST-DEPENDENT PARAMETERS
C
      CALL HOST(MARK)
C
      WRITE(LL,2001) MARK
 2001 FORMAT(2X,'********************************************',
     *     / 2X,'*                                          *',
     *     / 2X,'*          UNIVERSITY OF SALFORD           *',
     *     / 2X,'*        DEPARTMENT OF MATHEMATICS         *',
     *     / 2X,'*                   AND                    *',
     *     / 2X,'*            COMPUTER  SCIENCE             *',
     *     / 2X,'*                                          *',
     *     / 2X,'*           CONTOURING  PACKAGE            *',
     *     / 2X,'*                  HSU01                   *',
     *     / 2X,'*                                          *',
     *     / 2X,'*        ',A,                    '         *',
     *     / 2X,'*                                          *',
     *     / 2X,'********************************************')
C
C     SET UP A REPORT FILE
C
      NAME='REPORT'
      CALL APPCN(NAME)
C
CCC   CHECK THAT REPORT FILE IS NOT ALREADY OPEN
C
      CALL INQIRE(NAME,OPEN)
      IF (OPEN) THEN
	  WRITE(LL,'(//2X,''REPORT FILE IS ALREADY OPEN '')')
	  CALL ABORT
      ENDIF
C
      LR=IUNIT()
      OPEN(LR,FILE=NAME,STATUS='UNKNOWN')
C     OPEN(LR,FILE=NAME,STATUS='UNKNOWN')                               C-PRIME
C     OPEN(LR,FILE=NAME,STATUS='UNKNOWN')                               C-HP9000
C     OPEN(LR,FILE=NAME,STATUS='NEW',CARRIAGECONTROL='LIST')            C-VAX
C
CCC   list recent updates to package on terminal
C
      CALL UPDATE
C
CCC   next statement is in simply to suppress compiler message about
CCC   'unused' parameter NC
      IN(10)=NC
      IN(10)=1
C
CCC   TIME and DATE stamp the REPORT FILE
C
      CALL DATTIM(DATE,TIME)
      WRITE(LR,2002) DATE,TIME
 2002 FORMAT(2X,'********************************************',
     *     / 2X,'*                                          *',
     *     / 2X,'*    Date ',A   ,'        Time ',A   ,'    *',
     *     / 2X,'*                                          *',
     *     / 2X,'********************************************'
     *     ///)
C
      WRITE(LR,2001) MARK
C
C
  102 CONTINUE
      IF( QNO.EQ.102 ) CALL RSERR(FLAG)
      QNO=102
      WRITE(LL,2102)
 2102 FORMAT(//2X,'*** Q102 ***'/2X,'DO YOU WISH TO WORK IN ',
     *  //7X,'1  SINGLE JOB MODE ? ',
     *   /7X,'2  BATCH MODE ? ',
     *  //7X,'TYPE 1 OR 2')
      CALL ZREAD(.TRUE.,LINE,FLAG,*102)
      READ(LINE,IFORM,ERR=102) IBATCH
      IF( IBATCH.LT.1 .OR. IBATCH.GT.2 ) GOTO 102
C
      IF( IBATCH.EQ.1 ) THEN
	  BATCH=.FALSE.
      ELSE
	  BATCH=.TRUE.
      ENDIF
C
C
      IF( .NOT. BATCH ) THEN
C         SINGLE JOB MODE
C
  103     CONTINUE
	  IF( QNO.EQ.103 ) CALL RSERR(FLAG)
	  QNO=103
	  WRITE(LL,2103)
 2103     FORMAT(//2X,'*** Q103 ***'/2X,'DO YOU WISH TO ',
     *      //7X,'1  CREATE A NEW CONTROL FILE ? ',
     *       /7X,'2  READ A CONTROL FILE FROM DISC ? ',
     *      //7X,'TYPE 1 OR 2')
	  CALL ZREAD(.TRUE.,LINE,FLAG,*103)
	  READ(LINE,IFORM,ERR=103) IN(1)
	  IF( IN(1).LT.1 .OR. IN(1).GT.2 ) GOTO 103
C
C
      ELSE
C         BATCH MODE -- CONTROL FILES MUST BE ON DISC
	  IN(1)=2
	  BEOF=.FALSE.
C
C
  106     CONTINUE
	  IF( QNO.EQ.106 ) CALL RSERR(FLAG)
	  QNO=106
	  WRITE(LL,2106)
 2106     FORMAT(//2X,'*** Q106 ***'/
     *    2X,'TYPE IN THE NAME OF THE BATCH FILE')
	  CALL ZREAD(.TRUE.,LINE,FLAG,*106)
	  READ(LINE,'(A)',ERR=106) NAMES(11)
	  CALL CHKFN(NAMES(11),*106)
	  INQUIRE(FILE=NAMES(11),EXIST=EXISTS,ERR=106)
	  IF( .NOT.EXISTS ) THEN
	      WRITE(LL,2901) NAMES(11)
 2901         FORMAT(//'+++ BATCH FILE ',A,' DOES NOT EXIST +++')
	      GOTO 106
	  ENDIF
C
CCCC      OPEN BATCH PF FILE
C
	  RPBTCH=IUNIT()
	  OPEN (RPBTCH,FILE='PFBTC',STATUS='UNKNOWN')
C         OPEN (RPBTCH,FILE='PFBTC',STATUS='UNKNOWN')                   C-PRIME
C         OPEN (RPBTCH,FILE='PFBTC',STATUS='UNKNOWN')                   C-HP9000
C         OPEN (RPBTCH,FILE='PFBTC',STATUS='NEW')                       C-VAX
C
	  WRITE(LR,2200) NAMES(11)
 2200     FORMAT(//////
     *        / 2X,'********************************************',
     *        / 2X,'*                                          *',
     *        / 2X,'*            BATCH MODE JOB                *',
     *        / 2X,'*            **************                *',
     *        / 2X,'*                                          *',
     *        / 2X,'*   BATCH FILE IS                          *',
     *        / 2X,'*   ',A,                        '          *',
     *        / 2X,'*                                          *',
     *        / 2X,'*                                          *',
     *        / 2X,'********************************************')
C
C
      ENDIF
C
C
    2 CONTINUE
C
CCC   LOOP BACK POINT FOR BATCH MODE
C
      IN(3)=0
C
CCC   CALL INTERACTIVE CONTROL COMMON CREATION OR INPUT MODULE
C
      IDD(1)=0
      CALL SSU10
C
CCC   Check that batch file is not exhausted
CCC    -- only relevant for batch jobs
C
      IF( BATCH .AND. BEOF ) THEN
C        CLOSE BATCH PF FILE
	 CLOSE (RPBTCH)
	 GOTO 999
      ENDIF
C
CCC   CALL GRIDDING OR SPLINE MODULES AS NECESSARY
C
      IF( IGG(1).GT.0 .AND. IN(1).NE.0 ) THEN
C         GRIDDING REQUIRED
C
	  CALL SSU20
C
	  IF( IGG(10).NE.0 ) THEN
C             CHECK IF GRIDDING FAILED
	      WRITE(LL,'(//,2A//A)') '+++ GRIDDING IS SUSPECT -- SEE',
     *                ' REPORT FILE FOR MORE DETAILS +++',
     *                '+++    RUN CONTINUING    +++'
	  ENDIF
C
      ELSEIF( IKN(1).GT.0 .AND. IN(1).NE.0 ) THEN
C         SPLINES REQUIRED
	  CALL SSU27
	  IF( IKN(5).NE.0 ) THEN
C             CHECK IF SPLINES FAILED
	      WRITE(LL,'(//,2A//A)') '+++ SPLINES ARE SUSPECT -- SEE',
     *                ' REPORT FILE FOR MORE DETAILS +++',
     *                '+++    RUN CONTINUING    +++'
	  ENDIF
      ENDIF
C
C     INITIALISE LOGICALS BEFORE REDRAW LOOP
C
      LOG1= .FALSE.
      LOG2= IN(1).EQ.1
C
C
    1 CONTINUE
C
C     LOOP FOR REDRAW FACILITY
C     RESET QUESTION NUMBER
      QNO=0
C
C     WRITE THE CONTROL COMMON BLOCKS TO 'CMNFIL'
C
      CALL WRCMN
C
C     CALL CONTOURING MODULE
C
      CALL SSU30
C
C     IF NOT LINE PRINTER CONTOURER OR BATCH, CALL PREVIEWING/PLOTTING MODULE
C
      IF( ICS(1).GT.2 .AND. .NOT. BATCH ) CALL SSU40
C
C     IF REQUESTED, CALL AREA AND CENTRE OF MASS MODULE
C
      IF( IN(12).EQ.1 ) THEN
C         RESET FLAG SO ONLY CALLED ONCE
	  IN(12)=2
	  CALL SSU35
      ENDIF
C
C     CHECK WHETHER CONTROL FILE NEEDS TO BE SAVED:
C     INITIALLY, LOG1 IS SET TRUE IF CHANGES HAVE BEEN MADE IN REDRAW
C         SINCE LAST SAVE OF CONTROL FILE; LOG2 IS SET TRUE IF CNTRL
C         COMMON IS TO BE CREATED INTERACTIVELY.
C      SUBSEQUENTLY LOG1 AND LOG2 ARE BOTH SET FALSE IF A NEW CONTROL
C         FILE IS CREATED
C
      IF( (LOG1 .OR. LOG2 ) .AND. (.NOT. BATCH) ) THEN
C          MAY WISH TO CREATE NEW CONTROL FILE
C
  105     CONTINUE
	  IF( QNO.EQ.105 ) CALL RSERR(FLAG)
	  QNO=105
	  WRITE(LL,2105)
 2105     FORMAT(//2X,'*** Q105 ***'/2X,
     *        'DO YOU WISH TO CREATE A CONTROL FILE ?')
	  CALL ZREAD(.TRUE.,LINE,FLAG,*105)
	  READ(LINE,'(A)') ANS
	  IF( ANS.EQ.' ' ) ANS='Y'
	  IF( ANS.EQ.'Y' ) THEN
C
	      LOG1=.FALSE.
	      LOG2=.FALSE.
C
C             CALL CONTROL FILE CREATION MODULE
	      CALL SSU12
C
	  ELSEIF( ANS.EQ.'N' ) THEN
	      CONTINUE
	  ELSE
C             ERROR IN RESPONSE TO Q105
	      GOTO 105
	  ENDIF
C
      ENDIF
C
C
C     CHECK ON WHETHER TO ENTER REDRAW MODULE
C
      IF( IPS(1).GT.1 .AND. (.NOT. BATCH) ) THEN
C         NOT LINEPRINTER PLOTS, CALL REDRAW MODULE AND RESET
C         LOG1 TO INDICATE WHETHER CONTROL COMMON CHANGED
	  CALL SSU13
	  LOG1= LOG1 .OR. IN(14).NE.0
C         LOOP BACK TO CONTOURING CALL IF REDRAWING REQUESTED
	  IF( IN(3).EQ.1 ) GOTO 1
      ENDIF
C
CCC   If BATCH, re-initialise cntrl common and loop back.
CCC   Label 8888 has been used in a system-dependent, non-
CCC   local GOTO from subroutine ABORT.
C
 8888 CONTINUE
C
      IF( BATCH ) THEN
	  FIRST=.FALSE.
	  CALL INITCM(FIRST)
	  IN(1)=2
	  GOTO 2
      ENDIF
C
C
C
  999 CONTINUE
C
      WRITE(LL,2003)
      WRITE(LR,2003)
 2003 FORMAT(//2X,'*********  PROGRAM HSU01 FINISHED  *********'////)
C
      CLOSE(LR)
C
C     DELETE ALL UNWANTED FILES
      CALL TIDY
C
C     TERMINATE EXECUTION
      STOP '**** RUN COMPLETE ****'
C
C     **
C     **    END OF PROGRAM HSU01
C     **
C
      END
C+*+* HSU01  END    +*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*+*
 
