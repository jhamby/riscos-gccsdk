--- src/ui/Animation.cpp
+++ src/ui/Animation.cpp
@@ -40,12 +40,17 @@ Animation::Animation(const QString &path, QObject *parent) : QObject(parent),
 		{
 			m_format = GifFormat;
 
+#ifndef __riscos__
 			connect(m_gifMovie, &QMovie::frameChanged, [&](int frame)
 			{
 				Q_UNUSED(frame)
 
 				emit frameChanged();
 			});
+#else
+
+			connect(m_gifMovie, SIGNAL(frameChanged(int)), this, SLOT(setFrameChanged()));
+#endif
 		}
 		else
 		{
@@ -96,7 +101,7 @@ void Animation::createSvgRenderer()
 	{
 		m_format = SvgFormat;
 
-		connect(m_svgRenderer, &QSvgRenderer::repaintNeeded, this, &Animation::frameChanged);
+		connect(m_svgRenderer, SIGNAL(repaintNeeded()), this, SLOT(frameChanged()));
 	}
 	else
 	{
@@ -107,6 +112,14 @@ void Animation::createSvgRenderer()
 	}
 }
 
+#ifdef __riscos__
+void Animation::setFrameChanged()
+{
+
+		emit frameChanged();
+}
+#endif
+
 void Animation::start()
 {
 	if (!isRunning() && !m_path.isEmpty() && m_format != InvalidFormat)


