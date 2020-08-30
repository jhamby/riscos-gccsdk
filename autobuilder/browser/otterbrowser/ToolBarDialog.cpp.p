--- src/ui/ToolBarDialog.cpp
+++ src/ui/ToolBarDialog.cpp
@@ -378,6 +378,7 @@ void ToolBarDialog::editEntry()
 
 			textEntry.widget->setDefaultValue(options.value(QLatin1String("optionName"), choices.first()).toString().section(QLatin1Char('/'), -1));
 
+#ifndef __riscos__
 			connect(optionNameEntry.widget, &OptionWidget::commitData, [&]()
 			{
 				const bool needsReset(textEntry.widget->getDefaultValue() == textEntry.widget->getValue());
@@ -389,6 +390,9 @@ void ToolBarDialog::editEntry()
 					textEntry.widget->setValue(textEntry.widget->getDefaultValue());
 				}
 			});
+#else
+			connect(optionNameEntry.widget, SIGNAL(commitData()), this, SLOT(dialogDefaultValue(OptionEntry, OptionEntry)));
+#endif
 		}
 		else if (identifier == QLatin1String("ContentBlockingInformationWidget"))
 		{
@@ -474,6 +478,20 @@ void ToolBarDialog::editEntry()
 	delete item;
 }
 
+#ifdef __riscos__
+void ToolBarDialog::dialogDefaultValue(OptionEntry textEntry, OptionEntry optionNameEntry)
+{
+	const bool needsReset(textEntry.widget->getDefaultValue() == textEntry.widget->getValue());
+
+	textEntry.widget->setDefaultValue(optionNameEntry.widget->getValue().toString().section(QLatin1Char('/'), -1));
+
+	if (needsReset)
+	{
+		textEntry.widget->setValue(textEntry.widget->getDefaultValue());
+	}
+}
+#endif
+
 void ToolBarDialog::addBookmark(QAction *action)
 {
 	if (action && action->data().type() == QVariant::ULongLong)


