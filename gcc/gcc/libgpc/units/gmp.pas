{
Definitions for GNU multiple precision functions: arithmetic with
integer, rational and real numbers of arbitrary size and precision.

Translation of the C header (gmp.h) of the GMP library. Only tested
with GMP version 2.0.2!

To use the GMP unit, you will need the GMP library which can be
found in ftp://agnes.dida.physik.uni-essen.de/gnu-pascal/libs/ .

Copyright (C) 1998-2000 Free Software Foundation, Inc.

Author: Frank Heckenbach <frank@pascal.gnu.de>

This file is part of GNU Pascal.

GNU Pascal is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU Pascal is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Pascal; see the file COPYING. If not, write to the
Free Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.

As a special exception, if you link this file with files compiled
with a GNU compiler to produce an executable, this does not cause
the resulting executable to be covered by the GNU General Public
License. This exception does not however invalidate any other
reasons why the executable file might be covered by the GNU General
Public License.

Please also note the license of the GMP unit.
}

{$gnu-pascal}
{$if __GPC_RELEASE__ < 20000412}
{$error This unit requires GPC release 20000412 or newer}
{$endif}
{$nested-comments}

unit gmp;

interface

uses GPC;

type
  {$ifdef _SHORT_LIMB}
  mp_limb_t        = Cardinal;
  mp_limb_signed_t = Integer;
  {$elif defined(_LONG_LONG_LIMB)}
  mp_limb_t        = LongCard;
  mp_limb_signed_t = LongInt;
  {$else}
  mp_limb_t        = MedCard;
  mp_limb_signed_t = MedInt;
  {$endif}

  mp_ptr           = ^mp_limb_t;
  mp_size_t        = MedInt;
  mp_exp_t         = MedInt;

  mpz_t = record
    mp_alloc,
    mp_size  : {$ifdef __MP_SMALL__}
               ShortInt
               {$else}
               Integer
               {$endif};
    mp_d     : mp_ptr
  end;

  mpz_array_ptr = ^mpz_array;
  mpz_array = array [0 .. MaxVarSize div SizeOf (mpz_t)] of mpz_t;

  mpq_t = record
    mp_num,
    mp_den : mpz_t
  end;

  mpf_t = record
    mp_prec,
    mp_size : Integer;
    mp_exp  : mp_exp_t;
    mp_d    : mp_ptr
  end;

  TAllocFunction    = function (Size : SizeType) : Pointer;
  TReAllocFunction  = function (var Dest : Pointer; OldSize, NewSize : SizeType) : Pointer;
  TDeAllocProcedure = procedure (Src : Pointer; Size : SizeType);

procedure mp_set_memory_functions (AllocFunction : TAllocFunction;
                                   ReAllocFunction : TReAllocFunction;
                                   DeAllocProcedure : TDeAllocProcedure); C;

var
  mp_bits_per_limb : Integer; asmname 'mp_bits_per_limb';

{**************** Integer (i.e. Z) routines.  ****************}

procedure mpz_init             (var Dest : mpz_t); C;
procedure mpz_clear            (var Dest : mpz_t); C;
function  mpz_realloc          (var Dest : mpz_t; NewAlloc : mp_size_t) : Pointer; asmname '_mpz_realloc';
procedure mpz_array_init       (Dest : mpz_array_ptr; ArraySize, FixedNumBits : mp_size_t); C;

procedure mpz_set              (var Dest : mpz_t; protected var Src : mpz_t); C;
procedure mpz_set_ui           (var Dest : mpz_t; Src : MedCard); C;
procedure mpz_set_si           (var Dest : mpz_t; Src : MedInt); C;
procedure mpz_set_d            (var Dest : mpz_t; Src : Double); C;
procedure mpz_set_q            (var Dest : mpz_t; Src : mpq_t); C;
procedure mpz_set_f            (var Dest : mpz_t; Src : mpf_t); C;
function  mpz_set_str          (var Dest : mpz_t; Src : CString; Base : Integer) : Integer; C;

