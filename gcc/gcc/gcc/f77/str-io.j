ffestrGenio
ffestr_genio (ffelexToken t)
{
  char *p;
  int c;

  p = ffelex_token_text (t);

  assert (ffelex_token_type (t) == FFELEX_typeNAME);

  switch (ffelex_token_length (t))
    {
    case 3:
      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "FMT", "fmt", "Fmt")) == 0)
	return FFESTR_genioFMT;
      else if (c < 0)
	{
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "EOR", "eor", "EoR")) == 0)
	    return FFESTR_genioEOR;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "END", "end", "End")) == 0)
		return FFESTR_genioEND;
	    }
	  else
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "ERR", "err", "Err")) == 0)
		return FFESTR_genioERR;
	    }
	}
      else
	{
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "NML", "nml", "Nml")) == 0)
	    return FFESTR_genioNML;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "KEY", "key", "Key")) == 0)
		return FFESTR_genioKEY;
	    }
	  else
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "REC", "rec", "Rec")) == 0)
		return FFESTR_genioREC;
	    }
	}
      break;
    case 4:
      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "SIZE", "size", "Size")) == 0)
	return FFESTR_genioSIZE;
      else if (c < 0)
	{
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "DISP", "disp", "Disp")) == 0)
	    return FFESTR_genioDISP;
	}
      else
	{
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "UNIT", "unit", "Unit")) == 0)
	    return FFESTR_genioUNIT;
	}
      break;
    case 5:
      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "KEYGT", "keygt", "KeyGT")) == 0)
	return FFESTR_genioKEYGT;
      else if (c < 0)
	{
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "KEYGE", "keyge", "KeyGE")) == 0)
	    return FFESTR_genioKEYGE;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "KEYEQ", "keyeq", "KeyEQ")) == 0)
		return FFESTR_genioKEYEQ;
	    }
	}
      else
	{
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "NULLS", "nulls", "Nulls")) == 0)
	    return FFESTR_genioNULLS;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "KEYID", "keyid", "KeyID")) == 0)
		return FFESTR_genioKEYID;
	    }
	}
      break;
    case 6:
      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "STATUS", "status", "Status")) == 0)
	return FFESTR_genioSTATUS;
      else if (c < 0)
	{
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "IOSTAT", "iostat", "IOStat")) == 0)
	    return FFESTR_genioIOSTAT;
	}
      break;
    case 7:
      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "DISPOSE", "dispose", "Dispose")) == 0)
	return FFESTR_genioDISPOSE;
      else if (c < 0)
	{
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "ADVANCE", "advance", "Advance")) == 0)
	    return FFESTR_genioADVANCE;
	}
      break;
    }

  return FFESTR_genioNone;
}
