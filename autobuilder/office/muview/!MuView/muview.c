
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#include <oslib/displayfield.h>
#include <oslib/osfile.h>
#include <oslib/osfscontrol.h>
#include <oslib/osspriteop.h>
#include <oslib/osgbpb.h>
#include <oslib/quit.h>
#include <oslib/stringset.h>
#include <oslib/writablefield.h>
#include <oslib/window.h>
#include <oslib/colourtrans.h>
#include <oslib/iconbar.h>
#include <oslib/hourglass.h>
#include <oslib/saveas.h>
#include <oslib/hal.h>
#include <Systypes.h>
#include <Event.h>

#include "mupdf/fitz.h"
#include "mupdf/pdf.h"
#include <unixlib/local.h>

static int toolbox_quit;
static messagetrans_control_block messages_file;
static toolbox_o main_window;
static wimp_w w;
static int width, height, pageNum, size, scale, rotation;
static int converting;
static osspriteop_area *spritearea = NULL;
static char spritename[12];
static char imagefile[256];
static float scalefactor = 100.0;
static float rotatefactor = 0.0;

/* input options */
static const char *password = "";
//static int alphabits = 8;
//static float layout_w = 450;
//static float layout_h = 600;
//static float layout_em = 12;
//static char *layout_css = NULL;
//static int layout_use_doc_css = 1;

/* output options */
static const char *output = "/<MuView$TmpFile>";
//static const char *format = "pdf";
//static char *options = "resolution=120";

static pdf_document *pdfout = NULL;
static fz_context *ctx;
static fz_document *doc;
static int count = 0;

toolbox_action_list toolbox_events;     // holds toolbox event codes
wimp_message_list wimp_messages;

static void toolbox_poll(int active);

typedef struct toolbox_message_list {
	int first;
	int rest[];
} toolbox_message_list;

typedef union message_list {
	toolbox_message_list toolbox;
	wimp_message_list wimp;
	toolbox_action_list Toolbox;
} message_list;

typedef enum toolbox_result {
	TOOLBOX_RESULT_OK,
	TOOLBOX_RESULT_NOMEM,
	TOOLBOX_RESULT_OPEN,
	TOOLBOX_RESULT_WRITE
} toolbox_result;

void *minst;

#ifndef UNUSED
#define UNUSED(x) ((x)=(x))
#endif

#define PTR_WIMP_MESSAGE_LIST(l) ((wimp_message_list *) (message_list *) (l))
#define PTR_TOOLBOX_ACTION_LIST(l) ((toolbox_action_list *) (message_list *) (l))


/* Wimp Messages we're interested in */
/* static toolbox_message_list messages = {
	message_DATA_LOAD,
	{
		message_QUIT,
		0
	}
}; */

/* Toolbox events we're interested in */
/* static toolbox_message_list tbcodes = {
	Convert_Font,
	{
		action_ERROR,
		action_QUIT_QUIT,
		0
	}
};  */

static void toolbox_init(int argc, char **argv);
static void toolbox_exit(void);
static void register_toolbox_handlers(void);
static osbool toolbox_event_quit(bits event_code, toolbox_action *event,toolbox_block *id_block, void *handle);
static osbool redraw_window(wimp_event_no event_code,wimp_block *block, toolbox_block *id_block,void *handle);
static osbool key_handler(wimp_event_no event_code,wimp_block *block, toolbox_block *id_block,void *handle);
static osbool toolbox_event_error(bits event_code, toolbox_action *event,
		toolbox_block *id_block, void *handle);
static void register_message_handlers(void);
static int wimp_message_quit(wimp_message *message,void *handle);
static int wimp_message_dataload(wimp_message *message,void *handle);
static void bitmap_load(char *filename, int runPage, int resolution, int rotation);
static int bitmap_convert(char *filename, int file_type, char *output_file, int runPage, int resolution, int rotation);
static osbool close_window(wimp_event_no event_code, wimp_block *block, toolbox_block *id_block, void *handle);
static void destroy_window(void);
extern double get_timerValue(void);
osbool toolbox_saveas(bits event_code, toolbox_action *event, toolbox_block *id_block, void *handle);
osbool toolbox_savetofile(bits event_code, toolbox_action *event, toolbox_block *id_block, void *handle);

static int runpage(int number, float zoom, float rotate);

osbool toolbox_actionbutton(bits event_code, toolbox_action *event, toolbox_block *id_block, void *handle);
osbool update_menu(bits event_code, toolbox_action *block, toolbox_block *id_block, void *handle);

#define gotomenu_CANCEL 0x04
#define gotomenu_GOTO 0x05

