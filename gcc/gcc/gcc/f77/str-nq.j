ffestrInquire
ffestr_inquire (ffelexToken t)
{
  char *p;
  int c;

  p = ffelex_token_text (t);

  assert (ffelex_token_type (t) == FFELEX_typeNAME);

  switch (ffelex_token_length (t))
    {
    case 3:
      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "PAD", "pad", "Pad")) == 0)
	return FFESTR_inquirePAD;
      else if (c < 0)
	{
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "ERR", "err", "Err")) == 0)
	    return FFESTR_inquireERR;
	}
      break;
    case 4:
      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "READ", "read", "Read")) == 0)
	return FFESTR_inquireREAD;
      else if (c < 0)
	{
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "FORM", "form", "Form")) == 0)
	    return FFESTR_inquireFORM;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "FILE", "file", "File")) == 0)
		return FFESTR_inquireFILE;
	    }
	  else
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "NAME", "name", "Name")) == 0)
		return FFESTR_inquireNAME;
	    }
	}
      else
	{
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "UNIT", "unit", "Unit")) == 0)
	    return FFESTR_inquireUNIT;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "RECL", "recl", "RecL")) == 0)
		return FFESTR_inquireRECL;
	    }
	}
      break;
    case 5:
      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "KEYED", "keyed", "Keyed")) == 0)
	return FFESTR_inquireKEYED;
      else if (c < 0)
	{
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "DELIM", "delim", "Delim")) == 0)
	    return FFESTR_inquireDELIM;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "BLANK", "blank", "Blank")) == 0)
		return FFESTR_inquireBLANK;
	    }
	  else
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "EXIST", "exist", "Exist")) == 0)
		return FFESTR_inquireEXIST;
	    }
	}
      else
	{
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "WRITE", "write", "Write")) == 0)
	    return FFESTR_inquireWRITE;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "NAMED", "named", "Named")) == 0)
		return FFESTR_inquireNAMED;
	    }
	}
      break;
    case 6:
      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "IOSTAT", "iostat", "IOStat")) == 0)
	return FFESTR_inquireIOSTAT;
      else if (c < 0)
	{
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "ACTION", "action", "Action")) == 0)
	    return FFESTR_inquireACTION;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "ACCESS", "access", "Access")) == 0)
		return FFESTR_inquireACCESS;
	    }
	  else
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "DIRECT", "direct", "Direct")) == 0)
		return FFESTR_inquireDIRECT;
	    }
	}
      else
	{
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "OPENED", "opened", "Opened")) == 0)
	    return FFESTR_inquireOPENED;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "NUMBER", "number", "Number")) == 0)
		return FFESTR_inquireNUMBER;
	    }
	}
      break;
    case 7:
      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "NEXTREC", "nextrec", "NextRec")) == 0)
	return FFESTR_inquireNEXTREC;
      break;
    case 8:
      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "POSITION", "position", "Position")) == 0)
	return FFESTR_inquirePOSITION;
      else if (c < 0)
	{
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "IOLENGTH", "iolength", "IOLength")) == 0)
	    return FFESTR_inquireIOLENGTH;
	}
      break;
    case 9:
      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "READWRITE", "readwrite", "ReadWrite")) == 0)
	return FFESTR_inquireREADWRITE;
      else if (c < 0)
	{
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "FORMATTED", "formatted", "Formatted")) == 0)
	    return FFESTR_inquireFORMATTED;
	}
      break;
    case 10:
      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "SEQUENTIAL", "sequential", "Sequential")) == 0)
	return FFESTR_inquireSEQUENTIAL;
      else if (c < 0)
	{
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "RECORDTYPE", "recordtype", "RecordType")) == 0)
	    return FFESTR_inquireRECORDTYPE;
	}
      break;
    case 11:
      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "UNFORMATTED", "unformatted", "Unformatted")) == 0)
	return FFESTR_inquireUNFORMATTED;
      else if (c < 0)
	{
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "DEFAULTFILE", "defaultfile", "DefaultFile")) == 0)
	    return FFESTR_inquireDEFAULTFILE;
	}
      break;
    case 12:
      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "ORGANIZATION", "organization", "Organization")) == 0)
	return FFESTR_inquireORGANIZATION;
      break;
    case 15:
      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "CARRIAGECONTROL", "carriagecontrol", "CarriageControl")) == 0)
	return FFESTR_inquireCARRIAGECONTROL;
      break;
    }

  return FFESTR_inquireNone;
}
