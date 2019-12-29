#include "functionwidget.h"

FunctionWidget::FunctionWidget(int n_functions, int max_n_functions, QWidget* parent)
  : QWidget(parent)
{
  max_functions=max_n_functions;
  n_funcs=n_functions;
  createGadgets();
}

void FunctionWidget::set_user_def(bool user_def)
{
  n_functions_Lb->setEnabled(user_def);
  n_functions_SB->setEnabled(user_def);
  functions_tableView->setEnabled(user_def);
}

bool FunctionWidget::set_n_functions(int n_functions)
{
  if(n_functions!=n_funcs)
  {
    if(n_functions>max_functions)
    {
      QMessageBox::warning(this, tr("QMBF"),
                                 tr("Error: too many model functions"));
      return false;
    }
    n_functions_SB->setValue(n_functions);
  }
  return true;
}

int FunctionWidget::get_n_functions()
{
  return n_funcs;
}

void FunctionWidget::set_function(int f, std::string name)
{
  if(f>n_funcs-1)
  {
    set_n_functions(f+1);
  }
  QModelIndex index = functions_model->index(f, 0, QModelIndex());
  functions_model->setData(index, QVariant(QString(name.c_str())), Qt::EditRole);
}

std::string FunctionWidget::get_function(int f)
{
  if(f>n_funcs-1)
  {
    return std::string("");
  }
  QModelIndex index = functions_model->index(f, 0, QModelIndex());
  QString text=(functions_model->data(index, Qt::EditRole)).toString();
  return text.toStdString();
}

void FunctionWidget::reset()
{
  n_functions_SB->setValue(0);
  for(int row = 0; row < max_functions; ++row)
  {
    QModelIndex index = functions_model->index(row, 0, QModelIndex());
    functions_model->setData(index, QVariant(QString("")), Qt::EditRole);
  }
}

void FunctionWidget::modified_slot()
{
  functions_tableView->clearSelection();
  emit modified();
}

void FunctionWidget::updateSizes()
{
  int scrollBarWidth=0;
  if(functions_tableView->verticalScrollBar()->isVisible())
  {
    scrollBarWidth=functions_tableView->verticalScrollBar()->width();
  }
  functions_tableView->setColumnWidth(0, functions_tableView->width()-functions_tableView->verticalHeader()->width()-scrollBarWidth-table_padding);
}


void FunctionWidget::resizeEvent(QResizeEvent* event)
{
  updateSizes();
  QWidget::resizeEvent(event);
}


void FunctionWidget::n_functions_modified_slot(int new_n_functions)
{
  if(new_n_functions>n_funcs)
  {
    for(int f=n_funcs; f<=new_n_functions-1; ++f)
    {
      functions_tableView->setRowHidden(f, false);
    }
  }
  else if(new_n_functions<n_funcs)
  {
    for(int f=n_funcs-1; f>new_n_functions-1; --f)
    {
      functions_tableView->setRowHidden(f, true);
    }
  }
  n_funcs=new_n_functions;
  updateSizes();
  emit n_functions_modified(new_n_functions);
}

void FunctionWidget::functions_changed_slot(const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
  QModelIndex index;
  for(int row=topLeft.row(); row<=bottomRight.row(); ++row)
  {
    index=functions_model->index(row, 0, QModelIndex());
    QString new_name=(functions_model->data(index, Qt::EditRole)).toString();
    emit function_changed(row, new_name.toStdString());
  }
}


#ifdef Q_WS_MACX
void FunctionWidget::functions_key_pressed(QKeyEvent* key_event)
{
// fix up/down navigation for Mac OS X
  if(key_event->key()==Qt::Key_Down)
  {
    int row=(functions_tableView->currentIndex()).row();
    int column=(functions_tableView->currentIndex()).column();
    if( row < n_funcs-1 )
    {
      functions_tableView->setCurrentIndex(functions_model->index(row+1, column, QModelIndex()));
    }
  }
  if(key_event->key()==Qt::Key_Up)
  {
    int row=(functions_tableView->currentIndex()).row();
    int column=(functions_tableView->currentIndex()).column();
    if( row > 0 )
    {
      functions_tableView->setCurrentIndex(functions_model->index(row-1, column, QModelIndex()));
    }
  }
}
#endif


void FunctionWidget::createGadgets()
{
  vlayout=new QVBoxLayout;

  hlayout=new QHBoxLayout;

  n_functions_SB=new QSpinBox();
  n_functions_SB->setMinimumWidth(60);
  n_functions_SB->setMinimum(1);
  n_functions_SB->setValue(n_funcs);
  n_functions_SB->setMaximum(max_functions);
  connect(n_functions_SB, SIGNAL(valueChanged(int)), this, SLOT(modified_slot()));
  connect(n_functions_SB, SIGNAL(valueChanged(int)), this, SLOT(n_functions_modified_slot(int)));
  n_functions_Lb=new QLabel("Number of model functions:");

  hlayout->addWidget(n_functions_Lb, Qt::AlignLeft);
  hlayout->addWidget(n_functions_SB, 1, Qt::AlignRight);

  functions_tableView=new QTableView();
  functions_model=new QStandardItemModel(max_functions, 1);
  functions_model->setHeaderData(0, Qt::Horizontal, "Function");
  functions_tableView->setModel(functions_model);
  functions_delegate = new LineEditDelegate(false, false);
  functions_tableView->setItemDelegate(functions_delegate);
  for(int row = 0; row < max_functions; ++row)
  {
    QModelIndex index = functions_model->index(row, 0, QModelIndex());
    functions_model->setData(index, QVariant(QString("")), Qt::EditRole);
  }
  functions_tableView->setEditTriggers(QAbstractItemView::AllEditTriggers);
  for(int f=n_funcs; f<max_functions; ++f)
  {
    functions_tableView->setRowHidden(f, true);
  }
  connect(functions_tableView->model(), SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(modified_slot()));
  connect(functions_tableView->model(), SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(functions_changed_slot(const QModelIndex&, const QModelIndex&)));

#ifdef Q_WS_MACX
  connect(functions_delegate, SIGNAL(key_pressed(QKeyEvent*)), this, SLOT(functions_key_pressed(QKeyEvent*)));
#endif

  vlayout->addLayout(hlayout);
  vlayout->addWidget(functions_tableView);

  setLayout(vlayout);
}
