{
BP compatible Strings unit

Copyright (C) 1999-2000 Free Software Foundation, Inc.

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

{$gnu-pascal}

unit Strings;

interface

uses GPC;

function  StrLen     (Src : CString) : SizeType;                            asmname '_p_strlen';
function  StrEnd     (Src : CString) : CString;                             asmname '_p_strend';
function  StrMove    (Dest, Source : CString; Count : SizeType) : CString;  asmname '_p_strmove';
function  StrCopy    (Dest, Source : CString) : CString;                    asmname '_p_strcpy';
function  StrECopy   (Dest, Source : CString) : CString;                    asmname '_p_strecpy';
function  StrLCopy   (Dest, Source : CString; MaxLen : SizeType) : CString; asmname '_p_strlcpy';
function  StrPCopy   (Dest : CString; const Source : String) : CString;     asmname '_p_cstringcopystring';
function  StrCat     (Dest, Source : CString) : CString;                    asmname '_p_strcat';
function  StrLCat    (Dest, Source : CString; MaxLen : SizeType) : CString; asmname '_p_strlcat';
function  StrComp    (s1, s2 : CString) : Integer;                          asmname '_p_strcmp';
function  StrIComp   (s1, s2 : CString) : Integer;                          asmname '_p_strcasecmp';
function  StrLComp   (s1, s2 : CString; MaxLen : SizeType) : Integer;       asmname '_p_strlcmp';
function  StrLIComp  (s1, s2 : CString; MaxLen : SizeType) : Integer;       asmname '_p_strlcasecmp';
function  StrScan    (Src : CString; Ch : Char) : CString;                  asmname '_p_strscan';
function  StrRScan   (Src : CString; Ch : Char) : CString;                  asmname '_p_strrscan';
function  StrPos     (aString, SubString : CString) : CString;              asmname '_p_strpos';
function  StrRPos    (aString, SubString : CString) : CString;              asmname '_p_strrpos';
function  StrUpper   (s : CString) : CString;                               asmname '_p_strupper';
function  StrLower   (s : CString) : CString;                               asmname '_p_strlower';
function  StrPas     (aString : CString) : TString;                         asmname '_p_strpas';
function  StrEmpty   (s : CString) : Boolean;                               asmname '_p_strempty';
function  StrNew     (Src : CString) : CString;                             asmname '_p_strdup';
procedure StrDispose (s : CString);                                         asmname '_p_dispose';

implementation

function StrPas (aString : CString) : TString;
begin
  StrPas := CString2String (aString)
end;

end.
