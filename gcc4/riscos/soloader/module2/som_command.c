/* som_command.c
 *
 * Copyright 2006, 2007, 2009 GCCSDK Developers
 * Written by Lee Noar
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/elf.h>
#include "somanager.h"
#include "som.h"
#include "som_runcom.h"
#include "som_history.h"
#include "som_command.h"
#include "som_utils.h"

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

  for (som_library_object *global_library = linklist_first_som_library_object (&global.object_list);
       global_library != NULL;
       global_library = linklist_next_som_library_object (global_library))
    {
/*      unsigned text_size = global_library->object.flags.is_armeabihf ?
				page_align_size(global_library->object.text_size) :
				global_library->object.text_size;*/

      printf ("%-4d 0x%.8X  0x%.8X  0x%.8X  0x%.8X  %s\n",
	      global_library->object.index,
	      (unsigned int)global_library->object.text_segment,
	      (unsigned int)global_library->object.text_segment + global_library->object.text_size,
	      (unsigned int)global_library->object.data_rw_segment,
	      (unsigned int)(global_library->object.data_rw_segment +
			     global_library->object.data_size),
	      global_library->object.name);
    }

  printf ("\n%-4s %-11s %-11s %s\n",
	  column_title_string[column_title_IDX],
	  column_title_string[column_title_GOT],
	  column_title_string[column_title_DYNAMIC],
	  column_title_string[column_title_EXPIRES]);

  for (som_library_object *global_library = linklist_first_som_library_object (&global.object_list);
       global_library != NULL;
       global_library = linklist_next_som_library_object (global_library))
    {
      printf ("%-4d 0x%.8X  0x%.8X  ",
	     global_library->object.index,
	     (unsigned int)global_library->object.got_addr,
	     (unsigned int)global_library->object.dynamic_addr);

      if (global_library->usage_count != 0)
	printf ("%-24s", "None, still in use");
      else
	{
	  int t = global_library->expire_time - os_read_monotonic_time ();
	  if (t < 0)
	    printf ("%-24s", "Pending");
	  else
	    printf ("%02dh %02dm                 ",
		    ((t / 100) / 60) / 60,
		    ((t / 100) / 60) % 60);
	}

      printf ("%s\n", global_library->object.name);
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
      printf ("%s\n", client->object.name);
      printf ("     0x%.8X  0x%.8X  0x%.8X  0x%.8X\n",
	      (unsigned int)client->object.data_rw_segment,
	      (unsigned int)(client->object.data_rw_segment + client->object.data_size),
	      (unsigned int)client->object.got_addr,
	      (unsigned int)client->object.dynamic_addr);

      som_client_object *client_library =
		linklist_first_som_client_object (&client->object_list);

      while (client_library)
	{
	  printf("  => 0x%.8X  0x%.8X  0x%.8X  0x%.8X  %s\n",
		 (unsigned int)client_library->data_segment,
		 (unsigned int)(client_library->data_segment + client_library->library->object.data_size),
		 (unsigned int)client_library->got_addr,
		 (unsigned int)(client_library->data_segment +
			       (client_library->library->object.dynamic_addr - client_library->data_segment)),
		 client_library->library->object.name);

	  client_library = linklist_next_som_client_object (client_library);
	}
    }

  putchar('\n');
}

static _kernel_oserror *
command_address (const char *arg_string, int argc)
{
  som_PTR addr = NULL;
  int reg = -1;
  const char *c_args = copy_arg_string (arg_string);

  if (c_args == NULL)
    return somerr_no_memory;

  int arg_len = strlen (c_args);

  if (arg_len >= 2)
    {
      if (tolower(*c_args) == 'p' && tolower(*(c_args + 1)) == 'c')
	reg = 15;
      else if (tolower(*c_args) == 'l' && tolower(*(c_args + 1)) == 'r')
	reg = 14;
    }

  if (reg == -1 && tolower(*c_args) == 'r')
    reg = strtoul (c_args + 1, 0, 10);

  if (reg != -1)
    {
      unsigned int *exception_register;

      if (reg > 15)
	{
	  printf ("Bad register number given: r%d\n", reg);
	  goto exit;
	}

      /* Read the address of the exception register block.  */
      _swix(OS_ChangeEnvironment,
	    _INR(0,1) | _OUT(1),
	    13 /* Exception Registers */,
	    0, /* Zero to read */
	    &exception_register);

      if (exception_register == NULL)
	{
	  printf ("Unable to read address of exception register block\n");
	  goto exit;
	}

      addr = (som_PTR) exception_register[reg];
    }
  else
    addr = (som_PTR) strtoul (c_args, 0, 16);

  som_library_object *global_library;
  for (global_library = linklist_first_som_library_object (&global.object_list);
       global_library != NULL;
       global_library = linklist_next_som_library_object (global_library))
    {
      if (addr >= global_library->object.text_segment &&
	  addr < global_library->object.end_addr)
        break;
    }

  if (global_library != NULL)
    printf ("Address %p is at offset 0x%lX of library %s\n",
	    addr, addr - global_library->object.text_segment, global_library->object.name);
  else
    printf ("Address %p is not known to the Shared Object Manager\n", addr);

exit:
  som_free (c_args);
  return NULL;
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
      err = command_address (arg_string, argc);
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
