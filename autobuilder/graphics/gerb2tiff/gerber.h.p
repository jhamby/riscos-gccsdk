--- gerber.h.orig	2009-08-27 11:18:15.000000000 -0700
+++ gerber.h	2009-08-27 11:18:28.000000000 -0700
@@ -190,7 +190,7 @@
 		}
 		const char * unitText()	{	return (IsInches) ? "\"" :  "mm";	}
 		double getCoordinate( char * const text, int axisNumber, bool alwaysAbsolute = false);
-		void Gerber::calculateArc(Arc &arc);
+		void calculateArc(Arc &arc);
 		void process_AD_block(int DCode);
 		void process_D_command(int code);
 		void process_G_command(int code);
