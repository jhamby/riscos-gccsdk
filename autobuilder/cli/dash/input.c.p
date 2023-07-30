--- src/input.c.orig	2020-07-07 23:58:29.000000000 -0700
+++ src/input.c	2023-07-30 14:06:55.237976197 -0700
@@ -31,7 +31,10 @@
  * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
  * SUCH DAMAGE.
  */
-
+ 
+#ifdef __riscos__
+#include <kernel.h> /* for RISC OS readline32 */
+#endif
 #include <stdio.h>	/* defines BUFSIZ */
 #include <fcntl.h>
 #include <unistd.h>
@@ -195,6 +198,25 @@
 
 	} else
 #endif
+
+#ifdef __riscos__
+	if (parsefile->fd == 0 && buf) {
+		_kernel_swi_regs regs;
+		regs.r[0] = (int)buf;
+		regs.r[1] = IBUFSIZ;
+		regs.r[2] = 32;
+		regs.r[3] = 126;
+		regs.r[4] = 0;
+		_kernel_swi(125,&regs,&regs);
+			nr=regs.r[1];
+			if (nr > 0) { 
+				buf[nr]='\n';
+			buf[++nr]='\0';
+			return nr;
+			}
+		}
+#endif
+ 
 		nr = read(parsefile->fd, buf, IBUFSIZ - 1);
 
 
