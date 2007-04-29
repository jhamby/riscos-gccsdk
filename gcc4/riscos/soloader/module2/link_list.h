/* link_list.h
 *
 * Copyright 2007 GCCSDK Developers
 */

#ifndef LINKLIST_H
#define LINKLIST_H

#include <stddef.h>

/* A link_hdr must be the first member in a struct so that it shares the
 * same base address as the object it belongs to.
 */
typedef struct link_hdr
{
  struct link_hdr *prev, *next;

} link_hdr;

typedef struct link_list
{
  struct link_hdr *first, *last;

  int count;

} link_list;

extern inline void linklist_init_list(link_list *list)
{
  list->first = list->last = NULL;
  list->count = 0;
}

extern inline void linklist_init_link(link_hdr *link)
{
  link->prev = link->next = NULL;
}

extern void linklist_add_to_end(link_list *list, link_hdr *link);
extern void linklist_add_to_front(link_list *list, link_hdr *link);
extern void linklist_add_before(link_list *list, link_hdr *at, link_hdr *link);
extern void linklist_remove(link_list *list, link_hdr *link);

#define LINKLIST_DECLARE_TYPES(type) \
\
typedef link_list type##_list;

/* These access functions assume that the LINK_HDR member is called link */
#define LINKLIST_ACCESS_FUNCTIONS(type)	\
\
extern inline type *linklist_first_##type(link_list *list) \
{ \
  return (type *)list->first; \
} \
\
extern inline type *linklist_last_##type(link_list *list) \
{ \
  return (type *)list->last; \
} \
\
extern inline type *linklist_next_##type(type *obj) \
{ \
  return (type *)obj->link.next; \
} \
\
extern inline type *linklist_prev_##type(type *obj) \
{ \
  return (type *)obj->link.prev; \
}

#endif
