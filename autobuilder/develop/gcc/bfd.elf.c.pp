--- bfd/elf.c.orig	2021-08-08 17:55:18.771497403 -0700
+++ bfd/elf.c	2021-08-08 18:09:40.999617586 -0700
@@ -4756,6 +4756,11 @@
 		 the previous section, then we need a new segment.  */
 	      new_segment = TRUE;
 	    }
+#if 0
+	/* RISC OS: We need separate segments for text & data, but when the
+	   data segment immediately follows the text segment, LD insists on
+	   merging them into one. This change forces LD to maintain separate
+	   segments.  */
 	  else if ((abfd->flags & D_PAGED) != 0
 		   && (((last_hdr->lma + last_size - 1) & -maxpagesize)
 		       == (hdr->lma & -maxpagesize)))
@@ -4764,6 +4769,7 @@
 		 pages onto the same memory page.  */
 	      new_segment = FALSE;
 	    }
+#endif
 	  /* In the next test we have to be careful when last_hdr->lma is close
 	     to the end of the address space.  If the aligned address wraps
 	     around to the start of the address space, then there are no more
@@ -4787,6 +4793,8 @@
 		 Consider .tbss sections as loaded for this purpose.  */
 	      new_segment = TRUE;
 	    }
+#if 0
+	/* RISC OS: We need separate segments for text & data. */
 	  else if ((abfd->flags & D_PAGED) == 0)
 	    {
 	      /* If the file is not demand paged, which means that we
@@ -4794,6 +4802,7 @@
 		 file, then there is no other reason for a new segment.  */
 	      new_segment = FALSE;
 	    }
+#endif
 	  else if (info != NULL
 		   && info->separate_code
 		   && executable != ((hdr->flags & SEC_CODE) != 0))
