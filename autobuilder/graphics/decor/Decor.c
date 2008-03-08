

/*  Décor Version 4.20 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "DeskLib:Menu.h"
#include "DeskLib:Event.h"
#include "DeskLib:Resource.h"
#include "DeskLib:Handler.h"
#include "DeskLib:DragASpr.h"
#include "DeskLib:Drag.h"
#include "DeskLib:Filing.h"
#include "DeskLib:File.h"
#include "DeskLib:Template.h"
#include "DeskLib:SWI.h"
#include "DeskLib:Window.h"
#include "DeskLib:Error.h"
#include "DeskLib:EventMsg.h"
#include "DeskLib:Icon.h"
#include "DeskLib:Time.h"
#include "DeskLib:Hourglass.h"



#define SWI_JPEG_FileInfo	0x49981
#define SWI_JPEG_PlotFileScaled	0x49983
#define SWI_Wimp_SlotSize	0x400ec

static icon_handle icon_iconbar;
static window_handle win_info;
static window_handle win_control;
static window_handle win_back;
static menu_ptr menu_icon;

static BOOL finished = FALSE;
static BOOL choices_open = FALSE;
static BOOL shift_pressed = FALSE;

static BOOL auto_quit, delete_quit, auto_change, rejpeg, display_icon;
static BOOL let_pinboard;
static BOOL ignore_option = FALSE, sequential, already_running;
static int time, dither, bpp, oldtime;
static int current = -1, current_type, current_option = -1;

static char **back_path;
static int *options;
static int *max_entry;
static int num_paths = 0;
static int max_paths = 12;

static char **files;
static int *type;
static int num_files = 0;
static int max_files = 100;

BOOL temp_used = FALSE;
static char temp_path[256];
static char temp_image[256];
static int temp_type;

#define ICON_auto	2
#define ICON_delete	9
#define ICON_rejpeg	11
#define ICON_dither	10
#define ICON_display	13
#define ICON_autoquit	12
#define ICON_sequential	15
#define ICON_pinboard   16

#define sync_time() oldtime = Time_Monotonic() + time * 6000


int read_bpp(void) {
  int bits;

  SWI(2, 3, SWI_OS_ReadModeVariable, -1, 9, NULL, NULL, &bits);
  return bits;
}


void option_menu(int option) {
  Menu_SetFlags(menu_icon, 3, option == 0, 0);
  Menu_SetFlags(menu_icon, 4, option == 1, 0);
  Menu_SetFlags(menu_icon, 5, option == 2, 0);
}


int option_get(void) {
  int set;

  Menu_GetFlags(menu_icon, 3, &set, NULL);
  if (set) return 0;
  Menu_GetFlags(menu_icon, 4, &set, NULL);
  if (set) return 1;
  return 2;
}


void read_fileoption(FILE *file, int icon, BOOL *option) {
  fscanf(file, "%d", option);
  Icon_SetSelect(win_control, icon, *option);
}


void *clear_realloc(void *old, int old_size, int new_size) {
  int *clear;
  char *new_block = realloc(old, new_size);

  for (clear = (int *)(new_block + old_size);
       clear < (int *)(new_block + new_size);
       clear++) {
    *clear = 0;
  }

  return new_block;
}


void setup_read(void) {
  FILE *file;
  int path;

//  file = fopen("<Décor$Dir>.Config", "r");
  file = fopen("<Choices$Dir>.Décor", "r");

  if (!file) {
    Error_Report(0, "No choices found, using defaults.");

//                    "could not find its configuration file.  "
//                    "It will use default options.  Choose save from within "
//                    "the configuration to recreate the file.");

    Icon_SetSelect(win_control, ICON_auto, auto_change = TRUE);
    Icon_printf(win_control, 3, "%d", time = 25);
    Icon_SetSelect(win_control, ICON_delete, delete_quit = FALSE);
    Icon_SetSelect(win_control, ICON_rejpeg, rejpeg = TRUE);
    dither = 3;
    Icon_SetSelect(win_control, ICON_autoquit, auto_quit = FALSE);
    Icon_SetSelect(win_control, ICON_display, display_icon = TRUE);
    Icon_SetSelect(win_control, ICON_sequential, sequential = FALSE);
    Icon_SetSelect(win_control, ICON_pinboard, let_pinboard = FALSE);
    num_paths = 0;

  } else {
    read_fileoption(file, ICON_auto, &auto_change);
  
    fscanf(file, "%d", &time);
    Icon_printf(win_control, 3, "%d", time);
  
    read_fileoption(file, ICON_delete, &delete_quit);
    read_fileoption(file, ICON_rejpeg, &rejpeg);
  
    fscanf(file, "%d", &dither);
  
    read_fileoption(file, ICON_autoquit, &auto_quit);
    read_fileoption(file, ICON_display, &display_icon);
    read_fileoption(file, ICON_sequential, &sequential);

    read_fileoption(file, ICON_pinboard, &let_pinboard);
    Icon_SetShade(win_control, ICON_rejpeg, let_pinboard);
    Icon_SetShade(win_control, ICON_dither, let_pinboard);


    fscanf(file, "%d", &num_paths);
  
    if (num_paths > max_paths) {
      int old = num_paths * 4;
      while (num_paths > max_paths) max_paths *= 2;
      back_path = clear_realloc(back_path, old, sizeof(char **) * max_paths);
      options = clear_realloc(options, old, sizeof(int *) * max_paths);
      max_entry = clear_realloc(max_entry, old, sizeof(int *) * max_paths);
    }
  
    for (path = 0; path < num_paths; path++) {
      char opt[10];
      if (!back_path[path]) back_path[path] = malloc(256);
      fscanf(file, "%s %s", back_path[path], opt);
  
      if (strcmp(opt, "centre") == 0) {
        options[path] = 0;
      } else if (strcmp(opt, "scale") == 0) {
        options[path] = 1;
      } else  {
        options[path] = 2;
      }
    }

    fclose(file);
  }
}


int read_dir(char *image_path, int total_count) {
  char buffer[0x200];
  int read;
  int index = 0, num_files = 0;

  if (strcmp(image_path, "")) {
    do {
      if (!SWI(7, 5, SWI_OS_GBPB, 10, image_path, buffer, 1, index, 0x200, NULL,
          NULL, NULL, NULL, &read, &index)) {
        if (read) {
          int filetype = (*(int *)buffer & 0xfff00) >> 8;
          if (filetype == 0xc85 || filetype == 0xff9) {
            if (total_count + num_files == max_files) {
              int old = max_files * 4;
              max_files *= 2;
              files = clear_realloc(files, old, sizeof(char **) * max_files);
              type = clear_realloc(type, old, sizeof(int *) * max_files);
            }

            if (files[total_count + num_files]) free(files[total_count + num_files]);
            strcpy(files[total_count + num_files] = malloc(strlen(buffer + 20) + 1), buffer + 20);
            type[total_count + num_files] = filetype;
            num_files++;
          }
        }
      }
    } while (index != -1);
  }
  return num_files;
}



void read_cats(BOOL use_temp) {
  int path;
  num_files = 0;

  if (use_temp) {
    num_files = read_dir(temp_path, 0);
    if (!num_files) { Error_Report(0, "No sprite or JPEG files were found in this"
      " directory.  Décor will revert to its default paths.");
      read_cats(temp_used = FALSE);
    }
  } else {
    for (path = 0; path < num_paths; path++) {
      num_files += read_dir(back_path[path], num_files);
      max_entry[path] = num_files;
    }
    if (!num_files) Error_Report(0, "No sprite or JPEG files were found in any of the"
      " directories.  Décor will not be able to operate until this is corrected.");
  }
}


void change_jpeg(const char *filename, char *option) {
  char buffer[256];
  int width, height, flags, darea_handle;
  int size, bpp, mode;
  int *sprite_block;

  if (SWI(2, 4, SWI_JPEG_FileInfo, 1, filename, NULL, NULL, &width, &height)) {
    strcpy(buffer, "There were problems loading the JPEG file ");
    strcat(buffer, filename);
    strcat(buffer, ".");
    Error_Report(1, buffer);
    return;
  }

  flags = (bpp = read_bpp()) < 4;
  bpp = (int)pow(2, bpp);
  size = (width * height * bpp) / 8 + 0x1000;
  Hourglass_Start(80);

  SWI(9, 6, SWI_OS_DynamicArea, 0, -1, size, -1, 0, size, 0, -1, "Décor Workspace",
    NULL, &darea_handle, NULL, &sprite_block, NULL, NULL);

  sprite_block[0] = size;
  sprite_block[1] = 0;
  sprite_block[2] = sprite_block[3] = 16;

  SWI(2, 0, SWI_OS_SpriteOp, 0x109, sprite_block);

  SWI(1, 3, 0x06, 135, NULL, NULL, &mode);
  SWI(7, 0, SWI_OS_SpriteOp, 0x10f, sprite_block, "backdrop", flags, width, height, mode);
  SWI(4, 0, SWI_OS_SpriteOp, 0x13c, sprite_block, "backdrop", 0);

  SWI(5, 0, SWI_JPEG_PlotFileScaled, filename, 0, 0, 0, 3);
  SWI(4, 0, SWI_OS_SpriteOp, 0x13c, sprite_block, 0, 1);
  SWI(3, 0, SWI_OS_SpriteOp, 0x10c, sprite_block, "<Wimp$ScrapDir>.Backdrop");

  SWI(2, 0, SWI_OS_DynamicArea, 1, darea_handle);

  strcpy(buffer, "Backdrop -");
  strcat(buffer, option);
  strcat(buffer, " <Wimp$ScrapDir>.Backdrop");
  Wimp_StartTask(buffer);

  Hourglass_Off();
}


void change_sprite(const char *filename, char *option) {
  char buffer[256];

  strcpy(buffer, "Backdrop -");
  strcat(buffer, option);
  strcat(buffer, " ");
  strcat(buffer, filename);
  Wimp_StartTask(buffer);
}


static char last_file[256];

void backdrop_file(const char *filename, int opt, int type) {
  char *option = opt ? (opt == 1 ? "scale" : "tile") : "centre";

  if (filename != last_file)
    strcpy(last_file, filename);

  if ((current_type = type) == 0xc85 && !let_pinboard)
    change_jpeg(filename, option);
  else
    change_sprite(filename, option);
}


void backdrop_change(int change) {
  int new_value, path = 0;
  char filename[256];

  if (!num_files) return;

  if (change == 1) {
    if (num_files > 1) {
      if (sequential && (current != -1 || temp_used)) {
        if (shift_pressed) {
          shift_pressed = FALSE;
          if (current-- == 0) current = num_files - 1;
        } else {
          current = (current + 1) % num_files;
        }
      } else {
        do {
          new_value = div((int)rand(), num_files).rem;
        } while (new_value == current);
        current = new_value;
      }
    }
  } else if (change > 1) {
/*    backdrop_file("<Décor$Dir>.Backdrop", current_option = change - 2, 0xff9);*/
    backdrop_file(last_file, current_option = change - 2, 0xff9);
    option_menu(current_option);
    return;
  }

  /* Only set at this point if it's a temporaray image */
  if (current == -1) {
    backdrop_file(temp_image, current_option, temp_type);
  } else {
    if (temp_used) {
      strcpy(filename, temp_path);
    } else {
      while (current >= max_entry[path]) path++;
  
      strcpy(filename, back_path[path]);
    }

    strcat(filename, ".");
    strcat(filename, files[current]);
  
    if (!ignore_option) current_option = options[path];
  
    option_menu(current_option);

    backdrop_file(filename, current_option, type[current]);
  }
}


