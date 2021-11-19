/*
 * This file is part of Sargasso, http://zamez.org/sargasso
 * Licensed under the GNU General Public License,
 *                http://www.opensource.org/licenses/gpl-license
 * Copyright 2006 James Bursa <james@zamez.org>
 */

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <oslib/colourtrans.h>
#include <oslib/osfile.h>
#include <oslib/radiobutton.h>
#include <oslib/uri.h>
#include <oslib/taskmanager.h>
#include <oslib/wimp.h>
#include <oslib/wimpspriteop.h>
#include <rufl.h>
#include "feed.h"
#ifdef FORTIFY
#include "fortify.h"
#endif

#ifdef DMALLOC
#include "dmalloc.h"
#endif

#define MAX_LINES 200
#define MARGIN 10
#define FEEDS_READ "Choices:Sargasso.Feeds"
#define FEEDS_WRITE "<Choices$Write>.Sargasso.Feeds"
#define EXCLUDE_READ "Choices:Sargasso.Exclude"
#define EXCLUDE_WRITE "<Choices$Write>.Sargasso.Exclude"
#define CHOICES_READ "Choices:Sargasso.Choices"
#define CHOICES_WRITE "<Choices$Write>.Sargasso.Choices"
#define WRITE_DIR "<Choices$Write>.Sargasso"

#define WINDOW_WIDTH 1016

#define LOG(x) (printf(__FILE__ " %s %i: ", __PRETTY_FUNCTION__, __LINE__), printf x, fputc('\n', stdout));

typedef void (*click_callback) (unsigned int i);

struct paragraph
{
  int x0, y0, x1, y1;
  int background, colour;
  const char *font_family;
  rufl_style font_style;
  unsigned int font_size;
  click_callback click;
  unsigned int click_i;
  unsigned int lines;
  const xmlChar *text[MAX_LINES + 1];
  struct paragraph *next;
};

bool quit = false;
int interval = 5 * 60;
unsigned int font_size_main = 18;
unsigned int max_fetches = 5;
unsigned int feed_description = 0;
//unsigned int font_size = 18;
char http_proxy[255] = "";
char quality[] = "best";
os_t last_update = 0;

char type1[7] = "";
char type2[7] = "";
char type3[7] = "";
char type4[7] = "";

char exclude1[1030] = "";
char exclude2[1030] = "";
char exclude3[1030] = "";
char exclude4[1030] = "";

wimp_t task;
wimp_w info_window, main_window, feed_window, add_feed_window,
  warning_window = 0, choices_window, exclude_window;
osspriteop_area *sprites;
struct paragraph *main_window_paragraphs = 0;
struct paragraph *feed_window_paragraphs = 0;
unsigned int current_feed = 0;
char font_headings[200] = "Homerton";
char font_summaries[200] = "NewHall";
char font_links[200] = "Homerton";

#define ICON_FLAGS (wimp_ICON_TEXT | \
		(wimp_COLOUR_BLACK << wimp_ICON_FG_COLOUR_SHIFT))
wimp_MENU (5) iconbar_menu = { {"Sargasso"}, wimp_COLOUR_BLACK,
wimp_COLOUR_LIGHT_GREY, wimp_COLOUR_BLACK, wimp_COLOUR_WHITE,
200, 44, 0,
{{0, 0, ICON_FLAGS, {"Info"}},
 {0, 0, ICON_FLAGS, {"Choices..."}},
 {0, 0, ICON_FLAGS, {"Exclude..."}},
 {0, 0, ICON_FLAGS, {"Update feeds"}},
 {wimp_MENU_LAST, 0, ICON_FLAGS, {"Quit"}}}
};
wimp_MENU (2) main_menu = { {"Sargasso"}, wimp_COLOUR_BLACK,
wimp_COLOUR_LIGHT_GREY, wimp_COLOUR_BLACK, wimp_COLOUR_WHITE,
200, 44, 0,
{{0, 0, ICON_FLAGS, {"Add feed..."}},
 {wimp_MENU_LAST, 0, ICON_FLAGS, {"Remove feed"}}}
};

wimp_menu *current_menu;
unsigned int current_removing = 0;
wimp_i current_font_menu;

const char *default_feeds[] = {
  "https://news.google.co.uk/?output=rss/?format=rss&edition=uk",
  "https://www.ft.com/?output=rss/?format=rss&eidtion=uk",
  "http://newsrss.bbc.co.uk/rss/newsonline_uk_edition/front_page/rss.xml",
  "http://rss.cnn.com/rss/edition.rss",
  "http://www.drobe.co.uk/rss.php",
  "http://www.theregister.co.uk/feeds/latest.rdf",
  "http://www.iconbar.co.uk/rss-rss20.php",
  "http://www.mode7games.com/blog/?feed=rss2",
  "http://www.riscosopen.org/forum/posts.rss",
};

void gui_init (void);
void gui_quit (void);
wimp_w create_window (const char *name);
void gui_poll (void);
void mouse_click (wimp_w w, wimp_i i, int x, int y, wimp_mouse_state buttons);
void set_pointer (const char *name, int x, int y);
void reset_pointer (void);
void key_pressed (wimp_key * key);
void menu_selection (wimp_selection * selection);
void open_window (wimp_w w);
void close_window (wimp_w w);
void open_menu (wimp_menu * menu, int x, int y);
void update_main_window (void);
void set_extent (wimp_w w, int y);
void click_main_window (unsigned int i);
void update_feed_window (unsigned int i);
void click_feed_link (unsigned int i);
void click_item_link (unsigned int j);
struct paragraph *add_paragraph (struct paragraph **paragraph_list,
				 int x0, int y0, int x1, int background,
				 int colour, const char *font_family,
				 rufl_style font_style,
				 unsigned int font_size, const xmlChar * text,
				 click_callback click, unsigned int click_i);
void free_paragraph_list (struct paragraph **paragraph_list);
void redraw_window (wimp_draw * redraw, struct paragraph *paragraphs);
void fill_rectangle (int x0, int y0, int x1, int y1, int colour);
void choices_save (void);
void choices_load (void);
void exclude_save (void);
void exclude_load (void);
void die (const char *error);
void warn (const char *warning);
char *get_icon_string (wimp_w w, wimp_i i);
void set_icon_string (wimp_w w, wimp_i i, const char *text);
void set_icon_selected (wimp_w w, wimp_i i, int feed_description_main);
int get_icon_selected (wimp_w w, wimp_i i);