static int runpage(int number, float zoom, float rotate)
{
//	fz_page *page;
	fz_matrix ctm;
	fz_pixmap *pix = NULL;

//	page = fz_load_page(ctx, doc, number - 1);
	/* Compute a transformation matrix for the zoom and rotation desired. */
        /* The default resolution without scaling is 72 dpi. */
        ctm = fz_scale(zoom / 100, zoom / 100);
        ctm = fz_pre_rotate(ctm, rotate);

        /* Render page to an RGB pixmap. */
	fz_try(ctx)
        	pix = fz_new_pixmap_from_page_number(ctx, doc, number - 1, ctm, fz_device_rgb(ctx), 0);
        fz_catch(ctx)
        {
                fprintf(stderr, "cannot render page: %s\n", fz_caught_message(ctx));
                fz_drop_document(ctx, doc);
                fz_drop_context(ctx);
                return EXIT_FAILURE;
        }


	fz_save_pixmap_as_spr(ctx, pix, output);
	/* Clean up */
	fz_drop_pixmap(ctx, pix);

	return EXIT_SUCCESS;

}

int main(int argc, char **argv)
{

	/* Create a context to hold the exception stack and various caches. */
	ctx = fz_new_context(NULL, NULL, FZ_STORE_UNLIMITED);
	if (!ctx)
	{
		fprintf(stderr, "cannot create mupdf context\n");
		return EXIT_FAILURE;
	}

	/* Register the default file types to handle. */
	fz_try(ctx)
		fz_register_document_handlers(ctx);
	fz_catch(ctx)
	{
		fprintf(stderr, "cannot register document handlers: %s\n", fz_caught_message(ctx));
		fz_drop_context(ctx);
		return EXIT_FAILURE;
	}

/* 	fz_set_aa_level(ctx, alphabits);

	if (layout_css)
	{
		fz_buffer *buf = fz_read_file(ctx, layout_css);
		fz_set_user_css(ctx, fz_string_from_buffer(ctx, buf));
		fz_drop_buffer(ctx, buf);
	}

	fz_set_use_document_css(ctx, layout_use_doc_css); */


	toolbox_init(argc, argv);

        if (argv[1]) {
          pageNum = 1;
          scale = 4;
	fprintf(stderr, "Load: %s\n", argv[1]);
	fflush(stderr);

	strcpy(imagefile, argv[1]);
	bitmap_load(__unixify(argv[1], 0, NULL, 0, 0), 1, scale, rotation);

	}

	while (!toolbox_quit)
		toolbox_poll(0);

	toolbox_exit();

	fz_drop_context(ctx);

	return 0;
}


void toolbox_init(int argc, char **argv)
{
  os_error *error;

	toolbox_block toolbox_block;

    wimp_messages.messages[0]=0;
    toolbox_events.action_nos[0]=0;

	UNUSED(argc);
	UNUSED(argv);

	event_initialise(&toolbox_block);

	event_set_mask(wimp_MASK_NULL |
			wimp_MASK_LEAVING |
			wimp_MASK_ENTERING |
			wimp_QUEUE_MOUSE |
			wimp_MASK_LOSE |
			wimp_MASK_GAIN);

	register_toolbox_handlers();
	register_message_handlers();

	error = xtoolbox_initialise(0, 310,
				&wimp_messages,
				&toolbox_events,
				"<MuView$Dir>", &messages_file,
				&toolbox_block, NULL, NULL, NULL);
	if (error) {
		fprintf(stderr, "toolbox_initialise: 0x%x: %s\n",
			error->errnum, error->errmess);
			exit(1);
	}


	error = xtoolbox_create_object(0, (toolbox_id) "Window", &main_window);
	if (error) {
		fprintf(stderr, "toolbox_create_object: 0x%x: %s\n",
			error->errnum, error->errmess);
		 exit(1);


	  }

}