procedure mpz_init_set         (var Dest : mpz_t; protected var Src : mpz_t); C;
procedure mpz_init_set_ui      (var Dest : mpz_t; Src : MedCard); C;
procedure mpz_init_set_si      (var Dest : mpz_t; Src : MedInt); C;
procedure mpz_init_set_d       (var Dest : mpz_t; Src : Double); C;
function  mpz_init_set_str     (var Dest : mpz_t; Src : CString; Base : Integer) : Integer; C;

function  mpz_get_ui           (protected var Src : mpz_t) : MedCard; C;
function  mpz_get_si           (protected var Src : mpz_t) : MedInt; C;
function  mpz_get_d            (protected var Src : mpz_t) : Double; C;
{ Pass nil for Dest to let the function allocate memory for it }
function  mpz_get_str          (Dest : CString; Base : Integer; protected var Src : mpz_t) : CString; C;

procedure mpz_add              (var Dest : mpz_t; protected var Src1, Src2 : mpz_t); C;
procedure mpz_add_ui           (var Dest : mpz_t; protected var Src1 : mpz_t; Src2 : MedCard); C;
procedure mpz_sub              (var Dest : mpz_t; protected var Src1, Src2 : mpz_t); C;
procedure mpz_sub_ui           (var Dest : mpz_t; protected var Src1 : mpz_t; Src2 : MedCard); C;
procedure mpz_mul              (var Dest : mpz_t; protected var Src1, Src2 : mpz_t); C;
procedure mpz_mul_ui           (var Dest : mpz_t; protected var Src1 : mpz_t; Src2 : MedCard); C;
procedure mpz_mul_2exp         (var Dest : mpz_t; protected var Src1 : mpz_t; Src2 : MedCard); C;
procedure mpz_neg              (var Dest : mpz_t; protected var Src : mpz_t); C;
procedure mpz_abs              (var Dest : mpz_t; protected var Src : mpz_t); C;
procedure mpz_fac_ui           (var Dest : mpz_t; Src : MedCard); C;

procedure mpz_tdiv_q           (var Dest : mpz_t; protected var Src1, Src2 : mpz_t); C;
procedure mpz_tdiv_q_ui        (var Dest : mpz_t; protected var Src1 : mpz_t; Src2 : MedCard); C;
procedure mpz_tdiv_r           (var Dest : mpz_t; protected var Src1, Src2 : mpz_t); C;
procedure mpz_tdiv_r_ui        (var Dest : mpz_t; protected var Src1 : mpz_t; Src2 : MedCard); C;
procedure mpz_tdiv_qr          (var DestQ, DestR : mpz_t; protected var Src1, Src2 : mpz_t); C;
procedure mpz_tdiv_qr_ui       (var DestQ, DestR : mpz_t; protected var Src1 : mpz_t; Src2 : MedCard); C;

procedure mpz_fdiv_q           (var Dest : mpz_t; protected var Src1, Src2 : mpz_t); C;
function  mpz_fdiv_q_ui        (var Dest : mpz_t; protected var Src1 : mpz_t; Src2 : MedCard) : MedCard; C;
procedure mpz_fdiv_r           (var Dest : mpz_t; protected var Src1, Src2 : mpz_t); C;
function  mpz_fdiv_r_ui        (var Dest : mpz_t; protected var Src1 : mpz_t; Src2 : MedCard) : MedCard; C;
procedure mpz_fdiv_qr          (var DestQ, DestR : mpz_t; protected var Src1, Src2 : mpz_t); C;
function  mpz_fdiv_qr_ui       (var DestQ, DestR : mpz_t; protected var Src1 : mpz_t; Src2 : MedCard) : MedCard; C;
function  mpz_fdiv_ui          (protected var Src1 : mpz_t; Src2 : MedCard) : MedCard; C;

