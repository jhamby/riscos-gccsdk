--- bfd/elf.c.orig	2018-01-13 13:31:15.000000000 +0000
+++ bfd/elf.c	2018-05-28 13:45:33.548785830 +0100
@@ -4771,10 +4771,17 @@
 	    }
 	  else if (! writable
 		   && (hdr->flags & SEC_READONLY) == 0
+#if 0
+	/* RISC OS: We need separate segments for text & data, but when the
+	   data segment immediately follows the text segment, LD insists on
+	   merging them into one. This change forces LD to maintain separate
+	   segments.  */
 		   && ((info != NULL
 			&& info->relro_end > info->relro_start)
 		       || (((last_hdr->lma + last_size - 1) & -maxpagesize)
-			   != (hdr->lma & -maxpagesize))))
+			   != (hdr->lma & -maxpagesize)))
+#endif
+		   )
 	    {
 	      /* We don't want to put a writable section in a read only
 		 segment, unless they are on the same page in memory
