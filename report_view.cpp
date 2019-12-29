#include "report_view.h"

ReportView::ReportView(QString dir, QWidget* parent)
{
  current_dir=dir;

  setWindowFlags(Qt::Dialog);
  setWindowTitle("QMBF Report");

  vlayout=new QVBoxLayout();

  terminal=new QTextEdit();
  terminal->setReadOnly(true);
  terminal->setLineWrapMode(QTextEdit::NoWrap);
  terminal->setCurrentFont(QFont("Courier"));

  hlayout=new QHBoxLayout();

  savebutton=new QPushButton("Save As...");
  connect(savebutton, SIGNAL(clicked()), this, SLOT(save()));

  closebutton=new QPushButton("Close");
  connect(closebutton, SIGNAL(clicked()), this, SLOT(close()));

  vlayout->addWidget(terminal);

  hlayout->addStretch();
  hlayout->addWidget(savebutton);
  hlayout->addWidget(closebutton);
  hlayout->addStretch();

  vlayout->addLayout(hlayout);

  setLayout(vlayout);
  resize(900, 600);
}

void ReportView::save()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save QMBF Report"),
                                                  current_dir,
                                                  tr("Text files (*.txt);; All files (*)"));
  if(!fileName.isEmpty())
  {
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
      QMessageBox::warning(this, tr("QMBF"),
                           tr("Cannot write file %1:\n%2.")
                           .arg(fileName)
                           .arg(file.errorString()));
      return;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << terminal->toPlainText();
    QApplication::restoreOverrideCursor();
  }
}
