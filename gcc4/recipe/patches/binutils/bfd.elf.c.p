--- bfd/elf.c.orig	2009-09-10 13:47:12.000000000 +0200
+++ bfd/elf.c	2010-01-31 16:31:08.065017338 +0100
@@ -3759,9 +3759,18 @@ _bfd_elf_map_sections_to_segments (bfd *
 	    }
 	  else if (! writable
 		   && (hdr->flags & SEC_READONLY) == 0
+#if 0
+	/* RISC OS: We need separate segments for text & data, but when the
+ 	   data segment immediately follows the text segment, LD insists on
+	   merging them into one. This change forces LD to maintain separate
+	   segments. Perhaps this functionality should be offered as a command
+	   line option. */
 		   && (((last_hdr->lma + last_size - 1)
 			& ~(maxpagesize - 1))
 		       != (hdr->lma & ~(maxpagesize - 1))))
+#else
+		  )
+#endif
 	    {
 	      /* We don't want to put a writable section in a read only
 		 segment, unless they are on the same page in memory