static void bitmap_load(char *filename, int runPage, int resolution, int rotation)
{
  os_error *error;
  BBox extent;
  osbool success;
  double startTime;
  double endTime;
  const int window_title_len = 256;
  char* window_title = malloc(window_title_len);
  int length = 0;


	   xhourglass_on();
	   startTime = get_timerValue();

	if (bitmap_convert(filename, 0xff9, NULL, runPage, resolution, rotation) == 0) {

	error = xosfile_read_stamped_no_path("<MuView$TmpFile>", 0, 0, 0, &size, 0, 0);
 	if (error) {
		fprintf(stderr, "osfile_read_path: 0x%x: %s\n",
			error->errnum, error->errmess);
	   xhourglass_off();
			exit(1);
	}

	fprintf(stderr, "FileSize: %d\n", size);
	}
	else {

	   xhourglass_off();
		exit(1);
	}

	length += snprintf(window_title + length, window_title_len - length, "%s [%d/%d] [%d] [%d]", filename, runPage, count, resolution, rotation);

	 if (main_window)
	 	destroy_window();


	error = xtoolbox_create_object(0, (toolbox_id) "Window", &main_window);
	if (error) {
		fprintf(stderr, "toolbox_create_object: 0x%x: %s\n",
			error->errnum, error->errmess);
	   xhourglass_off();
		 exit(1);


	  }
	  	error = xwindow_set_title(0, main_window, window_title);
	if (error != NULL) {
		fprintf(stderr, "displayfield_set_value: 0x%x: %s\n",
			error->errnum, error->errmess);
	}

	success = event_register_wimp_handler(event_ANY,
			wimp_REDRAW_WINDOW_REQUEST, redraw_window, NULL);
	if (success == FALSE)
	fprintf(stderr, "registering RedrawWindow failed\n");

	success = event_register_wimp_handler(event_ANY,
			wimp_CLOSE_WINDOW_REQUEST, close_window, NULL);
	if (success == FALSE)
	fprintf(stderr, "registering CloseWindow failed\n");

	success = event_register_wimp_handler(event_ANY,
		wimp_KEY_PRESSED, key_handler, NULL);
	if (success == FALSE)
	fprintf(stderr, "registering KeyHandler failed\n");


 spritearea = (osspriteop_area *)calloc(1, size + 64);

  error = xosspriteop_clear_sprites(osspriteop_USER_AREA, spritearea);
  if (error) {
    fprintf(stderr, "osspriteop_clear_sprites: 0x%x: %s\n", error->errnum, error->errmess);
    if (spritearea) {
    free(spritearea);
    spritearea = NULL;
    }
    xhourglass_off();
    exit(1);
  }

        error = xosspriteop_load_sprite_file(osspriteop_USER_AREA, (osspriteop_area *)spritearea, "<MuView$TmpFile>");
	if (error) {
		fprintf(stderr, "osspriteop_load_sprite_file: 0x%x: %s\n",
		error->errnum, error->errmess);
		if (spritearea) {
		free(spritearea);
		spritearea = NULL;
		}
		xhourglass_off();
		exit(1);
}
  error = xosspriteop_return_name(osspriteop_USER_AREA, (osspriteop_area *)spritearea, spritename, 16, 1, 0);
	if (error) {
		fprintf(stderr, "osspriteop_return_name: 0x%x: %s\n",
			error->errnum, error->errmess);
		xhourglass_off();
		exit(1);
		}

fprintf(stderr, "Sprite Name: %s\n", spritename);

  error = xosspriteop_read_sprite_info(osspriteop_NAME, (osspriteop_area *)spritearea,
					(osspriteop_id)spritename, &width, &height, 0, 0);
	if (error) {
		fprintf(stderr, "osspriteop_load_sprite_file: 0x%x: %s\n",
			error->errnum, error->errmess);
    		xhourglass_off();
		exit(1);
}


xwindow_get_extent(0, main_window, &extent);
	fprintf(stderr, "before: x0: %d y0: %d x1: %d y1: %d\n", extent.x0, extent.y0, extent.x1, extent.y1);
extent.y0 = 0 - (height * 2);
extent.x1 = width * 2;
xwindow_set_extent(0, main_window, &extent);
	fprintf(stderr, "after: x0: %d y0: %d x1: %d y1: %d\n", extent.x0, extent.y0, extent.x1, extent.y1);
 xtoolbox_show_object(0, main_window, toolbox_POSITION_DEFAULT, 0, NULL, (toolbox_c)NULL);


	 endTime = get_timerValue();
	       fprintf(stderr, "Process time total %fs for %s\n", endTime - startTime, filename);

xhourglass_off();

}
static void toolbox_poll(int active)
{
	int event;
	unsigned int mask;
	wimp_block block;

	if (active || converting) {
		event_set_mask(0x3972);
		event_poll(&event, &block, 0);
	} else {
		event_get_mask(&mask);
		event_set_mask(mask | wimp_MASK_NULL);
		event_poll(&event, &block, 0);
	}
}

void toolbox_exit(void)
{
  fprintf(stderr, "count (Toolbox exit): %d\n", count);
  fflush(stderr);
	if (spritearea) {
	  free(spritearea);
	  spritearea = NULL;
	  }
	if (count) {
      fprintf(stderr, "count (Toolbox Exit): %d\n", count);
      fflush(stderr);

	  count = 0;
  	fz_drop_document(ctx, doc);

	}

}

