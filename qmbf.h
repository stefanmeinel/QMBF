// note: this is only used for OS X

#ifndef QMBF_H
#define QMBF_H

#include <QtWidgets>
#include "mainwindow.h"

class qmbf : public QApplication
{
    Q_OBJECT
  public:
    qmbf(int argc, char* argv[]);
    virtual ~qmbf();
  protected:
     bool event(QEvent*);
  private:
     void loadFile(const QString& fileName);
  public:
    MainWindow* mainwin;
};

#endif