procedure mpz_cdiv_q           (var Dest : mpz_t; protected var Src1, Src2 : mpz_t); C;
function  mpz_cdiv_q_ui        (var Dest : mpz_t; protected var Src1 : mpz_t; Src2 : MedCard) : MedCard; C;
procedure mpz_cdiv_r           (var Dest : mpz_t; protected var Src1, Src2 : mpz_t); C;
function  mpz_cdiv_r_ui        (var Dest : mpz_t; protected var Src1 : mpz_t; Src2 : MedCard) : MedCard; C;
procedure mpz_cdiv_qr          (var DestQ, DestR : mpz_t; protected var Src1,Src2 : mpz_t); C;
function  mpz_cdiv_qr_ui       (var DestQ, DestR : mpz_t; protected var Src1 : mpz_t; Src2 : MedCard) : MedCard; C;
function  mpz_cdiv_ui          (protected var Src1 : mpz_t; Src2:MedCard) : MedCard; C;

procedure mpz_mod              (var Dest : mpz_t; protected var Src1,Src2 : mpz_t); C;
procedure mpz_divexact         (var Dest : mpz_t; protected var Src1,Src2 : mpz_t); C;

procedure mpz_tdiv_q_2exp      (var Dest : mpz_t; protected var Src1 : mpz_t; Src2 : MedCard); C;
procedure mpz_tdiv_r_2exp      (var Dest : mpz_t; protected var Src1 : mpz_t; Src2 : MedCard); C;
procedure mpz_fdiv_q_2exp      (var Dest : mpz_t; protected var Src1 : mpz_t; Src2 : MedCard); C;
procedure mpz_fdiv_r_2exp      (var Dest : mpz_t; protected var Src1 : mpz_t; Src2 : MedCard); C;

procedure mpz_powm             (var Dest : mpz_t; protected var Base, Exponent, Modulus : mpz_t); C;
procedure mpz_powm_ui          (var Dest : mpz_t; protected var Base : mpz_t; Exponent : MedCard; protected var Modulus : mpz_t); C;
procedure mpz_pow_ui           (var Dest : mpz_t; protected var Base : mpz_t; Exponent : MedCard); C;
procedure mpz_ui_pow_ui        (var Dest : mpz_t; Base, Exponent : MedCard); C;

procedure mpz_sqrt             (var Dest : mpz_t; protected var Src : mpz_t); C;
procedure mpz_sqrtrem          (var Dest, DestR : mpz_t; protected var Src : mpz_t); C;
function  mpz_perfect_square_p (protected var Src : mpz_t) : Integer; C;

function  mpz_probab_prime_p   (protected var Src : mpz_t; Repetitions : Integer) : Integer; C;
procedure mpz_gcd              (var Dest : mpz_t; protected var Src1, Src2 : mpz_t); C;
function  mpz_gcd_ui           (var Dest : mpz_t; protected var Src1 : mpz_t; Src2 : MedCard) : MedCard; C;
procedure mpz_gcdext           (var Dest, DestA, DestB : mpz_t; protected var SrcA, SrcB : mpz_t); C;
function  mpz_invert           (var Dest : mpz_t; protected var Src, Modulus : mpz_t) : Integer; C;
function  mpz_jacobi           (protected var Src1, Src2 : mpz_t) : Integer; C;
function  mpz_legendre         (protected var Src1, Src2 : mpz_t) : Integer; C;

function  mpz_cmp              (protected var Src1, Src2 : mpz_t) : Integer; C;
function  mpz_cmp_ui           (protected var Src1 : mpz_t; Src2 : MedCard) : Integer; C;
function  mpz_cmp_si           (protected var Src1 : mpz_t; Src2 : MedInt) : Integer; C;
function  mpz_sgn              (protected var Src : mpz_t) : Integer;

procedure mpz_and              (var Dest : mpz_t; protected var Src1, Src2 : mpz_t); C;
procedure mpz_ior              (var Dest : mpz_t; protected var Src1, Src2 : mpz_t); C;
procedure mpz_com              (var Dest : mpz_t; protected var Src : mpz_t); C;
function  mpz_popcount         (protected var Src : mpz_t) : MedCard; C;
function  mpz_hamdist          (protected var Src1, Src2 : mpz_t) : MedCard; C;
function  mpz_scan0            (protected var Src : mpz_t; StartingBit : MedCard) : MedCard; C;
function  mpz_scan1            (protected var Src : mpz_t; StartingBit : MedCard) : MedCard; C;
procedure mpz_setbit           (var Dest : mpz_t; BitIndex : MedCard); C;
procedure mpz_clrbit           (var Dest : mpz_t; BitIndex : MedCard); C;

