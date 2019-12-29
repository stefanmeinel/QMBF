// note: this is only used for OS X

#include "qmbf.h"

qmbf::qmbf(int argc, char* argv[]) : QApplication(argc, argv)
{
  if((this->arguments()).size()==2)
  {
    QString file=(this->arguments()).at(1);
    mainwin = new MainWindow(true, file);
  }
  else
  {
    mainwin = new MainWindow(false, "");
  }
  mainwin->show();
}

qmbf::~qmbf()
{
}

void qmbf::loadFile(const QString &fileName)
{
#ifdef Q_WS_MACX
  mainwin->loadFile(fileName);
#endif
}

bool qmbf::event(QEvent *event)
{
#ifdef Q_WS_MACX
  switch (event->type())
  {
    case QEvent::FileOpen:
      loadFile(static_cast<QFileOpenEvent*>(event)->file());
      return true;
    default:
      return QApplication::event(event);
  }
#endif
  return QApplication::event(event);
}
