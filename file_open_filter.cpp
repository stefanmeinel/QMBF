// note: this is only used for OS X

#include "file_open_filter.h"


FileOpenFilter::FileOpenFilter(QObject* parent)
{
}


bool FileOpenFilter::eventFilter(QObject *obj, QEvent *event)
{
  if (event->type() == QEvent::FileOpen)
  {
    emit file_open(static_cast<QFileOpenEvent*>(event)->file());
  }
  // standard event processing
  return QObject::eventFilter(obj, event);
}
