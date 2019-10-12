/* types.h
 *
 * Copyright 2019 GCCSDK Developers
 * Written by Lee Noar
 */

#ifndef TYPES_H
#define TYPES_H

#include "link_list.h"

typedef unsigned int bool;
#define true 1
#define false 0

typedef unsigned int stack_t;
typedef unsigned char *eabi_PTR;
struct _armeabisupport_allocator;

typedef struct pmp_phy_page_entry
{
  int pmp_page_index;
  int physical_page_number;
  unsigned flags;
} pmp_phy_page_entry;

typedef struct pmp_log_page_entry
{
  int da_page_number;
  int pmp_page_index;
  unsigned flags;
} pmp_log_page_entry;

typedef struct pmp_page_info_entry
{
  int pmp_page_index;
  int physical_page_number;
  int da_page_number;
  unsigned flags;
} pmp_page_info_entry;

typedef struct dynamicarea_block
{
  unsigned int number;
  eabi_PTR base;
  eabi_PTR end;
 
} dynamicarea_block;

typedef struct _app_object
{
  link_hdr link;
  unsigned ID;
  link_list allocator_list;
  link_list abort_handler_list;
  link_list stack_list;
  unsigned abort_last_wimp_count;
  unsigned abort_highest_wimp_count;
  unsigned abort_total_count;
} app_object;
LINKLIST_ACCESS_FUNCTIONS (app_object)

typedef struct armeabisupport_globals
{
  struct _armeabisupport_allocator *stack_allocator;
  link_list app_list;
  link_list allocator_list;
  bool pre_filter_installed;
  bool post_filter_installed;
} armeabisupport_globals;

extern armeabisupport_globals global;

#endif
