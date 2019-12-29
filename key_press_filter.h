// note: this is only used for OS X

#ifndef KEY_PRESS_FILTER_H
#define KEY_PRESS_FILTER_H

#include <QObject>
#include <QEvent>
#include <QKeyEvent>

class KeyPressFilter : public QObject
{
    Q_OBJECT
  public:
    KeyPressFilter(QObject* parent=0);

  protected:
    bool eventFilter(QObject *obj, QEvent *event);

  signals:
    void key_pressed(QKeyEvent*);

};

#endif
