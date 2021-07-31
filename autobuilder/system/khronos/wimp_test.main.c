/*
 * Copyright (c) 2015, GCCSDK Developers.
 * Written by Lee Noar.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the copyright holder nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "oslib/wimp.h"
#include "oslib/os.h"
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "bcm_host.h"
#include "EGL/egl.h"
#include "GLES/gl.h"
#include "GLES2/gl2.h"
#include "EGL/eglext.h"

#include "interface/vmcs_host/vc_vchi_dispmanx.h"

#define wimp_INVALID_WINDOW (wimp_w)0xFFFFFFFF

#define CHECK(x) if (!(x)) { fprintf(stderr, "Failed at line %d\n", __LINE__); }

static DISPMANX_DISPLAY_HANDLE_T dispman_display;
static EGLDisplay egl_display;

static wimp_t TaskHandle;

static wimp_block poll_block;

static const char title[] = "OpenGL";

static int eigen_x;
static int eigen_y;
static int screen_width;
static int screen_height;

typedef struct opengl_window
{
  wimp_w handle;
  EGL_DISPMANX_WINDOW_T nativewindow;
  EGLDisplay display;
  EGLContext context;
  EGLSurface surface;

} opengl_window;

static opengl_window main_window;

static void create_layer(opengl_window *window, int x, int y, int width, int height)
{
  DISPMANX_UPDATE_HANDLE_T dispman_update;
  DISPMANX_ELEMENT_HANDLE_T dispman_element;
  VC_RECT_T dst_rect;
  VC_RECT_T src_rect;

  dst_rect.x = x;
  dst_rect.y = y;
  dst_rect.width = width;
  dst_rect.height = height;

  src_rect.x = 0;
  src_rect.y = 0;
  src_rect.width = width << 16;
  src_rect.height = height << 16;
  dispman_update = vc_dispmanx_update_start(0);
         
  dispman_element = vc_dispmanx_element_add (dispman_update, dispman_display,
					      1/*layer*/, &dst_rect, 0/*src*/,
					      &src_rect, DISPMANX_PROTECTION_NONE,
					      0 /*alpha*/, 0/*clamp*/, 0/*transform*/);
      
  vc_dispmanx_update_submit_sync(dispman_update);

  window->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  window->nativewindow.element = dispman_element;
  window->nativewindow.width = width;
  window->nativewindow.height = height;
}

static void move_resize_layer(opengl_window *window, int x, int y, int width, int height)
{
  DISPMANX_UPDATE_HANDLE_T dispman_update;

  VC_RECT_T dst_rect;
  dst_rect.x = x;
  dst_rect.y = y;
  dst_rect.width = width;
  dst_rect.height = height;

  VC_RECT_T src_rect;
  src_rect.x = 0;
  src_rect.y = 0;
  src_rect.width = width << 16;
  src_rect.height = height << 16;

  dispman_update = vc_dispmanx_update_start(0);
  vc_dispmanx_element_change_attributes(dispman_update,
					window->nativewindow.element,
					ELEMENT_CHANGE_DEST_RECT | ELEMENT_CHANGE_SRC_RECT,
					0,
					0,
					&dst_rect,
					&src_rect,
					0,
					VC_IMAGE_ROT0 );
  vc_dispmanx_update_submit_sync(dispman_update);
}

static void remove_layer(opengl_window *window)
{
  DISPMANX_UPDATE_HANDLE_T dispman_update;
  dispman_update = vc_dispmanx_update_start(0);
  vc_dispmanx_element_remove(dispman_update, window->nativewindow.element);
  vc_dispmanx_update_submit_sync(dispman_update);
}

static void create_context(opengl_window *window)
{
  EGLBoolean result;
  EGLint num_config;

  static const EGLint attribute_list[] =
  {
     EGL_RED_SIZE, 8,
     EGL_GREEN_SIZE, 8,
     EGL_BLUE_SIZE, 8,
     EGL_ALPHA_SIZE, 8,
     EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
     EGL_NONE
  };

  static const EGLint context_attributes[] = 
  {
     EGL_CONTEXT_CLIENT_VERSION, 2,
     EGL_NONE
  };

  EGLConfig config;

  result = eglChooseConfig(window->display, attribute_list, &config, 1, &num_config);
  CHECK(result);
  window->context = eglCreateContext(window->display, config, EGL_NO_CONTEXT, context_attributes);
  CHECK(window->context);
  EGLint attribs[] = { EGL_RENDER_BUFFER, EGL_SINGLE_BUFFER, EGL_NONE, EGL_NONE };
  window->surface = eglCreateWindowSurface(window->display,
					   config,
					   &window->nativewindow,
					   attribs/*NULL*/ );
  CHECK(window->surface);
}