procedure mpz_random           (var Dest : mpz_t; MaxSize : mp_size_t); C;
procedure mpz_random2          (var Dest : mpz_t; MaxSize : mp_size_t); C;
function  mpz_sizeinbase       (protected var Src : mpz_t; Base : Integer) : SizeType; C;

{**************** Rational (i.e. Q) routines.  ****************}

procedure mpq_canonicalize     (var Dest : mpq_t); C;

procedure mpq_init             (var Dest : mpq_t); C;
procedure mpq_clear            (var Dest : mpq_t); C;
procedure mpq_set              (var Dest : mpq_t; protected var Src : mpq_t); C;
procedure mpq_set_z            (var Dest : mpq_t; protected var Src : mpz_t); C;
procedure mpq_set_ui           (var Dest : mpq_t; Nom, Den : MedCard); C;
procedure mpq_set_si           (var Dest : mpq_t; Nom : MedInt; Den : MedCard); C;

procedure mpq_add              (var Dest : mpq_t; protected var Src1, Src2 : mpq_t); C;
procedure mpq_sub              (var Dest : mpq_t; protected var Src1, Src2 : mpq_t); C;
procedure mpq_mul              (var Dest : mpq_t; protected var Src1, Src2 : mpq_t); C;
procedure mpq_div              (var Dest : mpq_t; protected var Src1, Src2 : mpq_t); C;
procedure mpq_neg              (var Dest : mpq_t; protected var Src : mpq_t); C;
procedure mpq_inv              (var Dest : mpq_t; protected var Src : mpq_t); C;

function  mpq_cmp              (protected var Src1, Src2 : mpq_t) : Integer; C;
function  mpq_cmp_ui           (protected var Src1 : mpq_t; Nom2, Den2 : MedCard) : Integer; C;
function  mpq_sgn              (protected var Src : mpq_t) : Integer;
function  mpq_equal            (protected var Src1, Src2 : mpq_t) : Integer; C;

function  mpq_get_d            (protected var Src : mpq_t) : Double; C;
procedure mpq_set_num          (var Dest : mpq_t; protected var Src : mpz_t); C;
procedure mpq_set_den          (var Dest : mpq_t; protected var Src : mpz_t); C;
procedure mpq_get_num          (var Dest : mpz_t; protected var Src : mpq_t); C;
procedure mpq_get_den          (var Dest : mpz_t; protected var Src : mpq_t); C;

{**************** Float (i.e. R) routines.  ****************}

procedure mpf_set_default_prec (Precision : MedCard); C;
procedure mpf_init             (var Dest : mpf_t); C;
procedure mpf_init2            (var Dest : mpf_t; Precision : MedCard); C;
procedure mpf_clear            (var Dest : mpf_t); C;
procedure mpf_set_prec         (var Dest : mpf_t; Precision : MedCard); C;
function  mpf_get_prec         (protected var Src : mpf_t) : MedCard; C;
procedure mpf_set_prec_raw     (var Dest : mpf_t; Precision : MedCard); C;

procedure mpf_set              (var Dest : mpf_t; protected var Src : mpf_t); C;
procedure mpf_set_ui           (var Dest : mpf_t; Src : MedCard); C;
procedure mpf_set_si           (var Dest : mpf_t; Src : MedInt); C;
procedure mpf_set_d            (var Dest : mpf_t; Src : Double); C;
procedure mpf_set_z            (var Dest : mpf_t; protected var Src : mpz_t); C;
procedure mpf_set_q            (var Dest : mpf_t; protected var Src : mpq_t); C;
function  mpf_set_str          (var Dest : mpf_t; Src : CString; Base : Integer) : Integer; C;

procedure mpf_init_set         (var Dest : mpf_t; protected var Src : mpf_t); C;
procedure mpf_init_set_ui      (var Dest : mpf_t; Src : MedCard); C;
procedure mpf_init_set_si      (var Dest : mpf_t; Src : MedInt); C;
procedure mpf_init_set_d       (var Dest : mpf_t; Src : Double); C;
function  mpf_init_set_str     (var Dest : mpf_t; Src : CString; Base : Integer) : Integer; C;

