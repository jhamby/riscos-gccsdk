{
Mathematical routines

Copyright (C) 1997-2000 Free Software Foundation, Inc.

Authors: Frank Heckenbach <frank@pascal.gnu.de>
         Jukka Virtanen <jtv@hut.fi>

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
}

unit Math;

interface

uses Error;

const
  GPC_Pi = 3.14159265358979323846264338327950288419716;

function Arctan2 (y, x : Double) : Double; asmname '_p_arctan2';
function SinH    (x : Double)    : Double; asmname '_p_sinh';
function CosH    (x : Double)    : Double; asmname '_p_cosh';

function GPC_Frac (x : LongestReal) : LongestReal; asmname '_p_frac';
function GPC_Int  (x : LongestReal) : LongestReal; asmname '_p_int';

function Real_Pow     (x : Double  ; y : Integer) : Double;   asmname '_p_pow';
function LongReal_Pow (x : LongReal; y : Integer) : LongReal; asmname '_p_l_pow';

function Complex_Abs    (z : Complex)              : Double;  asmname '_p_z_abs';
function Complex_Arg    (z : Complex)              : Double;  asmname '_p_arg';
function Complex_Polar  (Length, Theta : Double)   : Complex; asmname '_p_polar';
function Complex_Arctan (z : Complex)              : Complex; asmname '_p_z_arctan';
function Complex_Sqrt   (z : Complex)              : Complex; asmname '_p_z_sqrt';
function Complex_Ln     (z : Complex)              : Complex; asmname '_p_z_ln';
function Complex_Exp    (z : Complex)              : Complex; asmname '_p_z_exp';
function Complex_Sin    (z : Complex)              : Complex; asmname '_p_z_sin';
function Complex_Cos    (z : Complex)              : Complex; asmname '_p_z_cos';
function Complex_Pow    (z : Complex; y : Integer) : Complex; asmname '_p_z_pow';
function Complex_Power  (z : Complex; y : Double)  : Complex; asmname '_p_z_expon';

implementation

{$B-,I-}

function GPC_Frac (x : LongestReal) : LongestReal;
const f = 1 shl (BitSizeOf (LongestInt) div 2);
var
  Negative : Boolean;
  Exp : Integer;
  DigitVal : LongestReal;
begin
  if (Ord (x = 0) + Ord (x < 0) + Ord (x > 0) <> 1) or (x + x = x) then
    begin
      GPC_Frac := x;
      Exit
    end;
  Negative := x < 0;
  if Negative then x := - x;
  Exp := 0;
  DigitVal := 1;
  while x / DigitVal >= f do
    begin
      DigitVal := DigitVal * f;
      Inc (Exp)
    end;
  while (Exp >= 0) and (x <> 0) do
    begin
      x := x - Trunc (x / DigitVal) * DigitVal;
      DigitVal := DigitVal / f;
      Dec (Exp)
    end;
  if Negative then GPC_Frac := - x else GPC_Frac := x
end;

function GPC_Int (x : LongestReal) : LongestReal;
begin
  if (Ord (x = 0) + Ord (x < 0) + Ord (x > 0) <> 1) or (x + x = x)
    then GPC_Int := 0
    else GPC_Int := x - GPC_Frac (x)
end;