void setup_save(void) {
  FILE *file = fopen("<Choices$Write>.Décor", "w");
  int path, non_null = 0;

  fputs(auto_change ? "1" : "0", file);
  fprintf(file, "\n%d\n", time);

  fputs(delete_quit ? "1\n" : "0\n", file);
  fputs(rejpeg ? "1\n" : "0\n", file);
  fprintf(file, "%d\n", dither);
  fputs(auto_quit ? "1\n" : "0\n", file);
  fputs(display_icon ? "1\n" : "0\n", file);
  fputs(sequential ? "1\n" : "0\n", file);
  fputs(let_pinboard ? "1\n" : "0\n", file);

  for (path = 0; path < num_paths; path++)
    if (strcmp(back_path[path], "")) non_null++; 

  fprintf(file, "%d\n", non_null);

  for (path = 0; path < num_paths; path++)
    if (strcmp(back_path[path], ""))
      fprintf(file, "%s\n%s\n", back_path[path],
       options[path] ? (options[path] == 1 ? "scale" : "tile") : "centre");

  fclose(file);
}


void setup_cancel(void) {
  Icon_SetSelect(win_control, ICON_auto, auto_change);
  Icon_SetSelect(win_control, ICON_rejpeg, rejpeg);
  Icon_SetSelect(win_control, ICON_delete, delete_quit);
  Icon_SetSelect(win_control, ICON_dither, dither == 1);
  Icon_SetSelect(win_control, ICON_display, display_icon);
  Icon_SetSelect(win_control, ICON_autoquit, auto_quit);
  Icon_SetSelect(win_control, ICON_sequential, sequential);
  Icon_printf(win_control, 3, "%d", time);
}


