/* gdkglobals-riscos.c
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"
#include "gdktypes.h"
#include "gdkprivate.h"
#include "gdkprivate-riscos.h"
#include "oslib/wimp.h"
#include "oslib/toolbox.h"

GdkDisplay *_gdk_display = NULL;
GdkScreen *_gdk_screen = NULL;
GdkWindow *_gdk_root = NULL;
toolbox_block *_tb_block = NULL;
messagetrans_control_block *_tb_messages = NULL;
char const *_resource_dir_name = NULL;
swap_redblue_t swap_redblue;
