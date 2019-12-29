#include "constantswidget.h"

ConstantsWidget::ConstantsWidget(int n_constants, int max_n_constants, QWidget* parent)
  : QWidget(parent)
{
  max_constants=max_n_constants;
  n_consts=n_constants;
  createGadgets();
}

void ConstantsWidget::set_user_def(bool user_def)
{
  n_constants_Lb->setEnabled(user_def);
  n_constants_SB->setEnabled(user_def);
  constantnames_tableView->setEnabled(user_def);
}

bool ConstantsWidget::set_n_constants(int n_constants)
{
  if(n_constants>max_constants)
  {
    QMessageBox::warning(this, tr("QMBF"),
                               tr("Error: too many constants"));
    return false;
  }
  n_constants_SB->setValue(n_constants);
  return true;
}

int ConstantsWidget::get_n_constants()
{
  return n_consts;
}

void ConstantsWidget::set_constant_name(int c, std::string name)
{
  if(c>n_consts-1)
  {
    set_n_constants(c+1);
  }
  QModelIndex index = constantnames_model->index(c, 0, QModelIndex());
  constantnames_model->setData(index, QVariant(QString(name.c_str())), Qt::EditRole);
}

std::string ConstantsWidget::get_constant_name(int c)
{
  if(c>n_consts-1)
  {
    return std::string("");
  }
  QModelIndex index = constantnames_model->index(c, 0, QModelIndex());
  QString text=(constantnames_model->data(index, Qt::EditRole)).toString();
  return text.toStdString();
}

void ConstantsWidget::set_constant_value(int c, double value)
{
  if(c>n_consts-1)
  {
    set_n_constants(c+1);
  }
  QModelIndex index = constantvalues_model->index(c, 0, QModelIndex());
  constantvalues_model->setData(index, QVariant(double_to_QString(value)), Qt::EditRole);
}

double ConstantsWidget::get_constant_value(int c)
{
  if(c>n_consts-1)
  {
    return 0.0;
  }
  QModelIndex index = constantvalues_model->index(c, 0, QModelIndex());
  return QString_to_double((constantvalues_model->data(index, Qt::EditRole)).toString());
}

void ConstantsWidget::reset()
{
  n_constants_SB->setValue(0);
  for(int row = 0; row < max_constants; ++row)
  {
    QModelIndex index = constantnames_model->index(row, 0, QModelIndex());
    constantnames_model->setData(index, QVariant(QString("")), Qt::EditRole);
  }
}

void ConstantsWidget::modified_slot()
{
  constantnames_tableView->clearSelection();
  constantvalues_tableView->clearSelection();
  emit modified();
}

void ConstantsWidget::constants_changed_slot(const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
  QModelIndex index;
  for(int row=topLeft.row(); row<=bottomRight.row(); ++row)
  {
    index=constantnames_model->index(row, 0, QModelIndex());
    QString new_name=(constantnames_model->data(index, Qt::EditRole)).toString();
    emit constant_changed(row, new_name.toStdString());
  }
}

void ConstantsWidget::updateSizes()
{
  int scrollBarWidth=0;
  if(constantvalues_tableView->verticalScrollBar()->isVisible())
  {
    scrollBarWidth=constantvalues_tableView->verticalScrollBar()->width();
  }
  constantnames_tableView->setColumnWidth(0, constantnames_tableView->width()-constantnames_tableView->verticalHeader()->width()-table_padding);
  constantvalues_tableView->setColumnWidth(0, constantvalues_tableView->width()-scrollBarWidth-table_padding);
}

void ConstantsWidget::resizeEvent(QResizeEvent* event)
{
  updateSizes();
  QWidget::resizeEvent(event);
}


void ConstantsWidget::n_constants_modified_slot(int new_n_constants)
{
  if(new_n_constants>n_consts)
  {
    for(int c=n_consts; c<=new_n_constants-1; ++c)
    {
      constantnames_tableView->setRowHidden(c, false);
      constantvalues_tableView->setRowHidden(c, false);
    }
  }
  else if(new_n_constants<n_consts)
  {
    for(int c=n_consts-1; c>new_n_constants-1; --c)
    {
      constantnames_tableView->setRowHidden(c, true);
      constantvalues_tableView->setRowHidden(c, true);
    }
  }
  n_consts=new_n_constants;
  emit n_constants_modified(new_n_constants);
  updateSizes();
}

void ConstantsWidget::scrollbar_moved(int newpos)
{
  (constantnames_tableView->verticalScrollBar())->setSliderPosition(newpos);
  (constantvalues_tableView->verticalScrollBar())->setSliderPosition(newpos);
}


#ifdef Q_WS_MACX
void ConstantsWidget::constantnames_key_pressed(QKeyEvent* key_event)
{
// fix up/down navigation for Mac OS X
  if(key_event->key()==Qt::Key_Down)
  {
    int row=(constantnames_tableView->currentIndex()).row();
    int column=(constantnames_tableView->currentIndex()).column();
    if( row < n_consts-1 )
    {
      constantnames_tableView->setCurrentIndex(constantnames_model->index(row+1, column, QModelIndex()));
    }
  }
  if(key_event->key()==Qt::Key_Up)
  {
    int row=(constantnames_tableView->currentIndex()).row();
    int column=(constantnames_tableView->currentIndex()).column();
    if( row > 0 )
    {
      constantnames_tableView->setCurrentIndex(constantnames_model->index(row-1, column, QModelIndex()));
    }
  }
}
#endif


