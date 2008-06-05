===================================================================
RCS file: /cvs/src/src/gas/app.c,v
retrieving revision 1.43
retrieving revision 1.44
diff -u -r1.43 -r1.44
--- gas/app.c	2007/07/03 11:01:02	1.43
+++ gas/app.c	2007/07/20 08:20:27	1.44
@@ -558,8 +558,14 @@
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
