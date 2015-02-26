/* UnixLib link list implementation.
   Copyright (c) 2015 UnixLib Developers.  */

#include <internal/linklist.h>

void __attribute__ ((visibility ("hidden")))
__linklist_add_to_end (__link_list *list,
		       __link_header *link)
{
  __link_header *oldlast;

  oldlast = list->last;
  link->next = NULL;
  link->prev = oldlast;

  if (oldlast == NULL)
    list->first = link;
  else
    oldlast->next = link;

  list->last = link;
  list->length++;
}

void __attribute__ ((visibility ("hidden")))
__linklist_add_to_front (__link_list *list,
		         __link_header *link)
{
  __link_header *oldfirst;

  oldfirst = list->first;
  link->next = oldfirst;
  link->prev = NULL;

  if (oldfirst == NULL)
    list->last = link;
  else
    oldfirst->prev = link;

  list->first = link;
  list->length++;
}

void __attribute__ ((visibility ("hidden")))
__linklist_add_before (__link_list *list,
		       __link_header *at,
		       __link_header *link)
{
  __link_header *oldprev;

  if (at == NULL)
    {
      __linklist_add_to_end (list, link);
      return;
    }

  oldprev = at->prev;
  if (oldprev == NULL)
    __linklist_add_to_front (list, link);
  else
    {
      oldprev->next = link;
      link->prev = oldprev;
      at->prev = link;
      link->next = at;
      list->length++;
    }
}

void __attribute__ ((visibility ("hidden")))
__linklist_remove (__link_list *list,
		   __link_header *link)
{
  __link_header *prev, *next;

  next = link->next;
  prev = link->prev;

  if (next == NULL)
    list->last = prev;
  else
    next->prev = prev;

  if (prev == NULL)
    list->first = next;
  else
    prev->next = next;

  link->next = link->prev = NULL;
  list->length--;
}
