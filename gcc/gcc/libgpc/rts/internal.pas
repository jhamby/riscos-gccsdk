{
Internal Pascal declarations of the GPC run time system that are not
meant to be used in normal programs. This file is a supplement to
gpc.pas. Use of the routines in this file from user programs is at
your own risk. It might work only under certain circumstances, and
the declarations might change without notice.

Note about the `GPC_' prefix:
This is inserted so that some identifiers don't conflict with the
built-in ones. Sometimes, the built-in ones do exactly the same as
the ones declared here, but often enough, they contain some "magic",
so they should be used instead of the plain declarations here.
In general, routines with a `GPC_' prefix should not be called from
programs. They may change or disappear in future GPC versions.

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
}

{$if __GPC_RELEASE__ < 20000412}
{$error
Trying to compile the RTS with a non-current GPC version is likely
to cause problems. If you are building the RTS separately from GPC,
make sure you install a current GPC version previously. If you are
building GPC now and this message appears, something is wrong --
if you are overriding the GCC_FOR_TARGET or GPC_FOR_TARGET make
variables, this might be the problem. If you are cross-building GPC,
build and install a current GPC cross-compiler first. Sorry.}
{$endif}

unit Internal;

interface

const
  { Maximum size of a variable }
  MaxVarSize = MaxInt;

type
  GPC_TChars = packed array [1 .. 1] of Char;
  GPC_PChars = ^GPC_TChars;
  GPC_PCStrings = ^GPC_TCStrings;
  GPC_TCStrings = array [0 .. MaxVarSize div SizeOf (CString)] of CString;
  GPC_UnixTimeType = LongInt; { This is hard-coded in the compiler. Do not change here. }
  GPC_MicroSecondTimeType = LongInt;
  GPC_FileSizeType = LongInt;

