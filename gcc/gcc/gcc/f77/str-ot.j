ffestrOther
ffestr_other (ffelexToken t)
{
  char *p;
  int c;

  p = ffelex_token_text (t);

  if (ffelex_token_type (t) == FFELEX_typeNAME)
    {
      switch (ffelex_token_length (t))
	{
	case 2:
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "LE", "le", "LE")) == 0)
	    return FFESTR_otherLE;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "GT", "gt", "GT")) == 0)
		return FFESTR_otherGT;
	      else if (c < 0)
		{
		  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "GE", "ge", "GE")) == 0)
		    return FFESTR_otherGE;
		  else if (c < 0)
		    {
		      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "EQ", "eq", "Eq")) == 0)
			return FFESTR_otherEQ;
		    }
		}
	      else
		{
		  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "IN", "in", "In")) == 0)
		    return FFESTR_otherIN;
		}
	    }
	  else
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "NE", "ne", "NE")) == 0)
		return FFESTR_otherNE;
	      else if (c < 0)
		{
		  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "LT", "lt", "LT")) == 0)
		    return FFESTR_otherLT;
		}
	      else
		{
		  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "OR", "or", "Or")) == 0)
		    return FFESTR_otherOR;
		}
	    }
	  break;
	case 3:
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "NOT", "not", "Not")) == 0)
	    return FFESTR_otherNOT;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "EQV", "eqv", "Eqv")) == 0)
		return FFESTR_otherEQV;
	      else if (c < 0)
		{
		  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "AND", "and", "And")) == 0)
		    return FFESTR_otherAND;
		}
	      else
		{
		  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "LEN", "len", "Len")) == 0)
		    return FFESTR_otherLEN;
		}
	    }
	  else
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "XOR", "xor", "XOr")) == 0)
		return FFESTR_otherXOR;
	      else if (c < 0)
		{
		  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "OUT", "out", "Out")) == 0)
		    return FFESTR_otherOUT;
		}
	    }
	  break;
	case 4:
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "NEQV", "neqv", "NEqv")) == 0)
	    return FFESTR_otherNEQV;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "KIND", "kind", "Kind")) == 0)
		return FFESTR_otherKIND;
	    }
	  else
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "TRUE", "true", "True")) == 0)
		return FFESTR_otherTRUE;
	    }
	  break;
	case 5:
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "INOUT", "inout", "InOut")) == 0)
	    return FFESTR_otherINOUT;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "FALSE", "false", "False")) == 0)
		return FFESTR_otherFALSE;
	    }
	  break;
	case 6:
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "RESULT", "result", "Result")) == 0)
	    return FFESTR_otherRESULT;
	  break;
	}
      return FFESTR_otherNone;
    }

  assert (ffelex_token_type (t) == FFELEX_typeNAMES);

  switch (ffelex_token_length (t))
    {
    default:
    case 6:
      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "RESULT", "result", "Result", 6)) == 0)
	return FFESTR_otherRESULT;
    case 5:
      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "INOUT", "inout", "InOut", 5)) == 0)
	return FFESTR_otherINOUT;
      else if (c < 0)
	{
	  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "FALSE", "false", "False", 5)) == 0)
	    return FFESTR_otherFALSE;
	}
    case 4:
      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "NEQV", "neqv", "NEqv", 4)) == 0)
	return FFESTR_otherNEQV;
      else if (c < 0)
	{
	  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "KIND", "kind", "Kind", 4)) == 0)
	    return FFESTR_otherKIND;
	}
      else
	{
	  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "TRUE", "true", "True", 4)) == 0)
	    return FFESTR_otherTRUE;
	}
    case 3:
      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "NOT", "not", "Not", 3)) == 0)
	return FFESTR_otherNOT;
      else if (c < 0)
	{
	  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "EQV", "eqv", "Eqv", 3)) == 0)
	    return FFESTR_otherEQV;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "AND", "and", "And", 3)) == 0)
		return FFESTR_otherAND;
	    }
	  else
	    {
	      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "LEN", "len", "Len", 3)) == 0)
		return FFESTR_otherLEN;
	    }
	}
      else
	{
	  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "XOR", "xor", "XOr", 3)) == 0)
	    return FFESTR_otherXOR;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "OUT", "out", "Out", 3)) == 0)
		return FFESTR_otherOUT;
	    }
	}
    case 2:
      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "LE", "le", "LE", 2)) == 0)
	return FFESTR_otherLE;
      else if (c < 0)
	{
	  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "GT", "gt", "GT", 2)) == 0)
	    return FFESTR_otherGT;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "GE", "ge", "GE", 2)) == 0)
		return FFESTR_otherGE;
	      else if (c < 0)
		{
		  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "EQ", "eq", "Eq", 2)) == 0)
		    return FFESTR_otherEQ;
		}
	    }
	  else
	    {
	      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "IN", "in", "In", 2)) == 0)
		return FFESTR_otherIN;
	    }
	}
      else
	{
	  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "NE", "ne", "NE", 2)) == 0)
	    return FFESTR_otherNE;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "LT", "lt", "LT", 2)) == 0)
		return FFESTR_otherLT;
	    }
	  else
	    {
	      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "OR", "or", "Or", 2)) == 0)
		return FFESTR_otherOR;
	    }
	}
    case 1:
      ;
    }

  return FFESTR_otherNone;
}
