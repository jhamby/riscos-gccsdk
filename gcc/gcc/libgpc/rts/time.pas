{
Time and date routines

Copyright (C) 1991-2000 Free Software Foundation, Inc.

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

unit Time;

interface

uses Error;

const { from types.h }
  DateLength = 11;
  TimeLength = 8;

type
  UnixTimeType = LongInt; { This is hard-coded in the compiler. Do not change here. }
  MicroSecondTimeType = LongInt;

  DateString = packed array [1 .. DateLength] of Char;
  TimeString = packed array [1 .. TimeLength] of Char;

  GPC_TimeStamp = {@@packed} record
    DateValid,
    TimeValid   : Boolean;
    Year        : Integer;
    Month       : 1 .. 12;
    Day         : 1 .. 31;
    DayOfWeek   : 0 .. 6;  { 0 means Sunday }
    Hour        : 0 .. 23;
    Minute      : 0 .. 59;
    Second      : 0 .. 61;
    MicroSecond : 0 .. 999999
  end;

const
  MonthName : array [1 .. 12] of String [9] =
    ('January', 'February', 'March', 'April', 'May', 'June',
     'July', 'August', 'September', 'October', 'November', 'December'); asmname '_p_monthname';

  MonthLength : array [1 .. 12] of Integer =
    (31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31); asmname '_p_monthlength';

function  GetDayOfWeek (Day, Month, Year : Integer) : Integer;                                            asmname '_p_dayofweek';
procedure UnixTimeToTimeStamp (UnixTime : UnixTimeType; var aTimeStamp : TimeStamp);                      asmname '_p_unix_time_to_time_stamp';
function  TimeStampToUnixTime (protected var aTimeStamp : TimeStamp) : UnixTimeType;                      asmname '_p_time_stamp_to_unix_time';
function  GetMicroSecondTime : MicroSecondTimeType;                                                       asmname '_p_get_micro_second_time';

{ Is the year a leap year? }
function  IsLeapYear (Year : Integer) : Boolean;                                                          asmname '_p_is_leap_year';

procedure Sleep (Seconds : Integer);                                                                      asmname '_p_sleep';
procedure UnixTimeToTime (UnixTime : UnixTimeType; var Year, Month, Day, Hour, Minute, Second : Integer); asmname '_p_unix_time_to_time';
function  TimeToUnixTime (Year, Month, Day, Hour, Minute, Second : Integer) : UnixTimeType;               asmname '_p_time_to_unix_time';

{ Get the real time. MicroSecond can be null and is ignored then. }
function  GetUnixTime (var MicroSecond : Integer) : UnixTimeType;                                         asmname '_p_get_unix_time';

{ Get the CPU time used. MicroSecond can be null and is ignored then.
  Now, GetCPUTime can measure long CPU times reliably on most systems
  (e.g. Solaris where it didn't work before). }
function  GetCPUTime (var MicroSecond : Integer) : Integer;                                               asmname '_p_get_cpu_time';

procedure GPC_GetTimeStamp   (var aTimeStamp : TimeStamp);                                                asmname '_p_gettimestamp';
procedure GPC_Date (protected var aTimeStamp : TimeStamp; var Result : DateString);                       asmname '_p_date';
procedure GPC_Time (protected var aTimeStamp : TimeStamp; var Result : TimeString);                       asmname '_p_time';

implementation

{$B-,I-}

function GetDayOfWeek (Day, Month, Year : Integer) : Integer;
const m : array [1 .. 12] of Integer = (0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4);
begin
  if Month <= 2 then Dec (Year);
  GetDayOfWeek := (Day + m [Month] + Year + Year div 4 - Year div 100 + Year div 400) mod 7
end;

function IsLeapYear (Year : Integer) : Boolean;
begin
  IsLeapYear := (Year mod 4 = 0) and ((Year mod 100 <> 0) or (Year mod 400 = 0))
end;

procedure GPC_Date (protected var aTimeStamp : TimeStamp; var Result : DateString);
begin
  with aTimeStamp do
    if not Datevalid
       or (Month < Low (Month)) or (Month > High (Month))
       or (Day < Low (Day)) or (Day > MonthLength [Month] + Ord ((Month = 2) and IsLeapYear (Year))) then
      begin
        SetReturnAddress (ReturnAddress (0));
        RuntimeError (750); { Invalid date supplied to library function `Date' }
        RestoreReturnAddress
      end
    else
      WriteStr (Result, Day : 2, ' ', Copy (MonthName [Month], 1, 3), ' ', Year : 4)
end;

procedure GPC_Time (protected var aTimeStamp : TimeStamp; var Result : TimeString);
const Zero : array [Boolean] of String (1) = ('', '0');
begin
  with aTimeStamp do
    if not TimeValid
       or (Hour < Low (Hour)) or (Hour > High (Hour))
       or (Minute < Low (Minute)) or (Minute > High (Minute))
       or (Second < Low (Second)) or (Second > High (Second)) then
      begin
        SetReturnAddress (ReturnAddress (0));
        RuntimeError (751); { Invalid time supplied to library function `Time' }
        RestoreReturnAddress
      end
    else
      WriteStr (Result, Zero [Hour   < 10], Hour,   ':',
                        Zero [Minute < 10], Minute, ':',
                        Zero [Second < 10], Second)
end;

procedure UnixTimeToTimeStamp (UnixTime : UnixTimeType; var aTimeStamp : TimeStamp);
var aYear, aMonth, aDay, aHour, aMinute, aSecond : Integer;
begin
  with aTimeStamp do
    if UnixTime >= 0 then
      begin
        UnixTimeToTime (UnixTime, aYear, aMonth, aDay, aHour, aMinute, aSecond);
        Year        := aYear;
        Month       := aMonth;
        Day         := aDay;
        DayOfWeek   := GetDayOfWeek (aDay, aMonth, aYear);
        DateValid   := True;
        Hour        := aHour;
        Minute      := aMinute;
        Second      := aSecond;
        MicroSecond := 0;
        TimeValid   := True
      end
    else
      begin
        { The values are specified in the standard, even if the Valid fields are False }
        Year        := 1;
        Month       := 1;
        Day         := 1;
        DayOfWeek   := 0;
        DateValid   := False;
        Hour        := 0;
        Minute      := 0;
        Second      := 0;
        MicroSecond := 0;
        TimeValid   := False
      end
end;

function TimeStampToUnixTime (protected var aTimeStamp : TimeStamp) : UnixTimeType;
begin
  with aTimeStamp do
  if not DateValid or not TimeValid
    then TimeStampToUnixTime := - 1
    else TimeStampToUnixTime := TimeToUnixTime (Year, Month, Day, Hour, Minute, Second)
end;

procedure GPC_GetTimeStamp (var aTimeStamp : TimeStamp);
var MicroSecond : Integer;
begin
  UnixTimeToTimeStamp (GetUnixTime (MicroSecond), aTimeStamp);
  aTimeStamp.MicroSecond := MicroSecond
end;

function GetMicroSecondTime : MicroSecondTimeType;
var
  MicroSecond : Integer;
  UnixTime : UnixTimeType;
begin
  UnixTime := GetUnixTime (MicroSecond);
  GetMicroSecondTime := 1000000 * UnixTime + MicroSecond
end;

end.
