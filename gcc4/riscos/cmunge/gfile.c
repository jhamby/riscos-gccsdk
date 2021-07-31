/*
 * Copyright (C) 1999-2000 Robin Watts/Justin Fletcher
 */

/* This file is for 'generalised recoverable file handling'. The intent is
   that by passing things through here when opening and shutting files we
   /should/ be able to keep track of things to undo if we fail. */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "gfile.h"
#include "str.h"
#include "filename.h"

/* Define this to debug this file */
/* #define DEBUG */

typedef struct filelist_s filelist;
struct filelist_s {
  filelist *next;
  FILE *handle;
  const char *filename;
  removetype remove;
};

static filelist *list=NULL;
static int failed=1;

#ifdef DEBUG
const char * const removenames[] =
{
  "never",
  "onfail",
  "onexit",
  "onclose",
  "tempfile"
};
#endif

/*********************************************** <c> Gerph *********
 Function:     file_atexit
 Description:  Tidy up after us at program termination
 Parameters:   none
 Returns:      none
 ******************************************************************/
static void file_atexit(void)
{
#ifdef DEBUG
  printf("file_atexit: Closing rogue files (%s)\n",failed ? "failed" :
         "success");
#endif
  while (list)
  {
    /* we close files that aren't closed */
#ifdef DEBUG
    printf("file_atexit: Looking at %s\n",list->filename);
#endif
    if (list->handle)
    {
#ifdef DEBUG
      printf("             Closing\n");
#endif
      fclose(list->handle); list->handle=NULL;
    }
    /* and remove ones that need it */
    switch (list->remove)
    {
      case remove_never:
        break;
      case remove_onfail:
#ifdef DEBUG
        printf("             Marked as remove on failure\n");
#endif
        if (failed)
          remove(list->filename);
        break;
      case remove_tempfile:
      case remove_onclose:
      case remove_onexit:
#ifdef DEBUG
        printf("             Marked as remove on close/exit/tempfile\n");
#endif
        remove(list->filename);
        break;
    }
    list=list->next;
  }
}

/*********************************************** <c> Gerph *********
 Function:     file_init
 Description:  Initialise the generalised file library
               This marks us as having failed so that if we get an
               'atexit' we remove the files we don't need
 Parameters:   none
 Returns:      none
 ******************************************************************/
void file_init(void)
{
  failed=1;
  list=NULL;
  atexit(file_atexit);
}

/*********************************************** <c> Gerph *********
 Function:     file_final
 Description:  Finalise the generalised file library
               This marks us as having succeeded. We no longer remove
               files on exit (if they are marked remove on fail)
 Parameters:   none
 Returns:      none
 ******************************************************************/
void file_final(void)
{
  failed=0;
}

/*********************************************** <c> Gerph *********
 Function:     file_find
 Description:  Find a file with a particular name
 Parameters:   filename-> the filename to find
 Returns:      pointer to filelist, or NULL if not found
 ******************************************************************/
static filelist *file_find(const char *filename)
{
  filelist *ptr=list;
  while (ptr)
  {
    if (strcmp(ptr->filename,filename)==0)
      return ptr;
    ptr=ptr->next;
  }
  return NULL;
}

/*********************************************** <c> Gerph *********
 Function:     file_write
 Description:  Open a file to write to it
 Parameters:   filename-> the file to write to
               removewhen = whether the file should be removed at all
 Returns:      FILE handle, or NULL if failed
 ******************************************************************/
FILE *file_write(const char *filename,removetype removewhen)
{
  filelist *ptr=file_find(filename);
  FILE *f=fopen(filename,"w");
  if (f == NULL)
  {
    filename = filename_unixtoriscos(filename, EXTLIST_NORCROFT_CMHG);
    f = fopen(filename, "w");
  }

  if (f==NULL)
    return NULL;

  if (ptr==NULL)
  {
    ptr=malloc(sizeof(filelist));
    if (ptr==NULL)
    {
      fclose(f);
      remove(filename);
      return NULL;
    }
    ptr->filename=strdup_strip(filename);
    ptr->remove=removewhen;
    ptr->next=list;
    list=ptr;
  }
#ifdef DEBUG
  printf("file_write: %s (%s)\n",filename,removenames[removewhen]);
#endif
  ptr->handle=f;
  return f;
}

/*********************************************** <c> Gerph *********
 Function:     file_read
 Description:  Open a file to read it
 Parameters:   filename-> the file to read from
               removewhen = whether the file should be removed at all
 Returns:      FILE handle, or NULL if failed
 ******************************************************************/
FILE *file_read(const char *filename,removetype removewhen)
{
  filelist *ptr=file_find(filename);
  FILE *f=fopen(filename,"r");
  if (f == NULL)
  {
    filename = filename_unixtoriscos(filename, EXTLIST_NORCROFT_CMHG);
    f = fopen(filename, "r");
  }

  if (f==NULL)
    return NULL;

  if (ptr==NULL)
  {
    ptr=malloc(sizeof(filelist));
    if (ptr==NULL)
    {
      fclose(f);
      remove(filename);
      return NULL;
    }
    ptr->filename=strdup_strip(filename);
    ptr->remove=removewhen;
    ptr->next=list;
    list=ptr;
  }
#ifdef DEBUG
  printf("file_read: %s (%s)\n",filename,removenames[removewhen]);
#endif
  ptr->handle=f;
  return f;
}

/*******************************************************************
 Function:     file_getfilename
 Description:  Get the filename associated with FILE handle
 Parameters:   fhandle = FILE handle of which filename is requested
 Returns:      filename->, or NULL if failed
 ******************************************************************/
const char *file_getfilename(FILE *f)
{
  filelist *ptr=list;
  while (ptr)
  {
    if (ptr->handle == f)
      return ptr->filename;
    ptr=ptr->next;
  }
  return NULL;
}

/*********************************************** <c> Gerph *********
 Function:     file_temp
 Description:  Return a temporary file - to be deleted on exit, however
               it was caused
 Parameters:   none
 Returns:      pointer to filename, or NULL if failed
 ******************************************************************/
const char *file_temp(void)
{
  filelist *ptr;

  ptr=malloc(sizeof(filelist));
  if (ptr==NULL)
    return NULL;
  ptr->filename=strdup_strip(tmpnam(NULL));
#ifdef DEBUG
  printf("file_temp: %s\n",ptr->filename);
#endif
  ptr->handle=NULL;
  ptr->remove=remove_tempfile;
  ptr->next=list;
  list=ptr;
  return ptr->filename;
}

/*********************************************** <c> Gerph *********
 Function:     file_close
 Description:  Close a file, removing it if necessary
 Parameters:   f-> the file to close
 Returns:      1 if failed, 0 otherwise
 ******************************************************************/
int file_close(FILE *f)
{
  filelist *ptr=list;
  filelist **lastp=&list;
  if (f==NULL)
    return 1; /* failed to close NULL? - actually this should be fatal */

  while (ptr)
  {
    if (ptr->handle==f)
    {
      ptr->handle=NULL;
      fclose(f);
#ifdef DEBUG
      printf("file_close: %s\n",ptr->filename);
#endif
      if (ptr->remove!=remove_tempfile)
      {
        *lastp=ptr->next; /* Unlink from chain */
        if (ptr->remove==remove_onclose)
          remove(ptr->filename);
        free((void *)ptr->filename);
        free(ptr);
      }
      return 0;
    }
    lastp=&ptr->next;
    ptr=ptr->next;
  }
#ifdef DEBUG
  printf("file_close: !!!! File not in the list !!!!\n");
#endif
  /* It's not in the list! Help! */
  fclose(f);
  return 1;
}
