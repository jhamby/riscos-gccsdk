{
File name routines

Copyright (C) 1998-2000 Free Software Foundation, Inc.

Authors: Dr A Olowofoyeku <African_Chief@bigfoot.com>
         Frank Heckenbach <frank@pascal.gnu.de>

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

unit FileName;

interface

uses Internal, Error, String, String2;

(*@@sietse1*)const TStringSize = 2048;
type TString = String (TStringSize);

{
  Define constants for different systems:

  OSDosFlag:         flag to indicate whether the target system is
                     Dos

  QuotingCharacter:  the character used to quote wild cards and
                     other special characters (#0 if not available)

  PathSeparator:     the separator of multiple paths, e.g. in the
                     PATH environment variable

  DirSeparator:      the separator of the directories within a full
                     file name

  DirSeparators:     a set of all possible directory and drive name
                     separators

  ExtSeparator:      the separator of a file name extension

  DirRoot:           the name of the root directory

  DirSelf:           the name of a directory in itself

  DirParent:         the name of the parent directory

  NullDeviceName:    the full file name of the null device

  TTYDeviceName:     the full file name of the current TTY

  ConsoleDeviceName: the full file name of the system console. On
                     Dos systems, this is the same as the TTY, but
                     on systems that allow remote login, this is a
                     different thing and may reach a completely
                     different user than the one running the
                     program, so use with care.

  EnvVarCharsFirst:  the characters accepted at the beginning of the
                     name of an environment variable without quoting

  EnvVarChars:       the characters accepted in the name of an
                     environment variable without quoting

  PathEnvVar:        the name of the environment variable which
                     (usually) contains the executable search path

  ShellEnvVar:       the name of the environment variable which
                     (usually) contains the path of the shell
                     executable (see GetShellPath)

  ShellExecCommand:  the option to the (default) shell to execute
                     the command specified in the following argument
                     (see GetShellPath)

  ConfigFileMask:    a mask for the option file name as returned by
                     ConfigFileName

  FileNamesCaseSensitive:
                     flag to indicate whether file names are case
                     sensitive
}

const
  UnixShellEnvVar        = 'SHELL';
  UnixShellExecCommand   = '-c';

{$ifdef __OS_DOS__}

const
  OSDosFlag              = True;
  QuotingCharacter       = #0;
  PathSeparator          = {$ifdef __CYGWIN32__} ':' {$else} ';' {$endif};
  DirSeparator           = '\';
  DirSeparators          = [':', '\', '/'];
  ExtSeparator           = '.';
  DirRoot                = '\';
  DirSelf                = '.';
  DirParent              = '..';
  NullDeviceName         = 'nul';
  TTYDeviceName          = 'con';
  ConsoleDeviceName      = 'con';
  EnvVarCharsFirst       = ['A' .. 'Z', 'a' .. 'z', '_'];
  EnvVarChars            = EnvVarCharsFirst + ['0' .. '9'];
  PathEnvVar             = 'PATH';
  ShellEnvVar            = 'COMSPEC';
  ShellExecCommand       = '/c';
  ConfigFileMask         = '*.cfg';
  FileNamesCaseSensitive = False;

{$else}

const
  OSDosFlag              = False;
  QuotingCharacter       = '\';
  PathSeparator          = ':';
  DirSeparator           = '/';
  DirSeparators          = ['/'];
  ExtSeparator           = '.';
  DirRoot                = '/';
  DirSelf                = '.';
  DirParent              = '..';
  NullDeviceName         = '/dev/null';
  TTYDeviceName          = '/dev/tty';
  ConsoleDeviceName      = '/dev/console';
  EnvVarCharsFirst       = ['A' .. 'Z', 'a' .. 'z', '_'];
  EnvVarChars            = EnvVarCharsFirst + ['0' .. '9'];
  PathEnvVar             = 'PATH';
  ShellEnvVar            = UnixShellEnvVar;
  ShellExecCommand       = UnixShellExecCommand;
  ConfigFileMask         = '.*';
  FileNamesCaseSensitive = True;

{$endif}

const
  WildCardChars = ['*', '?', '[', ']'];
  FileNameSpecialChars = (WildCardChars + SpaceCharacters + ['{', '}', '$', QuotingCharacter]) - DirSeparators;

type
  DirPtr = Pointer;

  PPStrings = ^TPStrings;
  TPStrings (Count : Cardinal) = array [1 .. Count] of ^String;

  GlobBuffer = record
    Count    : Integer;
    Result   : PCStrings;
    Internal : Pointer
  end;

{ Convert ch to lower case if FileNamesCaseSensitive is False, leave
  it unchanged otherwise. }
function  FileNameLoCase (ch : Char) : Char;                               asmname '_p_filenamelocase';

{ Change a file name to use the OS dependent directory separator }
function  Slash2OSDirSeparator (const s : String) : TString;               asmname '_p_slash2osdirseparator';

{ Change a file name to use '/' as directory separator }
function  OSDirSeparator2Slash (const s : String) : TString;               asmname '_p_osdirseparator2slash';

{ Like Slash2OSDirSeparator for CStrings -- NOTE: overwrites the
  CString }
function  Slash2OSDirSeparator_CString (s : CString) : CString;            asmname '_p_slash2osdirseparator_cstring';

{ Like OSDirSeparator2Slash for CStrings -- NOTE: overwrites the
  CString }
function  OSDirSeparator2Slash_CString (s : CString) : CString;            asmname '_p_osdirseparator2slash_cstring';

{ Add a DirSeparator to the end of s, if there is not already one
  and s denotes an existing directory }
function  AddDirSeparator (const s : String) : TString;                    asmname '_p_adddirseparator';

{ Like AddDirSeparator, but also if the directory does not exist }
function  ForceAddDirSeparator (const s : String) : TString;               asmname '_p_forceadddirseparator';

{ Remove all trailing DirSeparators from s, if there are any, as
  long as removing them doesn't change the meaning (i.e., they don't
  denote the root directory. }
function  RemoveDirSeparator (const s : String) : TString;                 asmname '_p_removedirseparator';

{ Returns the current directory using OS dependent directory
  separators }
function  GetCurrentDirectory     : TString;                               asmname '_p_get_current_directory';

{ Returns a directory suitable for storing temporary files using OS
  dependent directory separators. If found, the result always ends
  in DirSeparator. If no suitable directory is found, an empty
  string is returned. }
function  GetTempDirectory        : TString;                               asmname '_p_get_temp_directory';

{ Returns a non-existing file name in GetTempDirectory. If no temp
  directory is found, i.e. GetTempDirectory returns the empty
  string, a runtime error is raised. }
(*@@iocritical*)function  GetTempFileName         : TString;                               asmname '_p_get_temp_file_name';

{ The same as GetTempFileName, but returns a CString allocated from
  the heap. }
(*@@iocritical*)function  GetTempFileName_CString : CString;                               asmname '_p_get_temp_file_name_cstring';

{ Get the external name of a file }
function  FileName (var f : AnyFile) : TString;                            asmname '_p_file_name';

{ Returns True if the given file name is an existing plain file }
function  FileExists      (const aFileName : String) : Boolean;            asmname '_p_file_exists';

{ Returns True if the given file name is an existing directory }
function  DirectoryExists (const aFileName : String) : Boolean;            asmname '_p_directory_exists';

{ Returns True if the given file name is an existing file, directory
  or special file (device, pipe, socket, etc.) }
function  PathExists      (const aFileName : String) : Boolean;            asmname '_p_path_exists';

{ If a file of the given name exists in one of the directories given
  in DirList (separated by PathSeparator), returns the full path,
  otherwise returns an empty string. If aFileName already contains
  an element of DirSeparators, returns Slash2OSDirSeparator
  (aFileName) if it exists. }
function  FSearch (const aFileName, DirList : String) : TString;           asmname '_p_fsearch';

{ Like FSearch, but only find executable files. Under Dos, if not
  found, the function tries appending '.com', '.exe' and '.bat', so
  you don't have to specify these extensions in aFileName (and with
  respect to portability, it might be preferable not to do so). }
function  FSearchExecutable (const aFileName, DirList : String) : TString; asmname '_p_fsearch_executable';

{ Replaces all occurrences of `$FOO' and `~' in s by the value of
  the environment variables FOO or HOME, respectively. If a variable
  is not defined, the function returns False, and s contains the
  name of the undefined variable (or the empty string if the
  variable name is invalid, i.e., doesn't start with a character
  from EnvVarCharsFirst). Otherwise, if all variables are found, s
  contains the replaced string, and True is returned. }
function  ExpandEnvironment (var s : String) : Boolean;                    asmname '_p_expand_environment';

{ Expands the given path name to a full path name. Relative paths
  are expanded using the current directory, and occurrences of
  DirSelf and DirParent are resolved. Under Dos, the result is
  converted to lower case and a trailing ExtSeparator (except in a
  trailing DirSelf or DirParent) is removed, like Dos does. If the
  directory, i.e. the path without the file name, is invalid, the
  empty string is returned. }
function  FExpand       (const Path : String) : TString;                   asmname '_p_fexpand';

{ Like FExpand, but unquotes the directory before expanding it, and
  quotes WildCardChars again afterwards. Does not check if the
  directory is valid (because it may contain wild card characters).
  Symlinks are expanded only in the directory part, not the file
  name. }
function  FExpandQuoted (const Path : String) : TString;                   asmname '_p_fexpandquoted';

{ FExpands Path, and then removes the current directory from it, if
  it is a prefix of it. If OnlyCurDir is set, the current directory
  will be removed only if Path denotes a file in, not below, it. }
function  RelativePath (const Path : String; OnlyCurDir, Quoted : Boolean) : TString; asmname '_p_relative_path';

{ Is aFileName a UNC filename? (Always returns False on non-Dos
  systems.) }
function  IsUNC (const aFileName : String) : Boolean;                      asmname '_p_IsUNC';

{ Splits a file name into directory, name and extension }
procedure FSplit (const Path : String; var Dir, Name, Ext : String);       asmname '_p_fsplit';

{ Functions that extract one or two of the parts from FSplit.
  DirFromPath returns DirSelf + DirSeparator if the path contains no
  directory. }
function  DirFromPath     (const Path : String) : TString;                 asmname '_p_dir_from_path';
function  NameFromPath    (const Path : String) : TString;                 asmname '_p_name_from_path';
function  ExtFromPath     (const Path : String) : TString;                 asmname '_p_ext_from_path';
function  NameExtFromPath (const Path : String) : TString;                 asmname '_p_name_ext_from_path';

{ Start reading a directory. If successful, a pointer is returned
  that can be used for subsequent calls to ReadDir and finally
  CloseDir. On failure, an I/O error is raised and (in case it is
  ignored) nil is returned. }
(*@@iocritical*)function  OpenDir  (const Name : String) : DirPtr;                         asmname '_p_opendir';

{ Reads one entry from the directory Dir, and returns the file name.
  On errors or end of directory, the empty string is returned. }
function  ReadDir  (Dir : DirPtr) : TString;                               asmname '_p_readdir';

{ Closes a directory opened with OpenDir. }
(*@@iocritical*)procedure CloseDir (Dir : DirPtr);                                         asmname '_p_closedir';

{ Returns the first position of a non-quoted character of CharSet in
  s, or 0 if no such character exists. }
function  FindNonQuotedChar (Chars : CharSet; const s : String; From : Integer) : Integer; asmname '_p_findnonquotedchar';

{ Returns the first occurence of SubString in s that is not quoted
  at the beginning, or 0 if no such occurence exists. }
function  FindNonQuotedStr (const SubString, s : String; From : Integer) : Integer; asmname '_p_findnonquotedstr';

{ Does a string contain non-quoted wildcard characters? }
function  HasWildCards (const s : String) : Boolean;                       asmname '_p_haswildcards';

{ Does a string contain non-quoted wildcard characters, braces or
  spaces? }
function  HasWildCardsOrBraces (const s : String) : Boolean;               asmname '_p_haswildcardsorbraces';

{ Insert QuotingCharacter into s before any special characters }
function  QuoteFileName (const s : String; const SpecialCharacters : CharSet) : TString; asmname '_p_quote_filename';

{ Remove QuotingCharacter from s }
function  UnQuoteFileName (const s : String) : TString;                    asmname '_p_unquote_filename';

{ Splits s at non-quoted spaces and expands non-quoted braces like
  bash does. The result and its entries should be Disposed after
  usage. }
function  BraceExpand (const s : String) : PPStrings;                      asmname '_p_braceexpand';

{ Tests if a file name matches a shell wildcard pattern (?, *, []) }
function  FileNameMatch (const Pattern, Name : String) : Boolean;          asmname '_p_filenamematch';

{ FileNameMatch with BraceExpand }
function  MultiFileNameMatch (const Pattern, Name : String) : Boolean;     asmname '_p_multifilenamematch';

{ File name globbing }
{ GlobInit is implied by Glob and MultiGlob, not by GlobOn and
  MultiGlobOn. GlobOn and MultiGlobOn must be called after GlobInit,
  Glob or MultiGlob. MultiGlob and MultiGlobOn do brace expansion,
  Glob and GlobOn do not. GlobFree frees the memory allocated by the
  globbing functions and invalidates the results in Buf. It should
  be called after globbing. }
procedure GlobInit    (var Buf : GlobBuffer);                              asmname '_p_globinit';
procedure Glob        (var Buf : GlobBuffer; Pattern : CString);           asmname '_p_glob';
procedure GlobOn      (var Buf : GlobBuffer; Pattern : CString);           asmname '_p_globon';
procedure MultiGlob   (var Buf : GlobBuffer; Pattern : String);            asmname '_p_multiglob';
procedure MultiGlobOn (var Buf : GlobBuffer; Pattern : String);            asmname '_p_multiglobon';
procedure GlobFree    (var Buf : GlobBuffer);                              asmname '_p_globfree';

type
  TPasswordEntry = record
    UserName, RealName, Password, HomeDirectory, Shell : TString;
    UID, GID : Integer
  end;

  PPasswordEntries = ^TPasswordEntries;
  TPasswordEntries (Count : Integer) = array [1 .. Count] of TPasswordEntry;

{ Finds a password entry by user name. Returns True if found, False
  otherwise. }
function  GetPasswordEntryByName (const UserName : String; var Entry : TPasswordEntry) : Boolean; asmname '_p_getpasswordentrybyname';

{ Finds a password entry by UID. Returns True if found, False
  otherwise. }
function  GetPasswordEntryByUID  (UID : Integer; var Entry : TPasswordEntry) : Boolean; asmname '_p_getpasswordentrybyuid';

{ Returns all password entries, or nil if none found. }
function  GetPasswordEntries : PPasswordEntries; asmname '_p_getpasswordentries';

{ Returns the mount point (Unix) or drive (Dos) which is part of the
  given path. If the path does not contain any (i.e., is a relative
  path), an empty string is returned. Therefore, if you want to get
  the mount point or drive in any case, apply `FExpand' or
  `RealPath' to the argument. }
function  GetMountPoint (const Path : String) = Result : TString; asmname '_p_GetMountPoint';

{ Returns the path to the shell (as the return value) and the option
  that makes it execute the command specified in the following
  argument (in `Option'). Usually these are the environment value of
  ShellEnvVar, and ShellExecCommand, but on Dos systems, the
  function will first try UnixShellEnvVar, and UnixShellExecCommand
  because ShellEnvVar will usually point to command.com, but
  UnixShellEnvVar can point to bash which is usually a better choice
  when present. If UnixShellEnvVar is not set, or the shell given
  does not exist, it will use ShellEnvVar, and ShellExecCommand.
  Option may be null (in case you want to invoke the shell
  interactively). }
function  GetShellPath (var Option : String) : TString; asmname '_p_GetShellPath';

{ Returns the path of the running executable. NOTE: On most systems,
  this is *not* guaranteed to be the full path, but often just the
  same as `ParamStr (0)' which usually is the name given on the
  command line. Only on some systems with special support, it
  returns the full path when `ParamStr (0)' doesn't. }
function  ExecutablePath : TString; asmname '_p_executable_path';

{ Returns a file name suitable for a global (system-wide) or local
  (user-specific) configuration file, depending on the Global
  parameter. The function does not guarantee that the file name
  returned exists or is readable or writable.

  In the following table, the base name `<base>' is given with the
  Name parameter. If it is empty, the base name is the name of the
  running program (as returned by ExecutablePath, without directory
  and extension. `<prefix>' (Unix only) stands for the value of the
  Prefix parameter (usual values include '', '/usr' and
  '/usr/local'). `<dir>' (Dos only) stands for the directory where
  the running program resides. `$foo' stands for the value of the
  environment variable `foo'.

         Global                    Local
  Unix:  <prefix>/etc/<base>.conf  $HOME/.<base>

  Dos:   $DJDIR\etc\<base>.ini     $HOME\<base>.cfg
         <dir>\<base>.ini          <dir>\<base>.cfg

  As you see, there are two possibilities under Dos. If the first
  file exists, it is returned. Otherwise, if the second file exists,
  that is returned. If none of them exists (but the program might
  want to create a file), if the environment variable (DJDIR or
  HOME, respectively) is set, the first file name is returned,
  otherwise the second one. This rather complicated scheme should
  give the most reasonable results for systems with or without DJGPP
  installed, and with or without already existing config files. Note
  that DJDIR is always set on systems with DJGPP installed, while
  HOME is not. However, it is easy for users to set it if they want
  their config files in a certain directory rather than with the
  executables. }
function  ConfigFileName (const Prefix, Name : String; Global : Boolean) : TString; asmname '_p_config_file_name';

{ Returns a directory name suitable for global, machine-independent
  data. The function garantees that the name returned ends with a
  DirSeparator, but does not guarantee that it exists or is
  readable or writable.

  Note: If the prefix is empty, it is assumed to be '/usr'. (If you
  really want /share, you could pass '/' as the prefix, but that's
  very uncommon.)

  Unix: <prefix>/share/<base>/

  Dos:  $DJDIR\share\<base>\
        <dir>\

  About the symbols used above, and the two possibilities under Dos,
  see the comments for ConfigFileName. }
function  DataDirectoryName (const Prefix, Name : String) : TString; asmname '_p_data_directory_name';

{ Executes a command line. Reports execution errors via the IOResult
  mechanism and returns the exit status of the executed program.
  Execute calls RestoreTerminal with the argument True before and
  False after executing the process, ExecuteNoTerminal does not. }
(*@@IO critical*)function  Execute (const CmdLine : String) : Integer; asmname '_p_execute';
(*@@IO critical*)function  ExecuteNoTerminal (const CmdLine : String) : Integer; asmname '_p_executenoterminal';

implementation

{$B-,I-}

{ Currently needed in file.c and bind.c }
var
  DirSeparatorVar : Char = DirSeparator; asmname '_p_dirseparatorvar';

function IsDirSeparator (ch : Char) : Boolean; asmname '_p_isdirseparator';
function IsDirSeparator (ch : Char) : Boolean;
begin
  IsDirSeparator := ch in DirSeparators
end;

function FileNameLoCase (ch : Char) : Char;
begin
  if FileNamesCaseSensitive
    then FileNameLoCase := ch
    else FileNameLoCase := LoCase (ch)
end;

function Slash2OSDirSeparator (const s : String) = Result : TString;
var i : Integer;
begin
  Result := s;
  if DirSeparator <> '/' then
    for i := 1 to Length (Result) do
      if Result [i] = '/' then Result [i] := DirSeparator
end;

function OSDirSeparator2Slash (const s : String) = Result : TString;
var i : Integer;
begin
  Result := s;
  if DirSeparator <> '/' then
    for i := 1 to Length (Result) do
      if Result [i] = DirSeparator then Result [i] := '/'
end;

function Slash2OSDirSeparator_CString (s : CString) : CString;
var Temp : CString;
begin
  if DirSeparator <> '/' then
    begin
      Temp := s;
      while (Temp^ <> #0) do
        begin
          if Temp^ = '/' then Temp^ := DirSeparator;
          {$local X+} Inc (Temp) {$endlocal}
        end
    end;
  Slash2OSDirSeparator_CString := s
end;

function OSDirSeparator2Slash_CString (s : CString) : CString;
var Temp : CString;
begin
  if DirSeparator <> '/' then
    begin
      Temp := s;
      while (Temp^ <> #0) do
        begin
          if Temp^ = DirSeparator then Temp^ := '/';
          {$local X+} Inc (Temp) {$endlocal}
        end
    end;
  OSDirSeparator2Slash_CString := s
end;

function ForceAddDirSeparator (const s : String) = Result : TString;
begin
  Result := s;
  if (Result <> '') and not (Result [Length (Result)] in (DirSeparators {$ifdef __OS_DOS__} - [':'] {$endif}))
    {$ifdef __OS_DOS__} and ((Length (Result) <> 2) or (Result [2] <> ':')) {$endif} then
    Result := Result + DirSeparator
end;

function AddDirSeparator (const s : String) = Result : TString;
begin
  Result := s;
  if (Result <> '') and not (Result [Length (Result)] in (DirSeparators {$ifdef __OS_DOS__} - [':'] {$endif}))
    and DirectoryExists (Result)
    {$ifdef __OS_DOS__} and ((Length (Result) <> 2) or (Result [2] <> ':')) {$endif} then
    Result := Result + DirSeparator
end;

function RemoveDirSeparator (const s : String) = Result : TString;
begin
  Result := s;
  while (Length (Result) > 1) and (Result [Length (Result)] in (DirSeparators {$ifdef __OS_DOS__} - [':'] {$endif}))
    {$ifdef __OS_DOS__} and ((Length (Result) <> 3) or (Result [2] <> ':')) {$endif} do
    Delete (Result, Length (Result), 1)
end;

function GetCurrentDirectory : TString;
begin
  GetCurrentDirectory := FExpand (DirSelf)
end;

function GetTempDirectory : TString;
var TempDirectory : static TString = '';

  function CheckDir (const s : String) : Boolean;
  var
    b : BindingType;
    f : Text;
  begin
    TempDirectory := Slash2OSDirSeparator (s);
    if TempDirectory = '' then return False;
    TempDirectory := ForceAddDirSeparator (TempDirectory);
    Assign (f, TempDirectory);
    b := Binding (f);
    Unbind (f);
    CheckDir := b.Bound and b.Directory and b.Readable and b.Writable and b.Executable
  end;

begin
  if not (
           CheckDir (TempDirectory) or
           CheckDir (GetEnv ('TMPDIR')) or
           CheckDir (GetEnv ('TEMPDIR')) or
           CheckDir (GetEnv ('TMP')) or
           CheckDir (GetEnv ('TEMP')) or
           {$ifdef __OS_DOS__}
           CheckDir ('c:\temp\') or
           CheckDir ('c:\tmp\') or
           CheckDir ('\temp\') or
           CheckDir ('\tmp\') or
           {$else}
           CheckDir ('/var/tmp/') or
           CheckDir ('/tmp/') or
           CheckDir (GetEnv ('HOME')) or
           {$endif}
           CheckDir ('./')
         ) then TempDirectory := '';
  GetTempDirectory := TempDirectory
end;

function GetTempFileName = Result : TString;
var
  TempStr : static TString = '';
  TempDirectory : TString;
  i : Integer;
begin
  TempDirectory := GetTempDirectory;
  if TempDirectory = '' then
    IOError (497); { no temporary file name found }
  if TempStr = '' then
    begin
      WriteStr (TempStr, 'GP9a', ProcessID mod 100000 + 100000);
      TempStr [5] := 'a'
    end;
  repeat
    i := 3;
    while (i <= Length (TempStr)) and (TempStr [i] = 'z') do
      begin
        TempStr [i] := '0';
        Inc (i)
      end;
    if TempStr [i] = '9' then TempStr [i] := 'a' else Inc (TempStr [i]);
    Result := TempDirectory + TempStr
  until not FileExists (Result)
end;

function GetTempFileName_CString : CString;
begin
  GetTempFileName_CString := NewCString (GetTempFileName)
end;

function FileName (var f : AnyFile) : TString;
begin
  FileName := CString2String (FileName_CString (f))
end;

function FileExists (const aFileName : String) : Boolean;
var
  b : BindingType;
  f : Text;
begin
  if aFileName = '' then return False;
  Assign (f, aFileName);
  b := Binding (f);
  Unbind (f);
  FileExists := b.Bound and b.Existing
end;

function DirectoryExists (const aFileName : String) : Boolean;
var
  b : BindingType;
  f : Text;
begin
  if aFileName = '' then return False;
  Assign (f, aFileName);
  b := Binding (f);
  Unbind (f);
  DirectoryExists := b.Bound and b.Directory
end;

function PathExists (const aFileName : String) : Boolean;
var
  b : BindingType;
  f : Text;
begin
  if aFileName = '' then return False;
  Assign (f, aFileName);
  b := Binding (f);
  Unbind (f);
  PathExists := b.Bound and (b.Existing or b.Directory or b.Special)
end;

function InternalFSearch (const aFileName, DirList : String; ExecutableFlag : Boolean) = Res : TString;
var p0, p1 : Integer;

  function Exists : Boolean;
  var
    b : BindingType;
    f : Text;
  begin
    Assign (f, Res);
    b := Binding (f);
    Unbind (f);
    Exists := b.Bound and b.Existing and (not ExecutableFlag or b.Executable)
  end;

begin
  if CharPos (DirSeparators, aFileName) <> 0 then
    begin
      Res := Slash2OSDirSeparator (aFileName);
      if Exists then Exit
    end
  else
    begin
      p0 := 1;
      while p0 <= Length (DirList) do
        begin
          p1 := p0;
          while (p1 <= Length (DirList)) and (DirList [p1] <> PathSeparator) do Inc (p1);
          if p0 = p1
            then Res := DirSelf + DirSeparator + aFileName
            else Res := ForceAddDirSeparator (Slash2OSDirSeparator (
                 (*@@sig11 without -O2, type mismatch otherwise  DirList [p0 .. p1 - 1]*) Copy (DirList, p0, p1 - p0)
                 )) + aFileName;
          if Exists then Exit;
          p0 := p1 + 1
        end
    end;
  Res := ''
end;

function FSearch (const aFileName, DirList : String) : TString;
begin
  FSearch := InternalFSearch (aFileName, DirList, False)
end;

function FSearchExecutable (const aFileName, DirList : String) = Result : TString;
begin
  Result := InternalFSearch (aFileName, DirList, True);
  {$ifdef __OS_DOS__}
  if Result = '' then Result := InternalFSearch (aFileName + '.com', DirList, True);
  if Result = '' then Result := InternalFSearch (aFileName + '.exe', DirList, True);
  if Result = '' then Result := InternalFSearch (aFileName + '.bat', DirList, True);
  {$endif}
end;

function ExpandEnvironment (var s : String) : Boolean;
var
  p, q : Integer;
  EnvName, EnvValue : TString;
  Env : CString;
  Quoted, Found : Boolean;
  PasswordEntry : TPasswordEntry;
begin
  p := 0;
  repeat
    repeat
      if (p <> 0) and (DirSeparator <> QuotingCharacter) and (s [p] = QuotingCharacter) then Inc (p);
      Inc (p);
      if p > Length (s) then return True
    until (s [p] = '$') or ((p = 1) and (s [p] = '~'));
    if s [p] = '~' then
      begin
        q := CharPosFrom (DirSeparators + ['$'], s, p + 1);
        if q = 0 then q := Length (s) + 1;
        if q = p + 1 then
          begin
            EnvName := 'HOME';
            Env := CStringGetEnv (EnvName);
            Found := Env <> nil;
            EnvValue := CString2String (Env)
          end
        else
          begin
            EnvName := Copy (s, p, q - p);
            Found := GetPasswordEntryByName (Copy (s, p + 1, q - p - 1), PasswordEntry);
            if Found then EnvValue := PasswordEntry.HomeDirectory
          end
      end
    else
      begin
        EnvName := '';
        Quoted := False;
        q := p + 1;
        if (q <= Length (s)) and (s [q] in (EnvVarCharsFirst + [QuotingCharacter] - DirSeparators)) then
          while (q <= Length (s)) and (Quoted or (s [q] in (EnvVarChars + [QuotingCharacter] - DirSeparators))) do
            begin
              if not Quoted and (s [q] = QuotingCharacter)
                then Quoted := True
                else
                  begin
                    Quoted := False;
                    EnvName := EnvName + s [q]
                  end;
              Inc (q)
            end;
        Env := CStringGetEnv (EnvName);
        Found := Env <> nil;
        EnvValue := CString2String (Env)
      end;
    Delete (s, p, q - p);
    if not Found then
      begin
        s := EnvName;
        return False
      end;
    Insert (EnvValue, s, p);
    Inc (p, Length (EnvValue) - 1)
  until False
end;

function InternalFExpand (const Path : String; Quoted, CheckExisting : Boolean) = Result : TString;
var
  Existing : Boolean;
  Dir, Name, Ext : TString;
begin
  Existing := not Quoted and PathExists (Path);
  if Existing then
    begin
      Dir := Path;
      if OSDosFlag and (ExtFromPath (Dir) = ExtSeparator) then Delete (Dir, Length (Dir), 1);
      Result := CString2String (RealPath (OSDirSeparator2Slash (Dir)));
      if Result = '' then Result := Dir;
      if (Length (Result) > 2) and (Result [Length (Result)] = DirSelf)
        and (Result [Length (Result) - 1] in DirSeparators) then
        Delete (Result, Length (Result), 1);
      Result := RemoveDirSeparator (Slash2OSDirSeparator (Result))
    end
  else
    begin
      FSplit (Path, Dir, Name, Ext);
      if OSDosFlag and (Ext = ExtSeparator) then Ext := '';
      Name := Name + Ext;
      if (Name = DirSelf) or (Name = DirParent) then
        begin
          Dir := Dir + Name;
          Name := ''
        end;
      if Quoted then Dir := UnQuoteFileName (Dir);
      if Dir = '' then Dir := DirSelf;
      Result := CString2String (RealPath (OSDirSeparator2Slash (Dir)));
      if Result = '' then Result := Dir;
      if (Length (Result) > 2) and (Result [Length (Result)] = DirSelf)
        and (Result [Length (Result) - 1] in DirSeparators) then
        Delete (Result, Length (Result), 1);
      Result := RemoveDirSeparator (Slash2OSDirSeparator (Result));
      if Name <> '' then Result := ForceAddDirSeparator (Result);
      if Quoted then Result := QuoteFileName (Result, WildCardChars);
      if not CheckExisting or DirectoryExists (Result)
        then Result := Result + Name
        else Result := ''
    end;
  if not FileNamesCaseSensitive then Result := LoCaseStr (Result)
end;

function FExpand (const Path : String) : TString;
begin
  FExpand := InternalFExpand (Path, False, True)
end;

function FExpandQuoted (const Path : String) : TString;
begin
  FExpandQuoted := InternalFExpand (Path, True, False)
end;

function RelativePath (const Path : String; OnlyCurDir, Quoted : Boolean) = Result : TString;
var p : TString;
begin
  Result := InternalFExpand (Path, Quoted, False);
(*@@sig11 without -O2, type mismatch otherwise  p := AddDirSeparator (FExpand (DirSelf)); *)p := DirSelf;p := AddDirSeparator (FExpand (p));
  if IsPrefix (p, Result) and
    (not OnlyCurDir or (LastCharPos (DirSeparators, Result) <= Length (p))) then
    Delete (Result, 1, Length (p));
  if Result = '' then Result := DirSelf
end;

function IsUNC (const aFileName : String) : Boolean;
begin
  IsUNC := {$ifdef __OS_DOS__}
           (Length (aFileName) > 3) and
           (aFileName [1] in [DirSeparator, '/']) and
           (aFileName [2] in [DirSeparator, '/'])
           {$else}
           False;
           var Dummy : Integer;
           Dummy := Length (aFileName)
           {$endif}
end;

procedure FSplit (const Path : String; var Dir, Name, Ext : String);
var
  d, e : Integer;
  p, t : TString;
begin
  p := Path; { make a copy in case one of the destination variables is the same as Path }
  d := LastCharPos (DirSeparators, p);
  if IsUNC (p) and (d in [1, 2]) then d := Length (p);
  t := Copy (p, d + 1);
  e := LastPos (ExtSeparator, t);
  if (e <= 1) or (t = DirSelf) or (t = DirParent)
    then e := Length (p) + 1
    else Inc (e, d);
  Dir  := Slash2OSDirSeparator (Copy (p, 1, d));
  Name := Slash2OSDirSeparator (Copy (p, d + 1, e - d - 1));
  Ext  := Slash2OSDirSeparator (Copy (p, e))
end;

function DirFromPath (const Path : String) = Dir : TString;
var Name, Ext : TString;
begin
  FSplit (Path, Dir, Name, Ext);
  if Dir = '' then Dir := DirSelf + DirSeparator
end;

function NameFromPath (const Path : String) = Name : TString;
var Dir, Ext : TString;
begin
  FSplit (Path, Dir, Name, Ext)
end;

function ExtFromPath (const Path : String) = Ext : TString;
var Dir, Name : TString;
begin
  FSplit (Path, Dir, Name, Ext)
end;

function NameExtFromPath (const Path : String) : TString;
var Dir, Name, Ext : TString;
begin
  FSplit (Path, Dir, Name, Ext);
  NameExtFromPath := Name + Ext
end;

function OpenDir (const Name : String) = Res : DirPtr;
var s : TString;
begin
  s := RemoveDirSeparator (Name);
  if s = DirSelf then s := GetCurrentDirectory;
  Res := COpenDir (s);
  if Res = nil then IOErrorCString (495, Name); { cannot open directory `%s' }
end;

function ReadDir (Dir : DirPtr) : TString;
begin
  ReadDir := CString2String (CReadDir (Dir))
end;

function FindNonQuotedChar (Chars : CharSet; const s : String; From : Integer) : Integer;
var i : Integer;
begin
  i := From;
  while (i <= Length (s)) and not (s [i] in Chars) do
    begin
      if (DirSeparator <> QuotingCharacter) and (s [i] = QuotingCharacter) then Inc (i);
      Inc (i)
    end;
  if i > Length (s) then FindNonQuotedChar := 0 else FindNonQuotedChar := i
end;

function FindNonQuotedStr (const SubString, s : String; From : Integer) : Integer;
var i, n : Integer;
begin
  i := From;
  n := Length (s) - Length (SubString) + 1;
  while (i <= n) and (Copy (s, i, Length (SubString)) <> SubString) do
    begin
      if (DirSeparator <> QuotingCharacter) and (s [i] = QuotingCharacter) then Inc (i);
      Inc (i)
    end;
  if i > n then FindNonQuotedStr := 0 else FindNonQuotedStr := i
end;

function HasWildCards (const s : String) : Boolean;
begin
  HasWildCards := FindNonQuotedChar (WildCardChars, s, 1) <> 0
end;

function HasWildCardsOrBraces (const s : String) : Boolean;
begin
  HasWildCardsOrBraces := FindNonQuotedChar (WildCardChars + [' ', '{'], s, 1) <> 0
end;

function QuoteFileName (const s : String; const SpecialCharacters : CharSet) = Result : TString;
var i : Integer;
begin
  Result := s;
  i := 1;
  if DirSeparator <> QuotingCharacter then
    while i <= Length (Result) do
      begin
        if Result [i] in SpecialCharacters then
          begin
            Insert (QuotingCharacter, Result, i);
            Inc (i)
          end;
        Inc (i)
      end
end;

function UnQuoteFileName (const s : String) = Result : TString;
var i : Integer;
begin
  Result := s;
  i := 1;
  if DirSeparator <> QuotingCharacter then
    while i <= Length (Result) do
      begin
        if Result [i] = QuotingCharacter then Delete (Result, i, 1);
        Inc (i)
      end
end;

function BraceExpand (const s : String) = Result : PPStrings;
type
  PPStrList = ^TPStrList;
  TPStrList = record
    Next : PPStrList;
    p : ^String
  end;

var
  st : TString;
  Count, c, i, j : Integer;
  List, p : PPStrList;

  function FindNonQuotedChr (ch, cho, chc : Char; const s : String; From, m : Integer) = i : Integer;
  var c : Integer;
  begin
    c := 0;
    i := From;
    while i <= m do
      begin
        if s [i] in [ch, cho, chc] then
          begin
            j := i - 1;
            if DirSeparator <> QuotingCharacter then
              while (j > 0) and (s [j] = QuotingCharacter) do Dec (j);
            if Odd (i - j) then
              if (s [i] = ch) and (c = 0) then
                Exit
              else if s [i] = cho then
                Inc (c)
              else if s [i] = chc then
                Dec (c)
          end;
        Inc (i)
      end
  end;

  procedure Expand (const s : String);
  var io, ic, ip, ia : Integer;
  begin
    io := FindNonQuotedChr ('{', '{', '{', s, 1, Length (s));
    ic := FindNonQuotedChr ('}', '{', '}', s, io + 1, Length (s));
    if ic > Length (s) then
      begin
        Inc (Count);
        New (p);
        p^.Next := List;
        p^.p := NewString (s);
        List := p;
        Exit
      end;
    ip := io;
    repeat
      ia := FindNonQuotedChr (',', '{', '}', s, ip + 1, ic - 1);
      Expand (Copy (s, 1, io - 1) + Copy (s, ip + 1, ia - ip - 1) + Copy (s, ic + 1));
      ip := ia
    until ia >= ic
  end;

begin
  Count := 0;
  List := nil;
  st := TrimRightStr (s);
  repeat
    TrimLeft (st);
    if st = '' then Break;
    i := FindNonQuotedChr (' ', ' ', ' ', st, 1, Length (st));
    Expand (Copy (st, 1, i - 1));
    Delete (st, 1, i)
  until False;
  New (Result, Count);
  for c := Count downto 1 do
    begin
      Result^[c] := List^.p;
      p := List;
      List := List^.Next;
      Dispose (p)
    end
end;

{ @@ This routine is just a quick hack for systems that don't have
  fnmatch() in their libc. Especially, the handling of `*' is very inelegant. }
function FNMatch2 (const Pattern, Name : String) : Boolean;
var
  zp, zp0, zn, z2 : Integer;
  ch : Char;
  (*@@ s : set of Char;*)s:array[char] of boolean;
  Quoted, Negate : Boolean;
  Pattern2 : TString;

begin
  FNMatch2 := False;
  zn := 1;
  zp := 1;
  Quoted := False;
  while zp <= Length (Pattern) do
    begin
      ch := Pattern [zp];
      Inc (zp);
      if not Quoted and (DirSeparator <> QuotingCharacter) and (ch = QuotingCharacter) then
        begin
          Quoted := True;
          Continue
        end;
      if not Quoted and (ch = '*') then
        begin
          while (zp <= Length (Pattern)) and (Pattern [zp] = '*') do Inc (zp);
          Pattern2 := Copy (Pattern, zp);
          for z2 := Length (Name) + 1 downto zn do
            if FNMatch2 (Pattern2, Copy (Name, z2)) then
              begin
                FNMatch2 := True;
                Exit
              end;
          Exit
        end;
      if zn > Length (Name) then Exit;
      if not Quoted and (ch = '[') then
        begin
          Negate := (zp <= Length (Pattern)) and (Pattern [zp] = '^');
          if Negate then Inc (zp);
          (* s := []; *) for ch := low (ch) to high (ch) do s [ch] := false;
          zp0 := zp;
          while (zp <= Length (Pattern)) and ((zp = zp0) or (Pattern [zp] <> ']')) do
            begin
              ch := Pattern [zp];
              Inc (zp);
              if (zp <= Length (Pattern)) and (Pattern [zp] = '-') then
                begin
(*@@fjf262                  s := s + [FileNameLoCase (ch) .. FileNameLoCase (Pattern [zp + 1])]; *) ch := FileNameLoCase (ch); while ch <= FileNameLoCase (Pattern [zp + 1]) do begin s [ch] := true; inc (ch) end;
                  Inc (zp, 2)
                end
              else (* Include (s, FileNameLoCase (ch)) *)s[FileNameLoCase (ch)]:=true
            end;
          Inc (zp);
          if not (* (FileNameLoCase (Name [zn]) in s) *)s[FileNameLoCase (name[zn])] xor Negate then Exit
        end
      else
        if (FileNameLoCase (Name [zn]) <> FileNameLoCase (ch)) and (Quoted or (ch <> '?')) then Exit;
      Inc (zn);
      Quoted := False
    end;
  FNMatch2 := zn > Length (Name)
end;

function FileNameMatch (const Pattern, Name : String) : Boolean;
begin
  if HaveFNMatch <> 0
    then FileNameMatch := FNMatch  (Pattern, Name) = 0
    else FileNameMatch := FNMatch2 (Pattern, Name)
end;

function MultiFileNameMatch (const Pattern, Name : String) = Result : Boolean;
var
  s : PPStrings;
  c : Integer;
begin
  Result := False;
  s := BraceExpand (Pattern);
  for c := 1 to s^.Count do
    begin
      if not Result and FileNameMatch (s^[c]^, Name) then Result := True;
      Dispose (s^[c])
    end;
  Dispose (s)
end;

procedure GlobInit (var Buf : GlobBuffer);
begin
  Buf.Count := 0;
  Buf.Result := nil;
  Buf.Internal := nil
end;

procedure Glob (var Buf : GlobBuffer; Pattern : CString);
begin
  GlobInit (Buf);
  GlobOn (Buf, Pattern)
end;

procedure MultiGlob (var Buf : GlobBuffer; Pattern : String);
begin
  GlobInit (Buf);
  MultiGlobOn (Buf, Pattern)
end;

procedure MultiGlobOn (var Buf : GlobBuffer; Pattern : String);
var
  s : PPStrings;
  c : Integer;
begin
  s := BraceExpand (Pattern);
  for c := 1 to s^.Count do
    begin
      GlobOn (Buf, s^[c]^);
      Dispose (s^[c])
    end;
  Dispose (s)
end;

procedure PWC2P (const CEntry : TCPasswordEntry; var Entry : TPasswordEntry);
begin
  Entry.UserName      := CString2String (CEntry.UserName);      Dispose (CEntry.UserName);
  Entry.RealName      := CString2String (CEntry.RealName);      Dispose (CEntry.RealName);
  Entry.Password      := CString2String (CEntry.Password);      Dispose (CEntry.Password);
  Entry.HomeDirectory := CString2String (CEntry.HomeDirectory); Dispose (CEntry.HomeDirectory);
  Entry.Shell         := CString2String (CEntry.Shell);         Dispose (CEntry.Shell);
  Entry.UID           := CEntry.UID;
  Entry.GID           := CEntry.GID
end;

function GetPasswordEntryByName (const UserName : String; var Entry : TPasswordEntry) = Res : Boolean;
var CEntry : TCPasswordEntry;
begin
  Res := CGetPwNam (UserName, CEntry);
  if Res then PWC2P (CEntry, Entry)
end;

function GetPasswordEntryByUID (UID : Integer; var Entry : TPasswordEntry) = Res : Boolean;
var CEntry : TCPasswordEntry;
begin
  Res := CGetPwUID (UID, CEntry);
  if Res then PWC2P (CEntry, Entry)
end;

function GetPasswordEntries = Res : PPasswordEntries;
var
  CEntries : PCPasswordEntries;
  Count, i : Integer;
begin
  Count := CGetPwEnt (CEntries);
  if (Count = 0) or (CEntries = nil) then
    Res := nil
  else
    begin
      New (Res, Count);
      for i := 1 to Count do PWC2P (CEntries^[i - 1], Res^[i]);
      Dispose (CEntries)
    end
end;

function GetMountPoint (const Path : String) = Result : TString;
{$ifdef __OS_DOS__}
var
  i, j : Integer;
begin
  if IsUNC (Path) then
    begin
      i := 0;
      j := 0;
      while (i < Length (Path)) and (j < 4) do
        begin
          Inc (i);
          if Path [i] in [DirSeparator, '/'] then Inc (j)
        end;
      if j = 4 then
        Result := Copy (Path, 1, i - 1) { return servername + sharename }
      else
        Result := Path
    end
  else if Path [2] = ':' then
    Result := Copy (Path, 1, 2)
  else
    Result := ''
end;
{$else}
var
  NewPath : TString;
  f : File;
  b, LastBinding : BindingType;
begin
  NewPath := Path;
  Assign (f, NewPath);
  b := Binding (f);
  repeat
    Result := NewPath;
    NewPath := RemoveDirSeparator (DirFromPath (Result));
    if NewPath = Result then Break;
    LastBinding := b;
    Assign (f, NewPath);
    b := Binding (f)
  until b.Directory and LastBinding.Directory
    and ((b.Device <> LastBinding.Device) or (b.INode = LastBinding.INode));
  if Result = DirSelf then Result := ''
end;
{$endif}

function GetShellPath (var Option : String) = Res : TString;
begin
  {$ifdef __OS_DOS__}
  Res := GetEnv (UnixShellEnvVar);
  if FileExists (Res) then
    begin
      if @Option <> nil then Option := UnixShellExecCommand;
      Exit
    end;
  {$endif}
  Res := GetEnv (ShellEnvVar);
  if @Option <> nil then Option := ShellExecCommand
end;

function ExecutablePath : TString;
var
  CStringBuffer : TStringBuf;
  Buffer : static TString;
  BufferInitialized : Boolean = False;
begin
  if not BufferInitialized then
    begin
      BufferInitialized := True;
      Buffer := CString2String (CStringExecutablePath (CStringBuffer))
    end;
  ExecutablePath := Buffer
end;

{$ifdef __OS_DOS__}
function DJGPPChoose (const EnvVar, Suffix1, Name2 : String) : TString;
var EnvValue, Name1 : TString;
begin
  EnvValue := GetEnv (EnvVar);
  if EnvValue = '' then
    DJGPPChoose := Name2
  else
    begin
      Name1 := ForceAddDirSeparator (EnvValue) + Suffix1;
      if PathExists (Name1) or not PathExists (Name2) then
        DJGPPChoose := Name1
      else
        DJGPPChoose := Name2
    end
end;
{$endif}

function ConfigFileName (const Prefix, Name : String; Global : Boolean) : TString;
var Dummy, Dir, PName, Ext : TString;
begin
  {$ifdef __OS_DOS__}
  Dummy := Prefix;
  FSplit (ExecutablePath, Dir, PName, Ext);
  if Dir = '' then Dir := DirSelf + DirSeparator;
  if Name <> '' then PName := Name;
  if Global
    then ConfigFileName := DJGPPChoose ('DJDIR', 'etc' + DirSeparator + PName + '.ini', ForceAddDirSeparator (Dir) + PName + '.ini')
    else ConfigFileName := DJGPPChoose ('HOME', PName + '.cfg', ForceAddDirSeparator (Dir) + PName + '.cfg')
  {$else}
  if Name <> ''
    then PName := Name
    else FSplit (ExecutablePath, Dir, PName, Ext);
  if Global
    then ConfigFileName := Prefix + DirSeparator + 'etc' + DirSeparator + PName + '.conf'
    else ConfigFileName := ForceAddDirSeparator (GetEnv ('HOME')) + '.' + PName
  {$endif}
end;

function DataDirectoryName (const Prefix, Name : String) : TString;
var RealPrefix, Dir, PName, Ext : TString;
begin
  RealPrefix := Prefix;
  {$ifdef __OS_DOS__}
  FSplit (ExecutablePath, Dir, PName, Ext);
  if Dir = '' then Dir := DirSelf + DirSeparator;
  if Name <> '' then PName := Name;
  DataDirectoryName := DJGPPChoose ('DJDIR', 'share' + DirSeparator + PName + DirSeparator, ForceAddDirSeparator (Dir))
  {$else}
  if RealPrefix = '' then RealPrefix := DirSeparator + 'usr';
  if Name <> ''
    then PName := Name
    else FSplit (ExecutablePath, Dir, PName, Ext);
  DataDirectoryName := RealPrefix + DirSeparator + 'share' + DirSeparator + PName + DirSeparator
  {$endif}
end;

function InternalExecute (const CmdLine : String; RestoreTerminalFlag : Boolean) : Integer;
var Status : Integer;
begin
  if RestoreTerminalFlag then RestoreTerminal (True);
  Status := CSystem (OSDirSeparator2Slash (CmdLine));
  if RestoreTerminalFlag then RestoreTerminal (False);
  if StatusExited (Status) then
    InternalExecute := StatusExitCode (Status)
  else
    begin
      IOError (488); { `Execute': cannot execute program }
      InternalExecute := - 1
    end
end;

function Execute (const CmdLine : String) : Integer;
begin
  Execute := InternalExecute (CmdLine, True)
end;

function ExecuteNoTerminal (const CmdLine : String) : Integer;
begin
  ExecuteNoTerminal := InternalExecute (CmdLine, False)
end;

end.