int
main (int argc, char *argv[])
{
  char error[255];
  struct stat s;

/* 	memdebug_memdebug("memdump"); */

  if (!feed_init ())
    die (feed_error);

  interval = (5 + time (0) % 20) * 60;
  choices_load ();
  exclude_load ();
  gui_init ();

  if (stat (FEEDS_READ, &s))
    {
      warn ("Welcome to Sargasso! A selection of feeds have been "
	    "added. To add more feeds, use the main menu.");
      mkdir (WRITE_DIR, S_IRWXU);
      for (unsigned int i = 0; i != sizeof default_feeds /
	   sizeof default_feeds[0]; i++)
	feed_add (default_feeds[i]);
      feed_list_save (FEEDS_WRITE);
    }
  else if (!feed_list_load (FEEDS_READ))
    {
      snprintf (error, sizeof error, "Unable to load feed list: %s",
		feed_error);
      warn (error);
    }

  update_main_window ();
  open_window (main_window);

  last_update = os_read_monotonic_time ();

  while (!quit)
    gui_poll ();

  gui_quit ();
  feed_quit ();

  return 0;
}


void
gui_init (void)
{
  rufl_code code;
  const wimp_MESSAGE_LIST (1) messages = { {message_QUIT} };
  int size;
  fileswitch_object_type obj_type;
  wimp_icon_create icon = {
    wimp_ICON_BAR_RIGHT,
    {{0, 0, 68, 68},
     wimp_ICON_SPRITE | wimp_ICON_HCENTRED | wimp_ICON_VCENTRED |
     (wimp_BUTTON_CLICK << wimp_ICON_BUTTON_TYPE_SHIFT),
     {"!sargasso"}}
  };
  os_error *error;

  error = xwimp_initialise (wimp_VERSION_RO3, "Sargasso",
			    (const wimp_message_list *) &messages, 0, &task);
  if (error)
    {
      LOG (("xwimp_initialise: 0x%x: %s", error->errnum, error->errmess));
      die (error->errmess);
    }

  code = rufl_init ();
  if (code != rufl_OK)
    {
      LOG (("rufl_init: %i", code));
      die ("Failed to initialise Unicode font library");
    }

  error = xosfile_read_stamped_no_path ("<Sargasso$Dir>.Sprites",
					&obj_type, 0, 0, &size, 0, 0);
  if (error)
    {
      LOG (("xosfile_read_stamped_no_path: 0x%x: %s",
	    error->errnum, error->errmess));
      die (error->errmess);
    }
  if (obj_type != fileswitch_IS_FILE)
    die ("Sprites file missing");
  sprites = malloc (size + 4);
  if (!sprites)
    die ("Out of memory");
  sprites->size = size + 4;
  sprites->sprite_count = 0;
  sprites->first = 16;
  sprites->used = 16;
  error = xosspriteop_load_sprite_file (osspriteop_USER_AREA,
					sprites, "<Sargasso$Dir>.Sprites");
  if (error)
    {
      LOG (("xosspriteop_load_sprite_file: 0x%x: %s",
	    error->errnum, error->errmess));
      die (error->errmess);
    }

  error = xwimp_create_icon (&icon, 0);
  if (error)
    {
      LOG (("xwimp_create_icon: 0x%x: %s", error->errnum, error->errmess));
      die (error->errmess);
    }

  error = xwimp_open_template ("<Sargasso$Dir>.Templates");
  if (error)
    {
      LOG (("xwimp_open_template: 0x%x: %s", error->errnum, error->errmess));
      die (error->errmess);
    }

  info_window = create_window ("info");
  main_window = create_window ("main");
  feed_window = create_window ("main");
  add_feed_window = create_window ("add_feed");
  warning_window = create_window ("warning");
  choices_window = create_window ("choices");
  exclude_window = create_window ("exclude");

  iconbar_menu.entries[0].sub_menu = (wimp_menu *) info_window;

  error = xwimp_close_template ();
  if (error)
    {
      LOG (("xwimp_close_template: 0x%x: %s", error->errnum, error->errmess));
      die (error->errmess);
    }
}


void
gui_quit (void)
{
  free_paragraph_list (&main_window_paragraphs);
  free_paragraph_list (&feed_window_paragraphs);
  rufl_quit ();
}


wimp_w
create_window (const char *name)
{
  char name_buf[12];
  int window_size;
  int data_size;
  wimp_window *window;
  char *data;
  wimp_w w;
  os_error *error;

  assert (strlen (name) < 12);

  strncpy (name_buf, name, sizeof name_buf);

  error = xwimp_load_template (wimp_GET_SIZE, 0, 0, wimp_NO_FONTS,
			       name_buf, 0, &window_size, &data_size, 0);
  if (error)
    {
      LOG (("xwimp_load_template: 0x%x: %s", error->errnum, error->errmess));
      die (error->errmess);
    }

  window = malloc (window_size);
  data = malloc (data_size);
  if (!window || !data)
    die ("Out of memory");

  error = xwimp_load_template (window, data, data + data_size,
			       wimp_NO_FONTS, name_buf, 0, 0, 0, 0);
  if (error)
    {
      LOG (("xwimp_load_template: 0x%x: %s", error->errnum, error->errmess));
      die (error->errmess);
    }

  error = xwimp_create_window (window, &w);
  if (error)
    {
      LOG (("xwimp_create_window: 0x%x: %s", error->errnum, error->errmess));
      die (error->errmess);
    }

  return w;
}


