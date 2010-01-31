--- bfd/opncls.c.orig	2008-12-21 16:50:19.000000000 +0100
+++ bfd/opncls.c	2008-12-21 16:53:02.000000000 +0100
@@ -27,6 +27,12 @@
 #include "libbfd.h"
 #include "libiberty.h"
 
+#ifdef __riscos__
+#include <limits.h>
+#include <kernel.h>
+#include <unixlib/local.h>
+#endif
+
 #ifndef S_IXUSR
 #define S_IXUSR 0100	/* Execute by owner.  */
 #endif
@@ -637,6 +643,23 @@ bfd_close (bfd *abfd)
 	  chmod (abfd->filename,
 		 (0777
 		  & (buf.st_mode | ((S_IXUSR | S_IXGRP | S_IXOTH) &~ mask))));
+
+#ifdef __riscos__
+	  /* When it is ELF format and we're running on RISC OS, set
+	     the 0xE1F filetype.  */
+	  if (bfd_get_flavour (abfd) == bfd_target_elf_flavour)
+	    {
+	      char robuf[_POSIX_PATH_MAX];
+	      /* Set filetype ELF.  */
+	      if (__riscosify_std (abfd->filename, 0, robuf, sizeof (robuf), NULL))
+		{
+		  _kernel_osfile_block bl;
+
+		  bl.load = 0xE1F; /* ELF binary */
+		  _kernel_osfile (18, robuf, &bl);
+		}
+	    }
+#endif
 	}
     }
 
@@ -690,6 +713,23 @@ bfd_close_all_done (bfd *abfd)
 	  chmod (abfd->filename,
 		 (0777
 		  & (buf.st_mode | ((S_IXUSR | S_IXGRP | S_IXOTH) &~ mask))));
+
+#ifdef __riscos__
+	  /* When it is ELF format and we're running on RISC OS, set
+	     the 0xE1F filetype.  */
+	  if (bfd_get_flavour (abfd) == bfd_target_elf_flavour)
+	    {
+	      char robuf[_POSIX_PATH_MAX];
+	      /* Set filetype ELF.  */
+	      if (__riscosify_std (abfd->filename, 0, robuf, sizeof (robuf), NULL))
+		{
+		  _kernel_osfile_block bl;
+
+		  bl.load = 0xE1F; /* ELF binary */
+		  _kernel_osfile (18, robuf, &bl);
+		}
+	    }
+#endif
 	}
     }
 
