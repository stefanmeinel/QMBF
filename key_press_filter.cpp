// note: this is only used for OS X

#include "key_press_filter.h"


KeyPressFilter::KeyPressFilter(QObject* parent)
{
}


bool KeyPressFilter::eventFilter(QObject *obj, QEvent *event)
{
  if (event->type() == QEvent::KeyPress)
  {
    QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
    emit key_pressed(keyEvent);
  }
  // standard event processing
  return QObject::eventFilter(obj, event);
}