void
gui_poll (void)
{
  static bool in_window = false;
  os_t t;
  wimp_block block;
  wimp_event_no event;
  wimp_pointer pointer;
  osbool more;
  os_error *error;

  t = os_read_monotonic_time ();

  error = xwimp_poll_idle (0,
			   &block,
			   feed_work_needed || in_window ? t + 5 : t + 100,
			   0, &event);
  if (error)
    {
      LOG (("xwimp_poll: 0x%x: %s", error->errnum, error->errmess));
      warn (error->errmess);
    }

  switch (event)
    {
    case wimp_NULL_REASON_CODE:
      if (feed_work_needed)
	{
	  if (feed_work ())
	    {
	      update_main_window ();
	      if (feed_count && feeds[current_feed].updated)
		update_feed_window (current_feed);
	    }
	}

      t = os_read_monotonic_time ();
      if (last_update + interval * 100 <= t)
	{
	  LOG (("updating"));
	  last_update = t;
	  feed_update ();
	}

      error = xwimp_get_pointer_info (&pointer);
      if (error)
	{
	  LOG (("xwimp_get_pointer_info: 0x%x: %s",
		error->errnum, error->errmess));
	  warn (error->errmess);
	}
      mouse_click (pointer.w, pointer.i, pointer.pos.x, pointer.pos.y, 0);
      break;

    case wimp_REDRAW_WINDOW_REQUEST:
      error = xwimp_redraw_window (&block.redraw, &more);
      if (error)
	{
	  LOG (("xwimp_redraw_window: 0x%x: %s",
		error->errnum, error->errmess));
	  break;
	}
      while (more)
	{
	  if (block.redraw.w == main_window)
	    redraw_window (&block.redraw, main_window_paragraphs);
	  else if (block.redraw.w == feed_window)
	    redraw_window (&block.redraw, feed_window_paragraphs);
	  error = xwimp_get_rectangle (&block.redraw, &more);
	  if (error)
	    {
	      LOG (("xwimp_get_rectangle: 0x%x: %s",
		    error->errnum, error->errmess));
	      break;
	    }
	}
      break;

    case wimp_OPEN_WINDOW_REQUEST:
      error = xwimp_open_window (&block.open);
      if (error)
	{
	  LOG (("xwimp_open_window: 0x%x: %s",
		error->errnum, error->errmess));
	  warn (error->errmess);
	}
      break;

    case wimp_CLOSE_WINDOW_REQUEST:
      error = xwimp_close_window (block.close.w);
      if (error)
	{
	  LOG (("xwimp_close_window: 0x%x: %s",
		error->errnum, error->errmess));
	  warn (error->errmess);
	}
      break;

    case wimp_POINTER_LEAVING_WINDOW:
      reset_pointer ();
      in_window = false;
      break;

    case wimp_POINTER_ENTERING_WINDOW:
      in_window = true;
      break;

    case wimp_MOUSE_CLICK:
      mouse_click (block.pointer.w, block.pointer.i,
		   block.pointer.pos.x, block.pointer.pos.y,
		   block.pointer.buttons);
      break;

    case wimp_KEY_PRESSED:
      key_pressed (&block.key);
      break;

    case wimp_MENU_SELECTION:
      menu_selection (&block.selection);
      break;

    case wimp_USER_MESSAGE:
    case wimp_USER_MESSAGE_RECORDED:
      switch (block.message.action)
	{
	case message_QUIT:
	  quit = true;
	  break;
	}
      break;
    }
}


void
mouse_click (wimp_w w, wimp_i i, int x, int y, wimp_mouse_state buttons)
{
  wimp_window_state state;
  int wx, wy;
  struct paragraph *p = NULL;
  char minutes[10];
  char maxfetches[2];
  char fontmain[3];
  int mins;
  os_error *error;

  if (w == wimp_ICON_BAR)
    {
      if (buttons == wimp_CLICK_MENU)
	open_menu ((wimp_menu *) & iconbar_menu, x - 64, 96 + 44 * 5);
      else if (buttons == wimp_CLICK_SELECT || buttons == wimp_CLICK_ADJUST)
	open_window (main_window);

    }
  else if (w == main_window || w == feed_window)
    {
      state.w = w;
      error = xwimp_get_window_state (&state);
      if (error)
	{
	  LOG (("xwimp_get_window_state: 0x%x: %s",
		error->errnum, error->errmess));
	  warn (error->errmess);
	}
      wx = x - (state.visible.x0 - state.xscroll);
      wy = -(y - (state.visible.y1 - state.yscroll));
      if (w == main_window)
	p = main_window_paragraphs;
      else
	p = feed_window_paragraphs;
      for (; p; p = p->next)
	{
	  if (p->x0 <= wx && wx <= p->x1 &&
	      p->y0 <= wy && wy <= p->y1 && p->click)
	    break;
	}
      if (p && buttons == wimp_CLICK_SELECT)
	p->click (p->click_i);
      else if (buttons == wimp_CLICK_MENU && w == main_window)
	{
	  if (p)
	    main_menu.entries[1].icon_flags &= ~wimp_ICON_SHADED;
	  else
	    main_menu.entries[1].icon_flags |= wimp_ICON_SHADED;
	  open_menu ((wimp_menu *) & main_menu, x - 64, y);
	  current_removing = p ? p->click_i : 0;
	}
      else if (buttons == 0)
	{
	  if (p)
	    set_pointer ("ptr_point", 6, 0);
	  else
	    reset_pointer ();
	}

    }
  else if (w == add_feed_window && buttons)
    {
      if (i == 1)
	{
	  feed_add (get_icon_string (add_feed_window, 0));
	  update_main_window ();
	  if (!feed_list_save (FEEDS_WRITE))
	    warn (feed_error);
	  if (buttons == wimp_CLICK_SELECT)
	    close_window (add_feed_window);
	}
      else if (i == 2)
	{
	  close_window (add_feed_window);
	}

    }
  else if (w == warning_window && buttons)
    {
      if (i == 1)
	close_window (warning_window);

    }
  else if (w == exclude_window && buttons)
    {
      if (i == 1)
	{
	  strcpy (type1, get_icon_string (exclude_window, 6));
	  strcpy (type2, get_icon_string (exclude_window, 7));
	  strcpy (type3, get_icon_string (exclude_window, 8));
	  strcpy (type4, get_icon_string (exclude_window, 9));
	  strcpy (exclude1, get_icon_string (exclude_window, 2));
	  strcpy (exclude2, get_icon_string (exclude_window, 3));
	  strcpy (exclude3, get_icon_string (exclude_window, 4));
	  strcpy (exclude4, get_icon_string (exclude_window, 5));
	  exclude_save ();
	  if (buttons == wimp_CLICK_SELECT)
	    close_window (exclude_window);
	}
      else if (i == 0)
	close_window (exclude_window);

    }
  else if (w == choices_window && buttons)
    {
      if (i == 6)
	{
	  mins = atoi (get_icon_string (choices_window, 1));
	  if (mins < 5)
	    {
	      mins = 5;
	      warn ("To avoid overloading feed providers, "
		    "the minimum update interval " "is 5 minutes.");
	    }
	  max_fetches = atoi (get_icon_string (choices_window, 19));
	  if (max_fetches < 1)
	    {
	      max_fetches = 1;
	    }
	  else if (max_fetches > 9)
	    {
	      max_fetches = 9;
	    }
	  interval = mins * 60;
	  font_size_main = atoi (get_icon_string (choices_window, 26));
	  feed_description = get_icon_selected (choices_window, 25);
	  if (font_size_main < 10)
	    font_size_main = 10;
	  else if (font_size_main > 22)
	    font_size_main = 22;
	  feed_description = get_icon_selected (choices_window, 25);
	  strcpy (http_proxy, get_icon_string (choices_window, 18));
	  strcpy (quality, get_icon_string (choices_window, 24));
	  choices_save ();
	  if (buttons == wimp_CLICK_SELECT)
	    close_window (choices_window);
	}
      else if (i == 5)
	{
	  close_window (choices_window);
	}
      else if (i == 2 || i == 3)
	{
	  mins = atoi (get_icon_string (choices_window, 1));
	  if ((i == 2 && buttons == wimp_CLICK_SELECT) ||
	      (i == 3 && buttons == wimp_CLICK_ADJUST))
	    mins -= 3;
	  else
	    mins += 3;
	  if (mins < 5)
	    mins = 5;
	  else if (999 < mins)
	    mins = 999;
	  snprintf (minutes, sizeof minutes, "%i", mins);
	  set_icon_string (choices_window, 1, minutes);
	  xwimp_set_caret_position (choices_window, 1,
				    0, 0, -1, strlen (minutes));
	}
      else if (i == 21 || i == 22)
	{
	  max_fetches = atoi (get_icon_string (choices_window, 19));
	  if ((i == 21 && buttons == wimp_CLICK_SELECT) ||
	      (i == 22 && buttons == wimp_CLICK_ADJUST))
	    max_fetches -= 1;
	  else
	    max_fetches += 1;
	  if (max_fetches < 1)
	    max_fetches = 1;
	  else if (max_fetches > 9)
	    max_fetches = 9;
	  snprintf (maxfetches, sizeof maxfetches, "%i", max_fetches);
	  set_icon_string (choices_window, 19, maxfetches);
	  xwimp_set_caret_position (choices_window, 19,
				    0, 0, -1, strlen (maxfetches));
	}
      else if (i == 28 || i == 29)
	{
	  font_size_main = atoi (get_icon_string (choices_window, 26));
	  if ((i == 28 && buttons == wimp_CLICK_SELECT) ||
	      (i == 29 && buttons == wimp_CLICK_ADJUST))
	    font_size_main -= 1;
	  else
	    font_size_main += 1;
	  if (font_size_main < 10)
	    font_size_main = 10;
	  else if (font_size_main > 22)
	    font_size_main = 22;
	  snprintf (fontmain, sizeof fontmain, "%i", font_size_main);
	  set_icon_string (choices_window, 26, fontmain);
	  xwimp_set_caret_position (choices_window, 26,
				    0, 0, -1, strlen (fontmain));
	}
      else if (i == 25)
	{
	  if ((i == 25 && buttons == wimp_CLICK_SELECT) ||
	      (i == 25 && buttons == wimp_CLICK_ADJUST))
		feed_description = !feed_description;
	  	set_icon_selected (choices_window, 25, feed_description);
	}
      else if (i == 7 || i == 8 || i == 10 || i == 11 || i == 13 || i == 14)
	{
	  if (i == 8 || i == 11 || i == 14)
	    i--;
	  wimp_window_state state;
	  wimp_icon_state icon_state;
	  state.w = w;
	  xwimp_get_window_state (&state);
	  icon_state.w = w;
	  icon_state.i = i;
	  xwimp_get_icon_state (&icon_state);
	  open_menu ((wimp_menu *) rufl_family_menu,
		     state.visible.x0 + icon_state.icon.extent.x1,
		     state.visible.y1 + icon_state.icon.extent.y1);
	  current_font_menu = i;
	}
    }
}


