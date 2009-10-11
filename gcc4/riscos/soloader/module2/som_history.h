/* som_history.h
 *
 * Copyright 2009 GCCSDK Developers
 * Written by Lee Noar
 */

#ifndef SOM_HISTORY_H
#define SOM_HISTORY_H

#include "link_list.h"
#include "som.h"

/* A link in the list of library histories.  */
typedef struct _som_object_history som_object_history;
struct _som_object_history
{
  char *name;

  /* Pointer the the Global Offset Table of the object.  */
  som_PTR GOT;

  /* Pointer to the start of the client's R/W segment for this object.  */
  som_PTR data_start;

  /* Pointer to the end of the client's R/W segment for this object.  */
  som_PTR data_end;
};

/* A link in the list of client histories.  */
typedef struct _som_client_history som_client_history;
struct _som_client_history
{
  link_hdr link;

  /* Pointer to filename of client.  */
  char *name;

  /* Number of elements in the object history.  */
  int object_history_size;

  /* We don't need a dynamically resizable list as we know how many
     objects the client was linked to and it wont be added to, so
     use an array to store them.  */
  struct _som_object_history object_history[];
};
LINKLIST_ACCESS_FUNCTIONS (som_client_history)

/* Add the given client to the list of recently deregistered clients.  */
void
som_history_add_client(som_client *client);

/* Ouput the history list to stdout.  */
void
command_history (void);

#endif