#ifdef Q_WS_MACX
void ConstantsWidget::constantvalues_key_pressed(QKeyEvent* key_event)
{
// fix up/down navigation for Mac OS X
  if(key_event->key()==Qt::Key_Down)
  {
    int row=(constantvalues_tableView->currentIndex()).row();
    int column=(constantvalues_tableView->currentIndex()).column();
    if( row < n_consts-1 )
    {
      constantvalues_tableView->setCurrentIndex(constantvalues_model->index(row+1, column, QModelIndex()));
    }
  }
  if(key_event->key()==Qt::Key_Up)
  {
    int row=(constantvalues_tableView->currentIndex()).row();
    int column=(constantvalues_tableView->currentIndex()).column();
    if( row > 0 )
    {
      constantvalues_tableView->setCurrentIndex(constantvalues_model->index(row-1, column, QModelIndex()));
    }
  }
}
#endif

void ConstantsWidget::createGadgets()
{
  vlayout=new QVBoxLayout;

  hlayout1=new QHBoxLayout;

  n_constants_SB=new QSpinBox();
  n_constants_SB->setMinimumWidth(60);
  n_constants_SB->setMinimum(0);
  n_constants_SB->setValue(0);
  n_constants_SB->setMaximum(max_constants);
  connect(n_constants_SB, SIGNAL(valueChanged(int)), this, SLOT(modified_slot()));
  connect(n_constants_SB, SIGNAL(valueChanged(int)), this, SLOT(n_constants_modified_slot(int)));
  n_constants_Lb=new QLabel("Number of constants:");

  hlayout1->addWidget(n_constants_Lb, Qt::AlignLeft);
  hlayout1->addWidget(n_constants_SB, 1, Qt::AlignRight);

  hlayout2=new QHBoxLayout;
  hlayout2->setSpacing(0);

  constantnames_tableView=new QTableView();
  constantnames_tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  constantnames_model=new QStandardItemModel(max_constants, 1);
  constantnames_model->setHeaderData(0, Qt::Horizontal, "Name");
  constantnames_tableView->setModel(constantnames_model);
  constantnames_delegate = new LineEditDelegate(false, false);
  constantnames_tableView->setItemDelegate(constantnames_delegate);
  for(int row = 0; row < max_constants; ++row)
  {
    QModelIndex index = constantnames_model->index(row, 0, QModelIndex());
    constantnames_model->setData(index, QVariant(QString("")), Qt::EditRole);
  }
  constantnames_tableView->setEditTriggers(QAbstractItemView::AllEditTriggers);
  for(int c=n_consts; c<max_constants; ++c)
  {
    constantnames_tableView->setRowHidden(c, true);
  }
  connect(constantnames_tableView->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(scrollbar_moved(int)));
  connect(constantnames_tableView->model(), SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(modified_slot()));
  connect(constantnames_tableView->model(), SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(constants_changed_slot(const QModelIndex&, const QModelIndex&)));

#ifdef Q_WS_MACX
  connect(constantnames_delegate, SIGNAL(key_pressed(QKeyEvent*)), this, SLOT(constantnames_key_pressed(QKeyEvent*)));
#endif

  constantvalues_tableView=new QTableView();
  constantvalues_tableView->verticalHeader()->hide();
  constantvalues_model=new QStandardItemModel(max_constants, 1);
  constantvalues_model->setHeaderData(0, Qt::Horizontal, "Value");
  constantvalues_tableView->setModel(constantvalues_model);
  constantvalues_delegate = new LineEditDelegate(false, true);
  constantvalues_tableView->setItemDelegate(constantvalues_delegate);
  for(int row = 0; row < max_constants; ++row)
  {
    QModelIndex index = constantvalues_model->index(row, 0, QModelIndex());
    constantvalues_model->setData(index, QVariant(QString("")), Qt::EditRole);
  }
  constantvalues_tableView->setEditTriggers(QAbstractItemView::AllEditTriggers);
  for(int c=n_consts; c<max_constants; ++c)
  {
    constantvalues_tableView->setRowHidden(c, true);
  }
  connect(constantvalues_tableView->model(), SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(modified_slot()));
  connect(constantvalues_tableView->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(scrollbar_moved(int)));

#ifdef Q_WS_MACX
  connect(constantvalues_delegate, SIGNAL(key_pressed(QKeyEvent*)), this, SLOT(constantvalues_key_pressed(QKeyEvent*)));
#endif

  hlayout2->addWidget(constantnames_tableView);
  hlayout2->addWidget(constantvalues_tableView);

  vlayout->addLayout(hlayout1);
  vlayout->addLayout(hlayout2);

  setLayout(vlayout);
}