type
  AnyFile = Text; (*@@ create `AnyFile' parameters*)
  PAnyFile = ^AnyFile;

{ RTS initialization, called before entering the main Pascal program, from init.pas }

procedure GPC_Initialize (ArgumentCount : Integer; Arguments, StartEnvironment : GPC_PCStrings); asmname '_p_initialize';

function GPC_GetStartEnvironment (ValueIfNotFound : GPC_PCStrings) : GPC_PCStrings; asmname '_p_get_start_environment';

procedure GPC_CheckInOutRes; asmname '_p_check_inoutres';

(*@@IO critical*) procedure GPC_Initialize_Std_Files; asmname '_p_initialize_std_files';
procedure GPC_Init_Misc;            asmname '_p_init_misc';
procedure GPC_Init_Time;            asmname '_p_init_time';

procedure GPC_Init_Malloc (procedure WarnProc (s : CString)); asmname '_p_init_malloc';

{ Automatically called at the end of the program }
procedure GPC_Done_Files; asmname '_p_done_files';

type
  PFileAssociation = ^TFileAssociation;
  TFileAssociation = record
    Next : PFileAssociation;
    IntName, ExtName : CString
  end;

procedure FlushAllFiles (OnlyTTYs : Boolean); asmname '_p_fflush';
procedure ExitProgram (Status : Integer; AbortFlag : Boolean); attribute (noreturn); asmname '_p_exit';

{ Generic file handling routines and their support, from file.c }

type
  InternalIOSelectTypePtr = ^InternalIOSelectType;
  InternalIOSelectType = record
    fi : PAnyFile;
    WantedReadOrEOF,
    WantedRead,
    WantedEOF,
    WantedWrite,
    WantedException,
    WantedAlways,
    OccurredReadOrEOF,
    OccurredRead,
    OccurredEOF,
    OccurredWrite,
    OccurredException : Boolean
  end;

function  FileName_CString (protected var aFile : AnyFile) : CString; asmname '_p_filename';

function  InternalIOSelect (Events : InternalIOSelectTypePtr; EventsLow, Count : Integer; MicroSeconds : GPC_MicroSecondTimeType) : Integer; asmname '_p_select';
procedure GPC_InitFDR (var aFile : AnyFile; Name : CString; Size, Flags : Integer); asmname '_p_initfdr';
procedure GPC_DoneFDR (var aFile : AnyFile);                                        asmname '_p_donefdr';

var
  HaveFNMatch : Integer; asmname '_p_have_fnmatch'; external;

function  FNMatch (Pattern, Name : CString) : Integer; asmname '_p_fnmatch';
function  COpenDir (Name : CString) : Pointer; asmname '_p_opendir_c';
function  CReadDir (Dir : Pointer) : CString; asmname '_p_readdir_c';

{ The result of the following function is a pointer to a *static* buffer! }
function  RealPath (Path : CString) : CString; asmname '_p_realpath';

{ The result of the following function is a pointer to a *static* buffer! }
function  GetInternalFileName (protected var f : AnyFile) : CString; asmname '_p_get_file_name';

function  CStringExecutablePath (Buffer : CString) : CString; asmname '_p_c_executable_path';

function  ProcessID : Integer; asmname '_p_pid';

function  Kill (PID, Signal : Integer) : Boolean; asmname '_p_kill';

type
  TSignalHandler = procedure (Signal : Integer);

{ OldHandler and OldRestart may be null }
function InstallSignalHandler (Signal : Integer; Handler : TSignalHandler;
                               Restart, UnlessIgnored : Boolean;
                               var OldHandler : TSignalHandler; var OldRestart : Boolean) : Boolean; asmname '_p_sigaction';

{ Routines to read various things from files, from read.c }

procedure GPC_LazyTryGet (var aFile : AnyFile);                    asmname '_p_lazytryget';
procedure GPC_LazyGet    (var aFile : AnyFile);                    asmname '_p_lazyget';
procedure GPC_LazyUnget  (var aFile : AnyFile);                    asmname '_p_lazyunget';

{ If an error occurs or remaining characters (including whitespace!) are left,
  the position of the error, not the error code, is returned (BP compatibility!) }
function GPC_Val_ByteInt_NoCheck   (Source : GPC_PChars; StrLength, Flags : Integer; var Dest : ByteInt  ) : Integer; asmname '_p_val_byteint_nocheck';
function GPC_Val_ShortInt_NoCheck  (Source : GPC_PChars; StrLength, Flags : Integer; var Dest : ShortInt ) : Integer; asmname '_p_val_shortint_nocheck';
function GPC_Val_Integer_NoCheck   (Source : GPC_PChars; StrLength, Flags : Integer; var Dest : Integer  ) : Integer; asmname '_p_val_integer_nocheck';
function GPC_Val_MedInt_NoCheck    (Source : GPC_PChars; StrLength, Flags : Integer; var Dest : MedInt   ) : Integer; asmname '_p_val_medint_nocheck';
function GPC_Val_LongInt_NoCheck   (Source : GPC_PChars; StrLength, Flags : Integer; var Dest : LongInt  ) : Integer; asmname '_p_val_longint_nocheck';
function GPC_Val_ByteCard_NoCheck  (Source : GPC_PChars; StrLength, Flags : Integer; var Dest : ByteCard ) : Integer; asmname '_p_val_bytecard_nocheck';
function GPC_Val_ShortCard_NoCheck (Source : GPC_PChars; StrLength, Flags : Integer; var Dest : ShortCard) : Integer; asmname '_p_val_shortcard_nocheck';
function GPC_Val_Cardinal_NoCheck  (Source : GPC_PChars; StrLength, Flags : Integer; var Dest : Cardinal ) : Integer; asmname '_p_val_cardinal_nocheck';
function GPC_Val_MedCard_NoCheck   (Source : GPC_PChars; StrLength, Flags : Integer; var Dest : MedCard  ) : Integer; asmname '_p_val_medcard_nocheck';
function GPC_Val_LongCard_NoCheck  (Source : GPC_PChars; StrLength, Flags : Integer; var Dest : LongCard ) : Integer; asmname '_p_val_longcard_nocheck';
{$if 0} { Not yet built-in }
function GPC_Val_ByteInt_Check     (Source : GPC_PChars; StrLength, Flags : Integer; var Dest : ByteInt;   MinRange, MaxRange : ByteInt  ) : Integer; asmname '_p_val_byteint_check';
function GPC_Val_ShortInt_Check    (Source : GPC_PChars; StrLength, Flags : Integer; var Dest : ShortInt;  MinRange, MaxRange : ShortInt ) : Integer; asmname '_p_val_shortint_check';
function GPC_Val_Integer_Check     (Source : GPC_PChars; StrLength, Flags : Integer; var Dest : Integer;   MinRange, MaxRange : Integer  ) : Integer; asmname '_p_val_integer_check';
function GPC_Val_MedInt_Check      (Source : GPC_PChars; StrLength, Flags : Integer; var Dest : MedInt;    MinRange, MaxRange : MedInt   ) : Integer; asmname '_p_val_medint_check';
function GPC_Val_LongInt_Check     (Source : GPC_PChars; StrLength, Flags : Integer; var Dest : LongInt;   MinRange, MaxRange : LongInt  ) : Integer; asmname '_p_val_longint_check';
function GPC_Val_ByteCard_Check    (Source : GPC_PChars; StrLength, Flags : Integer; var Dest : ByteCard;  MinRange, MaxRange : ByteCard ) : Integer; asmname '_p_val_bytecard_check';
function GPC_Val_ShortCard_Check   (Source : GPC_PChars; StrLength, Flags : Integer; var Dest : ShortCard; MinRange, MaxRange : ShortCard) : Integer; asmname '_p_val_shortcard_check';
function GPC_Val_Cardinal_Check    (Source : GPC_PChars; StrLength, Flags : Integer; var Dest : Cardinal;  MinRange, MaxRange : Cardinal ) : Integer; asmname '_p_val_cardinal_check';
function GPC_Val_MedCard_Check     (Source : GPC_PChars; StrLength, Flags : Integer; var Dest : MedCard;   MinRange, MaxRange : MedCard  ) : Integer; asmname '_p_val_medcard_check';
function GPC_Val_LongCard_Check    (Source : GPC_PChars; StrLength, Flags : Integer; var Dest : LongCard;  MinRange, MaxRange : LongCard ) : Integer; asmname '_p_val_longcard_check';
{$endif}
function GPC_Val_ShortReal         (Source : GPC_PChars; StrLength, Flags : Integer; var Dest : ShortReal) : Integer; asmname '_p_val_shortreal';
function GPC_Val_Real              (Source : GPC_PChars; StrLength, Flags : Integer; var Dest : Real     ) : Integer; asmname '_p_val_real';
function GPC_Val_LongReal          (Source : GPC_PChars; StrLength, Flags : Integer; var Dest : LongReal ) : Integer; asmname '_p_val_longreal';

{ Temporary work-around (IRIX, see gpc.pas) }
procedure GPC_WriteStr_Real (s1 : GPC_PChars; var long_curlen : Integer; Capacity : Integer;
  Num : LongReal; Width, Prec : Integer); asmname '_p_writestr_one_real';

{ Internal declarations for built-in functionality }

{ Returns a description for a signal }
function CStringStrSignal (Signal : Integer) : CString; asmname '_p_c_strsignal';

const { from types.h }
  Binding_Name_Length = 255;

type
  { The standard fields are Bound and Name. The others are extensions. }
  GPC_BindingType = {@@packed} record
    Bound             : Boolean;
    Force             : Boolean;      { Can be set to allow binding to
                                        directories or non-accessible files }
    Extensions_Valid  : Boolean;
    Readable          : Boolean;
    Writable          : Boolean;
    Executable        : Boolean;
    Existing          : Boolean;      { Binding points to an existing file }
    Directory         : Boolean;      { Binding points to an existing
                                        directory; Existing is False then }
    Special           : Boolean;      { Binding points to an existing special
                                        file (device, pipe, socket, etc.);
                                        `Existing' is False then }
    SymLink           : Boolean;      { Binding points to a symbolic link }
    AccessTime,                       { Time of last access }
    ModificationTime,                 { Time of last modification }
    ChangeTime        : GPC_UnixTimeType; { Time of last change }
    User,                             { User ID of owner }
    Group,                            { Group ID of owner }
    Mode,                             { Access permissions, cf. ChMod }
    Device,                           { Device the file is on }
    INode             : Integer;      { Unix INode number }
    TextBinary        : Boolean;      { Open a Text file in binary mode }
    Handle            : Integer;      { Can be set to bind a Pascal file to
                                        a given file handle }
    Name              : String (Binding_Name_Length)
  end;

procedure GPC_Bind         (var aFile : AnyFile; protected var aBinding : BindingType); asmname '_p_bind';
procedure GPC_Unbind       (var aFile : AnyFile);                                       asmname '_p_unbind';

var
  GPC_FileMode : Integer; asmname '_p_filemode'; external;

procedure GPC_Flush (var aFile : AnyFile);                                        asmname '_p_flush';

{ This is used to write error messages etc. We don't use the Pascal
  I/O system here because this is usually called at the very end of
  a program after the Pascal I/O system is shut down. }
function WriteErrorMessage (s : CString; StdErrFlag : Boolean) : Boolean; asmname '_p_write_error_message';

function PExecuteC (ProgramName : CString; ArgV: GPC_PCStrings; This_PName, Temp_Base : CString;
                    var ErrMsg_Fmt, ErrMsg_Arg : CString; Flags : Integer) : Integer; asmname 'pexecute';

{ Various other versions of Reset, Rewrite and Extend are still overloaded magically }
procedure GPC_Rewrite  (          var aFile : AnyFile; aFileName : CString; aLength : Integer); asmname '_p_rewrite';
procedure GPC_Extend   (          var aFile : AnyFile; aFileName : CString; aLength : Integer); asmname '_p_extend';
procedure GPC_Reset    (          var aFile : AnyFile; aFileName : CString; aLength : Integer); asmname '_p_reset';
procedure CloseFile    (          var aFile : AnyFile);                                        asmname '_p_close';
procedure GPC_Erase    (          var aFile : AnyFile);                                        asmname '_p_erase';
procedure GPC_Rename   (          var aFile : AnyFile; NewName : CString);                     asmname '_p_rename';
Procedure GPC_ChDir    (Path : CString);                                                       asmname '_p_chdir';
Procedure GPC_MkDir    (Path : CString);                                                       asmname '_p_mkdir';
Procedure GPC_RmDir    (Path : CString);                                                       asmname '_p_rmdir';
procedure GPC_Erase    (          var aFile : AnyFile);                                        asmname '_p_erase';
procedure GPC_Rename   (          var aFile : AnyFile; NewName : CString);                     asmname '_p_rename';
Procedure GPC_ChDir    (Path : CString);                                                       asmname '_p_chdir';
Procedure GPC_MkDir    (Path : CString);                                                       asmname '_p_mkdir';
Procedure GPC_RmDir    (Path : CString);                                                       asmname '_p_rmdir';

{ Random access file routines, from randfile.c }

function  GPC_GetSize      (          var aFile : AnyFile) : GPC_FileSizeType;  asmname '_p_getsize';
procedure GPC_Truncate     (          var aFile : AnyFile);                     asmname '_p_truncate';
procedure GPC_DefineSize   (          var aFile : AnyFile; NewSize  : GPC_FileSizeType); asmname '_p_definesize';
procedure GPC_SeekAll      (          var aFile : AnyFile; NewPlace : GPC_FileSizeType); asmname '_p_seekall';
procedure GPC_SeekRead     (          var aFile : AnyFile; NewPlace : GPC_FileSizeType); asmname '_p_seekread';
procedure GPC_SeekWrite    (          var aFile : AnyFile; NewPlace : GPC_FileSizeType); asmname '_p_seekwrite';
procedure GPC_SeekUpdate   (          var aFile : AnyFile; NewPlace : GPC_FileSizeType); asmname '_p_seekupdate';
function  GPC_Empty        (protected var aFile : AnyFile) : Boolean;           asmname '_p_empty';
procedure GPC_Update       (          var aFile : AnyFile);                     asmname '_p_update';
function  GPC_LastPosition (          var aFile : AnyFile) : GPC_FileSizeType;  asmname '_p_lastposition';
function  GPC_Position     (          var aFile : AnyFile) : GPC_FileSizeType;  asmname '_p_position';

{ Versions with only 3 parameters are still overloaded magically }
(*@@*)procedure GPC_BlockRead  (var aFile : File;           var Buf : Void; Count : Cardinal; var BytesRead    : Cardinal); asmname '_p_blockread';
(*@@*)procedure GPC_BlockWrite (var aFile : File; protected var Buf : Void; Count : Cardinal; var BytesWritten : Cardinal); asmname '_p_blockwrite';

{ Routines to output various things, from write.c }

procedure GPC_Page      (var aFile : Text);                       asmname '_p_page';
procedure GPC_Put       (var aFile : AnyFile);                    asmname '_p_put';

{ Transcendental functions for Reals and LongReals }

function Real_Sin        (x : Double)   : Double;   asmname '_p_sin';
function Real_Cos        (x : Double)   : Double;   asmname '_p_cos';
function Real_Arctan     (x : Double)   : Double;   asmname '_p_arctan';
function Real_Sqrt       (x : Double)   : Double;   asmname '_p_sqrt';
function Real_Ln         (x : Double)   : Double;   asmname '_p_ln';
function Real_Exp        (x : Double)   : Double;   asmname '_p_exp';
function LongReal_Arctan (x : LongReal) : LongReal; asmname '_p_l_arctan';
function LongReal_Sqrt   (x : LongReal) : LongReal; asmname '_p_l_sqrt';
function LongReal_Ln     (x : LongReal) : LongReal; asmname '_p_l_ln';
function LongReal_Exp    (x : LongReal) : LongReal; asmname '_p_l_exp';
function LongReal_Sin    (x : LongReal) : LongReal; asmname '_p_l_sin';
function LongReal_Cos    (x : LongReal) : LongReal; asmname '_p_l_cos';

{ Extended Pascal `**' operator }
function Real_Power      (x, y : Double)   : Double;   asmname '_p_expon';
function LongReal_Power  (x, y : LongReal) : LongReal; asmname '_p_l_expon';

function StatusExited     (Status : Integer) : Boolean; attribute (const); asmname '_p_WIfExited';
function StatusExitCode   (Status : Integer) : Integer; attribute (const); asmname '_p_WExitStatus';
function CSystem (CmdLine : CString) : Integer; asmname '_p_csystem';
procedure CSetEnv (VarName, Value, NewEnvCString : CString; UnSet : Boolean); asmname '_p_csetenv';

type
  TCPasswordEntry = record
    UserName, RealName, Password, HomeDirectory, Shell : CString;
    UID, GID : Integer
  end;

  PCPasswordEntries = ^TCPasswordEntries;
  TCPasswordEntries = array [0 .. MaxVarSize div SizeOf (TCPasswordEntry)] of TCPasswordEntry;

function  CGetPwNam (UserName : CString; var Entry : TCPasswordEntry) : Boolean; asmname '_p_cgetpwnam';
function  CGetPwUID (UID : Integer;      var Entry : TCPasswordEntry) : Boolean; asmname '_p_cgetpwuid';
function  CGetPwEnt (var Entries : PCPasswordEntries) : Integer; asmname '_p_cgetpwent';

implementation

begin
  GPC_Initialize_Std_Files; (*@@iocritical*)GPC_CheckInOutRes;
  GPC_Init_Misc;
  GPC_Init_Time
end.
