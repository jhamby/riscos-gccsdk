{
Integer and real to decimal conversion routines.
Only to be used internally by the RTS.

Copyright (C) 1997-2000 Free Software Foundation, Inc.

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
}

unit NumToDec;

interface

function  IsInfinity   (x : Extended) : Boolean; attribute (const); asmname '_p_isinf';
function  IsNotANumber (x : Extended) : Boolean; attribute (const); asmname '_p_isnan';
procedure SplitReal    (x : Extended; var Exponent : Integer; var Mantissa : Extended); asmname '_p_frexp';

{ This is an implementation-dependent constant according to
  ISO 10206 and can be defined while compiling the RTS. }
const RealDefaultDigits = 15;

{ Sufficient width to hold a LongInt in decimal representation }
const MaxLongIntWidth = BitSizeOf (LongInt) div 3 + 2;

{ @@ A variable for a constant value is not the most efficient thing, but
  the only way to communicate the constant to C code (as C does not know
  `Low'). In the future, the C code will simply be rewritten in Pascal,
  and the variable can be eliminated then. }
var
  Low_Integer : Integer = Low (Integer); asmname '_p_low_integer';

function CardToDecimal     (x : Cardinal; BufEnd : PChar) : PChar; asmname '_p_card_to_decimal';
function LongCardToDecimal (x : LongCard; BufEnd : PChar) : PChar; asmname '_p_longcard_to_decimal';
function LongRealToDecimal (x : LongReal; Width, Precision : Integer; WidthGiven, Blank, CapitalExp : Boolean; var BufSize : Integer) : PChar; asmname '_p_longreal_to_decimal';

implementation

{$B-,I-,X+}

function CardToDecimal (x : Cardinal; BufEnd : PChar) : PChar;
begin
  repeat
    Dec (BufEnd);
    BufEnd^ := Succ ('0', x mod 10);
    x := x div 10
  until x = 0;
  CardToDecimal := BufEnd
end;

function LongCardToDecimal (x : LongCard; BufEnd : PChar) : PChar;
begin
  repeat
    Dec (BufEnd);
    BufEnd^ := Succ ('0', x mod 10);
    x := x div 10
  until x = 0;
  LongCardToDecimal := BufEnd
end;

function LongRealToDecimal (x : LongReal; Width, Precision: Integer; WidthGiven, Blank, CapitalExp : Boolean; var BufSize : Integer) : PChar;
const ExpBufSize = MaxLongIntWidth + 3;
var
  FirstChar, ExpSign: Char;
  CharFirst, Count, Exp, AddExp, ExpLength, DigitsLeft, DigitsRight : Integer;
  v, DigitVal : LongReal;
  Buf, BufPos, TempPos, ExpBufEnd, ExpBufPos : PChar;
  ExpBuf: array [1 .. ExpBufSize] of Char;

  inline procedure BufPut (ch : Char);
  begin
    BufPos^ := ch;
    Inc (BufPos)
  end;

  inline procedure ExpPut (ch : Char);
  begin
    Dec (ExpBufPos);
    ExpBufPos^ := ch
  end;

  inline function GetDigit : Char;
  var digit : Integer;
  begin
    digit := Trunc (v / DigitVal);
    v := v - digit * DigitVal;
    DigitVal := DigitVal / 10;
    GetDigit := Succ ('0', digit)
  end;

begin
  BufSize := 0;
  if IsNotANumber (x) then
    begin
      if (Precision < 0) and Blank then
        LongRealToDecimal := ' NaN'
      else
        LongRealToDecimal := 'NaN';
      Exit
    end;
  if IsInfinity (x) then
    begin
      if x < 0 then
        LongRealToDecimal := '-Inf'
      else if (Precision < 0) and Blank then
        LongRealToDecimal := ' Inf'
      else
        LongRealToDecimal := 'Inf';
      Exit
    end;

  { Preparations for possible second try; see below }
  AddExp := 0;
  repeat
    v:=x;

    { Check if there must be a leading minus sign or blank }
    if v < 0 then
      begin
        FirstChar := '-';
        CharFirst := 1;
        v := -v
      end
    { No blank in front of positive Reals with precision given.
      ISO Standard is *very* consistent. }
    else
      begin
        FirstChar := ' ';
        if (Precision < 0) and Blank then CharFirst := 1 else CharFirst := 0
      end;

    { Find the exponent }
    Exp := 0;
    DigitVal := 1;
    if (Precision < 0) and (v <> 0) then
      while v < DigitVal do
        begin
          DigitVal := DigitVal / 10;
          Dec (Exp)
        end;
    while v / DigitVal >= 10 do
      begin
        DigitVal := DigitVal * 10;
        Inc (Exp)
      end;

    { Second try: exponent must be bigger; see below }
    if AddExp = 1 then
      begin
        DigitVal := DigitVal * 10;
        Inc (Exp)
      end;

    { Output the exponent to temporary buffer }
    if Precision < 0 then
      begin
        ExpBufEnd := @ExpBuf [ExpBufSize];
        if Exp >= 0 then
          ExpSign := '+'
        else
          begin
            ExpSign := '-';
            Exp := -Exp
          end;
        ExpBufPos := CardToDecimal (Exp, ExpBufEnd);
        { Print at least 2 digits of the exponent because that's "usual" }
        if Exp < 10 then ExpPut ('0');
        ExpPut (ExpSign);
        if CapitalExp then ExpPut ('E') else ExpPut ('e');
        ExpLength := ExpBufEnd - ExpBufPos
      end
    else
      ExpLength := 0;

    { Find number of digits to print before and after the (floating) point }
    if Precision < 0 then
      begin
        DigitsLeft := 1;
        if not WidthGiven then
          DigitsRight := RealDefaultDigits
        else
          DigitsRight := Max (1, Width - (CharFirst + DigitsLeft + 1 + ExpLength))
      end
    else
      begin
        DigitsLeft := Exp + 1;
        DigitsRight := Precision
      end;

    { Allocate the buffer }
    BufSize := CharFirst + DigitsLeft + 1 + DigitsRight + ExpLength + 1;
    GetMem (Buf, BufSize);
    BufPos := Buf;
    if CharFirst > 0 then BufPut (FirstChar);

    { Output the mantissa }
    for Count := 1 to DigitsLeft do BufPut (GetDigit);
    if DigitsRight > 0 then
      begin
        BufPut ('.');
        for Count := 1 to DigitsRight do BufPut (GetDigit)
      end;

    { Rounding }
    AddExp := 0;
    if v / DigitVal >= 5 then
      begin
        TempPos := Pred (BufPos);
        Count := DigitsLeft + DigitsRight;
        while (Count > 0) and (TempPos^ = '9') do
          begin
            TempPos^ := '0';
            Dec (TempPos);
            Dec (Count);
            if Count = DigitsLeft then Dec (TempPos)
          end;
        if Count > 0 then
          Inc (TempPos^)
        else
          begin
            { If the mantissa was 999.999, and the number must be rounded up,
              we have to start all over with a bigger exponent }
            Dispose (Buf);
            AddExp := 1
          end
      end
  until AddExp = 0;

  { Copy the exponent to the real buffer }
  for Count := 1 to ExpLength do
    begin
      BufPut (ExpBufPos^);
      Inc (ExpBufPos)
    end;

  { Terminate the buffer and return a pointer to it }
  BufPut (#0);
  LongRealToDecimal := Buf
end;

end.
