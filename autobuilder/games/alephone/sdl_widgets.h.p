--- Source_Files/Misc/sdl_widgets.h.orig	2005-08-20 15:16:02.000000000 +0100
+++ Source_Files/Misc/sdl_widgets.h	2008-09-04 11:26:53.878000000 +0100
@@ -944,7 +944,7 @@
 	virtual ~SelectorWidget () {}
 
 protected:
-	SelectorWidget::SelectorWidget (widget* in_widget)
+	SelectorWidget (widget* in_widget)
 		: SDLWidgetWidget (in_widget)
 		, m_callback (NULL)
 		{}
@@ -955,7 +955,7 @@
 class PopupSelectorWidget : public SelectorWidget
 {
 public:
-	PopupSelectorWidget::PopupSelectorWidget (w_select_popup* select_popup_w)
+	PopupSelectorWidget (w_select_popup* select_popup_w)
 		: SelectorWidget (select_popup_w)
 		, m_select_popup (select_popup_w)
 		{ select_popup_w->set_popup_callback (boost::bind(&PopupSelectorWidget::massage_callback, this, _1), NULL); }
@@ -974,7 +974,7 @@
 class SelectSelectorWidget : public SelectorWidget
 {
 public:
-	SelectSelectorWidget::SelectSelectorWidget (w_select* select_w)
+	SelectSelectorWidget (w_select* select_w)
 		: SelectorWidget (select_w)
 		, m_select (select_w)
 		{ m_select->set_selection_changed_callback (boost::bind(&SelectSelectorWidget::massage_callback, this, _1)); }
@@ -996,7 +996,7 @@
 class ColourSelectorWidget : public SelectSelectorWidget
 {
 public:
-	ColourSelectorWidget::ColourSelectorWidget (w_player_color* player_color_w)
+	ColourSelectorWidget (w_player_color* player_color_w)
 		: SelectSelectorWidget (player_color_w) {}
 
 	// We ignore the labels and use swatches of colour instead
@@ -1028,7 +1028,7 @@
 class StaticTextWidget : public SDLWidgetWidget
 {
 public:
-	StaticTextWidget::StaticTextWidget (w_static_text* static_text_w)
+	StaticTextWidget (w_static_text* static_text_w)
 		: SDLWidgetWidget (static_text_w)
 		, m_static_text (static_text_w)
 		{}
