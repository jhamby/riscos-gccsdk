/* Generated automaticaly by the program `gengenrtl'
   from the RTL description file `rtl.def' */

#include "config.h"
#include "system.h"
#include "obstack.h"
#include "rtl.h"

extern struct obstack *rtl_obstack;

static rtx obstack_alloc_rtx PROTO((int length));
static rtx obstack_alloc_rtx (length)
     register int length;
{
  rtx rt = (rtx) obstack_alloc (rtl_obstack, length);

  memset(rt, 0, sizeof(struct rtx_def) - sizeof(rtunion));

  return rt;
}

rtx
gen_rtx_fmt_ee (code, mode, arg0, arg1)
     RTX_CODE code;
     enum machine_mode mode;
     rtx arg0;
     rtx arg1;
{
  rtx rt = obstack_alloc_rtx (sizeof (struct rtx_def) + 1 * sizeof (rtunion));
  PUT_CODE (rt, code);
  PUT_MODE (rt, mode);
  XEXP (rt, 0) = arg0;
  XEXP (rt, 1) = arg1;

  return rt;
}

rtx
gen_rtx_fmt_ue (code, mode, arg0, arg1)
     RTX_CODE code;
     enum machine_mode mode;
     rtx arg0;
     rtx arg1;
{
  rtx rt = obstack_alloc_rtx (sizeof (struct rtx_def) + 1 * sizeof (rtunion));
  PUT_CODE (rt, code);
  PUT_MODE (rt, mode);
  XEXP (rt, 0) = arg0;
  XEXP (rt, 1) = arg1;

  return rt;
}

rtx
gen_rtx_fmt_iss (code, mode, arg0, arg1, arg2)
     RTX_CODE code;
     enum machine_mode mode;
     int arg0;
     char * arg1;
     char * arg2;
{
  rtx rt = obstack_alloc_rtx (sizeof (struct rtx_def) + 2 * sizeof (rtunion));
  PUT_CODE (rt, code);
  PUT_MODE (rt, mode);
  XINT (rt, 0) = arg0;
  XSTR (rt, 1) = arg1;
  XSTR (rt, 2) = arg2;

  return rt;
}

rtx
gen_rtx_fmt_is (code, mode, arg0, arg1)
     RTX_CODE code;
     enum machine_mode mode;
     int arg0;
     char * arg1;
{
  rtx rt = obstack_alloc_rtx (sizeof (struct rtx_def) + 1 * sizeof (rtunion));
  PUT_CODE (rt, code);
  PUT_MODE (rt, mode);
  XINT (rt, 0) = arg0;
  XSTR (rt, 1) = arg1;

  return rt;
}

rtx
gen_rtx_fmt_i (code, mode, arg0)
     RTX_CODE code;
     enum machine_mode mode;
     int arg0;
{
  rtx rt = obstack_alloc_rtx (sizeof (struct rtx_def) + 0 * sizeof (rtunion));
  PUT_CODE (rt, code);
  PUT_MODE (rt, mode);
  XINT (rt, 0) = arg0;

  return rt;
}

rtx
gen_rtx_fmt_isE (code, mode, arg0, arg1, arg2)
     RTX_CODE code;
     enum machine_mode mode;
     int arg0;
     char * arg1;
     rtvec arg2;
{
  rtx rt = obstack_alloc_rtx (sizeof (struct rtx_def) + 2 * sizeof (rtunion));
  PUT_CODE (rt, code);
  PUT_MODE (rt, mode);
  XINT (rt, 0) = arg0;
  XSTR (rt, 1) = arg1;
  XVEC (rt, 2) = arg2;

  return rt;
}

rtx
gen_rtx_fmt_iE (code, mode, arg0, arg1)
     RTX_CODE code;
     enum machine_mode mode;
     int arg0;
     rtvec arg1;
{
  rtx rt = obstack_alloc_rtx (sizeof (struct rtx_def) + 1 * sizeof (rtunion));
  PUT_CODE (rt, code);
  PUT_MODE (rt, mode);
  XINT (rt, 0) = arg0;
  XVEC (rt, 1) = arg1;

  return rt;
}

rtx
gen_rtx_fmt_s (code, mode, arg0)
     RTX_CODE code;
     enum machine_mode mode;
     char * arg0;
{
  rtx rt = obstack_alloc_rtx (sizeof (struct rtx_def) + 0 * sizeof (rtunion));
  PUT_CODE (rt, code);
  PUT_MODE (rt, mode);
  XSTR (rt, 0) = arg0;

  return rt;
}

