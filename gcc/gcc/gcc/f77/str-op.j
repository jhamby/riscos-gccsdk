ffestrOpen
ffestr_open (ffelexToken t)
{
  char *p;
  int c;

  p = ffelex_token_text (t);

  assert (ffelex_token_type (t) == FFELEX_typeNAME);

  switch (ffelex_token_length (t))
    {
    case 3:
      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "KEY", "key", "Key")) == 0)
	return FFESTR_openKEY;
      else if (c < 0)
	{
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "ERR", "err", "Err")) == 0)
	    return FFESTR_openERR;
	}
      else
	{
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "PAD", "pad", "Pad")) == 0)
	    return FFESTR_openPAD;
	}
      break;
    case 4:
      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "NAME", "name", "Name")) == 0)
	return FFESTR_openNAME;
      else if (c < 0)
	{
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "FILE", "file", "File")) == 0)
	    return FFESTR_openFILE;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "DISP", "disp", "Disp")) == 0)
		return FFESTR_openDISP;
	    }
	  else
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "FORM", "form", "Form")) == 0)
		return FFESTR_openFORM;
	    }
	}
      else
	{
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "TYPE", "type", "Type")) == 0)
	    return FFESTR_openTYPE;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "RECL", "recl", "Recl")) == 0)
		return FFESTR_openRECL;
	    }
	  else
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "UNIT", "unit", "Unit")) == 0)
		return FFESTR_openUNIT;
	    }
	}
      break;
    case 5:
      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "DELIM", "delim", "Delim")) == 0)
	return FFESTR_openDELIM;
      else if (c < 0)
	{
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "BLANK", "blank", "Blank")) == 0)
	    return FFESTR_openBLANK;
	}
      break;
    case 6:
      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "MAXREC", "maxrec", "MaxRec")) == 0)
	return FFESTR_openMAXREC;
      else if (c < 0)
	{
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "ACTION", "action", "Action")) == 0)
	    return FFESTR_openACTION;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "ACCESS", "access", "Access")) == 0)
		return FFESTR_openACCESS;
	    }
	  else
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "IOSTAT", "iostat", "IOStat")) == 0)
		return FFESTR_openIOSTAT;
	    }
	}
      else
	{
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "STATUS", "status", "Status")) == 0)
	    return FFESTR_openSTATUS;
	  else if (c < 0)
	    {
	      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "SHARED", "shared", "Shared")) == 0)
		return FFESTR_openSHARED;
	    }
	}
      break;
    case 7:
      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "DISPOSE", "dispose", "Dispose")) == 0)
	return FFESTR_openDISPOSE;
      break;
    case 8:
      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "READONLY", "readonly", "Readonly")) == 0)
	return FFESTR_openREADONLY;
      else if (c < 0)
	{
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "POSITION", "position", "Position")) == 0)
	    return FFESTR_openPOSITION;
	}
      else
	{
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "USEROPEN", "useropen", "UserOpen")) == 0)
	    return FFESTR_openUSEROPEN;
	}
      break;
    case 9:
      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "BLOCKSIZE", "blocksize", "BlockSize")) == 0)
	return FFESTR_openBLOCKSIZE;
      break;
    case 10:
      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "RECORDSIZE", "recordsize", "RecordSize")) == 0)
	return FFESTR_openRECORDSIZE;
      else if (c < 0)
	{
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "EXTENDSIZE", "extendsize", "ExtendSize")) == 0)
	    return FFESTR_openEXTENDSIZE;
	}
      else
	{
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "RECORDTYPE", "recordtype", "RecordType")) == 0)
	    return FFESTR_openRECORDTYPE;
	}
      break;
    case 11:
      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "DEFAULTFILE", "defaultfile", "DefaultFile")) == 0)
	return FFESTR_openDEFAULTFILE;
      else if (c < 0)
	{
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "BUFFERCOUNT", "buffercount", "BufferCount")) == 0)
	    return FFESTR_openBUFFERCOUNT;
	}
      else
	{
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "INITIALSIZE", "initialsize", "InitialSize")) == 0)
	    return FFESTR_openINITIALSIZE;
	}
      break;
    case 12:
      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "ORGANIZATION", "organization", "Organization")) == 0)
	return FFESTR_openORGANIZATION;
      else if (c < 0)
	{
	  if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "NOSPANBLOCKS", "nospanblocks", "NoSpanBlocks")) == 0)
	    return FFESTR_openNOSPANBLOCKS;
	}
      break;
    case 15:
      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "CARRIAGECONTROL", "carriagecontrol", "CarriageControl")) == 0)
	return FFESTR_openCARRIAGECONTROL;
      break;
    case 17:
      if ((c = ffesrc_strcmp_2c (ffe_case_match (), p, "ASSOCIATEVARIABLE", "associatevariable", "AssociateVariable")) == 0)
	return FFESTR_openASSOCIATEVARIABLE;
      break;
    }

  return FFESTR_openNone;
}