void
set_pointer (const char *name, int x, int y)
{
  os_error *error;
  error = xosspriteop_set_pointer_shape (osspriteop_USER_AREA, sprites,
					 (osspriteop_id) name, 1, x, y, 0, 0);
  if (error)
    {
      LOG (("xosspriteop_set_pointer_shape: 0x%x: %s",
	    error->errnum, error->errmess));
      warn (error->errmess);
    }
}


void
reset_pointer (void)
{
  os_error *error;
  error = xwimpspriteop_set_pointer_shape ("ptr_default", 1, 0, 0, 0, 0);
  if (error)
    {
      LOG (("xwimpspriteop_set_pointer_shape: 0x%x: %s",
	    error->errnum, error->errmess));
      warn (error->errmess);
    }
}


void
key_pressed (wimp_key * key)
{
  os_error *error;

  if (key->w == add_feed_window)
    {
      if (key->c == wimp_KEY_ESCAPE)
	{
	  mouse_click (key->w, 2, 0, 0, wimp_CLICK_SELECT);
	  return;
	}
      else if (key->c == wimp_KEY_RETURN)
	{
	  mouse_click (key->w, 1, 0, 0, wimp_CLICK_SELECT);
	  return;
	}

    }
  else if (key->w == choices_window)
    {
      if (key->c == wimp_KEY_ESCAPE)
	{
	  mouse_click (key->w, 5, 0, 0, wimp_CLICK_SELECT);
	  return;
	}
      else if (key->c == wimp_KEY_RETURN)
	{
	  mouse_click (key->w, 6, 0, 0, wimp_CLICK_SELECT);
	  return;
	}
      else if (key->c == wimp_KEY_DOWN)
	{
	  mouse_click (key->w, 2, 0, 0, wimp_CLICK_SELECT);
	  return;
	}
      else if (key->c == wimp_KEY_UP)
	{
	  mouse_click (key->w, 3, 0, 0, wimp_CLICK_SELECT);
	  return;
	}

    }
  else if (key->w == exclude_window)
    {
      if (key->c == wimp_KEY_ESCAPE)
	{
	  mouse_click (key->w, 0, 0, 0, wimp_CLICK_SELECT);
	  return;
	}
      else if (key->c == wimp_KEY_RETURN)
	{
	  mouse_click (key->w, 1, 0, 0, wimp_CLICK_SELECT);
	  return;
	}

    }
  else if (key->w == warning_window)
    {
      if (key->c == wimp_KEY_ESCAPE || key->c == wimp_KEY_RETURN)
	{
	  close_window (warning_window);
	}
    }

  error = xwimp_process_key (key->c);
  if (error)
    {
      LOG (("xwimp_process_key: 0x%x: %s", error->errnum, error->errmess));
      warn (error->errmess);
    }
}