rtx
gen_rtx_fmt_Ess (code, mode, arg0, arg1, arg2)
     RTX_CODE code;
     enum machine_mode mode;
     rtvec arg0;
     char * arg1;
     char * arg2;
{
  rtx rt = obstack_alloc_rtx (sizeof (struct rtx_def) + 2 * sizeof (rtunion));
  PUT_CODE (rt, code);
  PUT_MODE (rt, mode);
  XVEC (rt, 0) = arg0;
  XSTR (rt, 1) = arg1;
  XSTR (rt, 2) = arg2;

  return rt;
}

rtx
gen_rtx_fmt_sEss (code, mode, arg0, arg1, arg2, arg3)
     RTX_CODE code;
     enum machine_mode mode;
     char * arg0;
     rtvec arg1;
     char * arg2;
     char * arg3;
{
  rtx rt = obstack_alloc_rtx (sizeof (struct rtx_def) + 3 * sizeof (rtunion));
  PUT_CODE (rt, code);
  PUT_MODE (rt, mode);
  XSTR (rt, 0) = arg0;
  XVEC (rt, 1) = arg1;
  XSTR (rt, 2) = arg2;
  XSTR (rt, 3) = arg3;

  return rt;
}

rtx
gen_rtx_fmt_eE (code, mode, arg0, arg1)
     RTX_CODE code;
     enum machine_mode mode;
     rtx arg0;
     rtvec arg1;
{
  rtx rt = obstack_alloc_rtx (sizeof (struct rtx_def) + 1 * sizeof (rtunion));
  PUT_CODE (rt, code);
  PUT_MODE (rt, mode);
  XEXP (rt, 0) = arg0;
  XVEC (rt, 1) = arg1;

  return rt;
}

rtx
gen_rtx_fmt_E (code, mode, arg0)
     RTX_CODE code;
     enum machine_mode mode;
     rtvec arg0;
{
  rtx rt = obstack_alloc_rtx (sizeof (struct rtx_def) + 0 * sizeof (rtunion));
  PUT_CODE (rt, code);
  PUT_MODE (rt, mode);
  XVEC (rt, 0) = arg0;

  return rt;
}

rtx
gen_rtx_fmt_e (code, mode, arg0)
     RTX_CODE code;
     enum machine_mode mode;
     rtx arg0;
{
  rtx rt = obstack_alloc_rtx (sizeof (struct rtx_def) + 0 * sizeof (rtunion));
  PUT_CODE (rt, code);
  PUT_MODE (rt, mode);
  XEXP (rt, 0) = arg0;

  return rt;
}

rtx
gen_rtx_fmt_sse (code, mode, arg0, arg1, arg2)
     RTX_CODE code;
     enum machine_mode mode;
     char * arg0;
     char * arg1;
     rtx arg2;
{
  rtx rt = obstack_alloc_rtx (sizeof (struct rtx_def) + 2 * sizeof (rtunion));
  PUT_CODE (rt, code);
  PUT_MODE (rt, mode);
  XSTR (rt, 0) = arg0;
  XSTR (rt, 1) = arg1;
  XEXP (rt, 2) = arg2;

  return rt;
}

rtx
gen_rtx_fmt_ss (code, mode, arg0, arg1)
     RTX_CODE code;
     enum machine_mode mode;
     char * arg0;
     char * arg1;
{
  rtx rt = obstack_alloc_rtx (sizeof (struct rtx_def) + 1 * sizeof (rtunion));
  PUT_CODE (rt, code);
  PUT_MODE (rt, mode);
  XSTR (rt, 0) = arg0;
  XSTR (rt, 1) = arg1;

  return rt;
}

rtx
gen_rtx_fmt_sE (code, mode, arg0, arg1)
     RTX_CODE code;
     enum machine_mode mode;
     char * arg0;
     rtvec arg1;
{
  rtx rt = obstack_alloc_rtx (sizeof (struct rtx_def) + 1 * sizeof (rtunion));
  PUT_CODE (rt, code);
  PUT_MODE (rt, mode);
  XSTR (rt, 0) = arg0;
  XVEC (rt, 1) = arg1;

  return rt;
}