static const char *vertex_shader_src = 
"attribute vec2 coord2d;"
"attribute vec3 v_color;"
"varying vec3 f_color;"
"void main(void) {"
"  gl_Position = vec4(coord2d, 0.0, 1.0);"
"  f_color = v_color;"
"}";

static const char *fragment_shader_src =
"varying vec3 f_color;"
"void main(void) {"
"  gl_FragColor = vec4(f_color.x, f_color.y, f_color.z, 1.0);"
"}";

static void
draw_window(opengl_window *window)
{
  eglMakeCurrent(window->display, window->surface,
		 window->surface, window->context);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glViewport(0, 0, (GLsizei)window->nativewindow.width,
		   (GLsizei)window->nativewindow.height);

  GLuint vbo_triangle;
  GLint attribute_coord2d, attribute_v_color;

  GLfloat triangle_attributes[] = {
     0.0,  0.8,   1.0, 1.0, 0.0,
    -0.8, -0.8,   0.0, 0.0, 1.0,
     0.8, -0.8,   1.0, 0.0, 0.0,
  };


  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertex_shader_src, NULL);
  glCompileShader(vertexShader);
  GLint status;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    char buffer[512];
    glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
    printf("vertex shader log:\n%s\n", buffer);
  }

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragment_shader_src, NULL);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    char buffer[512];
    glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
    printf("fragment shader log:\n%s\n", buffer);
  }

  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);

  glLinkProgram(shaderProgram);
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
  if (status != GL_TRUE) {
    char buffer[512];
    glGetProgramInfoLog(shaderProgram, 512, NULL, buffer);
    printf("shader program log:\n%s\n", buffer);
  }
  glUseProgram(shaderProgram);

  const char *attribute_name = "v_color";
  attribute_v_color = glGetAttribLocation(shaderProgram, attribute_name);
  if (attribute_v_color == -1) {
    fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
    return;
  }

  GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(posAttrib);
  
  glGenBuffers(1, &vbo_triangle);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_attributes), triangle_attributes, GL_STATIC_DRAW);

  attribute_name = "coord2d";
  attribute_coord2d = glGetAttribLocation(shaderProgram, attribute_name);
  if (attribute_coord2d == -1) {
    fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
    return;
  }

  glEnableVertexAttribArray(attribute_coord2d);
  glEnableVertexAttribArray(attribute_v_color);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
  glVertexAttribPointer(
    attribute_coord2d,   // attribute
    2,                   // number of elements per vertex, here (x,y)
    GL_FLOAT,            // the type of each element
    GL_FALSE,            // take our values as-is
    5 * sizeof(GLfloat), // next coord2d appears every 5 floats
    0                    // offset of the first element
  );
  glVertexAttribPointer(
    attribute_v_color,      // attribute
    3,                      // number of elements per vertex, here (r,g,b)
    GL_FLOAT,               // the type of each element
    GL_FALSE,               // take our values as-is
    5 * sizeof(GLfloat),    // next color appears every 5 floats
    (GLvoid*) (2 * sizeof(GLfloat))  // offset of first element
  );

  
  glDrawArrays(GL_TRIANGLES, 0, 3);

  eglSwapBuffers(window->display, window->surface);
}

static wimp_w
create_window(void)
{
  _kernel_oserror *err;
  wimp_window window_blk;
  wimp_w handle;
  
  window_blk.flags = wimp_WINDOW_NEW_FORMAT |
		     wimp_WINDOW_MOVEABLE |
		     wimp_WINDOW_AUTO_REDRAW |
		     wimp_WINDOW_CLOSE_ICON |
		     wimp_WINDOW_TITLE_ICON;

  window_blk.title_fg = wimp_COLOUR_BLACK;

  window_blk.extent.x1 = screen_width << eigen_x;
  window_blk.extent.y0 = -(screen_height << eigen_y);

  window_blk.title_bg = wimp_COLOUR_LIGHT_GREY;
  window_blk.work_fg = wimp_COLOUR_BLACK;
  window_blk.work_bg = wimp_COLOUR_WHITE;
  window_blk.scroll_outer = wimp_COLOUR_MID_LIGHT_GREY;
  window_blk.scroll_inner = wimp_COLOUR_VERY_LIGHT_GREY;
  window_blk.highlight_bg = wimp_COLOUR_CREAM;
  window_blk.extra_flags = 0;
  window_blk.extent.x0 = 0;
  window_blk.extent.y1 = 0;
  window_blk.title_flags = wimp_ICON_TEXT |
			 wimp_ICON_HCENTRED |
			 wimp_ICON_VCENTRED;
  window_blk.work_flags = wimp_BUTTON_CLICK << wimp_ICON_BUTTON_TYPE_SHIFT;
  window_blk.xmin = 0;
  window_blk.ymin = 0;
  window_blk.icon_count = 0;

  strcpy(window_blk.title_data.text, title);

  err = _swix(Wimp_CreateWindow, _IN(1)|_OUT(0), &window_blk, &handle);
  if (err) {
    fprintf (stderr, "RISCOS: Failed to create window; %s", err->errmess);
    return wimp_INVALID_WINDOW;
  }

  return handle;
}

