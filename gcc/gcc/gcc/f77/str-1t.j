ffestrFirst
ffestr_first (ffelexToken t)
{
  char *p;
  int c;

  p = ffelex_token_text (t);

  if (ffelex_token_type (t) == FFELEX_typeNAME)
    {
      switch (ffelex_token_length (t))
	{
	case 2:
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "GO", "go", "Go")) == 0)
	    return FFESTR_firstGO;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "DO", "do", "Do")) == 0)
		return FFESTR_firstDO;
	    }
	  else
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "IF", "if", "If")) == 0)
		return FFESTR_firstIF;
	    }
	  break;
	case 3:
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "END", "end", "End")) == 0)
	    return FFESTR_firstEND;
	  break;
	case 4:
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "OPEN", "open", "Open")) == 0)
	    return FFESTR_firstOPEN;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "ELSE", "else", "Else")) == 0)
		return FFESTR_firstELSE;
	      else if (c < 0)
		{
		  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "CASE", "case", "Case")) == 0)
		    return FFESTR_firstCASE;
		  else if (c < 0)
		    {
		      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "CALL", "call", "Call")) == 0)
			return FFESTR_firstCALL;
		      else if (c < 0)
			{
			  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "BYTE", "byte", "Byte")) == 0)
			    return FFESTR_firstBYTE;
			}
		    }
		  else
		    {
		      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "DATA", "data", "Data")) == 0)
			return FFESTR_firstDATA;
		    }
		}
	      else
		{
		  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "FIND", "find", "Find")) == 0)
		    return FFESTR_firstFIND;
		  else if (c < 0)
		    {
		      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "EXIT", "exit", "Exit")) == 0)
			return FFESTR_firstEXIT;
		    }
		  else
		    {
		      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "GOTO", "goto", "GoTo")) == 0)
			return FFESTR_firstGOTO;
		    }
		}
	    }
	  else
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "STOP", "stop", "Stop")) == 0)
		return FFESTR_firstSTOP;
	      else if (c < 0)
		{
		  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "REAL", "real", "Real")) == 0)
		    return FFESTR_firstREAL;
		  else if (c < 0)
		    {
		      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "READ", "read", "Read")) == 0)
			return FFESTR_firstREAD;
		    }
		  else
		    {
		      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "SAVE", "save", "Save")) == 0)
			return FFESTR_firstSAVE;
		    }
		}
	      else
		{
		  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "TYPE", "type", "Type")) == 0)
		    return FFESTR_firstTYPE;
		  else if (c < 0)
		    {
		      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "THEN", "then", "Then")) == 0)
			return FFESTR_firstTHEN;
		    }
		  else
		    {
		      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "WORD", "word", "Word")) == 0)
			return FFESTR_firstWORD;
		    }
		}
	    }
	  break;
	case 5:
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "ENDIF", "endif", "EndIf")) == 0)
	    return FFESTR_firstENDIF;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "CYCLE", "cycle", "Cycle")) == 0)
		return FFESTR_firstCYCLE;
	      else if (c < 0)
		{
		  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "CLOSE", "close", "Close")) == 0)
		    return FFESTR_firstCLOSE;
		  else if (c < 0)
		    {
		      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "BLOCK", "block", "Block")) == 0)
			return FFESTR_firstBLOCK;
		    }
		}
	      else
		{
		  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "ENDDO", "enddo", "EndDo")) == 0)
		    return FFESTR_firstENDDO;
		}
	    }
	  else
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "PRINT", "print", "Print")) == 0)
		return FFESTR_firstPRINT;
	      else if (c < 0)
		{
		  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "PAUSE", "pause", "Pause")) == 0)
		    return FFESTR_firstPAUSE;
		  else if (c < 0)
		    {
		      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "ENTRY", "entry", "Entry")) == 0)
			return FFESTR_firstENTRY;
		    }
		}
	      else
		{
		  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "WRITE", "write", "Write")) == 0)
		    return FFESTR_firstWRITE;
		}
	    }
	  break;
	case 6:
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "ELSEIF", "elseif", "ElseIf")) == 0)
	    return FFESTR_firstELSEIF;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "DEFINE", "define", "Define")) == 0)
		return FFESTR_firstDEFINE;
	      else if (c < 0)
		{
		  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "COMMON", "common", "Common")) == 0)
		    return FFESTR_firstCOMMON;
		  else if (c < 0)
		    {
		      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "ASSIGN", "assign", "Assign")) == 0)
			return FFESTR_firstASSIGN;
		    }
		  else
		    {
		      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "DECODE", "decode", "Decode")) == 0)
			return FFESTR_firstDECODE;
		    }
		}
	      else
		{
		  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "DOUBLE", "double", "Double")) == 0)
		    return FFESTR_firstDBL;
		  else if (c < 0)
		    {
		      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "DELETE", "delete", "Delete")) == 0)
			return FFESTR_firstDELETE;
		    }
		}
	    }
	  else
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "RETURN", "return", "Return")) == 0)
		return FFESTR_firstRETURN;
	      else if (c < 0)
		{
		  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "FORMAT", "format", "Format")) == 0)
		    return FFESTR_firstFORMAT;
		  else if (c < 0)
		    {
		      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "ENCODE", "encode", "Encode")) == 0)
			return FFESTR_firstENCODE;
		    }
		}
	      else
		{
		  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "SELECT", "select", "Select")) == 0)
		    return FFESTR_firstSELECT;
		  else if (c < 0)
		    {
		      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "REWIND", "rewind", "Rewind")) == 0)
			return FFESTR_firstREWIND;
		    }
		}
	    }
	  break;
	case 7:
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "INQUIRE", "inquire", "Inquire")) == 0)
	    return FFESTR_firstINQUIRE;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "ENDFILE", "endfile", "EndFile")) == 0)
		return FFESTR_firstENDFILE;
	      else if (c < 0)
		{
		  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "DOWHILE", "dowhile", "DoWhile")) == 0)
		    return FFESTR_firstDOWHILE;
		  else if (c < 0)
		    {
		      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "COMPLEX", "complex", "Complex")) == 0)
			return FFESTR_firstCMPLX;
		    }
		}
	      else
		{
		  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "INCLUDE", "include", "Include")) == 0)
		    return FFESTR_firstINCLUDE;
		}
	    }
	  else
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "PROGRAM", "program", "Program")) == 0)
		return FFESTR_firstPROGRAM;
	      else if (c < 0)
		{
		  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "LOGICAL", "logical", "Logical")) == 0)
		    return FFESTR_firstLGCL;
		  else if (c < 0)
		    {
		      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "INTEGER", "integer", "Integer")) == 0)
			return FFESTR_firstINTGR;
		    }
		}
	      else
		{
		  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "VIRTUAL", "virtual", "Virtual")) == 0)
		    return FFESTR_firstVIRTUAL;
		}
	    }
	  break;
	case 8:
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "FUNCTION", "function", "Function")) == 0)
	    return FFESTR_firstFUNCTION;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "ENDBLOCK", "endblock", "EndBlock")) == 0)
		return FFESTR_firstENDBLOCK;
	      else if (c < 0)
		{
		  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "CONTINUE", "continue", "Continue")) == 0)
		    return FFESTR_firstCONTINUE;
		}
	      else
		{
		  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "EXTERNAL", "external", "External")) == 0)
		    return FFESTR_firstEXTERNAL;
		}
	    }
	  else
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "NAMELIST", "namelist", "NameList")) == 0)
		return FFESTR_firstNAMELIST;
	      else if (c < 0)
		{
		  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "IMPLICIT", "implicit", "Implicit")) == 0)
		    return FFESTR_firstIMPLICIT;
		}
	      else
		{
		  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "VOLATILE", "volatile", "Volatile")) == 0)
		    return FFESTR_firstVOLATILE;
		}
	    }
	  break;
	case 9:
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "DIMENSION", "dimension", "Dimension")) == 0)
	    return FFESTR_firstDIMENSION;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "BLOCKDATA", "blockdata", "BlockData")) == 0)
		return FFESTR_firstBLOCKDATA;
	      else if (c < 0)
		{
		  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "BACKSPACE", "backspace", "Backspace")) == 0)
		    return FFESTR_firstBACKSPACE;
		}
	      else
		{
		  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "CHARACTER", "character", "Character")) == 0)
		    return FFESTR_firstCHRCTR;
		}
	    }
	  else
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "INTRINSIC", "intrinsic", "Intrinsic")) == 0)
		return FFESTR_firstINTRINSIC;
	      else if (c < 0)
		{
		  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "ENDSELECT", "endselect", "EndSelect")) == 0)
		    return FFESTR_firstENDSELECT;
		}
	      else
		{
		  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "PARAMETER", "parameter", "Parameter")) == 0)
		    return FFESTR_firstPARAMETER;
		}
	    }
	  break;
	case 10:
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "SELECTCASE", "selectcase", "SelectCase")) == 0)
	    return FFESTR_firstSELECTCASE;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "ENDPROGRAM", "endprogram", "EndProgram")) == 0)
		return FFESTR_firstENDPROGRAM;
	    }
	  else
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "SUBROUTINE", "subroutine", "Subroutine")) == 0)
		return FFESTR_firstSUBROUTINE;
	    }
	  break;
	case 11:
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "ENDFUNCTION", "endfunction", "EndFunction")) == 0)
	    return FFESTR_firstENDFUNCTION;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "CASEDEFAULT", "casedefault", "CaseDefault")) == 0)
		return FFESTR_firstCASEDEFAULT;
	    }
	  else
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "EQUIVALENCE", "equivalence", "Equivalence")) == 0)
		return FFESTR_firstEQUIVALENCE;
	    }
	  break;
	case 12:
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "ENDBLOCKDATA", "endblockdata", "EndBlockData")) == 0)
	    return FFESTR_firstENDBLOCKDATA;
	  break;
	case 13:
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "ENDSUBROUTINE", "endsubroutine", "EndSubroutine")) == 0)
	    return FFESTR_firstENDSUBROUTINE;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "DOUBLECOMPLEX", "doublecomplex", "DoubleComplex")) == 0)
		return FFESTR_firstDBLCMPLX;
	    }
	  break;
	case 15:
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "DOUBLEPRECISION", "doubleprecision", "DoublePrecision")) == 0)
	    return FFESTR_firstDBLPRCSN;
	  break;
	}
      return FFESTR_firstNone;
    }

  assert (ffelex_token_type (t) == FFELEX_typeNAMES);

  switch (ffelex_token_length (t))
    {
    default:
    case 15:
      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "DOUBLEPRECISION", "doubleprecision", "DoublePrecision", 15)) == 0)
	return FFESTR_firstDBLPRCSN;
    case 14:
    case 13:
      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "ENDSUBROUTINE", "endsubroutine", "EndSubroutine", 13)) == 0)
	return FFESTR_firstENDSUBROUTINE;
      else if (c < 0)
	{
	  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "DOUBLECOMPLEX", "doublecomplex", "DoubleComplex", 13)) == 0)
	    return FFESTR_firstDBLCMPLX;
	}
    case 12:
      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "ENDBLOCKDATA", "endblockdata", "EndBlockData", 12)) == 0)
	return FFESTR_firstENDBLOCKDATA;
    case 11:
      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "ENDFUNCTION", "endfunction", "EndFunction", 11)) == 0)
	return FFESTR_firstENDFUNCTION;
      else if (c < 0)
	{
	  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "CASEDEFAULT", "casedefault", "CaseDefault", 11)) == 0)
	    return FFESTR_firstCASEDEFAULT;
	}
      else
	{
	  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "EQUIVALENCE", "equivalence", "Equivalence", 11)) == 0)
	    return FFESTR_firstEQUIVALENCE;
	}
    case 10:
      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "SELECTCASE", "selectcase", "SelectCase", 10)) == 0)
	return FFESTR_firstSELECTCASE;
      else if (c < 0)
	{
	  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "ENDPROGRAM", "endprogram", "EndProgram", 10)) == 0)
	    return FFESTR_firstENDPROGRAM;
	}
      else
	{
	  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "SUBROUTINE", "subroutine", "Subroutine", 10)) == 0)
	    return FFESTR_firstSUBROUTINE;
	}
    case 9:
      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "DIMENSION", "dimension", "Dimension", 9)) == 0)
	return FFESTR_firstDIMENSION;
      else if (c < 0)
	{
	  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "BLOCKDATA", "blockdata", "BlockData", 9)) == 0)
	    return FFESTR_firstBLOCKDATA;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "BACKSPACE", "backspace", "Backspace", 9)) == 0)
		return FFESTR_firstBACKSPACE;
	    }
	  else
	    {
	      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "CHARACTER", "character", "Character", 9)) == 0)
		return FFESTR_firstCHRCTR;
	    }
	}
      else
	{
	  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "INTRINSIC", "intrinsic", "Intrinsic", 9)) == 0)
	    return FFESTR_firstINTRINSIC;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "ENDSELECT", "endselect", "EndSelect", 9)) == 0)
		return FFESTR_firstENDSELECT;
	    }
	  else
	    {
	      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "PARAMETER", "parameter", "Parameter", 9)) == 0)
		return FFESTR_firstPARAMETER;
	    }
	}
    case 8:
      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "FUNCTION", "function", "Function", 8)) == 0)
	return FFESTR_firstFUNCTION;
      else if (c < 0)
	{
	  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "ENDBLOCK", "endblock", "EndBlock", 8)) == 0)
	    return FFESTR_firstENDBLOCK;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "CONTINUE", "continue", "Continue", 8)) == 0)
		return FFESTR_firstCONTINUE;
	    }
	  else
	    {
	      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "EXTERNAL", "external", "External", 8)) == 0)
		return FFESTR_firstEXTERNAL;
	    }
	}
      else
	{
	  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "NAMELIST", "namelist", "NameList", 8)) == 0)
	    return FFESTR_firstNAMELIST;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "IMPLICIT", "implicit", "Implicit", 8)) == 0)
		return FFESTR_firstIMPLICIT;
	    }
	  else
	    {
	      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "VOLATILE", "volatile", "Volatile", 8)) == 0)
		return FFESTR_firstVOLATILE;
	    }
	}
    case 7:
      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "INQUIRE", "inquire", "Inquire", 7)) == 0)
	return FFESTR_firstINQUIRE;
      else if (c < 0)
	{
	  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "ENDFILE", "endfile", "EndFile", 7)) == 0)
	    return FFESTR_firstENDFILE;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "DOWHILE", "dowhile", "DoWhile", 7)) == 0)
		return FFESTR_firstDOWHILE;
	      else if (c < 0)
		{
		  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "COMPLEX", "complex", "Complex", 7)) == 0)
		    return FFESTR_firstCMPLX;
		}
	    }
	  else
	    {
	      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "INCLUDE", "include", "Include", 7)) == 0)
		return FFESTR_firstINCLUDE;
	    }
	}
      else
	{
	  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "PROGRAM", "program", "Program", 7)) == 0)
	    return FFESTR_firstPROGRAM;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "LOGICAL", "logical", "Logical", 7)) == 0)
		return FFESTR_firstLGCL;
	      else if (c < 0)
		{
		  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "INTEGER", "integer", "Integer", 7)) == 0)
		    return FFESTR_firstINTGR;
		}
	    }
	  else
	    {
	      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "VIRTUAL", "virtual", "Virtual", 7)) == 0)
		return FFESTR_firstVIRTUAL;
	    }
	}
    case 6:
      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "ELSEIF", "elseif", "ElseIf", 6)) == 0)
	return FFESTR_firstELSEIF;
      else if (c < 0)
	{
	  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "DEFINE", "define", "Define", 6)) == 0)
	    return FFESTR_firstDEFINE;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "COMMON", "common", "Common", 6)) == 0)
		return FFESTR_firstCOMMON;
	      else if (c < 0)
		{
		  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "ASSIGN", "assign", "Assign", 6)) == 0)
		    return FFESTR_firstASSIGN;
		}
	      else
		{
		  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "DECODE", "decode", "Decode", 6)) == 0)
		    return FFESTR_firstDECODE;
		}
	    }
	  else
	    {
	      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "DOUBLE", "double", "Double", 6)) == 0)
		return FFESTR_firstDBL;
	      else if (c < 0)
		{
		  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "DELETE", "delete", "Delete", 6)) == 0)
		    return FFESTR_firstDELETE;
		}
	    }
	}
      else
	{
	  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "RETURN", "return", "Return", 6)) == 0)
	    return FFESTR_firstRETURN;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "FORMAT", "format", "Format", 6)) == 0)
		return FFESTR_firstFORMAT;
	      else if (c < 0)
		{
		  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "ENCODE", "encode", "Encode", 6)) == 0)
		    return FFESTR_firstENCODE;
		}
	    }
	  else
	    {
	      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "SELECT", "select", "Select", 6)) == 0)
		return FFESTR_firstSELECT;
	      else if (c < 0)
		{
		  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "REWIND", "rewind", "Rewind", 6)) == 0)
		    return FFESTR_firstREWIND;
		}
	    }
	}
    case 5:
      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "ENDIF", "endif", "EndIf", 5)) == 0)
	return FFESTR_firstENDIF;
      else if (c < 0)
	{
	  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "CYCLE", "cycle", "Cycle", 5)) == 0)
	    return FFESTR_firstCYCLE;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "CLOSE", "close", "Close", 5)) == 0)
		return FFESTR_firstCLOSE;
	      else if (c < 0)
		{
		  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "BLOCK", "block", "Block", 5)) == 0)
		    return FFESTR_firstBLOCK;
		}
	    }
	  else
	    {
	      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "ENDDO", "enddo", "EndDo", 5)) == 0)
		return FFESTR_firstENDDO;
	    }
	}
      else
	{
	  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "PRINT", "print", "Print", 5)) == 0)
	    return FFESTR_firstPRINT;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "PAUSE", "pause", "Pause", 5)) == 0)
		return FFESTR_firstPAUSE;
	      else if (c < 0)
		{
		  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "ENTRY", "entry", "Entry", 5)) == 0)
		    return FFESTR_firstENTRY;
		}
	    }
	  else
	    {
	      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "WRITE", "write", "Write", 5)) == 0)
		return FFESTR_firstWRITE;
	    }
	}
    case 4:
      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "OPEN", "open", "Open", 4)) == 0)
	return FFESTR_firstOPEN;
      else if (c < 0)
	{
	  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "ELSE", "else", "Else", 4)) == 0)
	    return FFESTR_firstELSE;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "CASE", "case", "Case", 4)) == 0)
		return FFESTR_firstCASE;
	      else if (c < 0)
		{
		  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "CALL", "call", "Call", 4)) == 0)
		    return FFESTR_firstCALL;
		  else if (c < 0)
		    {
		      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "BYTE", "byte", "Byte", 4)) == 0)
			return FFESTR_firstBYTE;
		    }
		}
	      else
		{
		  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "DATA", "data", "Data", 4)) == 0)
		    return FFESTR_firstDATA;
		}
	    }
	  else
	    {
	      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "FIND", "find", "Find", 4)) == 0)
		return FFESTR_firstFIND;
	      else if (c < 0)
		{
		  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "EXIT", "exit", "Exit", 4)) == 0)
		    return FFESTR_firstEXIT;
		}
	      else
		{
		  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "GOTO", "goto", "GoTo", 4)) == 0)
		    return FFESTR_firstGOTO;
		}
	    }
	}
      else
	{
	  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "STOP", "stop", "Stop", 4)) == 0)
	    return FFESTR_firstSTOP;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "REAL", "real", "Real", 4)) == 0)
		return FFESTR_firstREAL;
	      else if (c < 0)
		{
		  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "READ", "read", "Read", 4)) == 0)
		    return FFESTR_firstREAD;
		}
	      else
		{
		  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "SAVE", "save", "Save", 4)) == 0)
		    return FFESTR_firstSAVE;
		}
	    }
	  else
	    {
	      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "TYPE", "type", "Type", 4)) == 0)
		return FFESTR_firstTYPE;
	      else if (c < 0)
		{
		  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "THEN", "then", "Then", 4)) == 0)
		    return FFESTR_firstTHEN;
		}
	      else
		{
		  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "WORD", "word", "Word", 4)) == 0)
		    return FFESTR_firstWORD;
		}
	    }
	}
    case 3:
      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "END", "end", "End", 3)) == 0)
	return FFESTR_firstEND;
    case 2:
      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "GO", "go", "Go", 2)) == 0)
	return FFESTR_firstGO;
      else if (c < 0)
	{
	  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "DO", "do", "Do", 2)) == 0)
	    return FFESTR_firstDO;
	}
      else
	{
	  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "IF", "if", "If", 2)) == 0)
	    return FFESTR_firstIF;
	}
    case 1:
      ;
    }

  return FFESTR_firstNone;
}