rtx
gen_rtx_fmt_iuueiee (code, mode, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
     RTX_CODE code;
     enum machine_mode mode;
     int arg0;
     rtx arg1;
     rtx arg2;
     rtx arg3;
     int arg4;
     rtx arg5;
     rtx arg6;
{
  rtx rt = obstack_alloc_rtx (sizeof (struct rtx_def) + 6 * sizeof (rtunion));
  PUT_CODE (rt, code);
  PUT_MODE (rt, mode);
  XINT (rt, 0) = arg0;
  XEXP (rt, 1) = arg1;
  XEXP (rt, 2) = arg2;
  XEXP (rt, 3) = arg3;
  XINT (rt, 4) = arg4;
  XEXP (rt, 5) = arg5;
  XEXP (rt, 6) = arg6;

  return rt;
}

rtx
gen_rtx_fmt_iuueiee0 (code, mode, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
     RTX_CODE code;
     enum machine_mode mode;
     int arg0;
     rtx arg1;
     rtx arg2;
     rtx arg3;
     int arg4;
     rtx arg5;
     rtx arg6;
{
  rtx rt = obstack_alloc_rtx (sizeof (struct rtx_def) + 7 * sizeof (rtunion));
  PUT_CODE (rt, code);
  PUT_MODE (rt, mode);
  XINT (rt, 0) = arg0;
  XEXP (rt, 1) = arg1;
  XEXP (rt, 2) = arg2;
  XEXP (rt, 3) = arg3;
  XINT (rt, 4) = arg4;
  XEXP (rt, 5) = arg5;
  XEXP (rt, 6) = arg6;

  return rt;
}

rtx
gen_rtx_fmt_iuueieee (code, mode, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
     RTX_CODE code;
     enum machine_mode mode;
     int arg0;
     rtx arg1;
     rtx arg2;
     rtx arg3;
     int arg4;
     rtx arg5;
     rtx arg6;
     rtx arg7;
{
  rtx rt = obstack_alloc_rtx (sizeof (struct rtx_def) + 7 * sizeof (rtunion));
  PUT_CODE (rt, code);
  PUT_MODE (rt, mode);
  XINT (rt, 0) = arg0;
  XEXP (rt, 1) = arg1;
  XEXP (rt, 2) = arg2;
  XEXP (rt, 3) = arg3;
  XINT (rt, 4) = arg4;
  XEXP (rt, 5) = arg5;
  XEXP (rt, 6) = arg6;
  XEXP (rt, 7) = arg7;

  return rt;
}

rtx
gen_rtx_fmt_iuu (code, mode, arg0, arg1, arg2)
     RTX_CODE code;
     enum machine_mode mode;
     int arg0;
     rtx arg1;
     rtx arg2;
{
  rtx rt = obstack_alloc_rtx (sizeof (struct rtx_def) + 2 * sizeof (rtunion));
  PUT_CODE (rt, code);
  PUT_MODE (rt, mode);
  XINT (rt, 0) = arg0;
  XEXP (rt, 1) = arg1;
  XEXP (rt, 2) = arg2;

  return rt;
}

rtx
gen_rtx_fmt_iuuis00 (code, mode, arg0, arg1, arg2, arg3, arg4)
     RTX_CODE code;
     enum machine_mode mode;
     int arg0;
     rtx arg1;
     rtx arg2;
     int arg3;
     char * arg4;
{
  rtx rt = obstack_alloc_rtx (sizeof (struct rtx_def) + 6 * sizeof (rtunion));
  PUT_CODE (rt, code);
  PUT_MODE (rt, mode);
  XINT (rt, 0) = arg0;
  XEXP (rt, 1) = arg1;
  XEXP (rt, 2) = arg2;
  XINT (rt, 3) = arg3;
  XSTR (rt, 4) = arg4;

  return rt;
}

rtx
gen_rtx_fmt_iuuuiiiiiieeiiEeEssE (code, mode, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19)
     RTX_CODE code;
     enum machine_mode mode;
     int arg0;
     rtx arg1;
     rtx arg2;
     rtx arg3;
     int arg4;
     int arg5;
     int arg6;
     int arg7;
     int arg8;
     int arg9;
     rtx arg10;
     rtx arg11;
     int arg12;
     int arg13;
     rtvec arg14;
     rtx arg15;
     rtvec arg16;
     char * arg17;
     char * arg18;
     rtvec arg19;
{
  rtx rt = obstack_alloc_rtx (sizeof (struct rtx_def) + 19 * sizeof (rtunion));
  PUT_CODE (rt, code);
  PUT_MODE (rt, mode);
  XINT (rt, 0) = arg0;
  XEXP (rt, 1) = arg1;
  XEXP (rt, 2) = arg2;
  XEXP (rt, 3) = arg3;
  XINT (rt, 4) = arg4;
  XINT (rt, 5) = arg5;
  XINT (rt, 6) = arg6;
  XINT (rt, 7) = arg7;
  XINT (rt, 8) = arg8;
  XINT (rt, 9) = arg9;
  XEXP (rt, 10) = arg10;
  XEXP (rt, 11) = arg11;
  XINT (rt, 12) = arg12;
  XINT (rt, 13) = arg13;
  XVEC (rt, 14) = arg14;
  XEXP (rt, 15) = arg15;
  XVEC (rt, 16) = arg16;
  XSTR (rt, 17) = arg17;
  XSTR (rt, 18) = arg18;
  XVEC (rt, 19) = arg19;

  return rt;
}

rtx
gen_rtx_fmt_ssiEEsi (code, mode, arg0, arg1, arg2, arg3, arg4, arg5, arg6)
     RTX_CODE code;
     enum machine_mode mode;
     char * arg0;
     char * arg1;
     int arg2;
     rtvec arg3;
     rtvec arg4;
     char * arg5;
     int arg6;
{
  rtx rt = obstack_alloc_rtx (sizeof (struct rtx_def) + 6 * sizeof (rtunion));
  PUT_CODE (rt, code);
  PUT_MODE (rt, mode);
  XSTR (rt, 0) = arg0;
  XSTR (rt, 1) = arg1;
  XINT (rt, 2) = arg2;
  XVEC (rt, 3) = arg3;
  XVEC (rt, 4) = arg4;
  XSTR (rt, 5) = arg5;
  XINT (rt, 6) = arg6;

  return rt;
}

rtx
gen_rtx_fmt_Ei (code, mode, arg0, arg1)
     RTX_CODE code;
     enum machine_mode mode;
     rtvec arg0;
     int arg1;
{
  rtx rt = obstack_alloc_rtx (sizeof (struct rtx_def) + 1 * sizeof (rtunion));
  PUT_CODE (rt, code);
  PUT_MODE (rt, mode);
  XVEC (rt, 0) = arg0;
  XINT (rt, 1) = arg1;

  return rt;
}

rtx
gen_rtx_fmt_eEeei (code, mode, arg0, arg1, arg2, arg3, arg4)
     RTX_CODE code;
     enum machine_mode mode;
     rtx arg0;
     rtvec arg1;
     rtx arg2;
     rtx arg3;
     int arg4;
{
  rtx rt = obstack_alloc_rtx (sizeof (struct rtx_def) + 4 * sizeof (rtunion));
  PUT_CODE (rt, code);
  PUT_MODE (rt, mode);
  XEXP (rt, 0) = arg0;
  XVEC (rt, 1) = arg1;
  XEXP (rt, 2) = arg2;
  XEXP (rt, 3) = arg3;
  XINT (rt, 4) = arg4;

  return rt;
}

rtx
gen_rtx_fmt_ (code, mode)
     RTX_CODE code;
     enum machine_mode mode;
{
  rtx rt = obstack_alloc_rtx (sizeof (struct rtx_def) + -1 * sizeof (rtunion));
  PUT_CODE (rt, code);
  PUT_MODE (rt, mode);

  return rt;
}

rtx
gen_rtx_fmt_w (code, mode, arg0)
     RTX_CODE code;
     enum machine_mode mode;
     HOST_WIDE_INT arg0;
{
  rtx rt = obstack_alloc_rtx (sizeof (struct rtx_def) + 0 * sizeof (rtunion));
  PUT_CODE (rt, code);
  PUT_MODE (rt, mode);
  XWINT (rt, 0) = arg0;

  return rt;
}

rtx
gen_rtx_fmt_e0ww (code, mode, arg0, arg1, arg2)
     RTX_CODE code;
     enum machine_mode mode;
     rtx arg0;
     HOST_WIDE_INT arg1;
     HOST_WIDE_INT arg2;
{
  rtx rt = obstack_alloc_rtx (sizeof (struct rtx_def) + 3 * sizeof (rtunion));
  PUT_CODE (rt, code);
  PUT_MODE (rt, mode);
  XEXP (rt, 0) = arg0;
  XWINT (rt, 2) = arg1;
  XWINT (rt, 3) = arg2;

  return rt;
}

rtx
gen_rtx_fmt_i0 (code, mode, arg0)
     RTX_CODE code;
     enum machine_mode mode;
     int arg0;
{
  rtx rt = obstack_alloc_rtx (sizeof (struct rtx_def) + 1 * sizeof (rtunion));
  PUT_CODE (rt, code);
  PUT_MODE (rt, mode);
  XINT (rt, 0) = arg0;

  return rt;
}

rtx
gen_rtx_fmt_0 (code, mode)
     RTX_CODE code;
     enum machine_mode mode;
{
  rtx rt = obstack_alloc_rtx (sizeof (struct rtx_def) + 0 * sizeof (rtunion));
  PUT_CODE (rt, code);
  PUT_MODE (rt, mode);

  return rt;
}

rtx
gen_rtx_fmt_ei (code, mode, arg0, arg1)
     RTX_CODE code;
     enum machine_mode mode;
     rtx arg0;
     int arg1;
{
  rtx rt = obstack_alloc_rtx (sizeof (struct rtx_def) + 1 * sizeof (rtunion));
  PUT_CODE (rt, code);
  PUT_MODE (rt, mode);
  XEXP (rt, 0) = arg0;
  XINT (rt, 1) = arg1;

  return rt;
}

rtx
gen_rtx_fmt_e0 (code, mode, arg0)
     RTX_CODE code;
     enum machine_mode mode;
     rtx arg0;
{
  rtx rt = obstack_alloc_rtx (sizeof (struct rtx_def) + 1 * sizeof (rtunion));
  PUT_CODE (rt, code);
  PUT_MODE (rt, mode);
  XEXP (rt, 0) = arg0;

  return rt;
}

rtx
gen_rtx_fmt_u00 (code, mode, arg0)
     RTX_CODE code;
     enum machine_mode mode;
     rtx arg0;
{
  rtx rt = obstack_alloc_rtx (sizeof (struct rtx_def) + 2 * sizeof (rtunion));
  PUT_CODE (rt, code);
  PUT_MODE (rt, mode);
  XEXP (rt, 0) = arg0;

  return rt;
}

rtx
gen_rtx_fmt_ei0 (code, mode, arg0, arg1)
     RTX_CODE code;
     enum machine_mode mode;
     rtx arg0;
     int arg1;
{
  rtx rt = obstack_alloc_rtx (sizeof (struct rtx_def) + 2 * sizeof (rtunion));
  PUT_CODE (rt, code);
  PUT_MODE (rt, mode);
  XEXP (rt, 0) = arg0;
  XINT (rt, 1) = arg1;

  return rt;
}

rtx
gen_rtx_fmt_eeeee (code, mode, arg0, arg1, arg2, arg3, arg4)
     RTX_CODE code;
     enum machine_mode mode;
     rtx arg0;
     rtx arg1;
     rtx arg2;
     rtx arg3;
     rtx arg4;
{
  rtx rt = obstack_alloc_rtx (sizeof (struct rtx_def) + 4 * sizeof (rtunion));
  PUT_CODE (rt, code);
  PUT_MODE (rt, mode);
  XEXP (rt, 0) = arg0;
  XEXP (rt, 1) = arg1;
  XEXP (rt, 2) = arg2;
  XEXP (rt, 3) = arg3;
  XEXP (rt, 4) = arg4;

  return rt;
}

rtx
gen_rtx_fmt_eee (code, mode, arg0, arg1, arg2)
     RTX_CODE code;
     enum machine_mode mode;
     rtx arg0;
     rtx arg1;
     rtx arg2;
{
  rtx rt = obstack_alloc_rtx (sizeof (struct rtx_def) + 2 * sizeof (rtunion));
  PUT_CODE (rt, code);
  PUT_MODE (rt, mode);
  XEXP (rt, 0) = arg0;
  XEXP (rt, 1) = arg1;
  XEXP (rt, 2) = arg2;

  return rt;
}

rtx
gen_rtx_fmt_Ee (code, mode, arg0, arg1)
     RTX_CODE code;
     enum machine_mode mode;
     rtvec arg0;
     rtx arg1;
{
  rtx rt = obstack_alloc_rtx (sizeof (struct rtx_def) + 1 * sizeof (rtunion));
  PUT_CODE (rt, code);
  PUT_MODE (rt, mode);
  XVEC (rt, 0) = arg0;
  XEXP (rt, 1) = arg1;

  return rt;
}

rtx
gen_rtx_fmt_uuEiiiiiibbii (code, mode, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12)
     RTX_CODE code;
     enum machine_mode mode;
     rtx arg0;
     rtx arg1;
     rtvec arg2;
     int arg3;
     int arg4;
     int arg5;
     int arg6;
     int arg7;
     int arg8;
     void * arg9;
     void * arg10;
     int arg11;
     int arg12;
{
  rtx rt = obstack_alloc_rtx (sizeof (struct rtx_def) + 12 * sizeof (rtunion));
  PUT_CODE (rt, code);
  PUT_MODE (rt, mode);
  XEXP (rt, 0) = arg0;
  XEXP (rt, 1) = arg1;
  XVEC (rt, 2) = arg2;
  XINT (rt, 3) = arg3;
  XINT (rt, 4) = arg4;
  XINT (rt, 5) = arg5;
  XINT (rt, 6) = arg6;
  XINT (rt, 7) = arg7;
  XINT (rt, 8) = arg8;
  XBITMAP (rt, 9) = arg9;
  XBITMAP (rt, 10) = arg10;
  XINT (rt, 11) = arg11;
  XINT (rt, 12) = arg12;

  return rt;
}

rtx
gen_rtx_fmt_iiiiiiiitt (code, mode, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)
     RTX_CODE code;
     enum machine_mode mode;
     int arg0;
     int arg1;
     int arg2;
     int arg3;
     int arg4;
     int arg5;
     int arg6;
     int arg7;
     void * arg8;
     void * arg9;
{
  rtx rt = obstack_alloc_rtx (sizeof (struct rtx_def) + 9 * sizeof (rtunion));
  PUT_CODE (rt, code);
  PUT_MODE (rt, mode);
  XINT (rt, 0) = arg0;
  XINT (rt, 1) = arg1;
  XINT (rt, 2) = arg2;
  XINT (rt, 3) = arg3;
  XINT (rt, 4) = arg4;
  XINT (rt, 5) = arg5;
  XINT (rt, 6) = arg6;
  XINT (rt, 7) = arg7;
  XTREE (rt, 8) = arg8;
  XTREE (rt, 9) = arg9;

  return rt;
}

rtx
gen_rtx_fmt_eti (code, mode, arg0, arg1, arg2)
     RTX_CODE code;
     enum machine_mode mode;
     rtx arg0;
     void * arg1;
     int arg2;
{
  rtx rt = obstack_alloc_rtx (sizeof (struct rtx_def) + 2 * sizeof (rtunion));
  PUT_CODE (rt, code);
  PUT_MODE (rt, mode);
  XEXP (rt, 0) = arg0;
  XTREE (rt, 1) = arg1;
  XINT (rt, 2) = arg2;

  return rt;
}

rtx
gen_rtx_fmt_bi (code, mode, arg0, arg1)
     RTX_CODE code;
     enum machine_mode mode;
     void * arg0;
     int arg1;
{
  rtx rt = obstack_alloc_rtx (sizeof (struct rtx_def) + 1 * sizeof (rtunion));
  PUT_CODE (rt, code);
  PUT_MODE (rt, mode);
  XBITMAP (rt, 0) = arg0;
  XINT (rt, 1) = arg1;

  return rt;
}

rtx
gen_rtx_fmt_uuuu (code, mode, arg0, arg1, arg2, arg3)
     RTX_CODE code;
     enum machine_mode mode;
     rtx arg0;
     rtx arg1;
     rtx arg2;
     rtx arg3;
{
  rtx rt = obstack_alloc_rtx (sizeof (struct rtx_def) + 3 * sizeof (rtunion));
  PUT_CODE (rt, code);
  PUT_MODE (rt, mode);
  XEXP (rt, 0) = arg0;
  XEXP (rt, 1) = arg1;
  XEXP (rt, 2) = arg2;
  XEXP (rt, 3) = arg3;

  return rt;
}