function  mpf_get_d            (protected var Src : mpf_t) : Double; C;
{ Pass nil for Dest to let the function allocate memory for it }
function  mpf_get_str          (Dest : CString; var Exponent : mp_exp_t; Base : Integer; NumberOfDigits : SizeType; protected var Src : mpf_t) : CString; C;

procedure mpf_add              (var Dest : mpf_t; protected var Src1, Src2 : mpf_t); C;
procedure mpf_add_ui           (var Dest : mpf_t; protected var Src1 : mpf_t; Src2 : MedCard); C;
procedure mpf_sub              (var Dest : mpf_t; protected var Src1, Src2 : mpf_t); C;
procedure mpf_ui_sub           (var Dest : mpf_t; Src1 : MedCard; protected var Src2 : mpf_t); C;
procedure mpf_sub_ui           (var Dest : mpf_t; protected var Src1 : mpf_t; Src2 : MedCard); C;
procedure mpf_mul              (var Dest : mpf_t; protected var Src1, Src2 : mpf_t); C;
procedure mpf_mul_ui           (var Dest : mpf_t; protected var Src1 : mpf_t; Src2 : MedCard); C;
procedure mpf_div              (var Dest : mpf_t; protected var Src1, Src2 : mpf_t); C;
procedure mpf_ui_div           (var Dest : mpf_t; Src1 : MedCard; protected var Src2 : mpf_t); C;
procedure mpf_div_ui           (var Dest : mpf_t; protected var Src1 : mpf_t; Src2 : MedCard); C;
procedure mpf_sqrt             (var Dest : mpf_t; protected var Src : mpf_t); C;
procedure mpf_sqrt_ui          (var Dest : mpf_t; Src : MedCard); C;
procedure mpf_neg              (var Dest : mpf_t; protected var Src : mpf_t); C;
procedure mpf_abs              (var Dest : mpf_t; protected var Src : mpf_t); C;
procedure mpf_mul_2exp         (var Dest : mpf_t; protected var Src1 : mpf_t; Src2 : MedCard); C;
procedure mpf_div_2exp         (var Dest : mpf_t; protected var Src1 : mpf_t; Src2 : MedCard); C;

function  mpf_cmp              (protected var Src1, Src2 : mpf_t) : Integer; C;
function  mpf_cmp_si           (protected var Src1 : mpf_t; Src2 : MedInt) : Integer;
function  mpf_cmp_ui           (protected var Src1 : mpf_t; Src2 : MedCard) : Integer;
function  mpf_eq               (protected var Src1, Src2; NumberOfBits : MedCard) : Integer; C;
procedure mpf_reldiff          (var Dest : mpf_t; protected var Src1, Src2 : mpf_t); C;
function  mpf_sgn              (protected var Src : mpf_t)  : Integer;

procedure mpf_random2          (var Dest : mpf_t; MaxSize : mp_size_t; MaxExp : mp_exp_t); C;

