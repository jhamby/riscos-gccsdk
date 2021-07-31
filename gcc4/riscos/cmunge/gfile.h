/*
 * Copyright (C) 1999-2000 Robin Watts/Justin Fletcher
 */

#ifndef GFILE_H
#define GFILE_H

/* This file is for 'generalised recoverable file handling'. The intent is
   that by passing things through here when opening and shutting files we
   /should/ be able to keep track of things to undo if we fail. */

typedef enum {
  remove_never,
  remove_onfail,
  remove_onexit,
  remove_onclose,
  remove_tempfile /* It's a temporary file to remove on exit */
} removetype;

/*********************************************** <c> Gerph *********
 Function:     file_init
 Description:  Initialise the generalised file library
               This marks us as having failed so that if we get an
               'atexit' we remove the files we don't need
 Parameters:   none
 Returns:      none
 ******************************************************************/
void file_init(void);

/*********************************************** <c> Gerph *********
 Function:     file_final
 Description:  Finalise the generalised file library
               This marks us as having succeeded. We no longer remove
               files on exit (if they are marked remove on fail)
 Parameters:   none
 Returns:      none
 ******************************************************************/
void file_final(void);

/*********************************************** <c> Gerph *********
 Function:     file_write
 Description:  Open a file to write to it
 Parameters:   filename-> the file to write to
               removewhen = whether the file should be removed at all
 Returns:      FILE handle, or NULL if failed
 ******************************************************************/
FILE *file_write(const char *filename,removetype removewhen);

/*********************************************** <c> Gerph *********
 Function:     file_read
 Description:  Open a file to read it
 Parameters:   filename-> the file to read frp,
               removewhen = whether the file should be removed at all
 Returns:      FILE handle, or NULL if failed
 ******************************************************************/
FILE *file_read(const char *filename,removetype removewhen);

/*******************************************************************
 Function:     file_getfilename
 Description:  Get the filename associated with FILE handle which
               we got from file_read() or file_write().  The filename
               can be different than the one priorly given to
               file_read()/file_write() because of e.g.
               canonicalistion or suffix swapping done.
 Parameters:   fhandle = FILE handle of which filename is requested
 Returns:      filename->, or NULL if failed
 ******************************************************************/
const char *file_getfilename(FILE *f);

/*********************************************** <c> Gerph *********
 Function:     file_temp
 Description:  Return a temporary file - to be deleted on exit, however
               it was caused
 Parameters:   none
 Returns:      pointer to filename, or NULL if failed
 ******************************************************************/
const char *file_temp(void);

/*********************************************** <c> Gerph *********
 Function:     file_close
 Description:  Close a file, removing it if necessary
 Parameters:   f-> the file to close
 Returns:      1 if failed, 0 otherwise
 ******************************************************************/
int file_close(FILE *f);

#endif
