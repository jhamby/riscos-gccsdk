{
RTS initialization

Copyright (C) 1999-2000 Free Software Foundation, Inc.

Authors: Frank Heckenbach <frank@pascal.gnu.de>

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

unit Init;

interface

{ Use all RTS units, so their initializers are run from here. }
uses Endian, Error, FileName, Files, GetOpt, Heap, Internal, Math,
     Move, NumToDec, Random, Sets, String, String2, Time;

{
  Handle options for the Run Time System from the command line.
  Since the normal use of the command line is passing arguments to
  the user program, passing arguments to the runtime system is made
  somewhat complicated: First argument has to be `--gpc-rts'. Other
  flags that the RTS recognizes (if the first argument is
  `--gpc-rts') are output with `-h' option (see below).
  `--' indicates the end of RTS arguments.
}
procedure GPC_Init_Arguments; asmname '_p_init_arguments';

procedure GPC_Initialize (ArgumentCount : Integer; Arguments, StartEnvironment : GPC_PCStrings); asmname '_p_initialize';

implementation

{$B-,I-}

{ Declare a variable whose presence can be checked at link time
  to ensure that the correct RTS version is linked. }
{$ifndef RTS_RELEASE_STRING}
{$error RTS_RELEASE_STRING not set! Please compile this file properly from
the Makefile, or define that symbol manually if you know what you are doing.}
{$endif}
var
  RTSReleaseCheck : Integer = $47525453; asmname '__GPC_RTS_VERSION_'(*@@@@ + RTS_RELEASE_STRING + '__'*);

  {
    True if the EOLn hack is wanted.

    If False: EOLn will validate the file buffer if tst_UND

    If True, when EOLn is tested when all of the following are true
      1) tst_UND
      2) tst_UNREAD (nothing has been read after reset)
      3) tst_TXT
      4) tst_TTY
      5) tst_LGET,
    then the EOLn test returns True.

    If the EOLn is *not tested*, it is False. This is to make the
    program with `if EOLn then Readln;' in the very beginning work,
    they skip the EOLn when they test it, if you don't test, you
    don't have to skip it.
  }
  EOLnResetHack    : Boolean = False; asmname '_p_eoln_reset_hack';

  { True if direct access routines should work only for direct access files. }
  ForceDirectFiles : Boolean = False; asmname '_p_force_direct_files';

  FileAssociation  : PFileAssociation = nil; asmname '_p_FileAssociation';

  CurrentStdIn     : ^Text = nil; asmname '_p_current_stdin';

procedure GPC_Init_Arguments;
const
  LongOptions : array [1 .. 13] of OptionType =
    (('gpc-rts',            NoArgument,       nil, '='),
     ('help',               NoArgument,       nil, 'h'),
     ('version',            NoArgument,       nil, 'v'),
     ('abort-on-error',     NoArgument,       nil, 'a'),
     ('debug',              NoArgument,       nil, 'd'),
     ('eoln-hack',          NoArgument,       nil, 'e'),
     ('direct-files',       NoArgument,       nil, 'f'),
     ('input',              RequiredArgument, nil, 'i'),
     ('file-name',          RequiredArgument, nil, 'n'),
     ('show-rts-arguments', NoArgument,       nil, 's'),
     ('warn',               NoArgument,       nil, 'w'),
     ('error-file',         RequiredArgument, nil, 'E'),
     ('error-fd',           RequiredArgument, nil, 'F'));
var
  ArgsDone : static Boolean = False;
  HelpFlag, SkipArgs, NoSkip : Boolean;
  i, p : Integer;
  c : Char;
  Name : TString;
  ap : PFileAssociation;
  StdFile : ^Text = nil; { `-i' option strings written here, and given
                           as standard input to user program }
begin
  if ArgsDone or (CParameters = nil) then Exit;
  ArgsDone := True;
  HelpFlag := False;
  NoSkip := False;
  SkipArgs := False;
  if (CParamCount > 1) and (CStringComp (CParameters^[1], '--gpc-rts') = 0) then
    repeat
      c := GetOptLong ('G:hvadefi:n:swE:F:', LongOptions, null, False);
      if c = EndOfOptions then Break;
      SkipArgs := True;
      case c of
        '=' : ; { Ignore multiple '--gpc-rts' arguments }
        'h' : HelpFlag := True;
        'v' : begin
                Writeln ('GNU Pascal Run Time System version ' + RTS_RELEASE_STRING + '
Copyright (C) 1987-2000 Free Software Foundation, Inc.
The GNU Pascal RTS comes with NO WARRANTY, to the extent permitted
by law. You may redistribute copies of the GNU Pascal RTS under the
terms of the GNU General Public License. For more information about
these matters, see the file named COPYING.

Report bugs to <gpc@gnu.de>.
');
                Halt
              end;
        'a' : AbortOnError := True;
        'd' : Inc (RTSDebugFlag);
        'e' : EOLnResetHack := not EOLnResetHack;
        'f' : ForceDirectFiles := not ForceDirectFiles;
        'i' : begin
                if StdFile = nil then
                  begin
                    New (StdFile);
                    Rewrite (Stdfile^)
                  end;
                Writeln (StdFile^, OptionArgument)
              end;
        'n' : begin
                p := Pos (':', OptionArgument);
                if p = 0 then
                  begin
                    Writeln (StdErr, ParamStr (0), ': missing external file name for `', OptionArgument, '''' + NewLine);
                    HelpFlag := True
                  end
                else
                  begin
                    New (ap);
                    ap^.Next := FileAssociation;
                    ap^.IntName := NewCString (Copy (OptionArgument, 1, p - 1));
                    ap^.ExtName := NewCString (Copy (OptionArgument, p + 1));
                    FileAssociation := ap
                  end
              end;
        's' : NoSkip := True;
        'w' : RTSWarnFlag := True;
        'E' : ErrorFileName := NewCString (OptionArgument);
        'F' : begin
                {$local I+} ReadStr (OptionArgument, i); {$endlocal}
                ErrorFD := i
              end;
        else
          Dec (FirstNonOption);
          Break
      end
    until False;
  if HelpFlag then
    begin
      Write ('Allowed command line options for the GNU Pascal Run Time System after `--gpc-rts'':
  -h, --help                Display this help and exit
  -v, --version             Output RTS version information and exit
  -a, --abort-on-error      Abort with SIGABRT on runtime error
  -d, --debug               Increase RTS debugging level. Only meaningful if
                            RTS was compiled with `-DDEBUG''. This RTS was' {$ifdef DEBUG} ' not' {$endif}'
                            compiled with `-DDEBUG''.
  -e, --eoln-hack           Toggle EOLn hack right after Reset for terminals
  -f, --direct-files        Toggle forcing direct access files
  -i LINE, --input LINE     Implicitly add LINE to the beginning of Input
  -n INTERNAL_NAME:EXTERNAL_NAME, --file-name INTERNAL_NAME:EXTERNAL_NAME
                            Set an external file name
  -s, --show-rts-arguments  Let the program see RTS command line arguments
  -w, --warn                Give runtime warning messages
  -E FILENAME, --error-file FILENAME
                            Write runtime error messages also to FILENAME
  -F FD, --error-fd FD      Write runtime error messages also to fd #FD
  --                        Following arguments are not meant for the RTS
');
      Halt
    end;
  { Make RTS arguments invisible to the program unless the `-s' parameter was given. }
  if SkipArgs and not NoSkip then
    begin
      for i := 1 to CParamCount - FirstNonOption do { Leave argument #0 as it is. }
        CParameters^[i] := CParameters^[i + FirstNonOption - 1];
      Dec (CParamCount, FirstNonOption - 1)
    end;
  if StdFile <> nil then
    begin
      Reset (StdFile^);
      CurrentStdIn := StdFile;
      Close (Input);
      Reset (Input)
    end
end;

procedure InitInit; asmname 'init_Init';

procedure GPC_Initialize (ArgumentCount : Integer; Arguments, StartEnvironment : GPC_PCStrings);
begin
  CParamCount := ArgumentCount;
  CParameters := Arguments;
  InitInit; (*@@ call our own initializer *)
  GPC_Init_Environment (GPC_GetStartEnvironment (StartEnvironment));
  GPC_Init_Arguments
end;

end.