function Real_Pow (x : Double; y : Integer) = r : Double;
begin
  if y < 0 then
    begin
      if x = 0 then
        begin
          SetReturnAddress (ReturnAddress (0));
          RuntimeError (703); { Executed `x pow y' when x is zero and y < 0 }
          RestoreReturnAddress
        end;
      x := 1 / x;
      y := - y
    end;
  r := 1;
  while y <> 0 do
    begin
      if Odd (y) then r := r * x;
      y := y shr 1;
      if y <> 0 then x := Sqr (x)
    end
end;

function LongReal_Pow (x : LongReal; y : Integer) = r : LongReal;
begin
  if y < 0 then
    begin
      if x = 0 then
        begin
          SetReturnAddress (ReturnAddress (0));
          RuntimeError (703); { Executed `x pow y' when x is zero and y < 0 }
          RestoreReturnAddress
        end;
      x := 1 / x;
      y := - y
    end;
  r := 1;
  while y <> 0 do
    begin
      if Odd (y) then r := r * x;
      y := y shr 1;
      if y <> 0 then x := Sqr (x)
    end
end;

function Complex_Abs (z : Complex) : Double;
begin
  Complex_Abs := Sqrt (Sqr (Re (z)) + Sqr (Im (z)))
end;

function Complex_Arg (z : Complex) : Double;
begin
  if (Re (z) = 0) and (Im (z) = 0) then
    begin
      SetReturnAddress (ReturnAddress (0));
      RuntimeError (704); { Cannot take `Arg' of zero }
      RestoreReturnAddress
    end;
  Complex_Arg := Arctan2 (Im (z), Re (z))
end;

function Complex_Polar (Length, Theta : Double) : Complex;
begin
  Complex_Polar := Cmplx (Length * cos (Theta), Length * sin (Theta))
end;

function Complex_Arctan (z : Complex) : Complex;
var
  a, b, d : Double;
  t : Complex;
begin
  {
    From Extended Pascal standard:

     Arctan (z) = - i / 2 * ln ((1 + i * z) / (1 - i * z))

    From pen and paper :-)

     Arctan (x + yi) = - i / 2 * ln (((1 - y) b - a) / d + i * (2 x / d))

    where a = x^2
      and b = 1 + y
      and d = b^2 + x^2
      and x = Re (z)
      and y = Im (z)
  }
  a := Sqr (Re (z));
  b := 1 + Im (z);
  d := Sqr (b) + a;
  t := Ln (Cmplx (((1 - Im (z)) * b - a) / d, 2 * Re (z) / d));
  Complex_Arctan := Cmplx (Im (t) / 2, - Re (t) / 2)
end;

function Complex_Sqrt (z : Complex) : Complex;
var
  a : Double;
  Sign : Integer;
begin
  {
    sqrt (x + yi) = +/- (sqrt ((a + x) / 2) +/- i * sqrt ((a - x) / 2))

    where
      a = Abs (x + yi)

    Principal value defined in the Extended Pascal standard: exp (0.5 * ln (z)),
    i.e. Sign (Re (sqrt (z)) := 1, Sign (Im (sqrt (z))) := Sign (Im (z))
  }
  a := Abs (z);
  if Im (z) < 0 then Sign := -1 else Sign := 1;
  if a <> 0
    then Complex_Sqrt := Cmplx (sqrt ((a + Re (z)) / 2), Sign * sqrt ((a - Re (z)) / 2))
    else Complex_Sqrt := 0
end;

function Complex_Ln (z : Complex) : Complex;
begin
  Complex_Ln := Cmplx (Ln (Abs (z)), Arctan2 (Im (z), Re (z)))
end;

function Complex_Exp (z : Complex) : Complex;
var ex : Double;
begin
  ex := exp (Re (z));
  Complex_Exp := Cmplx (ex * cos (Im (z)), ex * sin (Im (z)))
end;

function Complex_Sin (z : Complex) : Complex;
begin
  Complex_Sin := Cmplx (sin (Re (z)) * CosH (Im (z)), cos (Re (z)) * SinH (Im (z)))
end;

function Complex_Cos (z : Complex) : Complex;
begin
  Complex_Cos := Cmplx (cos (Re (z)) * CosH (Im (z)), - sin (Re (z)) * SinH (Im (z)))
end;

function Complex_Pow (z : Complex; y : Integer) = r : Complex;
var a : Double;
begin
  if y < 0 then
    begin
      a := Abs (z);
      if a = 0 then
        begin
          SetReturnAddress (ReturnAddress (0));
          RuntimeError (706); { Executed `x pow y' when complex x is zero and y < 0 }
          RestoreReturnAddress
        end;
      z := Cmplx (Re (z) / a, - Im (z) / a); { z := 1 / z }
      y := - y
    end;
  r := 1;
  while y <> 0 do
    begin
      if Odd (y) then r := r * z;
      y := y shr 1;
      if y <> 0 then z := Sqr (z)
    end
end;

function Complex_Power (z : Complex; y : Double) : Complex;
begin
  Complex_Power := Exp (y * Ln (z))
end;

end.
