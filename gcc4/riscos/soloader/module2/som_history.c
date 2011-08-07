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
  som_object *object;
  som_client_history *client_hist;
  int object_count = 0;

  /* Work out how many shared objects the client was linked to.  */
  for (object = linklist_first_som_object (&client->object_list);
       object != NULL;
       object = linklist_next_som_object (object))
    object_count++;

  /* Allocate enough space to hold details of client and all objects.  */
  if (som_alloc (sizeof (*client_hist) + sizeof (som_object_history) * object_count,
	        (void **) (void *) &client_hist) != NULL)
    return;
  memset (client_hist, 0, sizeof (*client_hist));

  /* Record the client's name.  */
  if (som_alloc (strlen (client->name) + 1,
		(void **) (void *) &client_hist->name) != NULL)
    goto error;
  strcpy (client_hist->name, client->name);

  client_hist->object_history_size = object_count;

  for (object = linklist_first_som_object (&client->object_list), object_count = 0;
       object != NULL;
       object = linklist_next_som_object (object), object_count++)
    {
      som_object_history *obj_hist =
		(som_object_history *) &client_hist->object_history[object_count];

      /* Record this object's name.  */
      if (som_alloc (strlen (object->name) + 1,
		    (void **) (void *) &obj_hist->name) != NULL)
        goto error;
      strcpy (obj_hist->name, object->name);

      obj_hist->data_start = object->private_rw_ptr;
      obj_hist->data_end = object->private_rw_ptr + object->rw_size;
      obj_hist->GOT = object->private_got_ptr;
    }

  linklist_add_to_end (&global.client_history, &client_hist->link);

  /* If we exceed the maximum size for the list, then remove the oldest.  */
  if (global.client_history.count > MAX_HISTORY_COUNT)
    {
      som_client_history *to_be_removed =
		(som_client_history *)global.client_history.first;

      for (object_count = to_be_removed->object_history_size - 1;
	   object_count >= 0;
	   object_count--)
	som_free (to_be_removed->object_history[object_count].name);

      linklist_remove (&global.client_history, &to_be_removed->link);
      som_free (to_be_removed->name);
      som_free (to_be_removed);
    }

  return;

error:
  for (object_count = client_hist->object_history_size - 1;
       object_count >= 0;
       object_count--)
    if (client_hist->object_history[object_count].name)
      som_free (client_hist->object_history[object_count].name);

  if (client_hist->name)
    som_free (client_hist->name);

  som_free (client_hist);
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
      som_object_history *obj_hist = &client_hist->object_history[0];
      int obj_count;

      /* The 1st object in the list is the client itself.  */
      printf ("%s\n", client_hist->name);
      printf ("     0x%.8X  0x%.8X  0x%.8X  <executable>\n",
	      (unsigned int)obj_hist->data_start,
	      (unsigned int)obj_hist->data_end,
	      (unsigned int)obj_hist->GOT);

      for (obj_count = client_hist->object_history_size - 2, obj_hist++;
	   obj_count >= 0;
	   obj_count--, obj_hist++)
	printf("  => 0x%.8X  0x%.8X  0x%.8X  %s\n",
	       (unsigned int)obj_hist->data_start,
	       (unsigned int)obj_hist->data_end,
	       (unsigned int)obj_hist->GOT,
	       obj_hist->name);
      putchar ('\n');
    }
}
