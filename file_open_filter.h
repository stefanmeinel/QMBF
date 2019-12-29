// note: this is only used for OS X

#ifndef FILE_OPEN_FILTER_H
#define FILE_OPEN_FILTER_H

#include <QObject>
#include <QEvent>
#include <QFileOpenEvent>

class FileOpenFilter : public QObject
{
    Q_OBJECT
  public:
    FileOpenFilter(QObject* parent=0);

  protected:
    bool eventFilter(QObject *obj, QEvent *event);

  signals:
    void file_open(QString filename);

};

#endif
