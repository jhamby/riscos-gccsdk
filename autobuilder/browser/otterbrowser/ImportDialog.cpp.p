--- src/ui/ImportDialog.cpp
+++ src/ui/ImportDialog.cpp
@@ -53,9 +53,9 @@ ImportDialog::ImportDialog(Importer *importer, QWidget *parent) : Dialog(parent)
 
 	setWindowTitle(m_importer->getTitle());
 
-	connect(m_ui->importPathWidget, &FilePathWidget::pathChanged, this, &ImportDialog::setPath);
-	connect(m_ui->buttonBox, &QDialogButtonBox::accepted, this, &ImportDialog::handleImportRequested);
-	connect(m_ui->buttonBox, &QDialogButtonBox::rejected, this, &ImportDialog::reject);
+	connect(m_ui->importPathWidget, SIGNAL(pathChanged(QString)), this, SLOT(setPath(QString)));
+	connect(m_ui->buttonBox, SIGNAL(accepted()), this, SLOT(handleImportRequested()));
+	connect(m_ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
 }
 
 void ImportDialog::closeEvent(QCloseEvent *event)
@@ -133,11 +133,11 @@ void ImportDialog::handleImportRequested()
 	m_ui->buttonBox->setEnabled(m_importer->canCancel());
 	m_ui->stackedWidget->setCurrentIndex(1);
 
-	disconnect(m_ui->buttonBox, &QDialogButtonBox::rejected, this, &ImportDialog::reject);
-	connect(m_ui->buttonBox, &QDialogButtonBox::rejected, m_importer, &Importer::cancel);
-	connect(m_importer, &Importer::importStarted, this, &ImportDialog::handleImportStarted);
-	connect(m_importer, &Importer::importProgress, this, &ImportDialog::handleImportProgress);
-	connect(m_importer, &Importer::importFinished, this, &ImportDialog::handleImportFinished);
+	disconnect(m_ui->buttonBox, SIGNAL(rejected()), this, SIGNAL(reject()));
+	connect(m_ui->buttonBox, SIGNAL(rejected()), m_importer, SIGNAL(cancel()));
+	connect(m_importer, SIGNAL(importStarted()), this, SLOT(handleImportStarted(Importer::ImportType, int)));
+	connect(m_importer, SIGNAL(importProgress()), this, SLOT(handleImportProgress(Importer::ImportType, int)));
+	connect(m_importer, SIGNAL(importFinished()), this, SLOT(handleImportFinished(Importer::ImportType, int)));
 
 	m_importer->import(m_path);
 }
@@ -193,8 +193,8 @@ void ImportDialog::handleImportFinished(Importer::ImportType type, Importer::Imp
 	m_ui->buttonBox->addButton(QDialogButtonBox::Close);
 	m_ui->buttonBox->setEnabled(true);
 
-	disconnect(m_ui->buttonBox, &QDialogButtonBox::rejected, m_importer, &Importer::cancel);
-	connect(m_ui->buttonBox, &QDialogButtonBox::rejected, this, &ImportDialog::close);
+	disconnect(m_ui->buttonBox, SIGNAL(rejected()), m_importer, SLOT(cancel()));
+	connect(m_ui->buttonBox, SIGNAL(rejected()), this, SLOT(close()));
 }
 
 }