void setup_update(void) {
  int oldtime;

  auto_change = Icon_GetSelect(win_control, ICON_auto);
  rejpeg = Icon_GetSelect(win_control, ICON_rejpeg);
  delete_quit = Icon_GetSelect(win_control, ICON_delete);

  if (Icon_GetSelect(win_control, ICON_dither)) {
    if (dither == 3) {
      dither = 1;
      backdrop_change(FALSE);
    }
  } else {
    if (dither == 1) {
      dither = 3;
      backdrop_change(FALSE);
    }
  }

  auto_quit = Icon_GetSelect(win_control, ICON_autoquit);
  display_icon = Icon_GetSelect(win_control, ICON_display);
  sequential = Icon_GetSelect(win_control, ICON_sequential);

  oldtime = time;
  if ((time = Icon_GetInteger(win_control, 3)) <= 0) time = oldtime;
  sync_time();
}

       
void record_paths(void) {
  int path, icon;

  for (path = 0; path < 12; path++) {
    icon = path * 4 + 7;

    if (!back_path[path]) back_path[path] = malloc(256); 
    Icon_GetText(win_back, icon, back_path[path]);
    if (strcmp(back_path[path], "") && path >= num_paths) num_paths = path + 1;

    if (Icon_GetSelect(win_back, icon + 1)) {
      options[path] = 0;
    } else if (Icon_GetSelect(win_back, icon + 2)) {
      options[path] = 1;
    } else {
      options[path] = 2;
    }
  }

  ignore_option = FALSE;
  read_cats(FALSE);
}