static void destroy_window(opengl_window *window)
{
  eglMakeCurrent(window->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
  eglDestroySurface(window->display, window->surface);
  eglDestroyContext(window->display, window->context);
  wimp_w block[] = { window->handle };
  _swix(Wimp_DeleteWindow, _IN(1), block);
}

static void open_window(opengl_window *window)
{
  wimp_open open;
  _kernel_oserror *err;

  open.w = window->handle;
  open.visible.x0 = 100;
  open.visible.x1 = open.visible.x0 + 500;
  open.visible.y1 = (1080 * 2) - 100;
  open.visible.y0 = open.visible.y1 - 500;
  open.xscroll = 0;
  open.yscroll = 0;
  open.next = wimp_BACKGROUND;
  err = _swix(Wimp_OpenWindow, _IN(1), &open);
  if (err) {
    fprintf (stderr, "RISCOS: Failed to open window; %s", err->errmess);
    return;
  }

  create_layer(window, open.visible.x0 >> eigen_x,
		       screen_height - (open.visible.y1 >> eigen_y),
		       500 >> eigen_x,
		       500 >> eigen_y);
  create_context(window);
}

	

//------------------------------------------------------------------------------

static void exit_func(void)
// Function to be passed to atexit().
{
   // Release OpenGL resources
   eglMakeCurrent(egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
   eglTerminate(egl_display);
} // exit_func()

//==============================================================================

static void app_init(void)
{
  os_VDU_VAR_LIST(6) var_list = { { os_VDUVAR_XEIG_FACTOR,
				    os_VDUVAR_YEIG_FACTOR,
				    os_VDUVAR_XWIND_LIMIT,
				    os_VDUVAR_YWIND_LIMIT,
				    os_VDUVAR_END_LIST } };

  _swix(OS_ReadVduVariables, _INR(0,1), &var_list, &var_list);

  eigen_x = var_list.var[0];
  eigen_y = var_list.var[1];
  screen_width = var_list.var[2] + 1;
  screen_height = var_list.var[3] + 1;

  EGLBoolean result;

  // Get an EGL display connection
  egl_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  assert(egl_display != EGL_NO_DISPLAY);

  // Initialize the EGL display connection
  result = eglInitialize(egl_display, NULL, NULL);
  assert(EGL_FALSE != result);
  eglBindAPI(EGL_OPENGL_ES_API);

  dispman_display = vc_dispmanx_display_open(0 /* LCD */);
}

static void handle_open_event(wimp_open *open)
{
  _swix(Wimp_OpenWindow, _IN(1), open);

  move_resize_layer(&main_window, open->visible.x0 >> eigen_x,
				  screen_height - (open->visible.y1 >> eigen_y),
				  (open->visible.x1 - open->visible.x0) >> eigen_x,
				  (open->visible.y1 - open->visible.y0) >> eigen_y);
}

static void handle_close_event(wimp_close *close)
{
  wimp_w block[1] = { close->w };
  _swix(Wimp_CloseWindow, _IN(1), block);

  remove_layer(&main_window);
  eglMakeCurrent(main_window.display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
  eglDestroySurface(main_window.display, main_window.surface);
  eglDestroyContext(main_window.display, main_window.context);
}

int main ()
{
  wimp_MESSAGE_LIST(2) message_list = { { 0 } };
  bool quit = false;

  bcm_host_init();
  app_init();

  _kernel_oserror *err;
  err = _swix(Wimp_Initialise, _INR(0,3)|_OUT(1),
	      wimp_VERSION_RO38, 0x4B534154,
	      "OpenGL", &message_list,
	      &TaskHandle);
  if (err != NULL)
    fprintf(stderr, "Failed to initialise task; %s\n", err->errmess);

  atexit(exit_func);

  main_window.handle = create_window();
  open_window(&main_window);
  draw_window(&main_window);

  while (!quit)
  {
    wimp_event_no event_type;

    _swix(Wimp_Poll, _INR(0,1)|_OUT(0), 0, &poll_block, &event_type);

    switch (event_type)
    {
      case wimp_OPEN_WINDOW_REQUEST:
	handle_open_event(&poll_block.open);
	break;
      case wimp_CLOSE_WINDOW_REQUEST:
	handle_close_event(&poll_block.close);
	quit = true;
	break;
      case wimp_USER_MESSAGE: {
	const wimp_message *message = &poll_block.message;
	if (message->action == message_QUIT)
	  quit = true;
	break;
      }
    }
  }

  destroy_window(&main_window);

  return 0;
}