void
menu_selection (wimp_selection * selection)
{
  char minutes[10];
  char maxfetches[2];
  char fontmain[3];
  wimp_pointer pointer;

  xwimp_get_pointer_info (&pointer);

  if (current_menu == (wimp_menu *) & iconbar_menu)
    {
      switch (selection->items[0])
	{
	case 1:
	  snprintf (minutes, sizeof minutes, "%i", interval / 60);
	  snprintf (maxfetches, sizeof maxfetches, "%i", max_fetches);
	  snprintf (fontmain, sizeof fontmain, "%i", font_size_main);
	  set_icon_string (choices_window, 18, http_proxy);
	  set_icon_string (choices_window, 1, minutes);
	  set_icon_string (choices_window, 19, maxfetches);
	  set_icon_string (choices_window, 24, quality);
	  set_icon_string (choices_window, 26, fontmain);
	  set_icon_selected (choices_window, 25, feed_description);
	  set_icon_string (choices_window, 8, font_headings);
	  set_icon_string (choices_window, 11, font_summaries);
	  set_icon_string (choices_window, 14, font_links);
	  open_window (choices_window);
	  xwimp_set_caret_position (choices_window, 1,
				    0, 0, -1, strlen (minutes));
	  break;
	case 2:

	  set_icon_string (exclude_window, 6, type1);
	  set_icon_string (exclude_window, 7, type2);
	  set_icon_string (exclude_window, 8, type3);
	  set_icon_string (exclude_window, 9, type4);
	  set_icon_string (exclude_window, 2, exclude1);
	  set_icon_string (exclude_window, 3, exclude2);
	  set_icon_string (exclude_window, 4, exclude3);
	  set_icon_string (exclude_window, 5, exclude4);
	  open_window (exclude_window);
	  xwimp_set_caret_position (exclude_window, 6,
				    0, 0, -1, strlen (type1));
	  break;
	case 3:
	  last_update = os_read_monotonic_time ();
	  feed_update ();
	  break;
	case 4:
	  quit = true;
	  break;
	}

    }
  else if (current_menu == (wimp_menu *) & main_menu)
    {
      switch (selection->items[0])
	{
	case 0:
	  set_icon_string (add_feed_window, 0, "http://");
	  open_window (add_feed_window);
	  xwimp_set_caret_position (add_feed_window, 0, 0, 0, -1, 7);
	  break;
	case 1:
	  feed_remove (current_removing);
	  update_main_window ();
	  close_window (feed_window);
	  if (!feed_list_save (FEEDS_WRITE))
	    warn (feed_error);
	  break;
	}
    }
  else if (current_menu == (wimp_menu *) rufl_family_menu)
    {
      char *font =
	((wimp_menu *) rufl_family_menu)->entries[selection->items[0]].data.
	indirected_text.text;
      if (current_font_menu == 7)
	strncpy (font_headings, font, sizeof font_headings);
      else if (current_font_menu == 10)
	strncpy (font_summaries, font, sizeof font_summaries);
      else if (current_font_menu == 13)
	strncpy (font_links, font, sizeof font_links);
      set_icon_string (choices_window, 8, font_headings);
      set_icon_string (choices_window, 11, font_summaries);
      set_icon_string (choices_window, 14, font_links);
      update_main_window ();
      if (feed_count)
	update_feed_window (current_feed);
    }

  if (pointer.buttons == wimp_CLICK_ADJUST)
    xwimp_create_menu (current_menu, pointer.pos.x, pointer.pos.y);
}


void
open_window (wimp_w w)
{
  const os_VDU_VAR_LIST (5) vars = { {os_MODEVAR_XWIND_LIMIT,
				      os_MODEVAR_YWIND_LIMIT,
				      os_MODEVAR_XEIG_FACTOR,
				      os_MODEVAR_YEIG_FACTOR,
				      os_VDUVAR_END_LIST}
  };
  int vals[4];
  int width, height;
  wimp_window_state state;
  int dx, dy;
  os_error *error;

  error = xos_read_vdu_variables ((const os_vdu_var_list *) &vars, vals);
  if (error)
    {
      LOG (("xos_read_vdu_variables: 0x%x: %s",
	    error->errnum, error->errmess));
      warn (error->errmess);
      return;
    }
  width = (vals[0] + 1) << vals[2];
  height = (vals[1] + 1) << vals[3];

  state.w = w;
  error = xwimp_get_window_state (&state);
  if (error)
    {
      LOG (("xwimp_get_window_state: 0x%x: %s",
	    error->errnum, error->errmess));
      warn (error->errmess);
      return;
    }

  dx = (state.visible.x1 - state.visible.x0) / 2;
  dy = (state.visible.y1 - state.visible.y0) / 2;
  state.visible.x0 = width / 2 - dx;
  state.visible.y0 = height / 2 - dy;
  state.visible.x1 = width / 2 + dx;
  state.visible.y1 = height / 2 + dy;
  state.xscroll = 0;
  state.yscroll = 0;
  state.next = wimp_TOP;

  error = xwimp_open_window ((wimp_open *) & state);
  if (error)
    {
      LOG (("xwimp_open_window: 0x%x: %s", error->errnum, error->errmess));
      warn (error->errmess);
    }
}


void
close_window (wimp_w w)
{
  os_error *error;

  error = xwimp_close_window (w);
  if (error)
    {
      LOG (("xwimp_close_window: 0x%x: %s", error->errnum, error->errmess));
      warn (error->errmess);
    }
}


void
open_menu (wimp_menu * menu, int x, int y)
{
  os_error *error;

  error = xwimp_create_menu (menu, x, y);
  if (error)
    {
      LOG (("xwimp_create_menu: 0x%x: %s", error->errnum, error->errmess));
      warn (error->errmess);
    }
  current_menu = menu;
}


void
update_main_window (void)
{
  static char *status = 0;
  int y = 0;
  int y1;
  unsigned int i;
  unsigned int j;
  unsigned int new_items;
  rufl_style style;
  struct paragraph *p;

  free_paragraph_list (&main_window_paragraphs);
  if (status)
    {
      free (status);
      status = 0;
    }

  status = malloc (feed_count * 40);
  if (!status)
    {
      LOG (("out of memory"));
      return;
    }

  for (i = 0; i != feed_count; i++)
    {
      new_items = 0;
      for (j = 0; j != feeds[i].item_count; j++)
	if (feeds[i].item[j].new_item)
	  new_items++;
      style = new_items ? rufl_WEIGHT_900 : rufl_WEIGHT_400;

      p = add_paragraph (&main_window_paragraphs, 0, y, 700,
			 0xffaa99, 0x000000,
			 font_headings, style, font_size_main * 10,
			 feeds[i].title ? (char *) feeds[i].title :
			 (const xmlChar *) feeds[i].url,
			 click_main_window, i);
      y1 = p->y1;

      switch (feeds[i].status)
	{
	case FEED_NEW:
	case FEED_FETCHING:
	case FEED_UPDATE:
	  snprintf (status + i * 40, 40, "Fetching");
	  break;
	case FEED_OK:
	  if (new_items)
	    snprintf (status + i * 40, 40,
		      "%i items (%i new)", feeds[i].item_count, new_items);
	  else
	    snprintf (status + i * 40, 40, "%i items", feeds[i].item_count);
	  break;
	case FEED_ERROR:
	  snprintf (status + i * 40, 40, "Failed");
	  break;
	}
      p = add_paragraph (&main_window_paragraphs, 700, y, WINDOW_WIDTH,
			 0xffaa99, 0x000000,
			 font_headings, style, font_size_main * 10,
			 (const xmlChar *) status + i * 40,
			 click_main_window, i);
      y = p->y1 = y1;

      if (feeds[i].error)
	{
	  p = add_paragraph (&main_window_paragraphs, 0, y, WINDOW_WIDTH,
			     0, 0x0000a0,
			     font_summaries, style, 200,
			     (const xmlChar *) feeds[i].error,
			     click_main_window, i);
	  y = p->y1;
	}

      if (feeds[i].description && feed_description)
	{
	  p = add_paragraph (&main_window_paragraphs, 0, y, WINDOW_WIDTH,
			     0, 0x000000,
			     font_summaries, rufl_WEIGHT_400, 200,
			     feeds[i].description, click_main_window, i);
	  y = p->y1;
	}

      y += MARGIN;
    }

  set_extent (main_window, y);
}


