{
Random number generator

Copyright (C) 1997-2000 Free Software Foundation, Inc.

Authors: Frank Heckenbach <frank@pascal.gnu.de>
         Toby Ewing <ewing@iastate.edu>

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

unit Random;

interface

uses Internal, Error, Time;

type
  RandomSeedType = Cardinal (32);
  RandomizeType  = ^procedure;
  SeedRandomType = ^procedure (Seed : RandomSeedType);
  RandRealType   = ^function : LongestReal;
  RandIntType    = ^function (Max : LongestCard) : LongestCard;

{ RandomizePtr, SeedRandomPtr, RandRealPtr and RandIntPtr point to these routines by default }
procedure Default_Randomize;                                 asmname '_p_default_randomize';
procedure Default_SeedRandom (Seed : RandomSeedType);        asmname '_p_default_seedrandom';
function  Default_RandReal : LongestReal;                    asmname '_p_default_randreal';
function  Default_RandInt (Max : LongestCard) : LongestCard; asmname '_p_default_randint';

{ GPC_Randomize, SeedRandom, GPC_RandReal and GPC_RandInt call the actual routines through RandomizePtr, RandRealPtr and RandIntPtr }
procedure GPC_Randomize;                                 asmname '_p_randomize';
procedure SeedRandom (Seed : RandomSeedType);            asmname '_p_seedrandom';
function  GPC_RandReal : LongestReal;                    asmname '_p_randreal';
function  GPC_RandInt (Max : LongestCard) : LongestCard; asmname '_p_randint';

var
  RandomizePtr  : RandomizeType = @Default_Randomize; asmname '_p_randomize_ptr';
  SeedRandomPtr : SeedRandomType = @Default_SeedRandom; asmname '_p_seedrandom_ptr';
  RandRealPtr   : RandRealType = @Default_RandReal; asmname '_p_randreal_ptr';
  RandIntPtr    : RandIntType = @Default_RandInt; asmname '_p_randint_ptr';

implementation

{$B-,I-}

{
Random number generator from ACM Transactions on Modeling and
Computer Simulation 8(1) 3-30, 1990. Supposedly it has a period of
-1 + 2^19937. The original was in C; this translation returns the
same values as the original. It is called the Mersenne Twister.

The following code was written by Toby Ewing <ewing@iastate.edu> and
slightly modified by Frank Heckenbach <frank@pascal.gnu.de>. It was
inspired by C code written by Makoto Matsumoto
<matumoto@math.keio.ac.jp> and Takuji Nishimura, considering the
suggestions by Topher Cooper and Marc Rieffel in July-August 1997.

GetRandom generates one pseudorandom Integer number which is
uniformly distributed in its range, for each call. SeedRandom (Seed)
sets initial values to the working area of 624 words. Before
GetRandom, SeedRandom must be called once. (The Seed is any 32-bit
Integer except for 0.)
}

type
  RandomStateType = Cardinal (64);
  RandomType = Cardinal (32);

const
  RandomRange = High (RandomType) + 1;
  N = 624; { Period parameters }
  M = 397;
  DefaultSeed = 4357;

var
  mti : Integer;
  mt : array [0 .. N] of RandomStateType; { the array for the state vector }
  RandomInitialized : Boolean = False;

procedure Default_SeedRandom (Seed : RandomSeedType);
{ Set initial seeds to mt [N] using the generator Line 25 of Table 1 in
  [KNUTH 1981, The Art of Computer Programming Vol. 2 (2nd Ed.), pp 102]. }
begin
  if Seed = 0 then Seed := DefaultSeed;
  mt [0] := Seed and $ffffffff;
  for mti := 1 to N do mt [mti] := (69069 * mt [mti - 1]) and $ffffffff;
  mti := n;
  RandomInitialized := True
end;

procedure Default_Randomize;
var
  Seed : RandomSeedType = 0;
  f : file of RandomSeedType;
  b : BindingType;
begin
  Assign (f, '/dev/urandom');
  b := Binding (f);
  if not (b.Bound and b.Special) then
    begin
      Assign (f, '/dev/random');
      b := Binding (f)
    end;
  if b.Bound and b.Special then
    begin
      Reset (f);
      Read (f, Seed);
      Close (f)
    end;
  if (IOResult <> 0) or (Seed = 0) then
    begin
      Seed := GetUnixTime (null);
      if Seed <= 0 then Seed := ProcessID
    end;
  Default_SeedRandom (Seed)
end;

function GetRandom : RandomType;
const
  Matrix_A = $9908b0df; { constant vector a }
  T_Mask_B = $9d2c5680; { Tempering parameters }
  T_Mask_C = $efc60000;
  Up_Mask  = $80000000; { most significant w-r bits }
  Low_Mask = $7fffffff; { least significant r bits }
  mag01 : array [0 .. 1] of RandomStateType = (0, Matrix_A);
var
  y : RandomStateType;
  kk : Integer;
begin
  if not RandomInitialized then Default_Randomize;
  if mti >= N then { generate N words at one time }
    begin
      for kk := 0 to N - M do
        begin
          y := (mt [kk] and Up_Mask) or (mt [kk + 1] and Low_Mask);
          mt [kk] := mt [kk + M] xor (y shr 1) xor mag01 [y and 1]
        end;
      for kk := N - M to N - 1 do
        begin
          y := (mt [kk] and Up_Mask) or (mt [kk + 1] and Low_Mask);
          mt [kk] := mt [kk + M - N] xor (y shr 1) xor mag01 [y and 1]
        end;
      y := (mt [N - 1] and Up_Mask) or (mt [0] and Low_Mask);
      mt [N - 1] := mt [M - 1] xor (y shr 1) xor mag01 [y and 1];
      mti := 0
    end;
  y := mt [mti];
  Inc (mti);
  y := y xor (y shr 11);
  y := y xor ((y shl 7) and T_Mask_B);
  y := y xor ((y shl 15) and T_Mask_C);
  y := y xor (y shr 18);
  GetRandom := y
end;

function Default_RandReal : LongestReal;
var y : RandomType;
begin
  y := GetRandom;
  Default_RandReal := LongestReal (y) / RandomRange
end;

function Default_RandInt (Max : LongestCard) = s : LongestCard;
var f, m, r, a, b : LongestCard;
begin
  repeat
    m := Max;
    s := 0;
    f := 1;
    while m > 1 do
      begin
        if m >= RandomRange then
          r := GetRandom
        else
          begin
            a := RandomRange - RandomRange mod m;
            repeat
              b := GetRandom
            until b < a;
            r := b mod m
          end;
        Inc (s, r * f);
        f := f * RandomRange;
        m := (m - 1) div RandomRange + 1
      end
  until (s < Max) or (Max = 0)
end;

procedure GPC_Randomize;
begin
  RandomizePtr^
end;

procedure SeedRandom (Seed : RandomSeedType);
begin
  SeedRandomPtr^ (Seed)
end;

function GPC_RandReal : LongestReal;
begin
  GPC_RandReal := RandRealPtr^
end;

function GPC_RandInt (Max : LongestCard) : LongestCard;
begin
  GPC_RandInt := RandIntPtr^ (Max)
end;

end.