/**
 * Register event handlers with the toolbox
 */
void register_toolbox_handlers(void)
{
	 osbool success;


 	success = event_register_toolbox_handler(event_ANY,
			action_QUIT_QUIT, toolbox_event_quit, NULL);
	if (success == FALSE)
		fprintf(stderr, "registering quit_QUIT failed\n");

	success = event_register_toolbox_handler(event_ANY,
			action_ERROR, toolbox_event_error, NULL);
	if (success == FALSE)
	fprintf(stderr, "registering action_ERROR failed\n");

	success = event_register_toolbox_handler(event_ANY,
			action_SAVE_AS_ABOUT_TO_BE_SHOWN, toolbox_saveas, NULL);
	if (success == FALSE)
		fprintf(stderr, "registering action_SAVE_AS_ABOUT_TO_BE_SHOWN failed\n");

        success = event_register_toolbox_handler(event_ANY,
                        action_SAVE_AS_SAVE_TO_FILE, toolbox_savetofile, NULL);
        if (success == FALSE)
                fprintf(stderr, "registering action_SAVE_AS_SAVE_TO_FILE failed\n");

	success = event_register_toolbox_handler(event_ANY,
			gotomenu_GOTO, toolbox_actionbutton, NULL);
	if (success == FALSE)
	fprintf(stderr, "registering resizemenu_RESIZE failed\n");

	success = event_register_toolbox_handler(event_ANY,
			action_WINDOW_ABOUT_TO_BE_SHOWN, update_menu, NULL);
	if (success == FALSE)
		fprintf(stderr, "registering action_WINDOW_ABOUT_TO_BE_SHOWN failed\n");


}
static osbool close_window(wimp_event_no event_code, wimp_block *block, toolbox_block *id_block, void *handle)
{
        destroy_window();

    if (count) {
      fprintf(stderr, "Count (Close Window): %d\n", count);
      fflush(stderr);
      count = 0;
  	fz_drop_document(ctx, doc);

    }

  return TRUE;

}

static void destroy_window(void)
{
  event_deregister_wimp_handler(event_ANY,
			wimp_REDRAW_WINDOW_REQUEST, redraw_window, NULL);
  event_deregister_wimp_handler(event_ANY,
			wimp_CLOSE_WINDOW_REQUEST, close_window, NULL);
  toolbox_delete_object(0, main_window);

  main_window = NULL;

  if (spritearea) {

    free(spritearea);
    spritearea = NULL;
    }


}

static osbool key_handler(wimp_event_no event_code,wimp_block *block, toolbox_block *id_block,void *handle)
{

 	UNUSED(event_code);
//	UNUSED(block);
	UNUSED(id_block);
	UNUSED(handle);
	fprintf(stderr, "Key: %d\n", block->key.c);

	switch (block->key.c)
	{
	  case wimp_KEY_PAGE_DOWN:
	  fprintf(stderr, "Page Down:\n");
	  pageNum++;
	  if (pageNum > count)
            pageNum = 1;
          if (count != 1)
	    bitmap_load(__unixify(imagefile, 0, NULL, 0, 0), pageNum, scale, rotation);
	  break;
	  case wimp_KEY_PAGE_UP:
	  fprintf(stderr, "Page Up:\n");
	  pageNum--;
	  if (pageNum < 1 )
	    pageNum = count;
	  if (count != 1)
	    bitmap_load(__unixify(imagefile, 0, NULL, 0, 0), pageNum, scale, rotation);
	  break;
	  case wimp_KEY_UP:
	  scale++;
	  if (scale > 11)
	    scale = 1;
	  fprintf(stderr, "Key Up:%d\n", scale);
	  bitmap_load(__unixify(imagefile, 0, NULL, 0, 0), pageNum, scale, rotation);
	  break;
	  case wimp_KEY_DOWN:
	  scale--;
	  if (scale < 1)
	    scale = 11;
	  fprintf(stderr, "Key Down: %d\n", scale);
	  bitmap_load(__unixify(imagefile, 0, NULL, 0, 0), pageNum, scale, rotation);
	  break;
	  case wimp_KEY_RIGHT:
	  rotation++;
	  if (rotation > 4)
	    rotation = 1;
	  fprintf(stderr, "Key right:%d\n", rotation);
	  bitmap_load(__unixify(imagefile, 0, NULL, 0, 0), pageNum, scale, rotation);
	  break;
	  case wimp_KEY_LEFT:
	  rotation--;
	  if (rotation < 1)
	    rotation = 4;
	  fprintf(stderr, "Key left: %d\n", rotation);
	  bitmap_load(__unixify(imagefile, 0, NULL, 0, 0), pageNum, scale, rotation);
	  break;
	  default:
	  break;
	}

  return TRUE;

}

