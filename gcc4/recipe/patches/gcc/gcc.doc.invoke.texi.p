--- gcc/doc/invoke.texi.orig	2006-09-25 23:21:58.000000000 +0200
+++ gcc/doc/invoke.texi	2010-07-04 19:34:09.616906960 +0200
@@ -395,6 +395,7 @@ Objective-C and Objective-C++ Dialects}.
 @gccoptlist{-mapcs-frame  -mno-apcs-frame @gol
 -mabi=@var{name} @gol
 -mapcs-stack-check  -mno-apcs-stack-check @gol
+-mmodule  -mno-module @gol
 -mapcs-float  -mno-apcs-float @gol
 -mapcs-reentrant  -mno-apcs-reentrant @gol
 -msched-prolog  -mno-sched-prolog @gol
@@ -7205,7 +7206,8 @@ architectures:
 @item -mabi=@var{name}
 @opindex mabi
 Generate code for the specified ABI@.  Permissible values are: @samp{apcs-gnu},
-@samp{atpcs}, @samp{aapcs}, @samp{aapcs-linux} and @samp{iwmmxt}.
+@samp{apcs-32}, @samp{atpcs}, @samp{aapcs}, @samp{aapcs-linux} and
+@samp{iwmmxt}.
 
 @item -mapcs-frame
 @opindex mapcs-frame
@@ -7219,8 +7221,6 @@ leaf functions.  The default is @option{
 @opindex mapcs
 This is a synonym for @option{-mapcs-frame}.
 
-@ignore
-@c not currently implemented
 @item -mapcs-stack-check
 @opindex mapcs-stack-check
 Generate code to check the amount of stack space available upon entry to
@@ -7229,9 +7229,16 @@ insufficient space available then either
 @samp{__rt_stkovf_split_small} or @samp{__rt_stkovf_split_big} will be
 called, depending upon the amount of stack space required.  The run time
 system is required to provide these functions.  The default is
-@option{-mno-apcs-stack-check}, since this produces smaller code.
+@option{-mno-apcs-stack-check}, since this produces smaller code.  However,
+the APCS-32@ ABI has @option{-mapcs-stack-check} as default except when
+module code is generated with @option{-mmodule}.
+
+@item -mmodule
+@opindex mmodule
+Generate code suited for building RISC OS modules.  All writable data
+are accessed via an offset determined by the value at R10-536.  This is
+only supported for APCS-32@ ABI.  The default is @option{-mno-module}.
 
-@c not currently implemented
 @item -mapcs-float
 @opindex mapcs-float
 Pass floating point arguments using the float point registers.  This is
@@ -7241,6 +7248,7 @@ arithmetic is going to be performed by t
 @option{-mno-apcs-float}, since integer only code is slightly increased in
 size if @option{-mapcs-float} is used.
 
+@ignore
 @c not currently implemented
 @item -mapcs-reentrant
 @opindex mapcs-reentrant