void
set_extent (wimp_w w, int y)
{
  os_box extent = { 0, 0, WINDOW_WIDTH, 0 };
  wimp_window_state state;
  os_error *error;

  extent.y0 = -y;
  error = xwimp_set_extent (w, &extent);
  if (error)
    {
      LOG (("xwimp_set_extent: 0x%x: %s", error->errnum, error->errmess));
      warn (error->errmess);
    }

  state.w = w;
  error = xwimp_get_window_state (&state);
  if (error)
    {
      LOG (("xwimp_get_window_state: 0x%x: %s",
	    error->errnum, error->errmess));
      warn (error->errmess);
    }

  if (state.flags & wimp_WINDOW_OPEN)
    {
      error = xwimp_close_window (w);
      if (error)
	{
	  LOG (("xwimp_close_window: 0x%x: %s",
		error->errnum, error->errmess));
	  warn (error->errmess);
	}

      /*state.visible.y0 = 100;
         state.visible.y1 = 100 + y; */
      error = xwimp_open_window ((wimp_open *) & state);
      if (error)
	{
	  LOG (("xwimp_open_window: 0x%x: %s",
		error->errnum, error->errmess));
	  warn (error->errmess);
	}
    }
}


void
click_main_window (unsigned int i)
{
  unsigned int j;

  update_feed_window (i);
  open_window (feed_window);

  for (j = 0; j != feeds[i].item_count; j++)
    feeds[i].item[j].new_item = false;
  update_main_window ();
}


void
update_feed_window (unsigned int i)
{
  int y = 0;
  int y1;
  unsigned int j, k;
  struct paragraph *p;
  rufl_style style;

  current_feed = i;

  free_paragraph_list (&feed_window_paragraphs);

  p = add_paragraph (&feed_window_paragraphs, 0, y, WINDOW_WIDTH,
		     0xffaa99, 0x000000,
		     font_headings, rufl_WEIGHT_400, font_size_main * 10,
		     (const xmlChar *) feeds[i].title ? (const xmlChar *)
		     feeds[i].title : (const xmlChar *) feeds[i].url, 0, 0);
  y = p->y1;

  if (feeds[i].link)
    {
      p = add_paragraph (&feed_window_paragraphs, 0, y, WINDOW_WIDTH,
			 0, 0xff0000,
			 font_links, rufl_WEIGHT_400, 160,
			 feeds[i].link, click_feed_link, i);
      y = p->y1;
    }
  if (feeds[i].pub_date)
    {
      p = add_paragraph (&feed_window_paragraphs, 0, y, WINDOW_WIDTH,
			 0, 0x0060ee,
			 font_summaries, rufl_WEIGHT_400 | rufl_SLANTED,
			 160, feeds[i].pub_date, 0, 0);
      y = p->y1;
    }
  if (feeds[i].description)
    {
      p = add_paragraph (&feed_window_paragraphs, 0, y, WINDOW_WIDTH,
			 0, 0x000000,
			 font_summaries, rufl_WEIGHT_400, 200,
			 feeds[i].description, 0, 0);
      y = p->y1;
    }
  if (feeds[i].copyright)
    {
      p = add_paragraph (&feed_window_paragraphs, 0, y, WINDOW_WIDTH,
			 0, 0x666666,
			 font_summaries, rufl_WEIGHT_400 | rufl_SLANTED, 160,
			 feeds[i].copyright, 0, 0);
      y = p->y1;
    }

  y += MARGIN;

  for (j = 0; j != feeds[i].item_count; j++)
    {
      style = feeds[i].item[j].new_item ? rufl_WEIGHT_900 : rufl_WEIGHT_400;

      p = add_paragraph (&feed_window_paragraphs, 0, y, WINDOW_WIDTH,
			 0xa0dddd, 0x000000,
			 font_headings, style, 200,
			 feeds[i].item[j].title ?
			 (const xmlChar *) feeds[i].item[j].
			 title : (const xmlChar *) "", 0, j);
      y1 = y = p->y1;

      if (feeds[i].item[j].pub_date)
	{
	  p = add_paragraph (&feed_window_paragraphs, 0, y, 420,
			     0, 0x0060ee,
			     font_summaries,
			     rufl_WEIGHT_400 | rufl_SLANTED, 160,
			     feeds[i].item[j].pub_date, 0, j);
	  if (y1 < p->y1)
	    y1 = p->y1;
	}
      if (feeds[i].item[j].author)
	{
	  p = add_paragraph (&feed_window_paragraphs, 420, y, 720,
			     0, 0x666666,
			     font_summaries,
			     rufl_WEIGHT_400 | rufl_SLANTED, 160,
			     feeds[i].item[j].author, 0, j);
	  if (y1 < p->y1)
	    y1 = p->y1;
	}
      if (feeds[i].item[j].category)
	{
	  p = add_paragraph (&feed_window_paragraphs, 720, y, WINDOW_WIDTH,
			     0, 0xee6000,
			     font_summaries, rufl_WEIGHT_400, 160,
			     feeds[i].item[j].category, 0, j);
	  if (y1 < p->y1)
	    y1 = p->y1;
	}
      y = y1;

      if (feeds[i].item[j].link)
	{
	  p = add_paragraph (&feed_window_paragraphs, 0, y, WINDOW_WIDTH,
			     0, 0xff0000,
			     font_links, rufl_WEIGHT_400, 160,
			     feeds[i].item[j].link, click_item_link, j);
	  y = p->y1;
	}

      if (feeds[i].item[j].description)
	{
	  if (feeds[i].item[j].paragraph[0] == NULL)
	    {
	      p = add_paragraph (&feed_window_paragraphs, 0, y, WINDOW_WIDTH,
				 0, 0x000000,
				 font_summaries, rufl_WEIGHT_400, 180,
				 feeds[i].item[j].description, 0, j);
	      y = p->y1;
	    }
	  else
	    {
	      for (k = 0; k != MAX_DESC_ITEMS; k++)
		{
		  if (feeds[i].item[j].paragraph[k] != NULL)
		    {
		      if (feeds[i].item[j].paragraph_style[k] == 1)
			style = rufl_WEIGHT_900;
		      else if (feeds[i].item[j].paragraph_style[k] == 2)
			style = rufl_SLANTED | rufl_WEIGHT_900;
		      else
			style = rufl_WEIGHT_400;
		      p =
			add_paragraph (&feed_window_paragraphs, 0, y,
				       WINDOW_WIDTH, 0, 0x000000,
				       font_summaries, style, 180,
				       feeds[i].item[j].paragraph[k], 0, j);
		      y = p->y1;
		    }
		}
	    }

	  y += MARGIN;

	}
    }

  set_extent (feed_window, y);
}