osbool redraw_window(wimp_event_no event_code, wimp_block *block, toolbox_block *id_block, void *handle)
{
 	UNUSED(event_code);
//	UNUSED(event);
//	UNUSED(block);
	UNUSED(id_block);
	UNUSED(handle);
  osbool more;
  wimp_draw redraw;
  int x0, y0;
  int xeig, yeig;

  BBox extent;
   xwindow_get_wimp_handle(0, main_window, &w);
   redraw.w = w;
   xwimp_redraw_window(&redraw, &more);
	os_factors factors;
	os_read_mode_variable(os_CURRENT_MODE, os_MODEVAR_XEIG_FACTOR, &xeig);
	os_read_mode_variable(os_CURRENT_MODE, os_MODEVAR_YEIG_FACTOR, &yeig);

  x0 = redraw.box.xmin - redraw.xscroll;
  y0 = redraw.box.ymax - redraw.yscroll;
  xwindow_get_extent(0, id_block->this_obj, &extent);
    y0 += extent.ymin;

fprintf(stderr, "x: %d y: %d\n", xeig, yeig);
fflush(stderr);
        factors.xmul=2 - xeig;
        factors.ymul=2 - yeig;
        factors.xdiv=1;
        factors.ydiv=1;
        while (more)
        {
	         xosspriteop_put_sprite_scaled(osspriteop_NAME, spritearea, (osspriteop_id)spritename, x0, y0, 0, &factors, NULL);
	         xwimp_get_rectangle(&redraw, &more);
	}


 return TRUE;
}

static int bitmap_convert(char *filename, int file_type, char *output_file, int runPage, int resolution, int rotation)
{

  os_error *error;
  char tmpoptions[32];

	 switch (resolution) {
	   case 1:
	   scalefactor = 50.0;
	   break;
	   case 2:
	   scalefactor = 70.0;
	   break;
	   case 3:
	   scalefactor = 100.0;
	   break;
	   case 4:
	   scalefactor = 140.0;
	   break;
	   case 5:
	   scalefactor = 200.0;
	   break;
	   case 6:
	   scalefactor = 280.0;
	   break;
	   case 7:
	   scalefactor = 400.0;
	   break;
	   case 8:
	   scalefactor = 560.0;
	   break;
	   case 9:
	   scalefactor = 800.0;
	   break;
	   case 10:
	   scalefactor = 1120.0;
	   break;
	   case 11:
	   scalefactor = 1600.0;
	   break;
	   default:
	   scale = 4;
}

	 switch (rotation) {
	   case 1:
	   rotatefactor = 0.0;
	   break;
	   case 2:
	   rotatefactor = 90.0;
	   break;
	   case 3:
	   rotatefactor = 180.0;
	   break;
	   case 4:
	   rotatefactor = 270.0;
	   break;
	   default:
	   rotation = 1;
	 }

fprintf(stderr, "Res: %.2f\n", scalefactor);
fflush(stderr);

sprintf(tmpoptions, "resolution=%.2f", scalefactor);

fprintf(stderr, "Options: %s %d\n", tmpoptions, count);
fflush(stderr);

        if (!count) {
	/* Open the output document. */
//	fz_try(ctx)
//		out = fz_new_document_writer(ctx, output, "spr", tmpoptions);
//	fz_catch(ctx)
//	{
//		fprintf(stderr, "cannot create document: %s\n", fz_caught_message(ctx));
//		fz_drop_context(ctx);
//		return EXIT_FAILURE;
//	}

	/* Open the document. */
	fz_try(ctx)
	doc = fz_open_document(ctx, filename);
        fz_catch(ctx)
        {
                fprintf(stderr, "cannot open document: %s\n", fz_caught_message(ctx));
                fz_drop_context(ctx);
                return EXIT_FAILURE;
        }

	if (fz_needs_password(ctx, doc))
	{
	error = xtoolbox_create_object(0, (toolbox_id) "Window", &main_window);
	if (error) {
		fprintf(stderr, "toolbox_create_object: 0x%x: %s\n",
			error->errnum, error->errmess);
	   xhourglass_off();
		 exit(1);


	  }
		if (!fz_authenticate_password(ctx, doc, password))
			fz_throw(ctx, FZ_ERROR_GENERIC, "cannot authenticate password: %s", filename);
	}

//	fz_layout_document(ctx, doc, layout_w, layout_h, layout_em);
	/* Count the number of pages. */
	fz_try(ctx)
	count = fz_count_pages(ctx, doc);
	fz_catch(ctx)
        {
		fprintf(stderr, "cannot count number of pages: %s\n", fz_caught_message(ctx));
		fz_drop_document(ctx, doc);
		fz_drop_context(ctx);
		return EXIT_FAILURE;
        }

        }

	fprintf(stderr, "Page: %d Total: %d Scale factor: %0f\n", runPage, count, scalefactor);
	fflush(stderr);

	return(runpage(runPage, scalefactor, rotatefactor));

}

