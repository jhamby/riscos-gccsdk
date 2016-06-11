--- pdf/Makefile.cross.orig	2015-07-23 13:04:49.408102365 +0100
+++ pdf/Makefile.cross	2015-07-23 13:05:18.680103476 +0100
@@ -12,7 +12,7 @@
 CXXFLAGS = -Wall -DACORN -D__APCS_32 -O3  -I . -I ../GuiLib/Task -I ../GuiLib/Wimp -I../GuiLib/Util -I../GuiLib/GuiFlex -I../DrawFile -I ../xpdf -I ../xpdf/xpdf/xpdf -I ../xpdf/xpdf/goo -I ../xpdf/xpdf/fofi -I ../xpdf/xpdf/splash
 
 !RunImage: $(OBJS)
-	gcc $(CXXFLAGS) -static -o !RunImage$(AB_EXEEXT) $(OBJS) -L ../DrawFile -lDrawFile -L ../GuiLib/GuiFlex -lGuiFlex -L ../GuiLib/Task -lGuiTask -L ../GuiLib/Util -lGuiUtil -L ../GuiLib/Wimp -lGuiWimp -L ../xpdf -lpdf -L ../xpdf/xpdf/goo -lGoo -L ../xpdf/xpdf/fofi -lfofi -L ../xpdf/xpdf/splash -lsplash -L/linux1/gccsdk/env/lib -lfreetype -lz -lstdc++
+	gcc $(CXXFLAGS) -static -o !RunImage$(AB_EXEEXT) $(OBJS) -L ../DrawFile -lDrawFile -L ../GuiLib/GuiFlex -lGuiFlex -L ../GuiLib/Task -lGuiTask -L ../GuiLib/Util -lGuiUtil -L ../GuiLib/Wimp -lGuiWimp -L ../xpdf -lpdf -L ../xpdf/xpdf/goo -lGoo -L ../xpdf/xpdf/fofi -lfofi -L ../xpdf/xpdf/splash -lsplash -L/linux1/gccsdk/env/lib -lfreetype -lpng -lz -lstdc++ -lbz2
 
 clean:
 	rm -f $(OBJS) !RunImage$(AB_EXEEXT)
