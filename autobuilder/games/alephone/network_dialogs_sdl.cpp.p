--- Source_Files/Network/network_dialogs_sdl.cpp.orig	2005-10-02 13:45:35.000000000 +0100
+++ Source_Files/Network/network_dialogs_sdl.cpp	2008-09-04 11:19:43.349000000 +0100
@@ -838,7 +838,7 @@
 class SdlGatherDialog : public GatherDialog
 {
 public:
-	SdlGatherDialog::SdlGatherDialog()
+	SdlGatherDialog()
 	{
 		m_dialog.add(new w_static_text("GATHER NETWORK GAME", TITLE_FONT, TITLE_COLOR));
 	
@@ -921,7 +921,7 @@
 class SdlJoinDialog : public JoinDialog
 {
 public:
-	SdlJoinDialog::SdlJoinDialog()
+	SdlJoinDialog()
 	{
 		m_dialog.add(new w_static_text("JOIN NETWORK GAME", TITLE_FONT, TITLE_COLOR));
 		m_dialog.add(new w_spacer());
