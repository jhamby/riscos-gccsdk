--- src/ui/Animation.h
+++ src/ui/Animation.h
@@ -51,6 +51,9 @@ public slots:
 	void start();
 	void stop();
 
+protected slots:
+	void setFrameChanged();
+
 protected:
 	void createSvgRenderer();
 


