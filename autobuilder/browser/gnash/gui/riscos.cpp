//
//   Copyright (C) 2005, 2006 Free Software Foundation, Inc.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

//
//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "gnash.h"
//#include "movie_definition.h"
#include "gui.h"
#include "rc.h"
#include "riscossup.h"
#include "render_handler.h"
#include "log.h"

#include "oslib/wimpspriteop.h"

/**
 * A note on coordinate systems:
 *
 * The RISC OS Wimp uses a coordinate system whose origin lies at the bottom
 * left of the screen, with positive X running from left to right and
 * positive Y running from bottom to top.
 *
 * Gnash (and the AGG glue code) use a coordinate system with an origin at
 * the top left of the window's visible area, with positive X running from
 * left to right and positive Y running from top to bottom.
 *
 * Therefore, to convert between the two, the Y coordinate must be flipped:
 *
 * RO -> Gnash: y' = window_top - y
 * Gnash -> RO: y' = (window_top - window_bottom) - y
 */

namespace gnash
{

RiscosGui::~RiscosGui()
{
    if (_task)
      wimp_close_down(_task);
    _task = (wimp_t) 0;
}

RiscosGui::RiscosGui(unsigned long xid, float scale, bool loop, unsigned int depth)
 : Gui(xid, scale, loop, depth), _task((wimp_t)0), _window((wimp_w)0),
   _quit(false), _timeout(0), _mouse_track(false), _mouse_state(0),
   _mouse_x(0), _mouse_y(0),
   m_draw_minx(0), m_draw_miny(0), m_draw_maxx(0), m_draw_maxy(0),
   _screen_height(480), _screen_width(640)
{
}


bool
RiscosGui::init(int argc, char **argv[])
{
    GNASH_REPORT_FUNCTION;

/*    wimp_MESSAGE_LIST(4) messages = { { message_MODE_CHANGE,
                                        message_DATA_LOAD,
                                        message_DATA_OPEN,
                                        message_QUIT } };*/
    os_error *error;

    glue.init(argc, argv);

    error = xwimp_initialise(wimp_VERSION_RO38, "Gnash",
                             (wimp_message_list *)0/*&messages*/,
                             0, &_task);
    if (error) {
      log_msg("%s\n", error->errmess);
      return false;
    }

    if (!create_window())
      return false;

#ifdef RENDERER_AGG
    os_VDU_VAR_LIST(2) vduvars = { { os_VDUVAR_SCREEN_START,
                                     os_VDUVAR_END_LIST} };
    int vduvals[2];
    error = xos_read_vdu_variables((const os_vdu_var_list *)&vduvars,
                                   vduvals);
    if (error) {
      log_msg("%s\n", error->errmess);
      return false;
    }

    os_mode mode;
    os_mode_selector *mode_block;

    /* read current screenmode details */
    error = xosscreenmode_current(&mode);
    if (error) {
      log_msg("%s", error->errmess);
      return false;
    }

    if ((unsigned int)mode >= 256) {
      mode_block = (os_mode_selector *)mode;
      _screen_width = mode_block->xres;
      _screen_height = mode_block->yres;
    }

    /** \todo Mode specifiers */

    log_msg("Framebuffer address: %p\n", (void *)vduvals[0]);
    log_msg("Screen Res: %d x %d\n", _screen_width, _screen_height);

    glue.prepFramebuffer((void *)vduvals[0], _screen_width, _screen_height);
#endif

    _renderer = glue.createRenderHandler();
    set_render_handler(_renderer);
    // hack?
    _renderer->set_scale(1.0f, 1.0f);

    return true;
}


bool
RiscosGui::createWindow(const char *title, int width, int height)
{
//First call the old createWindow function and then set the title.
//In case there's some need to not setting the title.
    title = title; // TODO: set title string

    bool ret = createWindow(width, height);
    wimp_window_state state;
    os_error *error;

    state.w = _window;
    error = xwimp_get_window_state(&state);
    if (error) {
      log_msg("%s\n", error->errmess);
      return false;
    }

    state.visible.x1 = state.visible.x0 + (width * 2);
    state.visible.y1 = state.visible.y0 + (height * 2);

    _window_vis = state.visible;

    error = xwimp_open_window((wimp_open *)&state);
    if (error) {
      log_msg("%s\n", error->errmess);
      return false;
    }

    return ret;
}

bool
RiscosGui::createWindow(int width, int height)
{
    GNASH_REPORT_FUNCTION;
    _width = width;
    _height = height;

    glue.setRenderHandlerSize(width, height);

    return true;
}

void
RiscosGui::renderBuffer()
{
    // bounding box is window-relative
    wimp_draw draw;

    /* Fake up a redraw block, flipping Y coordinates for RO wimp */
    draw.w = _window;
    draw.box.x0 = m_draw_minx * 2;
    draw.box.y0 = (_window_vis.y1 - _window_vis.y0) - (m_draw_maxy * 2);
    draw.box.x1 = m_draw_maxx * 2;
    draw.box.y1 = (_window_vis.y1 - _window_vis.y0) - (m_draw_miny * 2);

    /* and render it */
    renderBufferInternal(false, draw);
}

void
RiscosGui::setTimeout(unsigned int timeout)
{
    _timeout = os_read_monotonic_time() + timeout / 10;
}

void
RiscosGui::setInterval(unsigned int interval)
{
    _interval = interval;
}

void
RiscosGui::set_invalidated_region(const rect& bounds)
{
    // Note: Bounds coordinates are in TWIPS

#ifdef RENDERER_AGG
    // forward to renderer
    _renderer->set_invalidated_region(bounds);

    if (bounds.width() > 1e10f) {
      // Region is entire screen. Don't convert to integer as this will
      // overflow.

      m_draw_minx = 0;
      m_draw_miny = 0;
      m_draw_maxx = _width - 1;
      m_draw_maxy = _height - 1;
    } else {
      // remember for renderBuffer()
      _renderer->world_to_pixel(&m_draw_minx, &m_draw_miny,
                                bounds.get_x_min(), bounds.get_y_min());
      _renderer->world_to_pixel(&m_draw_maxx, &m_draw_maxy,
                                bounds.get_x_max(), bounds.get_y_max());

      // add two pixels because of anti-aliasing...
      m_draw_minx = valid_coord(m_draw_minx - 2, _width);
      m_draw_miny = valid_coord(m_draw_miny - 2, _height);
      m_draw_maxx = valid_coord(m_draw_maxx + 2, _width);
      m_draw_maxy = valid_coord(m_draw_maxy + 2, _height);
    }

//    log_msg("DrawRect: (%i, %i), (%i, %i)\n",
//            m_draw_minx, m_draw_miny, m_draw_maxx, m_draw_maxy);
#endif
}

bool
RiscosGui::run()
{
    GNASH_REPORT_FUNCTION;

    os_t t, now;
    wimp_block block;
    wimp_event_no event;
    osbool more;
    os_error *error;

    t = os_read_monotonic_time();

    while (!_quit) {
      error = xwimp_poll_idle(wimp_SAVE_FP, &block, t, NULL, &event);
      if (error) {
        log_msg("%s\n", error->errmess);
        return false;
      }

      switch (event) {
      case wimp_NULL_REASON_CODE:
        now = os_read_monotonic_time();
        if (_timeout && _timeout <= now) {
          _quit = true;
        } else {
          // Query mouse state
          if (!poll_mouse_state())
            return false;

          // TODO: pay attention to interval
//          if ((os_t)_interval <= (now - t) * 10) {
            advance_movie(this);
//          }
          now = os_read_monotonic_time();
          t = now + 10;
        }
        break;
      case wimp_REDRAW_WINDOW_REQUEST:
        {
          /* Invalidate the entire window area */
          rect bounds(-1e10f, -1e10f, 1e10f, 1e10f);
          set_invalidated_region(bounds);

          /* and draw it */
          renderBufferInternal(true, block.redraw);
        }
        break;
      case wimp_OPEN_WINDOW_REQUEST:
        _window_vis = block.open.visible;

        error = xwimp_open_window(&block.open);
        if (error)
          log_msg("%s\n", error->errmess);
        break;
      case wimp_CLOSE_WINDOW_REQUEST:
        _quit = true;
        break;
      case wimp_POINTER_LEAVING_WINDOW:
        _mouse_track = false;
        break;
      case wimp_POINTER_ENTERING_WINDOW:
        _mouse_track = true;
        break;
      case wimp_MOUSE_CLICK:
        if ((block.pointer.buttons & 0xf) != _mouse_state) {
          int mask = block.pointer.buttons & 0xf; // don't care about drags
          /* Flip select and adjust -
           * Gnash internals expect the opposite to what RISC OS provides */
          if ((mask & 0x1) && !(mask & 0x4))
            mask = 0x4 | (mask & 0x2);
          else if (!(mask & 0x1) && (mask & 0x4))
            mask = 0x1 | (mask & 0x2);
          notify_mouse_clicked(true, mask);

          _mouse_state = block.pointer.buttons & 0xf;
        }
        break;
      case wimp_KEY_PRESSED:
        break;
      case wimp_USER_DRAG_BOX:
        break;
      case wimp_MENU_SELECTION:
        break;
      case wimp_SCROLL_REQUEST:
        break;
      case wimp_LOSE_CARET:
        break;
      case wimp_GAIN_CARET:
        break;
      case wimp_POLLWORD_NON_ZERO:
        break;
      case wimp_USER_MESSAGE:
      case wimp_USER_MESSAGE_RECORDED:
      case wimp_USER_MESSAGE_ACKNOWLEDGE:
        switch (block.message.action) {
        case message_QUIT:
          _quit = true;
          break;
        default:
//          user_message(event, &(block.message));
          break;
        }
        break;
      }
    }

    return true;
}

bool
RiscosGui::createMenu()
{
    GNASH_REPORT_FUNCTION;

    return true;
}

bool
RiscosGui::setupEvents()
{
  GNASH_REPORT_FUNCTION;

  return true;
}

/**
 * Creates a window
 *
 * \return true on success, false otherwise
 */
bool RiscosGui::create_window()
{
    wimp_WINDOW(0) window = {
                { 400, 400, 800, 800 },
                0, 0,
                wimp_TOP,
                wimp_WINDOW_MOVEABLE | wimp_WINDOW_BACK_ICON |
                wimp_WINDOW_CLOSE_ICON | wimp_WINDOW_TITLE_ICON |
                wimp_WINDOW_NEW_FORMAT | wimp_WINDOW_IGNORE_XEXTENT |
                wimp_WINDOW_IGNORE_YEXTENT,
                wimp_COLOUR_BLACK, wimp_COLOUR_LIGHT_GREY,
                wimp_COLOUR_BLACK, wimp_COLOUR_WHITE,
                wimp_COLOUR_DARK_GREY, wimp_COLOUR_DARK_GREY,
                wimp_COLOUR_CREAM,
                0,
                { 0, 0, 65535, 65535 },
                wimp_ICON_TEXT | wimp_ICON_HCENTRED,
                wimp_BUTTON_CLICK << wimp_ICON_BUTTON_TYPE_SHIFT,
                wimpspriteop_AREA,
                2, 1,
                { "Gnash" },
                0
    };
    os_error *error;

    error = xwimp_create_window((wimp_window *)&window, &_window);
    if (error) {
      log_msg("%s\n", error->errmess);
      return false;
    }

    return true;
}

int
RiscosGui::valid_coord(int coord, int max)
{
	if (coord<0) return 0;
	else if (coord>=max) return max;
	return coord;
}

/**
 * Poll the mouse state, checking for movement or button releases
 *
 * \return true on success, false on error
 */
bool
RiscosGui::poll_mouse_state()
{
  wimp_pointer ptr;
  os_error *error;

  error = xwimp_get_pointer_info(&ptr);
  if (error) {
    log_msg("%s\n", error->errmess);
    return false;
  }

  if (_mouse_state) {
    // Any buttons released?
    if ((ptr.buttons & 0xf) != _mouse_state) {
      int mask = (ptr.buttons & 0xf) ^ _mouse_state;
      /* Flip select and adjust */
      if ((mask & 0x1) && !(mask & 0x4))
        mask = 0x4 | (mask & 0x2);
      else if (!(mask & 0x1) && (mask & 0x4))
        mask = 0x1 | (mask & 0x2);
      notify_mouse_clicked(false, mask);

      _mouse_state = ptr.buttons & 0xf;
    }
  }

  if (_mouse_track) {
    if (ptr.pos.x != _mouse_x && ptr.pos.y != _mouse_y) {
      _mouse_x = ptr.pos.x;
      _mouse_y = ptr.pos.y;

      notify_mouse_moved((_mouse_x - _window_vis.x0) / 2,
                   (_window_vis.y1 - _mouse_y) / 2);
    }
  }

  return true;
}

/**
 * Internal buffer rendering routine
 *
 * \param full True iff full redraw (i.e wimp_redraw_window)
 * \param draw Redraw block
 */
void
RiscosGui::renderBufferInternal(bool full, wimp_draw &draw)
{
    os_error *error;
    osbool more;
    int clip_x0, clip_y0, clip_x1, clip_y1;

    if (full)
      error = xwimp_redraw_window(&draw, &more);
    else
      error = xwimp_update_window(&draw, &more);

    if (error) {
      log_msg("%s\n", error->errmess);
      return;
    }

    while (more) {
      /* Calculate clip rectangle, flipping Y coordinates for Gnash */
      clip_x0 = (draw.clip.x0 - draw.box.x0) / 2;
      clip_y0 = (draw.box.y1 - draw.clip.y1) / 2;
      clip_x1 = (draw.clip.x1 - draw.box.x0) / 2;
      clip_y1 = (draw.box.y1 - draw.clip.y0) / 2;

      glue.render(_window_vis.x0 / 2,
                _screen_height - (_window_vis.y1 / 2),
                clip_x0, clip_y0, clip_x1, clip_y1);

      error = xwimp_get_rectangle(&draw, &more);
      if (error) {
        log_msg("%s\n", error->errmess);
        return;
      }
    }
}

// end of namespace gnash
}