osbool toolbox_savetofile(bits event_code, toolbox_action *event, toolbox_block *id_block, void *handle)
{
  bits file_type;
  os_error *error;
  char buffer[256];
  fz_matrix ctm;
  fz_rect mediabox;
  fz_device *dev = NULL;
  fz_page *page;

  fz_output *out = NULL;
  fz_cookie cookie = { 0 };

  fz_rect tbounds;

  fz_var(dev);
  fz_var(out);


          error = xsaveas_get_file_type(0, id_block->this_obj, &file_type);
          if (error) {
                fprintf(stderr, "xsaveas_get_file_type: 0x%x: %s\n",
                        error->errnum, error->errmess);
          }

          fprintf(stderr, "File Type: %d\n", file_type);

          if (file_type == 0xff9)
            return TRUE;

          error = xsaveas_get_file_name(0, id_block->this_obj, buffer, sizeof(buffer), 0);
          if (error) {
                fprintf(stderr, "xsaveas_get_file_type: 0x%x: %s\n",
                        error->errnum, error->errmess);
          }

          fprintf(stderr, "File Name: %s\n", buffer);

          xhourglass_on();

          page = fz_load_page(ctx, doc, pageNum - 1);
          mediabox = fz_bound_page(ctx, page);

          if (file_type == 0xaad) {

             ctm = fz_pre_rotate(fz_scale(scalefactor / 100, scalefactor / 100), rotatefactor);
             tbounds = fz_transform_rect(mediabox, ctm);

             fz_try(ctx)
             {

		out = fz_new_output_with_path(ctx, buffer, 0);

		dev = fz_new_svg_device(ctx, out, tbounds.x1-tbounds.x0, tbounds.y1-tbounds.y0, FZ_SVG_TEXT_AS_PATH, 1);

		fz_run_page(ctx, page, dev, ctm, &cookie);
		fz_close_device(ctx, dev);
		fz_close_output(ctx, out);
              }
	      fz_always(ctx)
	      {
		fz_drop_device(ctx, dev);
		fz_drop_output(ctx, out);
	      }
	      fz_catch(ctx)
	      {
		fz_drop_page(ctx, page);
		fz_rethrow(ctx);
	      }
	  }
	else if (file_type == 0xadf) {
                fz_buffer *contents = NULL;
                pdf_obj *resources = NULL;

                fz_var(contents);
                fz_var(resources);

                pdfout = pdf_create_document(ctx);

                fz_try(ctx)
                {
                        pdf_obj *page_obj;

                        dev = pdf_page_write(ctx, pdfout, mediabox, &resources, &contents);
                        fz_run_page(ctx, page, dev, fz_identity, &cookie);
                        fz_close_device(ctx, dev);
                        fz_drop_device(ctx, dev);
                        dev = NULL;

                        page_obj = pdf_add_page(ctx, pdfout, mediabox, rotation, resources, contents);
                        pdf_insert_page(ctx, pdfout, -1, page_obj);
                        pdf_drop_obj(ctx, page_obj);

                        pdf_save_document(ctx, pdfout, buffer, NULL);
			pdf_drop_document(ctx, pdfout);

                }
                fz_always(ctx)
                {
                        pdf_drop_obj(ctx, resources);
                        fz_drop_buffer(ctx, contents);
                        fz_drop_device(ctx, dev);
                }
                fz_catch(ctx)
                {
                        fz_drop_page(ctx, page);
                        fz_rethrow(ctx);
                }

	}
	else if (file_type == 0xfff || file_type == 0xfaf) {
		fz_stext_page *text = NULL;

		ctm = fz_pre_scale(fz_rotate(rotatefactor), scalefactor / 100, scalefactor / 100);

		fz_var(text);

		fz_try(ctx)
		{
	                out = fz_new_output_with_path(ctx, buffer, 0);

			fz_stext_options stext_options;

			stext_options.flags = 0;
			text = fz_new_stext_page(ctx, mediabox);
			dev = fz_new_stext_device(ctx,  text, &stext_options);
			fz_run_page(ctx, page, dev, ctm, &cookie);
			fz_close_device(ctx, dev);
			fz_drop_device(ctx, dev);
			dev = NULL;

                        if (file_type == 0xfff) {
			   fz_print_stext_page_as_text(ctx, out, text);
			   fz_write_printf(ctx, out, "\f\n");

			}
			else if (file_type == 0xfaf) {
			     fz_print_stext_header_as_html(ctx, out);
			     fz_print_stext_page_as_html(ctx, out, text, page);
			     fz_print_stext_trailer_as_html(ctx, out);
			}

			fz_close_output(ctx, out);

		}
		fz_always(ctx)
		{
			fz_drop_device(ctx, dev);
			fz_drop_output(ctx, out);
			fz_drop_stext_page(ctx, text);
		}
		fz_catch(ctx)
		{
			fz_drop_page(ctx, page);
			fz_rethrow(ctx);
		}
	}
/*	else if (file_type == 0xff5) {
	}
	fz_write_ps_file_header(ctx, out);
	fz_write_ps_file_trailer(ctx, out, output_page);
	else if (file_type == 0xffd) {
	} */

          xosfile_set_type(buffer, file_type);
          xsaveas_file_save_completed((1), id_block->this_obj, buffer);

          xhourglass_off();


  return TRUE;

}