void
click_feed_link (unsigned int i)
{
  os_error *error;
  error = xuri_dispatch (0, feeds[i].link, task, 0, 0, 0);
  if (error)
    {
      LOG (("xuri_dispatch: 0x%x: %s", error->errnum, error->errmess));
    }
}


void
click_item_link (unsigned int j)
{
  char youtubedl[255];
  os_error *error;

  sprintf (youtubedl, "%s %s %s", "<Sargasso$Dir>.youtubedl", quality,
	   feeds[current_feed].item[j].link);

  if (strncmp
      ((const char *) "https://www.youtube.com/watch?v=",
       (const char *) feeds[current_feed].item[j].link,
       strlen ((const char *) "https://www.youtube.com/watch?v=")) == 0)
    {
      error = xtaskmanager_start_task (youtubedl);
    }
  else
    {
      error = xuri_dispatch (0, feeds[current_feed].item[j].link, task,
			     0, 0, 0);
    }
  if (error)
    {
      LOG (("xuri_dispatch: 0x%x: %s", error->errnum, error->errmess));
    }
}


struct paragraph *
add_paragraph (struct paragraph **paragraph_list,
	       int x0, int y0, int x1, int background, int colour,
	       const char *font_family, rufl_style font_style,
	       unsigned int font_size, const xmlChar * text,
	       click_callback click, unsigned int click_i)
{
  struct paragraph *p;
  const xmlChar *t;
  size_t len = strlen ((const char *) text);
  unsigned int lines = 0;
  size_t char_offset;
  int actual_x;
  rufl_code code;

  p = malloc (sizeof *p);
  if (!p)
    return 0;

  p->x0 = x0;
  p->y0 = y0;
  p->x1 = x1;
  p->background = background;
  p->colour = colour;
  p->font_family = font_family;
  p->font_style = font_style;
  p->font_size = font_size;
  p->click = click;
  p->click_i = click_i;
  p->lines = 0;
  p->text[0] = text;

  t = text;
  while (len && lines != MAX_LINES)
    {
      code = rufl_split (font_family, font_style, font_size, t, len,
			 x1 - x0 - MARGIN - MARGIN, &char_offset, &actual_x);
      if (code != rufl_OK)
	{
	  LOG (("rufl_split: %i", code));
	  break;
	}
      if (char_offset != len)
	{
	  size_t space;
	  for (space = char_offset; space && t[space] != ' '; space--)
	    continue;
	  if (space)
	    char_offset = space + 1;
	}
      len -= char_offset;
      t += char_offset;
      p->text[++lines] = t;
    }

  p->y1 = p->y0 + MARGIN + lines * font_size * 0.2 + MARGIN;
  p->lines = lines;

  p->next = *paragraph_list;
  *paragraph_list = p;

  return p;
}


void
free_paragraph_list (struct paragraph **paragraph_list)
{
  struct paragraph *p = *paragraph_list;
  struct paragraph *next;

  while (p)
    {
      next = p->next;
      free (p);
      p = next;
    }

  *paragraph_list = 0;
}


void
redraw_window (wimp_draw * redraw, struct paragraph *paragraphs)
{
  int ox = redraw->box.x0 - redraw->xscroll;
  int oy = redraw->box.y1 - redraw->yscroll;
  int clip_x0 = redraw->clip.x0 - ox;
  int clip_y0 = oy - redraw->clip.y1;
  int clip_x1 = redraw->clip.x1 - ox;
  int clip_y1 = oy - redraw->clip.y0;
  struct paragraph *p;
  unsigned int i;
  os_error *error;
  rufl_code code;

  for (p = paragraphs; p; p = p->next)
    {
      if (p->x1 < clip_x0 || clip_x1 < p->x0 ||
	  p->y1 < clip_y0 || clip_y1 < p->y0)
	continue;
      if (p->background)
	fill_rectangle (ox + p->x0, oy - p->y1,
			ox + p->x1, oy - p->y0, p->background);
      for (i = 0; i != p->lines; i++)
	{
	  error = xcolourtrans_set_font_colours (font_CURRENT,
						 p->background << 8,
						 p->colour << 8, 14, 0, 0, 0);
	  if (error)
	    {
	      LOG (("xcolourtrans_set_font_colours: 0x%x: %s",
		    error->errnum, error->errmess));
	      return;
	    }
	  code = rufl_paint (p->font_family, p->font_style,
			     p->font_size, p->text[i],
			     p->text[i + 1] - p->text[i],
			     ox + p->x0 + MARGIN,
			     oy - p->y0 - MARGIN -
			     i * p->font_size * 0.2 -
			     p->font_size * 0.15, rufl_BLEND_FONT);
	  if (code != rufl_OK)
	    {
	      LOG (("rufl_paint: %i", code));
	      return;
	    }
	}
    }
}


void
fill_rectangle (int x0, int y0, int x1, int y1, int colour)
{
  os_error *error;

  error = xcolourtrans_set_gcol (colour << 8, colourtrans_USE_ECFS_GCOL,
				 os_ACTION_OVERWRITE, 0, 0);
  if (error)
    {
      LOG (("xcolourtrans_set_gcol: 0x%x: %s",
	    error->errnum, error->errmess));
      return;
    }

  error = xos_plot (os_MOVE_TO, x0, y0);
  if (error)
    {
      LOG (("xos_plot: 0x%x: %s", error->errnum, error->errmess));
      return;
    }

  error = xos_plot (os_PLOT_RECTANGLE | os_PLOT_TO, x1, y1);
  if (error)
    {
      LOG (("xos_plot: 0x%x: %s", error->errnum, error->errmess));
      return;
    }
}


void
choices_save (void)
{
  FILE *choices;
  char error[200];

  choices = fopen (CHOICES_WRITE, "w");
  if (!choices)
    {
      LOG (("fopen: %i: %s", errno, strerror (errno)));
      snprintf (error, sizeof error, "Unable to save choices: %s",
		strerror (errno));
      warn (error);
      return;
    }
  fprintf (choices, "interval: %i\n", interval);
  fprintf (choices, "max_fetches: %i\n", max_fetches);
  fprintf (choices, "font_size_main: %i\n", font_size_main);
  fprintf (choices, "feed_description: %i\n", feed_description);
  fprintf (choices, "proxy: %s\n", http_proxy);
  fprintf (choices, "quality: %s\n", quality);
  fprintf (choices, "font_headings: %s\n", font_headings);
  fprintf (choices, "font_summaries: %s\n", font_summaries);
  fprintf (choices, "font_links: %s\n", font_links);
  fclose (choices);
}


