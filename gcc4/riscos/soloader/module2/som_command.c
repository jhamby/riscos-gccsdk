/* som_command.c
 *
 * Copyright 2006, 2007, 2009 GCCSDK Developers
 * Written by Lee Noar
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/elf.h>
#include "somanager.h"
#include "som.h"
#include "som_runcom.h"
#include "som_history.h"
#include "som_command.h"

const char *column_title_string[] =
{
  "Idx",
  "Text from",
  "Text to",
  "Data from",
  "Data to",
  "Library name",
  "GOT",
  "Dynamic",
  "Time to expire"
};

static void
command_status_libraries (void)
{
  if (global.object_list.count == 0)
  {
    puts ("There are no libraries currently registered.\n");
    return;
  }

  printf ("Number of libraries registered: %d\n\n",
	  global.object_list.count);

  printf ("%-4s %-11s %-11s %-11s %-11s %s\n",
	  column_title_string[column_title_IDX],
	  column_title_string[column_title_TEXT_FROM],
	  column_title_string[column_title_TEXT_TO],
	  column_title_string[column_title_DATA_FROM],
	  column_title_string[column_title_DATA_TO],
	  column_title_string[column_title_LIBRARY_NAME]);

  for (som_object *object = linklist_first_som_object (&global.object_list);
       object != NULL;
       object = linklist_next_som_object (object))
    printf ("%-4d 0x%.8X  0x%.8X  0x%.8X  0x%.8X  %s\n",
	    object->index,
	    (unsigned int)object->base_addr,
	    (unsigned int)object->rw_addr,
	    (unsigned int)object->rw_addr,
	    (unsigned int)(object->rw_addr + object->rw_size),
	    object->name);

  printf ("\n%-4s %-11s %-11s %s\n",
	  column_title_string[column_title_IDX],
	  column_title_string[column_title_GOT],
	  column_title_string[column_title_DYNAMIC],
	  column_title_string[column_title_EXPIRES]);

  for (som_object *object = linklist_first_som_object (&global.object_list);
       object != NULL;
       object = linklist_next_som_object (object))
    {
      printf ("%-4d 0x%.8X  0x%.8X  ",
	     object->index,
	     (unsigned int)object->got_addr,
	     (unsigned int)object->dynamic_addr);

      if (object->usage_count != 0)
	printf ("%-24s", "None, still in use");
      else
	{
	  int t = object->expire_time - os_read_monotonic_time ();
	  if (t < 0)
	    printf ("%-24s", "Pending");
	  else
	    printf ("%02dh %02dm                 ",
		    ((t / 100) / 60) / 60,
		    ((t / 100) / 60) % 60);
	}

      printf ("%s\n", object->name);
    }

  putchar('\n');
}

static void
command_status_clients (void)
{
  if (global.client_list.count == 0)
  {
    puts("There are no clients currently registered.\n");
    return;
  }

  printf ("Number of clients registered: %d\n\n", global.client_list.count);

  printf ("     %-11s %-11s %-11s %-11s\n",
	  column_title_string[column_title_DATA_FROM],
	  column_title_string[column_title_DATA_TO],
	  column_title_string[column_title_GOT],
	  column_title_string[column_title_DYNAMIC]);

  for (som_client *client = linklist_first_som_client (&global.client_list);
       client != NULL;
       client = linklist_next_som_client (client))
    {
      /* The 1st object in the client list is the client itself.  */
      som_object *object = linklist_first_som_object (&client->object_list);

      printf ("%s\n", client->name);
      printf ("     0x%.8X  0x%.8X  0x%.8X  0x%.8X\n",
	      (unsigned int)object->private_rw_ptr,
	      (unsigned int)(object->private_rw_ptr + object->rw_size),
	      (unsigned int)object->private_got_ptr,
	      (unsigned int)(object->private_rw_ptr +
			    (object->dynamic_addr - object->rw_addr)));

      object = linklist_next_som_object (object);

      while (object)
	{
	  printf("  => 0x%.8X  0x%.8X  0x%.8X  0x%.8X  %s\n",
		 (unsigned int)object->private_rw_ptr,
		 (unsigned int)(object->private_rw_ptr + object->rw_size),
		 (unsigned int)object->private_got_ptr,
		 (unsigned int)(object->private_rw_ptr +
			       (object->dynamic_addr - object->rw_addr)),
		 object->name);

	  object = linklist_next_som_object (object);
	}
    }

  putchar('\n');
}