osbool toolbox_actionbutton(bits event_code, toolbox_action *event, toolbox_block *id_block, void *handle)
{
	UNUSED(event_code);
	UNUSED(event);
	UNUSED(id_block);
	UNUSED(handle);

 char pageNumber[5];
 toolbox_c selected;
 os_error *error;
 int button;

 fprintf(stderr, "Action:\n");

  xwritablefield_get_value(0, id_block->this_obj, 0x02, pageNumber, 5, 0);
  fprintf(stderr, "Page Nummber: %d\n", atoi(pageNumber));

  if (atoi(pageNumber) <= count && atoi(pageNumber) != 0)
    pageNum = atoi(pageNumber);

  bitmap_load(__unixify(imagefile, 0, NULL, 0, 0), pageNum, scale, rotation);

  return TRUE;

}

osbool update_menu(bits event_code, toolbox_action *event, toolbox_block *id_block, void *handle)
{
 	UNUSED(event_code);
	UNUSED(event);
	UNUSED(handle);
	char pageNumber[5];

  os_error *error;
  char menu_title[32];

        error = xwindow_get_title(0, id_block->this_obj, menu_title, 32, NULL);
    	if (error) {
		fprintf(stderr, "xwindow_get_title: 0x%x: %s\n",
			error->errnum, error->errmess);
	}
          fprintf(stderr, "Menu Title: %s\n", menu_title);
          fprintf(stderr, "This object: %d \n", id_block->this_cmp);

	  if (strcmp(menu_title, "Goto Page") == 0)
	  {
	    sprintf(pageNumber, "%d\n", pageNum);
	    xwritablefield_set_value(0, id_block->this_obj, 0x02, pageNumber);
	    fprintf(stderr, "Goto Page:\n");

          }
  return TRUE;

}


/**
 * Handle quit events
 */
osbool toolbox_event_quit(bits event_code, toolbox_action *event,
		toolbox_block *id_block, void *handle)
{
	UNUSED(event_code);
	UNUSED(event);
	UNUSED(id_block);
	UNUSED(handle);

	toolbox_quit = 1;

	return TRUE;
}

/**
 * Handle toolbox errors
 */
osbool toolbox_event_error(bits event_code, toolbox_action *event,
		toolbox_block *id_block, void *handle)
{
	toolbox_action_error_block *error =
			(toolbox_action_error_block *) event;

	UNUSED(event_code);
	UNUSED(id_block);
	UNUSED(handle);

	fprintf(stderr, "toolbox error: 0x%x: %s\n",
		error->errnum, error->errmess);

	return TRUE;
}

/**
 * Register message handlers
 */
void register_message_handlers(void)
{
	osbool success;

	success = event_register_message_handler(message_QUIT,
			wimp_message_quit, NULL);
	if (success == FALSE)
		fprintf(stderr, "registering message_QUIT handler failed\n");

	success = event_register_message_handler(message_DATA_LOAD,
			wimp_message_dataload, NULL);
	if (success == FALSE)
		fprintf(stderr,
			"registering message_DATA_LOAD handler failed\n");

	success = event_register_message_handler(message_DATA_OPEN,
			wimp_message_dataload, NULL);
	if (success == FALSE)
		fprintf(stderr,
			"registering message_DATA_OPEN handler failed\n");

}

