/* som_history.c
 *
 * Copyright 2009-2011 GCCSDK Developers
 * Written by Lee Noar
 */

#include "som_history.h"
#include "som.h"
#include "som_alloc.h"
#include "som_command.h"
#include <string.h>
#include <kernel.h>
#include <stdio.h>

#define MAX_HISTORY_COUNT 5

/* Add the given client to the list of recently deregistered clients.  */
void
som_history_add_client(som_client *client)
{
  som_client_object *client_library;
  som_client_history *client_hist = NULL;
  int object_count = 0, name_size;

  /* Start with the client's name.  */
  name_size = strlen (client->object.name) + 1;

  /* Work out how many shared objects the client was linked to and how much
     memory is required to hold all of their names.  */
  for (client_library = linklist_first_som_client_object (&client->object_list);
       client_library != NULL;
       client_library = linklist_next_som_client_object (client_library))
    {
      name_size += strlen (client_library->library->object.name) + 1;
      object_count++;
    }

  /* Allocate enough space to hold details of client and all objects.  */
  if (som_alloc (sizeof (*client_hist) + sizeof (som_object_history) * object_count,
	        (void **) (void *) &client_hist) != NULL)
    return;
  memset (client_hist, 0, sizeof (*client_hist));

  /* Allocate enough memory to store all names.  */
  if (som_alloc (name_size, (void **) (void *) &client_hist->all_names) != NULL)
    {
      som_free (&client_hist);
      return;
    }

  char *name_ptr = client_hist->all_names;

  /* Record the client's name.  */
  client_hist->client.name = name_ptr;
  strcpy (name_ptr, client->object.name);
  name_ptr += strlen (name_ptr) + 1;

  client_hist->client.data_start = client->object.rw_addr;
  client_hist->client.data_end = client->object.rw_addr + client->object.rw_size;
  client_hist->client.GOT = client->object.got_addr;

  /* Record the client's objects.  */
  client_hist->object_history_size = object_count;

  for (client_library = linklist_first_som_client_object (&client->object_list), object_count = 0;
       client_library != NULL;
       client_library = linklist_next_som_client_object (client_library), object_count++)
    {
      som_object_history *obj_hist =
		(som_object_history *) &client_hist->object_history[object_count];

      /* Record this object's name.  */
      obj_hist->name = name_ptr;
      strcpy (name_ptr, client_library->library->object.name);
      name_ptr += strlen (name_ptr) + 1;

      obj_hist->data_start = client_library->rw_addr;
      obj_hist->data_end = client_library->rw_addr + client_library->library->object.rw_size;
      obj_hist->GOT = client_library->got_addr;
    }

  linklist_add_to_end (&global.client_history, &client_hist->link);

  /* If we exceed the maximum size for the list, then remove the oldest.  */
  if (global.client_history.count > MAX_HISTORY_COUNT)
    {
      som_client_history *to_be_removed =
		linklist_first_som_client_history (&global.client_history);

      linklist_remove (&global.client_history, &to_be_removed->link);

      som_free (to_be_removed->all_names);
      som_free (to_be_removed);
    }

  return;
}

void
command_history (void)
{
  if (global.client_history.count == 0)
    {
      puts ("The client history is empty, no clients have deregistered yet.\n");
      return;
    }

  puts ("The following clients were the last ones to deregister.\n"
	"Newest on the list is the last to be shown.\n");

  printf ("     %-11s %-11s %-11s %s\n",
	  column_title_string[column_title_DATA_FROM],
	  column_title_string[column_title_DATA_TO],
	  column_title_string[column_title_GOT],
	  column_title_string[column_title_LIBRARY_NAME]);

  som_client_history *client_hist;
  for (client_hist = linklist_first_som_client_history (&global.client_history);
       client_hist != NULL;
       client_hist = linklist_next_som_client_history (client_hist))
    {
      printf ("%s\n", client_hist->client.name);
      printf ("     0x%.8X  0x%.8X  0x%.8X  <executable>\n",
	      (unsigned int)client_hist->client.data_start,
	      (unsigned int)client_hist->client.data_end,
	      (unsigned int)client_hist->client.GOT);

      som_object_history *obj_hist;
      int obj_count;

      for (obj_count = client_hist->object_history_size,
	   obj_hist = client_hist->object_history;
	   obj_count;
	   obj_count--, obj_hist++)
	printf("  => 0x%.8X  0x%.8X  0x%.8X  %s\n",
	       (unsigned int)obj_hist->data_start,
	       (unsigned int)obj_hist->data_end,
	       (unsigned int)obj_hist->GOT,
	       obj_hist->name);
      putchar ('\n');
    }
}
