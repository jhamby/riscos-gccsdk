--- interface/khronos/vg/vg_int_mat3x3.c.orig	2015-06-18 20:04:26.000000000 +0100
+++ interface/khronos/vg/vg_int_mat3x3.c	2015-06-25 15:11:21.244847790 +0100
@@ -25,6 +25,12 @@
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
+#ifndef __TARGET_MODULE__
+/* Avoid referencing vcos from the API libraries, ig, libOpenVG.so.  */
+#define vcos_assert assert
+#define vcos_verify(cond) (cond) ? 1 : (printf("%s:%d - %s", __FILE__, __LINE__, #cond), 0)
+#endif
+
 #include "interface/khronos/common/khrn_int_common.h"
 #include "interface/khronos/vg/vg_int_mat3x3.h"
 #include "interface/khronos/common/khrn_int_math.h"
