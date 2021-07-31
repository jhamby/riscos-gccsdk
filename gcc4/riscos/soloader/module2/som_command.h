/* som_command.h
 *
 * Copyright 2009 GCCSDK Developers
 * Written by Lee Noar
 */

#ifndef SOM_COMMAND_H
#define SOM_COMMAND_H

extern const char *column_title_string[];

enum column_title
{
  column_title_IDX,
  column_title_TEXT_FROM,
  column_title_TEXT_TO,
  column_title_DATA_FROM,
  column_title_DATA_TO,
  column_title_LIBRARY_NAME,
  column_title_GOT,
  column_title_DYNAMIC,
  column_title_EXPIRES
};

#endif
