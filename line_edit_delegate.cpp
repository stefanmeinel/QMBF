#include <QtWidgets>
#include "line_edit_delegate.h"

LineEditDelegate::LineEditDelegate(bool readonly, bool doubleval, QObject *parent)
  : QItemDelegate(parent)
{
  _readonly=readonly;
  _doubleval=doubleval;
}

QWidget* LineEditDelegate::createEditor(QWidget *parent,
                                        const QStyleOptionViewItem &,
                                        const QModelIndex &) const
{
  QLineEdit* editor=new QLineEdit(parent);
  if(_readonly) editor->setReadOnly(true);
  if(_doubleval) editor->setValidator(new QDoubleValidator(editor));

#ifdef Q_WS_MACX
  KeyPressFilter *keyPressFilter = new KeyPressFilter();
  editor->installEventFilter(keyPressFilter);
  connect(keyPressFilter, SIGNAL(key_pressed(QKeyEvent*)), this, SLOT(key_pressed_slot(QKeyEvent*)));
#endif

  return editor;
}

void LineEditDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
  QString text=index.model()->data(index, Qt::DisplayRole).toString();

  QLineEdit* LineEdit=static_cast<QLineEdit*>(editor);
  LineEdit->setText(text);
}

void LineEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const
{
  QLineEdit* LineEdit=static_cast<QLineEdit*>(editor);
  QString text=LineEdit->text();

  model->setData(index, text, Qt::EditRole);
}

void LineEditDelegate::updateEditorGeometry(QWidget *editor,  const QStyleOptionViewItem &option,
                                            const QModelIndex &) const
{
  editor->setGeometry(option.rect);
}

#ifdef Q_WS_MACX
void LineEditDelegate::key_pressed_slot(QKeyEvent* key_event)
{
  emit key_pressed(key_event);
}
#endif