static void
command_address (const char *arg_string, int argc)
{
  som_PTR addr = (som_PTR) strtoul (arg_string, 0, 0);

  som_object *object;
  for (object = linklist_first_som_object (&global.object_list);
       object != NULL;
       object = linklist_next_som_object (object))
    {
      if (addr >= object->base_addr && addr < object->end_addr)
        break;
    }

  if (object != NULL)
    printf ("Address given is at offset 0x%lX of library %s\n",
	    addr - object->base_addr, object->name);
  else
    printf ("Address given is not known to the Shared Object Manager\n");
}

static void
command_expire (const char *arg_string, int argc)
{
  /* If no args given then return current expiry time.  */
  if (argc == 0)
    printf ("Current expiry time is %dh %dm\n",
	    ((global.max_idle_time / 100) /60) / 60,
	    ((global.max_idle_time / 100) /60) % 60);
  else
    {
      /* Parse the args, could be:
         <x>hr <y>m
         <x>hr
         <y>m
         Step through command tail searching for numbers, when one is found
         check the character after to identify it.  */
      int hrs = -1, mins = -1;
      const char *p = arg_string;

      while (*p >= ' ')
	{
	  if (*p >= '0' && *p <= '9')
	    {
	      unsigned int num = os_read_unsigned (&p, 10);

	      if (*p == 'h')
		{
		  hrs = num;
		  p++;
		}
	      else if (*p == 'm')
		{
		  mins = num;
		  p++;
		}
	    }
	  else
	    p++;
	}

      if (hrs < 0 && mins < 0)
	{
	  printf ("Syntax: *SOMExpire [<n>h] [<n>m]\n");
	  return;
	}

      if (hrs < 0)
	hrs = 0;
      if (mins < 0)
	mins = 0;

      global.max_idle_time = (((hrs * 60) + mins) * 60) * 100;
    }
}

static void
command_status(const char *arg_string, int argc)
{
  if (argc == 0)
    {
      command_status_libraries ();
      command_status_clients ();
      return;
    }

  const char *end;
  int len;

  /* Argument string is terminated by carriage return.  */
  for (end = arg_string, len = 0;
       *end != '\0' && *end != ' ' && *end != '\r';
       end++, len++)
    /* */;

  if (strnicmp (arg_string, "libraries", len) == 0)
    command_status_libraries ();
  else if (strnicmp (arg_string, "clients", len) == 0)
    command_status_clients ();
  else
    {
      puts("Syntax: *SOMStatus [clients] | [libraries]");
      puts("*SOMStatus [clients] | [c] displays information about all clients registered.");
      puts("*SOMStatus [libraries] | [l] displays information about all libraries registered.");
      puts("*SOMStatus with no parameter displays both libraries and clients.\n");
    }
}

_kernel_oserror *
module_command (const char *arg_string, int argc, int number, void *pw)
{
  _kernel_oserror *err = NULL;

  switch (number)
    {
    case CMD_SOMStatus:
      command_status (arg_string, argc);
      break;

    case CMD_SOMAddress:
      command_address (arg_string, argc);
      break;

    case CMD_SOMExpire:
      command_expire (arg_string, argc);
      break;

    case CMD_SOMAllowQuit:
      global.flags.no_client_check = true;
      break;

    case CMD_SOMRun:
      err = command_run (arg_string, argc);
      break;

    case CMD_SOMHistory:
      command_history ();
      break;
    }

  return err;
}