/**
 * Handle quit messages
 */
osbool wimp_message_quit(wimp_message *message, void *handle)
{
	UNUSED(message);
	UNUSED(handle);

	toolbox_quit = 1;

	return TRUE;
}

/**
 * Handle dataload messages
 */
osbool wimp_message_dataload(wimp_message *message, void *handle)
{
	os_error *error;
	wimp_message_data_xfer *dl = &message->data.data_xfer;

	UNUSED(handle);
        fprintf(stderr, "Filename: %s\n", dl->file_name);

	       pageNum = 1;
	       scale = 4;
	       rotation = 1;

  if (dl->file_type > 0xfff)
  	return FALSE;

    if (dl->w != wimp_ICON_BAR)
    {
    switch(dl->file_type)
    {
    case 0xadf : // pdf
     break;
    case 0xa76 : // epub
     break;
    default:
      return FALSE;
    }
    }

    switch (dl->file_type)
    {
    case 0xadf : // pdf
     break;
    case 0x695 : // gif
     break;
     case 0xb60 : // png
     break;
    case 0xc85 : // jpg
     break;
    case 0x69c : // bmp
     break;
    case 0xff0 : // tiff
     break;
    case 0xaad : // svg
     break;
    case 0xab1 : // cbz
     break;
    case 0xa76 : // epub
     break;
    case 0xaaf : // fb2
     break;
    case 0xab2 : // xps
     break;
    default:
      return FALSE;
    }

	message->action = message_DATA_LOAD_ACK;
	message->your_ref = message->my_ref;

	error = xwimp_send_message(wimp_USER_MESSAGE, message, message->sender);
	if (error != NULL) {
		fprintf(stderr, "wimp_send_message: 0x%x: %s\n",
			error->errnum, error->errmess);
	  }

    if (count) {
       fprintf(stderr, "count1b: %d\n", count);
       fflush(stderr);
       count = 0;

  	fz_drop_document(ctx, doc);

    }

	strcpy(imagefile, dl->file_name);
	pageNum = 1;
	scale = 4;
	rotation = 1;
	bitmap_load(__unixify(dl->file_name, 0, NULL, 0, 0), 1, scale, rotation);

	return TRUE;
}
/**
 * Handle saveas events
 */
osbool toolbox_saveas(bits event_code, toolbox_action *event,
		toolbox_block *id_block, void *handle)
  {

    os_error *error;
    char menu_title[32];


	UNUSED(event_code);
	UNUSED(event);
	UNUSED(handle);

        error = xmenu_get_title(0, id_block->parent_obj, menu_title, 32, NULL);
    	if (error) {
		fprintf(stderr, "xmenu_get_title: 0x%x: %s\n",
			error->errnum, error->errmess);
	}
          fprintf(stderr, "Menu Title: %s\n", menu_title);
          fprintf(stderr, "This object: %d \n", id_block->parent_cmp);

      	  if (strcmp(menu_title, "Save") == 0)
	  {

	    switch(id_block->parent_cmp)
	    {
	      case 0:
	      xsaveas_set_file_type(0, id_block->this_obj, 0xfff); // text
	      break;

	      case 1:
	      xsaveas_set_file_type(0, id_block->this_obj, 0xfaf); // html
	      break;

	      case 2:
	      xsaveas_set_file_type(0, id_block->this_obj, 0xff5); // poscript
	      break;

	      case 3:
	      xsaveas_set_file_type(0, id_block->this_obj, 0xffd); // data (pcl)
	      break;

	      case 4:
	      xsaveas_set_file_type(0, id_block->this_obj, 0xaad); // svg
	      break;

	      case 5:
	      xsaveas_set_file_type(0, id_block->this_obj, 0xadf); // pdf
	      break;

            }

	}

        else
        {

           error = xsaveas_set_file_size(0, id_block->this_obj, size - sizeof(int));
    	   if (error) {
		fprintf(stderr, "xsaveas_set_file_size: 0x%x: %s\n",
			error->errnum, error->errmess);
	   }

           error = xsaveas_set_data_address(0, id_block->this_obj, (byte *) spritearea + sizeof(int), size,
          				(byte *) spritearea + sizeof(int), size);
       	   if (error) {
		fprintf(stderr, "xsaveas_set_data_address: 0x%x: %s\n",
			error->errnum, error->errmess);
	   }
        }

	return TRUE;
  }