void set_pathwindow(void) {
  int path, paths, icon;

  paths = num_paths;
  if (paths > 12) paths = 12;

  for (path = 0; path < paths; path++) {
    icon = path * 4 + 7;

    Icon_SetText(win_back, icon, back_path[path]);

    Icon_SetSelect(win_back, icon + 1, options[path] == 0);
    Icon_SetSelect(win_back, icon + 2, options[path] == 1);
    Icon_SetSelect(win_back, icon + 3, options[path] == 2);
  }
  Window_Show(win_back, open_WHEREVER);
}



void enable_menu(int state) {
  Menu_SetFlags(menu_icon, 1, 0, state);
  Menu_SetFlags(menu_icon, 7, 0, state);
}



static BOOL ButtonClick(event_pollblock *event, void *reference) {
  window_handle window = event->data.mouse.window;
  icon_handle icon = event->data.mouse.icon;
  int button = event->data.mouse.button.value;

  UNUSED(reference);

  if (window == -2) {
    switch (button) {
      case 1:
        if (!choices_open) {
          Window_Show(win_control, open_WHEREVER);
          enable_menu(1);
          choices_open = TRUE;
        }
        break;

      case 2:
        Menu_Show(menu_icon, event->data.mouse.pos.x, -1);
        break;

      case 4:
        {
          int shift;
          int ctrl;

          SWI(3, 2, SWI_OS_Byte, 129, 0xff, 0xff, NULL, &shift);
          if (shift == 0xff) shift_pressed = TRUE;
          SWI(3, 2, SWI_OS_Byte, 129, 0xfe, 0xff, NULL, &ctrl);

          if (ctrl == 0xff) {
            let_pinboard = !let_pinboard;
            Icon_SetShade(win_control, ICON_rejpeg, let_pinboard);
            Icon_SetShade(win_control, ICON_dither, let_pinboard);
            backdrop_change(FALSE);
          } else {
            backdrop_change(TRUE);
            sync_time();
          }
        }
    }
  } else if (window == win_control && (button & 0x05)) {
    switch (icon) {
      case 6:
        Window_Hide(win_control);
        set_pathwindow();
        break;

      case 7:
        setup_cancel();
        Window_Hide(win_control);
        enable_menu(0);
        choices_open = FALSE;
        break;

      case 8:
        setup_update();
        if (button == 4) {
          Window_Hide(win_control);
          enable_menu(0);
          if (!display_icon) Wimp_DeleteIcon(-1, icon_iconbar);
          choices_open = FALSE;
        }
      break;

      case ICON_autoquit:
        Icon_Deselect(win_control, ICON_delete);
        break;

      case ICON_delete:
        Icon_Deselect(win_control, ICON_autoquit);
        break;

      case ICON_pinboard:
        let_pinboard = !let_pinboard;
        Icon_SetShade(win_control, ICON_rejpeg, let_pinboard);
        Icon_SetShade(win_control, ICON_dither, let_pinboard);
        break;

      case 14:
        setup_update();
        setup_save();
        if (button == 4) {
          Window_Hide(win_control);
          enable_menu(0);
          if (!display_icon) Wimp_DeleteIcon(-1, icon_iconbar);
          choices_open = FALSE;
        }
        break;
    }
  } else if (window == win_back && (button & 0x05)) {
    if (icon == 2) {
      Window_Hide(win_back);
      Window_Show(win_control, open_WHEREVER);
      record_paths();
    } else if (icon == 3) {
      Window_Hide(win_back);
      Window_Show(win_control, open_WHEREVER);
    }
  }

  return TRUE;
}


