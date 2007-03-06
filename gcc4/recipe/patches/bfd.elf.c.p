--- bfd/elf.c.orig	2007-03-05 20:47:01.000000000 +0000
+++ bfd/elf.c	2007-03-06 20:19:27.000000000 +0000
@@ -3749,9 +3749,17 @@
 	}
       else if (! writable
 	       && (hdr->flags & SEC_READONLY) == 0
+#if 0
+	/* RISC OS: We need separate segments for text & data, but when the
+	   data segment immediately follows the text segment, LD insists on
+	   merging them into one. This change forces LD to maintain separate
+	   segments. Perhaps this functionality should be offered as a command
+	   line option. */
 	       && (((last_hdr->lma + last_size - 1)
 		    & ~(maxpagesize - 1))
-		   != (hdr->lma & ~(maxpagesize - 1))))
+		   != (hdr->lma & ~(maxpagesize - 1)))
+#endif
+	      )
 	{
 	  /* We don't want to put a writable section in a read only
              segment, unless they are on the same page in memory
