#include <QApplication>

#ifdef Q_WS_MACX

  #include "qmbf.h"

  int main(int argc, char *argv[])
  {
    Q_INIT_RESOURCE(QMBF);
    qmbf app(argc, argv);
    app.setWindowIcon(QIcon(":/images/qmbf_48x48.png"));
    return app.exec();
  }

#else

  #include "mainwindow.h"

  int main(int argc, char *argv[])
  {
    Q_INIT_RESOURCE(QMBF);
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/images/qmbf_48x48.png"));
    if((app.arguments()).size()==2)
    {
      QString file=(app.arguments()).at(1);
      MainWindow mainwin(true, file);
      mainwin.show();
      return app.exec();
    }
    MainWindow mainwin(false, "");
    mainwin.show();
    return app.exec();
  }

#endif
