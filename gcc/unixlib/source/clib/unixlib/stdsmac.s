;----------------------------------------------------------------------------
;
; $Source: $
; $Date: $
; $Revision: $
; $State: $
; $Author: $
;
;----------------------------------------------------------------------------

; ------------------------------------------------------------------------
; Original code (stdcmac.h/stdsmac.h) was downloaded from
; <URL:http://www.arm.com/support/faqdev/1208.html>
; Changes made:
;   - Changed undocumented objasm IF/ELSE/ENDIF into an objasm/as compatible
;     [ | ] construction.
;   - _spaces_remove removes TABs too (oddly enough, this is not needed for
;     objasm).
;   - _arg_remove supports TABs too (oddly enough, this is not needed for
;     objasm).
;   - stdsmac.h renamed to stdsmac.s.
;
; ------------------------------------------------------------------------

; Standard Assembler Macros
; Dominic Symes

; Global vars for arg parsing

	GBLS	_arg0
	GBLS	_arg1

; _spaces_remove
; remove start and end spaces from global variable wstring

        MACRO
        _spaces_remove $wstring
	LCLL	_ok
	LCLS	_chr
_ok	SETL	{TRUE}
	WHILE	_ok
_chr	SETS	("*" :CC: $wstring) :RIGHT: 1
	[ _chr <> " " :LAND: _chr <> :CHR:9
_ok	SETL	{FALSE}
	|
$wstring    SETS ($wstring :LEFT: (:LEN: $wstring - 1))
	]
	WEND

_ok	SETL	{TRUE}
	WHILE	_ok
_chr	SETS	($wstring :CC: "*") :LEFT: 1
	[ _chr <> " " :LAND: _chr <> :CHR:9
_ok	SETL	{FALSE}
	|
$wstring    SETS ($wstring :RIGHT: (:LEN: $wstring - 1))
	]
        WEND
        MEND

; _lbracket_remove
; Attempt to remove a single left bracket - error if not there

	MACRO
	_lbracket_remove $s
	ASSERT	$s:LEFT:1 = "("
$s	SETS	$s:RIGHT:(:LEN:$s-1)
	_spaces_remove $s
	MEND

; _rbracket_remove
; Attempt to remove a single right bracket - error if not there
; then removes excess spaces

	MACRO
	_rbracket_remove $s
	ASSERT	$s:RIGHT:1 = ")"
$s	SETS	$s:LEFT:(:LEN:$s-1)
	_spaces_remove $s
	MEND

; _comment_remove
; Remove any comment from line end and then strip any spaces

	MACRO
	_comment_remove $s
	_spaces_remove $s
	[ (("**":CC:$s):RIGHT:2) = "*/"
	  WHILE ($s:RIGHT:2) <> "/*"
$s	    SETS $s:LEFT:(:LEN:$s-1)
	  WEND
	]
$s	SETS $s:LEFT:(:LEN:$s-2)
	_spaces_remove $s
	MEND

; _arg_remove
; Pull an argument from the front of a spaces stripped string

	MACRO
	_arg_remove $s,$arg
	LCLA	_arglen
	LCLL	_ok
_arglen	SETA	0
_ok	SETL	{TRUE}
	WHILE	_ok
	  [ _arglen >= :LEN:$s
_ok	    SETL {FALSE}	; break if used up input string
	  |
$arg	    SETS ($s:LEFT:(_arglen+1)):RIGHT:1	; next character
	    [ $arg = " " :LOR: $arg = :CHR:9
_ok	      SETL {FALSE}
	    |
_arglen	      SETA _arglen+1
	    ]
	  ]
	WEND
$arg	SETS $s:LEFT:_arglen
$s	SETS $s:RIGHT:(:LEN:$s-_arglen)
	_spaces_remove $s
	MEND

; define
; Purpose: Allow #defines for common C/Assembler headers
; Syntax : #<space/tab>define<spaces><symbol><spaces><value></*comment*/>

	MACRO
$la	define $a
_arg0	SETS	"$a"
	ASSERT	"$la"="#"	; syntax: # define fred 1
	_comment_remove _arg0
	_arg_remove _arg0,_arg1
$_arg1	EQU	$_arg0
	MEND


; COMMENT
; Purpose: Allow comments in common C/Assembler headers
; Syntax : COMMENT <anything you like!>

	MACRO
	COMMENT $a,$b,$c,$d,$e,$f,$g,$h
	MEND

; local labels use label$l to get a local label and LOCAL to start a new
; area

	GBLA LocalCount
	GBLS l
LocalCount SETA 1
l	SETS "x$LocalCount"

; increment local variable number

	MACRO
	LOCAL
LocalCount SETA LocalCount+1
l	SETS "x$LocalCount"
	MEND

	END
