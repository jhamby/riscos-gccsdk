/* som_command.c
 *
 * Copyright 2006, 2007 GCCSDK Developers
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "somanager.h"
#include "som.h"
#include "som_alloc.h"
#include <sys/elf.h>

extern _kernel_oserror *command_run(const char *, int);

static void command_status(void)
{
som_object *object;

  printf("Shared Object Manager Status\n");
  printf("----------------------------\n");
  printf(" Number of libraries loaded by system: %d\n", global.object_list.count);

  object = linklist_first_som_object(&global.object_list);
  while (object)
  {
    printf("               Library name: %s\n", object->name);
    printf("              Address space: 0x%p -> 0x%p (size: 0x%lX)\n",
	object->base_addr, object->end_addr, object->end_addr - object->base_addr);
    printf("       R/W segment (public): 0x%p -> 0x%p (size: 0x%X)\n",
	object->rw_addr, object->rw_addr + object->rw_size, object->rw_size);
    printf("           GOT ptr (public): 0x%p\n", object->got_addr);
    printf("   Dynamic segment (public): 0x%p\n", object->dynamic_addr);
    printf("             Time to expire: ");

    if (object->usage_count != 0)
      printf("None, still in use\n\n");
    else
    {
    int t = object->expire_time - os_read_monotonic_time();

      if (t <= 0)
	printf("Pending\n\n");
      else
      {
	printf("%dh ", (((t / 100) / 60) / 60));
	printf("%dm\n\n", (((t / 100) / 60) % 60));
      }
    }

    object = linklist_next_som_object(object);
  }

  printf(" Number of clients registered: %d\n", global.client_list.count);

som_client *client = linklist_first_som_client(&global.client_list);

  while (client)
  {
    printf("           Client name: %s\n", client->name);
    printf("                    ID: %X\n", client->unique_ID);

    /* The 1st object in the client list is the client itself. */
    object = linklist_first_som_object(&client->object_list);
    printf(" R/W segment (private): 0x%p -> 0x%p (size: %X)\n",
	object->private_rw_ptr, object->private_rw_ptr + object->rw_size, object->rw_size);
    printf("     GOT ptr (private): 0x%p\n", object->private_got_ptr);

    /*
     * Display details of libraries each client is using
     */
    if (object->flags.type == object_flag_type_CLIENT)
      object = linklist_next_som_object(object);

    while (object)
    {
      printf("            Library handle: %X (%s)\n", object->handle, object->name);
      printf("     R/W segment (private): 0x%p -> 0x%p (size: %X)\n",
	object->private_rw_ptr, object->private_rw_ptr + object->rw_size, object->rw_size);
      printf("         GOT ptr (private): 0x%p\n", object->private_got_ptr);
      printf(" Dynamic segment (private): 0x%p\n\n",
	object->private_rw_ptr + (object->dynamic_addr - object->rw_addr));

      object = linklist_next_som_object(object);
    }

    client = linklist_next_som_client(client);
  }
}

static void command_address(const char *arg_string, int argc)
{
som_PTR addr;

  addr = (som_PTR)strtoul(arg_string, 0, 0);

som_object *object = linklist_first_som_object(&global.object_list);

  while (object)
  {
    if (addr >= object->base_addr && addr < object->end_addr)
    {
      printf("Address given is at offset 0x%lX of library %s\n",
	addr - object->base_addr, object->name);
      break;
    }

    object = linklist_next_som_object(object);
  }

  if (!object)
    printf("Address given is not known to the Shared Object Manager\n");
}

static void command_expire(const char *arg_string, int argc)
{
  /* If no args given then return current expiry time. */
  if (argc == 0)
    printf("Current expiry time is %dh %dm\n",
	((global.max_idle_time / 100) / 60) / 60, ((global.max_idle_time / 100) / 60) % 60);
  else
  {
    /* Parse the args, could be:
       <x>hr <y>m
       <x>hr
       <y>m
       Step through command tail searching for numbers, when one is found
       check the character after to identify it. */
  int hrs = -1, mins = -1;
  const char *p = arg_string;

    while (*p >= ' ')
    {
      if (*p >= '0' && *p <= '9')
      {
      unsigned int num = os_read_unsigned(&p, 10);

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
      printf("Syntax: *SOMExpire [<n>h] [<n>m]\n");
      return;
    }

    if (hrs < 0)
      hrs = 0;
    if (mins < 0)
      mins = 0;

    global.max_idle_time = (((hrs * 60) + mins) * 60) * 100;
  }
}

_kernel_oserror *
module_command(const char *arg_string, int argc, int number, void *pw)
{
_kernel_oserror *err = NULL;

  switch (number)
  {
  case CMD_SOMStatus:
    command_status();
    break;

  case CMD_SOMAddress:
    command_address(arg_string, argc);
    break;

  case CMD_SOMExpire:
    command_expire(arg_string, argc);
    break;

  case CMD_SOMAllowQuit:
    global.flags.no_client_check = true;
    break;

  case CMD_SOMRun:
    err = command_run(arg_string, argc);
    break;
  }

  return err;
}