static BOOL MenuChoice(event_pollblock *event, void *reference) {
  mouse_block ptr;

  UNUSED(reference);

  switch (event->data.selection[0]) {
    case 1:
      Window_Show(win_control, open_WHEREVER);
      enable_menu(1);
      break;

    case 2:
      sync_time();
      backdrop_change(TRUE);
      break;

    case 3:
    case 4:
    case 5:
      {
        int choice = event->data.selection[0];

        if (current_option != choice - 3) backdrop_change(choice - 1);
      }
      break;

    case 6:
      read_cats(temp_used = ignore_option = FALSE);
      Menu_SetFlags(menu_icon, 6, FALSE, TRUE);
      backdrop_change(TRUE);
      break;

    case 7:
      Wimp_DeleteIcon(-1, icon_iconbar);
      return TRUE;
      break;

    case 8:
      finished = TRUE;
  }

  Wimp_GetPointerInfo(&ptr);
  if (ptr.button.data.adjust) Menu_ShowLast();

  return TRUE;
}


static BOOL GetDragPath(event_pollblock *event, void *reference) {
  message_dataload *dataload = &event->data.message.data.dataload;

  UNUSED(reference);

  if (dataload->filetype == 0x1000 && (dataload->icon - 7) % 4 == 0)
    Icon_SetText(win_back, dataload->icon, dataload->filename);

  return TRUE;
}


static BOOL ShowPicNow(event_pollblock *event, void *reference) {
  message_dataload *dataload = &event->data.message.data.dataload;
  int type = dataload->filetype;

  UNUSED(reference);

  if (type == 0xc85 || type == 0xff9) {
    sync_time();
    current = -1;
    backdrop_file(strcpy(temp_image, dataload->filename), option_get(), temp_type = type);
  } else if (type == 0x1000) {
    num_paths = 1;

    ignore_option = TRUE;
    strcpy(temp_path, dataload->filename);
    temp_used = TRUE;

    Menu_SetFlags(menu_icon, 6, FALSE, FALSE);
    if (sequential) current = -1;

    read_cats(TRUE);
    sync_time();
    backdrop_change(TRUE);
  } else {
    Error_Report(1, "Décor cannot load this");
  }

  return TRUE;
}


static BOOL Change(event_pollblock *event, void *reference) {

  UNUSED(event);
  UNUSED(reference);

  sync_time();
  if (auto_change && !already_running) backdrop_change(TRUE);
  return TRUE;
}


