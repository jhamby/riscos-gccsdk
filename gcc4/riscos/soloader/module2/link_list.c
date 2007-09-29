/* link_list.c
 *
 * Copyright 2007 GCCSDK Developers
 */

#include <stddef.h>
#include "link_list.h"

void
linklist_add_to_end (link_list *list, link_hdr *link)
{
  link_hdr *oldlast;

  oldlast = list->last;
  link->next = NULL;
  link->prev = oldlast;

  if (oldlast == NULL)
    list->first = link;
  else
    oldlast->next = link;

  list->last = link;
  list->count++;
}

void
linklist_add_to_front (link_list *list, link_hdr *link)
{
  link_hdr *oldfirst;

  oldfirst = list->first;
  link->next = oldfirst;
  link->prev = NULL;

  if (oldfirst == NULL)
    list->last = link;
  else
    oldfirst->prev = link;

  list->first = link;
  list->count++;
}

void
linklist_add_before (link_list *list, link_hdr *at, link_hdr *link)
{
  link_hdr *oldprev;

  if (at == NULL)
    {
      linklist_add_to_end (list, link);
      return;
    }

  oldprev = at->prev;
  if (oldprev == NULL)
    linklist_add_to_front (list, link);
  else
    {
      oldprev->next = link;
      link->prev = oldprev;
      at->prev = link;
      link->next = at;
      list->count++;
    }
}

void
linklist_remove (link_list *list, link_hdr *link)
{
  link_hdr *prev, *next;

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
  list->count--;
}
