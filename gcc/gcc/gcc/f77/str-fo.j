ffestrFormat
ffestr_format (ffelexToken t)
{
  char *p;
  int c;

  p = ffelex_token_text (t);

  assert (ffelex_token_type (t) == FFELEX_typeNAMES);

  switch (ffelex_token_length (t))
    {
    default:
    case 3:
      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "PEN", "pen", "PEn", 3)) == 0)
	return FFESTR_formatPEN;
    case 2:
      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "PF", "pf", "PF", 2)) == 0)
	return FFESTR_formatPF;
      else if (c < 0)
	{
	  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "EN", "en", "En", 2)) == 0)
	    return FFESTR_formatEN;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "BZ", "bz", "BZ", 2)) == 0)
		return FFESTR_formatBZ;
	      else if (c < 0)
		{
		  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "BN", "bn", "BN", 2)) == 0)
		    return FFESTR_formatBN;
		}
	    }
	  else
	    {
	      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "PE", "pe", "PE", 2)) == 0)
		return FFESTR_formatPE;
	      else if (c < 0)
		{
		  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "PD", "pd", "PD", 2)) == 0)
		    return FFESTR_formatPD;
		}
	    }
	}
      else
	{
	  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "SS", "ss", "SS", 2)) == 0)
	    return FFESTR_formatSS;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "SP", "sp", "SP", 2)) == 0)
		return FFESTR_formatSP;
	      else if (c < 0)
		{
		  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "PG", "pg", "PG", 2)) == 0)
		    return FFESTR_formatPG;
		}
	    }
	  else
	    {
	      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "TR", "tr", "TR", 2)) == 0)
		return FFESTR_formatTR;
	      else if (c < 0)
		{
		  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "TL", "tl", "TL", 2)) == 0)
		    return FFESTR_formatTL;
		}
	    }
	}
    case 1:
      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "L", "l", "L", 1)) == 0)
	return FFESTR_formatL;
      else if (c < 0)
	{
	  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "E", "e", "E", 1)) == 0)
	    return FFESTR_formatE;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "B", "b", "B", 1)) == 0)
		return FFESTR_formatB;
	      else if (c < 0)
		{
		  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "A", "a", "A", 1)) == 0)
		    return FFESTR_formatA;
		  else if (c < 0)
		    {
		      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "$", "$", "$", 1)) == 0)
			return FFESTR_formatDOLLAR;
		    }
		}
	      else
		{
		  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "D", "d", "D", 1)) == 0)
		    return FFESTR_formatD;
		}
	    }
	  else
	    {
	      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "H", "h", "H", 1)) == 0)
		return FFESTR_formatH;
	      else if (c < 0)
		{
		  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "G", "g", "G", 1)) == 0)
		    return FFESTR_formatG;
		  else if (c < 0)
		    {
		      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "F", "f", "F", 1)) == 0)
			return FFESTR_formatF;
		    }
		}
	      else
		{
		  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "I", "i", "I", 1)) == 0)
		    return FFESTR_formatI;
		}
	    }
	}
      else
	{
	  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "R", "r", "R", 1)) == 0)
	    return FFESTR_formatR;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "P", "p", "P", 1)) == 0)
		return FFESTR_formatP;
	      else if (c < 0)
		{
		  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "O", "o", "O", 1)) == 0)
		    return FFESTR_formatO;
		  else if (c < 0)
		    {
		      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "N", "n", "N", 1)) == 0)
			return FFESTR_formatN;
		    }
		}
	      else
		{
		  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "Q", "q", "Q", 1)) == 0)
		    return FFESTR_formatQ;
		}
	    }
	  else
	    {
	      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "X", "x", "X", 1)) == 0)
		return FFESTR_formatX;
	      else if (c < 0)
		{
		  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "T", "t", "T", 1)) == 0)
		    return FFESTR_formatT;
		  else if (c < 0)
		    {
		      if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "S", "s", "S", 1)) == 0)
			return FFESTR_formatS;
		    }
		}
	      else
		{
		  if ((c = ffesrc_strncmp_2c (ffe_case_match (), p, "Z", "z", "Z", 1)) == 0)
		    return FFESTR_formatZ;
		}
	    }
	}
    }

  return FFESTR_formatNone;
}