static BOOL ModeChange(event_pollblock *event, void *reference) {
  int new_bpp = read_bpp();

  UNUSED(event);
  UNUSED(reference);

  if (new_bpp != bpp) {
    bpp = new_bpp;
    if (!let_pinboard)
      backdrop_change(FALSE);
  }
  return FALSE;
}


BOOL find_others(void) {
  int entries[4];
  int more;

  SWI(3, 1, SWI_TaskManager_EnumerateTasks, 0, entries, sizeof(entries), &more);

  while (more >= 0) {
    if (strncmp((char *)entries[1], "Décor", 5) == 0) return TRUE; 
    SWI(3, 1, SWI_TaskManager_EnumerateTasks, more, entries,
              sizeof(entries), &more);
  }
  return FALSE;
}


int IconBar(void) {
  int block[9], handle;

  block[0] = -7;
  block[1] = 0;
  block[2] = 0;
  block[3] = 68;
  block[4] = 68;
  block[5] = (1 << 1) + (1 << 3) + (1 << 4) + (1 << 12) + (1 << 13);
  strcpy((char *)block + 24, "!décor");

  SWI(2, 1, 0x400c2, 0x20000000, block, &handle);
  return handle;
}


int main(void) {
  BOOL override;

  SWI(3, 2, SWI_OS_Byte, 129, -3, 0xff, NULL, &override);
  override = (override != 0xff);   

  Resource_Initialise("Décor");

  Event_Initialise("Décor");
  EventMsg_Initialise();

  Template_Initialise();
  Template_LoadFile("Templates");

  win_info = Window_Create("InfoWind", 0);
  win_control = Window_Create("Control", 0);
  win_back = Window_Create("Backdrops", 0);

  {
    long long value = 3;

    SWI(2, 0, SWI_OS_Word, 14, &value); 
    srand((int)value);
  }

  back_path = calloc(sizeof(char **) * max_paths, 1);
  {
    int opt_num;
    options = calloc(sizeof(int *) * max_paths, 1);
    for (opt_num = 0; opt_num < 12; opt_num++) options[opt_num] = 1;
  }

  max_entry = calloc(sizeof(int *) * max_paths, 1);

  files = calloc(sizeof(char **) * max_files, 1);
  type = calloc(sizeof(int *) * max_files, 1);

  setup_read();
  read_cats(FALSE);
  bpp = read_bpp();

  if ((already_running = find_others()))
    Error_Report(0, "Décor has detected other copies of itself running, and will not automatically change the backdrop but will display an icon.");

  if (!override || display_icon || already_running)  
    icon_iconbar = IconBar(); /*      Icon_BarIcon("!décor", iconbar_RIGHT);*/
  else
    icon_iconbar = 0;

  menu_icon = Menu_New("Décor", "Info,Choices...,Change now"
                       "|!Centred, Scaled, Tiled"
                       "|~Forget temp dir,Remove icon,Quit");
  Menu_AddSubMenu(menu_icon, 0, (menu_ptr)win_info);

  Event_Claim(event_NULL, event_ANY, event_ANY, Change, NULL);
  Event_Claim(event_OPEN, event_ANY, event_ANY, Handler_OpenWindow, NULL);
  Event_Claim(event_CLICK, event_ANY, event_ANY, ButtonClick, NULL);
  Event_Claim(event_MENU, event_ANY, event_ANY, MenuChoice, NULL);
  Event_Claim(event_CLOSE, event_ANY, event_ANY, Handler_CloseWindow, NULL);
  Event_Claim(event_REDRAW, event_ANY, event_ANY, Handler_NullRedraw, NULL);

  EventMsg_Claim(message_DATALOAD, win_back, GetDragPath, NULL);
  EventMsg_Claim(message_DATALOAD, -2, ShowPicNow, NULL);
  EventMsg_Claim(message_MODECHANGE, event_ANY, ModeChange, NULL);


  sync_time();
  if (!already_running) backdrop_change(TRUE);

  if (override || !auto_quit)
    while (!finished) {
      Wimp_PollIdle(event_mask, &event_lastevent, oldtime);
      Event_Process(&event_lastevent); 
    }

  Template_ClearAll();

  if (delete_quit) {
    Wimp_StartTask("Remove <Wimp$ScrapDir>.Backdrop");
    Wimp_StartTask("Backdrop -Remove");
  }

  Event_CloseDown();

  return 0;
}