{$if 0} (*@@ commented out because they use C files *)
function  mpz_inp_str          (var Dest : mpz_t; Src : CFilePtr; Base : Integer) : SizeType; C;
function  mpz_inp_raw          (var Dest : mpz_t; Src : CFilePtr) : SizeType ; C;
function  mpz_out_str          (Dest : CFilePtr; Base : Integer; protected var Src : mpz_t) : SizeType; C;
function  mpz_out_raw          (Dest : CFilePtr; protected var Src : mpz_t) : SizeType ; C;
(*@@ mpf_out_str has a bug in GMP 2.0.2: it writes a spurious #0 before the exponent for negative numbers*)
function  mpf_out_str          (Dest : CFilePtr; Base : Integer; NumberOfDigits : SizeType; protected var Src : mpf_t) : SizeType; C;
function  mpf_inp_str          (var Dest : mpf_t; Src : CFilePtr; Base : Integer) : SizeType; C;
{$endif}

{ Extensions to the GMP library, implemented in this unit }

procedure mpf_exp    (var Dest : mpf_t; protected var Src : mpf_t);
procedure mpf_ln     (var Dest : mpf_t; protected var Src : mpf_t);
procedure mpf_pow    (var Dest : mpf_t; protected var Src1, Src2 : mpf_t);
procedure mpf_arctan (var c : mpf_t; protected var x : mpf_t);
procedure mpf_pi     (var c : mpf_t);

implementation

{$L gmp}

(*@@ Should rather be inline and in the interface*)

function mpz_sgn (protected var Src : mpz_t) : Integer;
begin
  if Src.mp_size < 0 then
    mpz_sgn := -1
  else if Src.mp_size > 0 then
    mpz_sgn := 1
  else
    mpz_sgn := 0
end;

function mpq_sgn (protected var Src : mpq_t) : Integer;
begin
  if Src.mp_num.mp_size < 0 then
    mpq_sgn := -1
  else if Src.mp_num.mp_size > 0 then
    mpq_sgn := 1
  else
    mpq_sgn := 0
end;

function mpf_sgn (protected var Src : mpf_t)  : Integer;
begin
  if Src.mp_size < 0 then
    mpf_sgn := -1
  else if Src.mp_size > 0 then
    mpf_sgn := 1
  else
    mpf_sgn := 0
end;

(*@@ GMP 2.0.2 has a bug in mpf_cmp_si and mpf_cmp_ui, so work around :-( *)

function mpf_cmp_si (protected var Src1 : mpf_t; Src2 : MedInt) : Integer;
var Temp : mpf_t;
begin
  mpf_init_set_si (Temp, Src2);
  mpf_cmp_si := mpf_cmp (Src1, Temp);
  mpf_clear (Temp)
end;

function mpf_cmp_ui (protected var Src1 : mpf_t; Src2 : MedCard) : Integer;
var Temp : mpf_t;
begin
  mpf_init_set_ui (Temp, Src2);
  mpf_cmp_ui := mpf_cmp (Src1, Temp);
  mpf_clear (Temp)
end;

inline function GetExp (protected var x : mpf_t) = Exp : mp_exp_t;
(*@@ This is a kludge, but how to get the exponent (of base 2) in a better way? *)
begin
  Dispose (mpf_get_str (nil, Exp, 2, 0, x))
end;

procedure mpf_exp (var Dest : mpf_t; protected var Src : mpf_t);
{ $$ \exp x = \sum_{n = 0}^{\infty} \frac{x^n}{n!} $$
  The series is used for $x \in [0, 1]$, other values of $x$ are scaled. }
var
  y, s, c0 : mpf_t;
  Precision, n : MedCard;
  Exp, i : mp_exp_t;
  Negative : Boolean;
begin
  Precision := mpf_get_prec (Dest);
  mpf_init2 (y, Precision);
  mpf_set (y, Src);
  mpf_set_ui (Dest, 1);
  Negative := mpf_sgn (y) < 0;
  if Negative then mpf_neg (y, y);
  Exp := GetExp (y);
  if Exp > 0 then mpf_div_2exp (y, y, Exp);
  mpf_init2 (c0, Precision);
  mpf_init2 (s, Precision);
  mpf_set_ui (s, 1);
  n := 1;
  repeat
    mpf_mul (s, s, y);
    mpf_div_ui (s, s, n);
    mpf_set (c0, Dest);
    mpf_add (Dest, Dest, s);
    Inc (n)
  until mpf_eq (c0, Dest, Precision) <> 0;
  for i := 1 to Exp do mpf_mul (Dest, Dest, Dest);
  if Negative then mpf_ui_div (Dest, 1, Dest);
  mpf_clear (s);
  mpf_clear (c0);
  mpf_clear (y)
end;

procedure mpf_ln (var Dest : mpf_t; protected var Src : mpf_t);
{ $$ \ln x = \sum_{n = 1}^{\infty} - \frac{(1-x)^n}{n}, \quad x \in ]0, 2] \Rightarrow $$
  $$ \ln 2^i y = -i \ln \frac{1}{2} + \sum_{n = 1}^{\infty} - \frac{(1-y)^n}{n},
     \quad y \in \left[ \frac{1}{2}, 1 \right], i \in \mathbf{Z} $$ }
