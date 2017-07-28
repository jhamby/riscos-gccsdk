--- lib/link.c.orig	2017-02-05 03:41:52.000000000 +1300
+++ lib/link.c	2017-07-26 23:47:56.819392605 +1200
@@ -14,7 +14,7 @@
 
    You should have received a copy of the GNU General Public License
    along with this program; if not, see <http://www.gnu.org/licenses/>.  */
-
+#ifndef __riscos__
 #include <config.h>
 
 #include <unistd.h>
@@ -209,3 +209,4 @@
   return link (file1, file2);
 }
 #endif /* HAVE_LINK */
+#endif
