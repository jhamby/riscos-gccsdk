ffestrSecond
ffestr_second (ffelexToken t)
{
  char *p;
  int c;

  p = ffelex_token_text (t);

  assert (ffelex_token_type (t) == FFELEX_typeNAME);

  switch (ffelex_token_length (t))
    {
    case 2:
      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "IF", "if", "If")) == 0)
	return FFESTR_secondIF;
      else if (c < 0)
	{
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "DO", "do", "Do")) == 0)
	    return FFESTR_secondDO;
	}
      else
	{
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "TO", "to", "To")) == 0)
	    return FFESTR_secondTO;
	}
      break;
    case 4:
      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "FILE", "file", "File")) == 0)
	return FFESTR_secondFILE;
      else if (c < 0)
	{
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "CASE", "case", "Case")) == 0)
	    return FFESTR_secondCASE;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "BYTE", "byte", "Byte")) == 0)
		return FFESTR_secondBYTE;
	    }
	  else
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "DATA", "data", "Data")) == 0)
		return FFESTR_secondDATA;
	    }
	}
      else
	{
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "REAL", "real", "Real")) == 0)
	    return FFESTR_secondREAL;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "NONE", "none", "None")) == 0)
		return FFESTR_secondNONE;
	    }
	  else
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "WORD", "word", "Word")) == 0)
		return FFESTR_secondWORD;
	    }
	}
      break;
    case 5:
      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "WHILE", "while", "While")) == 0)
	return FFESTR_secondWHILE;
      else if (c < 0)
	{
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "BLOCK", "block", "Block")) == 0)
	    return FFESTR_secondBLOCK;
	}
      break;
    case 6:
      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "SELECT", "select", "Select")) == 0)
	return FFESTR_secondSELECT;
      else if (c < 0)
	{
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "DOUBLE", "double", "Double")) == 0)
	    return FFESTR_secondDOUBLE;
	}
      break;
    case 7:
      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "INTEGER", "integer", "Integer")) == 0)
	return FFESTR_secondINTEGER;
      else if (c < 0)
	{
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "DEFAULT", "default", "Default")) == 0)
	    return FFESTR_secondDEFAULT;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "COMPLEX", "complex", "Complex")) == 0)
		return FFESTR_secondCOMPLEX;
	    }
	}
      else
	{
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "PROGRAM", "program", "Program")) == 0)
	    return FFESTR_secondPROGRAM;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "LOGICAL", "logical", "Logical")) == 0)
		return FFESTR_secondLOGICAL;
	    }
	}
      break;
    case 8:
      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "FUNCTION", "function", "Function")) == 0)
	return FFESTR_secondFUNCTION;
      break;
    case 9:
      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "CHARACTER", "character", "Character")) == 0)
	return FFESTR_secondCHARACTER;
      else if (c < 0)
	{
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "BLOCKDATA", "blockdata", "BlockData")) == 0)
	    return FFESTR_secondBLOCKDATA;
	}
      else
	{
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "PRECISION", "precision", "Precision")) == 0)
	    return FFESTR_secondPRECISION;
	}
      break;
    case 10:
      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "SUBROUTINE", "subroutine", "Subroutine")) == 0)
	return FFESTR_secondSUBROUTINE;
      break;
    case 13:
      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "DOUBLECOMPLEX", "doublecomplex", "DoubleComplex")) == 0)
	return FFESTR_secondDOUBLECOMPLEX;
      break;
    case 15:
      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "DOUBLEPRECISION", "doubleprecision", "DoublePrecision")) == 0)
	return FFESTR_secondDOUBLEPRECISION;
      break;
    }

  return FFESTR_secondNone;
}
