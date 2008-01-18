--- app/plug-in/gimpplugin.c.orig	2008-01-18 00:55:10.000000000 -0800
+++ app/plug-in/gimpplugin.c	2008-01-18 00:56:10.000000000 -0800
@@ -452,9 +452,9 @@
           /*  If the plug-in opened a process group, kill the group instead
            *  of only the plug-in, so we kill the plug-in's children too
            */
-          if (getpgid (0) != getpgid (plug_in->pid))
+/*          if (getpgid (0) != getpgid (plug_in->pid))
             status = kill (- plug_in->pid, SIGKILL);
-          else
+          else */
             status = kill (plug_in->pid, SIGKILL);
         }
 
