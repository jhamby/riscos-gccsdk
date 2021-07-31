--- src/core/TransfersManager.cpp
+++ src/core/TransfersManager.cpp
@@ -952,10 +952,10 @@ void TransfersManager::addTransfer(Transfer *transfer)
 
 	transfer->setUpdateInterval(500);
 
-	connect(transfer, &Transfer::started, m_instance, &TransfersManager::handleTransferStarted);
-	connect(transfer, &Transfer::finished, m_instance, &TransfersManager::handleTransferFinished);
-	connect(transfer, &Transfer::changed, m_instance, &TransfersManager::handleTransferChanged);
-	connect(transfer, &Transfer::stopped, m_instance, &TransfersManager::handleTransferStopped);
+	connect(transfer, SIGNAL(started()), m_instance, SLOT(handleTransferStarted()));
+	connect(transfer, SIGNAL(finished()), m_instance, SLOT(handleTransferFinished()));
+	connect(transfer, SIGNAL(changed()), m_instance, SLOT(handleTransferChanged()));
+	connect(transfer, SIGNAL(stopped()), m_instance, SLOT(handleTransferStopped()));
 
 	if (transfer->getOptions().testFlag(Transfer::CanNotifyOption) && transfer->getState() != Transfer::CancelledState)
 	{


