#ifndef LE_DELEGATE_H
#define LE_DELEGATE_H

#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QLineEdit>

#ifdef Q_WS_MACX
  #include "key_press_filter.h"
#endif

class LineEditDelegate : public QItemDelegate
{
  Q_OBJECT

  public:
    LineEditDelegate(bool readonly, bool doubleval, QObject* parent=0);

    QWidget *createEditor(QWidget* parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) const;

    void updateEditorGeometry(QWidget* editor,
                              const QStyleOptionViewItem& option,
                              const QModelIndex& index) const;

  private:
     bool _readonly;
     bool _doubleval;

#ifdef Q_WS_MACX
  signals:
    void key_pressed(QKeyEvent* key_event);

  private slots:
    void key_pressed_slot(QKeyEvent* key_event);
#endif

};

#endif