void
choices_load (void)
{
  FILE *choices;
  char error[200];
  char s[100];

  choices = fopen (CHOICES_READ, "r");
  if (!choices)
    {
      LOG (("fopen: %i: %s", errno, strerror (errno)));
      if (errno != ENOENT)
	{
	  snprintf (error, sizeof error,
		    "Unable to read choices: %s", strerror (errno));
	  warn (error);
	}
      return;
    }
  while (fgets (s, sizeof s, choices))
    {
      sscanf (s, "interval: %i", &interval);
      sscanf (s, "max_fetches: %i", &max_fetches);
      sscanf (s, "font_size_main: %i", &font_size_main);
      sscanf (s, "feed_description: %i", &feed_description);
      sscanf (s, "proxy: %s", http_proxy);
      sscanf (s, "font_headings: %s", font_headings);
      sscanf (s, "font_summaries: %s", font_summaries);
      sscanf (s, "font_links: %s", font_links);
    }
  fclose (choices);
}


void
exclude_save (void)
{
  FILE *exclude;
  char error[262];

  exclude = fopen (EXCLUDE_WRITE, "w");
  if (!exclude)
    {
      LOG (("fopen: %i: %s", errno, strerror (errno)));
      snprintf (error, sizeof error, "Unable to save exclude: %s",
		strerror (errno));
      warn (error);
      return;
    }

  fprintf (exclude, "exclude1: %s,%s\n", type1, exclude1);
  fprintf (exclude, "exclude2: %s,%s\n", type2, exclude2);
  fprintf (exclude, "exclude3: %s,%s\n", type3, exclude3);
  fprintf (exclude, "exclude4: %s,%s\n", type4, exclude4);

  fclose (exclude);
}

void
exclude_load (void)
{
  FILE *exclude;
  char error[262];
  char s[1030];

  exclude = fopen (EXCLUDE_READ, "r");
  if (!exclude)
    {
      LOG (("fopen: %i: %s", errno, strerror (errno)));
      if (errno != ENOENT)
	{
	  snprintf (error, sizeof error,
		    "Unable to read exclude: %s", strerror (errno));
	  warn (error);
	}
      return;
    }
  while (fgets (s, sizeof s, exclude))
    {
      sscanf (s, "exclude1: %6[^,],%1023[^\n]", type1, exclude1);
      sscanf (s, "exclude2: %6[^,],%1023[^\n]", type2, exclude2);
      sscanf (s, "exclude3: %6[^,],%1023[^\n]", type3, exclude3);
      sscanf (s, "exclude4: %6[^,],%1023[^\n]", type4, exclude4);

    }
  fclose (exclude);
}


/**
 * Display an error and exit.
 */

void
die (const char *error)
{
  os_error warn_error;

  warn_error.errnum = 1;
  strncpy (warn_error.errmess, error, sizeof warn_error.errmess - 1);
  warn_error.errmess[sizeof warn_error.errmess - 1] = '\0';
  xwimp_report_error_by_category (&warn_error,
				  wimp_ERROR_BOX_OK_ICON |
				  wimp_ERROR_BOX_GIVEN_CATEGORY |
				  wimp_ERROR_BOX_CATEGORY_ERROR <<
				  wimp_ERROR_BOX_CATEGORY_SHIFT,
				  "Sargasso", "!sargasso",
				  (osspriteop_area *) 1, 0, 0);
  exit (EXIT_FAILURE);
}


void
warn (const char *warning)
{
  os_error error;

  LOG (("%s", warning));

  if (warning_window)
    {
      set_icon_string (warning_window, 0, warning);
      open_window (warning_window);
      xwimp_set_caret_position (warning_window, wimp_ICON_WINDOW,
				-100, -100, 1, 0);
      xos_bell ();
    }
  else
    {
      strncpy (error.errmess, warning, sizeof error.errmess);
      error.errmess[sizeof error.errmess - 1] = 0;
      xwimp_report_error_by_category (&error,
				      wimp_ERROR_BOX_OK_ICON |
				      wimp_ERROR_BOX_GIVEN_CATEGORY |
				      wimp_ERROR_BOX_CATEGORY_ERROR <<
				      wimp_ERROR_BOX_CATEGORY_SHIFT,
				      "Sargasso", "!sargasso",
				      (osspriteop_area *) 1, 0, 0);
    }
}


char *
get_icon_string (wimp_w w, wimp_i i)
{
  wimp_icon_state icon_state;

  icon_state.w = w;
  icon_state.i = i;

  os_error *error;

  error = xwimp_get_icon_state (&icon_state);
  if (error)
    {
      LOG (("xwimp_get_icon_state: 0x%x: %s", error->errnum, error->errmess));
      return NULL;
    }

  return icon_state.icon.data.indirected_text.text;
}


void
set_icon_string (wimp_w w, wimp_i i, const char *text)
{
  wimp_icon_state icon_state;

  unsigned int size;
  os_error *error;

  icon_state.w = w;
  icon_state.i = i;

  error = xwimp_get_icon_state (&icon_state);
  if (error)
    {
      LOG (("xwimp_get_icon_state: 0x%x: %s", error->errnum, error->errmess));
      return;
    }
  size = (unsigned int) icon_state.icon.data.indirected_text.size;

  if (!strncmp (icon_state.icon.data.indirected_text.text, text, size))
    return;

  strncpy (icon_state.icon.data.indirected_text.text, text, size);
  icon_state.icon.data.indirected_text.text[size - 1] = '\0';

  xwimp_set_icon_state (w, i, 0, 0);
}

void
set_icon_selected (wimp_w w, wimp_i i, int feed_description_main)
{
  xwimp_set_icon_state (w, i, (feed_description_main) ? wimp_ICON_SELECTED : 0, wimp_ICON_SELECTED);
}

int
get_icon_selected (wimp_w w, wimp_i i)
{
  wimp_icon_state icon_state;
  os_error *error;

  icon_state.w = w;
  icon_state.i = i;

  error = xwimp_get_icon_state (&icon_state);
  if (error)
    {
      LOG (("xwimp_get_icon_state: 0x%x: %s", error->errnum, error->errmess));
      return 0;
    }

  return((icon_state.icon.flags & wimp_ICON_SELECTED) != 0) ? 1 : 0;
}
