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

static __inline void report_text(const char *s)
{
  __asm volatile (
		  "	MOV	r0, %0;\n"
		  "	SWI	%[report_text0];\n"
		  : /* no outputs */
		  : "r" (s), [report_text0] "i" (0x74C80)
		  : "a1", "cc");
}

void reporter_printf(const char *format, ...)
{
    char buffer[512];
    char *p = buffer;
    time_t t;
    struct tm *tmp;

    va_list ap;
    va_start (ap, format);

    t = time(NULL);
    tmp = localtime(&t);

    *p++ = '[';
    if (&__program_name)
    {
      strcpy(p, __program_name);
      p += strlen(p);
      *p++ = ':';
      *p = '\0';
    }
    p += strftime(p, sizeof(buffer) - strlen(buffer) - 1, "%d/%m  %T", tmp);
    *p++ = ']';
    *p++ = ' ';
    *p++ = ':';
    *p++ = ' ';
    *p = '\0';

    p += vsnprintf(p, sizeof(buffer) - strlen(buffer) - 1, format, ap);
    *p++ = '\n';
    *p = '\0';

    report_text(buffer);
}
