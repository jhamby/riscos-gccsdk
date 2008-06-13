--- gas/app.c.orig	2006-03-10 11:57:18.000000000 +0100
+++ gas/app.c	2008-06-13 02:05:12.000000000 +0200
@@ -561,8 +561,14 @@ do_scrub_chars (int (*get) (char *, int)
 	  ch = GET ();
 	  if (ch == EOF)
 	    {
+	      /* This buffer is here specifically so
+		 that the UNGET below will work.  */
+	      static char one_char_buf[1];
+
 	      as_warn (_("end of file in string; '%c' inserted"), quotechar);
 	      state = old_state;
+	      from = fromend = one_char_buf + 1;
+	      fromlen = 1;
 	      UNGET ('\n');
 	      PUT (quotechar);
 	    }
