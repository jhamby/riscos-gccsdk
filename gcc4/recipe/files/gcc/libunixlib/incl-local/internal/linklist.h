/* UnixLib internal linked list interface definition.
   Copyright (c) 2015 UnixLib Developers.  */

#ifndef __INTERNAL_LINKLIST_H
#define __INTERNAL_LINKLIST_H 1

#include <stddef.h>

/* A __link_header must be the first member in a struct so that it
 * shares the same base address as the object it is linking.
 */
typedef struct __link_header
{
  struct __link_header *prev, *next;
} __link_header;

typedef struct __link_list
{
  struct __link_header *first, *last;
  int length;
} __link_list;

static inline void
__linklist_init_list (__link_list *list)
{
  list->first = list->last = NULL;
  list->length = 0;
}

static inline void
__linklist_init_link (__link_header *link)
{
  link->prev = link->next = NULL;
}

static inline __link_header *
__linklist_first (__link_list *list)
{
  return list->first;
}

static inline __link_header *
__linklist_last (__link_list *list)
{
  return list->last;
}

static inline __link_header *
__linklist_next (__link_header *link)
{
  return link->next;
}

static inline __link_header *
__linklist_prev (__link_header *link)
{
  return link->prev;
}

extern void __linklist_add_to_end (__link_list *list,
				   __link_header *link);
extern void __linklist_add_to_front (__link_list *list,
				     __link_header *link);
extern void __linklist_add_before (__link_list *list,
				   __link_header *at,
				   __link_header *link);
extern void __linklist_remove (__link_list *list,
			       __link_header *link);

#endif