var
  y, s, p, c0, Half : mpf_t;
  LnHalf : static mpf_t;
  LnHalfInited : static Boolean = False;
  n, Precision : MedCard;
  Exp : mp_exp_t;
  Dummy : Double;
begin
  if mpf_sgn (Src) <= 0 then
    begin
      Dummy := Ln (0); { Generate an error }
      Exit
    end;
  Precision := mpf_get_prec (Dest);
  mpf_init2 (y, Precision);
  mpf_set (y, Src);
  mpf_set_ui (Dest, 0);
  Exp := GetExp (y);
  if Exp <> 0 then
    begin
      if not LnHalfInited or (mpf_get_prec (LnHalf) < Precision) then
        begin
          if LnHalfInited then mpf_clear (LnHalf);
          mpf_init2 (LnHalf, Precision);
          mpf_init2 (Half, Precision);
          mpf_set_d (Half, 0.5);
          mpf_ln (LnHalf, Half);
          mpf_clear (Half)
        end;
      mpf_set (Dest, LnHalf);
      mpf_mul_ui (Dest, Dest, abs (Exp));
      if Exp > 0
        then
          begin
            mpf_neg (Dest, Dest);
            mpf_div_2exp (y, y, Exp)
          end
        else mpf_mul_2exp (y, y, - Exp)
    end;
  mpf_ui_sub (y, 1, y);
  mpf_init2 (c0, Precision);
  mpf_init2 (s, Precision);
  mpf_init2 (p, Precision);
  mpf_set_si (p, -1);
  n := 1;
  repeat
    mpf_mul (p, p, y);
    mpf_div_ui (s, p, n);
    mpf_set (c0, Dest);
    mpf_add (Dest, Dest, s);
    Inc (n)
  until mpf_eq (c0, Dest, Precision) <> 0;
  mpf_clear (p);
  mpf_clear (s);
  mpf_clear (c0);
  mpf_clear (y)
end;

procedure mpf_pow (var Dest : mpf_t; protected var Src1, Src2 : mpf_t);
var Temp : mpf_t;
begin
  mpf_init2 (Temp, mpf_get_prec (Src1));
  mpf_ln (Temp, Src1);
  mpf_mul (Temp, Temp, Src2);
  mpf_exp (Dest, Temp);
  mpf_clear (Temp)
end;

procedure mpf_arctan (var c : mpf_t; protected var x : mpf_t);
{ $$\arctan x = \sum_{n=0}^{\infty} (-1)^n \frac{x^{2n+1}}{2n+1}$$ }
var
  p, mx2, c0, s : mpf_t;
  Precision, n : MedCard;
begin
  Precision := mpf_get_prec (c);
  mpf_init2 (p, Precision);
  mpf_set (p, x);
  mpf_init2 (mx2, Precision);
  mpf_mul (mx2, x, x);
  mpf_neg (mx2, mx2);
  mpf_init2 (c0, Precision);
  mpf_init2 (s, Precision);
  mpf_set (c, x);
  n := 1;
  repeat
    mpf_mul (p, p, mx2);
    mpf_div_ui (s, p, 2 * n + 1);
    mpf_set (c0, c);
    mpf_add (c, c, s);
    Inc (n)
  until mpf_eq (c0, c, Precision) <> 0;
  mpf_clear (s);
  mpf_clear (c0);
  mpf_clear (mx2);
  mpf_clear (p)
end;

procedure mpf_pi (var c : mpf_t);
{ 4 arctan 1/5 - arctan 1/239 = pi/4 }
var b : mpf_t;
begin
  mpf_set_ui (c, 1);
  mpf_div_ui (c, c, 5);
  mpf_arctan (c, c);
  mpf_mul_ui (c, c, 4);
  mpf_init2 (b, mpf_get_prec (c));
  mpf_set_ui (b, 1);
  mpf_div_ui (b, b, 239);
  mpf_arctan (b, b);
  mpf_sub (c, c, b);
  mpf_mul_ui (c, c, 4);
  mpf_clear (b)
end;

end.
