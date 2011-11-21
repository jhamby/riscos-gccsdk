--- bfd/elf.c.orig	2011-07-03 15:37:06.000000000 +0200
+++ bfd/elf.c	2011-11-21 19:42:18.282241893 +0100
@@ -1262,6 +1262,7 @@ _bfd_elf_print_private_bfd_data (bfd *ab
 		}
 	      break;
 
+	    case DT_RISCOS_PIC: name ="RISCOS_PIC"; break;
 	    case DT_NEEDED: name = "NEEDED"; stringp = TRUE; break;
 	    case DT_PLTRELSZ: name = "PLTRELSZ"; break;
 	    case DT_PLTGOT: name = "PLTGOT"; break;
@@ -3923,8 +3924,17 @@ _bfd_elf_map_sections_to_segments (bfd *
 	    }
 	  else if (! writable
 		   && (hdr->flags & SEC_READONLY) == 0
+#if 0
+	/* RISC OS: We need separate segments for text & data, but when the
+ 	   data segment immediately follows the text segment, LD insists on
+	   merging them into one. This change forces LD to maintain separate
+	   segments. Perhaps this functionality should be offered as a command
+	   line option. */
 		   && (((last_hdr->lma + last_size - 1) & -maxpagesize)
 		       != (hdr->lma & -maxpagesize)))
+#else
+		  )
+#endif
 	    {
 	      /* We don't want to put a writable section in a read only
 		 segment, unless they are on the same page in memory
